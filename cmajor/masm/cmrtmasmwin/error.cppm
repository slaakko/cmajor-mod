// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#include <gdiplus.h>

export module cmajor.masm.win.rt.error;

import std.core;

export namespace cmajor::masm::win::rt {

void FatalError(const std::string& message);
void FatalGdiPlusError(Gdiplus::Status status);

} // namespace cmajor::masm::win::rt

export {

extern "C" int WinAllocateWindowsError(const char* errorMessage, uint64_t errorCode);
extern "C" int WinAllocateGdiPlusError(const char* errorMessage, int gdiplusStatus);
extern "C" void WinFatalError(const char* errorMessage);
extern "C" void WinFatalGdiplusError(int gdiplusStatus);
extern "C" uint64_t WinGetLastError();
extern "C" void WinFormatMessage(uint64_t errorCode, char16_t* buffer);
extern "C" void WinFatalWindowsError(uint64_t errorCode);

}
