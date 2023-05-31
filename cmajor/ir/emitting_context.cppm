// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ir.emitting.context;

import std.core;

export namespace cmajor::ir {

class EmittingContext
{
public:
    virtual ~EmittingContext();
    virtual void* NativeContext() = 0;
    virtual int OptimizationLevel() const = 0;
    virtual const std::string& TargetTripleStr() const = 0;
    virtual void* Triple() const = 0;
    virtual void* TargetMachine() = 0;
    virtual void* DataLayout() const = 0;
};

} // namespace cmajor::ir
