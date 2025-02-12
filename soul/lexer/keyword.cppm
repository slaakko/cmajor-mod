// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.keyword;

import std.core;
import soul.lexer.token;
import soul.lexer.lexeme;

export namespace soul::lexer {

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
    constexpr KeywordMap(const soul::lexer::Keyword<Char>* keywords_) : 
        caseInsensitive(false),
        keywords(keywords_), 
        keywordMap(LexemeCompare<Char>()), 
        caseInsensitiveKeywordMap(CaseInsensitiveLexemeCompare<Char>())
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
        if (caseInsensitive)
        {
            auto it = caseInsensitiveKeywordMap.find(lexeme);
            if (it != caseInsensitiveKeywordMap.cend())
            {
                return it->second;
            }
            else
            {
                return INVALID_TOKEN;
            }
        }
        else
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
    }
    void SetCaseInsensitive()
    {
        if (caseInsensitive) return;
        caseInsensitive = true;
        const soul::lexer::Keyword<Char>* kw = keywords;
        while (kw->str)
        {
            Lexeme<Char> lexeme(kw->str, StrEnd(kw->str));
            caseInsensitiveKeywordMap[lexeme] = kw->tokenID;
            ++kw;
        }
    }
private:
    bool caseInsensitive;
    const soul::lexer::Keyword<Char>* keywords;
    std::map<Lexeme<Char>, int64_t, LexemeCompare<Char>> keywordMap;
    std::map<Lexeme<Char>, int64_t, CaseInsensitiveLexemeCompare<Char>> caseInsensitiveKeywordMap;
};

} // namespace soul::lexer
