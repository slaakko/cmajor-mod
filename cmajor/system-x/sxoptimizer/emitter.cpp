// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.optimizer.emitter;

import cmajor.systemx.assembler;
import cmajor.systemx.machine;

namespace cmajor::systemx::optimizer {

void EmitParamRegArg(cmajor::systemx::intermediate::ArgInstruction& inst, cmajor::systemx::intermediate::CodeGenerator& codeGen)
{
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setInst->AddOperand(cmajor::systemx::intermediate::MakeRegOperand(
        cmajor::systemx::intermediate::GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::GetParamRegNumber(inst.ArgIndex()))));
    cmajor::systemx::intermediate::EmitArg(inst, codeGen, setInst, false);
}

void EmitParamRegParam(cmajor::systemx::intermediate::ParamInstruction& inst, cmajor::systemx::intermediate::CodeGenerator& codeGen)
{
    cmajor::systemx::intermediate::Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting param: reg not valid");
    }
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setInst->AddOperand(cmajor::systemx::intermediate::MakeRegOperand(reg));
    setInst->AddOperand(cmajor::systemx::intermediate::MakeRegOperand(
        cmajor::systemx::intermediate::GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::GetParamRegNumber(inst.ParamIndex()))));
    codeGen.Emit(setInst);
}

} // cmajor::systemx::optimizer
