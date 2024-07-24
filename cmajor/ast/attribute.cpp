// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.attribute;

import std.core;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

AttributeNode::AttributeNode(const soul::ast::Span& span_) : Node(NodeType::attributeNode, span_), name(), value(), hasValue(false)
{
}

AttributeNode::AttributeNode(const soul::ast::Span& span_, const std::u32string& name_, const std::u32string& value_) :
    Node(NodeType::attributeNode, span_), name(name_), value(value_), hasValue(false)
{
}

Node* AttributeNode::Clone(CloneContext& cloneContext) const
{
    AttributeNode* clone = new AttributeNode(GetSpan(), name, value);
    clone->hasValue = hasValue;
    return clone;
}

void AttributeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AttributeNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.GetBinaryStreamWriter().Write(name);
    writer.GetBinaryStreamWriter().Write(value);
    writer.GetBinaryStreamWriter().Write(hasValue);
}

void AttributeNode::Read(AstReader& reader)
{
    Node::Read(reader);
    name = reader.GetBinaryStreamReader().ReadUtf32String();
    value = reader.GetBinaryStreamReader().ReadUtf32String();
    hasValue = reader.GetBinaryStreamReader().ReadBool();
}

AttributesNode::AttributesNode(const soul::ast::Span& span_) : Node(NodeType::attributesNode, span_)
{
}

AttributeNode* AttributesNode::GetAttribute(const std::u32string& name) const
{
    std::map<std::u32string, AttributeNode*>::const_iterator it = attributeMap.find(name);
    if (it != attributeMap.cend())
    {
        return it->second;
    }
    return nullptr;
}

void AttributesNode::AddAttribute(const soul::ast::Span& span, const std::u32string& name)
{
    AddAttribute(span, name, U"true", false);
}

void AttributesNode::AddAttribute(const soul::ast::Span& span, const std::u32string& name, const std::u32string& value, bool hasValue)
{
    AttributeNode* prev = GetAttribute(name);
    if (prev != nullptr)
    {
        throw AttributeNotUniqueException("attribute '" + util::ToUtf8(name) + "' not unique", span, prev->GetSpan(), prev->ModuleId());
    }
    AttributeNode* attribute = new AttributeNode(span, name, value);
    if (hasValue)
    {
        attribute->SetHasValue();
    }
    AddAttribute(attribute);
}

void AttributesNode::AddAttribute(AttributeNode* attribute)
{
    attributes.push_back(std::unique_ptr<AttributeNode>(attribute));
    attributeMap[attribute->Name()] = attribute;
}

Node* AttributesNode::Clone(CloneContext& cloneContext) const
{
    std::unique_ptr<AttributesNode> clone(new AttributesNode(GetSpan()));
    for (const std::unique_ptr<AttributeNode>& attribute : attributes)
    {
        clone->AddAttribute(attribute->GetSpan(), attribute->Name(), attribute->Value(), attribute->HasValue());
    }
    return clone.release();
}

void AttributesNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AttributesNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.GetBinaryStreamWriter().Write(static_cast<int32_t>(attributes.size()));
    for (const std::unique_ptr<AttributeNode>& attribute : attributes)
    {
        writer.Write(attribute.get());
    }
}

void AttributesNode::Read(AstReader& reader)
{
    Node::Read(reader);
    int32_t n = reader.GetBinaryStreamReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        AttributeNode* attribute = reader.ReadAttributeNode();
        AddAttribute(attribute);
    }
}

AttributeNotUniqueException::AttributeNotUniqueException(const std::string& message_, const soul::ast::Span& span_,
    const soul::ast::Span& prevSpan_, const util::uuid& prevModuleId_) : std::runtime_error(message_), span(span_), 
    prevSpan(prevSpan_), prevModuleId(prevModuleId_)
{
}

} // namespace cmajor::ast
