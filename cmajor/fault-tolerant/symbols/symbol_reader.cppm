// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.symbol.reader;

import soul.ast.span;
import util;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class Symbol;
class SymbolTable;

class SymbolReader
{
public:
    SymbolReader(const std::string& filePath_);
    util::BinaryStreamReader& BinaryStreamReader() { return reader; }
    Symbol* ReadSymbol();
    soul::ast::Span ReadSpan();
    void SetSymbolTable(SymbolTable* symbolTable_) { symbolTable = symbolTable_; }
    SymbolTable* GetSymbolTable() const { return symbolTable; }
    void AddSymbolToBeResolved(Symbol* symbol);
    void ResolveSymbols();
private:
    std::string filePath;
    util::FileStream fileStream;
    util::BufferedStream bufferedStream;
    util::BinaryStreamReader reader;
    SymbolTable* symbolTable;
    std::vector<Symbol*> toBeResolved;
};

} // namespace cmajor::fault::tolerant::symbols
