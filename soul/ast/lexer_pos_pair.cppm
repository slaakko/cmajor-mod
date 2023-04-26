// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.lexer.pos.pair;

import std.core;

export namespace soul::ast::lexer::pos::pair {

struct LexerPosPair
{
    LexerPosPair() : start(-1), end(-1) {}
    LexerPosPair(int64_t start_, int64_t end_) { start = start_; end = end_; }
    bool IsValid() const { return start != -1 && end != -1; }
    int64_t start;
    int64_t end;
};

} // namespace soul::ast::lexer::pos::pair
