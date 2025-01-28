// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.token.value;

import std.core;
import soul.lexer;
import cmajor.fault.tolerant.ast;
import util;

export namespace cmajor::fault::tolerant::parser {

template<typename Char>
void ParseHexChar(const std::string& fileName, Char& value, const Char*& p, const Char* e, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token)
{
    if (p != e)
    {
        bool notHex = false;
        switch (*p)
        {
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
        {
            value = 16 * value + *p - '0';
            break;
        }
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        {
            value = 16 * value + 10 + *p - 'A';
            break;
        }
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        {
            value = 16 * value + 10 + *p - 'a';
            break;
        }
        default:
        {
            notHex = true;
            break;
        }
        }
        if (notHex)
        {
            throw std::runtime_error("hex character expected at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
        }
        ++p;
    }
    else
    {
        throw std::runtime_error("hex character expected at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
    }
}

template<typename Char>
Char ParseEscape(const std::string& fileName, const Char*& p, const Char* e, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token)
{
    Char value = '\0';
    if (p != e && (*p == 'x' || *p == 'X'))
    {
        ++p;
        while (p != e && ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F')))
        {
            ParseHexChar(fileName, value, p, e, token);
        }
    }
    else if (p != e && (*p == 'd' || *p == 'D'))
    {
        ++p;
        while (p != e && *p >= '0' && *p <= '9')
        {
            value = 10 * value + (*p - '0');
            ++p;
        }
    }
    else if (p != e && (*p >= '0' && *p <= '7'))
    {
        while (p != e && *p >= '0' && *p <= '7')
        {
            value = 8 * value + (*p - '0');
            ++p;
        }
    }
    else if (p != e && *p == 'u')
    {
        ++p;
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
    }
    else if (p != e && *p == 'U')
    {
        ++p;
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
        ParseHexChar(fileName, value, p, e, token);
    }
    else if (p != e)
    {
        switch (*p)
        {
        case 'a': value = '\a'; break;
        case 'b': value = '\b'; break;
        case 'f': value = '\f'; break;
        case 'n': value = '\n'; break;
        case 'r': value = '\r'; break;
        case 't': value = '\t'; break;
        case 'v': value = '\v'; break;
        default: value = *p; break;
        }
        ++p;
    }
    return value;

}

template<typename Char>
char32_t ParseCharacterLiteral(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token, 
    cmajor::fault::tolerant::ast::CharLiteralPrefix& prefix)
{
    prefix = cmajor::fault::tolerant::ast::CharLiteralPrefix::none;
    char32_t value = '\0';
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    if (p != e && *p == 'w')
    {
        prefix = cmajor::fault::tolerant::ast::CharLiteralPrefix::utf16Prefix;
        ++p;
    }
    else if (p != e && *p == 'u')
    {
        prefix = cmajor::fault::tolerant::ast::CharLiteralPrefix::utf32Prefix;
        ++p;
    }
    if (p != e && *p == '\'')
    {
        ++p;
        if (p != e && *p == '\\')
        {
            ++p;
            value = ParseEscape(fileName, p, e, token);
        }
        else
        {
            std::u32string s;
            while (p != e && *p != '\r' && *p != '\n' && *p != '\'')
            {
                s.append(1, *p);
                ++p;
            }
            std::u32string u = s;
            if (u.size() != 1)
            {
                throw std::runtime_error("invalid character literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
            }
            value = u.front();
        }
        if (p != e && *p == '\'')
        {
            ++p;
        }
        if (p != e)
        {
            throw std::runtime_error("invalid character literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
        }
    }
    else
    {
        throw std::runtime_error("invalid character literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
    }
    return value;
}

template<typename Char>
std::basic_string<Char> ParseStringLiteral(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token, 
    cmajor::fault::tolerant::ast::StringLiteralPrefix& prefix)
{
    prefix = cmajor::fault::tolerant::ast::StringLiteralPrefix::none;
    std::basic_string<Char> stringLiteral;
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    if (p != e && *p == 'w')
    {
        prefix = cmajor::fault::tolerant::ast::StringLiteralPrefix::utf16Prefix;
        ++p;
    }
    else if (p != e && *p == 'u')
    {
        prefix = cmajor::fault::tolerant::ast::StringLiteralPrefix::utf32Prefix;
        ++p;
    }
    if (p != e && *p == '@')
    {
        ++p;
        if (p != e && *p == '"')
        {
            ++p;
            while (p != e && *p != '"')
            {
                stringLiteral.append(1, *p);
                ++p;
            }
            if (p != e && *p == '"')
            {
                ++p;
            }
            if (p != e)
            {
                throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
            }
        }
    }
    else
    {
        if (p != e && *p == '"')
        {
            ++p;
            while (p != e && *p != '\r' && *p != '\n' && *p != '"')
            {
                if (*p == '\\')
                {
                    ++p;
                    stringLiteral.append(1, ParseEscape(fileName, p, e, token));
                }
                else
                {
                    stringLiteral.append(1, *p);
                    ++p;
                }
            }
            if (p != e && *p == '"')
            {
                ++p;
            }
            if (p != e)
            {
                throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
            }
        }
        else
        {
            throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
        }
    }
    return stringLiteral;
}

template<typename Char>
double ParseFloatingLiteral(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token, bool& floatLiteral)
{
    double value = 0.0;
    floatLiteral = false;
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    std::string str;
    while (p != e && ((*p >= '0' && *p <= '9') || *p == '.' || *p == 'e' || *p == 'E' || *p == '-' || *p == '+'))
    {
        str.append(1, static_cast<unsigned char>(*p));
        ++p;
    }
    if (p != e && (*p == 'f' || *p == 'F'))
    {
        ++p;
        floatLiteral = true;
    }
    if (p != e)
    {
        throw std::runtime_error("invalid floating literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
    }
    std::stringstream s;
    s.str(str);
    s >> value;
    if (s.fail() || s.bad())
    {
        throw std::runtime_error("invalid floating literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
    }
    return value;
}

template<typename Char>
uint64_t ParseIntegerLiteral(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token, bool& isUnsigned)
{
    uint64_t value = 0;
    isUnsigned = false;
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    if (p != e && *p == '0')
    {
        ++p;
        if (p != e && (*p == 'x' || *p == 'X'))
        {
            ++p;
            while (p != e && ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F')))
            {
                switch (*p)
                {
                case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                {
                    value = 16 * value + *p - '0';
                    break;
                }
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                {
                    value = 16 * value + 10 + *p - 'A';
                    break;
                }
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                {
                    value = 16 * value + 10 + *p - 'a';
                    break;
                }
                }
                ++p;
            }
        }
        else
        {
            while (p != e && *p >= '0' && *p <= '7')
            {
                value = 8 * value + (*p - '0');
                ++p;
            }
        }
    }
    else if (p != e && *p >= '1' && *p <= '9')
    {
        while (p != e && *p >= '0' && *p <= '9')
        {
            value = 10 * value + (*p - '0');
            ++p;
        }
    }
    else
    {
        throw std::runtime_error("invalid integer literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
    }
    if (p != e && (*p == 'u' || *p == 'U'))
    {
        ++p;
        isUnsigned = true;
    }
    if (p != e)
    {
        throw std::runtime_error("invalid integer literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
    }
    return value;
}

template<typename Char>
std::string ParseFilePath(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token)
{
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    const Char* begin = nullptr;
    const Char* end = nullptr;
    if (p != e && *p == '<')
    {
        ++p;
        begin = p;
        while (p != e && *p != '>')
        {
            ++p;
        }
    }
    if (p != e && *p == '>')
    {
        end = p;
        ++p;
    }
    if (p != e)
    {
        throw std::runtime_error("invalid file path at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.ToString()));
    }
    return util::ToUtf8(std::u32string(begin, end));
}

} // namespace cmajor::fault::tolerant::parser
