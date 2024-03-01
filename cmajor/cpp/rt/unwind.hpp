// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UNWIND_HPP
#define UNWIND_HPP
#include <rt_api.hpp>

using GlobalInitFunctionType = void(*)();

extern "C" RT_API void RtSetGlobalInitFunction(GlobalInitFunctionType globalInitFunction_);
extern "C" RT_API void RtBeginUnwindInfoInit();
extern "C" RT_API void RtEndUnwindInfoInit();
extern "C" RT_API void* RtPushUnwindInfo(void* unwindInfo);
extern "C" RT_API void RtPopUnwindInfo(void* prevUnwindInfo);
extern "C" RT_API void RtAddCompileUnitFunction(void* functionAddress, const char* functionName, const char* sourceFilePath);
extern "C" RT_API const char* RtGetCallStack();
extern "C" RT_API void RtDisposeCallStack();

#endif // UNWIND_HPP
