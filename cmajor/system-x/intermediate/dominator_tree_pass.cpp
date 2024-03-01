// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.dominator.tree.pass;

import cmajor.systemx.intermediate.dominator.tree.cooper;

namespace cmajor::systemx::intermediate {

DominatorTreePass::DominatorTreePass() : FunctionPass("dominator-tree")
{
}

void DominatorTreePass::Run(Function& function)
{
    dominatorTree.reset(new DominatorTreeCooper(function));
    dominatorTree->Compute();
}

void DominatorTreePass::Clear(Function& function)
{
    dominatorTree.reset();
}

} // cmajor::systemx::intermediate
