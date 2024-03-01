// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.error;

import std.core;

export const int exitCodeAccessViolation = 255;
export const int exitCodeInternalError = 254;
export const int exitCodeAssertionFailed = 253;
export const int exitCodeOutOfMemory = 252;
export const int exitCodeExceptionEscapedFromThread = 251;
export const int exitCodeSocketInitializationFailed = 250;

export extern "C" RT_API const char* RtGetError(int32_t errorId);
export extern "C" RT_API void RtDisposeError(int32_t errorId);
export extern "C" RT_API void RtThrowException(void* exception, void* exceptionTypeId);
export extern "C" RT_API void RtCaptureException(void** exception, uint64_t & exceptionClassIdHi, uint64_t & exceptionClassIdLo);
export extern "C" RT_API void RtThrowCapturedException(void* exception, uint64_t exceptionClassIdHi, uint64_t exceptionClassIdLo);
export extern "C" RT_API bool RtHandleException(void* exceptionTypeId);
export extern "C" RT_API void* RtGetException();
export extern "C" RT_API void* RtGetExceptionTypeId();

export namespace cmajor::rt {

int32_t InstallError(const std::string& errorMessage);
const char* GetError(int32_t errorId);
void DisposeError(int32_t errorId);

} // namespace cmajor::rt
