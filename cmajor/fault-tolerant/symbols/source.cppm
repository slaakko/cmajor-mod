// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.source;

import std.core;

export namespace cmajor::fault::tolerant::symbols {

class Symbol;
class SymbolWriter;
class SymbolReader;

class Source
{
public:
    Source();
    Source(const std::string& filePath_);
    void Write(SymbolWriter& writer);
    void Read(SymbolReader& reader);
    const std::string& FilePath() const { return filePath; }
    void SetLineStarts(const std::vector<int>& lineStarts_);
    const std::vector<int>& LineStarts() const { return lineStarts; }
    void SetIndex(int index_) { index = index_; }
    int Index() const { return index; }
    void AddSymbol(Symbol* symbol);
    const std::vector<Symbol*>& Symbols() const { return symbols; }
private:
    std::string filePath;
    std::vector<int> lineStarts;
    int index;
    std::vector<Symbol*> symbols;
};

} // namespace cmajor::fault::tolerant::symbols
