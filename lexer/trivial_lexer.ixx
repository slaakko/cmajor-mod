// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

export module lexer.trivial.lexer;

import std.core;
import lexer;

export namespace lexer {

class TrivialLexer : public lexer::Lexer
{
public:
    TrivialLexer(const std::u32string& content_, const std::string& fileName_, int fileIndex_);
    TrivialLexer(const char32_t* start_, const char32_t* end_, const std::string& fileName_, int fileIndex_);
    int NextState(int state, char32_t c) override;
};

std::string GetTrivialTokenInfo(int tokenId);

} // namespace lexer