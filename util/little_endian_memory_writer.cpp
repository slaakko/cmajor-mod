// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.little.endian.memory.writer;

namespace util {

LittleEndianMemoryWriter::LittleEndianMemoryWriter(uint8_t* ptr_, int64_t count_) : ptr(ptr_), pos(ptr), count(count_)
{
}

void LittleEndianMemoryWriter::Write(uint8_t x)
{
    if (pos - ptr >= count)
    {
        throw std::runtime_error("little endian memory writer: end of space");
    }
    *pos++ = x;
}

void LittleEndianMemoryWriter::Write(int8_t x)
{
    Write(static_cast<uint8_t>(x));
}

void LittleEndianMemoryWriter::Write(bool x)
{
    Write(x ? static_cast<uint8_t>(1) : static_cast<uint8_t>(0));
}

void LittleEndianMemoryWriter::Write(uint16_t x)
{
    uint8_t b1 = static_cast<uint8_t>(x >> 8u);
    uint8_t b0 = static_cast<uint8_t>(x);
    Write(b0);
    Write(b1);
}

void LittleEndianMemoryWriter::Write(int16_t x)
{
    Write(static_cast<uint16_t>(x));
}

void LittleEndianMemoryWriter::Write(uint32_t x)
{
    uint8_t b3 = static_cast<uint8_t>(x >> 24u);
    uint8_t b2 = static_cast<uint8_t>(x >> 16u);
    uint8_t b1 = static_cast<uint8_t>(x >> 8u);
    uint8_t b0 = static_cast<uint8_t>(x);
    Write(b0);
    Write(b1);
    Write(b2);
    Write(b3);
}

void LittleEndianMemoryWriter::Write(int32_t x)
{
    Write(static_cast<uint32_t>(x));
}

void LittleEndianMemoryWriter::Write(uint64_t x)
{
    uint8_t b7 = static_cast<uint8_t>(x >> 56u);
    uint8_t b6 = static_cast<uint8_t>(x >> 48u);
    uint8_t b5 = static_cast<uint8_t>(x >> 40u);
    uint8_t b4 = static_cast<uint8_t>(x >> 32u);
    uint8_t b3 = static_cast<uint8_t>(x >> 24u);
    uint8_t b2 = static_cast<uint8_t>(x >> 16u);
    uint8_t b1 = static_cast<uint8_t>(x >> 8u);
    uint8_t b0 = static_cast<uint8_t>(x);
    Write(b0);
    Write(b1);
    Write(b2);
    Write(b3);
    Write(b4);
    Write(b5);
    Write(b6);
    Write(b7);
}

void LittleEndianMemoryWriter::Write(int64_t x)
{
    Write(static_cast<uint64_t>(x));
}

void LittleEndianMemoryWriter::Write(float x)
{
    uint32_t* u = reinterpret_cast<uint32_t*>(&x);
    Write(*u);
}

void LittleEndianMemoryWriter::Write(double x)
{
    uint64_t* u = reinterpret_cast<uint64_t*>(&x);
    Write(*u);
}

void LittleEndianMemoryWriter::Write(char x)
{
    Write(static_cast<uint8_t>(x));
}

void LittleEndianMemoryWriter::Write(const std::string & x)
{
    for (char c : x)
    {
        Write(static_cast<uint8_t>(c));
    }
    Write(static_cast<uint8_t>(0u));
}

void LittleEndianMemoryWriter::WriteTimestamp(std::time_t x)
{
    uint32_t u = static_cast<uint32_t>(x);
    Write(u);
}

void LittleEndianMemoryWriter::Write(const std::vector<uint8_t>& x)
{
    int64_t blockSize = x.size();
    for (int64_t i = 0; i < blockSize; ++i)
    {
        Write(x[i]);
    }
}

} // namespace util
