
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/specifier.parser' using soul parser generator spg version 5.0.0

export module cmajor.specifier.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::specifier::parser {

template<typename LexerT>
struct SpecifierParser
{
    static soul::parser::Match Specifiers(LexerT& lexer);
    static soul::parser::Match Specifier(LexerT& lexer);
};

} // namespace cmajor::specifier::parser
