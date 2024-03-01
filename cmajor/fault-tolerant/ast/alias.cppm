// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.alias;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.specifier;
import cmajor.fault.tolerant.ast.keyword;
import cmajor.fault.tolerant.ast.punctuation;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.type_expr;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class UsingAliasNode : public SyntaxNode
{
public:
    UsingAliasNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetSpecifiers(SpecifiersNode* specifiersNode_);
    SpecifiersNode* GetSpecifiers() const { return specifiersNode.get(); }
    void SetUsingKeyword(UsingKeywordNode* usingKeyword_);
    UsingKeywordNode* UsingKeyword() const { return usingKeyword.get(); }
    void SetIdentifier(IdentifierNode* identifierNode_);
    IdentifierNode* Identifier() const { return identifierNode.get(); }
    void SetAssign(AssignNode* assignNode_);
    AssignNode* Assign() const { return assignNode.get(); }
    void SetTypeExpr(TypeExprNode* typeExprNode_);
    TypeExprNode* TypeExpr() const { return typeExprNode.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SpecifiersNode> specifiersNode;
    std::unique_ptr<UsingKeywordNode> usingKeyword;
    std::unique_ptr<IdentifierNode> identifierNode;
    std::unique_ptr<AssignNode> assignNode;
    std::unique_ptr<TypeExprNode> typeExprNode;
    std::unique_ptr<SemicolonNode> semicolon;
};

class TypedefAliasNode : public SyntaxNode
{
public:
    TypedefAliasNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetSpecifiersNode(SpecifiersNode* specifiersNode_);
    SpecifiersNode* GetSpecifiers() const { return specifiersNode.get(); }
    void SetTypedefKeyword(TypedefKeywordNode* typedefNode_);
    TypedefKeywordNode* Typedef() const { return typedefNode.get(); }
    void SetTypeExprNode(TypeExprNode* typeExprNode_);
    TypeExprNode* TypeExpr() const { return typeExprNode.get(); }
    void SetIdentifierNode(IdentifierNode* identifierNode);
    IdentifierNode* Identifier() const { return identifierNode.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SpecifiersNode> specifiersNode;
    std::unique_ptr<TypedefKeywordNode> typedefNode;
    std::unique_ptr<TypeExprNode> typeExprNode;
    std::unique_ptr<IdentifierNode> identifierNode;
    std::unique_ptr<SemicolonNode> semicolon;
};

} // namespace cmajor::fault::tolerant::ast
