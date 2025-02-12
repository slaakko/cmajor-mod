// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module util.little.endian.binary.stream.writer;

import util.unicode;

namespace util {

LitteEndianBinaryStreamWriter::LitteEndianBinaryStreamWriter(Stream& stream_) : stream(stream_)
{
}

void LitteEndianBinaryStreamWriter::Write(uint8_t x)
{
    stream.Write(x);
}

void LitteEndianBinaryStreamWriter::Write(int8_t x)
{
    Write(static_cast<uint8_t>(x));
}

void LitteEndianBinaryStreamWriter::Write(bool x)
{
    Write(uint8_t(x));
}

void LitteEndianBinaryStreamWriter::Write(uint16_t x)
{
    uint8_t b1 = static_cast<uint8_t>(x >> 8);
    uint8_t b0 = static_cast<uint8_t>(x);
    Write(b0);
    Write(b1);
}

void LitteEndianBinaryStreamWriter::Write(int16_t x)
{
    Write(static_cast<uint16_t>(x));
}

void LitteEndianBinaryStreamWriter::Write(uint32_t x)
{
    uint8_t b3 = static_cast<uint8_t>(x >> 24);
    uint8_t b2 = static_cast<uint8_t>(x >> 16);
    uint8_t b1 = static_cast<uint8_t>(x >> 8);
    uint8_t b0 = static_cast<uint8_t>(x);
    Write(b0);
    Write(b1);
    Write(b2);
    Write(b3);
}

void LitteEndianBinaryStreamWriter::Write(int32_t x)
{
    Write(static_cast<uint32_t>(x));
}

void LitteEndianBinaryStreamWriter::Write(uint64_t x)
{
    uint8_t b7 = static_cast<uint8_t>(x >> 56);
    uint8_t b6 = static_cast<uint8_t>(x >> 48);
    uint8_t b5 = static_cast<uint8_t>(x >> 40);
    uint8_t b4 = static_cast<uint8_t>(x >> 32);
    uint8_t b3 = static_cast<uint8_t>(x >> 24);
    uint8_t b2 = static_cast<uint8_t>(x >> 16);
    uint8_t b1 = static_cast<uint8_t>(x >> 8);
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

void LitteEndianBinaryStreamWriter::Write(int64_t x)
{
    Write(static_cast<uint64_t>(x));
}

void LitteEndianBinaryStreamWriter::Write(float x)
{
    uint32_t* u = reinterpret_cast<uint32_t*>(&x);
    Write(*u);
}

void LitteEndianBinaryStreamWriter::Write(double x)
{
    uint64_t* u = reinterpret_cast<uint64_t*>(&x);
    Write(*u);
}

void LitteEndianBinaryStreamWriter::Write(char x)
{
    Write(static_cast<uint8_t>(x));
}

void LitteEndianBinaryStreamWriter::Write(char8_t x)
{
    Write(static_cast<uint8_t>(x));
}

void LitteEndianBinaryStreamWriter::Write(char16_t x)
{
    Write(static_cast<uint16_t>(x));
}

void LitteEndianBinaryStreamWriter::Write(char32_t x)
{
    Write(static_cast<uint32_t>(x));
}

void LitteEndianBinaryStreamWriter::Write(const std::string& s)
{
    Write(s, true);
}

void LitteEndianBinaryStreamWriter::Write(const std::string& s, bool writeNull)
{
    for (char c : s)
    {
        Write(static_cast<uint8_t>(c));
    }
    if (writeNull)
    {
        Write(static_cast<uint8_t>(0));
    }
}

void LitteEndianBinaryStreamWriter::Write(const std::u16string& s)
{
    std::string utf8_str = ToUtf8(s);
    Write(utf8_str);
}

void LitteEndianBinaryStreamWriter::Write(const std::u32string& s)
{
    std::string utf8_str = ToUtf8(s);
    Write(utf8_str);
}

void LitteEndianBinaryStreamWriter::Write(const util::uuid& uuid)
{
    for (util::uuid::value_type x : uuid)
    {
        Write(x);
    }
}

void LitteEndianBinaryStreamWriter::WriteTime(time_t time)
{
    Write(static_cast<int64_t>(time));
}

void LitteEndianBinaryStreamWriter::WriteTimestamp(time_t time)
{
    Write(static_cast<uint32_t>(time));
}

} // namespace util
