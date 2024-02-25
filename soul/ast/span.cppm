// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.span;

import std.core;

export namespace soul::ast {

export namespace span {}

struct Span
{
    Span() : pos(-1), len(-1) {}
    Span(int pos_, int len_) : pos(pos_), len(len_) {}
    bool IsValid() const { return pos != -1; }
    bool Contains(int pos_) const { return pos_ >= pos && pos_ < pos + len; }
    void Union(const Span& that);
    int pos;
    int len;
};

struct LineColLen
{
    LineColLen() : line(0), col(0), len(0) {}
    LineColLen(int line_, int col_) : line(line_), col(col_), len(0) {}
    LineColLen(int line_, int col_, int len_) : line(line_), col(col_), len(len_) {}
    bool IsValid() const { return line != 0; }
    int line;
    int col;
    int len;
};

LineColLen SpanToLineColLen(const Span& span, const std::vector<int>& lineStarts);
int LineColLenToPos(const LineColLen& lineColLen, const std::vector<int>& lineStarts);

} // namespace soul::ast

