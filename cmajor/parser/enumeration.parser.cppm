
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/enumeration.parser' using soul parser generator spg version 5.0.0

export module cmajor.enumeration.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::enumeration::parser {

template<typename LexerT>
struct EnumerationParser
{
    static soul::parser::Match EnumType(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match UnderlyingType(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match EnumConstants(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::EnumTypeNode* enumType);
    static soul::parser::Match EnumConstant(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::EnumTypeNode* enumType);
};

} // namespace cmajor::enumeration::parser
