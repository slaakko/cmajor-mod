// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.alias;

import cmajor.ast.reader;
import cmajor.ast.writer;
import cmajor.ast.identifier;
import cmajor.ast.visitor;

namespace cmajor::ast {

AliasNode::AliasNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : Node(NodeType::aliasNode, sourcePos_, moduleId_), specifiers(), id(), typeExpr()
{
}

AliasNode::AliasNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, IdentifierNode* id_, Node* typeExpr_) :
    Node(NodeType::aliasNode, sourcePos_, moduleId_), specifiers(specifiers_), id(id_), typeExpr(typeExpr_)
{
    id->SetParent(this);
    typeExpr->SetParent(this);
}

Node* AliasNode::Clone(CloneContext& cloneContext) const
{
    AliasNode* clone = new AliasNode(GetSourcePos(), ModuleId(), specifiers, static_cast<IdentifierNode*>(id->Clone(cloneContext)), typeExpr->Clone(cloneContext));
    return clone;
}

void AliasNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AliasNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(specifiers);
    writer.Write(id.get());
    writer.Write(typeExpr.get());
}

void AliasNode::Read(AstReader& reader)
{
    Node::Read(reader);
    specifiers = reader.ReadSpecifiers();
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
}

IdentifierNode* AliasNode::Id() const
{
    return id.get();
}

Node* AliasNode::TypeExpr() const
{
    return typeExpr.get();
}

TypedefNode::TypedefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) :
    Node(NodeType::typedefNode, sourcePos_, moduleId_), specifiers(Specifiers::none), typeExpr(), id()
{
}

TypedefNode::TypedefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_) :
    Node(NodeType::typedefNode, sourcePos_, moduleId_), specifiers(specifiers_), typeExpr(typeExpr_), id(id_)
{
    typeExpr->SetParent(this);
    id->SetParent(this);
}

Node* TypedefNode::Clone(CloneContext& cloneContext) const
{
    TypedefNode* clone = new TypedefNode(GetSourcePos(), ModuleId(), specifiers, typeExpr->Clone(cloneContext), static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    return clone;
}

void TypedefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypedefNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(specifiers);
    writer.Write(typeExpr.get());
    writer.Write(id.get());
}

void TypedefNode::Read(AstReader& reader)
{
    Node::Read(reader);
    specifiers = reader.ReadSpecifiers();
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
}

} // namespace cmajor::ast
