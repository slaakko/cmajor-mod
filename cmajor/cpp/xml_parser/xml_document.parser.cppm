
// this file has been automatically generated from 'C:/work/cmajor-mod/cmajor/cpp/xml_parser/xml_document.parser' using soul parser generator spg version 4.1.0

export module soul.xml.document.parser;

import std.core;
import soul.lexer;
import soul.parser;
import soul.xml.processor;
import soul.lexer.trivial;

using namespace soul::xml::processor;
using namespace soul::lexer::trivial;

export namespace soul::xml::document::parser {

template<typename LexerT>
struct XmlParser
{
    static void Parse(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Document(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Prolog(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match XmlDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match VersionInfo(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match S(LexerT& lexer);
    static soul::parser::Match Eq(LexerT& lexer);
    static soul::parser::Match VersionNumber(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match VersionNumDQ(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match VersionNumSQ(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match VersionNum(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncodingDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncodingName(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncNameDQ(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncNameSQ(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncName(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match StandaloneDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match YesNo(LexerT& lexer);
    static soul::parser::Match Misc(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Comment(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Char(LexerT& lexer);
    static soul::parser::Match PI(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PITarget(LexerT& lexer);
    static soul::parser::Match Name(LexerT& lexer);
    static soul::parser::Match NameStartChar(LexerT& lexer);
    static soul::parser::Match NameChar(LexerT& lexer);
    static soul::parser::Match Xml(LexerT& lexer);
    static soul::parser::Match Element(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Attribute(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttValue(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttValueDQ(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttValueSQ(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Reference(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match CharRef(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match DecCodePoint(LexerT& lexer);
    static soul::parser::Match HexCodePoint(LexerT& lexer);
    static soul::parser::Match EntityRef(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Content(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match CharData(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match CharDataChar(LexerT& lexer);
    static soul::parser::Match CDSect(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match ETag(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match DocTypeDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match ExternalID(LexerT& lexer);
    static soul::parser::Match SystemLiteral(LexerT& lexer);
    static soul::parser::Match PubidLiteral(LexerT& lexer);
    static soul::parser::Match PubidChar(LexerT& lexer);
    static soul::parser::Match IntSubset(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match MarkupDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match ElementDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match ContentSpec(LexerT& lexer);
    static soul::parser::Match Mixed(LexerT& lexer);
    static soul::parser::Match Choice(LexerT& lexer);
    static soul::parser::Match CP(LexerT& lexer);
    static soul::parser::Match Seq(LexerT& lexer);
    static soul::parser::Match Children(LexerT& lexer);
    static soul::parser::Match AttlistDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttDef(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttType(LexerT& lexer);
    static soul::parser::Match StringType(LexerT& lexer);
    static soul::parser::Match TokenizedType(LexerT& lexer);
    static soul::parser::Match EnumeratedType(LexerT& lexer);
    static soul::parser::Match NotationType(LexerT& lexer);
    static soul::parser::Match Enumeration(LexerT& lexer);
    static soul::parser::Match Nmtoken(LexerT& lexer);
    static soul::parser::Match DefaultDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EntityDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match GEDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EntityDef(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EntityValue(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PEReference(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PEDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PEDef(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match NDataDecl(LexerT& lexer);
    static soul::parser::Match NotationDecl(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match DeclSep(LexerT& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PublicID(LexerT& lexer);
};

} // namespace soul::xml::document::parser
