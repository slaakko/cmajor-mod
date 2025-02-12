// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module util.little.endian.binary.stream.reader;

import util.binary.stream.reader;
import std.core;
import util.stream;
import util.uuid;

export namespace util {

class LittleEndianBinaryStreamReader
{
public:
    LittleEndianBinaryStreamReader(Stream& stream_);
    Stream& GetStream() { return stream; }
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
    char8_t ReadChar8();
    char16_t ReadWChar();
    char32_t ReadUChar();
    std::string ReadUtf8String();
    std::u16string ReadUtf16String();
    std::u32string ReadUtf32String();
    void ReadUuid(util::uuid& uuid);
    time_t ReadTime();
    time_t ReadTimestamp();
    int64_t Position() const { return stream.Position(); }
private:
    Stream& stream;
};

} // util
