// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.lexer.lexeme;

import std.core;

export namespace soul::lexer {

template<typename Char>
struct Lexeme
{
    Lexeme() : begin(nullptr), end(nullptr) {}
    Lexeme(const Char* begin_, const Char* end_) : begin(begin_), end(end_) {}
    std::basic_string<Char> ToString() const { return std::basic_string<Char>(begin, end); }
    int Length() const { return static_cast<int>(end - begin); }
    const Char* begin;
    const Char* end;
};

template<typename Char>
inline bool operator==(const Lexeme<Char>& left, const Lexeme<Char>& right)
{
    if (left.end - left.begin != right.end - right.begin) return false;
    const Char* p = left.begin;
    const Char* q = right.begin;
    while (p != left.end)
    {
        if (*p != *q) return false;
        ++p;
        ++q;
    }
    return true;
}

template<typename Char>
inline bool operator<(const Lexeme<Char>& left, const Lexeme<Char>& right)
{
    const Char* p = left.begin;
    const Char* q = right.begin;
    while (p != left.end && q != right.end)
    {
        if (*p < *q) return true;
        if (*p > *q) return false;
        ++p;
        ++q;
    }
    if (p == left.end) return q != right.end;
    return false;
}

template<typename Char>
struct LexemeCompare
{
    bool operator()(const Lexeme<Char>& left, const Lexeme<Char>& right) const
    {
        return left < right;
    }
};

} // namespace soul::lexer

