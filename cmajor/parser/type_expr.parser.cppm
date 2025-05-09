
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/type_expr.parser' using soul parser generator spg version 5.0.0

export module cmajor.type.expr.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::type::expr::parser {

template<typename LexerT>
struct TypeExprParser
{
    static std::unique_ptr<cmajor::ast::Node> Parse(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match TypeExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match PrefixTypeExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match PostfixTypeExpr(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match PrimaryTypeExpr(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::type::expr::parser
