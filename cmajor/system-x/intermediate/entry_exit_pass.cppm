// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.entry.exit.pass;

import cmajor.systemx.intermediate.pass;
import std.core;

export namespace cmajor::systemx::intermediate {

class EntryExitPass : public FunctionPass
{
public:
    EntryExitPass();
    void Run(Function& function) override;
    void Clear(Function& function) override;
};

} // cmajor::systemx::intermediate
