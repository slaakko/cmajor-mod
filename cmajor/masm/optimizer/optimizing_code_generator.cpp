// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.optimizing_code_generator;

import cmajor.masm.assembly;

namespace cmajor::masm::optimizer {

struct CaseLess
{
    bool operator()(const cmajor::masm::intermediate::CaseTarget& left, cmajor::masm::intermediate::CaseTarget& right) const
    {
        return left.caseValue->GetIntegerValue() < right.caseValue->GetIntegerValue();
    }
};

void EmitJumpTableSwitch(cmajor::masm::intermediate::SwitchInstruction& inst, cmajor::masm::intermediate::CodeGenerator& codeGenerator)
{
    for (const cmajor::masm::intermediate::CaseTarget& caseTarget : inst.CaseTargets())
    {
        if (!caseTarget.caseValue->IsIntegerValue())
        {
            cmajor::masm::intermediate::EmitSwitch(inst, codeGenerator);
            return;
        }
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    int size = static_cast<int>(inst.Cond()->GetType()->Size());
    std::vector<cmajor::masm::intermediate::CaseTarget> caseTargets = inst.CaseTargets();
    std::sort(caseTargets.begin(), caseTargets.end(), CaseLess());
    cmajor::masm::assembly::Register* condReg = cmajor::masm::intermediate::MakeIntegerRegOperand(
        inst.Cond(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rcx), codeGenerator);
    int64_t low = caseTargets.front().caseValue->GetIntegerValue();
    int64_t count = caseTargets.back().caseValue->GetIntegerValue() - low + 1;
    if (count > maxJumpTableEntries)
    {
        cmajor::masm::intermediate::EmitSwitch(inst, codeGenerator);
        return;
    }
    if (low != 0)
    {
        cmajor::masm::assembly::Instruction* subInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::SUB);
        subInst->AddOperand(condReg);
        if (low > 0xFFFFFFFF)
        {
            cmajor::masm::assembly::Instruction* movRaxInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            cmajor::masm::assembly::Register* rax = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax);
            movRaxInst->AddOperand(rax);
            movRaxInst->AddOperand(assemblyContext->MakeIntegerLiteral(low, 8));
            codeGenerator.Emit(movRaxInst);
            subInst->AddOperand(rax);
        }
        else
        {
            subInst->AddOperand(assemblyContext->MakeIntegerLiteral(low, size));
        }
        codeGenerator.Emit(subInst);
    }
    std::map<int64_t, int32_t> caseMap;
    for (const cmajor::masm::intermediate::CaseTarget& caseTarget : inst.CaseTargets())
    {
        int64_t value = caseTarget.caseValue->GetIntegerValue() - low;
        int32_t targetLabelId = caseTarget.targetLabelId;
        caseMap[value] = targetLabelId;
    }
    cmajor::masm::assembly::Instruction* cmpInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CMP);
    cmpInst->AddOperand(condReg);
    cmpInst->AddOperand(assemblyContext->MakeIntegerLiteral(count, size));
    codeGenerator.Emit(cmpInst);
    cmajor::masm::assembly::Instruction* jaeIst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::JAE);
    jaeIst->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(inst.DefaultTargetBlock()->Id())));
    codeGenerator.Emit(jaeIst);
    cmajor::masm::assembly::Register* condReg8 = assemblyContext->GetRegisterPool()->GetRegisterGroup(condReg->Group(), true)->GetReg(8);
    if (condReg->Size() == 4)
    {
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(condReg);
        movInst->AddOperand(condReg);
        codeGenerator.Emit(movInst);
    }
    else if (condReg->Size() != 8)
    {
        cmajor::masm::assembly::Instruction* movzxInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOVZX);
        movzxInst->AddOperand(condReg8);
        movzxInst->AddOperand(condReg);
        codeGenerator.Emit(movzxInst);
    }
    std::string jumpTabLabel = "@jmptab" + std::to_string(assemblyContext->GetNextJumpTabLabelId());
    cmajor::masm::assembly::Instruction* setJumpTabAddrInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
    cmajor::masm::assembly::Register* jumpTabAddrReg = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbx);
    setJumpTabAddrInst->AddOperand(jumpTabAddrReg);
    setJumpTabAddrInst->AddOperand(assemblyContext->MakeSymbol(jumpTabLabel));
    codeGenerator.Emit(setJumpTabAddrInst);
    cmajor::masm::assembly::Instruction* movSxdInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOVSXD);
    cmajor::masm::assembly::Register* jumpReg = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax);
    movSxdInst->AddOperand(jumpReg);
    movSxdInst->AddOperand(assemblyContext->MakeSizePrefix(4, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(jumpTabAddrReg,
        assemblyContext->MakeBinaryExpr(condReg8, assemblyContext->MakeIntegerLiteral(4, 4), cmajor::masm::assembly::Operator::mul),
        cmajor::masm::assembly::Operator::add))));
    codeGenerator.Emit(movSxdInst);
    cmajor::masm::assembly::Instruction* addInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::ADD);
    addInst->AddOperand(jumpReg);
    addInst->AddOperand(jumpTabAddrReg);
    codeGenerator.Emit(addInst);
    cmajor::masm::assembly::Instruction* jmpIst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::JMP);
    jmpIst->AddOperand(jumpReg);
    codeGenerator.Emit(jmpIst);
    for (int64_t i = 0; i < count; ++i)
    {
        cmajor::masm::assembly::Instruction* offsetInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::DD);
        if (i == 0)
        {
            offsetInst->SetLabel(jumpTabLabel);
            offsetInst->SetWriteln();
        }
        int32_t targetLabelId = inst.DefaultTargetLabelId();
        auto it = caseMap.find(i);
        if (it != caseMap.end())
        {
            targetLabelId = it->second;
        }
        offsetInst->AddOperand(assemblyContext->MakeBinaryExpr(assemblyContext->MakeSymbol("@" + std::to_string(targetLabelId)),
            assemblyContext->MakeSymbol(jumpTabLabel), cmajor::masm::assembly::Operator::sub));
        codeGenerator.Emit(offsetInst);
    }
}

OptimizingCodeGenerator::OptimizingCodeGenerator(cmajor::masm::intermediate::Context* context_, const std::string& assemblyFilePath_) :
    cmajor::masm::intermediate::CodeGenerator(context_, assemblyFilePath_)
{
}

void OptimizingCodeGenerator::Visit(cmajor::masm::intermediate::SwitchInstruction& inst)
{
    inst.SetAssemblyIndex(AssemblyFunction()->Index());
    if (inst.CaseTargets().size() <= jumpTableSwitchThreshold)
    {
        cmajor::masm::intermediate::EmitSwitch(inst, *this);
    }
    else
    {
        EmitJumpTableSwitch(inst, *this);
    }
}

} // cmajor::masm::optimizer
