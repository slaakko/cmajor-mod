// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.opt.codegen;

import cmajor.systemx.intermediate.code;
import cmajor.systemx.intermediate.data;
import cmajor.systemx.intermediate.linear.scan.reg.allocator;
import cmajor.systemx.intermediate.reg.allocator;
import cmajor.systemx.machine;
import cmajor.systemx.assembler;
import util;

namespace cmajor::systemx::intermediate {

struct ByCaseValue
{
    bool operator()(const CaseTarget& left, const CaseTarget& right) const
    {
        int64_t leftCaseValue = left.caseValue->GetIntegerValue();
        int64_t rightCaseValue = right.caseValue->GetIntegerValue();
        return leftCaseValue < rightCaseValue;
    }
};

const CaseTarget* GetCaseTarget(const std::vector<CaseTarget>& caseTargets, int64_t value)
{
    LongValue val(value, nullptr);
    CaseTarget caseValue(&val, -1);
    auto it = std::lower_bound(caseTargets.begin(), caseTargets.end(), caseValue, ByCaseValue());
    if (it == caseTargets.end())
    {
        --it;
    }
    if (it->caseValue->GetIntegerValue() == value)
    {
        return &(*it);
    }
    return nullptr;
}

void EmitOptSwitch(SwitchInstruction& inst, CodeGenerator& codeGen)
{
    if (inst.CaseTargets().size() <= 4)
    {
        EmitSwitch(inst, codeGen);
    }
    else
    {
        std::vector<CaseTarget> caseTargets = inst.CaseTargets();
        for (const CaseTarget& caseTarget : caseTargets)
        {
            if (!caseTarget.caseValue->IsIntegerValue())
            {
                EmitSwitch(inst, codeGen);
                return;
            }
        }
        std::sort(caseTargets.begin(), caseTargets.end(), ByCaseValue());
        const CaseTarget& first = caseTargets.front();
        const CaseTarget& last = caseTargets.back();
        int64_t n = last.caseValue->GetIntegerValue() - first.caseValue->GetIntegerValue() + 1;
        int64_t start = first.caseValue->GetIntegerValue();
        int64_t end = last.caseValue->GetIntegerValue();

        cmajor::systemx::assembler::Instruction* setStartInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        setStartInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regCX)));
        setStartInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(start));
        codeGen.Emit(setStartInst);

        cmajor::systemx::assembler::Instruction* cmpStartInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::CMP);
        cmpStartInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        cmpStartInst->AddOperand(MakeRegOperand(inst.Cond(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen));
        cmpStartInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regCX)));
        codeGen.Emit(cmpStartInst);

        cmajor::systemx::assembler::Instruction* bnInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::BN);
        bnInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        bnInst->AddOperand(cmajor::systemx::assembler::MakeLocalSymbol(inst.DefaultTargetBlock()->Id()));
        codeGen.Emit(bnInst);

        cmajor::systemx::assembler::Instruction* setEndInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        setEndInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regDX)));
        setEndInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(end));
        codeGen.Emit(setEndInst);

        cmajor::systemx::assembler::Instruction* cmpEndInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::CMP);
        cmpEndInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        cmpEndInst->AddOperand(MakeRegOperand(inst.Cond(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen));
        cmpEndInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regDX)));
        codeGen.Emit(cmpEndInst);

        cmajor::systemx::assembler::Instruction* bpInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::BP);
        bpInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        bpInst->AddOperand(cmajor::systemx::assembler::MakeLocalSymbol(inst.DefaultTargetBlock()->Id()));
        codeGen.Emit(bpInst);

        std::string jumpTableStructName = "jmptab@" + codeGen.CurrentFunction()->Name() + "@" + std::to_string(codeGen.CurrentLineNumber());

        cmajor::systemx::assembler::Instruction* ldoInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::LDOU);
        ldoInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regEX));
        ldoInst->AddOperand(cmajor::systemx::assembler::MakeGlobalSymbol(jumpTableStructName));
        codeGen.Emit(ldoInst);

        cmajor::systemx::assembler::Instruction* subInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::SUB);
        subInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regBX));
        subInst->AddOperand(MakeRegOperand(inst.Cond(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen));
        subInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regCX)));
        codeGen.Emit(subInst);

        cmajor::systemx::assembler::Instruction* i8AdduInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::I8ADDU);
        i8AdduInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regAX));
        i8AdduInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regBX));
        i8AdduInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regEX));
        codeGen.Emit(i8AdduInst);

        cmajor::systemx::assembler::Instruction* ldouInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::LDOU);
        ldouInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regAX));
        ldouInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regAX));
        codeGen.Emit(ldouInst);

        cmajor::systemx::assembler::Instruction* goInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::GO);
        goInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regAX));
        goInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regAX));
        codeGen.Emit(goInst);

        std::string dataSymbolName = jumpTableStructName + "_data";
        cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
        octaInst->AddOperand(cmajor::systemx::assembler::MakeGlobalSymbol(dataSymbolName));
        codeGen.AssemblyFile()->GetLinkSection()->GetOrCreateExternObject()->AddExternSymbol(cmajor::systemx::assembler::MakeGlobalSymbol(jumpTableStructName));
        cmajor::systemx::assembler::AssemblyStruct* jumpTableStruct = codeGen.AssemblyFile()->GetDataSection()->CreateStructure(jumpTableStructName);
        jumpTableStruct->AddInstruction(octaInst);
        for (int64_t i = 0; i < n; ++i)
        {
            cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
            if (i == 0)
            {
                octaInst->SetLabel(cmajor::systemx::assembler::MakeGlobalSymbol(dataSymbolName));
            }
            const CaseTarget* target = GetCaseTarget(caseTargets, start + i);
            BasicBlock* targetBlock = inst.DefaultTargetBlock();
            if (target)
            {
                targetBlock = target->targetBlock;
            }
            octaInst->AddOperand(cmajor::systemx::assembler::MakeGlobalSymbol(codeGen.CurrentFunction()->Name() + "@" + std::to_string(targetBlock->Id())));
            jumpTableStruct->AddInstruction(octaInst);
        }
    }
}

} // cmajor::systemx::intermediate
