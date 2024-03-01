// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.simplify.cfg.pass;

import cmajor.systemx.intermediate.pass;

export namespace cmajor::systemx::intermediate {

void Simplify(Function& function);

class SimplifyCfgPass : public FunctionPass
{
public:
    SimplifyCfgPass();
    void Run(Function& function) override;
};

} // cmajor::systemx::intermediate
