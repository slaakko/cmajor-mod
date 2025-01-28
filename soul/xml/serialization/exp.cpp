// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.serialization.exp;

import util.unicode;

namespace soul::xml::serialization {

std::string ToString(bool value)
{
    if (value) return "true"; else return "false";
}

std::string ToString(int8_t value)
{
    return std::to_string(value);
}

std::string ToString(uint8_t value)
{
    return std::to_string(value);
}

std::string ToString(int16_t value)
{
    return std::to_string(value);
}

std::string ToString(uint16_t value)
{
    return std::to_string(value);
}

std::string ToString(int32_t value)
{
    return std::to_string(value);
}

std::string ToString(uint32_t value)
{
    return std::to_string(value);
}

std::string ToString(int64_t value)
{
    return std::to_string(value);
}

std::string ToString(uint64_t value)
{
    return std::to_string(value);
}

std::string ToString(float value)
{
    return std::to_string(value);
}

std::string ToString(double value)
{
    return std::to_string(value);
}

std::string ToString(char value)
{
    return std::string(1, value);
}

std::string ToString(char16_t value)
{
    return util::ToUtf8(std::u16string(1, value));
}

std::string ToString(char32_t value)
{
    return util::ToUtf8(std::u32string(1, value));
}

std::string ToString(const date& value)
{
    return value.ToString();
}

std::string ToString(const datetime& value)
{
    return value.ToString();
}

std::string ToString(const time_point& value)
{
    return ToString(std::chrono::duration_cast<std::chrono::nanoseconds>(value.time_since_epoch()).count());
}

std::string ToString(const duration& value)
{
    return ToString(std::chrono::duration_cast<std::chrono::nanoseconds>(value).count());
}

std::string ToString(const uuid& value)
{
    return util::ToString(value);
}

std::string ToString(const std::u16string& value)
{
    return util::ToUtf8(value);
}

std::string ToString(const std::u32string& value)
{
    return util::ToUtf8(value);
}

} // namespace soul::xml::serialization
