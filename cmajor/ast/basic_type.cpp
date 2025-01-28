// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.basic.type;

import cmajor.ast.visitor;

namespace cmajor::ast {

AutoNode::AutoNode(const soul::ast::Span& span_) : Node(NodeType::autoNode, span_)
{
}

Node* AutoNode::Clone(CloneContext& cloneContext) const
{
    return new AutoNode(GetSpan());
}

void AutoNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BoolNode::BoolNode(const soul::ast::Span& span_) : Node(NodeType::boolNode, span_)
{
}

Node* BoolNode::Clone(CloneContext& cloneContext) const
{
    BoolNode* clone = new BoolNode(GetSpan());
    return clone;
}

void BoolNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SByteNode::SByteNode(const soul::ast::Span& span_) : Node(NodeType::sbyteNode, span_)
{
}

Node* SByteNode::Clone(CloneContext& cloneContext) const
{
    SByteNode* clone = new SByteNode(GetSpan());
    return clone;
}

void SByteNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ByteNode::ByteNode(const soul::ast::Span& span_) : Node(NodeType::byteNode, span_)
{
}

Node* ByteNode::Clone(CloneContext& cloneContext) const
{
    ByteNode* clone = new ByteNode(GetSpan());
    return clone;
}

void ByteNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShortNode::ShortNode(const soul::ast::Span& span_) : Node(NodeType::shortNode, span_)
{
}

Node* ShortNode::Clone(CloneContext& cloneContext) const
{
    ShortNode* clone = new ShortNode(GetSpan());
    return clone;
}

void ShortNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UShortNode::UShortNode(const soul::ast::Span& span_) : Node(NodeType::ushortNode, span_)
{
}

Node* UShortNode::Clone(CloneContext& cloneContext) const
{
    UShortNode* clone = new UShortNode(GetSpan());
    return clone;
}

void UShortNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntNode::IntNode(const soul::ast::Span& span_) : Node(NodeType::intNode, span_)
{
}

Node* IntNode::Clone(CloneContext& cloneContext) const
{
    IntNode* clone = new IntNode(GetSpan());
    return clone;
}

void IntNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UIntNode::UIntNode(const soul::ast::Span& span_) : Node(NodeType::uintNode, span_)
{
}

Node* UIntNode::Clone(CloneContext& cloneContext) const
{
    UIntNode* clone = new UIntNode(GetSpan());
    return clone;
}

void UIntNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LongNode::LongNode(const soul::ast::Span& span_) : Node(NodeType::longNode, span_)
{
}

Node* LongNode::Clone(CloneContext& cloneContext) const
{
    LongNode* clone = new LongNode(GetSpan());
    return clone;
}

void LongNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ULongNode::ULongNode(const soul::ast::Span& span_) : Node(NodeType::ulongNode, span_)
{
}

Node* ULongNode::Clone(CloneContext& cloneContext) const
{
    ULongNode* clone = new ULongNode(GetSpan());
    return clone;
}

void ULongNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FloatNode::FloatNode(const soul::ast::Span& span_) : Node(NodeType::floatNode, span_)
{
}

Node* FloatNode::Clone(CloneContext& cloneContext) const
{
    FloatNode* clone = new FloatNode(GetSpan());
    return clone;
}

void FloatNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DoubleNode::DoubleNode(const soul::ast::Span& span_) : Node(NodeType::doubleNode, span_)
{
}

Node* DoubleNode::Clone(CloneContext& cloneContext) const
{
    DoubleNode* clone = new DoubleNode(GetSpan());
    return clone;
}

void DoubleNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CharNode::CharNode(const soul::ast::Span& span_) : Node(NodeType::charNode, span_)
{
}

Node* CharNode::Clone(CloneContext& cloneContext) const
{
    CharNode* clone = new CharNode(GetSpan());
    return clone;
}

void CharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WCharNode::WCharNode(const soul::ast::Span& span_) : Node(NodeType::wcharNode, span_)
{
}

Node* WCharNode::Clone(CloneContext& cloneContext) const
{
    WCharNode* clone = new WCharNode(GetSpan());
    return clone;
}

void WCharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UCharNode::UCharNode(const soul::ast::Span& span_) : Node(NodeType::ucharNode, span_)
{
}

Node* UCharNode::Clone(CloneContext& cloneContext) const
{
    UCharNode* clone = new UCharNode(GetSpan());
    return clone;
}

void UCharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

VoidNode::VoidNode(const soul::ast::Span& span_) : Node(NodeType::voidNode, span_)
{
}

Node* VoidNode::Clone(CloneContext& cloneContext) const
{
    VoidNode* clone = new VoidNode(GetSpan());
    return clone;
}

void VoidNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::ast
