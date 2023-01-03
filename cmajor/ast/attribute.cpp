// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.attribute;

import std.core;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

AttributeNode::AttributeNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : Node(NodeType::attributeNode, sourcePos_, moduleId_), name(), value()
{
}

AttributeNode::AttributeNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, const std::u32string& name_, const std::u32string& value_) :
    Node(NodeType::attributeNode, sourcePos_, moduleId_), name(name_), value(value_)
{
}

Node* AttributeNode::Clone(CloneContext& cloneContext) const
{
    return new AttributeNode(GetSourcePos(), ModuleId(), name, value);
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
}

void AttributeNode::Read(AstReader& reader)
{
    Node::Read(reader);
    name = reader.GetBinaryStreamReader().ReadUtf32String();
    value = reader.GetBinaryStreamReader().ReadUtf32String();
}

AttributesNode::AttributesNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : Node(NodeType::attributesNode, sourcePos_, moduleId_)
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

void AttributesNode::AddAttribute(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, const std::u32string& name)
{
    AddAttribute(sourcePos, moduleId, name, U"true");
}

void AttributesNode::AddAttribute(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, const std::u32string& name, const std::u32string& value)
{
    AttributeNode* prev = GetAttribute(name);
    if (prev != nullptr)
    {
        throw AttributeNotUniqueException("attribute '" + util::ToUtf8(name) + "' not unique", sourcePos, moduleId, prev->GetSourcePos(), prev->ModuleId());
    }
    AttributeNode* attribute = new AttributeNode(sourcePos, moduleId, name, value);
    AddAttribute(attribute);
}

void AttributesNode::AddAttribute(AttributeNode* attribute)
{
    attributes.push_back(std::unique_ptr<AttributeNode>(attribute));
    attributeMap[attribute->Name()] = attribute;
}

Node* AttributesNode::Clone(CloneContext& cloneContext) const
{
    std::unique_ptr<AttributesNode> clone(new AttributesNode(GetSourcePos(), ModuleId()));
    for (const std::unique_ptr<AttributeNode>& attribute : attributes)
    {
        clone->AddAttribute(attribute->GetSourcePos(), attribute->ModuleId(), attribute->Name(), attribute->Value());
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
    writer.GetBinaryStreamWriter().WriteULEB128UInt(static_cast<uint32_t>(attributes.size()));
    for (const std::unique_ptr<AttributeNode>& attribute : attributes)
    {
        writer.Write(attribute.get());
    }
}

void AttributesNode::Read(AstReader& reader)
{
    Node::Read(reader);
    uint32_t n = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0u; i < n; ++i)
    {
        AttributeNode* attribute = reader.ReadAttributeNode();
        AddAttribute(attribute);
    }
}

AttributeNotUniqueException::AttributeNotUniqueException(const std::string& message_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_,
    const soul::ast::SourcePos& prevSourcePos_, const util::uuid& prevModuleId_) : std::runtime_error(message_), sourcePos(sourcePos_), moduleId(moduleId_), prevSourcePos(prevSourcePos_), prevModuleId(prevModuleId_)
{
}

} // namespace cmajor::ast
