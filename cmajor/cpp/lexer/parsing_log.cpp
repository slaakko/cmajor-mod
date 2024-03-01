// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <lexer/parsing_log.hpp>

namespace soul::lexer {

ParsingLog::ParsingLog() : maxLineLength(80)
{
}

ParsingLog::ParsingLog(int maxLineLength_) : maxLineLength(maxLineLength_)
{
}

ParsingLog::~ParsingLog()
{
}

int ParsingLog::MaxLineLength() const
{
    return maxLineLength;
}

} // namespace soul::lexer
