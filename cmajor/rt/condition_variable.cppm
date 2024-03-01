// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.condition.variable;

import std.core;

export extern "C" RT_API void* RtAllocateConditionVariable();
export extern "C" RT_API void RtFreeConditionVariable(void* nativeHandle);
export extern "C" RT_API int32_t RtNotifyOne(void* nativeHandle);
export extern "C" RT_API int32_t RtNotifyAll(void* nativeHandle);
export extern "C" RT_API int32_t RtWaitConditionVariable(void* nativeHandle, void* recursiveMutexHandle);
export extern "C" RT_API int32_t RtWaitConditionVariableDuration(void* nativeHandle, void* recursiveMutexHandle, int64_t nanoseconds);
