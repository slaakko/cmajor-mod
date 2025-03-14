// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.function;

import std.core;
import cmajor.ast.node;
import cmajor.ast.node.list;
import cmajor.ast.attribute;
import cmajor.ast.statement;
import cmajor.ast.templates;
import cmajor.ast.identifier;
import cmajor.ast.concepts;
import cmajor.ast.parameter;

export namespace cmajor::ast {

class FunctionNode : public Node
{
public:
    FunctionNode(const soul::ast::Span& span_);
    FunctionNode(NodeType nodeType_, const soul::ast::Span& span_);
    FunctionNode(const soul::ast::Span& span_, Specifiers specifiers_, Node* returnTypeExpr_, const std::u32string& groupId_, AttributesNode* attributes_);
    FunctionNode(NodeType nodeType_, const soul::ast::Span& span_, Specifiers specifiers_, Node* returnTypeExpr_, const std::u32string& groupId_, AttributesNode* attributes_);
    FunctionNode(const FunctionNode&) = delete;
    FunctionNode& operator=(const FunctionNode&) = delete;
    Node* Clone(CloneContext& cloneContext) const override;
    bool IsFunctionNode() const override { return true; }
    void CloneContent(FunctionNode* clone, CloneContext& cloneContext) const;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddTemplateParameter(TemplateParameterNode* templateParameter) override;
    void AddParameter(ParameterNode* parameter) override;
    void SwitchToBody();
    Specifiers GetSpecifiers() const override { return specifiers; }
    const Node* ReturnTypeExpr() const { return returnTypeExpr.get(); }
    Node* ReturnTypeExpr() { return returnTypeExpr.get(); }
    const std::u32string& GroupId() const { return groupId; }
    const NodeList<TemplateParameterNode>& TemplateParameters() const { return templateParameters; }
    const NodeList<ParameterNode>& Parameters() const { return parameters; }
    void SetReturnTypeExpr(Node* returnTypeExpr_);
    const WhereConstraintNode* WhereConstraint() const { return whereConstraint.get(); }
    WhereConstraintNode* WhereConstraint() { return whereConstraint.get(); }
    void SetSpecifiers(Specifiers specifiers_);
    void SetConstraint(WhereConstraintNode* whereConstraint_);
    const CompoundStatementNode* Body() const { return body.get(); }
    CompoundStatementNode* Body() { return body.get(); }
    void SetBody(CompoundStatementNode* body_);
    const CompoundStatementNode* BodySource() const { return bodySource.get(); }
    void SetBodySource(CompoundStatementNode* bodySource_);
    bool IsProgramMain() const { return programMain; }
    void SetProgramMain() { programMain = true; }
    AttributesNode* GetAttributes() const { return attributes.get(); }
    const soul::ast::Span& SpecifierSpan() const { return specifierSpan; }
    void SetSpecifierSpan(const soul::ast::Span& specifierSpan_) { specifierSpan = specifierSpan_; }
    const soul::ast::Span& GroupIdSpan() const { return groupIdSpan; }
    void SetGroupIdSpan(const soul::ast::Span& groupIdSpan_) { groupIdSpan = groupIdSpan_; }
private:
    Specifiers specifiers;
    std::unique_ptr<Node> returnTypeExpr;
    std::u32string groupId;
    NodeList<TemplateParameterNode> templateParameters;
    NodeList<ParameterNode> parameters;
    std::unique_ptr<WhereConstraintNode> whereConstraint;
    std::unique_ptr<CompoundStatementNode> body;
    std::unique_ptr<CompoundStatementNode> bodySource;
    std::unique_ptr<AttributesNode> attributes;
    bool programMain;
    soul::ast::Span specifierSpan;
    soul::ast::Span groupIdSpan;
};

class FunctionPtrNode : public Node
{
public:
    FunctionPtrNode(const soul::ast::Span& span_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void SetBoundExpression(void* boundExpression_) { boundExpression = boundExpression_; }
    void* GetBoundExpression() const { return boundExpression; }
private:
    void* boundExpression;
};
} // namespace cmajor::ast
