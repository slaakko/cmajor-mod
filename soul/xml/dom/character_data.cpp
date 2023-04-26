// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.character.data;

namespace soul::xml {

std::string XmlCharDataEscape(const std::string& text)
{
    std::string result;
    for (char c : text)
    {
        switch (c)
        {
            case '<': result.append("&lt;"); break;
            case '&': result.append("&amp;"); break;
            default: result.append(1, c); break;
        }
    }
    return result;
}

CharacterData::CharacterData(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_) : 
    CharacterData(kind_, sourcePos_, name_, std::string())
{
}

CharacterData::CharacterData(NodeKind kind_, const soul::ast::SourcePos& sourcePos_, const std::string& name_, const std::string& data_) : 
    Node(kind_, sourcePos_, name_), data(data_)
{
}

void CharacterData::Write(util::CodeFormatter& formatter)
{
    formatter.Write(XmlCharDataEscape(data));
}

bool CharacterData::ValueContainsNewLine() const
{
    return data.find('\n') != std::string::npos;
}

} // namespace soul::xml
