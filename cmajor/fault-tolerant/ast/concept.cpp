// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.concepts;

import cmajor.fault.tolerant.ast.visitor;

namespace cmajor::fault::tolerant::ast {

ConceptIdNode::ConceptIdNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), typeParameterList(soul::ast::Span(), CompletionContext::none)
{
}

void ConceptIdNode::SetRefinedConcept(QualifiedIdNode* refinedConcept_)
{
    refinedConcept.reset(refinedConcept_);
    AddChildNode(refinedConcept.get());
}

void ConceptIdNode::SetLAngle(LAngleNode* langle_)
{
    langle.reset(langle_);
    AddChildNode(langle.get());
}

void ConceptIdNode::AddTypeParameter(IdentifierNode* typeParameter)
{
    typeParameterList.AddNode(typeParameter);
}

void ConceptIdNode::AddComma(CommaNode* comma)
{
    typeParameterList.AddComma(comma);
}

void ConceptIdNode::SetRAngle(RAngleNode* rangle_)
{
    rangle.reset(rangle_);
    AddChildNode(rangle.get());
}

Node* ConceptIdNode::Clone() const
{
    ConceptIdNode* clone = new ConceptIdNode(GetSpan(), GetCompletionContext());
    if (refinedConcept)
    {
        clone->SetRefinedConcept(static_cast<QualifiedIdNode*>(refinedConcept->Clone()));
    }
    clone->SetLAngle(static_cast<LAngleNode*>(langle->Clone()));
    clone->typeParameterList.CloneFrom(typeParameterList);
    clone->SetRAngle(static_cast<RAngleNode*>(rangle->Clone()));
    clone->Make();
    return clone;
}

void ConceptIdNode::Make()
{
    typeParameterList.Make();
    AddChildNode(&typeParameterList);
    SyntaxNode::Make();
}

void ConceptIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstraintNode::ConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

BinaryConstraintNode::BinaryConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ConstraintNode(span_, completionContext_)
{
}

void BinaryConstraintNode::SetLeft(ConstraintNode* left_)
{
    left.reset(left_);
    AddChildNode(left.get());
}

void BinaryConstraintNode::SetOp(KeywordNode* op_)
{
    op.reset(op_);
    AddChildNode(op.get());
}

void BinaryConstraintNode::SetRight(ConstraintNode* right_)
{
    right.reset(right_);
    AddChildNode(right.get());
}

DisjunctiveConstraintNode::DisjunctiveConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : BinaryConstraintNode(span_, completionContext_)
{
}

Node* DisjunctiveConstraintNode::Clone() const
{
    DisjunctiveConstraintNode* clone = new DisjunctiveConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetLeft(static_cast<ConstraintNode*>(Left()->Clone()));
    clone->SetOp(static_cast<KeywordNode*>(Op()->Clone()));
    clone->SetRight(static_cast<ConstraintNode*>(Right()->Clone()));
    clone->Make();
    return clone;
}

void DisjunctiveConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConjunctiveConstraintNode::ConjunctiveConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : BinaryConstraintNode(span_, completionContext_)
{
}

Node* ConjunctiveConstraintNode::Clone() const
{
    ConjunctiveConstraintNode* clone = new ConjunctiveConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetLeft(static_cast<ConstraintNode*>(Left()->Clone()));
    clone->SetOp(static_cast<KeywordNode*>(Op()->Clone()));
    clone->SetRight(static_cast<ConstraintNode*>(Right()->Clone()));
    clone->Make();
    return clone;
}

void ConjunctiveConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ParenthesizedConstraintNode::ParenthesizedConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ConstraintNode(span_, completionContext_)
{
}

void ParenthesizedConstraintNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void ParenthesizedConstraintNode::SetConstraintExpr(ConstraintNode* constraintExpr_)
{
    constraintExpr.reset(constraintExpr_);
    AddChildNode(constraintExpr.get());
}

void ParenthesizedConstraintNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

Node* ParenthesizedConstraintNode::Clone() const
{
    ParenthesizedConstraintNode* clone = new ParenthesizedConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetConstraintExpr(static_cast<ConstraintNode*>(constraintExpr->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->Make();
    return clone;
}

void ParenthesizedConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

PredicateConstraintNode::PredicateConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ConstraintNode(span_, completionContext_)
{
}

void PredicateConstraintNode::SetInvokeExpr(Node* invokeExpr_)
{
    invokeExpr.reset(invokeExpr_);
    AddChildNode(invokeExpr.get());
}

Node* PredicateConstraintNode::Clone() const
{
    PredicateConstraintNode* clone = new PredicateConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetInvokeExpr(invokeExpr->Clone());
    clone->Make();
    return clone;
}

void PredicateConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

IsConstraintNode::IsConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ConstraintNode(span_, completionContext_)
{
}

void IsConstraintNode::SetTypeExpr(Node* typeExpr_)
{
    typeExpr.reset(typeExpr_);
    AddChildNode(typeExpr.get());
}

void IsConstraintNode::SetIsKeyword(IsKeywordNode* isKeyword_)
{
    isKeyword.reset(isKeyword_);
    AddChildNode(isKeyword.get());
}

void IsConstraintNode::SetConceptOrTypeName(Node* conceptOrTypeName_)
{
    conceptOrTypeName.reset(conceptOrTypeName_);
    AddChildNode(conceptOrTypeName.get());
}

Node* IsConstraintNode::Clone() const
{
    IsConstraintNode* clone = new IsConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetTypeExpr(typeExpr->Clone());
    clone->SetIsKeyword(static_cast<IsKeywordNode*>(isKeyword->Clone()));
    clone->SetConceptOrTypeName(conceptOrTypeName->Clone());
    clone->Make();
    return clone;
}

void IsConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MultiparamConstraintNode::MultiparamConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    ConstraintNode(span_, completionContext_), typeExprList(soul::ast::Span(), CompletionContext::none)
{
}

void MultiparamConstraintNode::SetConceptName(QualifiedIdNode* conceptName_)
{
    conceptName.reset(conceptName_);
    AddChildNode(conceptName.get());
}

void MultiparamConstraintNode::SetLAngle(LAngleNode* langle_)
{
    langle.reset(langle_);
    AddChildNode(langle.get());
}

void MultiparamConstraintNode::AddTypeExpr(Node* typeExpr)
{
    typeExprList.AddNode(typeExpr);
    AddChildNode(typeExpr);
}

void MultiparamConstraintNode::AddComma(CommaNode* comma)
{
    typeExprList.AddComma(comma);
    AddChildNode(comma);
}

void MultiparamConstraintNode::SetRAngle(RAngleNode* rangle_)
{
    rangle.reset(rangle_);
    AddChildNode(rangle.get());
}

Node* MultiparamConstraintNode::Clone() const
{
    MultiparamConstraintNode* clone = new MultiparamConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetConceptName(static_cast<QualifiedIdNode*>(conceptName->Clone()));
    clone->SetLAngle(static_cast<LAngleNode*>(langle->Clone()));
    clone->typeExprList.CloneFrom(typeExprList);
    clone->SetRAngle(static_cast<RAngleNode*>(rangle->Clone()));
    clone->Make();
    return clone;
}

void MultiparamConstraintNode::Make()
{
    typeExprList.Make();
    AddChildNode(&typeExprList);
    ConstraintNode::Make();
}

void MultiparamConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

TypeNameConstraintNode::TypeNameConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ConstraintNode(span_, completionContext_)
{
}

void TypeNameConstraintNode::SetTypeNameKeyword(TypeNameKeywordNode* typeNameKeyword_)
{
    typeNameKeyword.reset(typeNameKeyword_);
    AddChildNode(typeNameKeyword.get());
}

void TypeNameConstraintNode::SetTypeExpr(Node* typeExpr_)
{
    typeExpr.reset(typeExpr_);
    AddChildNode(typeExpr.get());
}

void TypeNameConstraintNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* TypeNameConstraintNode::Clone() const
{
    TypeNameConstraintNode* clone = new TypeNameConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetTypeNameKeyword(static_cast<TypeNameKeywordNode*>(typeNameKeyword->Clone()));
    clone->SetTypeExpr(typeExpr->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void TypeNameConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConstructorConstraintNode::ConstructorConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) :
    ConstraintNode(span_, completionContext_), parameterList(soul::ast::Span(), CompletionContext::none)
{
}

void ConstructorConstraintNode::SetTypeParamId(IdentifierNode* typeParamId_)
{
    typeParamId.reset(typeParamId_);
    AddChildNode(typeParamId.get());
}

void ConstructorConstraintNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* ConstructorConstraintNode::Clone() const
{
    ConstructorConstraintNode* clone = new ConstructorConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetTypeParamId(static_cast<IdentifierNode*>(typeParamId->Clone()));
    clone->parameterList.CloneFrom(parameterList);
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void ConstructorConstraintNode::Make()
{
    parameterList.Make();
    AddChildNode(&parameterList);
    ConstraintNode::Make();
}

void ConstructorConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

DestructorConstraintNode::DestructorConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ConstraintNode(span_, completionContext_)
{
}

void DestructorConstraintNode::SetComplement(ComplementNode* complement_)
{
    complement.reset(complement_);
    AddChildNode(complement.get());
}

void DestructorConstraintNode::SetTypeParamId(IdentifierNode* typeParamId_)
{
    typeParamId.reset(typeParamId_);
    AddChildNode(typeParamId.get());
}

void DestructorConstraintNode::SetLParen(LParenNode* lparen_)
{
    lparen.reset(lparen_);
    AddChildNode(lparen.get());
}

void DestructorConstraintNode::SetRParen(RParenNode* rparen_)
{
    rparen.reset(rparen_);
    AddChildNode(rparen.get());
}

void DestructorConstraintNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* DestructorConstraintNode::Clone() const
{
    DestructorConstraintNode* clone = new DestructorConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetComplement(static_cast<ComplementNode*>(complement->Clone()));
    clone->SetTypeParamId(static_cast<IdentifierNode*>(typeParamId->Clone()));
    clone->SetLParen(static_cast<LParenNode*>(lparen->Clone()));
    clone->SetRParen(static_cast<RParenNode*>(rparen->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void DestructorConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

MemberFunctionConstraintNode::MemberFunctionConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    ConstraintNode(span_, completionContext_), parameterList(soul::ast::Span(), CompletionContext::none)
{
}

void MemberFunctionConstraintNode::SetReturnType(Node* returnType_)
{
    returnType.reset(returnType_);
    AddChildNode(returnType.get());
}

void MemberFunctionConstraintNode::SetTypeParamId(IdentifierNode* typeParamId_)
{
    typeParamId.reset(typeParamId_);
    AddChildNode(typeParamId.get());
}

void MemberFunctionConstraintNode::SetDot(DotNode* dot_)
{
    dot.reset(dot_);
    AddChildNode(dot.get());
}

void MemberFunctionConstraintNode::SetFunctionGroupId(Node* functionGroupId_)
{
    functionGroupId.reset(functionGroupId_);
    AddChildNode(functionGroupId.get());
}

void MemberFunctionConstraintNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* MemberFunctionConstraintNode::Clone() const
{
    MemberFunctionConstraintNode* clone = new MemberFunctionConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetReturnType(returnType->Clone());
    clone->SetTypeParamId(static_cast<IdentifierNode*>(typeParamId->Clone()));
    clone->SetDot(static_cast<DotNode*>(dot->Clone()));
    clone->SetFunctionGroupId(functionGroupId->Clone());
    clone->parameterList.CloneFrom(parameterList);
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void MemberFunctionConstraintNode::Make()
{
    parameterList.Make();
    AddChildNode(&parameterList);
    ConstraintNode::Make();
}

void MemberFunctionConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

FunctionConstraintNode::FunctionConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    ConstraintNode(span_, completionContext_), parameterList(soul::ast::Span(), CompletionContext::none)
{
}

void FunctionConstraintNode::SetReturnType(Node* returnType_)
{
    returnType.reset(returnType_);
    AddChildNode(returnType.get());
}

void FunctionConstraintNode::SetFunctionGroupId(Node* functionGroupId_)
{
    functionGroupId.reset(functionGroupId_);
    AddChildNode(functionGroupId.get());
}

void FunctionConstraintNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* FunctionConstraintNode::Clone() const
{
    FunctionConstraintNode* clone = new FunctionConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetReturnType(returnType->Clone());
    clone->SetFunctionGroupId(functionGroupId->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void FunctionConstraintNode::Make()
{
    parameterList.Make();
    AddChildNode(&parameterList);
    ConstraintNode::Make();
}

void FunctionConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

WhereConstraintNode::WhereConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ConstraintNode(span_, completionContext_)
{
}

void WhereConstraintNode::SetWhereKeyword(WhereKeywordNode* whereKeyword_)
{
    whereKeyword.reset(whereKeyword_);
    AddChildNode(whereKeyword.get());
}

void WhereConstraintNode::SetConstraintExpr(ConstraintNode* constraintExpr_)
{
    constraintExpr.reset(constraintExpr_);
    AddChildNode(constraintExpr.get());
}

Node* WhereConstraintNode::Clone() const
{
    WhereConstraintNode* clone = new WhereConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetWhereKeyword(static_cast<WhereKeywordNode*>(whereKeyword->Clone()));
    clone->SetConstraintExpr(static_cast<ConstraintNode*>(constraintExpr->Clone()));
    clone->Make();
    return clone;
}

void WhereConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

EmbeddedConstraintNode::EmbeddedConstraintNode(const soul::ast::Span& span_, CompletionContext completionContext_) : ConstraintNode(span_, completionContext_)
{
}

void EmbeddedConstraintNode::SetWhereConstraint(WhereConstraintNode* whereConstraint_)
{
    whereConstraint.reset(whereConstraint_);
    AddChildNode(whereConstraint.get());
}

void EmbeddedConstraintNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* EmbeddedConstraintNode::Clone() const
{
    EmbeddedConstraintNode* clone = new EmbeddedConstraintNode(GetSpan(), GetCompletionContext());
    clone->SetWhereConstraint(static_cast<WhereConstraintNode*>(whereConstraint->Clone()));
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void EmbeddedConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AxiomStatementNode::AxiomStatementNode(const soul::ast::Span& span_, CompletionContext completionContext_) : SyntaxNode(span_, completionContext_)
{
}

void AxiomStatementNode::SetExpression(Node* expression_)
{
    expression.reset(expression_);
    AddChildNode(expression.get());
}

void AxiomStatementNode::SetSemicolon(SemicolonNode* semicolon_)
{
    semicolon.reset(semicolon_);
    AddChildNode(semicolon.get());
}

Node* AxiomStatementNode::Clone() const
{
    AxiomStatementNode* clone = new AxiomStatementNode(GetSpan(), GetCompletionContext());
    clone->SetExpression(expression->Clone());
    clone->SetSemicolon(static_cast<SemicolonNode*>(semicolon->Clone()));
    clone->Make();
    return clone;
}

void AxiomStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

AxiomNode::AxiomNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), parameterList(soul::ast::Span(), CompletionContext::none)
{
}

void AxiomNode::SetAxiomId(IdentifierNode* axiomId_)
{
    axiomId.reset(axiomId_);
    AddChildNode(axiomId.get());
}

void AxiomNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void AxiomNode::AddAxiomStatement(AxiomStatementNode* axiomStatement)
{
    axiomStatements.AddNode(axiomStatement);
    AddChildNode(axiomStatement);
}

void AxiomNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* AxiomNode::Clone() const
{
    AxiomNode* clone = new AxiomNode(GetSpan(), GetCompletionContext());
    clone->SetAxiomId(static_cast<IdentifierNode*>(axiomId->Clone()));
    clone->parameterList.CloneFrom(parameterList);
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    for (const auto& axiomStatement : axiomStatements.Nodes())
    {
        clone->AddAxiomStatement(static_cast<AxiomStatementNode*>(axiomStatement->Clone()));
    }
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void AxiomNode::Make()
{
    parameterList.Make();
    AddChildNode(&parameterList);
    axiomStatements.Make();
    AddChildNode(&axiomStatements);
    SyntaxNode::Make();
}

void AxiomNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

ConceptNode::ConceptNode(const soul::ast::Span& span_, CompletionContext completionContext_) : 
    SyntaxNode(span_, completionContext_), typeParameterList(soul::ast::Span(), CompletionContext::none)
{
}

void ConceptNode::SetSpecifiersNode(SpecifiersNode* specifiersNode_)
{
    specifiersNode.reset(specifiersNode_);
    AddChildNode(specifiersNode.get());
}

void ConceptNode::SetConceptKeyword(ConceptKeywordNode* conceptKeyword_)
{
    conceptKeyword.reset(conceptKeyword_);
    AddChildNode(conceptKeyword.get());
}

void ConceptNode::SetConceptId(IdentifierNode* conceptId_)
{
    conceptId.reset(conceptId_);
    AddChildNode(conceptId.get());
}

void ConceptNode::SetLAngle(LAngleNode* langle_)
{
    langle.reset(langle_);
    AddChildNode(langle.get());
}

void ConceptNode::AddTypeParameter(IdentifierNode* typeParameter)
{
    typeParameterList.AddNode(typeParameter);
}

Node* ConceptNode::GetFirstTypeParameter() const
{
    return typeParameterList.GetItem(0);
}

void ConceptNode::AddComma(CommaNode* comma)
{
    typeParameterList.AddComma(comma);
}

void ConceptNode::SetRAngle(RAngleNode* rangle_)
{
    rangle.reset(rangle_);
    AddChildNode(rangle.get());
}

void ConceptNode::SetRefinement(ConceptIdNode* refinement_)
{
    refinement.reset(refinement_);
    AddChildNode(refinement.get());
}

void ConceptNode::SetWhereConstraint(WhereConstraintNode* whereConstraint_)
{
    whereConstraint.reset(whereConstraint_);
    AddChildNode(whereConstraint.get());
}

void ConceptNode::SetLBrace(LBraceNode* lbrace_)
{
    lbrace.reset(lbrace_);
    AddChildNode(lbrace.get());
}

void ConceptNode::AddNode(Node* bodyConstraint)
{
    bodyConstraints.AddNode(bodyConstraint);
}

void ConceptNode::SetRBrace(RBraceNode* rbrace_)
{
    rbrace.reset(rbrace_);
    AddChildNode(rbrace.get());
}

Node* ConceptNode::Clone() const
{
    ConceptNode* clone = new ConceptNode(GetSpan(), GetCompletionContext());
    clone->SetSpecifiersNode(static_cast<SpecifiersNode*>(specifiersNode->Clone()));
    clone->SetConceptKeyword(static_cast<ConceptKeywordNode*>(conceptKeyword->Clone()));
    clone->SetConceptId(static_cast<IdentifierNode*>(conceptId->Clone()));
    clone->SetLAngle(static_cast<LAngleNode*>(langle->Clone()));
    clone->typeParameterList.CloneFrom(typeParameterList);
    clone->SetRAngle(static_cast<RAngleNode*>(rangle->Clone()));
    if (refinement)
    {
        clone->SetRefinement(static_cast<ConceptIdNode*>(refinement->Clone()));
    }
    if (whereConstraint)
    {
        clone->SetWhereConstraint(static_cast<WhereConstraintNode*>(whereConstraint->Clone()));
    }
    clone->SetLBrace(static_cast<LBraceNode*>(lbrace->Clone()));
    for (const auto& node : bodyConstraints.Nodes())
    {
        clone->AddNode(node->Clone());
    }
    clone->SetRBrace(static_cast<RBraceNode*>(rbrace->Clone()));
    clone->Make();
    return clone;
}

void ConceptNode::Make()
{
    typeParameterList.Make();
    AddChildNode(&typeParameterList);
    bodyConstraints.Make();
    AddChildNode(&bodyConstraints);
    SyntaxNode::Make();
}

void ConceptNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

} // namespace cmajor::fault::tolerant::ast
