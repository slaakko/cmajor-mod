// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module soul.xml.processor.attribute;

namespace soul::xml::processor {

Attribute::Attribute(const soul::ast::SourcePos& sourcePos_, const std::string& namespaceUri_, const std::string& localName_, const std::string& qualifiedName_, 
    const std::string& value_) : sourcePos(sourcePos_), namespaceUri(namespaceUri_), localName(localName_), qualifiedName(qualifiedName_), value(value_)
{
}

std::string Attribute::ToString() const
{
    std::string str;
    str.append("ATTRIBUTE:").append("attName='").append(qualifiedName).append("', attValue='").append(value).append("', line=").append(std::to_string(sourcePos.line));
    return str;
}

Attributes::Attributes()
{
}

void Attributes::Clear()
{
    attributes.clear();
}

void Attributes::AddAttribute(Attribute* attribute)
{
    attributes.push_back(std::unique_ptr<Attribute>(attribute));
}


const std::string* Attributes::GetAttributeValue(const std::string& qualifiedName) const
{
    for (const auto& attribute : attributes)
    {
        if (attribute->QualifiedName() == qualifiedName)
        {
            const std::string& value = attribute->Value();
            return &value;
        }
    }
    return nullptr;
}

std::string Attributes::ToString() const
{
    std::string str;
    str.append("[");
    bool first = true;
    for (const auto& attribute : attributes)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str.append(1, ' ');
        }
        str.append(attribute->ToString());
    }
    str.append("]");
    return str;
}

} // namespace soul::xml::processor
