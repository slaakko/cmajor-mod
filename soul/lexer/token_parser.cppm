// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.token.parser;

import std.core;
import util.unicode;

export namespace soul::lexer {

template<typename Char>
constexpr std::string ToString(const Char* begin, const Char* end)
{
    return util::ToUtf8(std::basic_string<Char>(begin, end));
}

constexpr std::string ToString(const char8_t* begin, const char8_t* end)
{
#ifdef OTAVA
    return std::string(static_cast<const char*>(static_cast<void*>(begin)), static_cast<const char*>(static_cast<void*>(end)));
#else
    return std::string(begin, end);
#endif
}

constexpr std::string ToString(const char* begin, const char* end)
{
    return std::string(begin, end);
}

// ParseSigned: parses an integral value from character range [begin, end[ in format ('+'|'-')?[0-9]+
// returns true if successful, false otherwise

template<typename Char, typename Value>
    requires std::integral<Value>
bool ParseSigned(const Char* begin, const Char* end, Value& value)
{
    value = 0;
    if (begin == end)
    {
        return false;
    }
    bool negative = false;
    int state = 0;
    while (begin != end)
    {
        Char ch = *begin;
        switch (state)
        {
            case 0:
            {
                if (ch == '+')
                {
                    state = 1;
                }
                else if (ch == '-')
                {
                    negative = true;
                    state = 1;
                }
                else if (ch >= '0' && ch <= '9')
                {
                    value = static_cast<Value>(ch) - static_cast<Value>('0');
                    state = 1;
                }
                else
                {
                    return false;
                }
                break;
            }
            case 1:
            {
                if (ch >= '0' && ch <= '9')
                {
                    value = 10 * value + static_cast<Value>(ch) - static_cast<Value>('0');
                }
                else
                {
                    return false;
                }
                break;
            }
        }
        ++begin;
    }
    if (state != 1)
    {
        return false;
    }
    else
    {
        if (negative)
        {
            value = -value;
        }
        return true;
    }
}

// ParseUnsigned: parses an integral value from character range [begin, end[ in format ('+')?[0-9]+
// returns true if successful, false otherwise

template<typename Char, typename Value>
    requires std::integral<Value>
bool ParseUnsigned(const Char* begin, const Char* end, Value& value)
{
    value = 0;
    if (begin == end)
    {
        return false;
    }
    int state = 0;
    while (begin != end)
    {
        Char ch = *begin;
        switch (state)
        {
            case 0:
            {
                if (ch == '+')
                {
                    state = 1;
                }
                else if (ch >= '0' && ch <= '9')
                {
                    value = static_cast<Value>(ch) - static_cast<Value>('0');
                    state = 1;
                }
                else
                {
                    return false;
                }
                break;
            }
            case 1:
            {
                if (ch >= '0' && ch <= '9')
                {
                    value = 10 * value + static_cast<Value>(ch) - static_cast<Value>('0');
                }
                else
                {
                    return false;
                }
                break;
            }
        }
        ++begin;
    }
    if (state != 1)
    {
        return false;
    }
    return true;
}

// ParseHex: parses an integral value from character range [begin, end[ in format [0-9a-fA-F]+
// returns true if successful, false otherwise

template<typename Char, typename Value>
    requires std::integral<Value>
bool ParseHex(const Char* begin, const Char* end, Value& value)
{
    value = 0;
    if (begin == end)
    {
        return false;
    }
    while (begin != end)
    {
        Char ch = *begin;
        if (ch >= '0' && ch <= '9')
        {
            value = 16 * value + static_cast<Value>(ch) - static_cast<Value>('0');
        }
        else if (ch >= 'a' && ch <= 'f')
        {
            value = 16 * value + 10 + static_cast<Value>(ch) - static_cast<Value>('a');
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            value = 16 * value + 10 + static_cast<Value>(ch) - static_cast<Value>('A');
        }
        else
        {
            return false;
        }
        ++begin;
    }
    return true;
}

template<typename Char, typename Value>
    requires std::integral<Value>
bool ParseOctal(const Char* begin, const Char* end, Value& value)
{
    value = 0;
    if (begin == end)
    {
        return false;
    }
    while (begin != end)
    {
        if (*begin >= '0' && *begin <= '7')
        {
            value = 8 * value + static_cast<int>(*begin) - static_cast<int>('0');
        }
        else
        {
            return false;
        }
        ++begin;
    }
    return true;
}

template<typename Char, typename Value>
    requires std::floating_point<Value>
bool ParseFloating(const Char* begin, const Char* end, Value& value)
{
    value = 0.0;
    if (begin == end)
    {
        return false;
    }
    bool negative = false;
    if (*begin == '+')
    {
        ++begin;
    }
    else if (*begin == '-')
    {
        negative = true;
        ++begin;
    }
    int state = 0;
    Value d = 10.0;
    int exponent = 0;
    bool negativeExponent = false;
    if (begin == end)
    {
        return false;
    }
    while (begin != end)
    {
        Char ch = *begin;
        switch (state)
        {
            case 0:
            {
                if (ch >= '0' && ch <= '9')
                {
                    value = 10 * value + static_cast<int>(ch) - static_cast<int>('0');
                }
                else if (ch == '.')
                {
                    state = 1;
                }
                else if (ch == 'e' || ch == 'E')
                {
                    state = 2;
                }
                else
                {
                    return false;
                }
                break;
            }
            case 1:
            {
                if (ch >= '0' && ch <= '9')
                {
                    value = 10 * value + (static_cast<int>(ch) - static_cast<int>('0')) / d;
                    d = 10 * d;
                }
                else if (ch == 'e' || ch == 'E')
                {
                    state = 2;
                }
                else
                {
                    return false;
                }
                break;
            }
            case 2:
            {
                if (ch == '+')
                {
                    state = 3;
                }
                else if (ch == '-')
                {
                    negativeExponent = true;
                    state = 3;
                }
                else if (ch >= '0' && ch <= '9')
                {
                    exponent = static_cast<int>(ch) - static_cast<int>('0');
                    state = 3;
                }
                else
                {
                    return false;
                }
                break;
            }
            case 3:
            {
                if (ch >= '0' && ch <= '9')
                {
                    exponent = 10 * exponent + static_cast<int>(ch) - static_cast<int>('0');
                }
                else
                {
                    return false;
                }
                break;
            }
        }
        ++begin;
    }
    if (negative)
    {
        value = -value;
    }
    if (exponent != 0)
    {
        if (negativeExponent)
        {
            exponent = -exponent;
        }
        value = value * static_cast<Value>(std::pow(10.0, exponent));
    }
    return true;
}

template<typename Char, typename LexerBaseT>
int8_t ParseSByte(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    int8_t value = 0;
    if (ParseSigned(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("sbyte token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
uint8_t ParseByte(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    uint8_t value = 0;
    if (ParseUnsigned(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("byte token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
int16_t ParseShort(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    int16_t value = 0;
    if (ParseSigned(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("short token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
uint16_t ParseUShort(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    uint16_t value = 0;
    if (ParseUnsigned(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("ushort token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
int32_t ParseInt(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    int32_t value = 0;
    if (ParseSigned(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("integer token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
uint32_t ParseUInt(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    uint32_t value = 0;
    if (ParseUnsigned(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("uint token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
int64_t ParseLong(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    int64_t value = 0;
    if (ParseSigned(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("long long token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
uint64_t ParseULong(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    uint64_t value = 0;
    if (ParseUnsigned(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("unsigned long long token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
uint64_t ParseHexULong(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    uint64_t value = 0;
    if (ParseHex(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("hexadecimal unsigned long long token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
uint64_t ParseOctalULong(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    uint64_t value = 0;
    if (ParseOctal(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("octal unsigned long long token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
float ParseFloat(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    float value = 0.0f;
    if (ParseFloating<Char, float>(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("float token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
double ParseDouble(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    double value = 0.0;
    if (ParseFloating<Char, double>(begin, end, value))
    {
        return value;
    }
    else
    {
        throw std::runtime_error("double token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

template<typename Char, typename LexerBaseT>
bool ParseBool(const Char* begin, const Char* end, LexerBaseT* lexer, int line)
{
    std::string s = soul::lexer::ToString(begin, end);
    if (s == "true")
    {
        return true;
    }
    else if (s == "false")
    {
        return false;
    }
    else
    {
        throw std::runtime_error("Boolean token expected in '" + lexer->FileName() + "' line " + std::to_string(line) + ": (" + soul::lexer::ToString(begin, end) + ")");
    }
}

} // namespace soul::lexer
