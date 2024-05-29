// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/eh.hpp>
#include <cmrt/classes.hpp>
#include <util/uuid.hpp>
#include <typeinfo>

thread_local void* currentException = nullptr;
thread_local void* currentExceptionTypeId;

void* RtmGetExceptionTypeId()
{
    const std::type_info& exceptionTypeId = typeid(cmajor::eh::Exception);
    const void* ti = &exceptionTypeId;
    return const_cast<void*>(ti);
}

void RtmThrowException(void* exception, void* exceptionTypeId)
{
    currentException = exception;
    currentExceptionTypeId = exceptionTypeId;
    throw cmajor::eh::Exception();
}

void RtmCaptureException(void** exception, void** exceptionTypeId)
{
    *exception = currentException;
    *exceptionTypeId = currentExceptionTypeId;
}

void RtmThrowCapturedException(void* exception, void* exceptionTypeId)
{
    currentException = exception;
    currentExceptionTypeId = exceptionTypeId;
    throw cmajor::eh::Exception();
}

bool RtmHandleException(void* exceptionTypeId)
{
    return RtmIs(currentExceptionTypeId, exceptionTypeId);
}

void* RtmGetException()
{
    return currentException;
}
