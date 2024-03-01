// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.concepts;

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

class ConceptIdNode : public SyntaxNode
{
public:
    ConceptIdNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetRefinedConcept(QualifiedIdNode* refinedConcept_);
    QualifiedIdNode* RefinedConcept() const { return refinedConcept.get(); }
    void SetLAngle(LAngleNode* langle_);
    LAngleNode* LAngle() const { return langle.get(); }
    void AddTypeParameter(IdentifierNode* typeParameter);
    void AddComma(CommaNode* comma) override;
    ListNode* TypeParameterList() { return &typeParameterList; }
    void SetRAngle(RAngleNode* rangle_);
    RAngleNode* RAngle() const { return rangle.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<QualifiedIdNode> refinedConcept;
    std::unique_ptr<LAngleNode> langle;
    ListNode typeParameterList;
    std::unique_ptr<RAngleNode> rangle;
};

class ConstraintNode : public SyntaxNode
{
public:
    ConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
};

class BinaryConstraintNode : public ConstraintNode
{
public:
    BinaryConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetLeft(ConstraintNode* left_);
    ConstraintNode* Left() const { return left.get(); }
    void SetOp(KeywordNode* op_);
    KeywordNode* Op() const { return op.get(); }
    void SetRight(ConstraintNode* right_);
    ConstraintNode* Right() const { return right.get(); }
private:
    std::unique_ptr<ConstraintNode> left;
    std::unique_ptr<KeywordNode> op;
    std::unique_ptr<ConstraintNode> right;
};

class DisjunctiveConstraintNode : public BinaryConstraintNode
{
public:
    DisjunctiveConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ConjunctiveConstraintNode : public BinaryConstraintNode
{
public:
    ConjunctiveConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
};

class ParenthesizedConstraintNode : public ConstraintNode
{
public:
    ParenthesizedConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetConstraintExpr(ConstraintNode* constraintExpr_);
    ConstraintNode* ConstraintExpr() const { return constraintExpr.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<ConstraintNode> constraintExpr;
    std::unique_ptr<RParenNode> rparen;
};

class PredicateConstraintNode : public ConstraintNode
{
public:
    PredicateConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetInvokeExpr(Node* invokeExpr_);
    Node* InvokeExpr() const { return invokeExpr.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> invokeExpr;
};

class IsConstraintNode : public ConstraintNode
{
public:
    IsConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetTypeExpr(Node* typeExpr_);
    Node* TypeExpr() const { return typeExpr.get(); }
    void SetIsKeyword(IsKeywordNode* isKeyword_);
    IsKeywordNode* IsKeyword() const { return isKeyword.get(); }
    void SetConceptOrTypeName(Node* conceptOrTypeName_);
    Node* ConceptOrTypeName() const { return conceptOrTypeName.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<IsKeywordNode> isKeyword;
    std::unique_ptr<Node> conceptOrTypeName;
};

class MultiparamConstraintNode : public ConstraintNode
{
public:
    MultiparamConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetConceptName(QualifiedIdNode* conceptName_);
    QualifiedIdNode* ConceptName() const { return conceptName.get(); }
    void SetLAngle(LAngleNode* langle_);
    LAngleNode* LAngle() const { return langle.get(); }
    void AddTypeExpr(Node* typeExpr);
    void AddComma(CommaNode* comma) override;
    ListNode* TypeExprList() { return &typeExprList; }
    void SetRAngle(RAngleNode* rangle_);
    RAngleNode* RAngle() const { return rangle.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<QualifiedIdNode> conceptName;
    std::unique_ptr<LAngleNode> langle;
    ListNode typeExprList;
    std::unique_ptr<RAngleNode> rangle;
};

class TypeNameConstraintNode : public ConstraintNode
{
public:
    TypeNameConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetTypeNameKeyword(TypeNameKeywordNode* typeNameKeyword_);
    TypeNameKeywordNode* TypeNameKeyword() const { return typeNameKeyword.get(); }
    void SetTypeExpr(Node* typeExpr_);
    Node* TypeExpr() const { return typeExpr.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<TypeNameKeywordNode> typeNameKeyword;
    std::unique_ptr<Node> typeExpr;
    std::unique_ptr<SemicolonNode> semicolon;
};

class ConstructorConstraintNode : public ConstraintNode
{
public:
    ConstructorConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetTypeParamId(IdentifierNode* typeParamId_);
    IdentifierNode* TypeParamId() const { return typeParamId.get(); }
    ParameterListNode* ParameterList() { return &parameterList; }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> typeParamId;
    ParameterListNode parameterList;
    std::unique_ptr<SemicolonNode> semicolon;
};

class DestructorConstraintNode : public ConstraintNode
{
public:
    DestructorConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetComplement(ComplementNode* complement_);
    ComplementNode* Complement() const { return complement.get(); }
    void SetTypeParamId(IdentifierNode* typeParamId_);
    IdentifierNode* TypeParamId() const { return typeParamId.get(); }
    void SetLParen(LParenNode* lparen_);
    LParenNode* LParen() const { return lparen.get(); }
    void SetRParen(RParenNode* rparen_);
    RParenNode* RParen() const { return rparen.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<ComplementNode> complement;
    std::unique_ptr<IdentifierNode> typeParamId;
    std::unique_ptr<LParenNode> lparen;
    std::unique_ptr<RParenNode> rparen;
    std::unique_ptr<SemicolonNode> semicolon;
};

class MemberFunctionConstraintNode : public ConstraintNode
{
public:
    MemberFunctionConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetReturnType(Node* returnType_);
    Node* ReturnType() const { return returnType.get(); }
    void SetTypeParamId(IdentifierNode* typeParamId_);
    IdentifierNode* TypeParamId() const { return typeParamId.get(); }
    void SetDot(DotNode* dot_);
    DotNode* Dot() const { return dot.get(); }
    void SetFunctionGroupId(Node* functionGroupId_);
    Node* FunctionGroupId() const { return functionGroupId.get(); }
    ParameterListNode* ParameterList() { return &parameterList; }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> returnType;
    std::unique_ptr<IdentifierNode> typeParamId;
    std::unique_ptr<DotNode> dot;
    std::unique_ptr<Node> functionGroupId;
    ParameterListNode parameterList;
    std::unique_ptr<SemicolonNode> semicolon;
};

class FunctionConstraintNode : public ConstraintNode
{
public:
    FunctionConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetReturnType(Node* returnType_);
    Node* ReturnType() const { return returnType.get(); }
    void SetFunctionGroupId(Node* functionGroupId_);
    Node* FunctionGroupId() const { return functionGroupId.get(); }
    ParameterListNode* ParameterList() { return &parameterList; }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> returnType;
    std::unique_ptr<Node> functionGroupId;
    ParameterListNode parameterList;
    std::unique_ptr<SemicolonNode> semicolon;
};

class WhereConstraintNode : public ConstraintNode
{
public:
    WhereConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetWhereKeyword(WhereKeywordNode* whereKeyword_);
    WhereKeywordNode* WhereKeyword() const { return whereKeyword.get(); }
    void SetConstraintExpr(ConstraintNode* constraintExpr_);
    ConstraintNode* ConstraintExpr() const { return constraintExpr.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<WhereKeywordNode> whereKeyword;
    std::unique_ptr<ConstraintNode> constraintExpr;
};

class EmbeddedConstraintNode : public ConstraintNode
{
public:
    EmbeddedConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetWhereConstraint(WhereConstraintNode* whereConstraint_);
    WhereConstraintNode* WhereConstraint() const { return whereConstraint.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<WhereConstraintNode> whereConstraint;
    std::unique_ptr<SemicolonNode> semicolon;
};

class AxiomStatementNode : public SyntaxNode
{
public:
    AxiomStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetExpression(Node* expression_);
    Node* Expression() const { return expression.get(); }
    void SetSemicolon(SemicolonNode* semicolon_);
    SemicolonNode* Semicolon() const { return semicolon.get(); }
    Node* Clone() const override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<Node> expression;
    std::unique_ptr<SemicolonNode> semicolon;
};

class AxiomNode : public SyntaxNode
{
public:
    AxiomNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetAxiomId(IdentifierNode* axiomId_);
    IdentifierNode* AxiomId() const { return axiomId.get(); }
    ParameterListNode* ParameterList() { return &parameterList; }
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddAxiomStatement(AxiomStatementNode* axiomStatement);
    NodeList<AxiomStatementNode>* AxiomStatements() { return &axiomStatements; }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<IdentifierNode> axiomId;
    ParameterListNode parameterList;
    std::unique_ptr<LBraceNode> lbrace;
    NodeList<AxiomStatementNode> axiomStatements;
    std::unique_ptr<RBraceNode> rbrace;
};

class ConceptNode : public SyntaxNode
{
public:
    ConceptNode(const soul::ast::Span& span_, CompletionContext completionContext_);
    void SetSpecifiersNode(SpecifiersNode* specifiersNode_);
    SpecifiersNode* Specifiers() const { return specifiersNode.get(); }
    void SetConceptKeyword(ConceptKeywordNode* conceptKeyword_);
    ConceptKeywordNode* ConceptKeyword() const { return conceptKeyword.get(); }
    void SetConceptId(IdentifierNode* conceptId_);
    IdentifierNode* ConceptId() const { return conceptId.get(); }
    void SetLAngle(LAngleNode* langle_);
    LAngleNode* LAngle() const { return langle.get(); }
    void AddTypeParameter(IdentifierNode* typeParameter);
    Node* GetFirstTypeParameter() const;
    ListNode* TypeParameterList() { return &typeParameterList; }
    void AddComma(CommaNode* comma) override;
    void SetRAngle(RAngleNode* rangle_);
    RAngleNode* RAngle() const { return rangle.get(); }
    void SetRefinement(ConceptIdNode* refinement_);
    ConceptIdNode* Refinement() const { return refinement.get(); }
    void SetWhereConstraint(WhereConstraintNode* whereConstraint_);
    WhereConstraintNode* WhereConstraint() const { return whereConstraint.get(); }
    void SetLBrace(LBraceNode* lbrace_);
    LBraceNode* LBrace() const { return lbrace.get(); }
    void AddNode(Node* bodyConstraint);
    NodeList<Node>* BodyConstraints() { return &bodyConstraints; }
    void SetRBrace(RBraceNode* rbrace_);
    RBraceNode* RBrace() const { return rbrace.get(); }
    Node* Clone() const override;
    void Make() override;
    void Accept(Visitor& visitor) override;
private:
    std::unique_ptr<SpecifiersNode> specifiersNode;
    std::unique_ptr<ConceptKeywordNode> conceptKeyword;
    std::unique_ptr<IdentifierNode> conceptId;
    std::unique_ptr<LAngleNode> langle;
    ListNode typeParameterList;
    std::unique_ptr<RAngleNode> rangle;
    std::unique_ptr<ConceptIdNode> refinement;
    std::unique_ptr<WhereConstraintNode> whereConstraint;
    std::unique_ptr<LBraceNode> lbrace;
    NodeList<Node> bodyConstraints;
    std::unique_ptr<RBraceNode> rbrace;
};

} // namespace cmajor::fault::tolerant::ast
