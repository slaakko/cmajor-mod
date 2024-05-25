
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cpp/xpath/xpath.parser' using soul parser generator spg version 5.0.0

#ifndef INCLUDE_GUARD_8834061ED3ECF80DC78E6D490EB0AC29D46C3D9A
#define INCLUDE_GUARD_8834061ED3ECF80DC78E6D490EB0AC29D46C3D9A

// export module soul.xml.xpath.parser;

#include <../lexer/lexer.hpp>
#include <../parser/parser.hpp>
#include <../util/util.hpp>
#include <../ast/spg.hpp>
#include <expr.hpp>
#include <xpath.lexer.hpp>
#include <xpath.token.hpp>
#include <token_parser.hpp>

namespace soul::xml::xpath::parser {

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

#endif // INCLUDE_GUARD_8834061ED3ECF80DC78E6D490EB0AC29D46C3D9A
