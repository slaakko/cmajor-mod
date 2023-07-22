
// this file has been automatically generated from 'C:/work/cmajor-mod/soul/xml/xpath/xpath.parser' using soul parser generator spg version 5.0.0

export module soul.xml.xpath.parser;

import std.core;
import soul.lexer;
import soul.parser;
import soul.xml.xpath.expr;

using namespace soul::xml::xpath::expr;

export namespace soul::xml::xpath::parser {

template<typename LexerT>
struct XPathParser
{
    static std::unique_ptr<soul::xml::xpath::expr::Expr> Parse(LexerT& lexer);
    static soul::parser::Match Expr(LexerT& lexer);
    static soul::parser::Match OrExpr(LexerT& lexer);
    static soul::parser::Match AndExpr(LexerT& lexer);
    static soul::parser::Match EqualityExpr(LexerT& lexer);
    static soul::parser::Match RelationalExpr(LexerT& lexer);
    static soul::parser::Match AdditiveExpr(LexerT& lexer);
    static soul::parser::Match MultiplicativeExpr(LexerT& lexer);
    static soul::parser::Match UnaryExpr(LexerT& lexer);
    static soul::parser::Match UnionExpr(LexerT& lexer);
    static soul::parser::Match PathExpr(LexerT& lexer);
    static soul::parser::Match LocationPath(LexerT& lexer);
    static soul::parser::Match AbsoluteLocationPath(LexerT& lexer);
    static soul::parser::Match AbbreviatedAbsoluteLocationPath(LexerT& lexer);
    static soul::parser::Match RelativeLocationPath(LexerT& lexer);
    static soul::parser::Match Step(LexerT& lexer);
    static soul::parser::Match AxisSpecifier(LexerT& lexer);
    static soul::parser::Match AxisName(LexerT& lexer);
    static soul::parser::Match AbbreviatedAxisSpecifier(LexerT& lexer);
    static soul::parser::Match NodeTest(LexerT& lexer);
    static soul::parser::Match Literal(LexerT& lexer);
    static soul::parser::Match NodeType(LexerT& lexer);
    static soul::parser::Match NameTest(LexerT& lexer);
    static soul::parser::Match NCName(LexerT& lexer);
    static soul::parser::Match QName(LexerT& lexer);
    static soul::parser::Match PrefixedName(LexerT& lexer);
    static soul::parser::Match Prefix(LexerT& lexer);
    static soul::parser::Match UnprefixedName(LexerT& lexer);
    static soul::parser::Match LocalPart(LexerT& lexer);
    static soul::parser::Match Predicate(LexerT& lexer);
    static soul::parser::Match AbbreviatedStep(LexerT& lexer);
    static soul::parser::Match FunctionCall(LexerT& lexer);
    static soul::parser::Match FunctionName(LexerT& lexer);
    static soul::parser::Match Argument(LexerT& lexer);
    static soul::parser::Match FilterExpr(LexerT& lexer);
    static soul::parser::Match PrimaryExpr(LexerT& lexer);
    static soul::parser::Match VariableReference(LexerT& lexer);
    static soul::parser::Match Number(LexerT& lexer);
    static soul::parser::Match EqualityOp(LexerT& lexer);
    static soul::parser::Match RelationalOp(LexerT& lexer);
    static soul::parser::Match AdditiveOp(LexerT& lexer);
    static soul::parser::Match MultiplicativeOp(LexerT& lexer);
    static soul::parser::Match CombinePathOp(LexerT& lexer);
    static soul::parser::Match OrKeyword(LexerT& lexer);
    static soul::parser::Match AndKeyword(LexerT& lexer);
    static soul::parser::Match DivKeyword(LexerT& lexer);
    static soul::parser::Match ModKeyword(LexerT& lexer);
    static soul::parser::Match ProcessingInstructionKeyword(LexerT& lexer);
};

} // namespace soul::xml::xpath::parser
