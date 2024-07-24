// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#pragma warning(disable : 4996)
#include <llvm/ADT/ilist_base.h>
#include <llvm/IR/DebugInfoMetadata.h>
#pragma warning(default : 4996)

module cmajor.llvm.interface;

import cmajor.llvm.emitting.context;
import cmajor.llvm.emitter;

namespace cmajor::llvm {

std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel)
{
    return std::unique_ptr<cmajor::ir::EmittingContext>(new cmllvm::LLVMEmittingContext(optimizationLevel));
}

std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return std::unique_ptr<cmajor::ir::Emitter>(new cmllvm::LLVMEmitter(emittingContext));
}

} // namespace cmajor::llvm
