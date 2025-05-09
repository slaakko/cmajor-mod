
// this file has been automatically generated from 'D:/work/cmajor-mod/cmajor/parser/class.parser' using soul parser generator spg version 5.0.0

export module cmajor.classes.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.parser.context;
import cmajor.ast;
import soul.ast.span;

using namespace cmajor::parser::context;
using namespace cmajor::ast;
using namespace soul::ast::span;

export namespace cmajor::classes::parser {

template<typename LexerT>
struct ClassParser
{
    static soul::parser::Match Class(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match InheritanceAndInterfaces(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode);
    static soul::parser::Match BaseClassOrInterface(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ClassContent(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode);
    static soul::parser::Match ClassMember(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode);
    static soul::parser::Match StaticConstructor(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode);
    static soul::parser::Match Constructor(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode);
    static soul::parser::Match Destructor(LexerT& lexer, cmajor::parser::context::Context* context, cmajor::ast::ClassNode* classNode);
    static soul::parser::Match Initializer(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match MemberFunction(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match ConversionFunction(LexerT& lexer, cmajor::parser::context::Context* context);
    static soul::parser::Match MemberVariable(LexerT& lexer, cmajor::parser::context::Context* context);
};

} // namespace cmajor::classes::parser
