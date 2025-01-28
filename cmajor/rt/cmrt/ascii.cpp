// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <cmrt/ascii.hpp>
#include <cctype>

char RtmAsciiToUpper(char c)
{
    return std::toupper(static_cast<unsigned char>(c));
}

char RtmAsciiToLower(char c)
{
    return std::tolower(static_cast<unsigned char>(c));
}
