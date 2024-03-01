// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.binder.statement.binder;

import cmajor.fault.tolerant.binder.expression.binder;
import cmajor.fault.tolerant.binder.concept_binder;
import cmajor.fault.tolerant.binder.type.resolver;

namespace cmajor::fault::tolerant::binder {

StatementBinder::StatementBinder(BoundCompileUnit* boundCompileUnit_, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_) : 
    boundCompileUnit(boundCompileUnit_), symbolTable(symbolTable_), scope(symbolTable->GlobalNs()->Scope())
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::CompileUnitNode& node)
{
    node.GlobalNs()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::NamespaceDefinitionNode& node)
{
    cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol)
    {
        scope = symbol->Scope();
    }
    node.Content().Accept(*this);
    scope = prevScope;
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::NamespaceImportNode& node)
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::UsingAliasNode& node)
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::DelegateNode& node)
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ClassDelegateNode& node)
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::InitializerListNode& node)
{
    for (const auto& initializer : node.Initializers())
    {
        initializer->Accept(*this);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ThisInitializerNode& node)
{
    for (const auto& arg : node.ArgumentList().Nodes())
    {
        BindExpression(arg.get(), boundCompileUnit, symbolTable, scope);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::BaseInitializerNode& node)
{
    for (const auto& arg : node.ArgumentList().Nodes())
    {
        BindExpression(arg.get(), boundCompileUnit, symbolTable, scope);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::MemberInitializerNode& node)
{
    BindExpression(node.Id(), boundCompileUnit, symbolTable, scope);
    for (const auto& arg : node.ArgumentList().Nodes())
    {
        BindExpression(arg.get(), boundCompileUnit, symbolTable, scope);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::FunctionNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsFunctionSymbol())
    {
        cmajor::fault::tolerant::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::fault::tolerant::symbols::FunctionSymbol*>(symbol);
        cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
        scope = functionSymbol->Scope();
        if (node.Constraint())
        {
            BindConstraint(node.Constraint(), boundCompileUnit, symbolTable, scope);
        }
        if (node.Body())
        {
            node.Body()->Accept(*this);
        }
        scope = prevScope;
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::StaticConstructorNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsStaticConstructorSymbol())
    {
        cmajor::fault::tolerant::symbols::StaticConstructorSymbol* staticConstructorSymbol = static_cast<cmajor::fault::tolerant::symbols::StaticConstructorSymbol*>(symbol);
        cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
        scope = staticConstructorSymbol->Scope();
        node.InitializerList()->Accept(*this);
        if (node.Constraint())
        {
            BindConstraint(node.Constraint(), boundCompileUnit, symbolTable, scope);
        }
        if (node.Body())
        {
            node.Body()->Accept(*this);
        }
        scope = prevScope;
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ConstructorNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsConstructorSymbol())
    {
        cmajor::fault::tolerant::symbols::ConstructorSymbol* constructorSymbol = static_cast<cmajor::fault::tolerant::symbols::ConstructorSymbol*>(symbol);
        cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
        scope = constructorSymbol->Scope();
        node.InitializerList()->Accept(*this);
        if (node.Constraint())
        {
            BindConstraint(node.Constraint(), boundCompileUnit, symbolTable, scope);
        }
        if (node.Body())
        {
            node.Body()->Accept(*this);
        }
        scope = prevScope;
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::DestructorNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsDestructorSymbol())
    {
        cmajor::fault::tolerant::symbols::DestructorSymbol* destructorSymbol = static_cast<cmajor::fault::tolerant::symbols::DestructorSymbol*>(symbol);
        cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
        scope = destructorSymbol->Scope();
        if (node.Constraint())
        {
            BindConstraint(node.Constraint(), boundCompileUnit, symbolTable, scope);
        }
        if (node.Body())
        {
            node.Body()->Accept(*this);
        }
        scope = prevScope;
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::MemberFunctionNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsMemberFunctionSymbol())
    {
        cmajor::fault::tolerant::symbols::MemberFunctionSymbol* memberFunctionSymbol = static_cast<cmajor::fault::tolerant::symbols::MemberFunctionSymbol*>(symbol);
        cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
        scope = memberFunctionSymbol->Scope();
        if (node.Constraint())
        {
            BindConstraint(node.Constraint(), boundCompileUnit, symbolTable, scope);
        }
        if (node.Body())
        {
            node.Body()->Accept(*this);
        }
        scope = prevScope;
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ConversionFunctionNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsConversionFunctionSymbol())
    {
        cmajor::fault::tolerant::symbols::ConversionFunctionSymbol* conversionFunctionSymbol = static_cast<cmajor::fault::tolerant::symbols::ConversionFunctionSymbol*>(symbol);
        cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
        scope = conversionFunctionSymbol->Scope();
        if (node.Constraint())
        {
            BindConstraint(node.Constraint(), boundCompileUnit, symbolTable, scope);
        }
        if (node.Body())
        {
            node.Body()->Accept(*this);
        }
        scope = prevScope;
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::CompoundStatementNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsDeclarationBlockSymbol())
    {
        cmajor::fault::tolerant::symbols::DeclarationBlockSymbol* declarationBlockSymbol = static_cast<cmajor::fault::tolerant::symbols::DeclarationBlockSymbol*>(symbol);
        cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
        scope = declarationBlockSymbol->Scope();
        node.Statements()->Accept(*this);
        scope = prevScope;
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ReturnStatementNode& node)
{
    if (node.ReturnValue())
    {
        BindExpression(node.ReturnValue(), boundCompileUnit, symbolTable, scope);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::IfStatementNode& node)
{
    BindExpression(node.Condition(), boundCompileUnit, symbolTable, scope);
    node.ThenStatement()->Accept(*this);
    if (node.ElseStatement())
    {
        node.ElseStatement()->Accept(*this);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::WhileStatementNode& node)
{
    BindExpression(node.Condition(), boundCompileUnit, symbolTable, scope);
    node.Statement()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::DoStatementNode& node)
{
    node.Statement()->Accept(*this);
    BindExpression(node.Condition(), boundCompileUnit, symbolTable, scope);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::RangeForStatementNode& node)
{
    BindExpression(node.TypeExpr(), boundCompileUnit, symbolTable, scope);
    BindExpression(node.Container(), boundCompileUnit, symbolTable, scope);
    node.Statement()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ForStatementNode& node)
{
    if (node.Condition())
    {
        BindExpression(node.Condition(), boundCompileUnit, symbolTable, scope);
    }
    node.InitStatement()->Accept(*this);
    node.LoopStatement()->Accept(*this);
    node.Statement()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::BreakStatementNode& node)
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ContinueStatementNode& node)
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::GotoStatementNode& node)
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::GotoCaseStatementNode& node)
{
    // todo: evaluate node.CaseExpr()
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::GotoDefaultStatementNode& node)
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::CaseStatementNode& node)
{
    // todo: evaluate each case expr
    node.Statements()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::DefaultStatementNode& node)
{
    node.Statements()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::SwitchStatementNode& node)
{
    BindExpression(node.Condition(), boundCompileUnit, symbolTable, scope);
    node.CaseStatements()->Accept(*this);
    if (node.DefaultStatement())
    {
        node.DefaultStatement()->Accept(*this);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::AssignmentStatementNode& node)
{
    BindExpression(node.Left(), boundCompileUnit, symbolTable, scope);
    BindExpression(node.Right(), boundCompileUnit, symbolTable, scope);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ExpressionStatementNode& node)
{
    BindExpression(node.Expression(), boundCompileUnit, symbolTable, scope);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::EmptyStatementNode& node)
{
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ConstructionStatementNode& node)
{
    for (const auto& arg : node.ArgumentList()->Nodes())
    {
        BindExpression(arg.get(), boundCompileUnit, symbolTable, scope);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::DeleteStatementNode& node)
{
    BindExpression(node.Ptr(), boundCompileUnit, symbolTable, scope);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::DestroyStatementNode& node)
{
    BindExpression(node.Ptr(), boundCompileUnit, symbolTable, scope);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ThrowStatementNode& node)
{
    if (node.Exception())
    {
        BindExpression(node.Exception(), boundCompileUnit, symbolTable, scope);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::CatchNode& node)
{
    cmajor::fault::tolerant::symbols::TypeSymbol* catchType = ResolveType(node.TypeExpr(), scope, symbolTable, boundCompileUnit);
    node.CatchBlock()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::TryStatementNode& node)
{
    node.TryBlock()->Accept(*this);
    for (const auto& catchNode : node.Catches()->Nodes())
    {
        catchNode->Accept(*this);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::LabeledStatementNode& node)
{
    node.Statement()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::AssertStatementNode& node)
{
    BindExpression(node.Expression(), boundCompileUnit, symbolTable, scope);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::CondCompStatementNode& node)
{
    node.IfGroup()->Accept(*this);
    for (const auto& elifGroup : node.ElifGroups()->Nodes())
    {
        elifGroup->Accept(*this);
    }
    if (node.ElseGroup())
    {
        node.ElseGroup()->Accept(*this);
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::IfGroupNode& node)
{
    node.Statements()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ElifGroupNode& node)
{
    node.Statements()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ElseGroupNode& node)
{
    node.Statements()->Accept(*this);
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ClassNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsClassTypeSymbol())
    {
        cmajor::fault::tolerant::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::fault::tolerant::symbols::ClassTypeSymbol*>(symbol);
        cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
        scope = classTypeSymbol->Scope();
        if (node.Constraint())
        {
            BindConstraint(node.Constraint(), boundCompileUnit, symbolTable, scope);
        }
        node.Members().Accept(*this);
        scope = prevScope;
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::InterfaceNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsInterfaceTypeSymbol())
    {
        cmajor::fault::tolerant::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::fault::tolerant::symbols::InterfaceTypeSymbol*>(symbol);
        if (!interfaceTypeSymbol->IsBound())
        {
            interfaceTypeSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = interfaceTypeSymbol->Scope();
            node.MemberFunctions()->Accept(*this);
            scope = prevScope;
        }
    }
}

void StatementBinder::Visit(cmajor::fault::tolerant::ast::ConceptNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsConceptSymbol())
    {
        cmajor::fault::tolerant::symbols::ConceptSymbol* conceptSymbol = static_cast<cmajor::fault::tolerant::symbols::ConceptSymbol*>(symbol);
        if (!conceptSymbol->IsBound())
        {
            conceptSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = conceptSymbol->Scope();
            BindConcept(&node, boundCompileUnit, symbolTable, scope);
            scope = prevScope;
        }
    }
}

void BindStatements(cmajor::fault::tolerant::ast::CompileUnitNode* node, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable)
{
    StatementBinder binder(boundCompileUnit, symbolTable);
    node->Accept(binder);
}

} // namespace cmajor::fault::tolerant::binder
