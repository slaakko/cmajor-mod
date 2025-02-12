// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.little.endian.binary.stream.reader;

import util.unicode;

namespace util {

LittleEndianBinaryStreamReader::LittleEndianBinaryStreamReader(Stream& stream_) : stream(stream_)
{
}

uint8_t LittleEndianBinaryStreamReader::ReadByte()
{
    int x = stream.ReadByte();
    if (x == -1)
    {
        throw EndOfStreamException("unexpected end of stream");
    }
    return static_cast<uint8_t>(x);
}

int8_t LittleEndianBinaryStreamReader::ReadSByte()
{
    return static_cast<int8_t>(ReadByte());
}

bool LittleEndianBinaryStreamReader::ReadBool()
{
    return static_cast<bool>(ReadByte());
}

uint16_t LittleEndianBinaryStreamReader::ReadUShort()
{
    uint8_t l = ReadByte();
    uint8_t h = ReadByte();
    return (static_cast<uint16_t>(h) << 8) | static_cast<uint16_t>(l);
}

int16_t LittleEndianBinaryStreamReader::ReadShort()
{
    uint16_t x = ReadUShort();
    return static_cast<int16_t>(x);
}

uint32_t LittleEndianBinaryStreamReader::ReadUInt()
{
    uint8_t b3 = ReadByte();
    uint8_t b2 = ReadByte();
    uint8_t b1 = ReadByte();
    uint8_t b0 = ReadByte();
    return (static_cast<uint32_t>(b0) << 24) | (static_cast<uint32_t>(b1) << 16) | (static_cast<uint32_t>(b2) << 8) | static_cast<uint32_t>(b3);
}

int32_t LittleEndianBinaryStreamReader::ReadInt()
{
    uint32_t x = ReadUInt();
    return static_cast<int32_t>(x);
}

uint64_t LittleEndianBinaryStreamReader::ReadULong()
{
    uint8_t b7 = ReadByte();
    uint8_t b6 = ReadByte();
    uint8_t b5 = ReadByte();
    uint8_t b4 = ReadByte();
    uint8_t b3 = ReadByte();
    uint8_t b2 = ReadByte();
    uint8_t b1 = ReadByte();
    uint8_t b0 = ReadByte();
    return (static_cast<uint64_t>(b0) << 56) | (static_cast<uint64_t>(b1) << 48) | (static_cast<uint64_t>(b2) << 40) | (static_cast<uint64_t>(b3) << 32) | (static_cast<uint64_t>(b4) << 24) |
        (static_cast<uint64_t>(b5) << 16) | (static_cast<uint64_t>(b6) << 8) | static_cast<uint64_t>(b7);
}

int64_t LittleEndianBinaryStreamReader::ReadLong()
{
    uint64_t x = ReadULong();
    return static_cast<int64_t>(x);
}

float LittleEndianBinaryStreamReader::ReadFloat()
{
    uint32_t x = ReadUInt();
    return *reinterpret_cast<float*>(&x);
}

double LittleEndianBinaryStreamReader::ReadDouble()
{
    uint64_t x = ReadULong();
    return *reinterpret_cast<double*>(&x);
}

char LittleEndianBinaryStreamReader::ReadChar()
{
    uint8_t x = ReadByte();
    return static_cast<char>(x);
}

char8_t LittleEndianBinaryStreamReader::ReadChar8()
{
    uint8_t x = ReadByte();
    return static_cast<char8_t>(x);
}

char16_t LittleEndianBinaryStreamReader::ReadWChar()
{
    uint16_t x = ReadUShort();
    return static_cast<char16_t>(x);
}

char32_t LittleEndianBinaryStreamReader::ReadUChar()
{
    uint32_t x = ReadUInt();
    return static_cast<char32_t>(x);
}

std::string LittleEndianBinaryStreamReader::ReadUtf8String()
{
    std::string s;
    uint8_t x = ReadByte();
    while (x != 0)
    {
        s.append(1, static_cast<char>(x));
        x = ReadByte();
    }
    return s;
}

std::u16string LittleEndianBinaryStreamReader::ReadUtf16String()
{
    std::string s = ReadUtf8String();
    return ToUtf16(s);
}

std::u32string LittleEndianBinaryStreamReader::ReadUtf32String()
{
    std::string s = ReadUtf8String();
    return ToUtf32(s);
}

void LittleEndianBinaryStreamReader::ReadUuid(util::uuid& uuid)
{
    for (util::uuid::value_type& x : uuid)
    {
        x = ReadByte();
    }
}

time_t LittleEndianBinaryStreamReader::ReadTime()
{
    return static_cast<time_t>(ReadLong());
}

time_t LittleEndianBinaryStreamReader::ReadTimestamp()
{
    return static_cast<time_t>(ReadUInt());
}

} // util
