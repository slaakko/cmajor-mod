// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.delegate;

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

class DelegateNode : public SyntaxNode
{
public:
    DelegateNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetDelegateKeyword(DelegateKeywordNode* delegateKeyword_);
    DelegateKeywordNode* DelegateKeyword() const { return delegateKeyword.get(); }
    void SetReturnType(Node* returnType_);
    Node* ReturnType() const { return returnType.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    ParameterListNode* ParameterList() { return &parameterList; }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<DelegateKeywordNode> delegateKeyword;
    std::unique_ptr<Node> returnType;
    std::unique_ptr<IdentifierNode> id;
    ParameterListNode parameterList;
    std::unique_ptr<SemicolonNode> semicolon;
};

class ClassDelegateNode : public SyntaxNode
{
public:
    ClassDelegateNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetClassKeyword(ClassKeywordNode* classKeyword_);
    ClassKeywordNode* ClassKeyword() const { return classKeyword.get(); }
    void SetDelegateKeyword(DelegateKeywordNode* delegateKeyword_);
    DelegateKeywordNode* DelegateKeyword() const { return delegateKeyword.get(); }
    void SetReturnType(Node* returnType_);
    Node* ReturnType() const { return returnType.get(); }
    void SetId(IdentifierNode* id_);
    IdentifierNode* Id() const { return id.get(); }
    ParameterListNode* ParameterList() { return &parameterList; }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<ClassKeywordNode> classKeyword;
    std::unique_ptr<DelegateKeywordNode> delegateKeyword;
    std::unique_ptr<Node> returnType;
    std::unique_ptr<IdentifierNode> id;
    ParameterListNode parameterList;
    std::unique_ptr<SemicolonNode> semicolon;
};

} // namespace cmajor::fault::tolerant::ast
