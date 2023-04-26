// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.memory.writer;

import std.core;
import util.time;

export namespace util {

class MemoryWriter
{
public:
    MemoryWriter(uint8_t* ptr_, int64_t count_);
    void Write(uint8_t x);
    void Write(int8_t x);
    void Write(uint16_t x);
    void Write(int16_t x);
    void Write(uint32_t x);
    void Write(int32_t x);
    void Write(uint64_t x);
    void Write(int64_t x);
    void Write(const DateTime& dt);
    void Write(const std::string& str);
private:
    uint8_t* ptr;
    uint8_t* pos;
    int64_t count;
};

} // namespace util
