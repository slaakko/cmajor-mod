// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.rt.directory;

import std.core;

export {

extern "C" bool RtmDirectoryExists(const char* directoryPath);
extern "C" void RtmCreateDirectories(const char* directoryPath);
extern "C" int32_t RtmBeginIterateDirectory(const char* directoryPath);
extern "C" const char* RtmGetNextFilePath(int32_t directoryIterationHandle);
extern "C" const char* RtmGetNextDirectoryPath(int32_t directoryIterationHandle);
extern "C" void RtmEndIterateDirectory(int32_t directoryIterationHandle);

}