// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CM_LLVM_EMITTING_CONTEXT_INCLUDED
#define CM_LLVM_EMITTING_CONTEXT_INCLUDED
#include <cmllvm/api.hpp>
#include <string>

namespace cmllvm {

class CMLLVM_API EmittingContext
{
public:
    virtual ~EmittingContext();
    virtual int OptimizationLevel() const = 0;
    virtual const std::string& TargetTripleStr() const = 0;
    virtual void* Triple() const = 0;
    virtual void* TargetMachine() = 0;
    virtual void* DataLayout() const = 0;
};

} // namespace cmllvm

#endif 
