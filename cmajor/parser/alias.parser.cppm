
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/alias.parser' using soul parser generator spg version 5.0.0

export module cmajor.alias.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::alias::parser {

template<typename LexerT>
struct AliasParser
{
    static soul::parser::Match UsingAlias(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Typedef(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::alias::parser
