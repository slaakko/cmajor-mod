// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.concepts;

import cmajor.symbols.type.map;
import cmajor.symbols.exception;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.templates;
import cmajor.symbols.modules;
import cmajor.symbols.symbol.table;
import cmajor.symbols.symbol.collector;
import cmajor.ast.parameter;
import util;

namespace cmajor::symbols {

ConceptGroupSymbol::ConceptGroupSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) : 
    Symbol(SymbolType::conceptGroupSymbol, sourcePos_, sourceModuleId_, name_)
{
}

void ConceptGroupSymbol::AddConcept(ConceptSymbol* conceptSymbol)
{
    Assert(conceptSymbol->GroupName() == Name(), "wrong concept group"); 
    int arity = conceptSymbol->Arity();
    auto it = arityConceptMap.find(arity);
    if (it != arityConceptMap.cend())
    {
        throw Exception("concept group '" + util::ToUtf8(FullName()) + "' already has concept with arity " + std::to_string(arity), GetSourcePos(), SourceModuleId());
    }
    arityConceptMap[arity] = conceptSymbol;
    conceptSymbol->SetConceptGroup(this);
}

void ConceptGroupSymbol::RemoveConcept(ConceptSymbol* conceptSymbol)
{
    int arity = conceptSymbol->Arity();
    arityConceptMap.erase(arity);
}

bool ConceptGroupSymbol::IsEmpty() const
{
    return arityConceptMap.empty();
}

ConceptSymbol* ConceptGroupSymbol::GetConcept(int arity)
{
    auto it = arityConceptMap.find(arity);
    if (it != arityConceptMap.cend())
    {
        return it->second;
    }
    else
    {
        throw Exception("concept with arity " + std::to_string(arity) + " not found from concept group '" + util::ToUtf8(FullName()) + "'", GetSourcePos(), SourceModuleId());
    }
}

bool ConceptGroupSymbol::HasProjectMembers() const
{
    for (const auto& p : arityConceptMap)
    {
        ConceptSymbol* conceptSymbol = p.second;
        if (conceptSymbol->IsProject())
        {
            return true;
        }
    }
    return false;
}

void ConceptGroupSymbol::AppendChildElements(soul::xml::Element* element, TypeMap& typeMap) const
{
    for (const auto& p : arityConceptMap)
    {
        ConceptSymbol* conceptSymbol = p.second;
        if (conceptSymbol->IsProject())
        {
            std::unique_ptr<soul::xml::Element> conceptElement = conceptSymbol->ToDomElement(typeMap);
            element->AppendChild(conceptElement.release());
        }
    }
}

void ConceptGroupSymbol::Check()
{
    Symbol::Check();
    for (const auto& p : arityConceptMap)
    {
        if (!p.second)
        {
            throw SymbolCheckException("concept group symbol has no concept symbol", GetSourcePos(), SourceModuleId());
        }
    }
}

std::string ConceptGroupSymbol::GetSymbolHelp() const
{
    if (arityConceptMap.size() == 1)
    {
        ConceptSymbol* cs = arityConceptMap.begin()->second;
        return cs->GetSymbolHelp();
    }
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(FullName())).append(" (").append(std::to_string(arityConceptMap.size())).append(" concepts)");
    return help;
}

std::vector<Symbol*> ConceptGroupSymbol::GetParamHelpSymbols() const
{
    std::vector<Symbol*> paramHelpSymbols;
    for (const auto& p : arityConceptMap)
    {
        ConceptSymbol* cs = p.second;
        paramHelpSymbols.push_back(cs);
    }
    return paramHelpSymbols;
}

ConceptSymbol::ConceptSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    ContainerSymbol(SymbolType::conceptSymbol, sourcePos_, sourceModuleId_, name_), refinedConcept(nullptr), typeId(util::nil_uuid()), hasSource(false), conceptGroup(nullptr)
{
}

void ConceptSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    Assert(!typeId.is_nil(), "type id not initialized"); 
    writer.GetBinaryStreamWriter().Write(typeId);
    writer.GetBinaryStreamWriter().Write(groupName);
    util::uuid refineConceptId = util::nil_uuid();
    if (refinedConcept)
    {
        refineConceptId = refinedConcept->TypeId();
    }
    writer.GetBinaryStreamWriter().Write(refineConceptId);
    uint32_t n = templateParameters.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(n);
    for (uint32_t i = 0; i < n; ++i)
    {
        TemplateParameterSymbol* templateParameter = templateParameters[i];
        Assert(!templateParameter->TypeId().is_nil(), "type id not initialized"); 
        writer.GetBinaryStreamWriter().Write(templateParameter->TypeId());
    }
    cmajor::ast::Node* node = GetRootModuleForCurrentThread()->GetSymbolTable().GetNode(this);
    Assert(node->IsConceptNode(), "concept node expected"); 
    writer.GetAstWriter().Write(node);
    writer.GetBinaryStreamWriter().Write(hasSource);
}

void ConceptSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->AddTypeOrConceptSymbolToTypeIdMap(this);
    groupName = reader.GetBinaryStreamReader().ReadUtf32String();
    util::uuid refinedConcepId;
    reader.GetBinaryStreamReader().ReadUuid(refinedConcepId);
    if (!refinedConcepId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceConceptRequest(reader, this, refinedConcepId);
    }
    uint32_t n = reader.GetBinaryStreamReader().ReadULEB128UInt();
    templateParameters.resize(n);
    for (uint32_t i = 0; i < n; ++i)
    {
        util::uuid templateParameterId;
        reader.GetBinaryStreamReader().ReadUuid(templateParameterId);
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, templateParameterId, i);
    }
    conceptNode.reset(reader.GetAstReader().ReadConceptNode());
    hasSource = reader.GetBinaryStreamReader().ReadBool();
}

void ConceptSymbol::EmplaceConcept(ConceptSymbol* conceptSymbol)
{
    refinedConcept = conceptSymbol;
}

void ConceptSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index >= 0 && index < templateParameters.size())
    {
        if (typeSymbol->GetSymbolType() == SymbolType::templateParameterSymbol)
        {
            TemplateParameterSymbol* templateParameter = static_cast<TemplateParameterSymbol*>(typeSymbol);
            templateParameters[index] = templateParameter;
        }
        else
        {
            throw Exception("invalid emplace type", GetSourcePos(), SourceModuleId());
        }
    }
    else
    {
        throw Exception("invalid emplace type index", GetSourcePos(), SourceModuleId());
    }
}

void ConceptSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddConcept(this);
    }
}

void ConceptSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("group name: " + util::ToUtf8(groupName));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    formatter.WriteLine("mangled name: " + util::ToUtf8(MangledName()));
    formatter.WriteLine("typeid: " + util::ToString(typeId));
    if (refinedConcept)
    {
        formatter.WriteLine("refined concept: " + util::ToUtf8(refinedConcept->FullName()));
    }
}

void ConceptSymbol::AddMember(Symbol* member)
{
    ContainerSymbol::AddMember(member);
    if (member->GetSymbolType() == SymbolType::templateParameterSymbol)
    {
        templateParameters.push_back(static_cast<TemplateParameterSymbol*>(member));
    }
}

void ConceptSymbol::ComputeName()
{
    std::u32string name = groupName;
    bool first = true;
    name.append(1, '<');
    for (TemplateParameterSymbol* templateParameter : templateParameters)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            name.append(U", ");
        }
        name.append(templateParameter->Name());
    }
    name.append(1, '>');
    SetName(name);
    ComputeMangledName();
}

void ConceptSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be static", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be inline", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be default", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be constexpr", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be decl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be nothrow", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be throw", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("concept symbol cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

void ConceptSymbol::Check()
{
    ContainerSymbol::Check();
    if (typeId.is_nil())
    {
        throw SymbolCheckException("concept symbol has empty type id", GetSourcePos(), SourceModuleId());
    }
    if (groupName.empty())
    {
        throw SymbolCheckException("concept symbol has empty group name", GetSourcePos(), SourceModuleId());
    }
    for (TemplateParameterSymbol* templateParameter : templateParameters)
    {
        if (!templateParameter)
        {
            throw SymbolCheckException("concept symbol has no template parameter", GetSourcePos(), SourceModuleId());
        }
    }
}

std::unique_ptr<Symbol> ConceptSymbol::RemoveFromParent()
{
    std::unique_ptr<Symbol> symbol = ContainerSymbol::RemoveFromParent();
    if (conceptGroup)
    {
        conceptGroup->RemoveConcept(this);
        if (conceptGroup->IsEmpty())
        {
            std::unique_ptr<Symbol> conceptGroupSymbol = conceptGroup->RemoveFromParent();
        }
    }
    return symbol;
}

AxiomSymbol::AxiomSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId, const std::u32string& name_) :
    ContainerSymbol(SymbolType::axiomSymbol, sourcePos_, sourceModuleId, name_)
{
}

} // namespace cmajor::symbols
