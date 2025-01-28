// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.function;

import cmajor.fault.tolerant.ast.node;
import cmajor.fault.tolerant.ast.node.list;
import cmajor.fault.tolerant.ast.keyword;
import cmajor.fault.tolerant.ast.attribute;
import cmajor.fault.tolerant.ast.specifier;
import cmajor.fault.tolerant.ast.templates;
import cmajor.fault.tolerant.ast.parameter;
import cmajor.fault.tolerant.ast.concepts;
import cmajor.fault.tolerant.ast.statement;
import cmajor.fault.tolerant.ast.punctuation;
import std.core;

export namespace cmajor::fault::tolerant::ast {

class FunctionNode : public SyntaxNode
{
public:
    FunctionNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetAttributes(AttributesNode* attributes_);
    AttributesNode* Attributes() const { return attributes.get(); }
    void SetSpecifiers(SpecifiersNode* specifiers_);
    SpecifiersNode* Specifiers() const { return specifiers.get(); }
    void SetReturnType(Node* returnType_);
    Node* ReturnType() const { return returnType.get(); }
    void SetFunctionGroupId(Node* functionGroupId_);
    Node* FunctionGroupId() const { return functionGroupId.get(); }
    TemplateParameterListNode* TemplateParameterList() { return &templateParameterList; }
    ParameterListNode* ParameterList() { return &parameterList; }
    void SetConstraint(WhereConstraintNode* constraint_);
    WhereConstraintNode* Constraint() const { return constraint.get(); }
    void SetBody(CompoundStatementNode* compoundStatement_);
    CompoundStatementNode* Body() const { return body.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<AttributesNode> attributes;
    std::unique_ptr<SpecifiersNode> specifiers;
    std::unique_ptr<Node> returnType;
    std::unique_ptr<Node> functionGroupId;
    TemplateParameterListNode templateParameterList;
    ParameterListNode parameterList;
    std::unique_ptr<WhereConstraintNode> constraint;
    std::unique_ptr<CompoundStatementNode> body;
    std::unique_ptr<SemicolonNode> semicolon;
};

class OperatorFunctionGroupIdNode : public SyntaxNode
{
public:
    OperatorFunctionGroupIdNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetOperatorKeyword(OperatorKeywordNode* operatorKeyword_);
    OperatorKeywordNode* OperatorKeyword() const { return operatorKeyword.get(); }
    void AddOperatorNode(Node* operatorNode);
    NodeList<Node>* OperatorNodes() { return &operatorNodes; }
    const std::u16string& Str() const override { return str; }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<OperatorKeywordNode> operatorKeyword;
    NodeList<Node> operatorNodes;
    std::u16string str;
};

} // namespace cmajor::fault::tolerant::ast
