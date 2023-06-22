// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP
#include <rt_api.hpp>
#include <stdint.h>

namespace cmajor::rt {

void InitDirectory();
void DoneDirectory();

} // namespace cmajor::rt

extern "C" RT_API bool RtDirectoryExists(const char* directoryPath);
extern "C" RT_API void RtCreateDirectories(const char* directoryPath);
extern "C" RT_API int32_t RtBeginIterateDirectory(const char* directoryPath);
extern "C" RT_API const char* RtGetNextFilePath(int32_t directoryIterationHandle);
extern "C" RT_API const char* RtGetNextDirectoryPath(int32_t directoryIterationHandle);
extern "C" RT_API void RtEndIterateDirectory(int32_t directoryIterationHandle);

#endif // DIRECTORY_HPP
