// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.lexer;

import soul.lexer;
import cmajor.lexers;
import std.core;

export namespace cmajor::fault::tolerant::parser {

using Lexer = soul::lexer::Lexer<cmajor::lexer::CmajorLexer<char16_t>, char16_t>;

using ContainerFileLexer = soul::lexer::Lexer<cmajor::container::file::lexer::CmajorContainerFileLexer<char16_t>, char16_t>;

} // namespace cmajor::fault::tolerant::parser
