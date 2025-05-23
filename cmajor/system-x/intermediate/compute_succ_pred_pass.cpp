// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.compute.succ.pred.pass;

import cmajor.systemx.intermediate.code;

namespace cmajor::systemx::intermediate {

void ComputeSuccessorsAndPredecessors(Function& function)
{
    BasicBlock* block = function.FirstBasicBlock();
    while (block)
    {
        std::vector<BasicBlock*> successors = block->LastInstruction()->Successors();
        for (BasicBlock* successor : successors)
        {
            block->AddSuccessor(successor);
            successor->AddPredecessor(block);
        }
        block = block->Next();
    }
}

void ClearSuccessorsAndPredecessors(Function& function)
{
    BasicBlock* block = function.FirstBasicBlock();
    while (block)
    {
        block->ClearSuccessorsAndPredecessors();
        block = block->Next();
    }
}

ComputeSuccPredPass::ComputeSuccPredPass() : FunctionPass("compute-succ-pred")
{
}

void ComputeSuccPredPass::Run(Function& function)
{
    ComputeSuccessorsAndPredecessors(function);
}

void ComputeSuccPredPass::Clear(Function& function)
{
    ClearSuccessorsAndPredecessors(function);
}

} // cmajor::systemx::intermediate
