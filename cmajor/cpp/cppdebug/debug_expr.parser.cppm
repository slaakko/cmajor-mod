
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cpp/cppdebug/debug_expr.parser' using soul parser generator spg version 5.0.0

export module cmajor.debug.expr.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.debug.expr;

using namespace cmajor::debug::expr;

export namespace cmajor::debug::expr::parser {

template<typename LexerT>
struct DebugExprParser
{
    static std::unique_ptr<cmajor::debug::DebugExprNode> Parse(LexerT& lexer);
    static soul::parser::Match DebugExpr(LexerT& lexer);
    static soul::parser::Match DisjunctiveDebugExpr(LexerT& lexer);
    static soul::parser::Match ConjunctiveDebugExpr(LexerT& lexer);
    static soul::parser::Match BitOrDebugExpr(LexerT& lexer);
    static soul::parser::Match BitXorDebugExpr(LexerT& lexer);
    static soul::parser::Match BitAndDebugExpr(LexerT& lexer);
    static soul::parser::Match EqualityDebugExpr(LexerT& lexer);
    static soul::parser::Match RelationalDebugExpr(LexerT& lexer);
    static soul::parser::Match ShiftDebugExpr(LexerT& lexer);
    static soul::parser::Match AdditiveDebugExpr(LexerT& lexer);
    static soul::parser::Match MultiplicativeDebugExpr(LexerT& lexer);
    static soul::parser::Match PrefixDebugExpr(LexerT& lexer);
    static soul::parser::Match PostfixDebugExpr(LexerT& lexer);
    static soul::parser::Match PrimaryDebugExpr(LexerT& lexer);
    static soul::parser::Match CastDebugExpr(LexerT& lexer);
    static soul::parser::Match TypeIdExpr(LexerT& lexer);
};

} // namespace cmajor::debug::expr::parser
