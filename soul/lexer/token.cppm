// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.token;

import std.core;
import soul.lexer.lexeme;
import soul.lexer.token.parser;

export namespace soul::lexer {

const int64_t CONTINUE_TOKEN = -2;
const int64_t INVALID_TOKEN = -1;
const int64_t END_TOKEN = 0;

template<typename Char, typename LexerBaseT>
struct Token
{
    using CharType = Char;

    Token(LexerBaseT* lexer_) : match(), lexer(lexer_), id(INVALID_TOKEN), line(1) {}
    Token(int64_t id_, LexerBaseT* lexer_) : match(), lexer(lexer_), id(id_), line(1) {}
    Token(int64_t id_, const Lexeme<Char>& match_, LexerBaseT* lexer_, int line_) : match(match_), lexer(lexer_), id(id_), line(line_) {}
    std::basic_string<Char> ToString() const { return match.ToString(); }
    Char Chr() const { return *match.begin; }
    int8_t ToSByte() const { return ParseSByte(match.begin, match.end, lexer, line); }
    uint8_t ToByte() const { return ParseByte(match.begin, match.end, lexer, line); }
    int16_t ToShort() const { return ParseShort(match.begin, match.end, lexer, line); }
    uint16_t ToUShort() const { return ParseUShort(match.begin, match.end, lexer, line); }
    int32_t ToInt() const { return ParseInt(match.begin, match.end, lexer, line); }
    uint32_t ToUInt() const { return ParseUInt(match.begin, match.end, lexer, line); }
    int64_t ToLong() const { return ParseLong(match.begin, match.end, lexer, line); }
    uint64_t ToULong() const { return ParseULong(match.begin, match.end, lexer, line); }
    uint64_t ToHexULong() const { return ParseHexULong(match.begin, match.end, lexer, line); }
    uint64_t ToOctalULong() const { return ParseOctalULong(match.begin, match.end, lexer, line); }
    float ToFloat() const { return ParseFloat(match.begin, match.end, lexer, line); }
    double ToDouble() const { return ParseDouble(match.begin, match.end, lexer, line); }
    bool ToBool() const { return ParseBool(match.begin, match.end, lexer, line); }
    Lexeme<Char> match;
    LexerBaseT* lexer;
    int64_t id;
    int line;
};

template<typename Char, typename LexerBaseT>
struct TokenLine
{
    TokenLine() : startState(0), endState(0) {}
    int TokenIndex(int columnNumber) const
    {
        int col = 1;
        int index = 0;
        for (const Token<Char, LexerBaseT>& token : tokens)
        {
            int len = token.match.end - token.match.begin;
            if (columnNumber >= col && columnNumber < col + len)
            {
                return index;
            }
            col += len;
            ++index;
        }
        return -1;
    }
    std::vector<Token<Char, LexerBaseT>> tokens;
    int startState;
    int endState;
};

} // namespace soul::lexer
