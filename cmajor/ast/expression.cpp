// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.expression;

import std.core;
import cmajor.ast.identifier;
import cmajor.ast.visitor;
import cmajor.ast.writer;
import cmajor.ast.reader;
import util;

namespace cmajor::ast {

DotNode::DotNode(const soul::ast::Span& span_) : UnaryNode(NodeType::dotNode, span_), memberId()
{
}

DotNode::DotNode(const soul::ast::Span& span_, Node* subject_, IdentifierNode* memberId_) : 
    UnaryNode(NodeType::dotNode, span_, subject_), memberId(memberId_)
{
    Union(Subject());
    Union(memberId.get());
    memberId->SetParent(this);
}

Node* DotNode::Clone(CloneContext& cloneContext) const
{
    DotNode* clone = new DotNode(GetSpan(), Subject()->Clone(cloneContext), static_cast<IdentifierNode*>(memberId->Clone(cloneContext)));
    return clone;
}

void DotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DotNode::Write(AstWriter& writer)
{
    UnaryNode::Write(writer);
    writer.Write(memberId.get());
}

void DotNode::Read(AstReader& reader)
{
    UnaryNode::Read(reader);
    memberId.reset(reader.ReadIdentifierNode());
    memberId->SetParent(this);
}

std::string DotNode::ToString() const
{
    return Subject()->ToString() + "." + memberId->ToString();
}

ArrowNode::ArrowNode(const soul::ast::Span& span_) : UnaryNode(NodeType::arrowNode, span_), memberId()
{
}

ArrowNode::ArrowNode(const soul::ast::Span& span_, Node* subject_, IdentifierNode* memberId_) :
    UnaryNode(NodeType::arrowNode, span_, subject_), memberId(memberId_)
{
    Union(Subject());
    Union(memberId.get());
    memberId->SetParent(this);
}

Node* ArrowNode::Clone(CloneContext& cloneContext) const
{
    ArrowNode* clone = new ArrowNode(GetSpan(), Subject()->Clone(cloneContext), static_cast<IdentifierNode*>(memberId->Clone(cloneContext)));
    return clone;
}

void ArrowNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArrowNode::Write(AstWriter& writer)
{
    UnaryNode::Write(writer);
    writer.Write(memberId.get());
}

void ArrowNode::Read(AstReader& reader)
{
    UnaryNode::Read(reader);
    memberId.reset(reader.ReadIdentifierNode());
    memberId->SetParent(this);
}

std::string ArrowNode::ToString() const
{
    return Subject()->ToString() + "->" + memberId->ToString();
}

EquivalenceNode::EquivalenceNode(const soul::ast::Span& span_) : BinaryNode(NodeType::equivalenceNode, span_)
{
}

EquivalenceNode::EquivalenceNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::equivalenceNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* EquivalenceNode::Clone(CloneContext& cloneContext) const
{
    EquivalenceNode* clone = new EquivalenceNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void EquivalenceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string EquivalenceNode::ToString() const
{
    return Left()->ToString() + "<=>" + Right()->ToString();
}

ImplicationNode::ImplicationNode(const soul::ast::Span& span_) : BinaryNode(NodeType::implicationNode, span_)
{
}

ImplicationNode::ImplicationNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::implicationNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* ImplicationNode::Clone(CloneContext& cloneContext) const
{
    ImplicationNode* clone = new ImplicationNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void ImplicationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ImplicationNode::ToString() const
{
    return Left()->ToString() + "=>" + Right()->ToString();
}

DisjunctionNode::DisjunctionNode(const soul::ast::Span& span_) : BinaryNode(NodeType::disjunctionNode, span_)
{
}

DisjunctionNode::DisjunctionNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::disjunctionNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* DisjunctionNode::Clone(CloneContext& cloneContext) const
{
    DisjunctionNode* clone = new DisjunctionNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void DisjunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string DisjunctionNode::ToString() const
{
    return Left()->ToString() + " || " + Right()->ToString();
}

ConjunctionNode::ConjunctionNode(const soul::ast::Span& span_) : BinaryNode(NodeType::conjunctionNode, span_)
{
}

ConjunctionNode::ConjunctionNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::conjunctionNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* ConjunctionNode::Clone(CloneContext& cloneContext) const
{
    ConjunctionNode* clone = new ConjunctionNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void ConjunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ConjunctionNode::ToString() const
{
    return Left()->ToString() + " && " + Right()->ToString();
}

BitOrNode::BitOrNode(const soul::ast::Span& span_) : BinaryNode(NodeType::bitOrNode, span_)
{
}

BitOrNode::BitOrNode(const soul::ast::Span& span_, Node* left_, Node* right_) : BinaryNode(NodeType::bitOrNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* BitOrNode::Clone(CloneContext& cloneContext) const
{
    BitOrNode* clone = new BitOrNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void BitOrNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string BitOrNode::ToString() const
{
    return Left()->ToString() + " | " + Right()->ToString();
}

BitXorNode::BitXorNode(const soul::ast::Span& span_) : BinaryNode(NodeType::bitXorNode, span_)
{
}

BitXorNode::BitXorNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::bitXorNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* BitXorNode::Clone(CloneContext& cloneContext) const
{
    BitXorNode* clone = new BitXorNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void BitXorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string BitXorNode::ToString() const
{
    return Left()->ToString() + " ^ " + Right()->ToString();
}

BitAndNode::BitAndNode(const soul::ast::Span& span_) : BinaryNode(NodeType::bitAndNode, span_)
{
}

BitAndNode::BitAndNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::bitAndNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* BitAndNode::Clone(CloneContext& cloneContext) const
{
    BitAndNode* clone = new BitAndNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void BitAndNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string BitAndNode::ToString() const
{
    return Left()->ToString() + " & " + Right()->ToString();
}

EqualNode::EqualNode(const soul::ast::Span& span_) : BinaryNode(NodeType::equalNode, span_)
{
}

EqualNode::EqualNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::equalNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* EqualNode::Clone(CloneContext& cloneContext) const
{
    EqualNode* clone = new EqualNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void EqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string EqualNode::ToString() const
{
    return Left()->ToString() + " == " + Right()->ToString();
}

NotEqualNode::NotEqualNode(const soul::ast::Span& span_) : BinaryNode(NodeType::notEqualNode, span_)
{
}

NotEqualNode::NotEqualNode(const soul::ast::Span& span_, Node* left_, Node* right_) : BinaryNode(NodeType::notEqualNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* NotEqualNode::Clone(CloneContext& cloneContext) const
{
    NotEqualNode* clone = new NotEqualNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void NotEqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string NotEqualNode::ToString() const
{
    return Left()->ToString() + " != " + Right()->ToString();
}

LessNode::LessNode(const soul::ast::Span& span_) : BinaryNode(NodeType::lessNode, span_)
{
}

LessNode::LessNode(const soul::ast::Span& span_, Node* left_, Node* right_) : BinaryNode(NodeType::lessNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* LessNode::Clone(CloneContext& cloneContext) const
{
    LessNode* clone = new LessNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void LessNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string LessNode::ToString() const
{
    return Left()->ToString() + " < " + Right()->ToString();
}

GreaterNode::GreaterNode(const soul::ast::Span& span_) : BinaryNode(NodeType::greaterNode, span_)
{
}

GreaterNode::GreaterNode(const soul::ast::Span& span_, Node* left_, Node* right_) : BinaryNode(NodeType::greaterNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* GreaterNode::Clone(CloneContext& cloneContext) const
{
    GreaterNode* clone = new GreaterNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void GreaterNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string GreaterNode::ToString() const
{
    return Left()->ToString() + " > " + Right()->ToString();
}

LessOrEqualNode::LessOrEqualNode(const soul::ast::Span& span_) : BinaryNode(NodeType::lessOrEqualNode, span_)
{
}

LessOrEqualNode::LessOrEqualNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::lessOrEqualNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* LessOrEqualNode::Clone(CloneContext& cloneContext) const
{
    LessOrEqualNode* clone = new LessOrEqualNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void LessOrEqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string LessOrEqualNode::ToString() const
{
    return Left()->ToString() + " <= " + Right()->ToString();
}

GreaterOrEqualNode::GreaterOrEqualNode(const soul::ast::Span& span_) : BinaryNode(NodeType::greaterOrEqualNode, span_)
{
}

GreaterOrEqualNode::GreaterOrEqualNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::greaterOrEqualNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* GreaterOrEqualNode::Clone(CloneContext& cloneContext) const
{
    GreaterOrEqualNode* clone = new GreaterOrEqualNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void GreaterOrEqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string GreaterOrEqualNode::ToString() const
{
    return Left()->ToString() + " >= " + Right()->ToString();
}

ShiftLeftNode::ShiftLeftNode(const soul::ast::Span& span_) : BinaryNode(NodeType::shiftLeftNode, span_)
{
}

ShiftLeftNode::ShiftLeftNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::shiftLeftNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* ShiftLeftNode::Clone(CloneContext& cloneContext) const
{
    ShiftLeftNode* clone = new ShiftLeftNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void ShiftLeftNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ShiftLeftNode::ToString() const
{
    return Left()->ToString() + " << " + Right()->ToString();
}

ShiftRightNode::ShiftRightNode(const soul::ast::Span& span_) : BinaryNode(NodeType::shiftRightNode, span_)
{
}

ShiftRightNode::ShiftRightNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::shiftRightNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* ShiftRightNode::Clone(CloneContext& cloneContext) const
{
    ShiftRightNode* clone = new ShiftRightNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void ShiftRightNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ShiftRightNode::ToString() const
{
    return Left()->ToString() + " >> " + Right()->ToString();
}

AddNode::AddNode(const soul::ast::Span& span_) : BinaryNode(NodeType::addNode, span_)
{
}

AddNode::AddNode(const soul::ast::Span& span_, Node* left_, Node* right_) : BinaryNode(NodeType::addNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* AddNode::Clone(CloneContext& cloneContext) const
{
    AddNode* clone = new AddNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void AddNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string AddNode::ToString() const
{
    return Left()->ToString() + " + " + Right()->ToString();
}

SubNode::SubNode(const soul::ast::Span& span_) : BinaryNode(NodeType::subNode, span_)
{
}

SubNode::SubNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::subNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* SubNode::Clone(CloneContext& cloneContext) const
{
    SubNode* clone = new SubNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void SubNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string SubNode::ToString() const
{
    return Left()->ToString() + " - " + Right()->ToString();
}

MulNode::MulNode(const soul::ast::Span& span_) : BinaryNode(NodeType::mulNode, span_)
{
}

MulNode::MulNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::mulNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* MulNode::Clone(CloneContext& cloneContext) const
{
    MulNode* clone = new MulNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void MulNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string MulNode::ToString() const
{
    return Left()->ToString() + " * " + Right()->ToString();
}

DivNode::DivNode(const soul::ast::Span& span_) : BinaryNode(NodeType::divNode, span_)
{
}

DivNode::DivNode(const soul::ast::Span& span_, Node* left_, Node* right_) :
    BinaryNode(NodeType::divNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* DivNode::Clone(CloneContext& cloneContext) const
{
    DivNode* clone = new DivNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void DivNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string DivNode::ToString() const
{
    return Left()->ToString() + " / " + Right()->ToString();
}

RemNode::RemNode(const soul::ast::Span& span_) : BinaryNode(NodeType::remNode, span_)
{
}

RemNode::RemNode(const soul::ast::Span& span_, Node* left_, Node* right_) : BinaryNode(NodeType::remNode, span_, left_, right_)
{
    Union(Left());
    Union(Right());
}

Node* RemNode::Clone(CloneContext& cloneContext) const
{
    RemNode* clone = new RemNode(GetSpan(), Left()->Clone(cloneContext), Right()->Clone(cloneContext));
    return clone;
}

void RemNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string RemNode::ToString() const
{
    return Left()->ToString() + " % " + Right()->ToString();
}

NotNode::NotNode(const soul::ast::Span& span_) : UnaryNode(NodeType::notNode, span_)
{
}

NotNode::NotNode(const soul::ast::Span& span_, Node* subject_) : UnaryNode(NodeType::notNode, span_, subject_)
{
    Union(Subject());
}

Node* NotNode::Clone(CloneContext& cloneContext) const
{
    NotNode* clone = new NotNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void NotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string NotNode::ToString() const
{
    return "!" + Subject()->ToString();
}

UnaryPlusNode::UnaryPlusNode(const soul::ast::Span& span_) : UnaryNode(NodeType::unaryPlusNode, span_)
{
}

UnaryPlusNode::UnaryPlusNode(const soul::ast::Span& span_, Node* subject_) : UnaryNode(NodeType::unaryPlusNode, span_, subject_)
{
    Union(Subject());
}

Node* UnaryPlusNode::Clone(CloneContext& cloneContext) const
{
    UnaryPlusNode* clone = new UnaryPlusNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void UnaryPlusNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string UnaryPlusNode::ToString() const
{
    return "+" + Subject()->ToString();
}

UnaryMinusNode::UnaryMinusNode(const soul::ast::Span& span_) : UnaryNode(NodeType::unaryMinusNode, span_)
{
}

UnaryMinusNode::UnaryMinusNode(const soul::ast::Span& span_, Node* subject_) : UnaryNode(NodeType::unaryMinusNode, span_, subject_)
{
    Union(Subject());
}

Node* UnaryMinusNode::Clone(CloneContext& cloneContext) const
{
    UnaryMinusNode* clone = new UnaryMinusNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void UnaryMinusNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string UnaryMinusNode::ToString() const
{
    return "-" + Subject()->ToString();
}

PrefixIncrementNode::PrefixIncrementNode(const soul::ast::Span& span_) : UnaryNode(NodeType::prefixIncrementNode, span_)
{
}

PrefixIncrementNode::PrefixIncrementNode(const soul::ast::Span& span_, Node* subject_) :
    UnaryNode(NodeType::prefixIncrementNode, span_, subject_)
{
    Union(Subject());
}

Node* PrefixIncrementNode::Clone(CloneContext& cloneContext) const
{
    PrefixIncrementNode* clone = new PrefixIncrementNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void PrefixIncrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string PrefixIncrementNode::ToString() const
{
    return "++" + Subject()->ToString();
}

PrefixDecrementNode::PrefixDecrementNode(const soul::ast::Span& span_) : UnaryNode(NodeType::prefixDecrementNode, span_)
{
}

PrefixDecrementNode::PrefixDecrementNode(const soul::ast::Span& span_, Node* subject_) :
    UnaryNode(NodeType::prefixDecrementNode, span_, subject_)
{
    Union(Subject());
}

Node* PrefixDecrementNode::Clone(CloneContext& cloneContext) const
{
    PrefixDecrementNode* clone = new PrefixDecrementNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void PrefixDecrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string PrefixDecrementNode::ToString() const
{
    return "--" + Subject()->ToString();
}

ComplementNode::ComplementNode(const soul::ast::Span& span_) : UnaryNode(NodeType::complementNode, span_)
{
}

ComplementNode::ComplementNode(const soul::ast::Span& span_, Node* subject_) : UnaryNode(NodeType::complementNode, span_, subject_)
{
    Union(Subject());
}

Node* ComplementNode::Clone(CloneContext& cloneContext) const
{
    ComplementNode* clone = new ComplementNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void ComplementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ComplementNode::ToString() const
{
    return "~" + Subject()->ToString();
}

DerefNode::DerefNode(const soul::ast::Span& span_) : UnaryNode(NodeType::derefNode, span_)
{
}

DerefNode::DerefNode(const soul::ast::Span& span_, Node* subject_) : UnaryNode(NodeType::derefNode, span_, subject_)
{
    Union(Subject());
}

Node* DerefNode::Clone(CloneContext& cloneContext) const
{
    DerefNode* clone = new DerefNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void DerefNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string DerefNode::ToString() const
{
    return "*" + Subject()->ToString();
}

AddrOfNode::AddrOfNode(const soul::ast::Span& span_) : UnaryNode(NodeType::addrOfNode, span_)
{
}

AddrOfNode::AddrOfNode(const soul::ast::Span& span_, Node* subject_) :
    UnaryNode(NodeType::addrOfNode, span_, subject_)
{
    Union(Subject());
}

Node* AddrOfNode::Clone(CloneContext& cloneContext) const
{
    AddrOfNode* clone = new AddrOfNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void AddrOfNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string AddrOfNode::ToString() const
{
    return "&" + Subject()->ToString();
}

IsNode::IsNode(const soul::ast::Span& span_) : Node(NodeType::isNode, span_), expr(), targetTypeExpr()
{
}

IsNode::IsNode(const soul::ast::Span& span_, Node* expr_, Node* targetTypeExpr_) :
    Node(NodeType::isNode, span_), expr(expr_), targetTypeExpr(targetTypeExpr_)
{
    expr->SetParent(this);
    targetTypeExpr->SetParent(this);
    Union(expr.get());
    Union(targetTypeExpr.get());
}

Node* IsNode::Clone(CloneContext& cloneContext) const
{
    IsNode* clone = new IsNode(GetSpan(), expr->Clone(cloneContext), targetTypeExpr->Clone(cloneContext));
    return clone;
}

void IsNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IsNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(expr.get());
    writer.Write(targetTypeExpr.get());
}

void IsNode::Read(AstReader& reader)
{
    Node::Read(reader);
    expr.reset(reader.ReadNode());
    expr->SetParent(this);
    targetTypeExpr.reset(reader.ReadNode());
    targetTypeExpr->SetParent(this);
}

std::string IsNode::ToString() const
{
    return expr->ToString() + " is " + targetTypeExpr->ToString();
}

AsNode::AsNode(const soul::ast::Span& span_) : Node(NodeType::asNode, span_), expr(), targetTypeExpr()
{
}

AsNode::AsNode(const soul::ast::Span& span_, Node* expr_, Node* targetTypeExpr_) : Node(NodeType::asNode, span_), expr(expr_), targetTypeExpr(targetTypeExpr_)
{
    expr->SetParent(this);
    targetTypeExpr->SetParent(this);
    Union(expr.get());
    Union(targetTypeExpr.get());
}

Node* AsNode::Clone(CloneContext& cloneContext) const
{
    AsNode* clone = new AsNode(GetSpan(), expr->Clone(cloneContext), targetTypeExpr->Clone(cloneContext));
    return clone;
}

void AsNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AsNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(expr.get());
    writer.Write(targetTypeExpr.get());
}

void AsNode::Read(AstReader& reader)
{
    Node::Read(reader);
    expr.reset(reader.ReadNode());
    expr->SetParent(this);
    targetTypeExpr.reset(reader.ReadNode());
    targetTypeExpr->SetParent(this);
}

std::string AsNode::ToString() const
{
    return expr->ToString() + " as " + targetTypeExpr->ToString();
}

IndexingNode::IndexingNode(const soul::ast::Span& span_) : Node(NodeType::indexingNode, span_), subject(), index()
{
}

IndexingNode::IndexingNode(const soul::ast::Span& span_, Node* subject_, Node* index_) :
    Node(NodeType::indexingNode, span_), subject(subject_), index(index_)
{
    subject->SetParent(this);
    index->SetParent(this);
    Union(subject.get());
    Union(index.get());
}

Node* IndexingNode::Clone(CloneContext& cloneContext) const
{
    IndexingNode* clone = new IndexingNode(GetSpan(), subject->Clone(cloneContext), index->Clone(cloneContext));
    return clone;
}

void IndexingNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IndexingNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(subject.get());
    writer.Write(index.get());
}

void IndexingNode::Read(AstReader& reader)
{
    Node::Read(reader);
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
    index.reset(reader.ReadNode());
    index->SetParent(this);
}

std::string IndexingNode::ToString() const
{
    return subject->ToString() + "[" + index->ToString() + "]";
}

InvokeNode::InvokeNode(const soul::ast::Span& span_) : Node(NodeType::invokeNode, span_), subject(), arguments()
{
}

InvokeNode::InvokeNode(const soul::ast::Span& span_, Node* subject_) :
    Node(NodeType::invokeNode, span_), subject(subject_), arguments()
{
    subject->SetParent(this);
    Union(subject.get());
}

Node* InvokeNode::Clone(CloneContext& cloneContext) const
{
    InvokeNode* clone = new InvokeNode(GetSpan(), subject->Clone(cloneContext));
    int n = arguments.Count();
    for (int i = 0; i < n; ++i)
    {
        Node* argument = arguments[i];
        clone->AddArgument(argument->Clone(cloneContext));
    }
    return clone;
}

void InvokeNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void InvokeNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(subject.get());
    arguments.Write(writer);
}

void InvokeNode::Read(AstReader& reader)
{
    Node::Read(reader);
    subject.reset(reader.ReadNode());
    subject->SetParent(this);
    arguments.Read(reader);
    arguments.SetParent(this);
}

void InvokeNode::AddArgument(Node* argument)
{
    Union(argument);
    argument->SetParent(this);
    arguments.Add(argument);
}

std::string InvokeNode::ToString() const
{
    std::string s = subject->ToString();
    s.append("(");
    int n = arguments.Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            s.append(", ");
        }
        s.append(arguments[i]->ToString());
    }
    s.append(")");
    return s;
}

PostfixIncrementNode::PostfixIncrementNode(const soul::ast::Span& span_) : UnaryNode(NodeType::postfixIncrementNode, span_)
{
}

PostfixIncrementNode::PostfixIncrementNode(const soul::ast::Span& span_, Node* subject_) :
    UnaryNode(NodeType::postfixIncrementNode, span_, subject_)
{
    Union(Subject());
}

Node* PostfixIncrementNode::Clone(CloneContext& cloneContext) const
{
    PostfixIncrementNode* clone = new PostfixIncrementNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void PostfixIncrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string PostfixIncrementNode::ToString() const
{
    return Subject()->ToString() + "++";
}

PostfixDecrementNode::PostfixDecrementNode(const soul::ast::Span& span_) : UnaryNode(NodeType::postfixDecrementNode, span_)
{
}

PostfixDecrementNode::PostfixDecrementNode(const soul::ast::Span& span_, Node* subject_) : UnaryNode(NodeType::postfixDecrementNode, span_, subject_)
{
    Union(Subject());
}

Node* PostfixDecrementNode::Clone(CloneContext& cloneContext) const
{
    PostfixDecrementNode* clone = new PostfixDecrementNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void PostfixDecrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string PostfixDecrementNode::ToString() const
{
    return Subject()->ToString() + "--";
}

SizeOfNode::SizeOfNode(const soul::ast::Span& span_) : Node(NodeType::sizeOfNode, span_), expression()
{
}

SizeOfNode::SizeOfNode(const soul::ast::Span& span_, Node* expression_) :
    Node(NodeType::sizeOfNode, span_), expression(expression_)
{
    expression->SetParent(this);
    Union(expression.get());
}

Node* SizeOfNode::Clone(CloneContext& cloneContext) const
{
    SizeOfNode* clone = new SizeOfNode(GetSpan(), expression->Clone(cloneContext));
    return clone;
}

void SizeOfNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void SizeOfNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(expression.get());
}

void SizeOfNode::Read(AstReader& reader)
{
    Node::Read(reader);
    expression.reset(reader.ReadNode());
    expression->SetParent(this);
}

std::string SizeOfNode::ToString() const
{
    return "sizeof(" + expression->ToString() + ")";
}

TypeNameNode::TypeNameNode(const soul::ast::Span& span_) : Node(NodeType::typeNameNode, span_), expression(), static_(false)
{
}

TypeNameNode::TypeNameNode(const soul::ast::Span& span_, Node* expression_) :
    Node(NodeType::typeNameNode, span_), expression(expression_), static_(false)
{
    expression->SetParent(this);
    Union(expression.get());
}

Node* TypeNameNode::Clone(CloneContext& cloneContext) const
{
    TypeNameNode* clone = new TypeNameNode(GetSpan(), expression->Clone(cloneContext));
    if (static_)
    {
        clone->SetStatic();
    }
    return clone;
}

void TypeNameNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeNameNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(expression.get());
    writer.GetBinaryStreamWriter().Write(static_);
}

void TypeNameNode::Read(AstReader& reader)
{
    Node::Read(reader);
    expression.reset(reader.ReadNode());
    expression->SetParent(this);
    static_ = reader.GetBinaryStreamReader().ReadBool();
}

std::string TypeNameNode::ToString() const
{
    return "typename(" + expression->ToString() + ")";
}

TypeIdNode::TypeIdNode(const soul::ast::Span& span_) : Node(NodeType::typeIdNode, span_), expression()
{
}

TypeIdNode::TypeIdNode(const soul::ast::Span& span_, Node* expression_) : Node(NodeType::typeIdNode, span_), expression(expression_)
{
    expression->SetParent(this);
    Union(expression.get());
}

Node* TypeIdNode::Clone(CloneContext& cloneContext) const
{
    TypeIdNode* clone = new TypeIdNode(GetSpan(), expression->Clone(cloneContext));
    return clone;
}

void TypeIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeIdNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(expression.get());
}

void TypeIdNode::Read(AstReader& reader)
{
    Node::Read(reader);
    expression.reset(reader.ReadNode());
    expression->SetParent(this);
}

std::string TypeIdNode::ToString() const
{
    return "typeid(" + expression->ToString() + ")";
}

CastNode::CastNode(const soul::ast::Span& span_) : Node(NodeType::castNode, span_), targetTypeExpr(), sourceExpr()
{
}

CastNode::CastNode(const soul::ast::Span& span_, Node* targetTypeExpr_, Node* sourceExpr_) :
    Node(NodeType::castNode, span_), targetTypeExpr(targetTypeExpr_), sourceExpr(sourceExpr_)
{
    targetTypeExpr->SetParent(this);
    sourceExpr->SetParent(this);
    Union(targetTypeExpr.get());
    Union(sourceExpr.get());
}

Node* CastNode::Clone(CloneContext& cloneContext) const
{
    CastNode* clone = new CastNode(GetSpan(), targetTypeExpr->Clone(cloneContext), sourceExpr->Clone(cloneContext));
    return clone;
}

void CastNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void CastNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(targetTypeExpr.get());
    writer.Write(sourceExpr.get());
}

void CastNode::Read(AstReader& reader)
{
    Node::Read(reader);
    targetTypeExpr.reset(reader.ReadNode());
    targetTypeExpr->SetParent(this);
    sourceExpr.reset(reader.ReadNode());
    sourceExpr->SetParent(this);
}

std::string CastNode::ToString() const
{
    return "cast<" + targetTypeExpr->ToString() + ">(" + sourceExpr->ToString() + ")";
}

ConstructNode::ConstructNode(const soul::ast::Span& span_) : Node(NodeType::constructNode, span_), typeExpr(), arguments()
{
}

ConstructNode::ConstructNode(const soul::ast::Span& span_, Node* typeExpr_) :
    Node(NodeType::constructNode, span_), typeExpr(typeExpr_), arguments()
{
    typeExpr->SetParent(this);
    Union(typeExpr.get());
}

Node* ConstructNode::Clone(CloneContext& cloneContext) const
{
    ConstructNode* clone = new ConstructNode(GetSpan(), typeExpr->Clone(cloneContext));
    int n = arguments.Count();
    for (int i = 0; i < n; ++i)
    {
        Node* argument = arguments[i];
        clone->AddArgument(argument->Clone(cloneContext));
    }
    return clone;
}

void ConstructNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstructNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(typeExpr.get());
    arguments.Write(writer);
}

void ConstructNode::Read(AstReader& reader)
{
    Node::Read(reader);
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    arguments.Read(reader);
    arguments.SetParent(this);
}

void ConstructNode::AddArgument(Node* argument)
{
    Union(argument);
    argument->SetParent(this);
    arguments.Add(argument);
}

std::string ConstructNode::ToString() const
{
    std::string s = "construct<" + typeExpr->ToString() + ">(";
    int n = arguments.Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            s.append(", ");
        }
        s.append(arguments[i]->ToString());
    }
    s.append(")");
    return s;
}

NewNode::NewNode(const soul::ast::Span& span_) : Node(NodeType::newNode, span_), typeExpr(), arguments()
{
}

NewNode::NewNode(const soul::ast::Span& span_, Node* typeExpr_) :
    Node(NodeType::newNode, span_), typeExpr(typeExpr_), arguments()
{
    typeExpr->SetParent(this);
    Union(typeExpr.get());
}

Node* NewNode::Clone(CloneContext& cloneContext) const
{
    NewNode* clone = new NewNode(GetSpan(), typeExpr->Clone(cloneContext));
    int n = arguments.Count();
    for (int i = 0; i < n; ++i)
    {
        Node* argument = arguments[i];
        clone->AddArgument(argument->Clone(cloneContext));
    }
    return clone;
}

void NewNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void NewNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(typeExpr.get());
    arguments.Write(writer);
}

void NewNode::Read(AstReader& reader)
{
    Node::Read(reader);
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    arguments.Read(reader);
    arguments.SetParent(this);
}

void NewNode::AddArgument(Node* argument)
{
    Union(argument);
    argument->SetParent(this);
    arguments.Add(argument);
}

std::string NewNode::ToString() const
{
    std::string s = "new ";
    s.append(typeExpr->ToString()).append("(");
    int n = arguments.Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            s.append(", ");
        }
        s.append(arguments[i]->ToString());
    }
    s.append(")");
    return s;
}

ThisNode::ThisNode(const soul::ast::Span& span_) : Node(NodeType::thisNode, span_)
{
}

Node* ThisNode::Clone(CloneContext& cloneContext) const
{
    ThisNode* clone = new ThisNode(GetSpan());
    return clone;
}

void ThisNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ThisNode::ToString() const
{
    return "this";
}

BaseNode::BaseNode(const soul::ast::Span& span_) : Node(NodeType::baseNode, span_)
{
}

Node* BaseNode::Clone(CloneContext& cloneContext) const
{
    BaseNode* clone = new BaseNode(GetSpan());
    return clone;
}

void BaseNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string BaseNode::ToString() const
{
    return "base";
}

ParenthesizedExpressionNode::ParenthesizedExpressionNode(const soul::ast::Span& span_) :
    UnaryNode(NodeType::parenthesizedExpressionNode, span_)
{
}

ParenthesizedExpressionNode::ParenthesizedExpressionNode(const soul::ast::Span& span_, Node* child_) :
    UnaryNode(NodeType::parenthesizedExpressionNode, span_, child_)
{
    Union(Subject());
}

Node* ParenthesizedExpressionNode::Clone(CloneContext& cloneContext) const
{
    ParenthesizedExpressionNode* clone = new ParenthesizedExpressionNode(GetSpan(), Subject()->Clone(cloneContext));
    return clone;
}

void ParenthesizedExpressionNode::Write(AstWriter& writer)
{
    UnaryNode::Write(writer);
}

void ParenthesizedExpressionNode::Read(AstReader& reader)
{
    UnaryNode::Read(reader);
}

void ParenthesizedExpressionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ParenthesizedExpressionNode::ToString() const
{
    return "(" + Subject()->ToString() + ")";
}

} // namespace cmajor::ast
