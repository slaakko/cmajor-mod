// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.call.stack;

import std.core;

export extern "C" RT_API void RtEnterFunction(const char* functionName, const char* sourceFilePath);
export extern "C" RT_API void RtSetLineNumber(int32_t lineNumber);
export extern "C" RT_API void RtExitFunction();
export extern "C" RT_API void RtPrintCallStack(void* fileHandle);
export extern "C" RT_API const char* RtGetStackTrace();
export extern "C" RT_API void RtDisposeStackTrace();
