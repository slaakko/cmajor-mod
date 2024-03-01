// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.expression;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.punctuation;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.keyword;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class EquivalenceNode : public TokenNode
{
public:
    EquivalenceNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ImplicationNode : public TokenNode
{
public:
    ImplicationNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class DisjunctionNode : public TokenNode
{
public:
    DisjunctionNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class BitOrNode : public TokenNode
{
public:
    BitOrNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class BitXorNode : public TokenNode
{
public:
    BitXorNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class EqualNode : public TokenNode
{
public:
    EqualNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class NotEqualNode : public TokenNode
{
public:
    NotEqualNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class LessEqualNode : public TokenNode
{
public:
    LessEqualNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class GreaterEqualNode : public TokenNode
{
public:
    GreaterEqualNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ShiftLeftNode : public TokenNode
{
public:
    ShiftLeftNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ShiftRightNode : public TokenNode
{
public:
    ShiftRightNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class PlusNode : public TokenNode
{
public:
    PlusNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class MinusNode : public TokenNode
{
public:
    MinusNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class DivNode : public TokenNode
{
public:
    DivNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class RemNode : public TokenNode
{
public:
    RemNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class IncrementNode : public TokenNode
{
public:
    IncrementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class DecrementNode : public TokenNode
{
public:
    DecrementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class NotNode : public TokenNode
{
public:
    NotNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class ComplementNode : public TokenNode
{
public:
    ComplementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::u16string str;
};

class BinaryExprNode : public BinaryNode
{
public:
    BinaryExprNode(Node* left_, Node* right_, Node* op_);
    Node* Operator() const { return op.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> op;
};

class PrefixExprNode : public UnaryNode
{
public:
    PrefixExprNode(Node* operand_, Node* op_);
    Node* Operator() const { return op.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> op;
};

class PostfixExprNode : public UnaryNode
{
public:
    PostfixExprNode(Node* operand_, Node* op_);
    Node* Operator() const { return op.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> op;
};

class SubscriptExprNode : public UnaryNode
{
public:
    SubscriptExprNode(Node* subject_, LBracketNode* lbracket_, Node* index_, RBracketNode* rbracket_);
    LBracketNode* LBracket() const { return lbracket.get(); }
    Node* Index() const { return index.get(); }
    RBracketNode* RBracket() const { return rbracket.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LBracketNode> lbracket;
    std::unique_ptr<Node> index;
    std::unique_ptr<RBracketNode> rbracket;
};

class InvokeExprNode : public UnaryNode
{
public:
    InvokeExprNode(Node* subject_, LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void AddNode(Node* node) override;
    void AddComma(CommaNode* comma) override;
    ListNode* ArgumentList() { return &argumentList; }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LParenNode> lparen;
    ListNode argumentList;
    std::unique_ptr<RParenNode> rparen;
};

class IsExprNode : public BinaryNode
{
public:
    IsExprNode(Node* subject_, Node* type_, Node* op_);
    Node* Operator() const { return op.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> op;
};

class AsExprNode : public BinaryNode
{
public:
    AsExprNode(Node* subject_, Node* type_, Node* op_);
    Node* Operator() const { return op.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> op;
};

class DotMemberNode : public BinaryNode
{
public:
    DotMemberNode(Node* subject_, DotNode* dot_, IdentifierNode* id_);
    DotNode* Dot() const { return dot.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<DotNode> dot;
};

class ArrowMemberNode : public BinaryNode
{
public:
    ArrowMemberNode(Node* subject_, ArrowNode* arrow_, IdentifierNode* id_);
    ArrowNode* Arrow() const { return arrow.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ArrowNode> arrow;
};

class ParenthesizedExprNode : public UnaryNode
{
public:
    ParenthesizedExprNode(Node* subject_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetLParen(LParenNode* lparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetRParen(RParenNode* rparen_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
};

class SizeOfExprNode : public UnaryNode
{
public:
    SizeOfExprNode(SizeOfKeywordNode* sizeOfNode_, Node* subject_);
    SizeOfKeywordNode* SizeOf() const { return sizeOfNode.get(); }
    LParenNode* LParen() const { return lparen.get(); }
    void SetLParen(LParenNode* lparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetRParen(RParenNode* rparen_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SizeOfKeywordNode> sizeOfNode;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
};

class TypeNameExprNode : public UnaryNode
{
public:
    TypeNameExprNode(TypeNameKeywordNode* typeNameNode_, Node* subject_);
    TypeNameKeywordNode* TypeName() const { return typeNameNode.get(); }
    LParenNode* LParen() const { return lparen.get(); }
    void SetLParen(LParenNode* lparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetRParen(RParenNode* rparen_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<TypeNameKeywordNode> typeNameNode;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
};

class TypeIdExprNode : public UnaryNode
{
public:
    TypeIdExprNode(TypeIdKeywordNode* typeIdNode_, Node* subject_);
    TypeIdKeywordNode* TypeId() const { return typeIdNode.get(); }
    LParenNode* LParen() const { return lparen.get(); }
    void SetLParen(LParenNode* lparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetRParen(RParenNode* rparen_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<TypeIdKeywordNode> typeIdNode;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
};

class CastExprNode : public BinaryNode
{
public:
    CastExprNode(CastKeywordNode* castNode_, LAngleNode* langle_, Node* typeExpr_, RAngleNode* rangle_, LParenNode* lparen_, Node* expr_, RParenNode* rparen_);
    CastKeywordNode* Cast() const { return castNode.get(); }
    LAngleNode* LAngle() const { return langle.get(); }
    RAngleNode* RAngle() const { return rangle.get(); }
    LParenNode* LParen() const { return lparen.get(); }
    RParenNode* RParen() const { return rparen.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<CastKeywordNode> castNode;
    std::unique_ptr<LAngleNode> langle;
    std::unique_ptr<RAngleNode> rangle;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
};

class ConstructExprNode : public UnaryNode
{
public:
    ConstructExprNode(ConstructKeywordNode* constructNode_, LAngleNode* langle_, Node* typeExpr_, RAngleNode* rangle_, LParenNode* lparen_);
    ConstructKeywordNode* Construct() const { return constructNode.get(); }
    LAngleNode* LAngle() const { return langle.get(); }
    RAngleNode* RAngle() const { return rangle.get(); }
    void AddNode(Node* arg) override;
    void AddComma(CommaNode* comma) override;
    const ListNode& Args() const { return args; }
    ListNode& Args() { return args; }
    LParenNode* LParen() const { return lparen.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ConstructKeywordNode> constructNode;
    std::unique_ptr<LAngleNode> langle;
    std::unique_ptr<RAngleNode> rangle;
    std::unique_ptr<LParenNode> lparen;
    ListNode args;
    std::unique_ptr<RParenNode> rparen;
};

class NewExprNode : public UnaryNode
{
public:
    NewExprNode(NewKeywordNode* newNode_, Node* typeExpr_);
    NewKeywordNode* NewKeyword() const { return newNode.get(); }
    void AddNode(Node* arg) override;
    void AddComma(CommaNode* comma) override;
    const ListNode& Args() const { return args; }
    ListNode& Args() { return args; }
    LParenNode* LParen() const { return lparen.get(); }
    void SetLParen(LParenNode* lparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetRParen(RParenNode* rparen_);
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<NewKeywordNode> newNode;
    std::unique_ptr<LParenNode> lparen;
    ListNode args;
    std::unique_ptr<RParenNode> rparen;
};

} // namespace cmajor::fault::tolerant::ast
