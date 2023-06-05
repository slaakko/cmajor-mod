// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.process;

import std.core;

export extern "C" RT_API void* RtCreateProcess(const char* command, int32_t redirections, int32_t & errorStringHandle);
export extern "C" RT_API void RtDeleteProcess(void* process);
export extern "C" RT_API const char* RtGetString(int32_t stringHandle);
export extern "C" RT_API void RtDisposeString(int32_t stringHandle);
export extern "C" RT_API bool RtProcessRunning(void* process, int32_t & errorStringHandle);
export extern "C" RT_API bool RtProcessWaitForExit(void* process, int32_t & errorStringHandle);
export extern "C" RT_API int RtProcessExitCode(void* process, int32_t & errorStringHandle);
export extern "C" RT_API bool RtProcessTerminate(void* process, int32_t & errorStringHandle);
export extern "C" RT_API bool RtProcessEof(void* process, int handle, int32_t & errorStringHandle);
export extern "C" RT_API int32_t RtProcessReadLine(void* process, int handle, int32_t & errorStringHandle);
export extern "C" RT_API int32_t RtProcessReadToEnd(void* process, int handle, int32_t & errorStringHandle);
export extern "C" RT_API bool RtProcessWriteLine(void* process, const char* line, int32_t & errorStrHandle);
