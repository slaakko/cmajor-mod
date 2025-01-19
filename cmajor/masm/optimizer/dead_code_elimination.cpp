// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.dead_code_elimination;

namespace cmajor::masm::optimizer {

void Walk(cmajor::masm::intermediate::BasicBlock* bb, std::set<cmajor::masm::intermediate::BasicBlock*>& reachable)
{
    if (reachable.find(bb) != reachable.end()) return;
    reachable.insert(bb);
    cmajor::masm::intermediate::Instruction* terminator = bb->LastInstruction();
    switch (terminator->GetOpCode())
    {
        case cmajor::masm::intermediate::OpCode::jmp:
        {
            cmajor::masm::intermediate::JmpInstruction* jmpInst = static_cast<cmajor::masm::intermediate::JmpInstruction*>(terminator);
            cmajor::masm::intermediate::BasicBlock* target = jmpInst->TargetBasicBlock();
            Walk(target, reachable);
            break;
        }
        case cmajor::masm::intermediate::OpCode::branch:
        {
            cmajor::masm::intermediate::BranchInstruction* branchInst = static_cast<cmajor::masm::intermediate::BranchInstruction*>(terminator);
            cmajor::masm::intermediate::BasicBlock* trueTarget = branchInst->TrueTargetBasicBlock();
            Walk(trueTarget, reachable);
            intermediate::BasicBlock* falseTarget = branchInst->FalseTargetBasicBlock();
            Walk(falseTarget, reachable);
            break;
        }
        case cmajor::masm::intermediate::OpCode::switch_:
        {
            cmajor::masm::intermediate::SwitchInstruction* switchInst = static_cast<cmajor::masm::intermediate::SwitchInstruction*>(terminator);
            cmajor::masm::intermediate::BasicBlock* defaultBlock = switchInst->DefaultTargetBlock();
            Walk(defaultBlock, reachable);
            for (const auto& caseTarget : switchInst->CaseTargets())
            {
                cmajor::masm::intermediate::BasicBlock* caseBlock = caseTarget.targetBlock;
                Walk(caseBlock, reachable);
            }
            break;
        }
    }
}

void DeadCodeElimination(cmajor::masm::intermediate::Function* fn)
{
    std::set<cmajor::masm::intermediate::BasicBlock*> reachable;
    bool optimized = false;
    intermediate::BasicBlock* entry = fn->FirstBasicBlock();
    Walk(entry, reachable);
    cmajor::masm::intermediate::BasicBlock* bb = fn->FirstBasicBlock();
    while (bb)
    {
        cmajor::masm::intermediate::BasicBlock* next = bb->Next();
        if (reachable.find(bb) == reachable.end())
        {
            fn->RemoveBasicBlock(bb);
            optimized = true;
        }
        bb = next;
    }
    if (optimized)
    {
        fn->SetNumbers();
    }
}

} // cmajor::masm::optimizer
