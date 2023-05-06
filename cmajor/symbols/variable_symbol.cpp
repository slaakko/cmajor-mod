module cmajor.symbols.variable.symbol;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================
/*
#include <cmajor/symbols/VariableSymbol.hpp>
#include <cmajor/symbols/TypeSymbol.hpp>
#include <cmajor/symbols/SymbolTable.hpp>
#include <cmajor/symbols/SymbolWriter.hpp>
#include <cmajor/symbols/SymbolReader.hpp>
#include <cmajor/symbols/Exception.hpp>
#include <cmajor/symbols/SymbolCollector.hpp>
#include <cmajor/symbols/Module.hpp>
#include <soulng/util/Unicode.hpp>
#include <soulng/util/Sha1.hpp>
#include <algorithm>
*/
import soul.ast.source.pos;
import cmajor.symbols.exception;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.class_.type.symbol; 
import cmajor.symbols.symbol.collector;
import cmajor.ast.specifier;
import cmajor.ir.emitter;
import util;
import std.core;

namespace cmajor::symbols {

//using namespace soulng::unicode;

VariableSymbol::VariableSymbol(SymbolType symbolType_, const soul::ast::SourcePos&  sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    Symbol(symbolType_, sourcePos_, sourceModuleId_, name_), type()
{
}

void VariableSymbol::Write(SymbolWriter& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(type->TypeId());
}

void VariableSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 0);
}

void VariableSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    //Assert(index == 0, "invalid emplace type index");
    type = typeSymbol;
}

const ContainerScope* VariableSymbol::GetTypeScope() const
{
    if (type)
    {
        return type->BaseType()->GetContainerScope();
    }
    else
    {
        return nullptr;
    }
}

ContainerScope* VariableSymbol::GetTypeScope()
{
    if (type)
    {
        return type->BaseType()->GetContainerScope();
    }
    else
    {
        return nullptr;
    }
}

void VariableSymbol::Check()
{
    Symbol::Check();
    if (!type)
    {
        throw SymbolCheckException("variable symbol contains null type pointer", GetSourcePos(), SourceModuleId());
    }
}

std::string VariableSymbol::GetSymbolHelp() const
{
    if (!type) return std::string();
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(type->FullName())).append(" ");
    help.append(util::ToUtf8(FullName()));
    return help;
}

ParameterSymbol::ParameterSymbol(const soul::ast::SourcePos&  sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    VariableSymbol(SymbolType::parameterSymbol, sourcePos_, sourceModuleId_, name_), artificialName(false)
{
}

void ParameterSymbol::Write(SymbolWriter& writer)
{
    VariableSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(artificialName);
}

void ParameterSymbol::Read(SymbolReader& reader)
{
    VariableSymbol::Read(reader);
    artificialName = reader.GetBinaryStreamReader().ReadBool();
}

bool ParameterSymbol::IsExportSymbol() const
{
    return VariableSymbol::IsExportSymbol();
}

std::u32string ParameterSymbol::CodeName() const
{
    if (artificialName)
    {
        return std::u32string();
    }
    return VariableSymbol::CodeName();
}
/* TODO
std::unique_ptr<soul::xml::Element> ParameterSymbol::CreateDomElement(TypeMap& typeMap)
{
    std::unique_ptr<soul::xml::Element> element(new soul::xml::Element(U"ParameterSymbol"));
    if (GetType())
    {
        std::unique_ptr<soul::xml::Element> typeElement(new soul::xml::Element(U"type"));
        int typeId = typeMap.GetOrInsertType(GetType());
        typeElement->SetAttribute(U"ref", U"type_" + util::ToUtf32(util::ToString(typeId)));
        element->AppendChild(std::unique_ptr<soul::xml::Node>(typeElement.release()));
    }
    return element;
}
*/
ParameterSymbol* ParameterSymbol::Clone() const
{
    ParameterSymbol* clone = new ParameterSymbol(GetSourcePos(), SourceModuleId(), Name());
    clone->SetType(const_cast<TypeSymbol*>(GetType()));
    clone->artificialName = artificialName;
    return clone;
}

std::string ParameterSymbol::GetSymbolHelp() const
{
    if (!GetType()) return std::string();
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(GetType()->FullName())).append(" ");
    help.append(util::ToUtf8(Name()));
    return help;
}

LocalVariableSymbol::LocalVariableSymbol(const soul::ast::SourcePos&  sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    VariableSymbol(SymbolType::localVariableSymbol, sourcePos_, sourceModuleId_, name_)
{
}
/*TODO
std::unique_ptr<soul::xml::Element> LocalVariableSymbol::CreateDomElement(TypeMap& typeMap)
{
    std::unique_ptr<soul::xml::Element> element(new soul::xml::Element(U"LocalVariableSymbol"));
    if (GetType())
    {
        std::unique_ptr<soul::xml::Element> typeElement(new soul::xml::Element(U"type"));
        int typeId = typeMap.GetOrInsertType(GetType());
        typeElement->SetAttribute(U"ref", U"type_" + util::ToUtf32(util::ToString(typeId)));
        element->AppendChild(std::unique_ptr<soul::xml::Node>(typeElement.release()));
    }
    return element;
}
*/
std::string LocalVariableSymbol::GetSymbolHelp() const
{
    if (!GetType()) return std::string();
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(GetType()->FullName())).append(" ");
    help.append(util::ToUtf8(Name()));
    return help;
}

MemberVariableSymbol::MemberVariableSymbol(const soul::ast::SourcePos&  sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    VariableSymbol(SymbolType::memberVariableSymbol, sourcePos_, sourceModuleId_, name_), layoutIndex(-1)
{
}

void MemberVariableSymbol::Write(SymbolWriter& writer)
{
    VariableSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(layoutIndex);
}

void MemberVariableSymbol::Read(SymbolReader& reader)
{
    VariableSymbol::Read(reader);
    layoutIndex = reader.GetBinaryStreamReader().ReadInt();
}

bool MemberVariableSymbol::IsExportSymbol() const
{
    return VariableSymbol::IsExportSymbol();
}

void MemberVariableSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddMemberVariable(this);
    }
}

void MemberVariableSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    formatter.WriteLine("mangled name: " + util::ToUtf8(MangledName()));
    formatter.WriteLine("type: " + util::ToUtf8(GetType()->FullName()));
    formatter.WriteLine("layout index: " + util::ToString(layoutIndex));
}

std::string MemberVariableSymbol::Syntax() const
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    syntax.append(util::ToUtf8(GetType()->DocName()));
    syntax.append(1, ' ');
    syntax.append(util::ToUtf8(DocName()));
    syntax.append(1, ';');
    return syntax;
}

void MemberVariableSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        SetStatic();
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be inline", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be default", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be constexpr", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be nothrow", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be throw", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("member variable cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

void* MemberVariableSymbol::GetDIMemberType(cmajor::ir::Emitter& emitter, uint64_t offsetInBits)
{
    //Assert(layoutIndex != -1, "invalid layout index");
    //Assert(Parent() && Parent()->IsClassTypeSymbol(), "parent class type expected");
    ClassTypeSymbol* parentClassType = static_cast<ClassTypeSymbol*>(Parent());
    std::pair<util::uuid, int32_t> memberVariableId = std::make_pair(parentClassType->TypeId(), layoutIndex);
    void* localDIType = emitter.GetDIMemberType(memberVariableId);
    if (!localDIType)
    {
        uint64_t sizeInBits = GetType()->SizeInBits(emitter);
        uint32_t alignInBits = GetType()->AlignmentInBits(emitter);
        void* scope = parentClassType->GetDIType(emitter);
        localDIType = emitter.CreateDIMemberType(scope, util::ToUtf8(Name()), GetSourcePos(), SourceModuleId(), sizeInBits, alignInBits, offsetInBits, GetType()->GetDIType(emitter));
        emitter.SetDIMemberType(memberVariableId, localDIType);
    }
    return localDIType;
}
/*TODO
std::unique_ptr<soul::xml::Element> MemberVariableSymbol::CreateDomElement(TypeMap& typeMap)
{
    std::unique_ptr<soul::xml::Element> element(new soul::xml::Element(U"MemberVariableSymbol"));
    if (GetType())
    {
        std::unique_ptr<soul::xml::Element> typeElement(new soul::xml::Element(U"type"));
        int typeId = typeMap.GetOrInsertType(GetType());
        typeElement->SetAttribute(U"ref", U"type_" + util::ToUtf32(util::ToString(typeId)));
        element->AppendChild(std::unique_ptr<soul::xml::Node>(typeElement.release()));
    }
    return element;
}
*/
void MemberVariableSymbol::Check()
{
    VariableSymbol::Check();
    if (layoutIndex == -1)
    {
        throw SymbolCheckException("member variable symbol contains invalid layout index", GetSourcePos(), SourceModuleId());
    }
}

GlobalVariableGroupSymbol::GlobalVariableGroupSymbol(const soul::ast::SourcePos&  sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    Symbol(SymbolType::globalVariableGroupSymbol, sourcePos_, sourceModuleId_, name_)
{
}

void GlobalVariableGroupSymbol::ComputeMangledName()
{
    std::u32string mangledName = util::ToUtf32(TypeString());
    mangledName.append(1, U'_').append(util::ToUtf32(util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()))));
    SetMangledName(mangledName);
}

void GlobalVariableGroupSymbol::AddGlobalVariable(GlobalVariableSymbol* globalVariableSymbol)
{
    std::pair<GlobalVariableSymbol*, std::string> key(globalVariableSymbol, globalVariableSymbol->CompileUnitFilePath());
    if (globalVariableSymbols.empty())
    {
        globalVariableSymbols.push_back(key);
        globalVariableSymbol->SetGlobalVariableGroup(this);
    }
    else
    {
        for (const std::pair<GlobalVariableSymbol*, std::string>& p : globalVariableSymbols)
        {
            if (p.first->Access() == SymbolAccess::internal_ || p.first->Access() == SymbolAccess::public_)
            {
                if (globalVariableSymbol->Access() == SymbolAccess::internal_ || globalVariableSymbol->Access() == SymbolAccess::public_)
                {
                    throw Exception("global variable group '" + util::ToUtf8(Name()) +
                        "' already has public or internal global variable with the given name defined in the source file " + p.second,
                        globalVariableSymbol->GetSourcePos(), globalVariableSymbol->SourceModuleId(), GetSourcePos(), SourceModuleId());
                }
            }
            else
            {
                if (p.second == globalVariableSymbol->CompileUnitFilePath())
                {
                    throw Exception("global variable group '" + util::ToUtf8(Name()) + "' already has global variable with the given name and compile unit",
                        globalVariableSymbol->GetSourcePos(), globalVariableSymbol->SourceModuleId(), GetSourcePos(), SourceModuleId());
                }
            }
        }
        std::vector<std::pair<GlobalVariableSymbol*, std::string>>::const_iterator it = std::find(globalVariableSymbols.cbegin(), globalVariableSymbols.cend(), key);
        if (it == globalVariableSymbols.cend())
        {
            globalVariableSymbols.push_back(key);
            globalVariableSymbol->SetGlobalVariableGroup(this);
        }
        else
        {
            throw Exception("global variable group '" + util::ToUtf8(Name()) + "' already has global variable with the given name and compile unit",
                globalVariableSymbol->GetSourcePos(), globalVariableSymbol->SourceModuleId(), GetSourcePos(), SourceModuleId());
        }
    }
}

void GlobalVariableGroupSymbol::RemoveGlobalVariable(GlobalVariableSymbol* globalVariableSymbol)
{
    std::pair<GlobalVariableSymbol*, std::string> key(globalVariableSymbol, globalVariableSymbol->CompileUnitFilePath());
    auto end = std::remove(globalVariableSymbols.begin(), globalVariableSymbols.end(), key);
    globalVariableSymbols.erase(end, globalVariableSymbols.end());
}

bool GlobalVariableGroupSymbol::IsEmpty() const
{
    return globalVariableSymbols.empty();
}

void GlobalVariableGroupSymbol::CollectGlobalVariables(const std::string& compileUnitFilePath, std::vector<GlobalVariableSymbol*>& globalVariables) const
{
    for (const std::pair<GlobalVariableSymbol*, std::string>& p : globalVariableSymbols)
    {
        if (p.second == compileUnitFilePath)
        {
            globalVariables.push_back(p.first);
            return;
        }
    }
    for (const std::pair<GlobalVariableSymbol*, std::string>& p : globalVariableSymbols)
    {
        if (p.first->Access() == SymbolAccess::public_ || p.first->Access() == SymbolAccess::internal_)
        {
            globalVariables.push_back(p.first);
            return;
        }
    }
    for (const std::pair<GlobalVariableSymbol*, std::string>& p : globalVariableSymbols)
    {
        globalVariables.push_back(p.first);
    }
}

const ContainerScope* GlobalVariableGroupSymbol::GetTypeScope() const
{
    for (const std::pair<GlobalVariableSymbol*, std::string>& p : globalVariableSymbols)
    {
        return p.first->GetTypeScope();
    }
    return nullptr;
}

ContainerScope* GlobalVariableGroupSymbol::GetTypeScope()
{
    for (const std::pair<GlobalVariableSymbol*, std::string>& p : globalVariableSymbols)
    {
        return p.first->GetTypeScope();
    }
    return nullptr;
}

std::string GlobalVariableGroupSymbol::GetSymbolHelp() const
{
    if (globalVariableSymbols.size() == 1)
    {
        return globalVariableSymbols.front().first->GetSymbolHelp();
    }
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(FullName())).append(" (").append(util::ToString(globalVariableSymbols.size())).append(" global variables)");
    return help;
}

std::u32string MakeGlobalVariableName(const std::u32string& groupName, const std::string& compileUnitId)
{
    std::u32string name = groupName;
    name.append(1, '_').append(util::ToUtf32(compileUnitId));
    return name;
}

GlobalVariableSymbol::GlobalVariableSymbol(const soul::ast::SourcePos&  sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& groupName_, const std::string& compileUnitId, const std::string& compileUnitFilePath_) :
    VariableSymbol(SymbolType::globalVariableSymbol, sourcePos_, sourceModuleId_, MakeGlobalVariableName(groupName_, compileUnitId)), groupName(groupName_), compileUnitFilePath(compileUnitFilePath_),
    globalVariableGroup(nullptr)
{
}

GlobalVariableSymbol::GlobalVariableSymbol(const soul::ast::SourcePos&  sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    VariableSymbol(SymbolType::globalVariableSymbol, sourcePos_, sourceModuleId_, name_), globalVariableGroup(nullptr)
{
}

void GlobalVariableSymbol::Write(SymbolWriter& writer)
{
    VariableSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(groupName);
    writer.GetBinaryStreamWriter().Write(compileUnitFilePath);
    bool hasInitializer = initializer != nullptr;
    bool privateAccess = Access() == SymbolAccess::private_;
    writer.GetBinaryStreamWriter().Write(hasInitializer);
    if (hasInitializer && !privateAccess)
    {
        WriteValue(initializer.get(), writer.GetBinaryStreamWriter());
    }
}

void GlobalVariableSymbol::Read(SymbolReader& reader)
{
    VariableSymbol::Read(reader);
    groupName = reader.GetBinaryStreamReader().ReadUtf32String();
    compileUnitFilePath = reader.GetBinaryStreamReader().ReadUtf8String();
    bool hasInitializer = reader.GetBinaryStreamReader().ReadBool();
    bool privateAccess = Access() == SymbolAccess::private_;
    if (hasInitializer && !privateAccess)
    {
        initializer = ReadValue(reader.GetBinaryStreamReader(), GetSourcePos(), SourceModuleId());
        initializer->SetType(GetType());
    }
}

bool GlobalVariableSymbol::IsExportSymbol() const
{
    return VariableSymbol::IsExportSymbol();
}

void GlobalVariableSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddGlobalVariable(this);
    }
}

void GlobalVariableSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("group name: " + util::ToUtf8(groupName));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    formatter.WriteLine("mangled name: " + util::ToUtf8(MangledName()));
    formatter.WriteLine("type: " + util::ToUtf8(GetType()->FullName()));
}

std::string GlobalVariableSymbol::Syntax() const
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    syntax.append(util::ToUtf8(GetType()->DocName()));
    syntax.append(1, ' ');
    syntax.append(util::ToUtf8(DocName()));
    syntax.append(1, ';');
    return syntax;
}

void GlobalVariableSymbol::ComputeMangledName()
{
    std::u32string mangledName = util::ToUtf32(TypeString());
    mangledName.append(1, U'_').append(Name());
    SetMangledName(mangledName);
}

void GlobalVariableSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be static", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be inline", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be default", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be constexpr", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be nothrow", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be throw", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("global variable cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}

void GlobalVariableSymbol::SetInitializer(std::unique_ptr<Value>&& initializer_)
{
    initializer = std::move(initializer_);
}

void* GlobalVariableSymbol::IrObject(cmajor::ir::Emitter& emitter)
{
    return emitter.GetOrInsertGlobal(util::ToUtf8(MangledName()), GetType()->IrType(emitter));
}

void GlobalVariableSymbol::CreateIrObject(cmajor::ir::Emitter& emitter)
{
    void* irObject = IrObject(emitter);
    void* init = nullptr;
    if (initializer == nullptr)
    {
        init = GetType()->CreateDefaultIrValue(emitter);
    }
    else
    {
        init = initializer->IrValue(emitter);
    }
    emitter.SetInitializer(irObject, init);
}

std::unique_ptr<Symbol> GlobalVariableSymbol::RemoveFromParent()
{
    std::unique_ptr<Symbol> symbol = VariableSymbol::RemoveFromParent();
    if (globalVariableGroup)
    {
        globalVariableGroup->RemoveGlobalVariable(this);
        if (globalVariableGroup->IsEmpty())
        {
            std::unique_ptr<Symbol> globalVariableGroupSymbol = globalVariableGroup->RemoveFromParent();
        }
    }
    return symbol;
}
} // namespace cmajor::symbols
