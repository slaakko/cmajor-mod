// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.passes;

import cmajor.systemx.intermediate.pass.manager;
import cmajor.systemx.intermediate.compute.succ.pred.pass;
import cmajor.systemx.intermediate.simplify.cfg.pass;
import cmajor.systemx.intermediate.entry.exit.pass;
import cmajor.systemx.intermediate.dominator.tree.cooper;
import cmajor.systemx.intermediate.dominator.tree.pass;
import cmajor.systemx.intermediate.write.dominator.tree.pass;
import cmajor.systemx.intermediate.write.cfg.pass;
import cmajor.systemx.intermediate.write.pass;
import cmajor.systemx.intermediate.simple.assembly.codegen.pass;

namespace cmajor::systemx::intermediate {

void CreatePasses()
{
    PassManager::Instance().AddPass(new ComputeSuccPredPass());
    PassManager::Instance().AddPass(new SimplifyCfgPass());
    PassManager::Instance().AddPass(new EntryExitPass());
    PassManager::Instance().AddPass(new DominatorTreePass());
    PassManager::Instance().AddPass(new WriteDominatorTreePass());
    PassManager::Instance().AddPass(new WriteCfgPass());
    PassManager::Instance().AddPass(new WritePass());
    PassManager::Instance().AddPass(new SimpleAssemblyCodeGenPass());
}

} // cmajor::systemx::intermediate
