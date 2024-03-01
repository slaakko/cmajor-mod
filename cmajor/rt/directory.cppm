// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <rt_api.hpp>

export module cmajor.rt.directory;

import std.core;

export namespace cmajor::rt {

void InitDirectory();
void DoneDirectory();

} // namespace cmajor::rt

export extern "C" RT_API bool RtDirectoryExists(const char* directoryPath);
export extern "C" RT_API void RtCreateDirectories(const char* directoryPath);
export extern "C" RT_API int32_t RtBeginIterateDirectory(const char* directoryPath);
export extern "C" RT_API const char* RtGetNextFilePath(int32_t directoryIterationHandle);
export extern "C" RT_API const char* RtGetNextDirectoryPath(int32_t directoryIterationHandle);
export extern "C" RT_API void RtEndIterateDirectory(int32_t directoryIterationHandle);
