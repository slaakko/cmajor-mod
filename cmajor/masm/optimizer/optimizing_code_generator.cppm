// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.optimizer.optimizing_code_generator;

import std.core;
import cmajor.masm.intermediate;
import cmajor.masm.assembly;

export namespace cmajor::masm::optimizer {

const int jumpTableSwitchThreshold = 4;
const int maxJumpTableEntries = 1024;

class OptimizingCodeGenerator : public cmajor::masm::intermediate::CodeGenerator
{
public:
    OptimizingCodeGenerator(cmajor::masm::intermediate::Context* context_, const std::string& assemblyFilePath_);
    void Emit(cmajor::masm::assembly::Instruction* assemblyInstruction) override;
    void Visit(cmajor::masm::intermediate::SwitchInstruction& inst) override;
    void Visit(cmajor::masm::intermediate::JmpInstruction& inst) override;
    void Visit(cmajor::masm::intermediate::RetInstruction& inst) override;
    void Visit(cmajor::masm::intermediate::NoOperationInstruction& inst) override;
    int ExitLabelId() const override;
    void EmitJumpToExit(cmajor::masm::intermediate::RetInstruction& retInst) override;
    void EmitBranchJumps(cmajor::masm::intermediate::BranchInstruction& branchInst) override;
private:
    void EmitXorInst(const std::string& label, cmajor::masm::assembly::Register* reg);
};

} // cmajor::masm::optimizer
