
// this file has been automatically generated from 'C:/work/soul-mod/soul/xml/serialization/xmlser.parser' using soul parser generator spg version 4.1.0

export module soul.xml.serialization.parser;

import std.core;
import soul.lexer;
import soul.parser;
import soul.xml.serialization.ast;

using namespace soul::xml::serialization::ast;

export namespace soul::xml::serialization::parser {

template<typename LexerT>
struct XmlSerParser
{
    static std::unique_ptr<soul::xml::serialization::ast::SourceFileNode> Parse(LexerT& lexer);
    static soul::parser::Match SourceFile(LexerT& lexer);
    static soul::parser::Match NamespaceContent(LexerT& lexer, soul::xml::serialization::ast::NamespaceNode* ns);
    static soul::parser::Match Declaration(LexerT& lexer);
    static soul::parser::Match ForwardClassDeclaration(LexerT& lexer);
    static soul::parser::Match UsingAliasDeclaration(LexerT& lexer);
    static soul::parser::Match Definition(LexerT& lexer);
    static soul::parser::Match Class(LexerT& lexer);
    static soul::parser::Match Inheritance(LexerT& lexer, soul::xml::serialization::ast::ClassNode* cls);
    static soul::parser::Match ClassContent(LexerT& lexer, soul::xml::serialization::ast::ClassNode* cls);
    static soul::parser::Match MemberVariable(LexerT& lexer);
    static soul::parser::Match Type(LexerT& lexer);
    static soul::parser::Match ScalarType(LexerT& lexer);
    static soul::parser::Match ClassType(LexerT& lexer);
    static soul::parser::Match CppBlock(LexerT& lexer, bool implementation);
    static soul::parser::Match Enum(LexerT& lexer);
    static soul::parser::Match EnumContent(LexerT& lexer, soul::xml::serialization::ast::EnumTypeNode* enm);
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

} // namespace soul::xml::serialization::parser
