// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.global.variable;

import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;

namespace cmajor::ast {

GlobalVariableNode::GlobalVariableNode(const soul::ast::Span& span_) : 
    Node(NodeType::globalVariableNode, span_), specifiers(Specifiers::none), cu(nullptr)
{
}

GlobalVariableNode::GlobalVariableNode(const soul::ast::Span& span_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_, 
    Node* initializer_, CompileUnitNode* cu_) :
    Node(NodeType::globalVariableNode, span_), specifiers(specifiers_), typeExpr(typeExpr_), id(id_), initializer(initializer_), cu(cu_)
{
    typeExpr->SetParent(this);
    id->SetParent(this);
    if (initializer)
    {
        initializer->SetParent(this);
    }
}

Node* GlobalVariableNode::Clone(CloneContext& cloneContext) const
{
    Node* clonedInitializer = nullptr;
    if (initializer)
    {
        clonedInitializer = initializer->Clone(cloneContext);
    }
    GlobalVariableNode* clone = new GlobalVariableNode(GetSpan(), specifiers, typeExpr->Clone(cloneContext), 
        static_cast<IdentifierNode*>(id->Clone(cloneContext)), clonedInitializer, cu);
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

} // namespace cmajor::ast
