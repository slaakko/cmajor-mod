// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.type.symbol;

import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;

namespace cmajor::fault::tolerant::symbols {

TypeSymbol::TypeSymbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_) : ContainerSymbol(kind_, name_, span_), id(util::random_uuid())
{
}

void TypeSymbol::Write(SymbolWriter& writer)
{
    ContainerSymbol::Write(writer);
    writer.BinaryStreamWriter().Write(id);
}

void TypeSymbol::Read(SymbolReader& reader)
{
    ContainerSymbol::Read(reader);
    reader.BinaryStreamReader().ReadUuid(id);
}

const Derivations& TypeSymbol::GetDerivations() const
{
    static Derivations empty;
    return empty;
}

} // namespace cmajor::fault::tolerant::symbols
