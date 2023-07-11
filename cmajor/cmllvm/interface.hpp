// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMLLVM_INTERFACE_INCLUDED
#define CMLLVM_INTERFACE_INCLUDED
#include <cmllvm/api.hpp>
#include <cmllvm/emitting_context.hpp>

namespace cmllvm {

CMLLVM_API void* CreateLLVMEmittingContext(int optimizationLevel);
CMLLVM_API void* CreateLLVMEmitter(EmittingContext* emittingContext);

} // namespace cmllvm

#endif // CMLLVM_INTERFACE_INCLUDED
