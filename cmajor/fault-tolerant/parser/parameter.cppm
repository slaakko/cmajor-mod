// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.parameter;

import cmajor.fault.tolerant.parser.lexer;
import cmajor.fault.tolerant.parser.context;
import cmajor.fault.tolerant.ast;
import soul.parser;
import std.core;

export namespace cmajor::fault::tolerant::parser {

soul::parser::Match ParseParameter(Context& context, Lexer& lexer);

soul::parser::Match ParseParameterList(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::ParameterListNode* parameterList);

} // namespace cmajor::fault::tolerant::parser
