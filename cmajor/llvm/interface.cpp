// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <cmllvm/interface.hpp>

module cmajor.llvm.interface;

//import cmajor.llvm.emitting.context;
//import cmajor.llvm.emitter;

namespace cmajor::llvm {

std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel)
{
    return std::unique_ptr<cmajor::ir::EmittingContext>(static_cast<cmajor::ir::EmittingContext*>(cmllvm::CreateLLVMEmittingContext(optimizationLevel)));
}

std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    void* context = emittingContext;
    return std::unique_ptr<cmajor::ir::Emitter>(static_cast<cmajor::ir::Emitter*>(cmllvm::CreateLLVMEmitter(static_cast<cmllvm::EmittingContext*>(context))));
}

} // namespace cmajor::llvm
