// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.inline_function_repository;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.type.binder;
import cmajor.binder.statement.binder;
import cmajor.binder.bound_class;
import cmajor.binder.bound.function;
import cmajor.binder.bound.statement;
import util;

namespace cmajor::binder {

InlineFunctionRepository::InlineFunctionRepository(BoundCompileUnit& boundCompileUnit_) : boundCompileUnit(boundCompileUnit_)
{
}

cmajor::symbols::FunctionSymbol* InlineFunctionRepository::Instantiate(cmajor::symbols::FunctionSymbol* inlineFunction, cmajor::symbols::ContainerScope* containerScope, 
    const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    if (inlineFunction->GetCompileUnit() == boundCompileUnit.GetCompileUnitNode()) return inlineFunction;
    while (inlineFunction->Master())
    {
        inlineFunction = inlineFunction->Master();
    }
    auto it = inlineFunctionMap.find(inlineFunction);
    if (it != inlineFunctionMap.cend())
    {
        return it->second;
    }
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(inlineFunction);
    if (!node)
    {
        node = inlineFunction->GetFunctionNode();
        symbolTable.MapNode(node, inlineFunction);
        // Assert(node, "function node not read"); TODO
    }
    cmajor::ast::FunctionNode* functionNode = static_cast<cmajor::ast::FunctionNode*>(node);
    std::unique_ptr<cmajor::ast::NamespaceNode> globalNs(new cmajor::ast::NamespaceNode(functionNode->GetSourcePos(), functionNode->ModuleId(), 
        new cmajor::ast::IdentifierNode(functionNode->GetSourcePos(), functionNode->ModuleId(), U"")));
    cmajor::ast::NamespaceNode* currentNs = globalNs.get();
    cmajor::ast::CloneContext cloneContext;
    cloneContext.SetInstantiateFunctionNode();
    bool fileScopeAdded = false;
    int n = inlineFunction->UsingNodes().Count();
    if (!inlineFunction->Ns()->IsGlobalNamespace() || n > 0)
    {
        cmajor::symbols::FileScope* primaryFileScope = new cmajor::symbols::FileScope();
        if (!inlineFunction->Ns()->IsGlobalNamespace())
        {
            primaryFileScope->AddContainerScope(inlineFunction->Ns()->GetContainerScope());
        }
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::Node* usingNode = inlineFunction->UsingNodes()[i];
            if (usingNode->GetNodeType() == cmajor::ast::NodeType::namespaceImportNode)
            {
                primaryFileScope->InstallNamespaceImport(containerScope, static_cast<cmajor::ast::NamespaceImportNode*>(usingNode));
            }
            else if (usingNode->GetNodeType() == cmajor::ast::NodeType::aliasNode)
            {
                // primaryFileScope->InstallAlias(containerScope, static_cast<AliasNode*>(usingNode)); TODO
            }
        }
        boundCompileUnit.AddFileScope(primaryFileScope);
        fileScopeAdded = true;
        std::u32string fullNsName = inlineFunction->Ns()->FullName();
        std::vector<std::u32string> nsComponents = util::Split(fullNsName, '.');
        for (const std::u32string& nsComponent : nsComponents)
        {
            cmajor::ast::NamespaceNode* nsNode = new cmajor::ast::NamespaceNode(functionNode->GetSourcePos(), functionNode->ModuleId(), 
                new cmajor::ast::IdentifierNode(functionNode->GetSourcePos(), functionNode->ModuleId(), nsComponent));
            currentNs->AddMember(nsNode);
            currentNs = nsNode;
        }
    }
    cmajor::ast::FunctionNode* functionInstanceNode = static_cast<cmajor::ast::FunctionNode*>(functionNode->Clone(cloneContext));
    if (inlineFunction->IsDefault())
    {
        functionInstanceNode->SetBody(new cmajor::ast::CompoundStatementNode(sourcePos, moduleId));
        inlineFunction->SetHasArtificialBody();
    }
    currentNs->AddMember(functionInstanceNode);
    std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().GetLock());
    symbolTable.SetCurrentCompileUnit(boundCompileUnit.GetCompileUnitNode());
    if (!inlineFunction->Parent()->IsClassTypeSymbol())
    {
        cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
        symbolCreatorVisitor.SetLeaveFunction();
        globalNs->Accept(symbolCreatorVisitor);
        std::unique_ptr<cmajor::symbols::FunctionSymbol> functionSymbol(symbolTable.GetCreatedFunctionSymbol());
        if (inlineFunction->IsDefault())
        {
            functionSymbol->SetHasArtificialBody();
        }
        functionSymbol->SetParent(inlineFunction->Parent());
        functionSymbol->SetLinkOnceOdrLinkage();
        if (inlineFunction->IsSystemDefault())
        {
            functionSymbol->SetSystemDefault();
        }
        TypeBinder typeBinder(boundCompileUnit);
        typeBinder.SetContainerScope(functionSymbol->GetContainerScope());
        typeBinder.SetCurrentFunctionSymbol(functionSymbol.get());
        functionInstanceNode->Accept(typeBinder);
        StatementBinder statementBinder(boundCompileUnit);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, functionSymbol.get()));
        statementBinder.SetCurrentFunction(boundFunction.get());
        statementBinder.SetContainerScope(functionSymbol->GetContainerScope());
        functionInstanceNode->Body()->Accept(statementBinder);
        BoundStatement* boundStatement = statementBinder.ReleaseStatement();
        // Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected"); TODO
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        boundCompileUnit.AddBoundNode(std::move(boundFunction));
        if (fileScopeAdded)
        {
            boundCompileUnit.RemoveLastFileScope();
        }
        cmajor::symbols::FunctionSymbol* result = functionSymbol.get();
        boundCompileUnit.GetSymbolTable().AddFunctionSymbol(std::move(functionSymbol));
        boundCompileUnit.AddGlobalNs(std::move(globalNs));
        inlineFunctionMap[inlineFunction] = result;
        result->SetFunctionId(inlineFunction->FunctionId());
        result->SetMaster(inlineFunction);
        result->SetCopy();
        return result;
    }
    else
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(inlineFunction->Parent());
        symbolTable.SetCurrentClass(classTypeSymbol);
        cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
        symbolCreatorVisitor.SetLeaveFunction();
        globalNs->Accept(symbolCreatorVisitor);
        std::unique_ptr<cmajor::symbols::FunctionSymbol> functionSymbol(symbolTable.GetCreatedFunctionSymbol());
        functionSymbol->SetVmtIndex(inlineFunction->VmtIndex());
        functionSymbol->SetImtIndex(inlineFunction->ImtIndex());
        if (inlineFunction->IsDefault())
        {
            functionSymbol->SetHasArtificialBody();
        }
        functionSymbol->SetParent(classTypeSymbol);
        functionSymbol->SetLinkOnceOdrLinkage();
        if (inlineFunction->IsSystemDefault())
        {
            functionSymbol->SetSystemDefault();
        }
        TypeBinder typeBinder(boundCompileUnit);
        typeBinder.SetContainerScope(functionSymbol->GetContainerScope());
        functionInstanceNode->Accept(typeBinder);
        StatementBinder statementBinder(boundCompileUnit);
        std::unique_ptr<BoundClass> boundClass(new BoundClass(classTypeSymbol));
        boundClass->SetInlineFunctionContainer();
        statementBinder.SetCurrentClass(boundClass.get());
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, functionSymbol.get()));
        statementBinder.SetCurrentFunction(boundFunction.get());
        statementBinder.SetContainerScope(functionSymbol->GetContainerScope());
        if (functionSymbol->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol)
        {
            cmajor::symbols::ConstructorSymbol* constructorSymbol = static_cast<cmajor::symbols::ConstructorSymbol*>(functionSymbol.get());
            cmajor::ast::Node* node = symbolTable.GetNode(functionSymbol.get());
            // Assert(node->GetNodeType() == cmajor::ast::NodeType::constructorNode, "constructor node expected"); TODO
            cmajor::ast::ConstructorNode* constructorNode = static_cast<cmajor::ast::ConstructorNode*>(node);
            statementBinder.SetCurrentConstructor(constructorSymbol, constructorNode);
        }
        else if (functionSymbol->GetSymbolType() == cmajor::symbols::SymbolType::destructorSymbol)
        {
            cmajor::symbols::DestructorSymbol* destructorSymbol = static_cast<cmajor::symbols::DestructorSymbol*>(functionSymbol.get());
            cmajor::ast::Node* node = symbolTable.GetNode(functionSymbol.get());
            // Assert(node->GetNodeType() == cmajor::ast::NodeType::destructorNode, "destructor node expected"); TODO
            cmajor::ast::DestructorNode* destructorNode = static_cast<cmajor::ast::DestructorNode*>(node);
            statementBinder.SetCurrentDestructor(destructorSymbol, destructorNode);
        }
        else if (functionSymbol->GetSymbolType() == cmajor::symbols::SymbolType::memberFunctionSymbol)
        {
            cmajor::symbols::MemberFunctionSymbol* memberFunctionSymbol = static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol.get());
            cmajor::ast::Node* node = symbolTable.GetNode(functionSymbol.get());
            // Assert(node->GetNodeType() == cmajor::ast::NodeType::memberFunctionNode, "member function node expected"); TODO
            cmajor::ast::MemberFunctionNode* memberFunctionNode = static_cast<cmajor::ast::MemberFunctionNode*>(node);
            statementBinder.SetCurrentMemberFunction(memberFunctionSymbol, memberFunctionNode);
        }
        functionInstanceNode->Body()->Accept(statementBinder);
        BoundStatement* boundStatement = statementBinder.ReleaseStatement();
        // Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected"); TODO
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        boundClass->AddMember(std::move(boundFunction));
        boundCompileUnit.AddBoundNode(std::move(boundClass));
        cmajor::symbols::FunctionSymbol* result = functionSymbol.get();
        boundCompileUnit.AddGlobalNs(std::move(globalNs));
        boundCompileUnit.GetSymbolTable().AddFunctionSymbol(std::move(functionSymbol));
        if (fileScopeAdded)
        {
            boundCompileUnit.RemoveLastFileScope();
        }
        inlineFunctionMap[inlineFunction] = result;
        result->SetFunctionId(inlineFunction->FunctionId());
        result->SetMaster(inlineFunction);
        result->SetCopy();
        return result;
    }
}

} // namespace cmajor::binder