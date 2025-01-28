// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.init.done;

import cmajor.systemx.intermediate.pass.manager;
import cmajor.systemx.intermediate.reg.allocator;

namespace cmajor::systemx::intermediate {

void Init()
{
    InitPassManager();
}

void Done()
{
    DonePassManager();
}

} // cmajor::systemx::intermediate
