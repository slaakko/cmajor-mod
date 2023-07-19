// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <lexer/lexer_class.hpp>

namespace soul::lexer {

bool parsing_error_thrown = false;

bool ParsingErrorThrown()
{
    return parsing_error_thrown;
}

void SetParsingErrorThrown()
{
    parsing_error_thrown = true;
}

} // soul::lexer