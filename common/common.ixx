// =================================
// Copyright (c) 2022 Olli Myöhänen
// Distributed under the MIT license
// =================================

export module cmajor.common;

namespace cmajor::common {

using soulng::lexer::Span;

class ColumnSpanProvider
{
public:
    virtual int GetColumn(const Span& span) const = 0;
};

} // cmajor::common

