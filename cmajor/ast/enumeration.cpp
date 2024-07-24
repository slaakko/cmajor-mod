// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.enumeration;

import std.core;
import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import cmajor.ast.literal;
import cmajor.ast.expression;
import util;

namespace cmajor::ast {

EnumTypeNode::EnumTypeNode(const soul::ast::Span& span_) : Node(NodeType::enumTypeNode, span_)
{
}

EnumTypeNode::EnumTypeNode(const soul::ast::Span& span_, Specifiers specifiers_, IdentifierNode* id_) :
    Node(NodeType::enumTypeNode, span_), specifiers(specifiers_), id(id_)
{
    id->SetParent(this);
}

Node* EnumTypeNode::Clone(CloneContext& cloneContext) const
{
    EnumTypeNode* clone = new EnumTypeNode(GetSpan(), specifiers, static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    int n = constants.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddConstant(static_cast<EnumConstantNode*>(constants[i]->Clone(cloneContext)));
    }
    return clone;
}

void EnumTypeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EnumTypeNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(specifiers);
    writer.Write(id.get());
    bool hasUnderlyingType = underlyingType != nullptr;
    writer.GetBinaryStreamWriter().Write(hasUnderlyingType);
    if (hasUnderlyingType)
    {
        writer.Write(underlyingType.get());
    }
    constants.Write(writer);
}

void EnumTypeNode::Read(AstReader& reader)
{
    Node::Read(reader);
    specifiers = reader.ReadSpecifiers();
    id.reset(reader.ReadIdentifierNode());
    bool hasUnderlyingType = reader.GetBinaryStreamReader().ReadBool();
    if (hasUnderlyingType)
    {
        underlyingType.reset(reader.ReadNode());
    }
    constants.Read(reader);
    constants.SetParent(this);
}

void EnumTypeNode::AddConstant(EnumConstantNode* constant)
{
    constant->SetParent(this);
    constants.Add(constant);
}

EnumConstantNode* EnumTypeNode::GetLastConstant() const
{
    int n = constants.Count();
    if (n > 0)
    {
        return constants[n - 1];
    }
    return nullptr;
}

void EnumTypeNode::SetUnderlyingType(Node* underlyingType_)
{
    underlyingType.reset(underlyingType_);
    underlyingType->SetParent(this);
}

EnumConstantNode::EnumConstantNode(const soul::ast::Span& span_) :
    Node(NodeType::enumConstantNode, span_), hasValue(false)
{
}

EnumConstantNode::EnumConstantNode(const soul::ast::Span& span_, IdentifierNode* id_, Node* value_) :
    Node(NodeType::enumConstantNode, span_), id(id_), value(value_), hasValue(false)
{
    id->SetParent(this);
    if (value)
    {
        value->SetParent(this);
    }
}

Node* EnumConstantNode::Clone(CloneContext& cloneContext) const
{
    EnumConstantNode* clone = new EnumConstantNode(GetSpan(), static_cast<IdentifierNode*>(id->Clone(cloneContext)), value->Clone(cloneContext));
    if (hasValue)
    {
        clone->SetHasValue();
    }
    return clone;
}

void EnumConstantNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void EnumConstantNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(id.get());
    writer.Write(value.get());
    writer.GetBinaryStreamWriter().Write(hasValue);
    writer.GetBinaryStreamWriter().Write(strValue);
}

void EnumConstantNode::Read(AstReader& reader)
{
    Node::Read(reader);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    value.reset(reader.ReadNode());
    value->SetParent(this);
    hasValue = reader.GetBinaryStreamReader().ReadBool();
    strValue = reader.GetBinaryStreamReader().ReadUtf8String();
}

Node* MakeNextEnumConstantValue(const soul::ast::Span& span, EnumTypeNode* enumType)
{
    EnumConstantNode* lastConstant = enumType->GetLastConstant();
    if (lastConstant)
    {
        Node* lastValue = lastConstant->GetValue();
        if (lastValue)
        {
            CloneContext cloneContext;
            Node* clonedValue = lastValue->Clone(cloneContext);
            if (enumType->GetUnderlyingType())
            {
                if (enumType->GetUnderlyingType()->IsUnsignedTypeNode())
                {
                    return new AddNode(span, clonedValue, new ByteLiteralNode(span, 1u));
                }
            }
            return new AddNode(span, clonedValue, new SByteLiteralNode(span, 1));
        }
        else
        {
            throw std::runtime_error("last constant returned null value");
        }
    }
    else
    {
        if (enumType->GetUnderlyingType())
        {
            if (enumType->GetUnderlyingType()->IsUnsignedTypeNode())
            {
                return new ByteLiteralNode(span, 0u);
            }
        }
        return new SByteLiteralNode(span, 0);
    }
}

} // namespace cmajor::ast
