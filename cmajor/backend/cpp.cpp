// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.backend.cpp;

import cmajor.cpp.backend;

namespace cmajor::backend {

std::unique_ptr<cmajor::ir::EmittingContext> CppBackEnd::CreateEmittingContext(int optimizationLevel)
{
    return cmajor::cpp::backend::CreateEmittingContext(optimizationLevel);
}

std::unique_ptr<cmajor::ir::Emitter> CppBackEnd::CreateEmitter(cmajor::ir::EmittingContext* emittingContext)
{
    return cmajor::cpp::backend::CreateEmitter(emittingContext);
}

std::unique_ptr<cmajor::codegen::CodeGenerator> CppBackEnd::CreateCodeGenerator(cmajor::ir::Emitter* emitter)
{
    return std::unique_ptr<cmajor::codegen::CodeGenerator>(new cmajor::cpp::backend::CppCodeGenerator(emitter));
}

} // namespace cmajor::backend
