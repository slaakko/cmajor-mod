// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <boost/uuid/uuid.hpp>

export module util.binary.stream.reader;

import std.core;
import util.stream;

export namespace util {

class BinaryStreamReader
{
public:
    BinaryStreamReader(Stream& stream_);
    Stream& GetStream() { return stream; }
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
    char8_t ReadChar8();
    char16_t ReadWChar();
    char32_t ReadUChar();
    std::string ReadUtf8String();
    std::u16string ReadUtf16String();
    std::u32string ReadUtf32String();
    uint32_t ReadULEB128UInt();
    uint64_t ReadULEB128ULong();
    int32_t ReadSLEB128Int();
    int64_t ReadSLEB128Long();
    void ReadUuid(boost::uuids::uuid& uuid);
    time_t ReadTime();
    int64_t Position() const { return stream.Position(); }
private:
    Stream& stream;
};

} // util
