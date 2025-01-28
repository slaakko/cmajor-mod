// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.enumeration;

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

class EnumConstantNode : public SyntaxNode
{
public:
    EnumConstantNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    void SetAssign(AssignNode* assign_);
    AssignNode* Assign() const { return assign.get(); }
    void SetValue(Node* value_);
    Node* Value() const { return value.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<AssignNode> assign;
    std::unique_ptr<Node> value;
};

class EnumConstantListNode : public ListNode
{
public:
    EnumConstantListNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void AddEnumConstant(EnumConstantNode* enumConstant);
    const std::vector<EnumConstantNode*>& EnumConstants() const { return enumConstants; }
    int Count() const { return enumConstants.size(); }
    void CloneFrom(const EnumConstantListNode& that);
private:
    std::vector<EnumConstantNode*> enumConstants;
};

class EnumTypeNode : public SyntaxNode
{
public:
    EnumTypeNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetEnumKeyword(EnumKeywordNode* enumKeyword_);
    EnumKeywordNode* EnumKeyword() const { return enumKeyword.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    void SetColon(ColonNode* colon_);
    ColonNode* Colon() const { return colon.get(); }
    void SetUnderlyingType(Node* underlyingType_);
    Node* UnderlyingType() const;
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddEnumConstant(EnumConstantNode* enumConstant);
    void AddComma(CommaNode* comma);
    EnumConstantListNode* EnumConstantList() { return &enumConstantList; }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<EnumKeywordNode> enumKeyword;
    std::unique_ptr<IdentifierNode> id;
    std::unique_ptr<ColonNode> colon;
    std::unique_ptr<Node> underlyingType;
    std::unique_ptr<LBraceNode> lbrace;
    EnumConstantListNode enumConstantList;
    std::unique_ptr<RBraceNode> rbrace;
};

Node* NextEnumConstantValue(EnumTypeNode* enumType);

} // namespace cmajor::fault::tolerant::ast
