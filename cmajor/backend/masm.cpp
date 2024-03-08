// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.backend.masm;

import cmajor.masm.backend;

namespace cmajor::backend {

std::unique_ptr<cmajor::ir::EmittingContext> MasmBackEnd::CreateEmittingContext(int optimizationLevel)
{
    return cmajor::masm::backend::CreateEmittingContext(optimizationLevel);
}

std::unique_ptr<cmajor::ir::Emitter> MasmBackEnd::CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return cmajor::masm::backend::CreateEmitter(emittingContext);
}

std::unique_ptr<cmajor::codegen::CodeGenerator> MasmBackEnd::CreateCodeGenerator(cmajor::ir::Emitter* emitter)
{
    return std::unique_ptr<cmajor::codegen::CodeGenerator>(new cmajor::masm::backend::MasmCodeGenerator(emitter));
}

} // namespace cmajor::backend
