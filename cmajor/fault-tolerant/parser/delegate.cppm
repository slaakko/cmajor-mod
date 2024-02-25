// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.delegate;

import cmajor.fault.tolerant.parser.lexer;
import cmajor.fault.tolerant.parser.context;
import cmajor.fault.tolerant.ast;
import soul.parser;
import std.core;

export namespace cmajor::fault::tolerant::parser {

soul::parser::Match ParseDelegate(Context& context, Lexer& lexer);

soul::parser::Match ParseClassDelegate(Context& context, Lexer& lexer);

} // namespace cmajor::fault::tolerant::parser
