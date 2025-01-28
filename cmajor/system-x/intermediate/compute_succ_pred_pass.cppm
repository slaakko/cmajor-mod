// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.compute.succ.pred.pass;

import cmajor.systemx.intermediate.pass;

export namespace cmajor::systemx::intermediate {

void ComputeSuccessorsAndPredecessors(Function& function);
void ClearSuccessorsAndPredecessors(Function& function);

class ComputeSuccPredPass : public FunctionPass
{
public:
    ComputeSuccPredPass();
    void Run(Function& function) override;
    void Clear(Function& function) override;
};

} // cmsx::intermediate
