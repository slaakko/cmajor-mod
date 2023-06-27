// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binary.message.number;

import std.core;
import util;

export namespace cmajor::bmp {

struct Number
{
    Number() : value(0) {}
    Number(uint32_t value_) : value(value_) {}
    uint32_t Length() const
    {
        uint32_t length = 0;
        uint32_t number = value;
        do
        {
            uint8_t b = number & 0x7F;
            number >>= 7;
            if (number != 0)
            {
                b |= 0x80;
            }
            ++length;
        } while (number != 0);
        return length;
    }
    void Write(util::MemoryWriter& writer)  const
    {
        writer.WriteULEB128UInt(value);
    }
    void Read(util::MemoryReader& reader)
    {
        value = reader.ReadULEB128UInt();
    }
    uint32_t value;
};

} // namespace cmajor::bmp
