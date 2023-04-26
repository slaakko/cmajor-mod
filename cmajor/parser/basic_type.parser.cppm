
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/basic_type.parser' using soul parser generator spg version 4.1.0

export module cmajor.basic.type.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;

using namespace cmajor::parser::context;
using namespace cmajor::ast;

export namespace cmajor::basic::type::parser {

template<typename LexerT>
struct BasicTypeParser
{
    static soul::parser::Match BasicType(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::basic::type::parser
