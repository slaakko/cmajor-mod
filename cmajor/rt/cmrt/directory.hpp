// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_CPP_DIRECTORY_INCLUDED
#define CMRT_CPP_DIRECTORY_INCLUDED
#include <stdint.h>

extern "C" bool RtmDirectoryExists(const char* directoryPath);
extern "C" void RtmCreateDirectories(const char* directoryPath);
extern "C" int32_t RtmBeginIterateDirectory(const char* directoryPath);
extern "C" const char* RtmGetNextFilePath(int32_t directoryIterationHandle);
extern "C" const char* RtmGetNextDirectoryPath(int32_t directoryIterationHandle);
extern "C" void RtmEndIterateDirectory(int32_t directoryIterationHandle);

#endif // CMRT_CPP_DIRECTORY_INCLUDED
