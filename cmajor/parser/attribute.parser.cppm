
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/parser/attribute.parser' using soul parser generator spg version 4.1.0

export module cmajor.attribute.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;

using namespace cmajor::parser::context;
using namespace cmajor::ast;

export namespace cmajor::attribute::parser {

template<typename LexerT>
struct AttributeParser
{
    static soul::parser::Match Attributes(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match Attribute(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::AttributesNode* attributes);
};

} // namespace cmajor::attribute::parser
