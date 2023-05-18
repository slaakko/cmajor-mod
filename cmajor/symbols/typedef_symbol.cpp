// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.typedefs;

import cmajor.symbols.symbol.table;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.exception;
import cmajor.symbols.type.symbol;
import cmajor.symbols.symbol.collector;
import soul.ast.source.pos;
import util;
import std.core;

namespace cmajor::symbols {

TypedefSymbol::TypedefSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_) :
    Symbol(SymbolType::typedefSymbol, sourcePos_, sourceModuleId_, name_), type()
{
}

void TypedefSymbol::Write(SymbolWriter& writer)
{
    Symbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(type->TypeId());
}

void TypedefSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    util::uuid typeId;
    reader.GetBinaryStreamReader().ReadUuid(typeId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, 0);
}

void TypedefSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    Assert(index == 0, "invalid emplace type index");
    type = typeSymbol;
}

bool TypedefSymbol::IsExportSymbol() const
{
    return Symbol::IsExportSymbol();
}

void TypedefSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject() && Access() == SymbolAccess::public_)
    {
        collector->AddTypedef(this);
    }
}

void TypedefSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    formatter.WriteLine("mangled name: " + util::ToUtf8(MangledName()));
    formatter.WriteLine("type: " + util::ToUtf8(type->FullName()));
}

std::string TypedefSymbol::Syntax() const
{
    std::string syntax = GetSpecifierStr();
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    syntax.append("typedef ");
    syntax.append(util::ToUtf8(GetType()->DocName()));
    syntax.append(1, ' ');
    syntax.append(util::ToUtf8(DocName()));
    syntax.append(1, ';');
    return syntax;
}

void TypedefSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be static", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be virtual", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be override", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be abstract", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be inline", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be explicit", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be external", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be suppressed", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be default", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be constexpr", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be cdecl", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be nothrow", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be throw", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be new", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be const", GetSourcePos(), SourceModuleId());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("typedef cannot be unit_test", GetSourcePos(), SourceModuleId());
    }
}
/*TODO
std::unique_ptr<soul::xml::Element> TypedefSymbol::CreateDomElement(TypeMap& typeMap)
{
    std::unique_ptr<soul::xml::Element> element(new soul::xml::Element(U"TypedefSymbol"));
    if (type)
    {
        std::unique_ptr<soul::xml::Element> typeElement(new soul::xml::Element(U"type"));
        int typeId = typeMap.GetOrInsertType(type);
        typeElement->SetAttribute(U"ref", U"type_" + ToUtf32(util::ToString(typeId)));
        element->AppendChild(std::unique_ptr<soul::xml::Node>(typeElement.release()));
    }
    return element;
}
*/
void TypedefSymbol::Check()
{
    Symbol::Check();
    if (!type)
    {
        throw SymbolCheckException("typedef symbol contains null type pointer", GetSourcePos(), SourceModuleId());
    }
}

std::string TypedefSymbol::GetSymbolHelp() const
{
    if (!type) return std::string();
    std::string help = Symbol::GetSymbolHelp();
    help.append(" = ").append(util::ToUtf8(type->FullName()));
    return help;
}
} // namespace cmajor::symbols
