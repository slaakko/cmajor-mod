// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binary.message.io;

import cmajor.binary.message.number;
import std.core;
import soul.ast.source.pos;
import util;

export namespace cmajor::bmp {

inline uint32_t Length(bool b)
{
    return 1;
}

inline uint32_t Length(int8_t x)
{
    return 1;
}

inline uint32_t Length(uint8_t x)
{
    return 1;
}

inline uint32_t Length(int16_t x)
{
    return 2;
}

inline uint32_t Length(uint16_t x)
{
    return 2;
}

inline uint32_t Length(int32_t x)
{
    return 4;
}

inline uint32_t Length(uint32_t x)
{
    return 4;
}

inline uint32_t Length(int64_t x)
{
    return 8;
}

inline uint32_t Length(uint64_t x)
{
    return 8;
}

inline uint32_t Length(float x)
{
    return 4;
}

inline uint32_t Length(double x)
{
    return 8;
}

inline uint32_t Length(char x)
{
    return 1;
}

inline uint32_t Length(char16_t x)
{
    return 2;
}

inline uint32_t Length(char32_t x)
{
    return 4;
}

inline uint32_t Length(const util::Date& x)
{
    return 4;
}

inline uint32_t Length(const util::DateTime& x)
{
    return 8;
}

inline uint32_t Length(const std::string& x)
{
    return x.length() + 1;
}

inline uint32_t Length(const util::uuid& x)
{
    return 16;
}

inline uint32_t Length(const cmajor::bmp::Number& x)
{
    return x.Length();
}

template<typename T>
concept EnumType = std::is_enum_v<T>;

template<EnumType T>
inline uint32_t Length(T x)
{
    return Length(static_cast<int32_t>(x));
}

template<typename T>
concept BinaryMessageType =
std::is_class_v<T> && requires(T t) { t.Length(); };

template<BinaryMessageType Msg>
inline uint32_t Length(const Msg& msg)
{
    return msg.Length();
}

template<typename T>
concept ScalarType = std::is_fundamental_v<T>;

template<ScalarType T>
inline uint32_t Length(const std::vector<T>& x)
{
    return 4 + x.size() * Length(T());
}

template<typename T>
inline uint32_t Length(const std::vector<T>& x)
{
    uint32_t length = 4;
    for (const auto& value : x)
    {
        length += Length(value);
    }
    return length;
}

inline void Write(util::MemoryWriter& writer, bool b)
{
    writer.Write(b);
}

inline void Write(util::MemoryWriter& writer, int8_t x)
{
    writer.Write(x); 
}

inline void Write(util::MemoryWriter& writer, uint8_t x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, int16_t x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, uint16_t x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, int32_t x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, uint32_t x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, int64_t x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, uint64_t x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, float x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, double x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, char x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, char16_t x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, char32_t x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, const util::Date& x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, const util::DateTime& x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, const std::string& x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, const util::uuid& x)
{
    writer.Write(x);
}

inline void Write(util::MemoryWriter& writer, const cmajor::bmp::Number& x)
{
    x.Write(writer);
}

template<typename T>
concept WritableEnumType = std::is_enum_v<T>;

template<WritableEnumType T>
inline void Write(util::MemoryWriter& writer, T x)
{
    Write(writer, static_cast<int32_t>(x));
}

template<typename T>
concept WritableBinaryMessageType =
std::is_class_v<T> && requires(T t, util::MemoryWriter& writer) { t.Write(writer); };

template<WritableBinaryMessageType Msg>
inline void Write(util::MemoryWriter& writer, const Msg& msg)
{
    msg.Write(writer);
}

template<typename T>
void Write(util::MemoryWriter& writer, const std::vector<T>& x)
{
    writer.Write(static_cast<uint32_t>(x.size()));
    for (const auto& m : x)
    {
        Write(writer, m);
    }
}

inline void Read(util::MemoryReader& reader, bool& value)
{
    value = reader.ReadBool();
}

inline void Read(util::MemoryReader& reader, int8_t& value)
{
    value = reader.ReadSByte();
}

inline void Read(util::MemoryReader& reader, uint8_t& value)
{
    value = reader.ReadByte();
}

inline void Read(util::MemoryReader& reader, int16_t& value)
{
    value = reader.ReadShort();
}

inline void Read(util::MemoryReader& reader, uint16_t& value)
{
    value = reader.ReadUShort();
}

inline void Read(util::MemoryReader& reader, int32_t& value)
{
    value = reader.ReadInt();
}

inline void Read(util::MemoryReader& reader, uint32_t& value)
{
    value = reader.ReadUInt();
}

inline void Read(util::MemoryReader& reader, int64_t& value)
{
    value = reader.ReadLong();
}

inline void Read(util::MemoryReader& reader, uint64_t& value)
{
    value = reader.ReadULong();
}

inline void Read(util::MemoryReader& reader, float& value)
{
    value = reader.ReadFloat();
}

inline void Read(util::MemoryReader& reader, double& value)
{
    value = reader.ReadDouble();
}

inline void Read(util::MemoryReader& reader, char& value)
{
    value = reader.ReadChar();
}

inline void Read(util::MemoryReader& reader, char16_t& value)
{
    value = reader.ReadWChar();
}

inline void Read(util::MemoryReader& reader, char32_t& value)
{
    value = reader.ReadUChar();
}

inline void Read(util::MemoryReader& reader, util::Date& value)
{
    value = reader.ReadDate();
}

inline void Read(util::MemoryReader& reader, util::DateTime& value)
{
    value = reader.ReadDateTime();
}

inline void Read(util::MemoryReader& reader, std::string& value)
{
    value = reader.ReadString();
}

inline void Read(util::MemoryReader& reader, util::uuid& value)
{
    value = reader.ReadUuid();
}

inline void Read(util::MemoryReader& reader, cmajor::bmp::Number& value)
{
    value.Read(reader);
}

template<typename T>
concept ReadableEnumType = std::is_enum_v<T>;

template<ReadableEnumType T>
inline void Read(util::MemoryReader& reader, T& value)
{
    int32_t x = 0;
    Read(reader, x);
    value = static_cast<T>(x);
}

template<typename T>
concept ReadableBinaryMessageType =
std::is_class_v<T> && requires(T t, util::MemoryReader& reader) { t.Read(reader); };

template<ReadableBinaryMessageType Msg>
inline void Read(util::MemoryReader& reader, Msg& msg)
{
    msg.Read(reader);
}

template<typename T>
void Read(util::MemoryReader& reader, std::vector<T>& x)
{
    x.clear();
    uint32_t length = 0;
    Read(reader, length);
    for (uint32_t i = 0; i < length; ++i)
    {
        T m;
        Read(reader, m);
        x.push_back(m);
    }
}

} // cmajor::bmp
