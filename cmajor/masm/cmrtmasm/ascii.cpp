// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.ascii;

char RtmAsciiToUpper(char c)
{
    return std::toupper(static_cast<unsigned char>(c));
}

char RtmAsciiToLower(char c)
{
    return std::tolower(static_cast<unsigned char>(c));
}
