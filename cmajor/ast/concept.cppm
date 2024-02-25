// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.concepts;

import cmajor.ast.node;
import cmajor.ast.identifier;
import cmajor.ast.node.list;

export namespace cmajor::ast {

class ParameterNode;

class ConstraintNode : public Node
{
public:
    ConstraintNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    bool NodeIsConstraintNode() const override { return true; }
    virtual bool IsHeaderConstraint() const { return false; }
    bool IsWhereConstraintNode() const { return GetNodeType() == NodeType::whereConstraintNode; }
};

class ParenthesizedConstraintNode : public ConstraintNode
{
public:
    ParenthesizedConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ParenthesizedConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConstraintNode* constraint_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const ConstraintNode* Constraint() const { return constraint.get(); }
    ConstraintNode* Constraint() { return constraint.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<ConstraintNode> constraint;
};

class BinaryConstraintNode : public ConstraintNode
{
public:
    BinaryConstraintNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    BinaryConstraintNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConstraintNode* left_, ConstraintNode* right_);
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const ConstraintNode* Left() const { return left.get(); }
    ConstraintNode* Left() { return left.get(); }
    const ConstraintNode* Right() const { return right.get(); }
    ConstraintNode* Right() { return right.get(); }
private:
    std::unique_ptr<ConstraintNode> left;
    std::unique_ptr<ConstraintNode> right;
};

class DisjunctiveConstraintNode : public BinaryConstraintNode
{
public:
    DisjunctiveConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DisjunctiveConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConstraintNode* left_, ConstraintNode* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class ConjunctiveConstraintNode : public BinaryConstraintNode
{
public:
    ConjunctiveConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConjunctiveConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConstraintNode* left_, ConstraintNode* right_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    std::string ToString() const override;
};

class WhereConstraintNode : public ConstraintNode
{
public:
    WhereConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    WhereConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, ConstraintNode* constraint_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const ConstraintNode* Constraint() const { return constraint.get(); }
    ConstraintNode* Constraint() { return constraint.get(); }
    std::string ToString() const override;
    void SetHeaderConstraint() { headerConstraint = true; }
    bool IsHeaderConstraint() const override { return headerConstraint; }
    void SetSemicolon() { semicolon = true; }
    bool Semicolon() const { return semicolon; }
private:
    std::unique_ptr<ConstraintNode> constraint;
    bool headerConstraint;
    bool semicolon;
};

class PredicateConstraintNode : public ConstraintNode
{
public:
    PredicateConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    PredicateConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* invokeExpr_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* InvokeExpr() const { return invokeExpr.get(); }
    Node* InvokeExpr() { return invokeExpr.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> invokeExpr;
};

class IsConstraintNode : public ConstraintNode
{
public:
    IsConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    IsConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* typeExpr_, Node* conceptOrTypeName_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* TypeExpr() const { return typeExpr.get(); }
    Node* TypeExpr() { return typeExpr.get(); }
    const Node* ConceptOrTypeName() const { return conceptOrTypeName.get(); }
    Node* ConceptOrTypeName() { return conceptOrTypeName.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<Node> conceptOrTypeName;
};

class MultiParamConstraintNode : public ConstraintNode
{
public:
    MultiParamConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    MultiParamConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, IdentifierNode* conceptId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const IdentifierNode* ConceptId() const { return conceptId.get(); }
    IdentifierNode* ConceptId() { return conceptId.get(); }
    const NodeList<Node>& TypeExprs() const { return typeExprs; }
    void AddTypeExpr(Node* typeExpr);
    std::string ToString() const override;
private:
    std::unique_ptr<IdentifierNode> conceptId;
    NodeList<Node> typeExprs;
};

class TypeNameConstraintNode : public ConstraintNode
{
public:
    TypeNameConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    TypeNameConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* typeId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* TypeId() const { return typeId.get(); }
    Node* TypeId() { return typeId.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> typeId;
};

class SignatureConstraintNode : public ConstraintNode
{
public:
    SignatureConstraintNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
};

class ConstructorConstraintNode : public SignatureConstraintNode
{
public:
    ConstructorConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConstructorConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, IdentifierNode* typeParamId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddParameter(ParameterNode* parameter) override;
    const IdentifierNode* TypeParamId() const { return typeParamId.get(); }
    IdentifierNode* TypeParamId() { return typeParamId.get(); }
    const NodeList<ParameterNode>& Parameters() const { return parameters; }
    std::string ToString() const override;
private:
    std::unique_ptr<IdentifierNode> typeParamId;
    NodeList<ParameterNode> parameters;
};

class DestructorConstraintNode : public SignatureConstraintNode
{
public:
    DestructorConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    DestructorConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, IdentifierNode* typeParamId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    IdentifierNode* TypeParamId() { return typeParamId.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<IdentifierNode> typeParamId;
};

class MemberFunctionConstraintNode : public SignatureConstraintNode
{
public:
    MemberFunctionConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    MemberFunctionConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* returnTypeExpr_, IdentifierNode* typeParamId_, const std::u32string& groupId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddParameter(ParameterNode* parameter) override;
    const Node* ReturnTypeExpr() const { return returnTypeExpr.get(); }
    Node* ReturnTypeExpr() { return returnTypeExpr.get(); }
    const IdentifierNode* TypeParamId() const { return typeParamId.get(); }
    IdentifierNode* TypeParamId() { return typeParamId.get(); }
    const std::u32string& GroupId() const { return groupId; }
    const NodeList<ParameterNode>& Parameters() const { return parameters; }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> returnTypeExpr;
    std::unique_ptr<IdentifierNode> typeParamId;
    std::u32string groupId;
    NodeList<ParameterNode> parameters;
};

class FunctionConstraintNode : public SignatureConstraintNode
{
public:
    FunctionConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    FunctionConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* returnTypeExpr_, const std::u32string& groupId_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddParameter(ParameterNode* parameter) override;
    const Node* ReturnTypeExpr() const { return returnTypeExpr.get(); }
    Node* ReturnTypeExpr() { return returnTypeExpr.get(); }
    const std::u32string& GroupId() const { return groupId; }
    const NodeList<ParameterNode>& Parameters() const { return parameters; }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> returnTypeExpr;
    std::u32string groupId;
    NodeList<ParameterNode> parameters;
};

class AxiomStatementNode : public Node
{
public:
    AxiomStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    AxiomStatementNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Node* expression_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const Node* Expression() const { return expression.get(); }
    Node* Expression() { return expression.get(); }
    std::string ToString() const override;
private:
    std::unique_ptr<Node> expression;
};

class AxiomNode : public Node
{
public:
    AxiomNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    AxiomNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    void AddParameter(ParameterNode* parameter) override;
    void AddStatement(AxiomStatementNode* statement);
    const IdentifierNode* Id() const { return id.get(); }
    IdentifierNode* Id() { return id.get(); }
    const NodeList<ParameterNode>& Parameters() const { return parameters; }
    const NodeList<AxiomStatementNode>& Statements() const { return statements; }
    void SetBeginBraceSourcePos(const soul::ast::SourcePos& beginBraceSourcePos_) { beginBraceSourcePos = beginBraceSourcePos_; }
    const soul::ast::SourcePos& BeginBraceSourcePos() const { return beginBraceSourcePos; }
    void SetEndBraceSourcePos(const soul::ast::SourcePos& endBraceSourcePos_) { endBraceSourcePos = endBraceSourcePos_; }
    const soul::ast::SourcePos& EndBraceSourcePos() const { return endBraceSourcePos; }
private:
    soul::ast::SourcePos beginBraceSourcePos;
    soul::ast::SourcePos endBraceSourcePos;
    std::unique_ptr<IdentifierNode> id;
    NodeList<ParameterNode> parameters;
    NodeList<AxiomStatementNode> statements;
};

class ConceptIdNode : public Node
{
public:
    ConceptIdNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConceptIdNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    const IdentifierNode* Id() const { return id.get(); }
    IdentifierNode* Id() { return id.get(); }
    const NodeList<Node>& TypeParameters() const { return typeParameters; }
    void AddTypeParameter(Node* typeParameter);
    int Arity() const { return typeParameters.Count(); }
    std::string ToString() const override;
private:
    std::unique_ptr<IdentifierNode> id;
    NodeList<Node> typeParameters;
};

class ConceptNode : public Node
{
public:
    ConceptNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConceptNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    ConceptNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, IdentifierNode* id_);
    ConceptNode(NodeType nodeType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_, Specifiers specifiers_, IdentifierNode* id_);
    Node* Clone(CloneContext& cloneContext) const override;
    void Accept(Visitor& visitor) override;
    void Write(AstWriter& writer) override;
    void Read(AstReader& reader) override;
    bool IsConceptNode() const override { return true; }
    virtual bool IsCommonConceptNode() const { return false; }
    const IdentifierNode* Id() const { return id.get(); }
    IdentifierNode* Id() { return id.get(); }
    int Arity() const { return typeParameters.Count(); }
    const NodeList<IdentifierNode>& TypeParameters() const { return typeParameters; }
    void AddTypeParameter(IdentifierNode* typeParameter);
    void SetRefinement(ConceptIdNode* refinement_);
    ConceptIdNode* Refinement() { return refinement.get(); }
    void AddConstraint(ConstraintNode* constraint);
    const NodeList<ConstraintNode>& Constraints() const { return constraints; }
    void AddAxiom(AxiomNode* axiom_);
    const NodeList<AxiomNode>& Axioms() const { return axioms; }
    Specifiers GetSpecifiers() const override { return specifiers; }
    void SetBeginBraceSourcePos(const soul::ast::SourcePos& beginBraceSourcePos_) { beginBraceSourcePos = beginBraceSourcePos_; }
    const soul::ast::SourcePos& BeginBraceSourcePos() const { return beginBraceSourcePos; }
    void SetEndBraceSourcePos(const soul::ast::SourcePos& endBraceSourcePos_) { endBraceSourcePos = endBraceSourcePos_; }
    const soul::ast::SourcePos& EndBraceSourcePos() const { return endBraceSourcePos; }
private:
    Specifiers specifiers;
    soul::ast::SourcePos beginBraceSourcePos;
    soul::ast::SourcePos endBraceSourcePos;
    std::unique_ptr<IdentifierNode> id;
    NodeList<IdentifierNode> typeParameters;
    std::unique_ptr<ConceptIdNode> refinement;
    NodeList<ConstraintNode> constraints;
    NodeList<AxiomNode> axioms;
};

class IntrinsicConstraintNode : public ConstraintNode
{
public:
    IntrinsicConstraintNode(NodeType nodeType_);
};

class SameConstraintNode : public IntrinsicConstraintNode
{
public:
    SameConstraintNode();
    SameConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    void Accept(Visitor& visitor) override;
    Node* Clone(CloneContext& cloneContext) const override;
};

class DerivedConstraintNode : public IntrinsicConstraintNode
{
public:
    DerivedConstraintNode();
    DerivedConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    void Accept(Visitor& visitor) override;
    Node* Clone(CloneContext& cloneContext) const override;
};

class ConvertibleConstraintNode : public IntrinsicConstraintNode
{
public:
    ConvertibleConstraintNode();
    ConvertibleConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    void Accept(Visitor& visitor) override;
    Node* Clone(CloneContext& cloneContext) const override;
};

class ExplicitlyConvertibleConstraintNode : public IntrinsicConstraintNode
{
public:
    ExplicitlyConvertibleConstraintNode();
    ExplicitlyConvertibleConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    void Accept(Visitor& visitor) override;
    Node* Clone(CloneContext& cloneContext) const override;
};

class CommonConstraintNode : public IntrinsicConstraintNode
{
public:
    CommonConstraintNode();
    CommonConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    void Accept(Visitor& visitor) override;
    Node* Clone(CloneContext& cloneContext) const override;
};

class NonreferenceTypeConstraintNode : public IntrinsicConstraintNode
{
public:
    NonreferenceTypeConstraintNode();
    NonreferenceTypeConstraintNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    void Accept(Visitor& visitor) override;
    Node* Clone(CloneContext& cloneContext) const override;
};

class SameConceptNode : public ConceptNode
{
public:
    SameConceptNode();
    SameConceptNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    bool IsIntrinsicConceptNode() const override { return true; }
};

class DerivedConceptNode : public ConceptNode
{
public:
    DerivedConceptNode();
    DerivedConceptNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    bool IsIntrinsicConceptNode() const override { return true; }
};

class ConvertibleConceptNode : public ConceptNode
{
public:
    ConvertibleConceptNode();
    ConvertibleConceptNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    bool IsIntrinsicConceptNode() const override { return true; }
};

class ExplicitlyConvertibleConceptNode : public ConceptNode
{
public:
    ExplicitlyConvertibleConceptNode();
    ExplicitlyConvertibleConceptNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    bool IsIntrinsicConceptNode() const override { return true; }
};

class CommonConceptNode : public ConceptNode
{
public:
    CommonConceptNode();
    CommonConceptNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    bool IsCommonConceptNode() const override { return true; }
    bool IsIntrinsicConceptNode() const override { return true; }
};

class NonreferenceTypeConceptNode : public ConceptNode
{
public:
    NonreferenceTypeConceptNode();
    NonreferenceTypeConceptNode(const soul::ast::SourcePos& sourcePos_, const util::uuid& moduleId_);
    bool IsIntrinsicConceptNode() const override { return true; }
};
} // namespace cmajor::ast

