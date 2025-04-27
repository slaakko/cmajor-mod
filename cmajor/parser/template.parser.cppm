
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/template.parser' using soul parser generator spg version 5.0.0

export module cmajor.templates.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::templates::parser {

template<typename LexerT>
struct TemplateParser
{
    static soul::parser::Match TemplateId(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match TemplateParameterList(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::Node* owner);
    static soul::parser::Match TemplateParameter(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match InstantiationRequest(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::templates::parser
