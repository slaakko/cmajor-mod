// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.write.pass;

import cmajor.systemx.intermediate.pass;

export namespace cmajor::systemx::intermediate {

class WritePass : public ModulePass
{
public:
    WritePass();
    void Run(Context* context) override;
};

} // cmajor::systemx::intermediate
