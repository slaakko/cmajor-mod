// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.llvm.interface;

import cmajor.llvm.emitting.context;
import cmajor.llvm.emitter;

namespace cmajor::llvm {

std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel)
{
    return std::unique_ptr<cmajor::ir::EmittingContext>(new LLvmEmittingContext(optimizationLevel));
}

std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return std::unique_ptr<cmajor::ir::Emitter>();
}

} // namespace cmajor::llvm
