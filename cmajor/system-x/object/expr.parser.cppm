
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/system-x/object/expr.parser' using soul parser generator spg version 5.0.0

export module cmajor.systemx.object.expr.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.systemx.object.expr;

using namespace cmajor::systemx::object::expr;

export namespace cmajor::systemx::object::expr::parser {

template<typename LexerT>
struct ExprParser
{
    static std::unique_ptr<cmajor::systemx::object::Expr> Parse(LexerT& lexer);
    static soul::parser::Match Expr(LexerT& lexer);
    static soul::parser::Match UnaryExpr(LexerT& lexer);
    static soul::parser::Match PostfixExpr(LexerT& lexer);
    static soul::parser::Match PrimaryExpr(LexerT& lexer);
    static soul::parser::Match DollarExpr(LexerT& lexer);
};

} // namespace cmajor::systemx::object::expr::parser
