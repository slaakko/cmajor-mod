// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.constant.symbol;

import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;
import cmajor.fault.tolerant.symbols.type.symbol;
import cmajor.fault.tolerant.symbols.symbol.table;

namespace cmajor::fault::tolerant::symbols {

ConstantSymbol::ConstantSymbol(const std::u16string& name_, const soul::ast::Span& span_) : Symbol(SymbolKind::constantSymbol, name_, span_), type(nullptr), typeId()
{
}

void ConstantSymbol::Write(SymbolWriter& writer)
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

void ConstantSymbol::Read(SymbolReader& reader)
{
    Symbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(typeId);
    if (!typeId.is_nil())
    {
        reader.AddSymbolToBeResolved(this);
    }
}

void ConstantSymbol::Resolve(SymbolTable* symbolTable)
{
    if (!typeId.is_nil())
    {
        type = symbolTable->GetType(typeId);
    }
}

} // namespace cmajor::fault::tolerant::symbols
