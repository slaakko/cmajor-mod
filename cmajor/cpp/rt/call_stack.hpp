// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CALL_STACK_HPP
#define CALL_STACK_HPP
#include <rt_api.hpp>
#include <stdint.h>

extern "C" RT_API void RtEnterFunction(const char* functionName, const char* sourceFilePath);
extern "C" RT_API void RtSetLineNumber(int32_t lineNumber);
extern "C" RT_API void RtExitFunction();
extern "C" RT_API void RtPrintCallStack(void* fileHandle);
extern "C" RT_API const char* RtGetStackTrace();
extern "C" RT_API void RtDisposeStackTrace();

#endif // CALL_STACK_HPP
