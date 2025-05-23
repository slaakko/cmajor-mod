// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.simplify.cfg.pass;

import cmajor.systemx.intermediate.code;
import cmajor.systemx.intermediate.util;

namespace cmajor::systemx::intermediate {

void Visit(BasicBlock* block, std::set<BasicBlock*>& visited)
{
    visited.insert(block);
    for (BasicBlock* succ : block->Successors())
    {
        if (visited.find(succ) == visited.cend())
        {
            Visit(succ, visited);
        }
    }
}

void RemoveUnreachableBlocks(Function& function)
{
    std::vector<BasicBlock*> toRemove;
    std::set<BasicBlock*> visited;
    Visit(function.FirstBasicBlock(), visited);
    BasicBlock* block = function.FirstBasicBlock();
    while (block)
    {
        if (visited.find(block) == visited.cend())
        {
            AddPtrToSet(block, toRemove);
        }
        block = block->Next();
    }
    for (BasicBlock* block : toRemove)
    {
        function.RemoveBasicBlock(block);
    }
}

void Simplify(Function& function)
{
    RemoveUnreachableBlocks(function);
}

SimplifyCfgPass::SimplifyCfgPass() : FunctionPass("simplify-cfg")
{
}

void SimplifyCfgPass::Run(Function& function)
{
    Simplify(function);
}

} // cmajor::systemx::intermediate
