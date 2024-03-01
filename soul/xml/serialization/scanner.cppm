// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.serialization.scanner;

import std.core;
import util;
import soul.lexer;

export namespace soul::xml::serialization::scanner {

template<typename Lexer>
std::string ScanEndCpp(Lexer& lexer)
{
    std::u32string cppText;
    auto token = lexer.GetToken(lexer.GetPos());
    if (token.id != soul::lexer::END_TOKEN)
    {
        cppText.append(token.match.ToString());
    }
    bool inString = false;
    bool prevWasBackslash = false;
    const char32_t* p = lexer.Pos();
    while (p != lexer.End())
    {
        if (*p == '"')
        {
            if (!inString)
            {
                inString = true;
                cppText.append(1, *p);
                ++p;
            }
            else if (!prevWasBackslash)
            {
                inString = false;
                cppText.append(1, *p);
                ++p;
            }
            else
            {
                prevWasBackslash = false;
                cppText.append(1, *p);
                ++p;
            }
        }
        else if (!inString && *p == '%')
        {
            ++p;
            if (*p == '>')
            {
                ++p;
                lexer.SetPos(lexer.Tokens().size() - 1);
                lexer.SetPos(p);
                ++lexer;
                break;
            }
            else
            {
                cppText.append(1, '%');
                cppText.append(1, *p);
                ++p;
            }
        }
        else
        {
            if (*p == '\\')
            {
                prevWasBackslash = true;
            }
            else
            {
                prevWasBackslash = false;
            }
            cppText.append(1, *p);
            ++p;
        }
    }
    if (p == lexer.End())
    {
        throw std::runtime_error("end of C++ block: token '%>' not found");
    }
    return util::ToUtf8(cppText);
}

} // soul::xml::serialization
