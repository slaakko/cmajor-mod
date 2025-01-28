// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/mutex.hpp>
#include <mutex>

void* RtmAllocateMutex()
{
    return new std::mutex();
}

void RtmFreeMutex(void* mutexHandle)
{
    delete static_cast<std::mutex*>(mutexHandle);
}

void* RtmAllocateRecursiveMutex()
{
    return new std::recursive_mutex();
}

void RtmFreeRecursiveMutex(void* recursiveMutexHandle)
{
    delete static_cast<std::recursive_mutex*>(recursiveMutexHandle);
}

void RtmLockMutex(void* mutexHandle)
{
    static_cast<std::mutex*>(mutexHandle)->lock();
}

void RtmUnlockMutex(void* mutexHandle)
{
    static_cast<std::mutex*>(mutexHandle)->unlock();
}

void RtmLockRecursiveMutex(void* recursiveMutexHandle)
{
    static_cast<std::recursive_mutex*>(recursiveMutexHandle)->lock();
}

void RtmUnlockRecursiveMutex(void* recursiveMutexHandle)
{
    static_cast<std::recursive_mutex*>(recursiveMutexHandle)->unlock();
}
