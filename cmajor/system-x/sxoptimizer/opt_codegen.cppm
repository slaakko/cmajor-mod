// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.optimizer.opt_codegen;

import cmajor.systemx.intermediate;
import std.core;

export namespace cmajor::systemx::optimizer {

class OptimizingCodeGenerator : public cmajor::systemx::intermediate::SimpleAssemblyCodeGenerator
{
public:
    OptimizingCodeGenerator(cmajor::systemx::intermediate::Context* context_, cmajor::systemx::assembler::AssemblyFile* assemblyFile_);
    void Visit(cmajor::systemx::intermediate::ArgInstruction& inst) override;
    void Visit(cmajor::systemx::intermediate::ParamInstruction& inst) override;
};

} // cmajor::systemx::optimizer
