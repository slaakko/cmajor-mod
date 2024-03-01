// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.environment;

import std.core;

export namespace cmajor::rt {

void InitEnvironment();
void DoneEnvironment();

} // namespace cmajor::rt

export extern "C" RT_API const char* RtGetEnvironmentVariable(const char* environmentVariableName);
export extern "C" RT_API int32_t RtGetCurrentWorkingDirectoryHandle();
export extern "C" RT_API const char* RtGetCurrentWorkingDirectory(int32_t currentWorkingDirectoryHandle);
export extern "C" RT_API void RtFreeCurrentWorkingDirectoryHandle(int32_t currentWorkingDirectoryHandle);
export extern "C" RT_API int32_t RtExecute(const char* command);
export extern "C" RT_API int32_t RtBeginExec(const char* command);
export extern "C" RT_API void RtAddRedirection(int32_t execHandle, int handle, const char* file);
export extern "C" RT_API int32_t RtEndExec(int32_t execHandle);
export extern "C" RT_API int32_t RtBeginGetPathToExecutable();
export extern "C" RT_API const char* RtGetPathToExecutable(int32_t pathHandle);
export extern "C" RT_API void RtEndGetPathToExecutable(int32_t pathHandle);
export extern "C" RT_API const char* RtGetOsInfo();
