// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.global.variable;

import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;

namespace cmajor::ast {

GlobalVariableNode::GlobalVariableNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_) : Node(NodeType::globalVariableNode, sourcePos_, moduleId_), specifiers(Specifiers::none), cu(nullptr)
{
}

GlobalVariableNode::GlobalVariableNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_, CompileUnitNode* cu_) :
    Node(NodeType::globalVariableNode, sourcePos_, moduleId_), specifiers(specifiers_), typeExpr(typeExpr_), id(id_), cu(cu_)
{
    typeExpr->SetParent(this);
    id->SetParent(this);
}

Node* GlobalVariableNode::Clone(CloneContext& cloneContext) const
{
    GlobalVariableNode* clone = new GlobalVariableNode(GetSourcePos(), ModuleId(), specifiers, typeExpr->Clone(cloneContext), static_cast<IdentifierNode*>(id->Clone(cloneContext)), cu);
    if (initializer)
    {
        clone->SetInitializer(initializer->Clone(cloneContext));
    }
    return clone;
}

void GlobalVariableNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void GlobalVariableNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(specifiers);
    writer.Write(typeExpr.get());
    writer.Write(id.get());
    bool hasInitializer = initializer != nullptr;
    writer.GetBinaryStreamWriter().Write(hasInitializer);
    if (hasInitializer)
    {
        writer.Write(initializer.get());
    }

}
void GlobalVariableNode::Read(AstReader& reader)
{
    Node::Read(reader);
    specifiers = reader.ReadSpecifiers();
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    bool hasInitializer = reader.GetBinaryStreamReader().ReadBool();
    if (hasInitializer)
    {
        initializer.reset(reader.ReadNode());
        initializer->SetParent(this);
    }
}

void GlobalVariableNode::SetInitializer(Node* initializer_)
{
    initializer.reset(initializer_);
    if (initializer)
    {
        initializer->SetParent(this);
    }
}
} // namespace cmajor::ast
