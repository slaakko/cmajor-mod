// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.unwind;

import std.core;

export extern "C" RT_API void* RtPushUnwindInfo(void* unwindInfo);
export extern "C" RT_API void RtPopUnwindInfo(void* prevUnwindInfo);
export extern "C" RT_API void RtAddCompileUnitFunction(void* functionAddress, const char* functionName, const char* sourceFilePath);
export extern "C" RT_API const char* RtGetCallStack();
export extern "C" RT_API void RtDisposeCallStack();

export void InitUnwind();
export void DoneUnwind();
