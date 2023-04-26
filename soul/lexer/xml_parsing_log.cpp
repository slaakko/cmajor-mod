// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.lexer.xml.parsing.log;

namespace soul::lexer {

XmlParsingLog::XmlParsingLog(std::ostream& stream_) : ParsingLog(), formatter(stream_)
{
    formatter.SetIndentSize(1);
}

XmlParsingLog::XmlParsingLog(std::ostream& stream_, int maxLineLength_) : ParsingLog(maxLineLength_), formatter(stream_)
{
    formatter.SetIndentSize(1);
}

void XmlParsingLog::IncIndent()
{
    formatter.IncIndent();
}

void XmlParsingLog::DecIndent()
{
    formatter.DecIndent();
}

void XmlParsingLog::WriteBeginRule(const std::string& ruleName)
{
    Write("<" + ruleName + ">");
}

void XmlParsingLog::WriteEndRule(const std::string& ruleName)
{
    Write("</" + ruleName + ">");
}

void XmlParsingLog::WriteTry(const std::string& s)
{
    WriteElement("try", s);
}

void XmlParsingLog::WriteSuccess(const std::string& match)
{
    WriteElement("success", match);
}

void XmlParsingLog::WriteFail()
{
    Write("<fail/>");
}

void XmlParsingLog::WriteElement(const std::string& elementName, const std::string& elementContent)
{
    std::u32string content = util::ToUtf32(elementContent);
    std::string converted = XmlEscape(content.c_str(), content.c_str() + content.length());
    int convertedLength = static_cast<int>(converted.length());
    int lineLength = 2 * static_cast<int>(elementName.length()) + 5 + convertedLength;
    std::string s = converted;
    if (lineLength > MaxLineLength())
    {
        lineLength += 3;
        s = converted.substr(0, std::max(0, convertedLength - (lineLength - MaxLineLength()))) + "...";
    }
    Write("<" + elementName + ">" + s + "</" + elementName + ">");
}

void XmlParsingLog::Write(const std::string& s)
{
    formatter.WriteLine(s);
}

} // namespace soul::lexer
