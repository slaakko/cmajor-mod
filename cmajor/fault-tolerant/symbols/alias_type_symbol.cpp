// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.alias.type.symbol;

import cmajor.fault.tolerant.symbols.symbol.table;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;
import cmajor.fault.tolerant.symbols.type.symbol;

namespace cmajor::fault::tolerant::symbols {

AliasTypeSymbol::AliasTypeSymbol(const std::u16string& name_, const soul::ast::Span& span_) : Symbol(SymbolKind::aliasTypeSymbol, name_, span_), type(nullptr)
{
}

void AliasTypeSymbol::Write(SymbolWriter& writer)
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

void AliasTypeSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(typeId);
    if (!typeId.is_nil())
    {
        reader.AddSymbolToBeResolved(this);
    }
}

void AliasTypeSymbol::Resolve(SymbolTable* symbolTable)
{
    if (!typeId.is_nil())
    {
        type = symbolTable->GetType(typeId);
    }
}

} // namespace cmajor::fault::tolerant::symbols
