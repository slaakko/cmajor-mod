// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/condition_variable.hpp>
#include <cmrt/error.hpp>
#include <condition_variable>
#include <mutex>

void* RtmAllocateConditionVariable()
{
    return new std::condition_variable_any();
}

void RtmFreeConditionVariable(void* nativeHandle)
{
    if (nativeHandle)
    {
        delete static_cast<std::condition_variable_any*>(nativeHandle);
    }
}

bool RtmNotifyOne(void* nativeHandle, int32_t& errorId)
{
    try
    {
        errorId = 0;
        std::condition_variable_any* conditionVariable = static_cast<std::condition_variable_any*>(nativeHandle);
        conditionVariable->notify_one();
        return true;
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return false;
    }
}

bool RtmNotifyAll(void* nativeHandle, int32_t& errorId)
{
    try
    {
        errorId = 0;
        std::condition_variable_any* conditionVariable = static_cast<std::condition_variable_any*>(nativeHandle);
        conditionVariable->notify_all();
        return true;
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return false;
    }
}

bool RtmWaitConditionVariable(void* nativeHandle, void* recursiveMutexHandle, int32_t& errorId)
{
    try
    {
        errorId = 0;
        std::recursive_mutex* recursiveMutex = static_cast<std::recursive_mutex*>(recursiveMutexHandle);
        std::condition_variable_any* conditionVariable = static_cast<std::condition_variable_any*>(nativeHandle);
        std::unique_lock<std::recursive_mutex> lock(*recursiveMutex);
        conditionVariable->wait(lock);
        return true;
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return false;
    }
}

enum class CondVarStatus : int32_t
{
    timeout = 0, no_timeout = 1
};

int RtmWaitConditionVariableDuration(void* nativeHandle, void* recursiveMutexHandle, int64_t nanoseconds, int32_t& errorId)
{
    try
    {
        errorId = 0;
        CondVarStatus status = CondVarStatus::timeout;
        std::recursive_mutex* recursiveMutex = static_cast<std::recursive_mutex*>(recursiveMutexHandle);
        std::condition_variable_any* conditionVariable = static_cast<std::condition_variable_any*>(nativeHandle);
        std::chrono::nanoseconds duration(nanoseconds);
        std::unique_lock<std::recursive_mutex> lock(*recursiveMutex);
        if (conditionVariable->wait_for(lock, duration) == std::cv_status::no_timeout)
        {
            status = CondVarStatus::no_timeout;
        }
        return static_cast<int>(status);
    }
    catch (const std::exception& ex)
    {
        errorId = cmajor::rt::AllocateError(ex.what());
        return 0;
    }
}
