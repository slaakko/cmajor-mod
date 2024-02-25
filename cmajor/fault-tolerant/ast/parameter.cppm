// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.parameter;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.punctuation;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class ParameterNode : public SyntaxNode
{
public:
    ParameterNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetTypeExpr(Node* typeExpr_);
    Node* TypeExpr() const { return typeExpr.get(); }
    void SetName(IdentifierNode* name_);
    IdentifierNode* Name() const { return name.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IdentifierNode> name;
};

class ParameterListNode : public ListNode
{
public:
    ParameterListNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void AddParameter(ParameterNode* parameter);
    ParameterNode* GetParameter(int index) const;
    int ParameterCount() const { return parameters.size(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void CloneFrom(const ParameterListNode& that);
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
    std::vector<ParameterNode*> parameters;
};

} // namespace cmajor::fault::tolerant::ast
