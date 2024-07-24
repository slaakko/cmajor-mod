// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.source_token_parser;

import cmajor.ast;
import std.core;

export namespace cmajor::parser {

void ParseSourceLine(const std::string& sourceLine, cmajor::ast::SourceTokenFormatter* formatter, bool& inBlockComment);

} // namespace cmajor::parser
