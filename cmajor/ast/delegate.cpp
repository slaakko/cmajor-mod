// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.delegate;

import cmajor.ast.parameter; 
import cmajor.ast.specifier;
import cmajor.ast.visitor;
import cmajor.ast.identifier;
import cmajor.ast.writer;
import cmajor.ast.reader;

namespace cmajor::ast {

DelegateNode::DelegateNode(const soul::ast::Span& span_) : 
    Node(NodeType::delegateNode, span_), specifiers(Specifiers::none), returnTypeExpr(), id(), parameters()
{
}

DelegateNode::DelegateNode(const soul::ast::Span& span_, Specifiers specifiers_, Node* returnTypeExpr_, IdentifierNode* id_) :
    Node(NodeType::delegateNode, span_), specifiers(specifiers_), returnTypeExpr(returnTypeExpr_), id(id_), parameters()
{
    returnTypeExpr->SetParent(this);
    id->SetParent(this);
}

Node* DelegateNode::Clone(CloneContext& cloneContext) const
{
    DelegateNode* clone = new DelegateNode(GetSpan(), specifiers, returnTypeExpr->Clone(cloneContext), static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    int n = parameters.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddParameter(static_cast<ParameterNode*>(parameters[i]->Clone(cloneContext)));
    }
    return clone;
}

void DelegateNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DelegateNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(specifiers);
    writer.Write(returnTypeExpr.get());
    writer.Write(id.get());
    parameters.Write(writer);
}

void DelegateNode::Read(AstReader& reader)
{
    Node::Read(reader);
    specifiers = reader.ReadSpecifiers();
    returnTypeExpr.reset(reader.ReadNode());
    returnTypeExpr->SetParent(this);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    parameters.Read(reader);
    parameters.SetParent(this);
}

void DelegateNode::AddParameter(ParameterNode* parameter)
{
    parameter->SetParent(this);
    parameters.Add(parameter);
}

ClassDelegateNode::ClassDelegateNode(const soul::ast::Span& span_) : 
    Node(NodeType::classDelegateNode, span_), specifiers(Specifiers::none), returnTypeExpr(), id(), parameters()
{
}

ClassDelegateNode::ClassDelegateNode(const soul::ast::Span& span_, Specifiers specifiers_, Node* returnTypeExpr_, IdentifierNode* id_) :
    Node(NodeType::classDelegateNode, span_), specifiers(specifiers_), returnTypeExpr(returnTypeExpr_), id(id_), parameters()
{
    returnTypeExpr->SetParent(this);
    id->SetParent(this);
}

Node* ClassDelegateNode::Clone(CloneContext& cloneContext) const
{
    ClassDelegateNode* clone = new ClassDelegateNode(GetSpan(), specifiers, returnTypeExpr->Clone(cloneContext), static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    int n = parameters.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddParameter(static_cast<ParameterNode*>(parameters[i]->Clone(cloneContext)));
    }
    return clone;
}

void ClassDelegateNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ClassDelegateNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(specifiers);
    writer.Write(returnTypeExpr.get());
    writer.Write(id.get());
    parameters.Write(writer);
}

void ClassDelegateNode::Read(AstReader& reader)
{
    Node::Read(reader);
    specifiers = reader.ReadSpecifiers();
    returnTypeExpr.reset(reader.ReadNode());
    returnTypeExpr->SetParent(this);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    parameters.Read(reader);
    parameters.SetParent(this);
}

void ClassDelegateNode::AddParameter(ParameterNode* parameter)
{
    parameter->SetParent(this);
    parameters.Add(parameter);
}

} // namespace cmajor::ast
