// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.locals;

namespace cmajor::masm::optimizer {

void MoveLocalsToEntryBlock(cmajor::masm::intermediate::Function* fn)
{
    cmajor::masm::intermediate::BasicBlock* bb = fn->FirstBasicBlock();
    cmajor::masm::intermediate::BasicBlock* entryBlock = bb;
    cmajor::masm::intermediate::Instruction* lastEntryBlockLocal = nullptr;
    while (bb)
    {
        cmajor::masm::intermediate::Instruction* inst = bb->FirstInstruction();
        while (inst)
        {
            cmajor::masm::intermediate::Instruction* next = inst->Next();
            if (inst->IsLocalInstruction())
            {
                if (bb == entryBlock)
                {
                    lastEntryBlockLocal = inst;
                }
                else
                {
                    std::unique_ptr<cmajor::masm::intermediate::Instruction> removedLocal = inst->Parent()->RemoveInstruction(inst);
                    entryBlock->Instructions()->InsertAfter(removedLocal.release(), lastEntryBlockLocal);
                    lastEntryBlockLocal = inst;
                }
            }
            inst = next;
        }
        bb = bb->Next();
    }
}

} // cmajor::masm::optimizer
