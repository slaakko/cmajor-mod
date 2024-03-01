// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef LEXER_LEXING_UTIL_INCLUDED
#define LEXER_LEXING_UTIL_INCLUDED
#include <lexer/base.hpp>
#include <lexer/token.hpp>

namespace soul::lexer {

template<typename Char>
bool NoWhiteSpaceBetweenTokens(const soul::lexer::Token<Char, LexerBase<Char>>& left, const soul::lexer::Token<Char, LexerBase<Char>>& right)
{
    if (left.match.end == right.match.begin) return true;
    return false;
}

} // namespace soul::lexer

#endif // LEXER_LEXING_UTIL_INCLUDED
