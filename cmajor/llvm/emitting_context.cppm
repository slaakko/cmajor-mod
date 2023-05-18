// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.emitting.context;

import std.core;
import cmajor.ir;

export namespace cmajor::llvm {

struct LLvmEmittingContextImpl;

class LLvmEmittingContext : public cmajor::ir::EmittingContext
{
public:
    LLvmEmittingContext(int optimizationLevel_);
    ~LLvmEmittingContext();
    void* NativeContext() override;
    int OptimizationLevel() const override { return optimizationLevel; }
    const std::string& TargetTripleStr() const override;
    void* Triple() const override;
    void* TargetMachine() override;
private:
    int optimizationLevel;
    LLvmEmittingContextImpl* impl;
};

} // cmajor::llvm
