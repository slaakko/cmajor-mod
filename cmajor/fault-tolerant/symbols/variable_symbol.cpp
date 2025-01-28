// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.variable.symbol;

import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;
import cmajor.fault.tolerant.symbols.symbol.table;
import cmajor.fault.tolerant.symbols.type.symbol;

namespace cmajor::fault::tolerant::symbols {

VariableSymbol::VariableSymbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_) : Symbol(kind_, name_, span_), type(nullptr)
{
}

void VariableSymbol::Write(SymbolWriter& writer)
{
    Symbol::Write(writer);
    if (type)
    {
        writer.BinaryStreamWriter().Write(type->Id());
    }
    else
    {
        writer.BinaryStreamWriter().Write(util::uuid());
    }
}

void VariableSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(typeId);
    if (!typeId.is_nil())
    {
        reader.AddSymbolToBeResolved(this);
    }
}

void VariableSymbol::Resolve(SymbolTable* symbolTable)
{
    if (!typeId.is_nil())
    {
        type = symbolTable->GetType(typeId);
    }
}

ParameterSymbol::ParameterSymbol(const std::u16string& name_, const soul::ast::Span& span_) : VariableSymbol(SymbolKind::parameterSymbol, name_, span_)
{
}

void ParameterSymbol::Write(SymbolWriter& writer)
{
    VariableSymbol::Write(writer);
}

void ParameterSymbol::Read(SymbolReader& reader)
{
    VariableSymbol::Read(reader);
}

LocalVariableSymbol::LocalVariableSymbol(const std::u16string& name_, const soul::ast::Span& span_) : VariableSymbol(SymbolKind::localVariableSymbol, name_, span_)
{
}

void LocalVariableSymbol::Write(SymbolWriter& writer)
{
    VariableSymbol::Write(writer);
}

void LocalVariableSymbol::Read(SymbolReader& reader)
{
    VariableSymbol::Read(reader);
}

MemberVariableSymbol::MemberVariableSymbol(const std::u16string& name_, const soul::ast::Span& span_) : VariableSymbol(SymbolKind::memberVariableSymbol, name_, span_)
{
}

void MemberVariableSymbol::Write(SymbolWriter& writer)
{
    VariableSymbol::Write(writer);
}

void MemberVariableSymbol::Read(SymbolReader& reader)
{
    VariableSymbol::Read(reader);
}

GlobalVariableSymbol::GlobalVariableSymbol(const std::u16string& name_, const soul::ast::Span& span_) : VariableSymbol(SymbolKind::globalVariableSymbol, name_, span_)
{
}

void GlobalVariableSymbol::Write(SymbolWriter& writer)
{
    VariableSymbol::Write(writer);
}

void GlobalVariableSymbol::Read(SymbolReader& reader)
{
    VariableSymbol::Read(reader);
}

} // namespace cmajor::fault::tolerant::symbols
