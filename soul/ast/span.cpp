// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.ast.span;

namespace soul::ast {

void Span::Union(const Span& that)
{
    int end = that.pos + that.len;
    len = end - pos;
}

LineColLen SpanToLineColLen(const Span& span, const std::vector<int>& lineStarts)
{
    if (!span.IsValid() || lineStarts.empty())
    {
        return LineColLen();
    }
    else
    {
        auto it = std::lower_bound(lineStarts.begin(), lineStarts.end(), span.pos);
        int lineStart = 0;
        int line = 0;
        if (it != lineStarts.end())
        {
            if (it != lineStarts.begin() && *it > span.pos)
            {
                --it;
            }
            line = static_cast<int>(it - lineStarts.begin());
            lineStart = lineStarts[line];
        }
        else
        {
            line = lineStarts.size();
            lineStart = lineStarts[line - 1];
        }
        int col = span.pos - lineStart + 1;
        int len = span.len;
        if (line == 0)
        {
            ++line;
        }
        return LineColLen(line, col, len);
    }
}

int LineColLenToPos(const LineColLen& lineColLen, const std::vector<int>& lineStarts)
{
    if (!lineColLen.IsValid() || lineStarts.empty() || lineColLen.line >= lineStarts.size())
    {
        return -1;
    }
    int pos = lineStarts[lineColLen.line] + lineColLen.col - 1;
    return pos;
}

} // namespace soul::ast
