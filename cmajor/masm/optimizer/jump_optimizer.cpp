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
            cmajor::masm::intermediate::Instruction* leader = target->FirstInstruction();
            int count = 0;
            while (leader->IsJumpInstruction())
            {
                cmajor::masm::intermediate::JmpInstruction* jmpLeader = static_cast<cmajor::masm::intermediate::JmpInstruction*>(leader);
                target = jmpLeader->TargetBasicBlock();
                leader = target->FirstInstruction();
                ++count;
            }
            if (target != firstTarget)
            {
                jmp->SetTargetBasicBlock(target);
                //std::cout << "jump " << count << "\n";
            }
        }
        bb = bb->Next();
    }
}

} // cmajor::masm::optimizer
