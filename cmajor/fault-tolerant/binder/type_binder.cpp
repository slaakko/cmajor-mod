// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.binder.type.binder;

import cmajor.fault.tolerant.binder.bound.compile.unit;
import cmajor.fault.tolerant.binder.type.resolver;

namespace cmajor::fault::tolerant::binder {

TypeBinder::TypeBinder(BoundCompileUnit* boundCompileUnit_, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_) : 
    boundCompileUnit(boundCompileUnit_), symbolTable(symbolTable_), scope(symbolTable->GlobalNs()->Scope())
{
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::CompileUnitNode& node)
{
    boundCompileUnit->AddFileScope(new cmajor::fault::tolerant::symbols::FileScope());
    DefaultVisitor::Visit(node);
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::NamespaceDefinitionNode& node)
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

void TypeBinder::Visit(cmajor::fault::tolerant::ast::NamespaceImportNode& node)
{
    boundCompileUnit->FirstFileScope()->InstallNamespaceImport(scope, &node, symbolTable);
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::UsingAliasNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsAliasTypeSymbol())
    {
        cmajor::fault::tolerant::symbols::AliasTypeSymbol* aliasTypeSymbol = static_cast<cmajor::fault::tolerant::symbols::AliasTypeSymbol*>(symbol);
        if (!aliasTypeSymbol->IsBound())
        {
            aliasTypeSymbol->SetBound();
            cmajor::fault::tolerant::symbols::TypeSymbol* type = ResolveType(node.TypeExpr(), scope, symbolTable, boundCompileUnit);
            if (type)
            {
                aliasTypeSymbol->SetType(type);
                boundCompileUnit->FirstFileScope()->InstallAlias(&node, aliasTypeSymbol->GetType());
            }
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::TypedefAliasNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsAliasTypeSymbol())
    {
        cmajor::fault::tolerant::symbols::AliasTypeSymbol* aliasTypeSymbol = static_cast<cmajor::fault::tolerant::symbols::AliasTypeSymbol*>(symbol);
        if (!aliasTypeSymbol->IsBound())
        {
            aliasTypeSymbol->SetBound();
            cmajor::fault::tolerant::symbols::TypeSymbol* type = ResolveType(node.TypeExpr(), scope, symbolTable, boundCompileUnit);
            if (type)
            {
                aliasTypeSymbol->SetType(type);
            }
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::StaticConstructorNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsStaticConstructorSymbol())
    {
        cmajor::fault::tolerant::symbols::StaticConstructorSymbol* staticConstructorSymbol = static_cast<cmajor::fault::tolerant::symbols::StaticConstructorSymbol*>(symbol);
        if (!staticConstructorSymbol->IsBound())
        {
            staticConstructorSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = staticConstructorSymbol->Scope();
            if (node.Body())
            {
                node.Body()->Accept(*this);
            }
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::ConstructorNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsConstructorSymbol())
    {
        cmajor::fault::tolerant::symbols::ConstructorSymbol* constructorSymbol = static_cast<cmajor::fault::tolerant::symbols::ConstructorSymbol*>(symbol);
        if (!constructorSymbol->IsBound())
        {
            constructorSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = constructorSymbol->Scope();
            int n = node.ParameterList()->ParameterCount();
            for (int i = 0; i < n; ++i)
            {
                cmajor::fault::tolerant::ast::ParameterNode* parameterNode = node.ParameterList()->GetParameter(i);
                cmajor::fault::tolerant::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), scope, symbolTable, boundCompileUnit);
                cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(parameterNode);
                if (symbol && symbol->IsParameterSymbol())
                {
                    cmajor::fault::tolerant::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::fault::tolerant::symbols::ParameterSymbol*>(symbol);
                    parameterSymbol->SetType(parameterType);
                }
            }
            if (node.Body())
            {
                node.Body()->Accept(*this);
            }
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::DestructorNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsDestructorSymbol())
    {
        cmajor::fault::tolerant::symbols::DestructorSymbol* destructorSymbol = static_cast<cmajor::fault::tolerant::symbols::DestructorSymbol*>(symbol);
        if (!destructorSymbol->IsBound())
        {
            destructorSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = destructorSymbol->Scope();
            if (node.Body())
            {
                node.Body()->Accept(*this);
            }
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::MemberFunctionNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsMemberFunctionSymbol())
    {
        cmajor::fault::tolerant::symbols::MemberFunctionSymbol* memberFunctionSymbol = static_cast<cmajor::fault::tolerant::symbols::MemberFunctionSymbol*>(symbol);
        if (!memberFunctionSymbol->IsBound())
        {
            memberFunctionSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = memberFunctionSymbol->Scope();
            cmajor::fault::tolerant::symbols::TypeSymbol* returnType = ResolveType(node.ReturnType(), scope, symbolTable, boundCompileUnit);
            if (returnType)
            {
                memberFunctionSymbol->SetReturnType(returnType);
            }
            int n = node.ParameterList()->ParameterCount();
            for (int i = 0; i < n; ++i)
            {
                cmajor::fault::tolerant::ast::ParameterNode* parameterNode = node.ParameterList()->GetParameter(i);
                cmajor::fault::tolerant::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), scope, symbolTable, boundCompileUnit);
                cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(parameterNode);
                if (symbol && symbol->IsParameterSymbol())
                {
                    cmajor::fault::tolerant::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::fault::tolerant::symbols::ParameterSymbol*>(symbol);
                    parameterSymbol->SetType(parameterType);
                }
            }
            if (node.Body())
            {
                node.Body()->Accept(*this);
            }
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::ConversionFunctionNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsConversionFunctionSymbol())
    {
        cmajor::fault::tolerant::symbols::ConversionFunctionSymbol* conversionFunctionSymbol = static_cast<cmajor::fault::tolerant::symbols::ConversionFunctionSymbol*>(symbol);
        if (!conversionFunctionSymbol->IsBound())
        {
            conversionFunctionSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = conversionFunctionSymbol->Scope();
            cmajor::fault::tolerant::symbols::TypeSymbol* returnType = ResolveType(node.Type(), scope, symbolTable, boundCompileUnit);
            if (returnType)
            {
                conversionFunctionSymbol->SetReturnType(returnType);
            }
            if (node.Body())
            {
                node.Body()->Accept(*this);
            }
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::MemberVariableNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsMemberVariableSymbol())
    {
        cmajor::fault::tolerant::symbols::MemberVariableSymbol* memberVariableSymbol = static_cast<cmajor::fault::tolerant::symbols::MemberVariableSymbol*>(symbol);
        if (!memberVariableSymbol->IsBound())
        {
            memberVariableSymbol->SetBound();
            cmajor::fault::tolerant::symbols::TypeSymbol* type = ResolveType(node.Type(), scope, symbolTable, boundCompileUnit);
            if (type)
            {
                memberVariableSymbol->SetType(type);
            }
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::ConstantNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsConstantSymbol())
    {
        cmajor::fault::tolerant::symbols::ConstantSymbol* constantSymbol = static_cast<cmajor::fault::tolerant::symbols::ConstantSymbol*>(symbol);
        if (!constantSymbol->IsBound())
        {
            constantSymbol->SetBound();
            cmajor::fault::tolerant::symbols::TypeSymbol* type = ResolveType(node.Type(), scope, symbolTable, boundCompileUnit);
            if (type)
            {
                constantSymbol->SetType(type);
            }
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::DelegateNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsDelegateSymbol())
    {
        cmajor::fault::tolerant::symbols::DelegateSymbol* delegateSymbol = static_cast<cmajor::fault::tolerant::symbols::DelegateSymbol*>(symbol);
        if (!delegateSymbol->IsBound())
        {
            delegateSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = delegateSymbol->Scope();
            cmajor::fault::tolerant::symbols::TypeSymbol* returnType = ResolveType(node.ReturnType(), scope, symbolTable, boundCompileUnit);
            if (returnType)
            {
                delegateSymbol->SetReturnType(returnType);
            }
            int n = node.ParameterList()->ParameterCount();
            for (int i = 0; i < n; ++i)
            {
                cmajor::fault::tolerant::ast::ParameterNode* parameterNode = node.ParameterList()->GetParameter(i);
                cmajor::fault::tolerant::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), scope, symbolTable, boundCompileUnit);
                cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(parameterNode);
                if (symbol && symbol->IsParameterSymbol())
                {
                    cmajor::fault::tolerant::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::fault::tolerant::symbols::ParameterSymbol*>(symbol);
                    parameterSymbol->SetType(parameterType);
                }
            }
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::ClassDelegateNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsClassDelegateSymbol())
    {
        cmajor::fault::tolerant::symbols::ClassDelegateSymbol* classDelegateSymbol = static_cast<cmajor::fault::tolerant::symbols::ClassDelegateSymbol*>(symbol);
        if (!classDelegateSymbol->IsBound())
        {
            classDelegateSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = classDelegateSymbol->Scope();
            cmajor::fault::tolerant::symbols::TypeSymbol* returnType = ResolveType(node.ReturnType(), scope, symbolTable, boundCompileUnit);
            if (returnType)
            {
                classDelegateSymbol->SetReturnType(returnType);
            }
            int n = node.ParameterList()->ParameterCount();
            for (int i = 0; i < n; ++i)
            {
                cmajor::fault::tolerant::ast::ParameterNode* parameterNode = node.ParameterList()->GetParameter(i);
                cmajor::fault::tolerant::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), scope, symbolTable, boundCompileUnit);
                cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(parameterNode);
                if (symbol && symbol->IsParameterSymbol())
                {
                    cmajor::fault::tolerant::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::fault::tolerant::symbols::ParameterSymbol*>(symbol);
                    parameterSymbol->SetType(parameterType);
                }
            }
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::EnumConstantNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsEnumConstantSymbol())
    {
        cmajor::fault::tolerant::symbols::EnumConstantSymbol* enumConstantSymbol = static_cast<cmajor::fault::tolerant::symbols::EnumConstantSymbol*>(symbol);
        if (!enumConstantSymbol->IsBound())
        {
            enumConstantSymbol->SetBound();
            // todo
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::EnumTypeNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsEnumTypeSymbol())
    {
        cmajor::fault::tolerant::symbols::EnumTypeSymbol* enumTypeSymbol = static_cast<cmajor::fault::tolerant::symbols::EnumTypeSymbol*>(symbol);
        if (!enumTypeSymbol->IsBound())
        {
            enumTypeSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = enumTypeSymbol->Scope();
            cmajor::fault::tolerant::symbols::TypeSymbol* underlyingType = ResolveType(node.UnderlyingType(), scope, symbolTable, boundCompileUnit);
            if (underlyingType)
            {
                enumTypeSymbol->SetUnderlyingType(underlyingType);
            }
            int n = node.EnumConstantList()->Count();
            for (int i = 0; i < n; ++i)
            {
                cmajor::fault::tolerant::ast::EnumConstantNode* enumConstantNode = node.EnumConstantList()->EnumConstants()[i];
                enumConstantNode->Accept(*this);
            }
            scope = prevScope;
            \
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::FunctionNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsFunctionSymbol())
    {
        cmajor::fault::tolerant::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::fault::tolerant::symbols::FunctionSymbol*>(symbol);
        if (!functionSymbol->IsBound())
        {
            functionSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = functionSymbol->Scope();
            cmajor::fault::tolerant::symbols::TypeSymbol* returnType = ResolveType(node.ReturnType(), scope, symbolTable, boundCompileUnit);
            if (returnType)
            {
                functionSymbol->SetReturnType(returnType);
            }
            int n = node.ParameterList()->ParameterCount();
            for (int i = 0; i < n; ++i)
            {
                cmajor::fault::tolerant::ast::ParameterNode* parameterNode = node.ParameterList()->GetParameter(i);
                cmajor::fault::tolerant::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), scope, symbolTable, boundCompileUnit);
                cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(parameterNode);
                if (symbol && symbol->IsParameterSymbol())
                {
                    cmajor::fault::tolerant::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::fault::tolerant::symbols::ParameterSymbol*>(symbol);
                    parameterSymbol->SetType(parameterType);
                }
            }
            if (node.Body())
            {
                node.Body()->Accept(*this);
            }
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::GlobalVariableNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsGlobalVariableSymbol())
    {
        cmajor::fault::tolerant::symbols::GlobalVariableSymbol* globalVariableSymbol = static_cast<cmajor::fault::tolerant::symbols::GlobalVariableSymbol*>(symbol);
        if (!globalVariableSymbol->IsBound())
        {
            globalVariableSymbol->SetBound();
            cmajor::fault::tolerant::symbols::TypeSymbol* type = ResolveType(node.Type(), scope, symbolTable, boundCompileUnit);
            if (type)
            {
                globalVariableSymbol->SetType(type);
            }
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::CompoundStatementNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsDeclarationBlockSymbol())
    {
        cmajor::fault::tolerant::symbols::DeclarationBlockSymbol* declarationBlockSymbol = static_cast<cmajor::fault::tolerant::symbols::DeclarationBlockSymbol*>(symbol);
        if (!declarationBlockSymbol->IsBound())
        {
            declarationBlockSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = declarationBlockSymbol->Scope();
            node.Statements()->Accept(*this);
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::ConstructionStatementNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsLocalVariableSymbol())
    {
        cmajor::fault::tolerant::symbols::LocalVariableSymbol* localVariableSymbol = static_cast<cmajor::fault::tolerant::symbols::LocalVariableSymbol*>(symbol);
        if (!localVariableSymbol->IsBound())
        {
            localVariableSymbol->SetBound();
            cmajor::fault::tolerant::symbols::TypeSymbol* type = ResolveType(node.TypeExpr(), scope, symbolTable, boundCompileUnit);
            if (type)
            {
                localVariableSymbol->SetType(type);
            }
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::ClassNode& node)
{
    cmajor::fault::tolerant::symbols::Symbol* symbol = symbolTable->GetMappedSymbol(&node);
    if (symbol && symbol->IsClassTypeSymbol())
    {
        cmajor::fault::tolerant::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::fault::tolerant::symbols::ClassTypeSymbol*>(symbol);
        if (!classTypeSymbol->IsBound())
        {
            classTypeSymbol->SetBound();
            cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
            scope = classTypeSymbol->Scope();
            for (const auto& baseClassOrInterfaceNode : node.BaseClassAndInterfaceList()->Nodes())
            {
                if (!baseClassOrInterfaceNode->IsComma())
                {
                    cmajor::fault::tolerant::symbols::TypeSymbol* type = ResolveType(baseClassOrInterfaceNode.get(), scope, symbolTable, boundCompileUnit);
                    if (type)
                    {
                        if (type->IsClassTypeSymbol())
                        {
                            classTypeSymbol->SetBaseClass(static_cast<cmajor::fault::tolerant::symbols::ClassTypeSymbol*>(type));
                        }
                        else if (type->IsInterfaceTypeSymbol())
                        {
                            classTypeSymbol->AddInterface(static_cast<cmajor::fault::tolerant::symbols::InterfaceTypeSymbol*>(type));
                        }
                    }
                }
            }
            node.Members().Accept(*this);
            scope = prevScope;
        }
    }
}

void TypeBinder::Visit(cmajor::fault::tolerant::ast::InterfaceNode& node)
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

void TypeBinder::Visit(cmajor::fault::tolerant::ast::ConceptNode& node)
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
            // todo
            scope = prevScope;
        }
    }
}

void BindTypes(cmajor::fault::tolerant::ast::CompileUnitNode* compileUnitNode, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable)
{
    TypeBinder typeBinder(boundCompileUnit, symbolTable);
    compileUnitNode->Accept(typeBinder);
}

} // namespace cmajor::fault::tolerant::binder
