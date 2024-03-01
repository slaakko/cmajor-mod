// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.node;

import cmajor.fault.tolerant.ast.completion.context;
import std.core;
import soul.ast.span;

export namespace cmajor::fault::tolerant::ast {

enum class NodeKind : int
{
    keywordNode, tokenNode, syntaxNode, nodeList
};

class Visitor;
class CommaNode;

class Node
{
public:
    Node(NodeKind nodeKind_, const soul::ast::Span& span_, CompletionContext completionContext_);
    virtual ~Node();
    NodeKind GetNodeKind() const { return nodeKind; }
    bool IsKeyword() const { return nodeKind == NodeKind::keywordNode; }
    bool IsTokenNode() const { return nodeKind == NodeKind::tokenNode; }
    bool IsSyntaxNode() const { return nodeKind == NodeKind::syntaxNode; }
    int Index() const { return index; }
    void SetIndex(int index_) { index = index_; }
    const soul::ast::Span& GetSpan() const { return span; }
    CompletionContext GetCompletionContext() const { return completionContext; }
    Node* Parent() const { return parent; }
    void SetParent(Node* parent_) { parent = parent_; }
    const std::vector<Node*>& ChildNodes() const { return childNodes; }
    virtual void AddChildNode(Node* childNode);
    Node* GetChildNode(int index) const;
    virtual void Make();
    Node* FindNode(int pos) const;
    virtual void AddNode(Node* node);
    virtual void AddComma(CommaNode* comma);
    virtual const std::u16string& Str() const;
    virtual bool IsUnsignedTypeNode() const { return false; }
    virtual bool IsComma() const { return false; }
    virtual bool IsDot() const { return false; }
    virtual Node* Clone() const { return nullptr; }
    virtual void Accept(Visitor& visitor);
private:
    NodeKind nodeKind;
    int index;
    soul::ast::Span span;
    CompletionContext completionContext;
    Node* parent;
    std::vector<Node*> childNodes;
};

class KeywordNode : public Node
{
public:
    KeywordNode(const soul::ast::Span& span_, CompletionContext completionContext_);
};

class TokenNode : public Node
{
public:
    TokenNode(const soul::ast::Span& span_, CompletionContext completionContext_);
};

class SyntaxNode : public Node
{
public:
    SyntaxNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    const std::vector<SyntaxNode*>& ChildSyntaxNodes() const { return childSyntaxNodes; }
    void AddChildNode(Node* childNode) override;
private:
    std::vector<SyntaxNode*> childSyntaxNodes;
};

class UnaryNode : public SyntaxNode
{
public:
    UnaryNode(Node* operand_);
    Node* Operand() const { return operand.get(); }
private:
    std::unique_ptr<Node> operand;
};

class BinaryNode : public SyntaxNode
{
public:
    BinaryNode(Node* left_, Node* right_);
    Node* Left() const { return left.get(); }
    Node* Right() const { return right.get(); }
private:
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

class ListNode : public SyntaxNode
{
public:
    ListNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void AddNode(Node* node) override;
    void AddComma(CommaNode* comma) override;
    const std::vector<std::unique_ptr<Node>>& Nodes() const { return nodes; }
    const std::vector<Node*>& Items() const { return items; }
    int ItemCount() const { return items.size(); }
    Node* GetItem(int index) const;
    void CloneFrom(const ListNode& that);
    void Accept(Visitor& visitor) override;
private:
    std::vector<std::unique_ptr<Node>> nodes;
    std::vector<Node*> items;
};

} // namespace cmajor::fault::tolerant::ast

