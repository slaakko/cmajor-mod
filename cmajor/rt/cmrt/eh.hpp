// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_EH_INCLUDED
#define CMRT_CPP_EH_INCLUDED

namespace cmajor::eh {

class Exception
{
};

} // namespace cmajor::eh

extern "C" void* RtmGetExceptionTypeId();
extern "C" void RtmThrowException(void* exception, void* exceptionTypeId);
extern "C" void RtmCaptureException(void** exception, void** exceptionTypeId);
extern "C" void RtmThrowCapturedException(void* exception, void* exceptionTypeId);
extern "C" bool RtmHandleException(void* exceptionTypeId);
extern "C" void* RtmGetException();

#endif // CMRT_CPP_EH_INCLUDED
