// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef CMRT_WIN_ERROR_INCLUDED
#define CMRT_WIN_ERROR_INCLUDED
#include <stdint.h>
#include <string>
#include <Windows.h>
#include <gdiplus.h>

namespace cmrt::win {

void FatalError(const std::string& message);
void FatalGdiPlusError(Gdiplus::Status status);

}

extern "C" int WinAllocateWindowsError(const char* errorMessage, uint64_t errorCode);
extern "C" int WinAllocateGdiPlusError(const char* errorMessage, int gdiplusStatus);
extern "C" void WinFatalError(const char* errorMessage);
extern "C" void WinFatalGdiplusError(int gdiplusStatus);
extern "C" uint64_t WinGetLastError();
extern "C" void WinFormatMessage(uint64_t errorCode, char16_t* buffer);
extern "C" void WinFatalWindowsError(uint64_t errorCode);

#endif // CMRT_WIN_ERROR_INCLUDED
