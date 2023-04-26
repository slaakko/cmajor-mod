
// this file has been automatically generated from 'C:/work/soul-mod/soul/xml/xml_parser/xml_document_parser.parser' using soul parser generator spg version 4.0.0

export module soul.xml.document.parser;

import std.core;
import soul.lexer;
import soul.parser;
import soul.xml.processor;
import soul.lexer.trivial;

using namespace soul::xml::processor;
using namespace soul::lexer::trivial;

export namespace soul::xml::document::parser {

template<typename Lexer>
struct XmlParser
{
    static void Parse(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Document(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Prolog(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match XmlDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match VersionInfo(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match S(Lexer& lexer);
    static soul::parser::Match Eq(Lexer& lexer);
    static soul::parser::Match VersionNumber(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match VersionNumDQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match VersionNumSQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match VersionNum(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncodingDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncodingName(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncNameDQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncNameSQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EncName(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match StandaloneDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match YesNo(Lexer& lexer);
    static soul::parser::Match Misc(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Comment(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Char(Lexer& lexer);
    static soul::parser::Match PI(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PITarget(Lexer& lexer);
    static soul::parser::Match Name(Lexer& lexer);
    static soul::parser::Match NameStartChar(Lexer& lexer);
    static soul::parser::Match NameChar(Lexer& lexer);
    static soul::parser::Match Xml(Lexer& lexer);
    static soul::parser::Match Element(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Attribute(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttValue(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttValueDQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttValueSQ(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Reference(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match CharRef(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match DecCodePoint(Lexer& lexer);
    static soul::parser::Match HexCodePoint(Lexer& lexer);
    static soul::parser::Match EntityRef(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match Content(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match CharData(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match CharDataChar(Lexer& lexer);
    static soul::parser::Match CDSect(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match ETag(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match DocTypeDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match ExternalID(Lexer& lexer);
    static soul::parser::Match SystemLiteral(Lexer& lexer);
    static soul::parser::Match PubidLiteral(Lexer& lexer);
    static soul::parser::Match PubidChar(Lexer& lexer);
    static soul::parser::Match IntSubset(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match MarkupDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match ElementDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match ContentSpec(Lexer& lexer);
    static soul::parser::Match Mixed(Lexer& lexer);
    static soul::parser::Match Choice(Lexer& lexer);
    static soul::parser::Match CP(Lexer& lexer);
    static soul::parser::Match Seq(Lexer& lexer);
    static soul::parser::Match Children(Lexer& lexer);
    static soul::parser::Match AttlistDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttDef(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match AttType(Lexer& lexer);
    static soul::parser::Match StringType(Lexer& lexer);
    static soul::parser::Match TokenizedType(Lexer& lexer);
    static soul::parser::Match EnumeratedType(Lexer& lexer);
    static soul::parser::Match NotationType(Lexer& lexer);
    static soul::parser::Match Enumeration(Lexer& lexer);
    static soul::parser::Match Nmtoken(Lexer& lexer);
    static soul::parser::Match DefaultDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EntityDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match GEDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EntityDef(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match EntityValue(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PEReference(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PEDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PEDef(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match NDataDecl(Lexer& lexer);
    static soul::parser::Match NotationDecl(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match DeclSep(Lexer& lexer, soul::xml::processor::XmlProcessor* processor);
    static soul::parser::Match PublicID(Lexer& lexer);
};

} // namespace soul::xml::document::parser
