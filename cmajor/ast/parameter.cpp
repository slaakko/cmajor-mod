// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.parameter;

import std.core;
import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

ParameterNode::ParameterNode(const soul::ast::Span& span_) :
    Node(NodeType::parameterNode, span_), typeExpr(), id(), artificialId(false)
{
}

ParameterNode::ParameterNode(const soul::ast::Span& span_, Node* typeExpr_, IdentifierNode* id_) :
    Node(NodeType::parameterNode, span_), typeExpr(typeExpr_), id(id_), artificialId(false)
{
    typeExpr->SetParent(this);
    if (id)
    {
        id->SetParent(this);
    }
}

Node* ParameterNode::Clone(CloneContext& cloneContext) const
{
    IdentifierNode* clonedId = nullptr;
    if (id)
    {
        clonedId = static_cast<IdentifierNode*>(id->Clone(cloneContext));
    }
    ParameterNode* clone = new ParameterNode(GetSpan(), typeExpr->Clone(cloneContext), clonedId);
    if (artificialId)
    {
        clone->artificialId = true;
    }
    return clone;
}

void ParameterNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParameterNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(typeExpr.get());
    bool hasId = id != nullptr;
    writer.GetBinaryStreamWriter().Write(hasId);
    if (hasId)
    {
        writer.Write(id.get());
    }
    writer.GetBinaryStreamWriter().Write(artificialId);
}

void ParameterNode::Read(AstReader& reader)
{
    Node::Read(reader);
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    bool hasId = reader.GetBinaryStreamReader().ReadBool();
    if (hasId)
    {
        id.reset(reader.ReadIdentifierNode());
        id->SetParent(this);
    }
    artificialId = reader.GetBinaryStreamReader().ReadBool();
}

void ParameterNode::SetId(IdentifierNode* id_)
{
    id.reset(id_);
    id->SetParent(this);
    artificialId = true;
}

} // namespace cmajor::ast
