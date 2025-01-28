// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.backend.systemx;

import cmajor.systemx.backend;

namespace cmajor::backend {

std::unique_ptr<cmajor::ir::EmittingContext> SystemXBackEnd::CreateEmittingContext(int optimizationLevel)
{
    return cmajor::systemx::backend::CreateEmittingContext(optimizationLevel);
}

std::unique_ptr<cmajor::ir::Emitter> SystemXBackEnd::CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return cmajor::systemx::backend::CreateEmitter(emittingContext);
}

std::unique_ptr<cmajor::codegen::CodeGenerator> SystemXBackEnd::CreateCodeGenerator(cmajor::ir::Emitter* emitter)
{
    return std::unique_ptr<cmajor::codegen::CodeGenerator>(new cmajor::systemx::backend::SystemXCodeGenerator(emitter));
}

} // namespace cmajor::backend
