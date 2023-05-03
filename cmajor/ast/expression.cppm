// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.expression;

import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

class IdentifierNode;

class DotNode : public UnaryNode
{
public:
    DotNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DotNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_, IdentifierNode* memberId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const IdentifierNode* MemberId() const { return memberId.get(); }
    IdentifierNode* MemberId() { return memberId.get(); }
private:
    std::unique_ptr<IdentifierNode> memberId;
};

class ArrowNode : public UnaryNode
{
public:
    ArrowNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ArrowNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_, IdentifierNode* memberId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    std::string ToString() const override;
    const IdentifierNode* MemberId() const { return memberId.get(); }
    IdentifierNode* MemberId() { return memberId.get(); }
private:
    std::unique_ptr<IdentifierNode> memberId;
};

class EquivalenceNode : public BinaryNode
{
public:
    EquivalenceNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    EquivalenceNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class ImplicationNode : public BinaryNode
{
public:
    ImplicationNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ImplicationNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class DisjunctionNode : public BinaryNode
{
public:
    DisjunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DisjunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class ConjunctionNode : public BinaryNode
{
public:
    ConjunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConjunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class BitOrNode : public BinaryNode
{
public:
    BitOrNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    BitOrNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class BitXorNode : public BinaryNode
{
public:
    BitXorNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    BitXorNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class BitAndNode : public BinaryNode
{
public:
    BitAndNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    BitAndNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class EqualNode : public BinaryNode
{
public:
    EqualNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    EqualNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class NotEqualNode : public BinaryNode
{
public:
    NotEqualNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    NotEqualNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class LessNode : public BinaryNode
{
public:
    LessNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    LessNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class GreaterNode : public BinaryNode
{
public:
    GreaterNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    GreaterNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class LessOrEqualNode : public BinaryNode
{
public:
    LessOrEqualNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    LessOrEqualNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class GreaterOrEqualNode : public BinaryNode
{
public:
    GreaterOrEqualNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    GreaterOrEqualNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class ShiftLeftNode : public BinaryNode
{
public:
    ShiftLeftNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ShiftLeftNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class ShiftRightNode : public BinaryNode
{
public:
    ShiftRightNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ShiftRightNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class AddNode : public BinaryNode
{
public:
    AddNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    AddNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class SubNode : public BinaryNode
{
public:
    SubNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    SubNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class MulNode : public BinaryNode
{
public:
    MulNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    MulNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class DivNode : public BinaryNode
{
public:
    DivNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DivNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class RemNode : public BinaryNode
{
public:
    RemNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    RemNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* left_, Node* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class NotNode : public UnaryNode
{
public:
    NotNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    NotNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class UnaryPlusNode : public UnaryNode
{
public:
    UnaryPlusNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    UnaryPlusNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class UnaryMinusNode : public UnaryNode
{
public:
    UnaryMinusNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    UnaryMinusNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class PrefixIncrementNode : public UnaryNode
{
public:
    PrefixIncrementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    PrefixIncrementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class PrefixDecrementNode : public UnaryNode
{
public:
    PrefixDecrementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    PrefixDecrementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class ComplementNode : public UnaryNode
{
public:
    ComplementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ComplementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class DerefNode : public UnaryNode
{
public:
    DerefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DerefNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class AddrOfNode : public UnaryNode
{
public:
    AddrOfNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    AddrOfNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class IsNode : public Node
{
public:
    IsNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    IsNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expr_, Node* targetTypeExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Expr() const { return expr.get(); }
    Node* Expr() { return expr.get(); }
    const Node* TargetTypeExpr() const { return targetTypeExpr.get(); }
    Node* TargetTypeExpr() { return targetTypeExpr.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> expr;
    std::unique_ptr<Node> targetTypeExpr;
};

class AsNode : public Node
{
public:
    AsNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    AsNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expr_, Node* targetTypeExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Expr() const { return expr.get(); }
    Node* Expr() { return expr.get(); }
    const Node* TargetTypeExpr() const { return targetTypeExpr.get(); }
    Node* TargetTypeExpr() { return targetTypeExpr.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> expr;
    std::unique_ptr<Node> targetTypeExpr;
};

class IndexingNode : public Node
{
public:
    IndexingNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    IndexingNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_, Node* index_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Subject() const { return subject.get(); }
    Node* Subject() { return subject.get(); }
    const Node* Index() const { return index.get(); }
    Node* Index() { return index.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<Node> index;
};

class InvokeNode : public Node
{
public:
    InvokeNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    InvokeNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddArgument(Node* argument) override;
    const Node* Subject() const { return subject.get(); }
    Node* Subject() { return subject.get(); }
    const NodeList<Node>& Arguments() const { return arguments; }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> subject;
    NodeList<Node> arguments;
};

class PostfixIncrementNode : public UnaryNode
{
public:
    PostfixIncrementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    PostfixIncrementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class PostfixDecrementNode : public UnaryNode
{
public:
    PostfixDecrementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    PostfixDecrementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* subject_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class SizeOfNode : public Node
{
public:
    SizeOfNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    SizeOfNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expression_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Expression() const { return expression.get(); }
    Node* Expression() { return expression.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> expression;
};

class TypeNameNode : public Node
{
public:
    TypeNameNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    TypeNameNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expression_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Expression() const { return expression.get(); }
    Node* Expression() { return expression.get(); }
    std::string ToString() const override;
    bool Static() const { return static_; }
    void SetStatic() { static_ = true; }
private:
    std::unique_ptr<Node> expression;
    bool static_;
};

class TypeIdNode : public Node
{
public:
    TypeIdNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    TypeIdNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expression_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Expression() const { return expression.get(); }
    Node* Expression() { return expression.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> expression;
};

class CastNode : public Node
{
public:
    CastNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    CastNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* targetTypeExpr_, Node* sourceExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* TargetTypeExpr() const { return targetTypeExpr.get(); }
    Node* TargetTypeExpr() { return targetTypeExpr.get(); }
    const Node* SourceExpr() const { return sourceExpr.get(); }
    Node* SourceExpr() { return sourceExpr.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> targetTypeExpr;
    std::unique_ptr<Node> sourceExpr;
};

class ConstructNode : public Node
{
public:
    ConstructNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConstructNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* typeExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddArgument(Node* argument) override;
    const Node* TypeExpr() const { return typeExpr.get(); }
    Node* TypeExpr() { return typeExpr.get(); }
    const NodeList<Node>& Arguments() const { return arguments; }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> typeExpr;
    NodeList<Node> arguments;
};

class NewNode : public Node
{
public:
    NewNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    NewNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* typeExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddArgument(Node* argument) override;
    const Node* TypeExpr() const { return typeExpr.get(); }
    Node* TypeExpr() { return typeExpr.get(); }
    const NodeList<Node>& Arguments() const { return arguments; }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> typeExpr;
    NodeList<Node> arguments;
};

class ThisNode : public Node
{
public:
    ThisNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class BaseNode : public Node
{
public:
    BaseNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class ParenthesizedExpressionNode : public UnaryNode
{
public:
    ParenthesizedExpressionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ParenthesizedExpressionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* child_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

} // namespace cmajor::ast
