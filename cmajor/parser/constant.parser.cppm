
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/constant.parser' using soul parser generator spg version 5.0.0

export module cmajor.constant.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::constant::parser {

template<typename LexerT>
struct ConstantParser
{
    static soul::parser::Match Constant(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::constant::parser
