
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/global_variable.parser' using soul parser generator spg version 4.1.0

export module cmajor.global.variable.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;

using namespace cmajor::parser::context;
using namespace cmajor::ast;

export namespace cmajor::global::variable::parser {

template<typename LexerT>
struct GlobalVariableParser
{
    static soul::parser::Match GlobalVariable(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::global::variable::parser