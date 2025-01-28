// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_ERROR_INCLUDED
#define UTIL_ERROR_INCLUDED
#include <stdexcept>
#include <stdint.h>
#include <string>

namespace util {

class WindowsException : public std::runtime_error
{
public:
    WindowsException(uint64_t errorCode_);
    uint64_t ErrorCode() const { return errorCode; }
    std::string ErrorMessage() const { return what(); }
private:
    uint64_t errorCode;
};

} // util

#endif // UTIL_ERROR_INCLUDED
