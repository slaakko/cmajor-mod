
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/literal.parser' using soul parser generator spg version 4.1.0

export module cmajor.literal.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;

using namespace cmajor::parser::context;
using namespace cmajor::ast;

export namespace cmajor::literal::parser {

template<typename LexerT>
struct LiteralParser
{
    static soul::parser::Match Literal(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match SimpleLiteral(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ComplexLiteral(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ArrayLiteral(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match StructuredLiteral(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::literal::parser
