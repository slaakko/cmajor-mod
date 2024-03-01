// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.module_pass_driver;

import cmajor.systemx.intermediate.pass;
import std.core;

export namespace cmajor::systemx::intermediate {

class ModulePassDriver
{
public:
    ModulePassDriver(Context* context_, const std::vector<ModulePass*>& passes_);
    void Run();
private:
    void InvalidatePasses(ModulePass* pass);
    Context* context;
    const std::vector<ModulePass*>& passes;
};

} // cmajor::systemx::intermediate
