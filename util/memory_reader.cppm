// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.memory.reader;

import std.core;
import util.time;
import util.uuid;

export namespace util {

class MemoryReader
{
public:
    MemoryReader(const uint8_t* ptr_, int64_t count_);
    bool ReadBool();
    uint8_t ReadByte();
    int8_t ReadSByte();
    uint16_t ReadUShort();
    int16_t ReadShort();
    uint32_t ReadUInt();
    int32_t ReadInt();
    uint64_t ReadULong();
    int64_t ReadLong();
    float ReadFloat();
    double ReadDouble();
    char ReadChar();
    char16_t ReadWChar();
    char32_t ReadUChar();
    Date ReadDate();
    DateTime ReadDateTime();
    std::string ReadString();
    uuid ReadUuid();
    uint32_t ReadULEB128UInt();
    const uint8_t* Ptr() const { return ptr; }
    const uint8_t* Pos() const { return pos; }
    int64_t Count() const { return count; }
private:
    const uint8_t* ptr;
    const uint8_t* pos;
    int64_t count;
};

} // namespace util
