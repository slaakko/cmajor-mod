// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.symbol.map;

import cmajor.symbols.function.symbol;
import cmajor.symbols.global.flags;
import std.core;

export namespace cmajor::symbols {

class SymbolMap
{
public:
    SymbolMap(const std::string& config, BackEnd backend, int optLevel);
    void Read();
    void Write();
    void Add(FunctionSymbol* functionSymbol);
    void Check();
private:
    std::string filePath;
    std::map<std::string, std::set<std::string>> fullNameMangledNameMap;
    std::map<std::string, std::set<std::string>> mangledNameFullNameMap;
};

SymbolMap* GetSymbolMap();
void SetSymbolMap(SymbolMap* symbolMap);

} // namespace cmajor::symbols
