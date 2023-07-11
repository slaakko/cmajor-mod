// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmllvm/interface.hpp>
#include <cmllvm/llvm_emitter.hpp>
#include <cmllvm/llvm_emitting_context.hpp>

namespace cmllvm {

void* CreateLLVMEmittingContext(int optimizationLevel)
{
    void* emittingContext = new LLVMEmittingContext(optimizationLevel);
    return emittingContext;
}

void* CreateLLVMEmitter(EmittingContext* emittingContext)
{
    void* emitter = new LLVMEmitter(emittingContext);
    return emitter;
}

} // namespace cmllvm
