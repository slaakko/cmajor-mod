// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.parser;

import std.core;
import soul.xml.content.handler;
import soul.lexer.file.map;

export namespace soul::xml::parser {

//  =============================================================================
//  ParseXmlFile parses given UTF-8 encoded XML file using given content handler.
//  =============================================================================

void ParseXmlFile(const std::string& xmlFileName, XmlContentHandler* contentHandler);

//  =======================================================================================================
//  The FileMap can be used to generate error messages containing source locations after successul parsing. 
//  (see soul::lexer::MakeMessage)
//  =======================================================================================================

void ParseXmlFile(const std::string& xmlFileName, XmlContentHandler* contentHandler, soul::lexer::FileMap& fileMap);

//  ============================================================================================
//  ParseXmlContent parses given UTF-8 or UTF-32 encoded XML string using given content handler.
//  The systemId parameter is used for error messages only. 
//  It can be for example a file name or URL that identifies the XML string to parse.
//  ============================================================================================

void ParseXmlContent(const std::string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler);

//  =======================================================================================================
//  The FileMap can be used to generate error messages containing source locations after successul parsing. 
//  (see soul::lexer::MakeMessage)
//  =======================================================================================================

void ParseXmlContent(const std::string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler, soul::lexer::FileMap& fileMap);

void ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler);

void ParseXmlContent(std::u32string&& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler, soul::lexer::FileMap& fileMap);

} // namespace soul::xml::parser
