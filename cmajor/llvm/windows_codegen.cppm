// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.codegen.windows;

import std.core;
import cmajor.llvm.codegen;

export namespace cmajor::llvm {

class WindowsCodeGenerator : public LLVMCodeGenerator
{
public:
    WindowsCodeGenerator(cmajor::ir::Emitter* emitter_);
    void Visit(cmajor::binder::BoundReturnStatement& boundReturnStatement) override;
    void Visit(cmajor::binder::BoundGotoCaseStatement& boundGotoCaseStatement) override;
    void Visit(cmajor::binder::BoundGotoDefaultStatement& boundGotoDefaultStatement) override;
    void Visit(cmajor::binder::BoundBreakStatement& boundBreakStatement) override;
    void Visit(cmajor::binder::BoundContinueStatement& boundContinueStatement) override;
    void Visit(cmajor::binder::BoundGotoStatement& boundGotoStatement) override;
    void Visit(cmajor::binder::BoundTryStatement& boundTryStatement) override;
    void Visit(cmajor::binder::BoundRethrowStatement& boundRethrowStatement) override;
protected:
    void CreateCleanup() override;
    void* GetPersonalityFunction() const override;
    void GenerateCodeForCleanups() override;
};

} // namespace cmajor::llvm
