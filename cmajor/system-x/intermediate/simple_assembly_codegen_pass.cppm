// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.simple.assembly.codegen.pass;

import cmajor.systemx.intermediate.pass;

export namespace cmajor::systemx::intermediate {

class SimpleAssemblyCodeGenPass : public ModulePass
{
public:
    SimpleAssemblyCodeGenPass();
    void Run(Context* context) override;
};

} // cmajor::systemx::intermediate
