// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.write.cfg.pass;

import cmajor.systemx.intermediate.pass;
import std.core;

export namespace cmajor::systemx::intermediate {

class WriteCfgPass : public FunctionPass
{
public:
    WriteCfgPass();
    void Run(Function& function) override;
};

} // cmajor::systemx::intermediate
