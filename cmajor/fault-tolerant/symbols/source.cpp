// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.source;

import cmajor.fault.tolerant.symbols.symbol;
import cmajor.fault.tolerant.symbols.symbol.writer;
import cmajor.fault.tolerant.symbols.symbol.reader;

namespace cmajor::fault::tolerant::symbols {

Source::Source() : filePath(), index(-1)
{
}

Source::Source(const std::string& filePath_) : filePath(filePath_), index(-1)
{
}

void Source::Write(SymbolWriter& writer)
{
    writer.BinaryStreamWriter().Write(filePath);
    int32_t n = lineStarts.size();
    writer.BinaryStreamWriter().Write(n);
    for (auto lineStart : lineStarts)
    {
        writer.BinaryStreamWriter().Write(static_cast<int32_t>(lineStart));
    }
}

void Source::Read(SymbolReader& reader)
{
    filePath = reader.BinaryStreamReader().ReadUtf8String();
    int32_t n = reader.BinaryStreamReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        int lineStart = reader.BinaryStreamReader().ReadInt();
        lineStarts.push_back(lineStart);
    }
}

void Source::SetLineStarts(const std::vector<int>& lineStarts_)
{
    lineStarts = lineStarts_;
}

void Source::AddSymbol(Symbol* symbol)
{
    symbols.push_back(symbol);
    symbol->SetSource(this);
}

} // namespace cmajor::fault::tolerant::symbols
