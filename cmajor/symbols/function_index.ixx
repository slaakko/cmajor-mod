module;
#include <boost/uuid/uuid.hpp>
//#include <boost/functional/hash.hpp>

// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.function.index;

import std.core;
import util.binary.stream.writer;
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
    void AddFunction(const boost::uuids::uuid& functionId, FunctionSymbol* functionSymbol);
    void SetMainFunctionId(const boost::uuids::uuid& functionId);
    const boost::uuids::uuid& GetMainFunctionId() const { return mainFunctionId; }
    void Write(util::BinaryStreamWriter& writer);
private:
    Module* module;
    //std::unordered_map<boost::uuids::uuid, FunctionSymbol*, boost::hash<boost::uuids::uuid>> functionMap;
    std::map<boost::uuids::uuid, FunctionSymbol*> functionMap;
    boost::uuids::uuid mainFunctionId;
};
} // namespace cmajor::symbols
