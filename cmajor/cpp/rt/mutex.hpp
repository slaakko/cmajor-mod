// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef MUTEX_HPP
#define MUTEX_HPP
#include <rt_api.hpp>

extern "C" RT_API void* RtAllocateMutex();
extern "C" RT_API void RtFreeMutex(void* nativeHandle);
extern "C" RT_API void RtLockMutex(void* nativeHandle);
extern "C" RT_API void RtUnlockMutex(void* nativeHandle);

extern "C" RT_API void* RtAllocateRecursiveMutex();
extern "C" RT_API void RtFreeRecursiveMutex(void* nativeHandle);
extern "C" RT_API void RtLockRecursiveMutex(void* nativeHandle);
extern "C" RT_API void RtUnlockRecursiveMutex(void* nativeHandle);

#endif // MUTEX_HPP
