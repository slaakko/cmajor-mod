// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.pass;

import std.core;

export namespace cmajor::systemx::intermediate {

class Function;
class Context;

enum class PassKind
{
    function, module, composite
};

class Pass
{
public:
    Pass(const std::string& name_, PassKind kind_);
    virtual ~Pass();
    PassKind Kind() const { return kind; }
    const std::string& Name() const { return name; }
    const std::set<Pass*>& RequiredPasses() const { return requiredPasses; }
    const std::set<Pass*>& InvalidatedPasses() const { return invalidatedPasses; }
    void AddRequiredPass(Pass* pass);
    void AddInvalidatedPass(Pass* pass);
    bool Valid() const { return valid; }
    void SetValid() { valid = true; }
    void ResetValid() { valid = false; }
private:
    std::string name;
    PassKind kind;
    bool valid;
    std::set<Pass*> requiredPasses;
    std::set<Pass*> invalidatedPasses;
};

class FunctionPass : public Pass
{
public:
    FunctionPass(const std::string& name_);
    virtual void Run(Function& function);
    virtual void Clear(Function& function);
};

class ModulePass : public Pass
{
public:
    ModulePass(const std::string& name_);
    ModulePass(const std::string& name_, PassKind kind_);
    virtual void Run(Context* context);
    virtual void Clear(Context* context);
};

class CompositePass : public ModulePass
{
public:
    CompositePass(const std::string& name_);
};

} // cmajor::systemx::intermediate
