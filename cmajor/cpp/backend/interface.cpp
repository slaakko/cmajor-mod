// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.cpp.backend.interface;

import cmajor.cpp.backend.emitting.context;
import cmajor.cpp.backend.emitter;

namespace cmajor::cpp::backend {

std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel)
{
    return std::unique_ptr<cmajor::ir::EmittingContext>(new CppEmittingContext(optimizationLevel));
}

std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return std::unique_ptr<cmajor::ir::Emitter>(new CppEmitter(emittingContext));
}

} // namespace cmajor::cpp::backend
