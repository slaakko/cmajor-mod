//#include <boost/functional/hash.hpp>

// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.function.index;

import std.core;
import util.binary.stream.writer;
import util.uuid;
/*
#include <cmajor/symbols/SymbolsApi.hpp>
#include <soulng/util/BinaryWriter.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <mutex>

*/
export namespace cmajor::symbols {

class Module;
class FunctionSymbol;

class FunctionIndex
{
public:
    FunctionIndex(Module* module_);
    void AddFunction(const util::uuid& functionId, FunctionSymbol* functionSymbol);
    void SetMainFunctionId(const util::uuid& functionId);
    const util::uuid& GetMainFunctionId() const { return mainFunctionId; }
    void Write(util::BinaryStreamWriter& writer);
private:
    Module* module;
    //std::unordered_map<util::uuid, FunctionSymbol*, boost::hash<util::uuid>> functionMap;
    std::map<util::uuid, FunctionSymbol*> functionMap;
    util::uuid mainFunctionId;
};
} // namespace cmajor::symbols
