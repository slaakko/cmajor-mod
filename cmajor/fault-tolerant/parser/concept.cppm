// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.concepts;

import cmajor.fault.tolerant.parser.lexer;
import cmajor.fault.tolerant.parser.context;
import cmajor.fault.tolerant.ast;
import soul.parser;
import std.core;

export namespace cmajor::fault::tolerant::parser {

soul::parser::Match ParseWhereConstraint(Context& context, Lexer& lexer);

soul::parser::Match ParseConcept(Context& context, Lexer& lexer);

} // namespace cmajor::fault::tolerant::parser
