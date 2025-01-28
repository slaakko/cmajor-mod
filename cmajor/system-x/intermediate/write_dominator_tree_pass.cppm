// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.write.dominator.tree.pass;

import cmajor.systemx.intermediate.pass;

export namespace cmajor::systemx::intermediate {

class WriteDominatorTreePass : public FunctionPass
{
public:
    WriteDominatorTreePass();
    void Run(Function& function) override;
};

} // cmajor::systemx::intermediate
