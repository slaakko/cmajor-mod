// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.std.io.stream;

import util.stream;
import std.core;

export namespace util {

class StdIOStream : public Stream
{
public:
    StdIOStream();
    int ReadByte() override;
    int64_t Read(uint8_t* buf, int64_t count) override;
    void Write(uint8_t x) override;
    void Write(uint8_t* buf, int64_t count) override;
};

} // util
