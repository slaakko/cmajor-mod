// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_MUTEX_INCLUDED
#define CMRT_CPP_MUTEX_INCLUDED

extern "C" void* RtmAllocateMutex();
extern "C" void RtmFreeMutex(void* mutexHandle);
extern "C" void* RtmAllocateRecursiveMutex();
extern "C" void RtmFreeRecursiveMutex(void* recursiveMutexHandle);
extern "C" void RtmLockMutex(void* mutexHandle);
extern "C" void RtmUnlockMutex(void* mutexHandle);
extern "C" void RtmLockRecursiveMutex(void* recursiveMutexHandle);
extern "C" void RtmUnlockRecursiveMutex(void* recursiveMutexHandle);

#endif // CMRT_CPP_MUTEX_INCLUDED
