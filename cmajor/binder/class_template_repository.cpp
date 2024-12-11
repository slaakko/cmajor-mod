// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.class_template_repository;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.type.resolver;
import cmajor.binder.type.binder;
import cmajor.binder.statement.binder;
import cmajor.binder.bound_class;
import cmajor.binder.bound.function;
import cmajor.binder.bound.statement;
import cmajor.binder.bound.expression;
import cmajor.binder.concepts;
import cmajor.binder.instantiation_guard;

namespace cmajor::binder {

size_t ClassIdMemberFunctionIndexHash::operator()(const std::pair<util::uuid, int>& p) const
{
    return util::HashValue(p.first) ^ std::hash<int>()(p.second);
}

ClassTemplateRepository::ClassTemplateRepository(BoundCompileUnit& boundCompileUnit_) : boundCompileUnit(boundCompileUnit_)
{
}

void ClassTemplateRepository::ResolveDefaultTemplateArguments(std::vector<cmajor::symbols::TypeSymbol*>& templateArgumentTypes, cmajor::symbols::ClassTypeSymbol* classTemplate, 
    cmajor::symbols::ContainerScope* containerScope, cmajor::ast::Node* node)
{
    int n = classTemplate->TemplateParameters().size();
    int m = templateArgumentTypes.size();
    if (m == n) return;
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::ast::Node* classTemplateNode = symbolTable.GetNodeNoThrow(classTemplate);
    if (!classTemplateNode)
    {
        classTemplateNode = classTemplate->GetClassNode();
        Assert(classTemplateNode, "class node not read");
    }
    Assert(classTemplateNode->GetNodeType() == cmajor::ast::NodeType::classNode, "class node expected");
    cmajor::ast::ClassNode* classNode = static_cast<cmajor::ast::ClassNode*>(classTemplateNode);
    int numFileScopeAdded = 0;
    int nu = classTemplate->UsingNodes().Count();
    if (nu > 0)
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        for (int i = 0; i < nu; ++i)
        {
            cmajor::ast::Node* usingNode = classTemplate->UsingNodes()[i];
            if (usingNode->GetNodeType() == cmajor::ast::NodeType::namespaceImportNode)
            {
                cmajor::ast::NamespaceImportNode* namespaceImportNode = static_cast<cmajor::ast::NamespaceImportNode*>(usingNode);
                fileScope->InstallNamespaceImport(containerScope, namespaceImportNode);
            }
            else if (usingNode->GetNodeType() == cmajor::ast::NodeType::aliasNode)
            {
                cmajor::ast::AliasNode* aliasNode = static_cast<cmajor::ast::AliasNode*>(usingNode);
                cmajor::symbols::TypeSymbol* type = ResolveType(aliasNode->TypeExpr(), boundCompileUnit, containerScope);
                fileScope->InstallAlias(aliasNode, type);
            }
        }
        boundCompileUnit.AddFileScope(fileScope);
        ++numFileScopeAdded;
    }
    if (!classTemplate->Ns()->IsGlobalNamespace())
    {
        cmajor::symbols::FileScope* primaryFileScope = new cmajor::symbols::FileScope();
        primaryFileScope->AddContainerScope(classTemplate->Ns()->GetContainerScope());
        boundCompileUnit.AddFileScope(primaryFileScope);
        ++numFileScopeAdded;
    }
    cmajor::symbols::ContainerScope resolveScope;
    resolveScope.SetParentScope(containerScope);
    std::vector<std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>> boundTemplateParameters;
    for (int i = 0; i < n; ++i)
    {
        cmajor::symbols::TemplateParameterSymbol* templateParameterSymbol = classTemplate->TemplateParameters()[i];
        cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameter = new cmajor::symbols::BoundTemplateParameterSymbol(
            templateParameterSymbol->GetSpan(), templateParameterSymbol->Name());
        boundTemplateParameters.push_back(std::unique_ptr<cmajor::symbols::BoundTemplateParameterSymbol>(boundTemplateParameter));
        if (i < m)
        {
            boundTemplateParameter->SetType(templateArgumentTypes[i]);
            resolveScope.Install(boundTemplateParameter);
        }
        else
        {
            if (i >= classNode->TemplateParameters().Count())
            {
                throw cmajor::symbols::Exception("too few template arguments", node->GetFullSpan());
            }
            cmajor::ast::Node* defaultTemplateArgumentNode = classNode->TemplateParameters()[i]->DefaultTemplateArgument();
            if (!defaultTemplateArgumentNode)
            {
                throw cmajor::symbols::Exception("too few template arguments", node->GetFullSpan());
            }
            cmajor::symbols::TypeSymbol* templateArgumentType = ResolveType(defaultTemplateArgumentNode, boundCompileUnit, &resolveScope);
            templateArgumentTypes.push_back(templateArgumentType);
        }
    }
    for (int i = 0; i < numFileScopeAdded; ++i)
    {
        boundCompileUnit.RemoveLastFileScope();
    }
}

void ClassTemplateRepository::BindClassTemplateSpecialization(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization, 
    cmajor::symbols::ContainerScope* containerScope, cmajor::ast::Node* node)
{
    if (classTemplateSpecialization->IsBound()) return;
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::ClassTypeSymbol* classTemplate = classTemplateSpecialization->GetClassTemplate();
    cmajor::ast::Node* classTemplateNode = symbolTable.GetNodeNoThrow(classTemplate);
    if (!classTemplateNode)
    {
        classTemplateNode = classTemplate->GetClassNode();
        Assert(classTemplateNode, "class node not read");
    }
    Assert(classTemplateNode->GetNodeType() == cmajor::ast::NodeType::classNode, "class node expected");
    cmajor::ast::ClassNode* classNode = static_cast<cmajor::ast::ClassNode*>(classTemplateNode);
    std::unique_ptr<cmajor::ast::NamespaceNode> globalNs(new cmajor::ast::NamespaceNode(classNode->GetSpan(), new cmajor::ast::IdentifierNode(classNode->GetSpan(), U"")));
    globalNs->SetFileIndex(classTemplateSpecialization->FileIndex());
    globalNs->SetModuleId(classTemplateSpecialization->ModuleId());
    cmajor::ast::NamespaceNode* currentNs = globalNs.get();
    cmajor::ast::CloneContext cloneContext;
    cloneContext.SetInstantiateClassNode();
    int nu = classTemplate->UsingNodes().Count();
    for (int i = 0; i < nu; ++i)
    {
        cmajor::ast::Node* usingNode = classTemplate->UsingNodes()[i];
        globalNs->AddMember(usingNode->Clone(cloneContext));
    }
    bool fileScopeAdded = false;
    if (!classTemplate->Ns()->IsGlobalNamespace())
    {
        cmajor::symbols::FileScope* primaryFileScope = new cmajor::symbols::FileScope();
        primaryFileScope->AddContainerScope(classTemplate->Ns()->GetContainerScope());
        boundCompileUnit.AddFileScope(primaryFileScope);
        fileScopeAdded = true;
        std::u32string fullNsName = classTemplate->Ns()->FullName();
        std::vector<std::u32string> nsComponents = util::Split(fullNsName, '.');
        for (const std::u32string& nsComponent : nsComponents)
        {
            cmajor::ast::NamespaceNode* nsNode = new cmajor::ast::NamespaceNode(classNode->GetSpan(), new cmajor::ast::IdentifierNode(classNode->GetSpan(), nsComponent));
            nsNode->SetFileIndex(classTemplateSpecialization->FileIndex());
            nsNode->SetModuleId(classTemplateSpecialization->ModuleId());
            currentNs->AddMember(nsNode);
            currentNs = nsNode;
        }
    }
    cmajor::ast::ClassNode* classInstanceNode = static_cast<cmajor::ast::ClassNode*>(classNode->Clone(cloneContext));
    currentNs->AddMember(classInstanceNode);
    int n = classTemplate->TemplateParameters().size();
    int m = classTemplateSpecialization->TemplateArgumentTypes().size();
    if (n != m)
    {
        throw cmajor::symbols::Exception("wrong number of template arguments", node->GetFullSpan());
    }
    bool templateParameterBinding = false;
    cmajor::symbols::ContainerScope resolveScope;
    resolveScope.SetParentScope(containerScope);
    for (int i = 0; i < n; ++i)
    {
        cmajor::symbols::TemplateParameterSymbol* templateParameter = classTemplate->TemplateParameters()[i];
        cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParameter = new cmajor::symbols::BoundTemplateParameterSymbol(
            templateParameter->GetSpan(), templateParameter->Name());
        boundTemplateParameter->SetParent(classTemplateSpecialization);
        cmajor::symbols::TypeSymbol* templateArgumentType = classTemplateSpecialization->TemplateArgumentTypes()[i];
        boundTemplateParameter->SetType(templateArgumentType);
        if (templateArgumentType->GetSymbolType() == cmajor::symbols::SymbolType::templateParameterSymbol)
        {
            templateParameterBinding = true;
            if (classTemplateSpecialization->IsPrototype())
            {
                if (classTemplateSpecialization->IsProject())
                {
                    resolveScope.Install(boundTemplateParameter);
                    cmajor::ast::TemplateParameterNode* templateParameterNode = classNode->TemplateParameters()[i];
                    cmajor::ast::Node* defaultTemplateArgumentNode = templateParameterNode->DefaultTemplateArgument();
                    if (defaultTemplateArgumentNode)
                    {
                        cmajor::symbols::TypeSymbol* templateArgumentType = ResolveType(defaultTemplateArgumentNode, boundCompileUnit, &resolveScope);
                        templateParameter->SetDefaultType(templateArgumentType);
                    }
                }
            }
        }
        classTemplateSpecialization->AddMember(boundTemplateParameter);
    }
    std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().GetLock());
    symbolTable.SetCurrentCompileUnit(boundCompileUnit.GetCompileUnitNode());
    InstantiationGuard instantiationGuard(symbolTable, classTemplateSpecialization->FileIndex(), classTemplateSpecialization->ModuleId());
    cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
    symbolCreatorVisitor.SetClassInstanceNode(classInstanceNode);
    symbolCreatorVisitor.SetClassTemplateSpecialization(classTemplateSpecialization);
    globalNs->Accept(symbolCreatorVisitor);
    TypeBinder typeBinder(boundCompileUnit);
    if (templateParameterBinding)
    {
        typeBinder.CreateMemberSymbols();
    }
    typeBinder.SetContainerScope(classTemplateSpecialization->GetContainerScope());
    globalNs->Accept(typeBinder);
    if (templateParameterBinding)
    {
        classTemplateSpecialization->SetGlobalNs(std::move(globalNs));
        if (fileScopeAdded)
        {
            boundCompileUnit.RemoveLastFileScope();
        }
    }
    else if (boundCompileUnit.BindingTypes())
    {
        classTemplateSpecialization->SetGlobalNs(std::move(globalNs));
        classTemplateSpecialization->SetStatementsNotBound();
        if (fileScopeAdded)
        {
            cmajor::symbols::FileScope* fileScope = boundCompileUnit.ReleaseLastFileScope();
            classTemplateSpecialization->SetFileScope(fileScope);
        }
    }
    else
    {
        StatementBinder statementBinder(boundCompileUnit);
        globalNs->Accept(statementBinder);
        classTemplateSpecialization->SetGlobalNs(std::move(globalNs));
        if (fileScopeAdded)
        {
            boundCompileUnit.RemoveLastFileScope();
        }
    }
}

cmajor::symbols::FunctionSymbol* ClassTemplateRepository::Instantiate(cmajor::symbols::FunctionSymbol* memberFunction, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction,
    cmajor::ast::Node* node)
{
    if (instantiatedMemberFunctions.find(memberFunction) != instantiatedMemberFunctions.cend())
    {
        return GetCopy(memberFunction);
    }
    instantiatedMemberFunctions.insert(memberFunction);
    try
    {
        cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
        cmajor::symbols::Symbol* parent = memberFunction->Parent();
        Assert(parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class template specialization expected"); 
        cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(parent);
        std::pair<util::uuid, int> classIdMemFunIndexPair = std::make_pair(classTemplateSpecialization->TypeId(), memberFunction->GetIndex());
        if (cmajor::symbols::GetBackEnd() != cmajor::symbols::BackEnd::masm && cmajor::symbols::GetBackEnd() != cmajor::symbols::BackEnd::cpp)
        {
            if (classIdMemberFunctionIndexSet.find(classIdMemFunIndexPair) != classIdMemberFunctionIndexSet.cend())
            {
                //          If <parent class id, member function index> pair is found from the classIdMemberFunctionIndexSet, the member function is already instantiated 
                //          for this compile unit, so return it.
                instantiatedMemberFunctions.insert(memberFunction);
                return memberFunction;
            }
        }
        if (classTemplateSpecialization->HasFullInstantiation() && !(cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && memberFunction->IsInline()))
        {
            instantiatedMemberFunctions.insert(memberFunction);
            return memberFunction;
        }
        Assert(classTemplateSpecialization->IsBound(), "class template specialization not bound"); 
        cmajor::ast::Node* memberFunctionNode = symbolTable.GetNodeNoThrow(memberFunction);
        if (!memberFunctionNode)
        {
            return nullptr;
        }
        boundCompileUnit.FinalizeBinding(classTemplateSpecialization);
        cmajor::symbols::ClassTypeSymbol* classTemplate = classTemplateSpecialization->GetClassTemplate();
        std::map<cmajor::symbols::TemplateParameterSymbol*, cmajor::symbols::TypeSymbol*> templateParameterMap;
        int n = classTemplateSpecialization->TemplateArgumentTypes().size();
        for (int i = 0; i < n; ++i)
        {
            cmajor::symbols::TemplateParameterSymbol* templateParameter = classTemplate->TemplateParameters()[i];
            cmajor::symbols::TypeSymbol* templateArgument = classTemplateSpecialization->TemplateArgumentTypes()[i];
            templateParameterMap[templateParameter] = templateArgument;
        }
        if (!classTemplateSpecialization->IsConstraintChecked())
        {
            classTemplateSpecialization->SetConstraintChecked();
            if (classTemplate->Constraint())
            {
                std::unique_ptr<BoundConstraint> boundConstraint;
                std::unique_ptr<cmajor::symbols::Exception> conceptCheckException;
                if (!CheckConstraint(classTemplate->Constraint(), classTemplate->UsingNodes(), boundCompileUnit, containerScope, currentFunction, classTemplate->TemplateParameters(),
                    templateParameterMap, boundConstraint, node, memberFunction, conceptCheckException))
                {
                    if (conceptCheckException)
                    {
                        throw cmajor::symbols::Exception("concept check of class template specialization '" + 
                            util::ToUtf8(classTemplateSpecialization->FullName()) + "' failed: " + conceptCheckException->Message(), node->GetFullSpan(), 
                            conceptCheckException->References());
                    }
                    else
                    {
                        throw cmajor::symbols::Exception("concept check of class template specialization '" + 
                            util::ToUtf8(classTemplateSpecialization->FullName()) + "' failed.", node->GetFullSpan());
                    }
                }
            }
        }
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        int nu = classTemplate->UsingNodes().Count();
        for (int i = 0; i < nu; ++i)
        {
            cmajor::ast::Node* usingNode = classTemplate->UsingNodes()[i];
            if (usingNode->GetNodeType() == cmajor::ast::NodeType::namespaceImportNode)
            {
                cmajor::ast::NamespaceImportNode* namespaceImportNode = static_cast<cmajor::ast::NamespaceImportNode*>(usingNode);
                fileScope->InstallNamespaceImport(containerScope, namespaceImportNode);
            }
            else if (usingNode->GetNodeType() == cmajor::ast::NodeType::aliasNode)
            {
                cmajor::ast::AliasNode* aliasNode = static_cast<cmajor::ast::AliasNode*>(usingNode);
                cmajor::symbols::TypeSymbol* type = ResolveType(aliasNode, boundCompileUnit, containerScope);
                fileScope->InstallAlias(aliasNode, type); 
            }
        }
        if (!classTemplate->Ns()->IsGlobalNamespace())
        {
            fileScope->AddContainerScope(classTemplate->Ns()->GetContainerScope());
        }
        boundCompileUnit.AddFileScope(fileScope);
        Assert(memberFunctionNode->IsFunctionNode(), "function node expected");
        cmajor::ast::FunctionNode* functionInstanceNode = static_cast<cmajor::ast::FunctionNode*>(memberFunctionNode);
        if (memberFunction->IsDefault())
        {
            functionInstanceNode->SetBodySource(new cmajor::ast::CompoundStatementNode(memberFunctionNode->GetSpan()));
        }
        Assert(functionInstanceNode->BodySource(), "body source expected"); 
        cmajor::ast::CloneContext cloneContext;
        functionInstanceNode->SetBody(static_cast<cmajor::ast::CompoundStatementNode*>(functionInstanceNode->BodySource()->Clone(cloneContext)));
        if (functionInstanceNode->WhereConstraint())
        {
            std::unique_ptr<BoundConstraint> boundConstraint;
            std::unique_ptr<cmajor::symbols::Exception> conceptCheckException;
            cmajor::symbols::FileScope* classTemplateScope = new cmajor::symbols::FileScope();
            classTemplateScope->AddContainerScope(classTemplateSpecialization->GetContainerScope());
            boundCompileUnit.AddFileScope(classTemplateScope);
            if (!CheckConstraint(functionInstanceNode->WhereConstraint(), classTemplate->UsingNodes(), boundCompileUnit, containerScope, currentFunction, 
                classTemplate->TemplateParameters(), templateParameterMap, boundConstraint, node, memberFunction, conceptCheckException))
            {
                boundCompileUnit.RemoveLastFileScope();
                if (conceptCheckException)
                {
                    std::vector<soul::ast::FullSpan> references;
                    references.push_back(conceptCheckException->Defined());
                    references.insert(references.end(), conceptCheckException->References().begin(), conceptCheckException->References().end());
                    throw cmajor::symbols::Exception("concept check of class template member function '" + util::ToUtf8(memberFunction->FullName()) + "' failed: " + 
                        conceptCheckException->Message(), node->GetFullSpan(), references);
                }
                else
                {
                    throw cmajor::symbols::Exception("concept check of class template template member function '" + util::ToUtf8(memberFunction->FullName()) + "' failed.", 
                        node->GetFullSpan());
                }
            }
            else
            {
                boundCompileUnit.RemoveLastFileScope();
            }
        }
        std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().GetLock());
        cmajor::symbols::FunctionSymbol* master = memberFunction;
        master->ResetImmutable();
        cmajor::symbols::FunctionSymbol* copy = master->Copy();
        copyMap[master] = copy;
        memberFunction = copy;
        boundCompileUnit.GetSymbolTable().AddFunctionSymbol(std::unique_ptr<cmajor::symbols::FunctionSymbol>(memberFunction));
        master->SetImmutable();
        symbolTable.SetCurrentCompileUnit(boundCompileUnit.GetCompileUnitNode());
        InstantiationGuard instantiationGuard(symbolTable, classTemplate->FileIndex(), classTemplate->ModuleId());
        cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
        if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
        {
            cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit.GetCompileUnitNode();
            if (compileUnitNode)
            {
                memberFunction->SetCompileUnitId(compileUnitNode->Id());
                memberFunction->SetFlag(cmajor::symbols::FunctionSymbolFlags::dontReuse);
                memberFunction->ComputeMangledName();
                master->SetInstantiatedName(memberFunction->MangledName());
                boundCompileUnit.SetCanReuse(memberFunction);
            }
        }
        symbolTable.BeginContainer(memberFunction);
        symbolTable.MapNode(functionInstanceNode, memberFunction);
        functionInstanceNode->Body()->Accept(symbolCreatorVisitor);
        symbolTable.EndContainer();
        TypeBinder typeBinder(boundCompileUnit);
        typeBinder.SetContainerScope(memberFunction->GetContainerScope());
        typeBinder.SetCurrentFunctionSymbol(memberFunction);
        functionInstanceNode->Body()->Accept(typeBinder);
        StatementBinder statementBinder(boundCompileUnit);
        std::unique_ptr<BoundClass> boundClass(new BoundClass(classTemplateSpecialization));
        statementBinder.SetCurrentClass(boundClass.get());
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, memberFunction));
        statementBinder.SetCurrentFunction(boundFunction.get());
        statementBinder.SetContainerScope(memberFunction->GetContainerScope());
        if (memberFunction->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol)
        {
            cmajor::symbols::ConstructorSymbol* constructorSymbol = static_cast<cmajor::symbols::ConstructorSymbol*>(memberFunction);
            cmajor::ast::Node* node = symbolTable.GetNode(memberFunction);
            Assert(node->GetNodeType() == cmajor::ast::NodeType::constructorNode, "constructor node expected");
            cmajor::ast::ConstructorNode* constructorNode = static_cast<cmajor::ast::ConstructorNode*>(node);
            statementBinder.SetCurrentConstructor(constructorSymbol, constructorNode);
        }
        else if (memberFunction->GetSymbolType() == cmajor::symbols::SymbolType::destructorSymbol)
        {
            cmajor::symbols::DestructorSymbol* destructorSymbol = static_cast<cmajor::symbols::DestructorSymbol*>(memberFunction);
            cmajor::ast::Node* node = symbolTable.GetNode(memberFunction);
            Assert(node->GetNodeType() == cmajor::ast::NodeType::destructorNode, "destructor node expected"); 
            cmajor::ast::DestructorNode* destructorNode = static_cast<cmajor::ast::DestructorNode*>(node);
            statementBinder.SetCurrentDestructor(destructorSymbol, destructorNode);
        }
        else if (memberFunction->GetSymbolType() == cmajor::symbols::SymbolType::memberFunctionSymbol)
        {
            cmajor::symbols::MemberFunctionSymbol* memberFunctionSymbol = static_cast<cmajor::symbols::MemberFunctionSymbol*>(memberFunction);
            cmajor::ast::Node* node = symbolTable.GetNode(memberFunction);
            Assert(node->GetNodeType() == cmajor::ast::NodeType::memberFunctionNode, "member function node expected");
            cmajor::ast::MemberFunctionNode* memberFunctionNode = static_cast<cmajor::ast::MemberFunctionNode*>(node);
            statementBinder.SetCurrentMemberFunction(memberFunctionSymbol, memberFunctionNode);
        }
        functionInstanceNode->Body()->Accept(statementBinder);
        CheckFunctionReturnPaths(memberFunction, functionInstanceNode->Body(), node, containerScope, boundCompileUnit);
        BoundStatement* boundStatement = statementBinder.ReleaseStatement();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected"); 
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        statementBinder.GenerateEnterAndExitFunctionCode(boundFunction.get());
        std::u32string instantiatedMemberFunctionMangledName = boundFunction->GetFunctionSymbol()->MangledName();
        boundClass->AddMember(std::move(boundFunction));
        if (cmajor::symbols::GetBackEnd() != cmajor::symbols::BackEnd::masm && cmajor::symbols::GetBackEnd() != cmajor::symbols::BackEnd::cpp)
        {
            classIdMemberFunctionIndexSet.insert(classIdMemFunIndexPair);
        }
        boundCompileUnit.AddBoundNode(std::move(boundClass));
        boundCompileUnit.RemoveLastFileScope();
        bool succeeded = InstantiateDestructorAndVirtualFunctions(classTemplateSpecialization, containerScope, currentFunction, node);
        if (succeeded)
        {
            return memberFunction;
        }
        else
        {
            return nullptr;
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(memberFunction->GetFullSpan());
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("could not instantiate member function '" + util::ToUtf8(memberFunction->FullName()) + "'. Reason: " + ex.Message(), 
            node->GetFullSpan(), references);
    }
}

bool ClassTemplateRepository::InstantiateDestructorAndVirtualFunctions(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization, 
    cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, cmajor::ast::Node* node)
{
    for (cmajor::symbols::FunctionSymbol* virtualMemberFunction : classTemplateSpecialization->Vmt())
    {
        if (virtualMemberFunction->Parent() == classTemplateSpecialization && !virtualMemberFunction->IsGeneratedFunction())
        {
            if (!Instantiate(virtualMemberFunction, containerScope, currentFunction, node))
            {
                return false;
            }
        }
    }
    if (classTemplateSpecialization->Destructor())
    {
        if (!classTemplateSpecialization->Destructor()->IsGeneratedFunction())
        {
            if (!Instantiate(classTemplateSpecialization->Destructor(), containerScope, currentFunction, node))
            {
                return false;
            }
        }
    }
    return true;
}

void ClassTemplateRepository::InstantiateAll(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization, cmajor::symbols::ContainerScope* containerScope, 
    BoundFunction* currentFunction, cmajor::ast::Node* node)
{
    try
    {
        BindClassTemplateSpecialization(classTemplateSpecialization, containerScope, node);
        for (cmajor::symbols::FunctionSymbol* memberFunction : classTemplateSpecialization->AllMemberFunctions())
        {
            if (!Instantiate(memberFunction, containerScope, currentFunction, node))
            {
                throw cmajor::symbols::Exception("instantiation of member function '" + util::ToUtf8(memberFunction->Name()) + "' failed", memberFunction->GetFullSpan());
            }
        }
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("full instantiation request for class template specialization '" + util::ToUtf8(classTemplateSpecialization->FullName()) + 
            "' failed. Reason: " + ex.Message(), node->GetFullSpan(), references);
    }
}

cmajor::symbols::FunctionSymbol* ClassTemplateRepository::GetCopy(cmajor::symbols::FunctionSymbol* master) const
{
    auto it = copyMap.find(master);
    if (it != copyMap.end())
    {
        return it->second;
    }
    else
    {
        return master;
    }
}

} // namespace cmajor::binder
