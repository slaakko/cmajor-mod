// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMLLVM_LLVM_EMITTING_CONTEXT
#define CMLLVM_LLVM_EMITTING_CONTEXT
#include <cmllvm/emitting_context.hpp>

namespace cmllvm {

struct LLVMEmittingContextImpl;

class LLVMEmittingContext: public EmittingContext
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

#endif // CMLLVM_LLVM_EMITTING_CONTEXT
