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

std::string ToIntegerLiteralStr(int64_t value, int size)
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

std::string ToFloatLiteralStr(float value)
{
    return std::to_string(value);
}

std::string ToDoubleLiteralStr(double value)
{
    return std::to_string(value);
}

std::string ToStringLiteralStr(const std::string& s)
{
    if (s.find('\'') == std::string::npos) return s;
    std::string result;
    for (char c : s)
    {
        if (c == '\'')
        {
            result.append(2, '\'');
        }
        else
        {
            result.append(1, c);
        }
    }
    return result;
}

IntegerLiteral::IntegerLiteral(int64_t value_, int size_) : Value(ToIntegerLiteralStr(value_, size_)), value(value_), size(size_)
{
}

FloatLiteral::FloatLiteral(float value_) : Value(ToFloatLiteralStr(value_)), value(value_)
{
}

DoubleLiteral::DoubleLiteral(double value_) : Value(ToDoubleLiteralStr(value_)), value(value_)
{
}

StringLiteral::StringLiteral(const std::string& value_) : Value("'" + ToStringLiteralStr(value_) + "'"), value(value_)
{
}

Value* StringLiteral::Split(int length) 
{
    SetName("'" + ToStringLiteralStr(value.substr(0, length)) + "'");
    if (value.length() < length)
    {
        return new StringLiteral(std::string());
    }
    else
    {
        return new StringLiteral(value.substr(length));
    }
}

} // namespace cmajor::masm::assembly
