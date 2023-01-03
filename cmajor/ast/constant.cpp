// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.constant;

import std.core;
import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

ConstantNode::ConstantNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : Node(NodeType::constantNode, sourcePos_, moduleId_), specifiers(Specifiers::none)
{
}

ConstantNode::ConstantNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_, Node* value_) :
    Node(NodeType::constantNode, sourcePos_, moduleId_), specifiers(specifiers_), typeExpr(typeExpr_), id(id_), value(value_)
{
    typeExpr->SetParent(this);
    id->SetParent(this);
    if (value)
    {
        value->SetParent(this);
    }
}

Node* ConstantNode::Clone(CloneContext& cloneContext) const
{
    Node* clonedValue = nullptr;
    if (value)
    {
        clonedValue = value->Clone(cloneContext);
    }
    ConstantNode* clone = new ConstantNode(GetSourcePos(), ModuleId(), specifiers, typeExpr->Clone(cloneContext), static_cast<IdentifierNode*>(id->Clone(cloneContext)), clonedValue);
    return clone;
}

void ConstantNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstantNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(specifiers);
    writer.Write(typeExpr.get());
    writer.Write(id.get());
    bool hasValue = value != nullptr;
    writer.GetBinaryStreamWriter().Write(hasValue);
    if (hasValue)
    {
        writer.Write(value.get());
    }
    writer.GetBinaryStreamWriter().Write(strValue);
}

void ConstantNode::Read(AstReader& reader)
{
    Node::Read(reader);
    specifiers = reader.ReadSpecifiers();
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    bool hasValue = reader.GetBinaryStreamReader().ReadBool();
    if (hasValue)
    {
        value.reset(reader.ReadNode());
        value->SetParent(this);
    }
    strValue = reader.GetBinaryStreamReader().ReadUtf32String();
}

} // namespace cmajor::ast
