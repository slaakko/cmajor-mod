// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.expression;

import cmajor.fault.tolerant.parser.lexer;
import cmajor.fault.tolerant.parser.context;
import cmajor.fault.tolerant.ast;
import soul.parser;
import std.core;

export namespace cmajor::fault::tolerant::parser {

soul::parser::Match ParseExpression(Context& context, Lexer& lexer);

void ParseArgumentList(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::Node* container);

soul::parser::Match ParseExpressionList(Context& context, Lexer& lexer, cmajor::fault::tolerant::ast::Node* container);

soul::parser::Match ParseInvokeExpr(Context& context, Lexer& lexer);

} // namespace cmajor::fault::tolerant::parser
