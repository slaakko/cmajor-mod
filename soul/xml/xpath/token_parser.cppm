// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.xpath.token.parser;

import std.core;
import util;
import soul.lexer.token;
import soul.lexer.base;
import soul.common.token.parser;

export namespace soul::xml::xpath::token::parser {

template<typename Char>
std::string ParseDQString(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token)
{
    std::u32string stringLiteral;
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    if (p != e && *p == '"')
    {
        ++p;
        while (p != e && *p != '\r' && *p != '\n' && *p != '"')
        {
            if (*p == '\\')
            {
                ++p;
                stringLiteral.append(1, soul::common::token::parser::ParseEscape(fileName, p, e, token));
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
            throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.match.ToString()));
        }
    }
    else
    {
        throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.match.ToString()));
    }
    return util::ToUtf8(stringLiteral);
}

template<typename Char>
std::string ParseSQString(const std::string& fileName, const soul::lexer::Token<Char, soul::lexer::LexerBase<Char>>& token)
{
    std::u32string stringLiteral;
    const Char* p = token.match.begin;
    const Char* e = token.match.end;
    if (p != e && *p == '\'')
    {
        ++p;
        while (p != e && *p != '\r' && *p != '\n' && *p != '\'')
        {
            if (*p == '\\')
            {
                ++p;
                stringLiteral.append(1, soul::common::token::parser::ParseEscape(fileName, p, e, token));
            }
            else
            {
                stringLiteral.append(1, *p);
                ++p;
            }
        }
        if (p != e && *p == '\'')
        {
            ++p;
        }
        if (p != e)
        {
            throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.match.ToString()));
        }
    }
    else
    {
        throw std::runtime_error("invalid string literal at " + fileName + ":" + std::to_string(token.line) + ": " + util::ToUtf8(token.match.ToString()));
    }
    return util::ToUtf8(stringLiteral);
}

} // namespace soul::xml::xpath::token::parser
