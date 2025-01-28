// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

#ifndef SOUL_XML_ATTIBUTE_NODE
#define SOUL_XML_ATTIBUTE_NODE
#include <ast/source_pos.hpp>
#include <dom/node.hpp>

namespace soul::xml {

std::string AttrValueEscape(const std::string& attributeValue, char delimiter);
std::string MakeXmlAttrValue(const std::string& value);

class AttributeNode : public Node
{
public:
    AttributeNode(const soul::ast::SourcePos& sourcePos_, const std::string& name_, const std::string& value_);
    const std::string& Value() const { return value; }
    void SetValue(const std::string& value_);
    void Write(util::CodeFormatter& formatter) override;
private:
    std::string value;
};

AttributeNode* MakeAttribute(const std::string& name, const std::string& value);

} // namespace soul::xml

#endif // SOUL_XML_ATTIBUTE_NODE
