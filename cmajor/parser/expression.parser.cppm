
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/expression.parser' using soul parser generator spg version 5.0.0

export module cmajor.expression.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::expression::parser {

template<typename LexerT>
struct ExpressionParser
{
    static soul::parser::Match Expression(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Equivalence(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Implication(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Disjunction(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Conjunction(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match BitOr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match BitXor(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match BitAnd(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Equality(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match EqOp(LexerT& lexer);
    static soul::parser::Match Relational(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match RelOp(LexerT& lexer);
    static soul::parser::Match Shift(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ShiftOp(LexerT& lexer);
    static soul::parser::Match Additive(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match AddOp(LexerT& lexer);
    static soul::parser::Match Multiplicative(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match MulOp(LexerT& lexer);
    static soul::parser::Match Prefix(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match PrefixOp(LexerT& lexer);
    static soul::parser::Match Postfix(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Primary(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ArgumentList(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::Node* node);
    static soul::parser::Match ExpressionList(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::Node* node);
    static soul::parser::Match SizeOfExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match TypeNameExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match TypeIdExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match CastExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConstructExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match NewExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match InvokeExpr(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::expression::parser
