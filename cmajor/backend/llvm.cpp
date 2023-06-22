// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.backend.llvm;

import cmajor.llvm;

namespace cmajor::backend {

std::unique_ptr<cmajor::ir::EmittingContext> LLvmBackEnd::CreateEmittingContext(int optimizationLevel)
{
    return cmajor::llvm::CreateEmittingContext(optimizationLevel);
}

std::unique_ptr<cmajor::ir::Emitter> LLvmBackEnd::CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return cmajor::llvm::CreateEmitter(emittingContext);
}

std::unique_ptr<cmajor::codegen::CodeGenerator> LLvmBackEnd::CreateCodeGenerator(cmajor::ir::Emitter* emitter)
{
#ifdef _WIN32
    return std::unique_ptr<cmajor::codegen::CodeGenerator>(new cmajor::llvm::WindowsCodeGenerator(emitter));
#else
    // TODO
#endif
}

} // namespace cmajor::backend
