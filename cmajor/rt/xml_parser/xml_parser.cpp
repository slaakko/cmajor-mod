// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#include <xml_parser/xml_parser.hpp>
#include <xml_parser/xml_document.parser.hpp>
#include <xml_parser/xml_parser_rules.hpp>

namespace soul::xml::parser {

void ParseXmlFile(const std::string& xmlFileName, XmlContentHandler* contentHandler)
{
    std::string xmlContent = util::ReadFile(xmlFileName);
    std::u32string fileContent;
    try
    {
        fileContent = util::ToUtf32(xmlContent);
    }
    catch (const util::UnicodeException& ex)
    {
        util::ThrowUnicodeException(std::string(ex.what()) + ", file=" + xmlFileName);
    }
    ParseXmlContent(fileContent, xmlFileName, contentHandler);
}

void ParseXmlFile(const std::string& xmlFileName, XmlContentHandler* contentHandler, soul::lexer::FileMap& fileMap)
{
    std::string xmlContent = util::ReadFile(xmlFileName);
    std::u32string fileContent; 
    try
    {
        fileContent = util::ToUtf32(xmlContent);
    }
    catch (const util::UnicodeException& ex)
    {
        util::ThrowUnicodeException(std::string(ex.what()) + ", file=" + xmlFileName);
    }
    return ParseXmlContent(std::move(fileContent), xmlFileName, contentHandler, fileMap);
}

void ParseXmlContent(const std::string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler)
{
    ParseXmlContent(util::ToUtf32(xmlContent), systemId, contentHandler);
}

void ParseXmlContent(const std::string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler, soul::lexer::FileMap& fileMap)
{
    ParseXmlContent(std::move(util::ToUtf32(xmlContent)), systemId, contentHandler, fileMap);
}

void ParseXmlContent(const std::u32string& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler)
{
    auto lexer = soul::lexer::trivial::MakeLexer(xmlContent.c_str(), xmlContent.c_str() + xmlContent.length(), systemId);
    lexer.SetRuleNameMapPtr(soul::xml::parser::rules::GetRuleNameMapPtr());
    soul::xml::processor::XmlProcessor processor(contentHandler);
    using LexerType = decltype(lexer);
    soul::xml::document::parser::XmlParser<LexerType>::Parse(lexer, &processor);
}

void ParseXmlContent(std::u32string&& xmlContent, const std::string& systemId, XmlContentHandler* contentHandler, soul::lexer::FileMap& fileMap)
{
    auto lexer = soul::lexer::trivial::MakeLexer(xmlContent.c_str(), xmlContent.c_str() + xmlContent.length(), systemId);
    lexer.SetRuleNameMapPtr(soul::xml::parser::rules::GetRuleNameMapPtr());
    int32_t fileId = fileMap.MapFile(systemId);
    lexer.SetFile(fileId);
    soul::xml::processor::XmlProcessor processor(contentHandler);
    using LexerType = decltype(lexer);
    soul::xml::document::parser::XmlParser<LexerType>::Parse(lexer, &processor);
    fileMap.AddFileContent(fileId, std::move(xmlContent), std::move(lexer.GetLineStartIndeces()));
}

} // namespace soul::xml::parser
