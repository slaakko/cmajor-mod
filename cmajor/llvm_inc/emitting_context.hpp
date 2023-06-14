// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef LLVM_INC_EMITTING_CONTEXT_INCLUDED
#define LLVM_INC_EMITTING_CONTEXT_INCLUDED
#include <string>

namespace llvm_inc {

struct LLvmEmittingContextImpl;

class LLvmEmittingContext
{
public:
    LLvmEmittingContext(int optimizationLevel_);
    ~LLvmEmittingContext();
    int OptimizationLevel() const { return optimizationLevel; }
    const std::string& TargetTripleStr() const;
    void* Triple() const;
    void* TargetMachine();
    void* DataLayout() const;
private:
    int optimizationLevel;
    LLvmEmittingContextImpl* impl;
};

} // namespace llvm_inc

#endif // LLVM_INC_EMITTING_CONTEXT_INCLUDED
