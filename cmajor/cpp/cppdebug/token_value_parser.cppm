// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.token.value.parser;

import soul.lexer;
import cmajor.ast;
import util;
import std.core;

export namespace cmajor::debug {

export namespace token::value::parser {};

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
            throw std::runtime_error("hex character expected at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(std::u32string(token.match.begin, token.match.end)));
        }
        ++p;
    }
    else
    {
        throw std::runtime_error("hex character expected at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(std::u32string(token.match.begin, token.match.end)));
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
std::basic_string<Char> ParseStringLiteral(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token, cmajor::ast::StringLiteralPrefix& prefix)
{
    prefix = cmajor::ast::StringLiteralPrefix::none;
    std::basic_string<Char> stringLiteral;
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    if (p != e && *p == 'w')
    {
        prefix = cmajor::ast::StringLiteralPrefix::utf16Prefix;
        ++p;
    }
    else if (p != e && *p == 'u')
    {
        prefix = cmajor::ast::StringLiteralPrefix::utf32Prefix;
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
std::string ParseString(const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token)
{
    cmajor::ast::StringLiteralPrefix prefix;
    std::u32string stringLit = ParseStringLiteral("", token, prefix);
    return util::ToUtf8(stringLit);
}

int ParseInt(const std::string& str)
{
    return std::stoi(str);
}

int64_t ParseLong(const std::string& str)
{
    return std::stoll(str);
}

std::string ParseVmtVariableName(const std::string& str)
{
    std::string::size_type leftAnglePos = str.find('<');
    if (leftAnglePos != std::string::npos)
    {
        std::string::size_type rightAnglePos = str.find('>', leftAnglePos + 1);
        if (rightAnglePos != std::string::npos)
        {
            return str.substr(leftAnglePos + 1, rightAnglePos - (leftAnglePos + 1));
        }
    }
    return std::string();
}

} // namespace cmajor::debug