// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.templates;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.identifier;
import cmajor.fault.tolerant.ast.punctuation;
import cmajor.fault.tolerant.ast.type_expr;
import cmajor.fault.tolerant.ast.keyword;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class TemplateIdNode : public SyntaxNode
{
public:
    TemplateIdNode(const soul::ast::Span& span_, CompletionContext completionContext_, QualifiedIdNode* primary_);
    QualifiedIdNode* Primary() const { return primary.get(); }
    void SetLAngle(LAngleNode* langle_);
    LAngleNode* LAngle() const { return langle.get(); }
    void AddTemplateArgument(TypeExprNode* templateArgument);
    void AddComma(CommaNode* comma) override;
    ListNode* TemplateArgumentList() { return &templateArgumentList; }
    void SetRAngle(RAngleNode* rangle_);
    RAngleNode* RAngle() const { return rangle.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<QualifiedIdNode> primary;
    std::unique_ptr<LAngleNode> langle;
    ListNode templateArgumentList;
    std::unique_ptr<RAngleNode> rangle;
};

class TemplateParameterNode : public SyntaxNode
{
public:
    TemplateParameterNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetIdentifier(IdentifierNode* identifier_);
    IdentifierNode* Identifier() const { return identifier.get(); }
    void SetAssign(AssignNode* assign_);
    AssignNode* Assign() const { return assign.get(); }
    void SetDefaultValue(Node* defaultValue_);
    Node* DefaultValue() const { return defaultValue.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> identifier;
    std::unique_ptr<AssignNode> assign;
    std::unique_ptr<Node> defaultValue;
};

class TemplateParameterListNode : public ListNode
{
public:
    TemplateParameterListNode();
    void SetLAngle(LAngleNode* langle_);
    LAngleNode* LAngle() const { return langle.get(); }
    void AddTemplateParameter(TemplateParameterNode* templateParameter);
    void SetRAngle(RAngleNode* rangle_);
    RAngleNode* RAngle() const { return rangle.get(); }
    const std::vector<TemplateParameterNode*>& TemplateParameters() const { return templateParameters; }
    void CloneFrom(const TemplateParameterListNode& that);
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LAngleNode> langle;
    std::unique_ptr<RAngleNode> rangle;
    std::vector<TemplateParameterNode*> templateParameters;
};

class FullInstantiationRequest : public SyntaxNode
{
public:
    FullInstantiationRequest(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetNewKeyword(NewKeywordNode* newKeyword_);
    NewKeywordNode* NewKeyword() const { return newKeyword.get(); }
    void SetClassKeyword(ClassKeywordNode* classKeyword_);
    ClassKeywordNode* ClassKeyword() const { return classKeyword.get(); }
    void SetTemplateId(TemplateIdNode* templateId_);
    TemplateIdNode* TemplateId() const { return templateId.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<NewKeywordNode> newKeyword;
    std::unique_ptr<ClassKeywordNode> classKeyword;
    std::unique_ptr<TemplateIdNode> templateId;
    std::unique_ptr<SemicolonNode> semicolon;
};

} // namespace cmajor::fault::tolerant::ast
