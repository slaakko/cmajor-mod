// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.alias;

import std.core;
import cmajor.ast.node;
import cmajor.ast.specifier;

export namespace cmajor::ast {

class IdentifierNode;

class AliasNode : public Node
{
public:
    AliasNode(const soul::ast::Span& span_);
    AliasNode(const soul::ast::Span& span_, Specifiers specifiers_, IdentifierNode* id_, Node* typeExpr_);
    Specifiers GetSpecifiers() const override { return specifiers; }
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    IdentifierNode* Id() const;
    Node* TypeExpr() const;
private:
    Specifiers specifiers;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<Node> typeExpr;
};

class TypedefNode : public Node
{
public:
    TypedefNode(const soul::ast::Span& span_);
    TypedefNode(const soul::ast::Span& span_, Specifiers specifiers_, Node* typeExpr_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    Specifiers GetSpecifiers() const override { return specifiers; }
    Node* TypeExpr() const { return typeExpr.get(); }
    IdentifierNode* Id() const { return id.get(); }
private:
    Specifiers specifiers;
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> id;
};

} // namespace cmajor::ast
