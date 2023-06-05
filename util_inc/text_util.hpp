// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef UTIL_TEXT_UTIL_INCLUDED
#define UTIL_TEXT_UTIL_INCLUDED
#include <string>
#include <stdexcept>

namespace util_inc {

class UnicodeException : public std::runtime_error
{
public:
    UnicodeException(const std::string& message_);
};

std::string PlatformStringToUtf8(const std::string& platformString);

} // namespace util_inc

#endif // UTIL_TEXT_UTIL_INCLUDED
