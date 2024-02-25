// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.symbol.writer;

import soul.ast.span;
import util;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class Symbol;
class SymbolTable;

class SymbolWriter
{
public:
    SymbolWriter(const std::string& filePath_);
    util::BinaryStreamWriter& BinaryStreamWriter() { return writer; }
    void Write(Symbol* symbol);
    void Write(const soul::ast::Span& span);
private:
    std::string filePath;
    util::FileStream fileStream;
    util::BufferedStream bufferedStream;
    util::BinaryStreamWriter writer;
};

void WriteSymbols(SymbolTable* symbolTable, const std::string& symbolTableFilePath);

} // namespace cmajor::fault::tolerant::symbols
