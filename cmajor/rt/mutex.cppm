// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.mutex;

import std.core;

export extern "C" RT_API void* RtAllocateMutex();
export extern "C" RT_API void RtFreeMutex(void* nativeHandle);
export extern "C" RT_API void RtLockMutex(void* nativeHandle);
export extern "C" RT_API void RtUnlockMutex(void* nativeHandle);

export extern "C" RT_API void* RtAllocateRecursiveMutex();
export extern "C" RT_API void RtFreeRecursiveMutex(void* nativeHandle);
export extern "C" RT_API void RtLockRecursiveMutex(void* nativeHandle);
export extern "C" RT_API void RtUnlockRecursiveMutex(void* nativeHandle);
