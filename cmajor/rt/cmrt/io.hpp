// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_IO_INCLUDED
#define CMRT_IO_INCLUDED
#include <stdint.h>

namespace cmajor::rt {

const int stdInFileHandle = 0;
const int stdOutFileHandle = 1;
const int stdErrFileHandle = 2;

enum class OpenMode : uint8_t
{
    none = 0,
    read = 1 << 0,
    write = 1 << 1,
    append = 1 << 2,
    binary = 1 << 3
};

enum class Origin : uint8_t
{
    seekSet, seekCur, seekEnd
};

inline OpenMode operator&(OpenMode left, OpenMode right)
{
    return OpenMode(uint8_t(left) & uint8_t(right));
}

inline OpenMode operator|(OpenMode left, OpenMode right)
{
    return OpenMode(uint8_t(left) | uint8_t(right));
}

void InitIO();
void DoneIO();

} // cmajor::rt

extern "C" void* RtmOpenFile(const char* filePath, cmajor::rt::OpenMode openMode, int32_t & errorId);
extern "C" void* RtmOpenStdFile(int handle, int32_t & errorId);
extern "C" void RtmPrintToFile(void* fileHandle, const char* s);
extern "C" bool RtmClose(void* fileHandle, int32_t & errorId);
extern "C" bool RtmDisposeFile(void* fileHandle, int32_t & errorId);
extern "C" int64_t RtmWrite(void* fileHandle, const uint8_t * buffer, int64_t count, int32_t & errorId);
extern "C" bool RtmWriteByte(void* fileHandle, uint8_t x, int32_t & errorId);
extern "C" int64_t RtmRead(void* fileHandle, uint8_t * buffer, int64_t bufferSize, int32_t & errorId);
extern "C" int32_t RtmReadByte(void* fileHandle, int32_t & errorId);
extern "C" bool RtmEof(void* fileHandle);
extern "C" bool RtmGetFileError(void* fileHandle, int32_t & errorId);
extern "C" bool RtmSeek(void* fileHandle, int64_t pos, cmajor::rt::Origin origin, int32_t & errorId);
extern "C" int64_t RtmTell(void* fileHandle, int32_t & errorId);
extern "C" bool RtmFlush(void* fileHandle, int32_t & errorId);
extern "C" bool RtmFileExists(const char* filePath, int32_t & errorId);
extern "C" bool RtmLastWriteTimeLess(const char* filePath1, const char* filePath2, int32_t & errorId);
extern "C" int64_t RtmGetFileSize(const char* filePath, int32_t & errorId);
extern "C" bool RtmRemoveFile(const char* filePath, int32_t & errorId);
extern "C" bool RtmCopyFile(const char* sourceFilePath, const char* targetFilePath, int32_t & errorId);
extern "C" bool RtmMoveFile(const char* sourceFilePath, const char* targetFilePath, int32_t & errorId);
extern "C" bool RtmIsConsoleHandle(int handle);
extern "C" int RtmGetCurrentWorkingDirectoryHandle(int32_t & errorId);
extern "C" const char* RtmGetCurrentWorkingDirectory(int handle);
extern "C" void RtmFreeCurrentWorkingDirectoryHandle(int handle);
extern "C" int RtmBeginGetPathToExecutable();
extern "C" const char* RtmGetPathToExecutable(int pathHandle);
extern "C" void RtmEndGetPathToExecutable(int pathHandle);

#endif // CMRT_IO_INCLUDED
