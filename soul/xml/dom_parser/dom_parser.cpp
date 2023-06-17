// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.dom.parser;

import soul.xml.document.handler;
import soul.xml.parser;

namespace soul::xml {

std::unique_ptr<soul::xml::Document> ParseXmlFile(const std::string& xmlFileName)
{
    return ParseXmlFile(xmlFileName, ParsingFlags::none);
}

std::unique_ptr<soul::xml::Document> ParseXmlFile(const std::string& xmlFileName, ParsingFlags parsingFlags)
{
    DocumentHandler documentHandler(parsingFlags);
    soul::xml::parser::ParseXmlFile(xmlFileName, &documentHandler);
    return documentHandler.GetDocument();
}

std::unique_ptr<soul::xml::Document> ParseXmlFile(const std::string& xmlFileName, soul::lexer::FileMap& fileMap)
{
    return ParseXmlFile(xmlFileName, fileMap, ParsingFlags::none);
}

std::unique_ptr<soul::xml::Document> ParseXmlFile(const std::string& xmlFileName, soul::lexer::FileMap& fileMap, ParsingFlags parsingFlags)
{
    DocumentHandler documentHandler(parsingFlags);
    soul::xml::parser::ParseXmlFile(xmlFileName, &documentHandler, fileMap);
    return documentHandler.GetDocument();
}

std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::string& xmlContent, const std::string& systemId)
{
    return ParseXmlContent(xmlContent, systemId, ParsingFlags::none);
}

std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::string& xmlContent, const std::string& systemId, ParsingFlags parsingFlags)
{
    DocumentHandler documentHandler(parsingFlags);
    soul::xml::parser::ParseXmlContent(xmlContent, systemId, &documentHandler);
    return documentHandler.GetDocument();
}

std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::string& xmlContent, const std::string& systemId, soul::lexer::FileMap& fileMap)
{
    return ParseXmlContent(xmlContent, systemId, fileMap, ParsingFlags::none);
}

std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::string& xmlContent, const std::string& systemId, soul::lexer::FileMap& fileMap, ParsingFlags parsingFlags)
{
    DocumentHandler documentHandler(parsingFlags);
    soul::xml::parser::ParseXmlContent(xmlContent, systemId, &documentHandler, fileMap);
    return documentHandler.GetDocument();
}

std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId)
{
    return ParseXmlContent(xmlContent, systemId, ParsingFlags::none);
}

std::unique_ptr<soul::xml::Document> ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId, ParsingFlags parsingFlags)
{
    DocumentHandler documentHandler(parsingFlags);
    soul::xml::parser::ParseXmlContent(xmlContent, systemId, &documentHandler);
    return documentHandler.GetDocument();
}

std::unique_ptr<soul::xml::Document> ParseXmlContent(std::u32string&& xmlContent, const std::string& systemId, soul::lexer::FileMap& fileMap)
{
    return ParseXmlContent(std::move(xmlContent), systemId, fileMap, ParsingFlags::none);
}

std::unique_ptr<soul::xml::Document> ParseXmlContent(std::u32string&& xmlContent, const std::string& systemId, soul::lexer::FileMap& fileMap, ParsingFlags parsingFlags)
{
    DocumentHandler documentHandler(parsingFlags);
    soul::xml::parser::ParseXmlContent(std::move(xmlContent), systemId, &documentHandler, fileMap);
    return documentHandler.GetDocument();
}

void SendDocument(util::TcpSocket& socket, soul::xml::Document& document)
{
    std::stringstream sstream;
    util::CodeFormatter formatter(sstream);
    document.Write(formatter);
    Write(socket, sstream.str());
}

std::unique_ptr<soul::xml::Document> ReceiveDocument(util::TcpSocket& socket)
{
    std::string str = ReadStr(socket);
    if (str.empty()) return std::unique_ptr<Document>();
    std::u32string content = util::ToUtf32(str);
    std::unique_ptr<soul::xml::Document> doc = ParseXmlContent(content, "socket");
    return doc;
}

} // namespace soul::xml
