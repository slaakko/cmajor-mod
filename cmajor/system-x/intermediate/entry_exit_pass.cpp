// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.entry.exit.pass;

import cmajor.systemx.intermediate.code;

namespace cmajor::systemx::intermediate {

EntryExitPass::EntryExitPass() : FunctionPass("entry-exit")
{
}

void EntryExitPass::Run(Function& function)
{
    function.AddEntryAndExitBlocks();
}

void EntryExitPass::Clear(Function& function)
{
    function.RemoveEntryAndExitBlocks();
}

} // cmajor::systemx::intermediate
