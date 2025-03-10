// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.backend.codegen;

import cmajor.codegen;
import cmajor.ir;
import cmajor.masm.backend;

export namespace cmajor::sbin::backend {

class SBinCodeGenerator : public cmajor::codegen::CodeGenerator
{
public:
    SBinCodeGenerator(cmajor::ir::Emitter* emitter_);
    void Visit(cmajor::binder::BoundCompileUnit& boundCompileUnit) override;
private:
    cmajor::ir::Emitter* emitter;
    cmajor::masm::backend::MasmCodeGenerator masmCodeGenerator;
};

} // namespace cmajor::sbin::backend
