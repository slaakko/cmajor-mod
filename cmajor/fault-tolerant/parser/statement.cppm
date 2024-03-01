// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.parser.statement;

import cmajor.fault.tolerant.parser.lexer;
import cmajor.fault.tolerant.parser.context;
import cmajor.fault.tolerant.ast;
import soul.parser;
import std.core;

export namespace cmajor::fault::tolerant::parser {

soul::parser::Match ParseStatement(Context& context, Lexer& lexer);

soul::parser::Match ParseAssignmentStatementNoSemicolon(Context& context, Lexer& lexer);

soul::parser::Match ParseAssignmentStatement(Context& context, Lexer& lexer);

soul::parser::Match ParseConstructionStatement(Context& context, Lexer& lexer);

soul::parser::Match ParseEmptyStatement(Context& context, Lexer& lexer);

soul::parser::Match ParseCompoundStatement(Context& context, Lexer& lexer);

soul::parser::Match ParseCondCompExpr(Context& context, Lexer& lexer);

} // namespace cmajor::fault::tolerant::parser
