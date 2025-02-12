// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.little.endian.memory.reader;

namespace util {

LittleEndianMemoryReader::LittleEndianMemoryReader(const uint8_t* ptr_, int64_t count_) : ptr(ptr_), pos(ptr), count(count_)
{
}

uint8_t LittleEndianMemoryReader::ReadByte()
{
    if (pos - ptr >= count)
    {
        throw std::runtime_error("little endian memory reader: unexpected end of data");
    }
    return *pos++;
}

int8_t LittleEndianMemoryReader::ReadSByte()
{
    return static_cast<int8_t>(ReadByte());
}

bool LittleEndianMemoryReader::ReadBool()
{
    uint8_t x = ReadByte();
    return x != 0u;
}

uint16_t LittleEndianMemoryReader::ReadUShort()
{
    uint8_t b0 = ReadByte();
    uint8_t b1 = ReadByte();
    return (static_cast<uint16_t>(b1) << 8u) | static_cast<uint16_t>(b0);
}

int16_t LittleEndianMemoryReader::ReadShort()
{
    return static_cast<int16_t>(ReadUShort());
}

uint32_t LittleEndianMemoryReader::ReadUInt()
{
    uint8_t b0 = ReadByte();
    uint8_t b1 = ReadByte();
    uint8_t b2 = ReadByte();
    uint8_t b3 = ReadByte();
    return (static_cast<uint32_t>(b3) << 24u) | (static_cast<uint32_t>(b2) << 16u) | (static_cast<uint32_t>(b1) << 8u) | static_cast<uint32_t>(b0);
}

int32_t LittleEndianMemoryReader::ReadInt()
{
    return static_cast<int32_t>(ReadUInt());
}

uint64_t LittleEndianMemoryReader::ReadULong()
{
    uint8_t b0 = ReadByte();
    uint8_t b1 = ReadByte();
    uint8_t b2 = ReadByte();
    uint8_t b3 = ReadByte();
    uint8_t b4 = ReadByte();
    uint8_t b5 = ReadByte();
    uint8_t b6 = ReadByte();
    uint8_t b7 = ReadByte();
    return (static_cast<uint64_t>(b7) << 56u) | (static_cast<uint64_t>(b6) << 48u) | (static_cast<uint64_t>(b5) << 40u) | (static_cast<uint64_t>(b4) << 32u) |
        (static_cast<uint64_t>(b3) << 24u) | (static_cast<uint64_t>(b2) << 16u) | (static_cast<uint64_t>(b1) << 8u) | static_cast<uint64_t>(b0);
}

int64_t LittleEndianMemoryReader::ReadLong()
{
    return static_cast<int64_t>(ReadULong());
}

float LittleEndianMemoryReader::ReadFloat()
{
    uint32_t x = ReadUInt();
    return *reinterpret_cast<float*>(&x);
}

double LittleEndianMemoryReader::ReadDouble()
{
    uint64_t x = ReadULong();
    return *reinterpret_cast<double*>(&x);
}

char LittleEndianMemoryReader::ReadChar()
{
    uint8_t x = ReadByte();
    return static_cast<char>(x);
}

std::string LittleEndianMemoryReader::ReadString()
{
    std::string s;
    uint8_t x = ReadByte();
    while (x)
    {
        s.append(1, static_cast<char>(x));
        x = ReadByte();
    }
    return s;
}

std::time_t LittleEndianMemoryReader::ReadTimeStamp()
{
    uint32_t x = ReadUInt();
    return static_cast<std::time_t>(x);
}

std::vector<uint8_t> LittleEndianMemoryReader::ReadBlock(int64_t blockSize)
{
    std::vector<uint8_t> block;
    for (int64_t i = 0; i < blockSize; ++i)
    {
        block.push_back(ReadByte());
    }
    return block;
}

} // namespace util
