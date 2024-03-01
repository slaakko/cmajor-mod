// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <cstdio>

module util.std.io.stream;

namespace util {

StdIOStream::StdIOStream()
{
}

int StdIOStream::ReadByte()
{
    int c = std::fgetc(stdin);
    return c;
}

int64_t StdIOStream::Read(uint8_t* buf, int64_t count)
{
    std::size_t bytesRead = std::fread(buf, 1, count, stdin);
    return bytesRead;
}

void StdIOStream::Write(uint8_t x)
{
    std::fputc(x, stdout);
}

void StdIOStream::Write(uint8_t* buf, int64_t count)
{
    std::fwrite(buf, 1, count, stdout);
}

} // util
