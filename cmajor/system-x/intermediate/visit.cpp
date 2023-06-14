// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.visit;
import cmajor.systemx.intermediate.code;

namespace cmajor::systemx::intermediate {

void PreorderVisit(BasicBlock* block, std::set<BasicBlock*>& visited, std::vector<BasicBlock*>& preorder)
{
    preorder.push_back(block);
    visited.insert(block);
    for (BasicBlock* succ : block->Successors())
    {
        if (visited.find(succ) == visited.cend())
        {
            PreorderVisit(succ, visited, preorder);
        }
    }
}

std::vector<BasicBlock*> Preorder(Function& function)
{
    std::vector<BasicBlock*> preorder;
    std::set<BasicBlock*> visited;
    PreorderVisit(function.FirstBasicBlock(), visited, preorder);
    return preorder;
}

void PostorderVisit(BasicBlock* block, std::set<BasicBlock*>& visited, std::vector<BasicBlock*>& postorder)
{
    visited.insert(block);
    for (BasicBlock* succ : block->Successors())
    {
        if (visited.find(succ) == visited.cend())
        {
            PostorderVisit(succ, visited, postorder);
        }
    }
    postorder.push_back(block);
}

std::vector<BasicBlock*> Postorder(Function& function)
{
    std::vector<BasicBlock*> postorder;
    std::set<BasicBlock*> visited;
    PostorderVisit(function.FirstBasicBlock(), visited, postorder);
    return postorder;
}

std::vector<BasicBlock*> ReversePostorder(Function& function)
{
    std::vector<BasicBlock*> reversePostorder = Postorder(function);
    std::reverse(reversePostorder.begin(), reversePostorder.end());
    return reversePostorder;
}

} // cmajor::systemx::intermediate
