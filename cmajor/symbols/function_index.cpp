// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.function.index;

import cmajor.symbols.function.symbol;
import cmajor.symbols.modules;
import cmajor.debug;
import util;

namespace cmajor::symbols {

FunctionIndex::FunctionIndex(Module* module_) : module(module_), mainFunctionId(util::nil_uuid())
{
}

void FunctionIndex::AddFunction(const util::uuid& functionId, FunctionSymbol* functionSymbol)
{
    std::lock_guard<std::recursive_mutex> lock(module->GetLock());
    if (functionMap.find(functionId) == functionMap.cend())
    {
        functionMap[functionId] = functionSymbol;
    }
}

void FunctionIndex::SetMainFunctionId(const util::uuid& functionId)
{
    mainFunctionId = functionId;
}

void FunctionIndex::Write(util::BinaryStreamWriter& writer)
{
    int32_t numFunctions = functionMap.size();
    cmajor::debug::WriteNumberOfFunctionIndexFunctionRecords(writer, numFunctions);
    for (const auto& p : functionMap)
    {
        FunctionSymbol* functionSymbol = p.second;
        cmajor::debug::WriteFunctionIndexFunctionRecord(writer, p.first, util::ToUtf8(functionSymbol->FullName()), util::ToUtf8(functionSymbol->MangledName()));
    }
}

} // namespace cmajor::symbols
