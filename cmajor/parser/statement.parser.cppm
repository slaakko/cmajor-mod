
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/statement.parser' using soul parser generator spg version 5.0.0

export module cmajor.statement.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::statement::parser {

template<typename LexerT>
struct StatementParser
{
    static soul::parser::Match Statement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Label(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match LabeledStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ControlStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match CompoundStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ReturnStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match IfStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match WhileStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match DoStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match RangeForStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ForStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ForInitStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ForLoopStatementExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match BreakStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ContinueStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match GotoStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match SwitchStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match CaseStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match DefaultStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match GotoCaseStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match GotoDefaultStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ExpressionStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match AssignmentStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match AssignmentStatementExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConstructionStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match DeleteStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match DestroyStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match EmptyStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ThrowStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match TryStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Catch(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match AssertStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConditionalCompilationStatement(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConditionalCompilationExpression(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConditionalCompilationDisjunction(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConditionalCompilationConjunction(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConditionalCompilationPrefix(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConditionalCompilationPrimary(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::statement::parser
