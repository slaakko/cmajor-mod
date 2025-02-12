// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.little.endian.memory.reader;

import std.core;

export namespace util {

class LittleEndianMemoryReader
{
public:
    LittleEndianMemoryReader(const uint8_t* ptr_, int64_t count_);
    uint8_t ReadByte();
    int8_t ReadSByte();
    bool ReadBool();
    uint16_t ReadUShort();
    int16_t ReadShort();
    uint32_t ReadUInt();
    int32_t ReadInt();
    uint64_t ReadULong();
    int64_t ReadLong();
    float ReadFloat();
    double ReadDouble();
    char ReadChar();
    std::string ReadString();
    std::time_t ReadTimeStamp();
    std::vector<uint8_t> ReadBlock(int64_t blockSize);
    const uint8_t* Ptr() const { return ptr; }
    const uint8_t* Pos() const { return pos; }
    void SetPos(const uint8_t* pos_) { pos = pos_; }
    int64_t Count() const { return count; }
private:
    const uint8_t* ptr;
    const uint8_t* pos;
    int64_t count;
};

} // namespace util
