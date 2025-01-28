// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef LEXER_KEYWORD_INCLUDED
#define LEXER_KEYWORD_INCLUDED
#include <stdint.h>
#include <lexer/token.hpp>
#include <map>

namespace soul::lexer {

template<typename Char>
constexpr const Char* StrEnd(const Char* s)
{
    while (*s)
    {
        ++s;
    }
    return s;
}

template<typename Char>
struct Keyword
{
    Keyword() : str(nullptr), tokenID(INVALID_TOKEN) {}
    Keyword(const Char* str_, int64_t tokenID_) : str(str_), tokenID(tokenID_) { }
    const Char* str;
    int64_t tokenID;
};

template<typename Char>
class KeywordMap
{
public:
    constexpr KeywordMap(const soul::lexer::Keyword<Char>* keywords_) : keywords(keywords_)
    {
        const soul::lexer::Keyword<Char>* kw = keywords;
        while (kw->str)
        {
            Lexeme<Char> lexeme(kw->str, StrEnd(kw->str));
            keywordMap[lexeme] = kw->tokenID;
            ++kw;
        }
    }
    int64_t GetKeywordToken(const Lexeme<Char>& lexeme) const
    {
        auto it = keywordMap.find(lexeme);
        if (it != keywordMap.cend())
        {
            return it->second;
        }
        else
        {
            return INVALID_TOKEN;
        }
    }
private:
    const soul::lexer::Keyword<Char>* keywords;
    std::map<Lexeme<Char>, int64_t, LexemeCompare<Char>> keywordMap;
};

} // namespace soul::lexer

#endif // LEXER_KEYWORD_INCLUDED
