
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/interface.parser' using soul parser generator spg version 5.0.0

export module cmajor.interface.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::interface::parser {

template<typename LexerT>
struct InterfaceParser
{
    static soul::parser::Match Interface(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match InterfaceContent(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::InterfaceNode* interfaceNode);
    static soul::parser::Match InterfaceMemberFunction(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match InterfaceFunctionGroupId(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::interface::parser
