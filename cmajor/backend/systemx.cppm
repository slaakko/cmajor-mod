// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.backend.systemx;

import std.core;
import cmajor.backend.selector;

export namespace cmajor::backend {

class SystemXBackEnd : public BackEnd
{
public:
    std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel) override;
    std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext) override;
    std::unique_ptr<cmajor::codegen::CodeGenerator> CreateCodeGenerator(cmajor::ir::Emitter* emitter) override;
    BackEndKind GetBackEndKind() override { return BackEndKind::systemxBackEnd; }
};

} // namespace cmajor::backend
