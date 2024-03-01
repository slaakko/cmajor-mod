// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.alias.type;

import cmajor.symbols.symbol.table;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.exception;
import cmajor.symbols.type.symbol;
import cmajor.symbols.symbol.collector;
import soul.ast.span;
import util;
import std.core;

namespace cmajor::symbols {

AliasTypeSymbol::AliasTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    Symbol(SymbolType::aliasTypeSymbol, span_, name_), type()
{
}

void AliasTypeSymbol::Write(SymbolWriter& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(type->TypeId());
}

void AliasTypeSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 0);
}

void AliasTypeSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    Assert(index == 0, "invalid emplace type index");
    type = typeSymbol;
}

bool AliasTypeSymbol::IsExportSymbol() const
{
    return Symbol::IsExportSymbol();
}

void AliasTypeSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddAliasType(this);
    }
}

void AliasTypeSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    formatter.WriteLine("mangled name: " + util::ToUtf8(MangledName()));
    formatter.WriteLine("type: " + util::ToUtf8(type->FullName()));
}

std::string AliasTypeSymbol::Syntax() const
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    syntax.append("alias type ");
    syntax.append(util::ToUtf8(GetType()->DocName()));
    syntax.append(1, ' ');
    syntax.append(util::ToUtf8(DocName()));
    syntax.append(1, ';');
    return syntax;
}

void AliasTypeSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be static", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be virtual", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be override", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be abstract", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be inline", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be suppressed", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be default", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be constexpr", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be nothrow", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be throw", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be const", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("alias type cannot be unit_test", GetFullSpan());
    }
}

std::unique_ptr<soul::xml::Element> AliasTypeSymbol::CreateDomElement(TypeMap& typeMap)
{
    std::unique_ptr<soul::xml::Element> element(soul::xml::MakeElement("AliasTypeSymbol"));
    if (type)
    {
        std::unique_ptr<soul::xml::Element> typeElement(soul::xml::MakeElement("type"));
        int typeId = typeMap.GetOrInsertType(type);
        typeElement->SetAttribute("ref", "type_" + util::ToString(typeId));
        element->AppendChild(typeElement.release());
    }
    return element;
}

void AliasTypeSymbol::Check()
{
    Symbol::Check();
    if (!type)
    {
        throw SymbolCheckException("alias type symbol contains null type pointer", GetFullSpan());
    }
}

std::string AliasTypeSymbol::GetSymbolHelp() const
{
    if (!type) return std::string();
    std::string help = Symbol::GetSymbolHelp();
    help.append(" = ").append(util::ToUtf8(type->FullName()));
    return help;
}
} // namespace cmajor::symbols
