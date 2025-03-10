// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.backend.sbin;

import std.core;
import cmajor.backend.selector;

export namespace cmajor::backend {

class SBinBackEnd : public BackEnd
{
public:
    std::unique_ptr<cmajor::ir::EmittingContext> CreateEmittingContext(int optimizationLevel) override;
    std::unique_ptr<cmajor::ir::Emitter> CreateEmitter(cmajor::ir::EmittingContext* emittingContext) override;
    std::unique_ptr<cmajor::codegen::CodeGenerator> CreateCodeGenerator(cmajor::ir::Emitter* emitter) override;
    BackEndKind GetBackEndKind() override { return BackEndKind::sbinBackEnd; }
};

} // namespace cmajor::backend
