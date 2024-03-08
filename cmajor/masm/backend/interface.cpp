// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.backend.interface;

import cmajor.masm.backend.emitting.context;
import cmajor.masm.backend.emitter;

namespace cmajor::masm::backend {

std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel)
{
    return std::unique_ptr<cmajor::ir::EmittingContext>(new MasmEmittingContext(optimizationLevel));
}

std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return std::unique_ptr<cmajor::ir::Emitter>(new MasmEmitter(emittingContext));
}

} // namespace cmajor::masm::backend
