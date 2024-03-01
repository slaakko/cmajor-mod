// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CONDITION_VARIABLE_HPP
#define CONDITION_VARIABLE_HPP
#include <rt_api.hpp>
#include <stdint.h>

extern "C" RT_API void* RtAllocateConditionVariable();
extern "C" RT_API void RtFreeConditionVariable(void* nativeHandle);
extern "C" RT_API int32_t RtNotifyOne(void* nativeHandle);
extern "C" RT_API int32_t RtNotifyAll(void* nativeHandle);
extern "C" RT_API int32_t RtWaitConditionVariable(void* nativeHandle, void* recursiveMutexHandle);
extern "C" RT_API int32_t RtWaitConditionVariableDuration(void* nativeHandle, void* recursiveMutexHandle, int64_t nanoseconds);

#endif // CONDITION_VARIABLE_HPP
