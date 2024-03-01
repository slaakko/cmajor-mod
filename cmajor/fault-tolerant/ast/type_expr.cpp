// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.type_expr;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

PointerNode::PointerNode(Node* subject_, StarNode* star_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), subject(subject_), star(star_)
{
    AddChildNode(subject.get());
    AddChildNode(star.get());
}

Node* PointerNode::Clone() const
{
    PointerNode* clone = new PointerNode(subject->Clone(), static_cast<StarNode*>(star->Clone()));
    clone->Make();
    return clone;
}

void PointerNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LValueRefNode::LValueRefNode(Node* subject_, AmpNode* amp_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), subject(subject_), amp(amp_)
{
    AddChildNode(subject.get());
    AddChildNode(amp.get());
}

Node* LValueRefNode::Clone() const
{
    LValueRefNode* clone = new LValueRefNode(subject->Clone(), static_cast<AmpNode*>(amp->Clone()));
    clone->Make();
    return clone;
}

void LValueRefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RValueRefNode::RValueRefNode(Node* subject_, AmpAmpNode* ampamp_) : SyntaxNode(soul::ast::Span(), CompletionContext::none), subject(subject_), ampamp(ampamp_)
{
    AddChildNode(subject.get());
    AddChildNode(ampamp.get());
}

Node* RValueRefNode::Clone() const
{
    RValueRefNode* clone = new RValueRefNode(subject->Clone(), static_cast<AmpAmpNode*>(ampamp->Clone()));
    clone->Make();
    return clone;
}

void RValueRefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrayNode::ArrayNode(Node* subject_, LBracketNode* lbracket_, Node* size_, RBracketNode* rbracket_) :
    SyntaxNode(soul::ast::Span(), CompletionContext::none), subject(subject_), lbracket(lbracket_), size(size_), rbracket(rbracket_)
{
    AddChildNode(subject.get());
    AddChildNode(lbracket.get());
    if (size)
    {
        AddChildNode(size.get());
    }
    AddChildNode(rbracket.get());
}

Node* ArrayNode::Clone() const
{
    Node* clonedSize = nullptr;
    if (size)
    {
        clonedSize = size->Clone();
    }
    ArrayNode* clone = new ArrayNode(subject->Clone(), static_cast<LBracketNode*>(lbracket->Clone()), clonedSize, static_cast<RBracketNode*>(rbracket->Clone()));
    clone->Make();
    return clone;
}

void ArrayNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeExprNode::TypeExprNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void TypeExprNode::AddNode(Node* node)
{
    AddChildNode(node);
    nodes.push_back(std::unique_ptr<Node>(node));
}

Node* TypeExprNode::Clone() const
{
    TypeExprNode* clone = new TypeExprNode(GetSpan(), GetCompletionContext());
    for (const auto& node : nodes)
    {
        clone->AddNode(node->Clone());
    }
    clone->Make();
    return clone;
}

void TypeExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
