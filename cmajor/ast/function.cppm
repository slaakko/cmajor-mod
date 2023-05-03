// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.function;

import std.core;
import cmajor.ast.node;
import cmajor.ast.node.list;

export namespace cmajor::ast {

class AttributesNode;
class WhereConstraintNode;
class CompoundStatementNode;
class TemplateParameterNode;

class FunctionNode : public Node
{
public:
    FunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    FunctionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    FunctionNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* returnTypeExpr_, const std::u32string& groupId_, AttributesNode* attributes_);
    FunctionNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, Node* returnTypeExpr_, const std::u32string& groupId_, AttributesNode* attributes_);
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
};

class FunctionPtrNode : public Node
{
public:
    FunctionPtrNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void SetBoundExpression(void* boundExpression_) { boundExpression = boundExpression_; }
    void* GetBoundExpression() const { return boundExpression; }
private:
    void* boundExpression;
};
} // namespace cmajor::ast
