// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.expression;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

EquivalenceNode::EquivalenceNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"<=>")
{
}

Node* EquivalenceNode::Clone() const
{
    EquivalenceNode* clone = new EquivalenceNode(GetSpan(), GetCompletionContext());
    return clone;
}

void EquivalenceNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ImplicationNode::ImplicationNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"=>")
{
}

Node* ImplicationNode::Clone() const
{
    ImplicationNode* clone = new ImplicationNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ImplicationNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DisjunctionNode::DisjunctionNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"||")
{
}

Node* DisjunctionNode::Clone() const
{
    DisjunctionNode* clone = new DisjunctionNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DisjunctionNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BitOrNode::BitOrNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"|")
{
}

Node* BitOrNode::Clone() const
{
    BitOrNode* clone = new BitOrNode(GetSpan(), GetCompletionContext());
    return clone;
}

void BitOrNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BitXorNode::BitXorNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"^")
{
}

Node* BitXorNode::Clone() const
{
    BitXorNode* clone = new BitXorNode(GetSpan(), GetCompletionContext());
    return clone;
}

void BitXorNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EqualNode::EqualNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"==")
{
}

Node* EqualNode::Clone() const
{
    EqualNode* clone = new EqualNode(GetSpan(), GetCompletionContext());
    return clone;
}

void EqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NotEqualNode::NotEqualNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"!=")
{
}

Node* NotEqualNode::Clone() const
{
    NotEqualNode* clone = new NotEqualNode(GetSpan(), GetCompletionContext());
    return clone;
}

void NotEqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

LessEqualNode::LessEqualNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"<=")
{
}

Node* LessEqualNode::Clone() const
{
    LessEqualNode* clone = new LessEqualNode(GetSpan(), GetCompletionContext());
    return clone;
}

void LessEqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

GreaterEqualNode::GreaterEqualNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u">=")
{
}

Node* GreaterEqualNode::Clone() const
{
    GreaterEqualNode* clone = new GreaterEqualNode(GetSpan(), GetCompletionContext());
    return clone;
}

void GreaterEqualNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShiftLeftNode::ShiftLeftNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"<<")
{
}

Node* ShiftLeftNode::Clone() const
{
    ShiftLeftNode* clone = new ShiftLeftNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ShiftLeftNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ShiftRightNode::ShiftRightNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u">>")
{
}

Node* ShiftRightNode::Clone() const
{
    ShiftRightNode* clone = new ShiftRightNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ShiftRightNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PlusNode::PlusNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"+")
{
}

Node* PlusNode::Clone() const
{
    PlusNode* clone = new PlusNode(GetSpan(), GetCompletionContext());
    return clone;
}

void PlusNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MinusNode::MinusNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"-")
{
}

Node* MinusNode::Clone() const
{
    MinusNode* clone = new MinusNode(GetSpan(), GetCompletionContext());
    return clone;
}

void MinusNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DivNode::DivNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"/")
{
}

Node* DivNode::Clone() const
{
    DivNode* clone = new DivNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DivNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

RemNode::RemNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"%")
{
}

Node* RemNode::Clone() const
{
    RemNode* clone = new RemNode(GetSpan(), GetCompletionContext());
    return clone;
}

void RemNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IncrementNode::IncrementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"++")
{
}

Node* IncrementNode::Clone() const
{
    IncrementNode* clone = new IncrementNode(GetSpan(), GetCompletionContext());
    return clone;
}

void IncrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DecrementNode::DecrementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"--")
{
}

Node* DecrementNode::Clone() const
{
    DecrementNode* clone = new DecrementNode(GetSpan(), GetCompletionContext());
    return clone;
}

void DecrementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NotNode::NotNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"!")
{
}

Node* NotNode::Clone() const
{
    NotNode* clone = new NotNode(GetSpan(), GetCompletionContext());
    return clone;
}

void NotNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ComplementNode::ComplementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : TokenNode(span_, completionContext_), str(u"~")
{
}

Node* ComplementNode::Clone() const
{
    ComplementNode* clone = new ComplementNode(GetSpan(), GetCompletionContext());
    return clone;
}

void ComplementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

BinaryExprNode::BinaryExprNode(Node* left_, Node* right_, Node* op_) : BinaryNode(left_, right_), op(op_)
{
    AddChildNode(op.get());
}

Node* BinaryExprNode::Clone() const
{
    BinaryExprNode* clone = new BinaryExprNode(Left()->Clone(), Right()->Clone(), op->Clone());
    clone->Make();
    return clone;
}

void BinaryExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PrefixExprNode::PrefixExprNode(Node* operand_, Node* op_) : UnaryNode(operand_), op(op_)
{
    AddChildNode(op.get());
}

Node* PrefixExprNode::Clone() const
{
    PrefixExprNode* clone = new PrefixExprNode(Operand()->Clone(), op->Clone());
    clone->Make();
    return clone;
}

void PrefixExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PostfixExprNode::PostfixExprNode(Node* operand_, Node* op_) : UnaryNode(operand_), op(op_)
{
    AddChildNode(op.get());
}

Node* PostfixExprNode::Clone() const
{
    PostfixExprNode* clone = new PostfixExprNode(Operand()->Clone(), op->Clone());
    clone->Make();
    return clone;
}

void PostfixExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SubscriptExprNode::SubscriptExprNode(Node* subject_, LBracketNode* lbracket_, Node* index_, RBracketNode* rbracket_) :
    UnaryNode(subject_), lbracket(lbracket_), index(index_), rbracket(rbracket_)
{
    AddChildNode(lbracket.get());
    AddChildNode(index.get());
    AddChildNode(rbracket.get());
}

Node* SubscriptExprNode::Clone() const
{
    SubscriptExprNode* clone = new SubscriptExprNode(Operand()->Clone(), static_cast<LBracketNode*>(lbracket->Clone()), index->Clone(), static_cast<RBracketNode*>(rbracket->Clone()));
    clone->Make();
    return clone;
}

void SubscriptExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

InvokeExprNode::InvokeExprNode(Node* subject_, LParenNode* lparen_) : UnaryNode(subject_), lparen(lparen_), argumentList(soul::ast::Span(), CompletionContext::none)
{
    AddChildNode(lparen.get());
}

void InvokeExprNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void InvokeExprNode::AddNode(Node* node)
{
    argumentList.AddNode(node);
}

void InvokeExprNode::AddComma(CommaNode* comma)
{
    argumentList.AddComma(comma);
}

Node* InvokeExprNode::Clone() const
{
    InvokeExprNode* clone = new InvokeExprNode(Operand()->Clone(), static_cast<LParenNode*>(lparen->Clone()));
    clone->argumentList.CloneFrom(argumentList);
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void InvokeExprNode::Make()
{
    argumentList.Make();
    AddChildNode(&argumentList);
    UnaryNode::Make();
}

void InvokeExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IsExprNode::IsExprNode(Node* subject_, Node* type_, Node* op_) : BinaryNode(subject_, type_), op(op_)
{
    AddChildNode(op.get());
}

Node* IsExprNode::Clone() const
{
    IsExprNode* clone = new IsExprNode(Left()->Clone(), Right()->Clone(), op->Clone());
    clone->Make();
    return clone;
}

void IsExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AsExprNode::AsExprNode(Node* subject_, Node* type_, Node* op_) : BinaryNode(subject_, type_), op(op_)
{
    AddChildNode(op.get());
}

Node* AsExprNode::Clone() const
{
    AsExprNode* clone = new AsExprNode(Left()->Clone(), Right()->Clone(), op->Clone());
    clone->Make();
    return clone;
}

void AsExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DotMemberNode::DotMemberNode(Node* subject_, DotNode* dot_, IdentifierNode* id_) : BinaryNode(subject_, id_), dot(dot_)
{
    AddChildNode(dot.get());
}

Node* DotMemberNode::Clone() const
{
    DotMemberNode* clone = new DotMemberNode(Left()->Clone(), static_cast<DotNode*>(dot->Clone()), static_cast<IdentifierNode*>(Right()->Clone()));
    clone->Make();
    return clone;
}

void DotMemberNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ArrowMemberNode::ArrowMemberNode(Node* subject_, ArrowNode* arrow_, IdentifierNode* id_) : BinaryNode(subject_, id_), arrow(arrow_)
{
    AddChildNode(arrow.get());
}

Node* ArrowMemberNode::Clone() const
{
    ArrowMemberNode* clone = new ArrowMemberNode(Left()->Clone(), static_cast<ArrowNode*>(arrow->Clone()), static_cast<IdentifierNode*>(Right()->Clone()));
    clone->Make();
    return clone;
}

void ArrowMemberNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ParenthesizedExprNode::ParenthesizedExprNode(Node* subject_) : UnaryNode(subject_)
{
}

void ParenthesizedExprNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void ParenthesizedExprNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

Node* ParenthesizedExprNode::Clone() const
{
    ParenthesizedExprNode* clone = new ParenthesizedExprNode(Operand()->Clone());
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void ParenthesizedExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

SizeOfExprNode::SizeOfExprNode(SizeOfKeywordNode* sizeOfNode_, Node* subject_) : UnaryNode(subject_), sizeOfNode(sizeOfNode_)
{
    AddChildNode(sizeOfNode.get());
}

void SizeOfExprNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void SizeOfExprNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

Node* SizeOfExprNode::Clone() const
{
    SizeOfExprNode* clone = new SizeOfExprNode(static_cast<SizeOfKeywordNode*>(sizeOfNode->Clone()), Operand()->Clone());
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void SizeOfExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeNameExprNode::TypeNameExprNode(TypeNameKeywordNode* typeNameNode_, Node* subject_) : UnaryNode(subject_), typeNameNode(typeNameNode_)
{
    AddChildNode(typeNameNode.get());
}

void TypeNameExprNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void TypeNameExprNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

Node* TypeNameExprNode::Clone() const
{
    TypeNameExprNode* clone = new TypeNameExprNode(static_cast<TypeNameKeywordNode*>(typeNameNode->Clone()), Operand()->Clone());
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void TypeNameExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeIdExprNode::TypeIdExprNode(TypeIdKeywordNode* typeIdNode_, Node* subject_) : UnaryNode(subject_), typeIdNode(typeIdNode_)
{
    AddChildNode(typeIdNode.get());
}

void TypeIdExprNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void TypeIdExprNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

Node* TypeIdExprNode::Clone() const
{
    TypeIdExprNode* clone = new TypeIdExprNode(static_cast<TypeIdKeywordNode*>(typeIdNode->Clone()), Operand()->Clone());
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void TypeIdExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

CastExprNode::CastExprNode(CastKeywordNode* castNode_, LAngleNode* langle_, Node* typeExpr_, RAngleNode* rangle_, LParenNode* lparen_, Node* expr_, RParenNode* rparen_) :
    BinaryNode(typeExpr_, expr_), castNode(castNode_), langle(langle_), rangle(rangle_), lparen(lparen_), rparen(rparen_)
{
    AddChildNode(castNode.get());
    AddChildNode(langle.get());
    AddChildNode(rangle.get());
    AddChildNode(lparen.get());
    AddChildNode(rparen.get());
}

Node* CastExprNode::Clone() const
{
    CastExprNode* clone = new CastExprNode(static_cast<CastKeywordNode*>(castNode->Clone()), static_cast<LAngleNode*>(langle->Clone()), Left()->Clone(),
        static_cast<RAngleNode*>(rangle->Clone()), static_cast<LParenNode*>(lparen->Clone()), Right()->Clone(), static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void CastExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstructExprNode::ConstructExprNode(ConstructKeywordNode* constructNode_, LAngleNode* langle_, Node* typeExpr_, RAngleNode* rangle_, LParenNode* lparen_) :
    UnaryNode(typeExpr_), constructNode(constructNode_), langle(langle_), rangle(rangle_), lparen(lparen_), args(soul::ast::Span(), CompletionContext::none)
{
    AddChildNode(constructNode.get());
    AddChildNode(langle.get());
    AddChildNode(rangle.get());
    AddChildNode(lparen.get());
}

void ConstructExprNode::AddNode(Node* arg)
{
    args.AddNode(arg);
}

void ConstructExprNode::AddComma(CommaNode* comma)
{
    args.AddComma(comma);
}

void ConstructExprNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

Node* ConstructExprNode::Clone() const 
{
    ConstructExprNode* clone = new ConstructExprNode(static_cast<ConstructKeywordNode*>(constructNode->Clone()), static_cast<LAngleNode*>(langle->Clone()),
        Operand()->Clone(), static_cast<RAngleNode*>(rangle->Clone()), static_cast<LParenNode*>(lparen->Clone()));
    clone->args.CloneFrom(args);
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void ConstructExprNode::Make()
{
    args.Make();
    AddChildNode(&args);
    UnaryNode::Make();
}

void ConstructExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

NewExprNode::NewExprNode(NewKeywordNode* newNode_, Node* typeExpr_) : UnaryNode(typeExpr_), newNode(newNode_), args(soul::ast::Span(), CompletionContext::none)
{
    AddChildNode(newNode.get());
}

void NewExprNode::AddNode(Node* arg)
{
    args.AddNode(arg);
}

void NewExprNode::AddComma(CommaNode* comma)
{
    args.AddComma(comma);
}

void NewExprNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void NewExprNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

Node* NewExprNode::Clone() const
{
    NewExprNode* clone = new NewExprNode(static_cast<NewKeywordNode*>(newNode->Clone()), Operand()->Clone());
    clone->args.CloneFrom(args);
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void NewExprNode::Make()
{
    args.Make();
    AddChildNode(&args);
    UnaryNode::Make();
}

void NewExprNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast

