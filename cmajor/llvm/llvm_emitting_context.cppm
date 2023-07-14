// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.emitting.context;

import cmajor.ir.emitting.context;
import std.core;

export namespace cmllvm {

struct LLVMEmittingContextImpl;

class LLVMEmittingContext : public cmajor::ir::EmittingContext
{
public:
    LLVMEmittingContext(int optimizationLevel_);
    ~LLVMEmittingContext();
    int OptimizationLevel() const override { return optimizationLevel; }
    const std::string& TargetTripleStr() const override;
    void* Triple() const override;
    void* TargetMachine() override;
    void* DataLayout() const override;
private:
    int optimizationLevel;
    LLVMEmittingContextImpl* impl;
};

} // namespace cmllvm
