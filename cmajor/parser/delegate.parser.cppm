
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/delegate.parser' using soul parser generator spg version 5.0.0

export module cmajor.delegate.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::delegate::parser {

template<typename LexerT>
struct DelegateParser
{
    static soul::parser::Match Delegate(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ClassDelegate(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::delegate::parser
