// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.function.pass.driver;

import cmajor.systemx.intermediate.pass;
import cmajor.systemx.intermediate.visitor;
import std.core;

export namespace cmajor::systemx::intermediate {

class FunctionPassDriver : public Visitor
{
public:
    FunctionPassDriver(Context* context_, const std::vector<FunctionPass*>& functionPasses_);
    void Visit(Function& function);
private:
    void InvalidatePasses(FunctionPass* pass);
    const std::vector<FunctionPass*>& functionPasses;
};

} // cmajor::systemx::intermediate
