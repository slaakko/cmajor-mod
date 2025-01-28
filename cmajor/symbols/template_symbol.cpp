// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.templates;

import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.exception;
import util;

namespace cmajor::symbols {

TemplateParameterSymbol::TemplateParameterSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    TypeSymbol(SymbolType::templateParameterSymbol, span_, name_), hasDefault(false), defaultType(nullptr)
{
}

void TemplateParameterSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(hasDefault);
    if (hasDefault)
    {
        util::uuid defaultTypeId = util::nil_uuid();
        if (defaultType != nullptr)
        {
            defaultTypeId = defaultType->TypeId();
        }
        writer.GetBinaryStreamWriter().Write(defaultTypeId);
    }
}

void TemplateParameterSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    hasDefault = reader.GetBinaryStreamReader().ReadBool();
    if (hasDefault)
    {
        util::uuid defaultTypeId;
        reader.GetBinaryStreamReader().ReadUuid(defaultTypeId);
        if (!defaultTypeId.is_nil())
        {
            reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, defaultTypeId, 0);
        }
    }
}

void TemplateParameterSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    Assert(index == 0, "invalid emplace type index"); 
    defaultType = typeSymbol;
}

TypeSymbol* TemplateParameterSymbol::Unify(TypeSymbol* type)
{
    return type;
}

TypeSymbol* TemplateParameterSymbol::UnifyTemplateArgumentType(SymbolTable& symbolTable, const std::map<TemplateParameterSymbol*, TypeSymbol*>& templateParameterMap)
{
    auto it = templateParameterMap.find(this);
    if (it != templateParameterMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

BoundTemplateParameterSymbol::BoundTemplateParameterSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    Symbol(SymbolType::boundTemplateParameterSymbol, span_, name_), type(nullptr)
{
}

void BoundTemplateParameterSymbol::Write(SymbolWriter& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(type->TypeId());
}

void BoundTemplateParameterSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 0);
}

std::unique_ptr<soul::xml::Element> BoundTemplateParameterSymbol::CreateDomElement(TypeMap& typeMap)
{
    std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement("BoundTemplateParameterSymbol"));
    if (type)
    {
        std::unique_ptr<soul::xml::Element> typeElement(soul::xml::MakeElement("type"));
        int typeId = typeMap.GetOrInsertType(type);
        typeElement->SetAttribute("ref", "type_" + std::to_string(typeId));
        element->AppendChild(typeElement.release());
    }
    return element;
}

void BoundTemplateParameterSymbol::Check()
{
    Symbol::Check();
    if (!type)
    {
        throw SymbolCheckException("bound template parameter symbol contains null type pointer", GetFullSpan());
    }
}

} // namespace cmajor::symbols
