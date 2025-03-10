// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.backend.sbin;

import cmajor.sbin.backend;
import cmajor.masm.backend;

namespace cmajor::backend {

std::unique_ptr<cmajor::ir::EmittingContext> SBinBackEnd::CreateEmittingContext(int optimizationLevel)
{
    return cmajor::masm::backend::CreateEmittingContext(optimizationLevel);
}

std::unique_ptr<cmajor::ir::Emitter> SBinBackEnd::CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return cmajor::masm::backend::CreateEmitter(emittingContext);
}

std::unique_ptr<cmajor::codegen::CodeGenerator> SBinBackEnd::CreateCodeGenerator(cmajor::ir::Emitter* emitter)
{
    return std::unique_ptr<cmajor::codegen::CodeGenerator>(new cmajor::sbin::backend::SBinCodeGenerator(emitter));
}

} // namespace cmajor::backend
