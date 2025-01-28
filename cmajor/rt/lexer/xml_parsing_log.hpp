// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef LEXER_XML_PARSING_LOG_INCLUDED
#define LEXER_XML_PARSING_LOG_INCLUDED
#include <lexer/parsing_log.hpp>
#include <util/code_formatter.hpp>
#include <util/text_util.hpp>
#include <util/unicode.hpp>

namespace soul::lexer {

class XmlParsingLog : public ParsingLog
{
public:
    XmlParsingLog(std::ostream& stream_);
    XmlParsingLog(std::ostream& stream_, int maxLineLength_);
    void IncIndent() override;
    void DecIndent() override;
    void WriteBeginRule(const std::string& ruleName) override;
    void WriteEndRule(const std::string& ruleName) override;
    void WriteTry(const std::string& s) override;
    void WriteSuccess(const std::string& match) override;
    void WriteFail() override;
    void WriteElement(const std::string& elementName, const std::string& elementContent);
    void Write(const std::string& s);
    util::CodeFormatter& Formatter() { return formatter; }
private:
    util::CodeFormatter formatter;
};

template<typename Char>
std::string XmlHexEscape(Char c)
{
    return "&#x" + util::ToHexString(uint32_t(c)) + ";";
}

template<typename Char>
std::string XmlCharStr(Char c)
{
    switch (c)
    {
    case '&': return "&amp;";
    case '<': return "&lt;";
    case '>': return "&gt;";
    case '\a': return "\\a";
    case '\b': return "\\b";
    case '\f': return "\\f";
    case '\n': return "\\n";
    case '\r': return "\\r";
    case '\t': return "\\t";
    case '\v': return "\\v";
    default:
    {
        if ((int32_t(c) >= 32 && int32_t(c) <= 126))
        {
            return std::string(1, char(c));
        }
        else
        {
            return XmlHexEscape(c);
        }
    }
    }
    return std::string();
}

template<typename Char>
std::string XmlEscape(const Char* begin, const Char* end)
{
    std::string result;
    result.reserve(2 * (end - begin));
    for (const Char* i = begin; i != end; ++i)
    {
        result.append(XmlCharStr(*i));
    }
    return result;
}

} // namespace soul::lexer

#endif // LEXER_XML_PARSING_LOG_INCLUDED
