// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef ERROR_HPP
#define ERROR_HPP
#include <rt_api.hpp>
#include <string>
#include <stdint.h>

const int exitCodeAccessViolation = 255;
const int exitCodeInternalError = 254;
const int exitCodeAssertionFailed = 253;
const int exitCodeOutOfMemory = 252;
const int exitCodeExceptionEscapedFromThread = 251;
const int exitCodeSocketInitializationFailed = 250;

extern "C" RT_API const char* RtGetError(int32_t errorId);
extern "C" RT_API void RtDisposeError(int32_t errorId);
extern "C" RT_API void RtThrowException(void* exception, void* exceptionTypeId);
extern "C" RT_API void RtCaptureException(void** exception, uint64_t & exceptionClassIdHi, uint64_t & exceptionClassIdLo);
extern "C" RT_API void RtThrowCapturedException(void* exception, uint64_t exceptionClassIdHi, uint64_t exceptionClassIdLo);
extern "C" RT_API bool RtHandleException(void* exceptionTypeId);
extern "C" RT_API void* RtGetException();
extern "C" RT_API void* RtGetExceptionTypeId();

namespace cmajor::rt {

int32_t InstallError(const std::string& errorMessage);
const char* GetError(int32_t errorId);
void DisposeError(int32_t errorId);

} // namespace cmajor::rt

#endif // ERROR_HPP
