// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.backend.selector;

import std.core;
import cmajor.ir;
import cmajor.codegen;

export namespace cmajor::backend {

enum class BackEndKind : int
{
    llvmBackEnd = 0, systemxBackEnd = 1, cppBackEnd = 2
};

class BackEnd
{
public:
    virtual ~BackEnd();
    virtual std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel) = 0;
    virtual std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext) = 0;
    virtual std::unique_ptr<cmajor::codegen::CodeGenerator> CreateCodeGenerator(cmajor::ir::Emitter* emitter) = 0;
    virtual BackEndKind GetBackEndKind() = 0;
};

void SetCurrentBackEnd(BackEndKind backEndKind);
BackEnd* GetCurrentBackEnd();

} // namespace cmajor::backend
