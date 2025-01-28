// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.binder.concept_binder;

import cmajor.fault.tolerant.binder.expression.binder;
import cmajor.fault.tolerant.binder.bound.compile.unit;
import cmajor.fault.tolerant.binder.type.resolver;

namespace cmajor::fault::tolerant::binder {

ConceptBinder::ConceptBinder(
    BoundCompileUnit* boundCompileUnit_,
    cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_,
    cmajor::fault::tolerant::symbols::ContainerScope* scope_) :
    boundCompileUnit(boundCompileUnit_), symbolTable(symbolTable_), scope(scope_), symbol(nullptr)
{
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::DisjunctiveConstraintNode& node)
{
    node.Left()->Accept(*this);
    node.Right()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::ConjunctiveConstraintNode& node)
{
    node.Left()->Accept(*this);
    node.Right()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::ParenthesizedConstraintNode& node)
{
    node.ConstraintExpr()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::PredicateConstraintNode& node)
{
    BindExpression(node.InvokeExpr(), boundCompileUnit, symbolTable, scope);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::IsConstraintNode& node)
{
    node.TypeExpr()->Accept(*this);
    node.ConceptOrTypeName()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::MultiparamConstraintNode& node)
{
    node.ConceptName()->Accept(*this);
    node.TypeExprList()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::TypeNameConstraintNode& node)
{
    node.TypeExpr()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::ConstructorConstraintNode& node)
{
    node.TypeParamId()->Accept(*this);
    node.ParameterList()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::DestructorConstraintNode& node)
{
    node.TypeParamId()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::MemberFunctionConstraintNode& node)
{
    node.TypeParamId()->Accept(*this);
    node.FunctionGroupId()->Accept(*this);
    node.ParameterList()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::FunctionConstraintNode& node)
{
    node.ReturnType()->Accept(*this);
    node.FunctionGroupId()->Accept(*this);
    node.ParameterList()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::WhereConstraintNode& node)
{
    node.ConstraintExpr()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::EmbeddedConstraintNode& node)
{
    node.WhereConstraint()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::AxiomStatementNode& node)
{
    node.Expression()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::AxiomNode& node)
{
    if (node.AxiomId())
    {
        node.AxiomId()->Accept(*this);
    }
    if (node.ParameterList())
    {
        node.ParameterList()->Accept(*this);
    }
    node.AxiomStatements()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::ConceptNode& node)
{
    node.ConceptId()->Accept(*this);
    node.TypeParameterList()->Accept(*this);
    if (node.Refinement())
    {
        node.Refinement()->Accept(*this);
    }
    if (node.WhereConstraint())
    {
        node.WhereConstraint()->Accept(*this);
    }
    node.BodyConstraints()->Accept(*this);
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::ConceptIdNode& node)
{
    cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
    node.RefinedConcept()->Accept(*this);
    if (symbol)
    {
        scope = symbol->Scope();
    }
    node.TypeParameterList()->Accept(*this);
    scope = prevScope;
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::IdentifierNode& node)
{
    symbol = scope->Lookup(&node,
        cmajor::fault::tolerant::symbols::ScopeLookup::this_and_base_and_parent | cmajor::fault::tolerant::symbols::ScopeLookup::map,
        symbolTable);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::fault::tolerant::symbols::FileScope>& fileScope : boundCompileUnit->FileScopes())
        {
            symbol = fileScope->Lookup(&node,
                cmajor::fault::tolerant::symbols::ScopeLookup::this_ | cmajor::fault::tolerant::symbols::ScopeLookup::map,
                symbolTable);
            if (symbol)
            {
                break;
            }
        }
    }
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::QualifiedIdNode& node)
{
    symbol = scope->Lookup(&node,
        cmajor::fault::tolerant::symbols::ScopeLookup::this_and_base_and_parent | cmajor::fault::tolerant::symbols::ScopeLookup::map,
        symbolTable);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::fault::tolerant::symbols::FileScope>& fileScope : boundCompileUnit->FileScopes())
        {
            symbol = fileScope->Lookup(&node,
                cmajor::fault::tolerant::symbols::ScopeLookup::this_ | cmajor::fault::tolerant::symbols::ScopeLookup::map,
                symbolTable);
            if (symbol)
            {
                break;
            }
        }
    }
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::DotMemberNode& node)
{
    cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
    node.Left()->Accept(*this);
    if (symbol)
    {
        if (symbol->IsNamespaceSymbol() || symbol->IsEnumTypeSymbol() || symbol->IsClassTypeSymbol() || symbol->IsArrayTypeSymbol())
        {
            scope = symbol->Scope();
        }
        else
        {
            cmajor::fault::tolerant::symbols::TypeSymbol* type = symbol->GetType();
            if (type)
            {
                scope = type->BaseType()->Scope();
            }
        }
    }
    node.Right()->Accept(*this);
    scope = prevScope;
}

void ConceptBinder::Visit(cmajor::fault::tolerant::ast::TemplateIdNode& node)
{
    symbol = ResolveType(&node, scope, symbolTable, boundCompileUnit);
}

void BindConstraint(cmajor::fault::tolerant::ast::Node* constraint, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable,
    cmajor::fault::tolerant::symbols::ContainerScope* scope)
{
    ConceptBinder binder(boundCompileUnit, symbolTable, scope);
    constraint->Accept(binder);
}

void BindConcept(cmajor::fault::tolerant::ast::ConceptNode* concept_, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable,
    cmajor::fault::tolerant::symbols::ContainerScope* scope)
{
    ConceptBinder binder(boundCompileUnit, symbolTable, scope);
    concept_->Accept(binder);
}

} // namespace cmajor::fault::tolerant::binder
