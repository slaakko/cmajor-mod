// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.jump_optimizer;

namespace cmajor::masm::optimizer {

void OptimizeJumps(cmajor::masm::intermediate::Function* fn)
{
    cmajor::masm::intermediate::BasicBlock* bb = fn->FirstBasicBlock();
    while (bb)
    {
        cmajor::masm::intermediate::Instruction* terminator = bb->LastInstruction();
        if (terminator->IsJumpInstruction())
        {
            cmajor::masm::intermediate::JmpInstruction* jmp = static_cast<cmajor::masm::intermediate::JmpInstruction*>(terminator);
            cmajor::masm::intermediate::BasicBlock* target = jmp->TargetBasicBlock();
            cmajor::masm::intermediate::BasicBlock* firstTarget = target;
            cmajor::masm::intermediate::Instruction* leader = target->Leader();
            while (leader->IsJumpInstruction())
            {
                cmajor::masm::intermediate::JmpInstruction* jmpLeader = static_cast<cmajor::masm::intermediate::JmpInstruction*>(leader);
                target = jmpLeader->TargetBasicBlock();
                leader = target->Leader();
            }
            if (target != firstTarget)
            {
                jmp->SetTargetBasicBlock(target);
            }
        }
        else if (terminator->IsBranchInstruction())
        {
            cmajor::masm::intermediate::BranchInstruction* branch = static_cast<cmajor::masm::intermediate::BranchInstruction*>(terminator);
            cmajor::masm::intermediate::BasicBlock* trueTarget = branch->TrueTargetBasicBlock();
            cmajor::masm::intermediate::BasicBlock* firstTrueTarget = trueTarget;
            cmajor::masm::intermediate::Instruction* trueLeader = trueTarget->Leader();
            while (trueLeader->IsJumpInstruction())
            {
                cmajor::masm::intermediate::JmpInstruction* jmpLeader = static_cast<cmajor::masm::intermediate::JmpInstruction*>(trueLeader);
                trueTarget = jmpLeader->TargetBasicBlock();
                trueLeader = trueTarget->Leader();
            }
            if (trueTarget != firstTrueTarget)
            {
                branch->SetTrueTargetBasicBlock(trueTarget);
            }
            cmajor::masm::intermediate::BasicBlock* falseTarget = branch->FalseTargetBasicBlock();
            cmajor::masm::intermediate::BasicBlock* firstFalseTarget = falseTarget;
            cmajor::masm::intermediate::Instruction* falseLeader = falseTarget->Leader();
            while (falseLeader->IsJumpInstruction())
            {
                cmajor::masm::intermediate::JmpInstruction* jmpLeader = static_cast<cmajor::masm::intermediate::JmpInstruction*>(falseLeader);
                falseTarget = jmpLeader->TargetBasicBlock();
                falseLeader = falseTarget->Leader();
            }
            if (falseTarget != firstFalseTarget)
            {
                branch->SetFalseTargetBasicBlock(falseTarget);
            }
        }
        bb = bb->Next();
    }
}

} // cmajor::masm::optimizer
