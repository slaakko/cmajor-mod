// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module soul.xml.dom.parser;

import std.core;
import soul.xml.document;
import soul.lexer.file.map;
import util;

export namespace soul::xml {

enum class ParsingFlags : int
{
    none = 0, generateWarnings = 1 << 0
};

constexpr ParsingFlags operator|(ParsingFlags left, ParsingFlags right)
{
    return ParsingFlags(int(left) | int(right));
}

constexpr ParsingFlags operator&(ParsingFlags left, ParsingFlags right)
{
    return ParsingFlags(int(left) & int(right));
}

constexpr ParsingFlags operator~(ParsingFlags flags)
{
    return ParsingFlags(~int(flags));
}

//  =======================================================================================================
//  ParseXmlFile parses given UTF-8 encoded XML file to a DOM document.
//  The FileMap can be used to generate error messages containing source locations after successul parsing. 
//  (see soul::lexer::MakeMessage)
//  =======================================================================================================

std::unique_ptr<soul::xml::Document> ParseXmlFile(const std::string& xmlFileName);
std::unique_ptr<soul::xml::Document> ParseXmlFile(const std::string& xmlFileName, ParsingFlags parsingFlags);
std::unique_ptr<soul::xml::Document> ParseXmlFile(const std::string& xmlFileName, soul::lexer::FileMap& fileMap);
std::unique_ptr<soul::xml::Document> ParseXmlFile(const std::string& xmlFileName, soul::lexer::FileMap& fileMap, ParsingFlags parsingFlags);

//  =======================================================================================================
//  ParseXmlContent parses given UTF-8 or UTF-32 encoded XML string to a DOM document.
//  The systemId parameter is used for error messages only. 
//  It can be for example a file name or URL that identifies the XML string to parse.
//  The FileMap can be used to generate error messages containing source locations after successul parsing. 
//  (see soul::lexer::MakeMessage)
//  =======================================================================================================

std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::string& xmlContent, const std::string& systemId);
std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::string& xmlContent, const std::string& systemId, ParsingFlags parsingFlags);
std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::string& xmlContent, const std::string& systemId, soul::lexer::FileMap& fileMap);
std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::string& xmlContent, const std::string& systemId, soul::lexer::FileMap& fileMap, ParsingFlags parsingFlags);
std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId);
std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId, ParsingFlags parsingFlags);
std::unique_ptr<soul::xml::Document> ParseXmlContent(std::u32string&& xmlContent, const std::string& systemId, soul::lexer::FileMap& fileMap);
std::unique_ptr<soul::xml::Document> ParseXmlContent(std::u32string&& xmlContent, const std::string& systemId, soul::lexer::FileMap& fileMap, ParsingFlags parsingFlags);

void SendDocument(util::TcpSocket& socket, soul::xml::Document& document);
std::unique_ptr<soul::xml::Document> ReceiveDocument(util::TcpSocket& socket);

} // namespace soul::xml
