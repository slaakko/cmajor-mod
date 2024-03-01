// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.write.dominator.tree.pass;

import cmajor.systemx.intermediate.dominator.tree.pass;
import cmajor.systemx.intermediate.pass.manager;
import cmajor.systemx.intermediate.dominator.tree.cooper;
import cmajor.systemx.intermediate.context;
import util;

namespace cmajor::systemx::intermediate {

WriteDominatorTreePass::WriteDominatorTreePass() : FunctionPass("write-dominator-tree")
{
}

void WriteDominatorTreePass::Run(Function& function)
{
    Pass* pass = PassManager::Instance().GetPass("dominator-tree");
    DominatorTreePass* dominatorTreePass = static_cast<DominatorTreePass*>(pass);
    DominatorTreeCooper* dominatorTree = dominatorTreePass->GetDominatorTree();
    if (dominatorTree->GetFunction() != &function)
    {
        throw std::runtime_error("invalid pass depencency");
    }
    std::string libDir = util::Path::GetDirectoryName(function.Parent()->GetContext()->FilePath());
    std::string dominatorTreeXmlFilePath = util::Path::Combine(libDir, function.Name() + ".dominator.tree.xml");
    dominatorTree->WriteXmlDocument(dominatorTreeXmlFilePath);
}

} // cmajor::systemx::intermediate
