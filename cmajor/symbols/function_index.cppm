// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.function.index;

import std.core;
import util.binary.stream.writer;
import util.uuid;

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
    std::map<util::uuid, FunctionSymbol*> functionMap;
    util::uuid mainFunctionId;
};
} // namespace cmajor::symbols
