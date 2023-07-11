// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CM_LLVM_API_INCLUDED
#define CM_LLVM_API_INCLUDED

#ifdef CMLLVM_EXPORTS
#define CMLLVM_API __declspec(dllexport)
#else
#define CMLLVM_API
#ifdef NDEBUG
#pragma comment(lib, "cmllvm.lib")
#else
#pragma comment(lib, "cmllvmd.lib")
#endif
#endif

#endif // CM_LLVM_API_INCLUDED
