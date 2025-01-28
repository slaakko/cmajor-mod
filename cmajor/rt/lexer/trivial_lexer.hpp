// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef LEXER_TRIVIAL_LEXER_INCLUDED
#define LEXER_TRIVIAL_LEXER_INCLUDED
#include <lexer/lexer_class.hpp>

namespace soul::lexer::trivial {

struct TrivialLexer_Variables : public soul::lexer::Variables
{
};

template<typename Char>
struct TrivialLexer;

template<typename Char>
soul::lexer::Lexer<TrivialLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName);

template<typename Char>
struct TrivialLexer
{
    using Variables = TrivialLexer_Variables;

    static int32_t NextState(int32_t state, Char chr, soul::lexer::LexerBase<Char>& lexer)
    {
        auto& token = lexer.CurrentToken();
        token.match = lexer.CurrentLexeme();
        token.id = static_cast<int64_t>(chr);
        if (chr == '\n')
        {
            lexer.SetLine(lexer.Line() + 1);
        }
        return -1;
    }
};

template<typename Char>
soul::lexer::Lexer<TrivialLexer<Char>, Char> MakeLexer(const Char* start, const Char* end, const std::string& fileName)
{
    auto lexer = soul::lexer::Lexer<TrivialLexer<Char>, Char>(start, end, fileName);
    return lexer;
}

} // namespace soul::lexer::trivial

#endif // LEXER_TRIVIAL_LEXER_INCLUDED
