// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.typedef_;

import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;

namespace cmajor::ast {

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
