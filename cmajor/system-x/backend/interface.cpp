// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.backend.interface;

import cmajor.systemx.backend.emitting.context;
import cmajor.systemx.backend.emitter;

namespace cmajor::systemx::backend {

std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel)
{
    return std::unique_ptr<cmajor::ir::EmittingContext>(new SystemXEmittingContext(optimizationLevel));
}

std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return std::unique_ptr<cmajor::ir::Emitter>(new SystemXEmitter(emittingContext));
}

} // namespace cmajor::systemx::backend
