// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.enum_symbol;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;
import cmajor.fault.tolerant.symbols.symbol.table;

namespace cmajor::fault::tolerant::symbols {

EnumTypeSymbol::EnumTypeSymbol(const std::u16string& name_, const soul::ast::Span& span_) : 
    TypeSymbol(SymbolKind::enumTypeSymbol, name_, span_), underlyingType(nullptr), underlyingTypeId()
{
}

void EnumTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.BinaryStreamWriter().Write(underlyingType->Id());
}

void EnumTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(underlyingTypeId);
    reader.AddSymbolToBeResolved(this);
}

void EnumTypeSymbol::Resolve(SymbolTable* symbolTable)
{
    underlyingType = symbolTable->GetType(underlyingTypeId);
}

EnumConstantSymbol::EnumConstantSymbol(const std::u16string& name_, const soul::ast::Span& span_) : Symbol(SymbolKind::enumConstantSymbol, name_, span_)
{
}

void EnumConstantSymbol::Write(SymbolWriter& writer)
{
    Symbol::Write(writer);
}

void EnumConstantSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
}

} // namespace cmajor::fault::tolerant::symbols
