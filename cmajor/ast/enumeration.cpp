// =================================
// Copyright (c) 2022 Seppo Laakko
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

EnumTypeNode::EnumTypeNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::enumTypeNode, sourcePos_, moduleId_)
{
}

EnumTypeNode::EnumTypeNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, Specifiers specifiers_, IdentifierNode* id_) :
    Node(NodeType::enumTypeNode, sourcePos_, moduleId_), specifiers(specifiers_), id(id_)
{
    id->SetParent(this);
}

Node* EnumTypeNode::Clone(CloneContext& cloneContext) const
{
    EnumTypeNode* clone = new EnumTypeNode(GetSourcePos(), ModuleId(), specifiers, static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    int n = constants.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddConstant(static_cast<EnumConstantNode*>(constants[i]->Clone(cloneContext)));
    }
    clone->SetBeginBraceSourcePos(beginBraceSourcePos);
    clone->SetEndBraceSourcePos(endBraceSourcePos);
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
    // Seppo
    //bool convertExternal = ModuleId() == writer.SourcePosConversionModuleId(); TODO
    //writer.Write(beginBraceSourcePos, convertExternal); TODO
    //writer.Write(endBraceSourcePos, convertExternal); TODO
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
    beginBraceSourcePos = reader.ReadSourcePos();
    endBraceSourcePos = reader.ReadSourcePos();
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

EnumConstantNode::EnumConstantNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) :
    Node(NodeType::enumConstantNode, sourcePos_, moduleId_), hasValue(false)
{
}

EnumConstantNode::EnumConstantNode(const soul::ast::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_, IdentifierNode* id_, Node* value_) :
    Node(NodeType::enumConstantNode, sourcePos_, moduleId_), id(id_), value(value_), hasValue(false)
{
    id->SetParent(this);
    if (value)
    {
        value->SetParent(this);
    }
}

Node* EnumConstantNode::Clone(CloneContext& cloneContext) const
{
    EnumConstantNode* clone = new EnumConstantNode(GetSourcePos(), ModuleId(), static_cast<IdentifierNode*>(id->Clone(cloneContext)), value->Clone(cloneContext));
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
    strValue = reader.GetBinaryStreamReader().ReadUtf32String();
}

Node* MakeNextEnumConstantValue(const soul::ast::SourcePos& span, const boost::uuids::uuid& moduleId, EnumTypeNode* enumType)
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
                    return new AddNode(span, moduleId, clonedValue, new ByteLiteralNode(span, moduleId, 1u));
                }
            }
            return new AddNode(span, moduleId, clonedValue, new SByteLiteralNode(span, moduleId, 1));
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
                return new ByteLiteralNode(span, moduleId, 0u);
            }
        }
        return new SByteLiteralNode(span, moduleId, 0);
    }
}
} // namespace cmajor::ast
