// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.little.endian.memory.writer;

import std.core;

export namespace util {

class LittleEndianMemoryWriter
{
public:
    LittleEndianMemoryWriter(uint8_t* ptr_, int64_t count_);
    void Write(uint8_t x);
    void Write(int8_t x);
    void Write(bool x);
    void Write(uint16_t x);
    void Write(int16_t x);
    void Write(uint32_t x);
    void Write(int32_t x);
    void Write(uint64_t x);
    void Write(int64_t x);
    void Write(float x);
    void Write(double x);
    void Write(char x);
    void Write(const std::string& x);
    void WriteTimestamp(std::time_t x);
    void Write(const std::vector<uint8_t>& x);
    uint8_t* Ptr() const { return ptr; }
    uint8_t* Pos() const { return pos; }
    void SetPos(uint8_t* pos_) { pos = pos_; }
    int64_t Count() const { return count; }
private:
    uint8_t* ptr;
    uint8_t* pos;
    int64_t count;
};

} // namespace util
