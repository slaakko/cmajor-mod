// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <rt/ascii.hpp>
#include <cctype>

char RtAsciiToUpper(char c)
{
    return std::toupper(static_cast<unsigned char>(c));
}

char RtAsciiToLower(char c)
{
    return std::tolower(static_cast<unsigned char>(c));
}
