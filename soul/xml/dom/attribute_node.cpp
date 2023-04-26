// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.attribute.node;

namespace soul::xml {

std::string AttrValueEscape(const std::string& attributeValue, char delimiter)
{
    std::string result;
    for (char c : attributeValue)
    {
        switch (c)
        {
            case '<': result.append("&lt;"); break;
            case '&': result.append("&amp;"); break;
            case '"': if (delimiter == '"') result.append("&quot;"); else result.append(1, '"'); break;
            case '\'': if (delimiter == '\'') result.append("&apos;"); else result.append(1, '\''); break;
            default: result.append(1, c); break;
        }
    }
    return result;
}

std::string MakeXmlAttrValue(const std::string& value)
{
    std::string result;
    if (value.find('"') == std::string::npos)
    {
        result.append(1, '"');
        result.append(AttrValueEscape(value, '"'));
        result.append(1, '"');
    }
    else if (value.find('\'') == std::string::npos)
    {
        result.append(1, '\'');
        result.append(AttrValueEscape(value, '\''));
        result.append(1, '\'');
    }
    else
    {
        result.append(1, '"');
        result.append(AttrValueEscape(value, '"'));
        result.append(1, '"');
    }
    return result;
}

AttributeNode::AttributeNode(const soul::ast::SourcePos& sourcePos_, const std::string& name_, const std::string& value_) : 
    Node(NodeKind::attributeNode, sourcePos_, name_), value(value_)
{
}

void AttributeNode::SetValue(const std::string& value_)
{
    value = value_;
}

void AttributeNode::Write(util::CodeFormatter& formatter)
{
    formatter.Write(" " + Name() + "=");
    formatter.Write(MakeXmlAttrValue(value));
}

AttributeNode* MakeAttribute(const std::string& name, const std::string& value)
{
    return new AttributeNode(soul::ast::SourcePos(), name, value);
}

} // namespace soul::xml
