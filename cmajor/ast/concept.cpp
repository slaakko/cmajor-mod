// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.ast.concepts;

import std.core;
import cmajor.ast.identifier;
import cmajor.ast.parameter;
import cmajor.ast.reader;
import cmajor.ast.writer;
import cmajor.ast.visitor;
import util.unicode;
import util;

namespace cmajor::ast {

ConstraintNode::ConstraintNode(NodeType nodeType_, const soul::ast::Span& span_) : Node(nodeType_, span_)
{
}

ParenthesizedConstraintNode::ParenthesizedConstraintNode(const soul::ast::Span& span_) : ConstraintNode(NodeType::parenthesizedConstraintNode, span_)
{
}

ParenthesizedConstraintNode::ParenthesizedConstraintNode(const soul::ast::Span& span_, ConstraintNode* constraint_) :
    ConstraintNode(NodeType::parenthesizedConstraintNode, span_), constraint(constraint_)
{
}

Node* ParenthesizedConstraintNode::Clone(CloneContext& cloneContext) const
{
    ParenthesizedConstraintNode* clone = new ParenthesizedConstraintNode(GetSpan(), static_cast<ConstraintNode*>(constraint->Clone(cloneContext)));
    return clone;
}

void ParenthesizedConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ParenthesizedConstraintNode::Write(AstWriter& writer)
{
    ConstraintNode::Write(writer);
    writer.Write(constraint.get());
}

void ParenthesizedConstraintNode::Read(AstReader& reader)
{
    ConstraintNode::Read(reader);
    constraint.reset(reader.ReadConstraintNode());
}

std::string ParenthesizedConstraintNode::ToString() const
{
    return "(" + constraint->ToString() + ")";
}

BinaryConstraintNode::BinaryConstraintNode(NodeType nodeType_, const soul::ast::Span& span_) : ConstraintNode(nodeType_, span_), left(), right()
{
}

BinaryConstraintNode::BinaryConstraintNode(NodeType nodeType_, const soul::ast::Span& span_, ConstraintNode* left_, ConstraintNode* right_) :
    ConstraintNode(nodeType_, span_), left(left_), right(right_)
{
    left->SetParent(this);
    right->SetParent(this);
}

void BinaryConstraintNode::Write(AstWriter& writer)
{
    ConstraintNode::Write(writer);
    writer.Write(left.get());
    writer.Write(right.get());
}

void BinaryConstraintNode::Read(AstReader& reader)
{
    ConstraintNode::Read(reader);
    left.reset(reader.ReadConstraintNode());
    left->SetParent(this);
    right.reset(reader.ReadConstraintNode());
    right->SetParent(this);
}

DisjunctiveConstraintNode::DisjunctiveConstraintNode(const soul::ast::Span& span_) : BinaryConstraintNode(NodeType::disjunctiveConstraintNode, span_)
{
}

DisjunctiveConstraintNode::DisjunctiveConstraintNode(const soul::ast::Span& span_, ConstraintNode* left_, ConstraintNode* right_) :
    BinaryConstraintNode(NodeType::disjunctiveConstraintNode, span_, left_, right_)
{
}

Node* DisjunctiveConstraintNode::Clone(CloneContext& cloneContext) const
{
    DisjunctiveConstraintNode* clone = new DisjunctiveConstraintNode(GetSpan(), static_cast<ConstraintNode*>(Left()->Clone(cloneContext)), static_cast<ConstraintNode*>(Right()->Clone(cloneContext)));
    return clone;
}

void DisjunctiveConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string DisjunctiveConstraintNode::ToString() const
{
    return Left()->ToString() + " or " + Right()->ToString();
}

ConjunctiveConstraintNode::ConjunctiveConstraintNode(const soul::ast::Span& span_) : BinaryConstraintNode(NodeType::conjunctiveConstraintNode, span_)
{
}

ConjunctiveConstraintNode::ConjunctiveConstraintNode(const soul::ast::Span& span_, ConstraintNode* left_, ConstraintNode* right_) :
    BinaryConstraintNode(NodeType::conjunctiveConstraintNode, span_, left_, right_)
{
}

Node* ConjunctiveConstraintNode::Clone(CloneContext& cloneContext) const
{
    ConjunctiveConstraintNode* clone = new ConjunctiveConstraintNode(GetSpan(), static_cast<ConstraintNode*>(Left()->Clone(cloneContext)), static_cast<ConstraintNode*>(Right()->Clone(cloneContext)));
    return clone;
}

void ConjunctiveConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

std::string ConjunctiveConstraintNode::ToString() const
{
    return Left()->ToString() + " and " + Right()->ToString();
}


WhereConstraintNode::WhereConstraintNode(const soul::ast::Span& span_) :
    ConstraintNode(NodeType::whereConstraintNode, span_), constraint(), headerConstraint(false), semicolon(false)
{
}

WhereConstraintNode::WhereConstraintNode(const soul::ast::Span& span_, ConstraintNode* constraint_) :
    ConstraintNode(NodeType::whereConstraintNode, span_), constraint(constraint_), headerConstraint(false), semicolon(false)
{
    constraint->SetParent(this);
}

Node* WhereConstraintNode::Clone(CloneContext& cloneContext) const
{
    WhereConstraintNode* clone = new WhereConstraintNode(GetSpan(), static_cast<ConstraintNode*>(constraint->Clone(cloneContext)));
    if (headerConstraint)
    {
        clone->SetHeaderConstraint();
    }
    if (semicolon)
    {
        clone->SetSemicolon();
    }
    return clone;
}

void WhereConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void WhereConstraintNode::Write(AstWriter& writer)
{
    ConstraintNode::Write(writer);
    writer.Write(constraint.get());
    writer.GetBinaryStreamWriter().Write(headerConstraint);
    writer.GetBinaryStreamWriter().Write(semicolon);
}

void WhereConstraintNode::Read(AstReader& reader)
{
    ConstraintNode::Read(reader);
    constraint.reset(reader.ReadConstraintNode());
    constraint->SetParent(this);
    headerConstraint = reader.GetBinaryStreamReader().ReadBool();
    semicolon = reader.GetBinaryStreamReader().ReadBool();
}

std::string WhereConstraintNode::ToString() const
{
    return "where " + constraint->ToString();
}

PredicateConstraintNode::PredicateConstraintNode(const soul::ast::Span& span_) :
    ConstraintNode(NodeType::predicateConstraintNode, span_), invokeExpr()
{
}

PredicateConstraintNode::PredicateConstraintNode(const soul::ast::Span& span_, Node* invokeExpr_) :
    ConstraintNode(NodeType::predicateConstraintNode, span_), invokeExpr(invokeExpr_)
{
    invokeExpr->SetParent(this);
}

Node* PredicateConstraintNode::Clone(CloneContext& cloneContext) const
{
    PredicateConstraintNode* clone = new PredicateConstraintNode(GetSpan(), invokeExpr->Clone(cloneContext));
    return clone;
}

void PredicateConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void PredicateConstraintNode::Write(AstWriter& writer)
{
    ConstraintNode::Write(writer);
    writer.Write(invokeExpr.get());
}

void PredicateConstraintNode::Read(AstReader& reader)
{
    ConstraintNode::Read(reader);
    invokeExpr.reset(reader.ReadNode());
    invokeExpr->SetParent(this);
}

std::string PredicateConstraintNode::ToString() const
{
    return invokeExpr->ToString();
}

IsConstraintNode::IsConstraintNode(const soul::ast::Span& span_) :
    ConstraintNode(NodeType::isConstraintNode, span_), typeExpr(), conceptOrTypeName()
{
}

IsConstraintNode::IsConstraintNode(const soul::ast::Span& span_, Node* typeExpr_, Node* conceptOrTypeName_) :
    ConstraintNode(NodeType::isConstraintNode, span_), typeExpr(typeExpr_), conceptOrTypeName(conceptOrTypeName_)
{
    typeExpr->SetParent(this);
    conceptOrTypeName->SetParent(this);
}

Node* IsConstraintNode::Clone(CloneContext& cloneContext) const
{
    IsConstraintNode* clone = new IsConstraintNode(GetSpan(), typeExpr->Clone(cloneContext), conceptOrTypeName->Clone(cloneContext));
    return clone;
}

void IsConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void IsConstraintNode::Write(AstWriter& writer)
{
    ConstraintNode::Write(writer);
    writer.Write(typeExpr.get());
    writer.Write(conceptOrTypeName.get());
}

void IsConstraintNode::Read(AstReader& reader)
{
    ConstraintNode::Read(reader);
    typeExpr.reset(reader.ReadNode());
    typeExpr->SetParent(this);
    conceptOrTypeName.reset(reader.ReadNode());
    conceptOrTypeName->SetParent(this);
}

std::string IsConstraintNode::ToString() const
{
    return typeExpr->ToString() + " is " + conceptOrTypeName->ToString();
}

MultiParamConstraintNode::MultiParamConstraintNode(const soul::ast::Span& span_) :
    ConstraintNode(NodeType::multiParamConstraintNode, span_), conceptId(), typeExprs()
{
}

MultiParamConstraintNode::MultiParamConstraintNode(const soul::ast::Span& span_, IdentifierNode* conceptId_) :
    ConstraintNode(NodeType::multiParamConstraintNode, span_), conceptId(conceptId_), typeExprs()
{
    conceptId->SetParent(this);
}

Node* MultiParamConstraintNode::Clone(CloneContext& cloneContext) const
{
    MultiParamConstraintNode* clone = new MultiParamConstraintNode(GetSpan(), static_cast<IdentifierNode*>(conceptId->Clone(cloneContext)));
    int n = typeExprs.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddTypeExpr(typeExprs[i]->Clone(cloneContext));
    }
    return clone;
}

void MultiParamConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void MultiParamConstraintNode::Write(AstWriter& writer)
{
    ConstraintNode::Write(writer);
    writer.Write(conceptId.get());
    typeExprs.Write(writer);
}

void MultiParamConstraintNode::Read(AstReader& reader)
{
    ConstraintNode::Read(reader);
    conceptId.reset(reader.ReadIdentifierNode());
    conceptId->SetParent(this);
    typeExprs.Read(reader);
    typeExprs.SetParent(this);
}

void MultiParamConstraintNode::AddTypeExpr(Node* typeExpr)
{
    typeExpr->SetParent(this);
    typeExprs.Add(typeExpr);
}

std::string MultiParamConstraintNode::ToString() const
{
    std::string s = conceptId->ToString();
    s.append(1, '<');
    int n = typeExprs.Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            s.append(", ");
        }
        s.append(typeExprs[i]->ToString());
    }
    s.append(1, '>');
    return s;
}

TypeNameConstraintNode::TypeNameConstraintNode(const soul::ast::Span& span_) :
    ConstraintNode(NodeType::typeNameConstraintNode, span_), typeId()
{
}

TypeNameConstraintNode::TypeNameConstraintNode(const soul::ast::Span& span_, Node* typeId_) :
    ConstraintNode(NodeType::typeNameConstraintNode, span_), typeId(typeId_)
{
    typeId->SetParent(this);
}

Node* TypeNameConstraintNode::Clone(CloneContext& cloneContext) const
{
    TypeNameConstraintNode* clone = new TypeNameConstraintNode(GetSpan(), typeId->Clone(cloneContext));
    return clone;
}

void TypeNameConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void TypeNameConstraintNode::Write(AstWriter& writer)
{
    ConstraintNode::Write(writer);
    writer.Write(typeId.get());
}

void TypeNameConstraintNode::Read(AstReader& reader)
{
    ConstraintNode::Read(reader);
    typeId.reset(reader.ReadNode());
}

std::string TypeNameConstraintNode::ToString() const
{
    return "typename " + typeId->ToString();
}

SignatureConstraintNode::SignatureConstraintNode(NodeType nodeType_, const soul::ast::Span& span_) : 
    ConstraintNode(nodeType_, span_)
{
}

ConstructorConstraintNode::ConstructorConstraintNode(const soul::ast::Span& span_) :
    SignatureConstraintNode(NodeType::constructorConstraintNode, span_), typeParamId(), parameters()
{
}

ConstructorConstraintNode::ConstructorConstraintNode(const soul::ast::Span& span_, IdentifierNode* typeParamId_) :
    SignatureConstraintNode(NodeType::constructorConstraintNode, span_), typeParamId(typeParamId_), parameters()
{
    typeParamId->SetParent(this);
}

Node* ConstructorConstraintNode::Clone(CloneContext& cloneContext) const
{
    ConstructorConstraintNode* clone = new ConstructorConstraintNode(GetSpan(), static_cast<IdentifierNode*>(typeParamId->Clone(cloneContext)));
    int n = parameters.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddParameter(static_cast<ParameterNode*>(parameters[i]->Clone(cloneContext)));
    }
    return clone;
}

void ConstructorConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConstructorConstraintNode::Write(AstWriter& writer)
{
    SignatureConstraintNode::Write(writer);
    writer.Write(typeParamId.get());
    parameters.Write(writer);
}

void ConstructorConstraintNode::Read(AstReader& reader)
{
    SignatureConstraintNode::Read(reader);
    typeParamId.reset(static_cast<IdentifierNode*>(reader.ReadNode()));
    typeParamId->SetParent(this);
    parameters.Read(reader);
    parameters.SetParent(this);
}

std::string ConstructorConstraintNode::ToString() const
{
    std::string s = typeParamId->ToString();
    s.append(1, '(');
    int n = parameters.Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            s.append(", ");
        }
        ParameterNode* p = parameters[i];
        s.append(p->TypeExpr()->ToString());
    }
    s.append(1, ')');
    return s;
}

void ConstructorConstraintNode::AddParameter(ParameterNode* parameter)
{
    parameter->SetParent(this);
    parameters.Add(parameter);
}

DestructorConstraintNode::DestructorConstraintNode(const soul::ast::Span& span_) :
    SignatureConstraintNode(NodeType::destructorConstraintNode, span_), typeParamId()
{
}

DestructorConstraintNode::DestructorConstraintNode(const soul::ast::Span& span_, IdentifierNode* typeParamId_) :
    SignatureConstraintNode(NodeType::destructorConstraintNode, span_), typeParamId(typeParamId_)
{
    typeParamId->SetParent(this);
}

Node* DestructorConstraintNode::Clone(CloneContext& cloneContext) const
{
    DestructorConstraintNode* clone = new DestructorConstraintNode(GetSpan(), static_cast<IdentifierNode*>(typeParamId->Clone(cloneContext)));
    return clone;
}

void DestructorConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void DestructorConstraintNode::Write(AstWriter& writer)
{
    SignatureConstraintNode::Write(writer);
    writer.Write(typeParamId.get());
}

void DestructorConstraintNode::Read(AstReader& reader)
{
    SignatureConstraintNode::Read(reader);
    typeParamId.reset(reader.ReadIdentifierNode());
    typeParamId->SetParent(this);
}

std::string DestructorConstraintNode::ToString() const
{
    std::string s = "~" + typeParamId->ToString();
    s.append("()");
    return s;
}

MemberFunctionConstraintNode::MemberFunctionConstraintNode(const soul::ast::Span& span_) :
    SignatureConstraintNode(NodeType::memberFunctionConstraintNode, span_), returnTypeExpr(), typeParamId(), groupId(), parameters()
{
}

MemberFunctionConstraintNode::MemberFunctionConstraintNode(const soul::ast::Span& span_, Node* returnTypeExpr_, IdentifierNode* typeParamId_, const std::u32string& groupId_) :
    SignatureConstraintNode(NodeType::memberFunctionConstraintNode, span_), returnTypeExpr(returnTypeExpr_), typeParamId(typeParamId_), groupId(groupId_), parameters()
{
    returnTypeExpr->SetParent(this);
    typeParamId->SetParent(this);
}

Node* MemberFunctionConstraintNode::Clone(CloneContext& cloneContext) const
{
    MemberFunctionConstraintNode* clone = new MemberFunctionConstraintNode(GetSpan(), returnTypeExpr->Clone(cloneContext), static_cast<IdentifierNode*>(typeParamId->Clone(cloneContext)), groupId);
    int n = parameters.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddParameter(static_cast<ParameterNode*>(parameters[i]->Clone(cloneContext)));
    }
    return clone;
}

void MemberFunctionConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void MemberFunctionConstraintNode::Write(AstWriter& writer)
{
    SignatureConstraintNode::Write(writer);
    writer.Write(returnTypeExpr.get());
    writer.Write(typeParamId.get());
    writer.GetBinaryStreamWriter().Write(groupId);
    parameters.Write(writer);
}

void MemberFunctionConstraintNode::Read(AstReader& reader)
{
    SignatureConstraintNode::Read(reader);
    returnTypeExpr.reset(reader.ReadNode());
    returnTypeExpr->SetParent(this);
    typeParamId.reset(reader.ReadIdentifierNode());
    typeParamId->SetParent(this);
    groupId = reader.GetBinaryStreamReader().ReadUtf32String();
    parameters.Read(reader);
    parameters.SetParent(this);
}

void MemberFunctionConstraintNode::AddParameter(ParameterNode* parameter)
{
    parameter->SetParent(this);
    parameters.Add(parameter);
}

std::string MemberFunctionConstraintNode::ToString() const
{
    std::string s;
    if (returnTypeExpr)
    {
        s.append(returnTypeExpr->ToString()).append(" ");
    }
    s.append(typeParamId->ToString());
    s.append(".").append(util::ToUtf8(groupId));
    s.append(1, '(');
    int n = parameters.Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            s.append(", ");
        }
        ParameterNode* p = parameters[i];
        s.append(p->TypeExpr()->ToString());
    }
    s.append(1, ')');
    return s;
}

FunctionConstraintNode::FunctionConstraintNode(const soul::ast::Span& span_) :
    SignatureConstraintNode(NodeType::functionConstraintNode, span_), returnTypeExpr(), groupId(), parameters()
{
}

FunctionConstraintNode::FunctionConstraintNode(const soul::ast::Span& span_, Node* returnTypeExpr_, const std::u32string& groupId_)
    : SignatureConstraintNode(NodeType::functionConstraintNode, span_), returnTypeExpr(returnTypeExpr_), groupId(groupId_), parameters()
{
    returnTypeExpr->SetParent(this);
}

Node* FunctionConstraintNode::Clone(CloneContext& cloneContext) const
{
    FunctionConstraintNode* clone = new FunctionConstraintNode(GetSpan(), returnTypeExpr->Clone(cloneContext), groupId);
    int n = parameters.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddParameter(static_cast<ParameterNode*>(parameters[i]->Clone(cloneContext)));
    }
    return clone;
}

void FunctionConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionConstraintNode::Write(AstWriter& writer)
{
    SignatureConstraintNode::Write(writer);
    writer.Write(returnTypeExpr.get());
    writer.GetBinaryStreamWriter().Write(groupId);
    parameters.Write(writer);
}

void FunctionConstraintNode::Read(AstReader& reader)
{
    SignatureConstraintNode::Read(reader);
    returnTypeExpr.reset(reader.ReadNode());
    returnTypeExpr->SetParent(this);
    groupId = reader.GetBinaryStreamReader().ReadUtf32String();
    parameters.Read(reader);
    parameters.SetParent(this);
}

void FunctionConstraintNode::AddParameter(ParameterNode* parameter)
{
    parameter->SetParent(this);
    parameters.Add(parameter);
}

std::string FunctionConstraintNode::ToString() const
{
    std::string s;
    if (returnTypeExpr)
    {
        s.append(returnTypeExpr->ToString()).append(" ");
    }
    s.append(util::ToUtf8(groupId));
    s.append(1, '(');
    int n = parameters.Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            s.append(", ");
        }
        ParameterNode* p = parameters[i];
        s.append(p->TypeExpr()->ToString());
    }
    s.append(1, ')');
    return s;
}

AxiomStatementNode::AxiomStatementNode(const soul::ast::Span& span_) : Node(NodeType::axiomStatementNode, span_), expression()
{
}

AxiomStatementNode::AxiomStatementNode(const soul::ast::Span& span_, Node* expression_) :
    Node(NodeType::axiomStatementNode, span_), expression(expression_)
{
    expression->SetParent(this);
}

Node* AxiomStatementNode::Clone(CloneContext& cloneContext) const
{
    AxiomStatementNode* clone = new AxiomStatementNode(GetSpan(), expression->Clone(cloneContext));
    return clone;
}

void AxiomStatementNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AxiomStatementNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(expression.get());
}

void AxiomStatementNode::Read(AstReader& reader)
{
    Node::Read(reader);
    expression.reset(reader.ReadNode());
    expression->SetParent(this);
}

std::string AxiomStatementNode::ToString() const
{
    return expression->ToString();
}

AxiomNode::AxiomNode(const soul::ast::Span& span_) : Node(NodeType::axiomNode, span_), id(), parameters(), statements()
{
}

AxiomNode::AxiomNode(const soul::ast::Span& span_, IdentifierNode* id_) :
    Node(NodeType::axiomNode, span_), id(id_), parameters(), statements()
{
    id->SetParent(this);
}

Node* AxiomNode::Clone(CloneContext& cloneContext) const
{
    AxiomNode* clone = new AxiomNode(GetSpan(), static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    int np = parameters.Count();
    for (int i = 0; i < np; ++i)
    {
        clone->AddParameter(static_cast<ParameterNode*>(parameters[i]->Clone(cloneContext)));
    }
    int ns = statements.Count();
    for (int i = 0; i < ns; ++i)
    {
        clone->AddStatement(static_cast<AxiomStatementNode*>(statements[i]->Clone(cloneContext)));
    }
    return clone;
}

void AxiomNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void AxiomNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(id.get());
    parameters.Write(writer);
    statements.Write(writer);
}

void AxiomNode::Read(AstReader& reader)
{
    Node::Read(reader);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    parameters.Read(reader);
    parameters.SetParent(this);
    statements.Read(reader);
    statements.SetParent(this);
}

void AxiomNode::AddParameter(ParameterNode* parameter)
{
    parameter->SetParent(this);
    parameters.Add(parameter);
}

void AxiomNode::AddStatement(AxiomStatementNode* statement)
{
    statement->SetParent(this);
    statements.Add(statement);
}

ConceptIdNode::ConceptIdNode(const soul::ast::Span& span_) : Node(NodeType::conceptIdNode, span_), id(), typeParameters()
{
}

ConceptIdNode::ConceptIdNode(const soul::ast::Span& span_, IdentifierNode* id_) : Node(NodeType::conceptIdNode, span_), id(id_), typeParameters()
{
    id->SetParent(this);
}

Node* ConceptIdNode::Clone(CloneContext& cloneContext) const
{
    ConceptIdNode* clone = new ConceptIdNode(GetSpan(), static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    int n = typeParameters.Count();
    for (int i = 0; i < n; ++i)
    {
        clone->AddTypeParameter(typeParameters[i]->Clone(cloneContext));
    }
    return clone;
}

void ConceptIdNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConceptIdNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(id.get());
    typeParameters.Write(writer);
}

void ConceptIdNode::Read(AstReader& reader)
{
    Node::Read(reader);
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    typeParameters.Read(reader);
    typeParameters.SetParent(this);
}

void ConceptIdNode::AddTypeParameter(Node* typeParameter)
{
    typeParameter->SetParent(this);
    typeParameters.Add(typeParameter);
}

std::string ConceptIdNode::ToString() const
{
    std::string s = id->ToString();
    s.append(1, '<');
    int n = typeParameters.Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            s.append(", ");
        }
        s.append(typeParameters[i]->ToString());
    }
    s.append(1, '>');
    return s;
}

ConceptNode::ConceptNode(const soul::ast::Span& span_) : Node(NodeType::conceptNode, span_), specifiers(Specifiers::none), id(), typeParameters(), refinement(), constraints(), axioms()
{
}

ConceptNode::ConceptNode(NodeType nodeType_, const soul::ast::Span& span_) : Node(nodeType_, span_), specifiers(Specifiers::none), id(), typeParameters(), refinement(), constraints(), axioms()
{
}

ConceptNode::ConceptNode(const soul::ast::Span& span_, Specifiers specifiers_, IdentifierNode* id_) :
    Node(NodeType::conceptNode, span_), specifiers(specifiers_), id(id_), typeParameters(), refinement(), constraints(), axioms()
{
    id->SetParent(this);
}

ConceptNode::ConceptNode(NodeType nodeType_, const soul::ast::Span& span_, Specifiers specifiers_, IdentifierNode* id_) :
    Node(nodeType_, span_), specifiers(specifiers_), id(id_), typeParameters(), refinement(), constraints(), axioms()
{
    id->SetParent(this);
}

Node* ConceptNode::Clone(CloneContext& cloneContext) const
{
    ConceptNode* clone = new ConceptNode(GetSpan(), specifiers, static_cast<IdentifierNode*>(id->Clone(cloneContext)));
    int nt = typeParameters.Count();
    for (int i = 0; i < nt; ++i)
    {
        clone->AddTypeParameter(static_cast<IdentifierNode*>(typeParameters[i]->Clone(cloneContext)));
    }
    if (refinement)
    {
        clone->SetRefinement(static_cast<ConceptIdNode*>(refinement->Clone(cloneContext)));
    }
    int nc = constraints.Count();
    for (int i = 0; i < nc; ++i)
    {
        clone->AddConstraint(static_cast<ConstraintNode*>(constraints[i]->Clone(cloneContext)));
    }
    int na = axioms.Count();
    for (int i = 0; i < na; ++i)
    {
        clone->AddAxiom(static_cast<AxiomNode*>(axioms[i]->Clone(cloneContext)));
    }
    return clone;
}

void ConceptNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ConceptNode::Write(AstWriter& writer)
{
    Node::Write(writer);
    writer.Write(specifiers);
    writer.Write(id.get());
    typeParameters.Write(writer);
    bool hasRefinement = refinement != nullptr;
    writer.GetBinaryStreamWriter().Write(hasRefinement);
    if (hasRefinement)
    {
        writer.Write(refinement.get());
    }
    constraints.Write(writer);
    axioms.Write(writer);
}

void ConceptNode::Read(AstReader& reader)
{
    Node::Read(reader);
    specifiers = reader.ReadSpecifiers();
    id.reset(reader.ReadIdentifierNode());
    id->SetParent(this);
    typeParameters.Read(reader);
    typeParameters.SetParent(this);
    bool hasRefinement = reader.GetBinaryStreamReader().ReadBool();
    if (hasRefinement)
    {
        refinement.reset(reader.ReadConceptIdNode());
        refinement->SetParent(this);
    }
    constraints.Read(reader);
    constraints.SetParent(this);
    axioms.Read(reader);
    axioms.SetParent(this);
}

void ConceptNode::AddTypeParameter(IdentifierNode* typeParameter)
{
    typeParameter->SetParent(this);
    typeParameters.Add(typeParameter);
}

void ConceptNode::SetRefinement(ConceptIdNode* refinement_)
{
    refinement.reset(refinement_);
    refinement->SetParent(this);
}

void ConceptNode::AddConstraint(ConstraintNode* constraint)
{
    constraint->SetParent(this);
    constraints.Add(constraint);
}

void ConceptNode::AddAxiom(AxiomNode* axiom_)
{
    axiom_->SetParent(this);
    axioms.Add(axiom_);
}

IntrinsicConstraintNode::IntrinsicConstraintNode(NodeType nodeType_) : ConstraintNode(nodeType_, soul::ast::Span())
{
}

IntrinsicConstraintNode::IntrinsicConstraintNode(NodeType nodeType_, const soul::ast::Span& span_) : ConstraintNode(nodeType_, span_)
{
}

SameConstraintNode::SameConstraintNode() : IntrinsicConstraintNode(NodeType::sameConstraintNode)
{
}

SameConstraintNode::SameConstraintNode(const soul::ast::Span& span_) : IntrinsicConstraintNode(NodeType::sameConstraintNode, span_)
{
}

void SameConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Node* SameConstraintNode::Clone(CloneContext& cloneContext) const
{
    return new SameConstraintNode();
}

DerivedConstraintNode::DerivedConstraintNode() : IntrinsicConstraintNode(NodeType::derivedConstraintNode)
{
}

DerivedConstraintNode::DerivedConstraintNode(const soul::ast::Span& span_) : IntrinsicConstraintNode(NodeType::derivedConstraintNode, span_)
{
}

void DerivedConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Node* DerivedConstraintNode::Clone(CloneContext& cloneContext) const
{
    return new DerivedConstraintNode();
}

ConvertibleConstraintNode::ConvertibleConstraintNode() : IntrinsicConstraintNode(NodeType::convertibleConstraintNode)
{
}

ConvertibleConstraintNode::ConvertibleConstraintNode(const soul::ast::Span& span_) : IntrinsicConstraintNode(NodeType::convertibleConstraintNode, span_)
{
}

void ConvertibleConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Node* ConvertibleConstraintNode::Clone(CloneContext& cloneContext) const
{
    return new ConvertibleConstraintNode();
}

ExplicitlyConvertibleConstraintNode::ExplicitlyConvertibleConstraintNode() : IntrinsicConstraintNode(NodeType::explicitlyConvertibleConstraintNode)
{
}

ExplicitlyConvertibleConstraintNode::ExplicitlyConvertibleConstraintNode(const soul::ast::Span& span_) : IntrinsicConstraintNode(NodeType::explicitlyConvertibleConstraintNode, span_)
{
}

void ExplicitlyConvertibleConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Node* ExplicitlyConvertibleConstraintNode::Clone(CloneContext& cloneContext) const
{
    return new ExplicitlyConvertibleConstraintNode();
}

CommonConstraintNode::CommonConstraintNode() : IntrinsicConstraintNode(NodeType::commonConstraintNode)
{
}

CommonConstraintNode::CommonConstraintNode(const soul::ast::Span& span_) : IntrinsicConstraintNode(NodeType::commonConstraintNode, span_)
{
}

void CommonConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Node* CommonConstraintNode::Clone(CloneContext& cloneContext) const
{
    return new CommonConstraintNode();
}

NonreferenceTypeConstraintNode::NonreferenceTypeConstraintNode() : IntrinsicConstraintNode(NodeType::nonreferenceTypeConstraintNode)
{
}

NonreferenceTypeConstraintNode::NonreferenceTypeConstraintNode(const soul::ast::Span& span_) : IntrinsicConstraintNode(NodeType::nonreferenceTypeConstraintNode, span_)
{
}

void NonreferenceTypeConstraintNode::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

Node* NonreferenceTypeConstraintNode::Clone(CloneContext& cloneContext) const
{
    return new NonreferenceTypeConstraintNode();
}

SameConceptNode::SameConceptNode(const soul::ast::Span& rootSpan, bool init) : ConceptNode(NodeType::sameConceptNode, rootSpan, Specifiers::public_, 
    new IdentifierNode(rootSpan, U"Same"))
{
    AddTypeParameter(new IdentifierNode(rootSpan, U"T"));
    AddTypeParameter(new IdentifierNode(rootSpan, U"U"));
    AddConstraint(new SameConstraintNode(rootSpan));
}

SameConceptNode::SameConceptNode(const soul::ast::Span& span_) : ConceptNode(NodeType::sameConceptNode, span_)
{
}

DerivedConceptNode::DerivedConceptNode(const soul::ast::Span& rootSpan, bool init) : ConceptNode(NodeType::derivedConceptNode, rootSpan, Specifiers::public_, 
    new IdentifierNode(rootSpan, U"Derived"))
{
    AddTypeParameter(new IdentifierNode(rootSpan, U"T"));
    AddTypeParameter(new IdentifierNode(rootSpan, U"U"));
    AddConstraint(new DerivedConstraintNode(rootSpan));
}

DerivedConceptNode::DerivedConceptNode(const soul::ast::Span& span_) : ConceptNode(NodeType::derivedConceptNode, span_)
{
}

ConvertibleConceptNode::ConvertibleConceptNode(const soul::ast::Span& rootSpan, bool init) : 
    ConceptNode(NodeType::convertibleConceptNode, rootSpan, Specifiers::public_, new IdentifierNode(rootSpan, U"Convertible"))
{
    AddTypeParameter(new IdentifierNode(rootSpan, U"T"));
    AddTypeParameter(new IdentifierNode(rootSpan, U"U"));
    AddConstraint(new ConvertibleConstraintNode(rootSpan));
}

ConvertibleConceptNode::ConvertibleConceptNode(const soul::ast::Span& span_) : ConceptNode(NodeType::convertibleConceptNode, span_)
{
}

ExplicitlyConvertibleConceptNode::ExplicitlyConvertibleConceptNode(const soul::ast::Span& rootSpan, bool init) :
    ConceptNode(NodeType::explicitlyConvertibleConceptNode, rootSpan, Specifiers::public_, new IdentifierNode(rootSpan, U"ExplicitlyConvertible"))
{
    AddTypeParameter(new IdentifierNode(rootSpan, U"T"));
    AddTypeParameter(new IdentifierNode(rootSpan, U"U"));
    AddConstraint(new ExplicitlyConvertibleConstraintNode(rootSpan));
}

ExplicitlyConvertibleConceptNode::ExplicitlyConvertibleConceptNode(const soul::ast::Span& span_) :
    ConceptNode(NodeType::explicitlyConvertibleConceptNode, span_)
{
}

CommonConceptNode::CommonConceptNode(const soul::ast::Span& rootSpan, bool init) :
    ConceptNode(NodeType::commonConceptNode, rootSpan, Specifiers::public_, new IdentifierNode(rootSpan, U"Common"))
{
    AddTypeParameter(new IdentifierNode(rootSpan, U"T"));
    AddTypeParameter(new IdentifierNode(rootSpan, U"U"));
    AddConstraint(new CommonConstraintNode(rootSpan));
}

CommonConceptNode::CommonConceptNode(const soul::ast::Span& span_) : ConceptNode(NodeType::commonConceptNode, span_)
{
}

NonreferenceTypeConceptNode::NonreferenceTypeConceptNode(const soul::ast::Span& rootSpan, bool init) :
    ConceptNode(NodeType::nonreferenceTypeConceptNode, rootSpan, Specifiers::public_, new IdentifierNode(rootSpan, U"NonreferenceType"))
{
    AddTypeParameter(new IdentifierNode(rootSpan, U"T"));
    AddConstraint(new NonreferenceTypeConstraintNode(rootSpan));
}

NonreferenceTypeConceptNode::NonreferenceTypeConceptNode(const soul::ast::Span& span_) : ConceptNode(NodeType::nonreferenceTypeConceptNode, span_)
{
}

} // namespace cmajor::ast
