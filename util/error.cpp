// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module util.error;

import util.unicode;

namespace util {

std::string WindowsErrorMessage(uint64_t errorCode)
{
    char16_t buf[4096];
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode, LANG_SYSTEM_DEFAULT, (LPWSTR)buf, 4096, nullptr);
    return ToUtf8(buf);
}

WindowsException::WindowsException(uint64_t errorCode_) : std::runtime_error(WindowsErrorMessage(errorCode_)), errorCode(errorCode_)
{
}

} // util
