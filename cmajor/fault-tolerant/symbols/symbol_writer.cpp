// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.symbol.writer;

import cmajor.fault.tolerant.symbols.symbol;
import cmajor.fault.tolerant.symbols.symbol.table;

namespace cmajor::fault::tolerant::symbols {

SymbolWriter::SymbolWriter(const std::string& filePath_) : 
    filePath(filePath_), fileStream(filePath, util::OpenMode::write | util::OpenMode::binary), bufferedStream(fileStream), writer(bufferedStream)
{
}

void SymbolWriter::Write(Symbol* symbol)
{
    writer.Write(static_cast<uint8_t>(symbol->Kind()));
    writer.Write(symbol->Name());
    Write(symbol->Span());
    symbol->Write(*this);
}

void SymbolWriter::Write(const soul::ast::Span& span)
{
    if (span.IsValid())
    {
        writer.Write(true);
        writer.Write(span.pos);
        writer.Write(span.len);
    }
    else
    {
        writer.Write(false);
    }
}

void WriteSymbols(SymbolTable* symbolTable, const std::string& symbolTableFilePath)
{
    SymbolWriter writer(symbolTableFilePath);
    symbolTable->Write(writer);
}

} // namespace cmajor::fault::tolerant::symbols
