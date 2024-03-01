// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.symbol.writer;

import std.core;
import cmajor.symbols.symbol;

namespace cmajor::symbols {

SymbolWriter::SymbolWriter(const std::string& fileName_) : astWriter(fileName_)
{
}

void SymbolWriter::Write(Symbol* symbol)
{
    astWriter.GetBinaryStreamWriter().Write(static_cast<uint8_t>(symbol->GetSymbolType()));
    astWriter.Write(symbol->GetSpan());
    astWriter.GetBinaryStreamWriter().Write(symbol->Name());
    symbol->Write(*this);
}

} // namespace cmajor::symbols
