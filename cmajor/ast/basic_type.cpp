// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.basic.type;

import cmajor.ast.visitor;

namespace cmajor::ast {

BoolNode::BoolNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::boolNode, sourcePos_, moduleId_)
{
}

Node* BoolNode::Clone(CloneContext& cloneContext) const
{
    BoolNode* clone = new BoolNode(GetSourcePos(), ModuleId());
    return clone;
}

void BoolNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SByteNode::SByteNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::sbyteNode, sourcePos_, moduleId_)
{
}

Node* SByteNode::Clone(CloneContext& cloneContext) const
{
    SByteNode* clone = new SByteNode(GetSourcePos(), ModuleId());
    return clone;
}

void SByteNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ByteNode::ByteNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::byteNode, sourcePos_, moduleId_)
{
}

Node* ByteNode::Clone(CloneContext& cloneContext) const
{
    ByteNode* clone = new ByteNode(GetSourcePos(), ModuleId());
    return clone;
}

void ByteNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShortNode::ShortNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::shortNode, sourcePos_, moduleId_)
{
}

Node* ShortNode::Clone(CloneContext& cloneContext) const
{
    ShortNode* clone = new ShortNode(GetSourcePos(), ModuleId());
    return clone;
}

void ShortNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UShortNode::UShortNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::ushortNode, sourcePos_, moduleId_)
{
}

Node* UShortNode::Clone(CloneContext& cloneContext) const
{
    UShortNode* clone = new UShortNode(GetSourcePos(), ModuleId());
    return clone;
}

void UShortNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IntNode::IntNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::intNode, sourcePos_, moduleId_)
{
}

Node* IntNode::Clone(CloneContext& cloneContext) const
{
    IntNode* clone = new IntNode(GetSourcePos(), ModuleId());
    return clone;
}

void IntNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UIntNode::UIntNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::uintNode, sourcePos_, moduleId_)
{
}

Node* UIntNode::Clone(CloneContext& cloneContext) const
{
    UIntNode* clone = new UIntNode(GetSourcePos(), ModuleId());
    return clone;
}

void UIntNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LongNode::LongNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::longNode, sourcePos_, moduleId_)
{
}

Node* LongNode::Clone(CloneContext& cloneContext) const
{
    LongNode* clone = new LongNode(GetSourcePos(), ModuleId());
    return clone;
}

void LongNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ULongNode::ULongNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::ulongNode, sourcePos_, moduleId_)
{
}

Node* ULongNode::Clone(CloneContext& cloneContext) const
{
    ULongNode* clone = new ULongNode(GetSourcePos(), ModuleId());
    return clone;
}

void ULongNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FloatNode::FloatNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::floatNode, sourcePos_, moduleId_)
{
}

Node* FloatNode::Clone(CloneContext& cloneContext) const
{
    FloatNode* clone = new FloatNode(GetSourcePos(), ModuleId());
    return clone;
}

void FloatNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DoubleNode::DoubleNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::doubleNode, sourcePos_, moduleId_)
{
}

Node* DoubleNode::Clone(CloneContext& cloneContext) const
{
    DoubleNode* clone = new DoubleNode(GetSourcePos(), ModuleId());
    return clone;
}

void DoubleNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CharNode::CharNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::charNode, sourcePos_, moduleId_)
{
}

Node* CharNode::Clone(CloneContext& cloneContext) const
{
    CharNode* clone = new CharNode(GetSourcePos(), ModuleId());
    return clone;
}

void CharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WCharNode::WCharNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::wcharNode, sourcePos_, moduleId_)
{
}

Node* WCharNode::Clone(CloneContext& cloneContext) const
{
    WCharNode* clone = new WCharNode(GetSourcePos(), ModuleId());
    return clone;
}

void WCharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

UCharNode::UCharNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::ucharNode, sourcePos_, moduleId_)
{
}

Node* UCharNode::Clone(CloneContext& cloneContext) const
{
    UCharNode* clone = new UCharNode(GetSourcePos(), ModuleId());
    return clone;
}

void UCharNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

VoidNode::VoidNode(const soul::lexer::SourcePos& sourcePos_, const boost::uuids::uuid& moduleId_) : Node(NodeType::voidNode, sourcePos_, moduleId_)
{
}

Node* VoidNode::Clone(CloneContext& cloneContext) const
{
    VoidNode* clone = new VoidNode(GetSourcePos(), ModuleId());
    return clone;
}

void VoidNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::ast
