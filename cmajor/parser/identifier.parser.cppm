
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/identifier.parser' using soul parser generator spg version 5.0.0

export module cmajor.identifier.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::identifier::parser {

template<typename LexerT>
struct IdentifierParser
{
    static soul::parser::Match Identifier(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match QualifiedId(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::identifier::parser
