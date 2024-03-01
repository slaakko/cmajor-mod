// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.unwind;

import std.core;

using GlobalInitFunctionType = void(*)();

export extern "C" RT_API void RtSetGlobalInitFunction(GlobalInitFunctionType globalInitFunction_);
export extern "C" RT_API void RtBeginUnwindInfoInit();
export extern "C" RT_API void RtEndUnwindInfoInit();
export extern "C" RT_API void* RtPushUnwindInfo(void* unwindInfo);
export extern "C" RT_API void RtPopUnwindInfo(void* prevUnwindInfo);
export extern "C" RT_API void RtAddCompileUnitFunction(void* functionAddress, const char* functionName, const char* sourceFilePath);
export extern "C" RT_API const char* RtGetCallStack();
export extern "C" RT_API void RtDisposeCallStack();
