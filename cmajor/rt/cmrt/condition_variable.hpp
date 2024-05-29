// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CONDITION_VARIABLE_INCLUDED
#define CMRT_CONDITION_VARIABLE_INCLUDED
#include <stdint.h>

extern "C" void* RtmAllocateConditionVariable();
extern "C" void RtmFreeConditionVariable(void* nativeHandle);
extern "C" bool RtmNotifyOne(void* nativeHandle, int32_t & errorId);
extern "C" bool RtmNotifyAll(void* nativeHandle, int32_t & errorId);
extern "C" bool RtmWaitConditionVariable(void* nativeHandle, void* recursiveMutexHandle, int32_t & errorId);
extern "C" int RtmWaitConditionVariableDuration(void* nativeHandle, void* recursiveMutexHandle, int64_t nanoseconds, int32_t & errorId);

#endif // CMRT_CONDITION_VARIABLE_INCLUDED
