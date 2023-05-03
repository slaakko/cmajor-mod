module cmajor.symbols.function.index;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

/*
#include <cmajor/symbols/FunctionIndex.hpp>
#include <cmajor/symbols/Module.hpp>
#include <cmajor/symbols/FunctionSymbol.hpp>
#include <cmajor/cmdebug/DebugInfoIo.hpp>
#include <soulng/util/Unicode.hpp>
*/
import cmajor.symbols.function.symbol;
import cmajor.symbols.module_;
import util;

namespace cmajor::symbols {

        //using namespace soulng::unicode;

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
