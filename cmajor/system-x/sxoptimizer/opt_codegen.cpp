// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.optimizer.opt_codegen;

import cmajor.systemx.optimizer.optimize;
import cmajor.systemx.optimizer.emitter;
import cmajor.systemx.machine;

namespace cmajor::systemx::optimizer {

OptimizingCodeGenerator::OptimizingCodeGenerator(cmajor::systemx::intermediate::Context* context_, cmajor::systemx::assembler::AssemblyFile* assemblyFile_) :
    SimpleAssemblyCodeGenerator(context_, assemblyFile_)
{
}

void OptimizingCodeGenerator::Visit(cmajor::systemx::intermediate::ArgInstruction& inst)
{
    if (HasOptimization(Optimizations::paramRegs))
    {
        if (inst.ArgIndex() < cmajor::systemx::machine::numParamRegs)
        {
            EmitParamRegArg(inst, *this);
        }
        else
        {
            SimpleAssemblyCodeGenerator::Visit(inst);
        }
    }
    else
    {
        SimpleAssemblyCodeGenerator::Visit(inst);
    }
}

void OptimizingCodeGenerator::Visit(cmajor::systemx::intermediate::ParamInstruction& inst)
{
    if (HasOptimization(Optimizations::paramRegs) && !inst.Parent()->Parent()->IsMain())
    {
        if (inst.ParamIndex() < cmajor::systemx::machine::numParamRegs)
        {
            EmitParamRegParam(inst, *this);
        }
        else
        {
            SimpleAssemblyCodeGenerator::Visit(inst);
        }
    }
    else
    {
        SimpleAssemblyCodeGenerator::Visit(inst);
    }
}

} // cmajor::systemx::optimizer
