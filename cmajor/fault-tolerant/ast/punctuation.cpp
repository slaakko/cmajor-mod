// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.punctuation;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

DotNode::DotNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u".")
{
}

Node* DotNode::Clone() const
{
    DotNode* clone = new DotNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrowNode::ArrowNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"->")
{
}

Node* ArrowNode::Clone() const
{
    ArrowNode* clone = new ArrowNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ArrowNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CommaNode::CommaNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u",")
{
}

Node* CommaNode::Clone() const
{
    CommaNode* clone = new CommaNode(GetSpan(), GetCompletionContext());
    return clone;
}

void CommaNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ColonNode::ColonNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u":")
{
}

Node* ColonNode::Clone() const
{
    ColonNode* clone = new ColonNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ColonNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SemicolonNode::SemicolonNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u";")
{
}

Node* SemicolonNode::Clone() const
{
    SemicolonNode* clone = new SemicolonNode(GetSpan(), GetCompletionContext());
    return clone;
}

void SemicolonNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LBraceNode::LBraceNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"{")
{
}

Node* LBraceNode::Clone() const
{
    LBraceNode* clone = new LBraceNode(GetSpan(), GetCompletionContext());
    return clone;
}

void LBraceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RBraceNode::RBraceNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"}")
{
}

Node* RBraceNode::Clone() const
{
    RBraceNode* clone = new RBraceNode(GetSpan(), GetCompletionContext());
    return clone;
}

void RBraceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LBracketNode::LBracketNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"[")
{
}

Node* LBracketNode::Clone() const
{
    LBracketNode* clone = new LBracketNode(GetSpan(), GetCompletionContext());
    return clone;
}

void LBracketNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RBracketNode::RBracketNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"]")
{
}

Node* RBracketNode::Clone() const
{
    RBracketNode* clone = new RBracketNode(GetSpan(), GetCompletionContext());
    return clone;
}

void RBracketNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LParenNode::LParenNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"(")
{
}

Node* LParenNode::Clone() const
{
    LParenNode* clone = new LParenNode(GetSpan(), GetCompletionContext());
    return clone;
}

void LParenNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RParenNode::RParenNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u")")
{
}

Node* RParenNode::Clone() const
{
    RParenNode* clone = new RParenNode(GetSpan(), GetCompletionContext());
    return clone;
}

void RParenNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AssignNode::AssignNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"=")
{
}

Node* AssignNode::Clone() const
{
    return new AssignNode(GetSpan(), GetCompletionContext());
}

void AssignNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LAngleNode::LAngleNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"<")
{
}

Node* LAngleNode::Clone() const
{
    LAngleNode* clone = new LAngleNode(GetSpan(), GetCompletionContext());
    return clone;
}

void LAngleNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RAngleNode::RAngleNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u">")
{
}

Node* RAngleNode::Clone() const
{
    RAngleNode* clone = new RAngleNode(GetSpan(), GetCompletionContext());
    return clone;
}

void RAngleNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

StarNode::StarNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"*")
{
}

Node* StarNode::Clone() const
{
    StarNode* clone = new StarNode(GetSpan(), GetCompletionContext());
    return clone;
}

void StarNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AmpNode::AmpNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"&")
{
}

Node* AmpNode::Clone() const
{
    AmpNode* clone = new AmpNode(GetSpan(), GetCompletionContext());
    return clone;
}

void AmpNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AmpAmpNode::AmpAmpNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"&&")
{
}

Node* AmpAmpNode::Clone() const
{
    AmpAmpNode* clone = new AmpAmpNode(GetSpan(), GetCompletionContext());
    return clone;
}

void AmpAmpNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

HashNode::HashNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"#")
{
}

Node* HashNode::Clone() const
{
    HashNode* clone = new HashNode(GetSpan(), GetCompletionContext());
    return clone;
}

void HashNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
