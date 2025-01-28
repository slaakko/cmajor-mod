// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef AST_LEXER_POS_PAIR_INCLUDED
#define AST_LEXER_POS_PAIR_INCLUDED 
#include <stdint.h>

namespace soul::ast::lexer::pos::pair {

struct LexerPosPair
{
    LexerPosPair() : start(-1), end(-1) {}
    LexerPosPair(int64_t start_, int64_t end_) { start = start_; end = end_; }
    bool IsValid() const { return start != -1 && end != -1; }
    int64_t start;
    int64_t end;
};

} // namespace soul::ast::lexer::pos::pair

#endif // AST_LEXER_POS_PAIR_INCLUDED
