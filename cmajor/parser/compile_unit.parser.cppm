
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/compile_unit.parser' using soul parser generator spg version 4.1.0

export module cmajor.compile.unit.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;

using namespace cmajor::parser::context;
using namespace cmajor::ast;

export namespace cmajor::compile::unit::parser {

template<typename LexerT>
struct CompileUnitParser
{
    static std::unique_ptr<cmajor::ast::CompileUnitNode> Parse(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match CompileUnit(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match NamespaceContent(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::NamespaceNode* ns);
    static soul::parser::Match UsingDirectives(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::NamespaceNode* ns);
    static soul::parser::Match Definitions(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::NamespaceNode* ns);
    static soul::parser::Match Definition(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Namespace(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::compile::unit::parser