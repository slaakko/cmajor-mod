
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/bm/schema.parser' using soul parser generator spg version 4.1.0

export module cmajor.binary.message.schema.parser;

import std.core;
import soul.lexer;
import soul.parser;
import cmajor.binary.message.schema.ast;

using namespace cmajor::binary::message::schema::ast;

export namespace cmajor::binary::message::schema::parser {

template<typename LexerT>
struct BinaryMessageSchemaParser
{
    static std::unique_ptr<cmajor::binary::message::schema::ast::SourceFileNode> Parse(LexerT& lexer);
    static soul::parser::Match SourceFile(LexerT& lexer);
    static soul::parser::Match NamespaceContent(LexerT& lexer, cmajor::binary::message::schema::ast::NamespaceNode* ns);
    static soul::parser::Match Declaration(LexerT& lexer);
    static soul::parser::Match ForwardClassDeclaration(LexerT& lexer);
    static soul::parser::Match UsingAliasDeclaration(LexerT& lexer);
    static soul::parser::Match Definition(LexerT& lexer);
    static soul::parser::Match Class(LexerT& lexer);
    static soul::parser::Match ClassContent(LexerT& lexer, cmajor::binary::message::schema::ast::ClassNode* cls);
    static soul::parser::Match MemberVariable(LexerT& lexer);
    static soul::parser::Match Type(LexerT& lexer);
    static soul::parser::Match ScalarType(LexerT& lexer);
    static soul::parser::Match ClassType(LexerT& lexer);
    static soul::parser::Match Enum(LexerT& lexer);
    static soul::parser::Match EnumContent(LexerT& lexer, cmajor::binary::message::schema::ast::EnumTypeNode* enm);
    static soul::parser::Match EnumConstant(LexerT& lexer);
    static soul::parser::Match Namespace(LexerT& lexer);
    static soul::parser::Match QualifiedId(LexerT& lexer);
    static soul::parser::Match QualifiedCppId(LexerT& lexer);
    static soul::parser::Match ExportModule(LexerT& lexer);
    static soul::parser::Match Import(LexerT& lexer);
    static soul::parser::Match ImportPrefix(LexerT& lexer);
    static soul::parser::Match ExportKeyword(LexerT& lexer);
    static soul::parser::Match ModuleKeyword(LexerT& lexer);
    static soul::parser::Match ImportKeyword(LexerT& lexer);
    static soul::parser::Match ImplementationPrefix(LexerT& lexer);
    static soul::parser::Match InterfacePrefix(LexerT& lexer);
    static soul::parser::Match ImplementationKeyword(LexerT& lexer);
    static soul::parser::Match InterfaceKeyword(LexerT& lexer);
};

} // namespace cmajor::binary::message::schema::parser
