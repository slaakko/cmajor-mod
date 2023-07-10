// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.ast.source.pos;

import std.core;

export namespace soul::ast {

struct SourcePos
{
    SourcePos() : pos(-1), file(-1), line(0), col(0) {}
    SourcePos(int line_, int col_) : pos(-1), file(-1), line(line_), col(col_) {}
    SourcePos(int file_, int line_, int col_) : pos(-1), file(file_), line(line_), col(col_) {}
    SourcePos(int64_t pos_, int line_, int col_) : pos(pos_), file(-1), line(line_), col(col_) {}
    SourcePos(int64_t pos_, int file_, int line_, int col_) : pos(pos_), file(file_), line(line_), col(col_) {}
    bool IsValid() const { return line != 0; }
    int64_t pos;
    int file;
    int line;
    int col;
};

inline bool operator==(const SourcePos& left, const SourcePos& right)
{
    return left.file == right.file && left.line == right.line && left.col == right.col;
}

inline bool operator<(const SourcePos& left, const SourcePos& right)
{
    if (left.file < right.file) return true;
    if (left.file > right.file) return false;
    if (left.line < right.line) return true;
    if (left.line > right.line) return false;
    return left.col < right.col;
}

std::string ToString(const SourcePos& sourcePos);

} // namespace soul::ast

export namespace soul::ast::source::pos {

} // namespace soul::ast::source::pos
