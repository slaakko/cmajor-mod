
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/function.parser' using soul parser generator spg version 4.1.0

export module cmajor.function.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;

using namespace cmajor::parser::context;
using namespace cmajor::ast;

export namespace cmajor::function::parser {

template<typename LexerT>
struct FunctionParser
{
    static soul::parser::Match Function(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match FunctionGroupId(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match OperatorFunctionGroupId(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::function::parser
