// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.type_expr;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.punctuation;
import cmajor.fault.tolerant.ast.identifier;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class PointerNode : public SyntaxNode
{
public:
    PointerNode(Node* subject_, StarNode* star_);
    Node* Subject() const { return subject.get(); }
    StarNode* Star() const { return star.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<StarNode> star;
};

class LValueRefNode : public SyntaxNode
{
public:
    LValueRefNode(Node* subject_, AmpNode* amp_);
    Node* Subject() const { return subject.get(); }
    AmpNode* Amp() const { return amp.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<AmpNode> amp;
};

class RValueRefNode : public SyntaxNode
{
public:
    RValueRefNode(Node* subject_, AmpAmpNode* ampamp_);
    Node* Subject() const { return subject.get(); }
    AmpAmpNode* AmpAmp() const { return ampamp.get(); }
    Node* Clone() const override;
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<AmpAmpNode> ampamp;
    void Accept(Visitor& visitor) override;
};

class ArrayNode : public SyntaxNode
{
public:
    ArrayNode(Node* subject_, LBracketNode* lbracket_, Node* size_, RBracketNode* rbracket_);
    Node* Subject() const { return subject.get(); }
    LBracketNode* LBracket() const { return lbracket.get(); }
    Node* Size() const { return size.get(); }
    RBracketNode* RBracket() const { return rbracket.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> subject;
    std::unique_ptr<LBracketNode> lbracket;
    std::unique_ptr<Node> size;
    std::unique_ptr<RBracketNode> rbracket;
};

class TypeExprNode : public SyntaxNode
{
public:
    TypeExprNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void AddNode(Node* node) override;
    std::vector<std::unique_ptr<Node>>& Nodes() { return nodes; }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::vector<std::unique_ptr<Node>> nodes;
};

} // namespace cmajor::fault::tolerant::ast
