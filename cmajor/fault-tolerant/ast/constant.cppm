// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.constant;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.node.list;
import cmajor.fault.tolerant.ast.punctuation;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.keyword;
import cmajor.fault.tolerant.ast.specifier;
import cmajor.fault.tolerant.ast.parameter;
import cmajor.fault.tolerant.ast.expression;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class ConstantNode : public SyntaxNode
{
public:
    ConstantNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetConstKeyword(ConstKeywordNode* constKeyword_);
    ConstKeywordNode* ConstKeyword() const { return constKeyword.get(); }
    void SetType(Node* type_);
    Node* Type() const { return type.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    void SetAssign(AssignNode* assign_);
    AssignNode* Assign() const { return assign.get(); }
    void SetExpr(Node* expr_);
    Node* Expr() const { return expr.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<ConstKeywordNode> constKeyword;
    std::unique_ptr<Node> type;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<AssignNode> assign;
    std::unique_ptr<Node> expr;
    std::unique_ptr<SemicolonNode> semicolon;
};

} // namespace cmajor::fault::tolerant::ast
