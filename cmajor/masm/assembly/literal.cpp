// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.literal;

import util;

namespace cmajor::masm::assembly {

std::string ToHexStr(const std::string& hexStr)
{
    if ((hexStr[0] >= 'a' && hexStr[0] <= 'f') || (hexStr[0] >= 'A' && hexStr[0] <= 'F'))
    {
        return "0" + hexStr;
    }
    return hexStr;
}

std::string ToLiteralStr(int64_t value, int size)
{
    switch (size)
    {
        case 1:
        {
            return ToHexStr(util::ToHexString(static_cast<uint8_t>(value)) + "H");
        }
        case 2:
        {
            return ToHexStr(util::ToHexString(static_cast<uint16_t>(value)) + "H");
        }
        case 4:
        {
            return ToHexStr(util::ToHexString(static_cast<uint32_t>(value)) + "H");
        }
        case 8:
        {
            return ToHexStr(util::ToHexString(static_cast<uint64_t>(value)) + "H");
        }
    }
    return ToHexStr(util::ToHexString(static_cast<uint64_t>(value)) + "H");
}

NumericLiteral::NumericLiteral(int64_t value_, int size_) : Value(ToLiteralStr(value_, size_)), value(value_), size(size_)
{
}

StringLiteral::StringLiteral(const std::string& value_) : Value("'" + value_ + "'")
{
}

} // namespace cmajor::masm::assembly
