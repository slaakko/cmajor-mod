
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/parameter.parser' using soul parser generator spg version 5.0.0

export module cmajor.parameter.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::parameter::parser {

template<typename LexerT>
struct ParameterParser
{
    static soul::parser::Match ParameterList(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::Node* owner);
    static soul::parser::Match Parameter(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::parameter::parser
