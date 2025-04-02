// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.type.binder;

import cmajor.binder.bound.compile.unit;
import cmajor.binder.type.resolver;
import cmajor.binder.concepts;
import cmajor.binder.evaluator;
import cmajor.binder.attribute.binder;
import cmajor.binder.bound.expression;
import cmajor.binder.bound.function;
import util;

namespace cmajor::binder {

ConstraintSymbolBinder* constraintSymbolBinder = nullptr;

ConstraintSymbolBinder::~ConstraintSymbolBinder()
{
}

void SetConstraintSymbolBinder(ConstraintSymbolBinder* constraintSymbolBinder_)
{
    constraintSymbolBinder = constraintSymbolBinder_;
}

class UsingNodeAdder : public cmajor::ast::Visitor
{
public:
    UsingNodeAdder(BoundCompileUnit& boundCompileUnit_, cmajor::symbols::ContainerScope* containerscope);
    void Visit(cmajor::ast::NamespaceNode& namespaceNode) override;
    void Visit(cmajor::ast::AliasNode& aliasNode) override;
    void Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode) override;
private:
    BoundCompileUnit& boundCompileUnit;
    cmajor::symbols::SymbolTable& symbolTable;
    cmajor::symbols::ContainerScope* containerScope;
};

UsingNodeAdder::UsingNodeAdder(BoundCompileUnit& boundCompileUnit_, cmajor::symbols::ContainerScope* containerScope_) :
    boundCompileUnit(boundCompileUnit_), symbolTable(boundCompileUnit.GetSymbolTable()), containerScope(containerScope_)
{
}

void UsingNodeAdder::Visit(cmajor::ast::NamespaceNode& namespaceNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&namespaceNode);
    containerScope = symbol->GetContainerScope();
    int n = namespaceNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* member = namespaceNode.Members()[i];
        member->Accept(*this);
    }
    containerScope = prevContainerScope;
}

void UsingNodeAdder::Visit(cmajor::ast::AliasNode& aliasNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&aliasNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::aliasTypeSymbol, "alias type symbol expected");
    cmajor::symbols::AliasTypeSymbol* aliasTypeSymbol = static_cast<cmajor::symbols::AliasTypeSymbol*>(symbol);
    boundCompileUnit.FirstFileScope()->InstallAlias(&aliasNode, aliasTypeSymbol->GetType());
}

void UsingNodeAdder::Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode)
{
    boundCompileUnit.FirstFileScope()->InstallNamespaceImport(containerScope, &namespaceImportNode, boundCompileUnit.GetContext());
}

TypeBinder::TypeBinder(BoundCompileUnit& boundCompileUnit_) :
    boundCompileUnit(boundCompileUnit_), symbolTable(boundCompileUnit.GetSymbolTable()), module(&boundCompileUnit.GetModule()), context(boundCompileUnit.GetContext()),
    containerScope(), enumType(nullptr), currentFunctionSymbol(nullptr), currentClassTypeSymbol(nullptr), typeResolverFlags(TypeResolverFlags::none), boundGlobalVariable(nullptr)
    
{
}

void TypeBinder::AddUsingNodesToCurrentCompileUnit(cmajor::ast::Node* node)
{
    cmajor::ast::NamespaceNode* namespaceNode = nullptr;
    cmajor::ast::Node* parent = node->Parent();
    while (parent)
    {
        if (parent->GetNodeType() == cmajor::ast::NodeType::namespaceNode)
        {
            namespaceNode = static_cast<cmajor::ast::NamespaceNode*>(parent);
        }
        parent = parent->Parent();
    }
    if (!namespaceNode)
    {
        throw cmajor::symbols::Exception("global namespace parent not found for node", node->GetFullSpan());
    }
    UsingNodeAdder usingNodeAdder(boundCompileUnit, containerScope);
    namespaceNode->Accept(usingNodeAdder);
}

void TypeBinder::Visit(cmajor::ast::CompileUnitNode& compileUnitNode)
{
    if (compileUnitNode.IsProgramMainUnit())
    {
        boundCompileUnit.GenerateGlobalInitializationFunction(compileUnitNode.GetSpan());
    }
    boundCompileUnit.AddFileScope(new cmajor::symbols::FileScope());
    compileUnitNode.GlobalNs()->Accept(*this);
}

void TypeBinder::Visit(cmajor::ast::NamespaceNode& namespaceNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&namespaceNode);
    symbol->ComputeMangledName(context);
    containerScope = symbol->GetContainerScope();
    int n = namespaceNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* member = namespaceNode.Members()[i];
        member->Accept(*this);
    }
    containerScope = prevContainerScope;
}

void TypeBinder::Visit(cmajor::ast::AliasNode& aliasNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&aliasNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::aliasTypeSymbol, "alias type symbol expected");
    cmajor::symbols::AliasTypeSymbol* aliasTypeSymbol = static_cast<cmajor::symbols::AliasTypeSymbol*>(symbol);
    BindAlias(aliasTypeSymbol, &aliasNode, true);
    boundCompileUnit.FirstFileScope()->InstallAlias(&aliasNode, aliasTypeSymbol->GetType());
    usingNodes.push_back(&aliasNode);
}

void TypeBinder::Visit(cmajor::ast::NamespaceImportNode& namespaceImportNode)
{
    boundCompileUnit.FirstFileScope()->InstallNamespaceImport(containerScope, &namespaceImportNode, context);
    usingNodes.push_back(&namespaceImportNode);
}

void TypeBinder::Visit(cmajor::ast::FunctionNode& functionNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&functionNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::functionSymbol, "function symbol expected");
    cmajor::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::symbols::FunctionSymbol*>(symbol);
    if (functionSymbol->IsBound()) return;
    functionSymbol->SetBound();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(&functionNode, functionSymbol);
    }
    cmajor::symbols::FunctionSymbol* prevFunctionSymbol = currentFunctionSymbol;
    currentFunctionSymbol = functionSymbol;
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && functionNode.WhereConstraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(functionNode.WhereConstraint(), containerScope, boundCompileUnit);
    }
    if (functionSymbol->IsFunctionTemplate())
    {
        functionSymbol->CloneUsingNodes(usingNodes);
        if (functionNode.WhereConstraint())
        {
            cmajor::ast::CloneContext cloneContext;
            functionSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(functionNode.WhereConstraint()->Clone(cloneContext)));
        }
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && functionSymbol->Constraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(functionSymbol->Constraint(), functionSymbol->GetContainerScope(), boundCompileUnit);
    }
    containerScope = functionSymbol->GetContainerScope();
    cmajor::ast::Specifiers specifiers = functionNode.GetSpecifiers();
    functionSymbol->SetSpecifiers(specifiers);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && functionSymbol->IsInline())
    {
        functionSymbol->CloneUsingNodes(usingNodes);
    }
    int n = functionNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = functionNode.Parameters()[i];
        cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags);
        cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(parameterNode);
        Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::parameterSymbol, "parameter symbol expected");
        cmajor::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::symbols::ParameterSymbol*>(symbol);
        parameterSymbol->SetType(parameterType);
        if (parameterType->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
        {
            cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(parameterType);
            if (interfaceTypeSymbol->IsProject())
            {
                cmajor::ast::Node* node = symbolTable.GetNode(interfaceTypeSymbol);
                Assert(node->GetNodeType() == cmajor::ast::NodeType::interfaceNode, "interface node expected");
                cmajor::ast::InterfaceNode* interfaceNode = static_cast<cmajor::ast::InterfaceNode*>(node);
                BindInterface(interfaceTypeSymbol, interfaceNode, false);
            }
        }
    }
    cmajor::symbols::TypeSymbol* returnType = ResolveType(functionNode.ReturnTypeExpr(), boundCompileUnit, containerScope, typeResolverFlags);
    functionSymbol->SetReturnType(returnType);
    if (returnType->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
    {
        cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(returnType);
        if (interfaceTypeSymbol->IsProject())
        {
            cmajor::ast::Node* node = symbolTable.GetNode(interfaceTypeSymbol);
            Assert(node->GetNodeType() == cmajor::ast::NodeType::interfaceNode, "interface node expected");
            cmajor::ast::InterfaceNode* interfaceNode = static_cast<cmajor::ast::InterfaceNode*>(node);
            BindInterface(interfaceTypeSymbol, interfaceNode, false);
        }
    }
    if (!functionSymbol->Constraint() && functionNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        functionSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(functionNode.WhereConstraint()->Clone(cloneContext)));
    }
    functionSymbol->ComputeName(context);
    cmajor::symbols::SymbolMap* symbolMap = cmajor::symbols::GetSymbolMap();
    if (symbolMap)
    {
        symbolMap->Add(functionSymbol);
    }
    for (cmajor::symbols::ParameterSymbol* parameterSymbol : functionSymbol->Parameters())
    {
        parameterSymbol->ComputeMangledName(context);
    }
    if (functionSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(functionNode.ReturnTypeExpr()->GetSpan(), U"@return");
        returnParam->SetParent(functionSymbol);
        returnParam->SetType(returnType->AddPointer(context));
        functionSymbol->SetReturnParam(returnParam);
    }
    if (functionNode.Body() && !functionSymbol->IsFunctionTemplate())
    {
        functionNode.Body()->Accept(*this);
    }
    else
    {
        if (!functionSymbol->IsExternal() && !functionSymbol->IsFunctionTemplate())
        {
            throw cmajor::symbols::Exception("function has no body", functionNode.GetFullSpan(), functionSymbol->GetFullSpan());
        }
    }
    boundCompileUnit.GetAttributeBinder()->BindAttributes(functionNode.GetAttributes(), symbol, boundCompileUnit, containerScope);
    containerScope = prevContainerScope;
    currentFunctionSymbol = prevFunctionSymbol;
}

void TypeBinder::Visit(cmajor::ast::ClassNode& classNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&classNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
        symbol->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type symbol expected"); 
    cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol);
    BindClass(classTypeSymbol, &classNode, true);
}

void TypeBinder::BindClassTemplate(cmajor::symbols::ClassTypeSymbol* classTemplate, cmajor::ast::ClassNode* classNode)
{
    classTemplate->CloneUsingNodes(usingNodes);
    if (classNode->WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        classTemplate->SetConstraint(static_cast<cmajor::ast::ConstraintNode*>(classNode->WhereConstraint()->Clone(cloneContext)));
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            constraintSymbolBinder->BindConstraintSymbols(classNode->WhereConstraint(), classTemplate->GetContainerScope(), boundCompileUnit);
        }
    }
    classTemplate->SetAccess(classNode->GetSpecifiers() & cmajor::ast::Specifiers::access_);
    classTemplate->ComputeName(context);
    std::vector<cmajor::symbols::TypeSymbol*> templateArgumentTypes;
    for (cmajor::symbols::TemplateParameterSymbol* templateParam : classTemplate->TemplateParameters())
    {
        templateArgumentTypes.push_back(templateParam);
    }
    cmajor::symbols::ClassTemplateSpecializationSymbol* prototype = symbolTable.MakeClassTemplateSpecialization(classTemplate, templateArgumentTypes);
    prototype->SetPrototype();
    prototype->SetAccess(cmajor::symbols::SymbolAccess::public_);
    boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(prototype, containerScope, classNode);
    classTemplate->SetPrototype(prototype);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && prototype->Constraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(prototype->Constraint(), prototype->GetContainerScope(), boundCompileUnit);
    }
}

void TypeBinder::BindClass(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::ast::ClassNode* classNode, bool fromOwnCompileUnit)
{
    if (classTypeSymbol->IsBound()) return;
    classTypeSymbol->SetBound();
    cmajor::symbols::ClassTypeSymbol* prevClassTypeSymbol = currentClassTypeSymbol;
    currentClassTypeSymbol = classTypeSymbol;
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(classNode->Id(), classTypeSymbol);
    }
    if (!fromOwnCompileUnit)
    {
        AddUsingNodesToCurrentCompileUnit(classNode);
    }
    if (classTypeSymbol->IsClassTemplate())
    {
        BindClassTemplate(classTypeSymbol, classNode);
        currentClassTypeSymbol = prevClassTypeSymbol;
        return;
    }
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = classTypeSymbol->GetContainerScope();
    classTypeSymbol->SetSpecifiers(classNode->GetSpecifiers());
    if (classNode->WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        classTypeSymbol->SetConstraint(static_cast<cmajor::ast::ConstraintNode*>(classNode->WhereConstraint()->Clone(cloneContext)));
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
        {
            constraintSymbolBinder->BindConstraintSymbols(classNode->WhereConstraint(), containerScope, boundCompileUnit);
        }
    }
    classTypeSymbol->ComputeName(context);
    int nb = classNode->BaseClassOrInterfaces().Count();
    for (int i = 0; i < nb; ++i)
    {
        cmajor::ast::Node* baseOrInterfaceNode = classNode->BaseClassOrInterfaces()[i];
        cmajor::symbols::TypeSymbol* baseOrInterfaceSymbol = ResolveType(baseOrInterfaceNode, boundCompileUnit, containerScope, typeResolverFlags);
        if (baseOrInterfaceSymbol->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* baseClassSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(baseOrInterfaceSymbol);
            if (baseClassSymbol->IsProject() && !cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::info))
            {
                cmajor::ast::Node* node = symbolTable.GetNode(baseClassSymbol);
                Assert(node->GetNodeType() == cmajor::ast::NodeType::classNode, "class node expected"); 
                cmajor::ast::ClassNode* baseClassNode = static_cast<cmajor::ast::ClassNode*>(node);
                BindClass(baseClassSymbol, baseClassNode, false);
            }
            if (classTypeSymbol->BaseClass())
            {
                throw cmajor::symbols::Exception("class type can have at most one base class", classNode->GetFullSpan(), baseClassSymbol->GetFullSpan());
            }
            else if (baseClassSymbol == classTypeSymbol)
            {
                throw cmajor::symbols::Exception("class cannot derive from itself", classNode->GetFullSpan());
            }
            else
            {
                classTypeSymbol->SetBaseClass(baseClassSymbol);
            }
        }
        else if (baseOrInterfaceSymbol->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
        {
            cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(baseOrInterfaceSymbol);
            if (interfaceTypeSymbol->IsProject())
            {
                cmajor::ast::Node* node = symbolTable.GetNode(interfaceTypeSymbol);
                Assert(node->GetNodeType() == cmajor::ast::NodeType::interfaceNode, "interface node expected"); 
                cmajor::ast::InterfaceNode* interfaceNode = static_cast<cmajor::ast::InterfaceNode*>(node);
                BindInterface(interfaceTypeSymbol, interfaceNode, false);
            }
            classTypeSymbol->AddImplementedInterface(interfaceTypeSymbol);
        }
        else
        {
            throw cmajor::symbols::Exception("symbol '" + util::ToUtf8(baseOrInterfaceSymbol->FullName()) + "' is not a class or interface type symbol",
                classNode->GetFullSpan(), baseOrInterfaceNode->GetFullSpan());
        }
    }
    int nm = classNode->Members().Count();
    for (int i = 0; i < nm; ++i)
    {
        cmajor::ast::Node* member = classNode->Members()[i];
        member->Accept(*this);
    }
    boundCompileUnit.GetAttributeBinder()->BindAttributes(classNode->GetAttributes(), classTypeSymbol, boundCompileUnit, containerScope);
    classTypeSymbol->InitVmt(context);
    classTypeSymbol->InitImts(context);
    classTypeSymbol->CreateLayouts(context);
    if (classTypeSymbol->IsPolymorphic() && !classTypeSymbol->IsPrototypeTemplateSpecialization())
    {
        symbolTable.AddPolymorphicClass(classTypeSymbol);
    }
    if (classTypeSymbol->StaticConstructor())
    {
        symbolTable.AddClassHavingStaticConstructor(classTypeSymbol);
    }
    if (classTypeSymbol->HasNontrivialDestructor())
    {
        classTypeSymbol->CreateDestructorSymbol(context);
    }
    containerScope = prevContainerScope;
    currentClassTypeSymbol = prevClassTypeSymbol;
}

void TypeBinder::Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&staticConstructorNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::staticConstructorSymbol, "static constructor symbol expected"); 
    cmajor::symbols::StaticConstructorSymbol* staticConstructorSymbol = static_cast<cmajor::symbols::StaticConstructorSymbol*>(symbol);
    if (staticConstructorSymbol->IsBound()) return;
    staticConstructorSymbol->SetBound();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(staticConstructorNode.ClassId(), staticConstructorSymbol);
    }
    cmajor::symbols::FunctionSymbol* prevFunctionSymbol = currentFunctionSymbol;
    currentFunctionSymbol = staticConstructorSymbol;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = staticConstructorSymbol->GetContainerScope();
    staticConstructorSymbol->SetSpecifiers(staticConstructorNode.GetSpecifiers());
    if (staticConstructorSymbol->Parent()->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        staticConstructorSymbol->SetTemplateSpecialization();
        staticConstructorSymbol->SetLinkOnceOdrLinkage();
    }
    if (!staticConstructorSymbol->Constraint() && staticConstructorNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        staticConstructorSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(staticConstructorNode.WhereConstraint()->Clone(cloneContext)));
    }
    staticConstructorSymbol->ComputeName(context);
    cmajor::symbols::SymbolMap* symbolMap = cmajor::symbols::GetSymbolMap();
    if (symbolMap)
    {
        symbolMap->Add(staticConstructorSymbol);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && staticConstructorNode.WhereConstraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(staticConstructorNode.WhereConstraint(), containerScope, boundCompileUnit);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && staticConstructorSymbol->Constraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(staticConstructorSymbol->Constraint(), containerScope, boundCompileUnit);
    }
    if (staticConstructorNode.Body())
    {
        staticConstructorNode.Body()->Accept(*this);
    }
    else
    {
        if (!staticConstructorSymbol->IsTemplateSpecialization())
        {
            throw cmajor::symbols::Exception("static constructor has no body", staticConstructorNode.GetFullSpan());
        }
    }
    boundCompileUnit.GetAttributeBinder()->BindAttributes(staticConstructorNode.GetAttributes(), symbol, boundCompileUnit, containerScope);
    containerScope = prevContainerScope;
    currentFunctionSymbol = prevFunctionSymbol;
}

void TypeBinder::Visit(cmajor::ast::ConstructorNode& constructorNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&constructorNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol, "constructor symbol expected");
    cmajor::symbols::ConstructorSymbol* constructorSymbol = static_cast<cmajor::symbols::ConstructorSymbol*>(symbol);
    if (constructorSymbol->IsBound()) return;
    constructorSymbol->SetBound();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(constructorNode.ClassId(), constructorSymbol);
    }
    cmajor::symbols::FunctionSymbol* prevFunctionSymbol = currentFunctionSymbol;
    currentFunctionSymbol = constructorSymbol;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = constructorSymbol->GetContainerScope();
    constructorSymbol->SetSpecifiers(constructorNode.GetSpecifiers());
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && constructorSymbol->IsInline())
    {
        constructorSymbol->CloneUsingNodes(usingNodes);
    }
    cmajor::symbols::Symbol* parent = constructorSymbol->Parent();
    if (parent->IsStatic())
    {
        throw cmajor::symbols::Exception("static class cannot contain instance constructors", constructorNode.GetFullSpan(), parent->GetFullSpan());
    }
    if (parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        constructorSymbol->SetTemplateSpecialization();
        constructorSymbol->SetLinkOnceOdrLinkage();
    }
    Assert(parent->IsClassTypeSymbol(), "class type symbol expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
    int n = constructorNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = constructorNode.Parameters()[i];
        cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
        cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(parameterNode);
        Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::parameterSymbol, "parameter symbol expected"); 
        cmajor::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::symbols::ParameterSymbol*>(symbol);
        parameterSymbol->SetType(parameterType);
        if (parameterType->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
        {
            cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(parameterType);
            if (interfaceTypeSymbol->IsProject())
            {
                cmajor::ast::Node* node = symbolTable.GetNode(interfaceTypeSymbol);
                Assert(node->GetNodeType() == cmajor::ast::NodeType::interfaceNode, "interface node expected");
                cmajor::ast::InterfaceNode* interfaceNode = static_cast<cmajor::ast::InterfaceNode*>(node);
                BindInterface(interfaceTypeSymbol, interfaceNode, false);
            }
        }
    }
    if (!constructorSymbol->Constraint() && constructorNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        constructorSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(constructorNode.WhereConstraint()->Clone(cloneContext)));
    }
    constructorSymbol->ComputeName(context);
    cmajor::symbols::SymbolMap* symbolMap = cmajor::symbols::GetSymbolMap();
    if (symbolMap)
    {
        symbolMap->Add(constructorSymbol);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && constructorNode.WhereConstraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(constructorNode.WhereConstraint(), containerScope, boundCompileUnit);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && constructorSymbol->Constraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(constructorSymbol->Constraint(), containerScope, boundCompileUnit);
    }
    for (cmajor::symbols::ParameterSymbol* parameterSymbol : constructorSymbol->Parameters())
    {
        parameterSymbol->ComputeMangledName(context);
    }
    if (constructorSymbol->IsDefaultConstructor(context))
    {
        classType->SetDefaultConstructor(constructorSymbol);
    }
    else if (constructorSymbol->IsCopyConstructor(context))
    {
        classType->SetCopyConstructor(constructorSymbol);
    }
    else if (constructorSymbol->IsMoveConstructor(context))
    {
        classType->SetMoveConstructor(constructorSymbol);
    }
    else if (constructorSymbol->Arity() == 2 && !constructorSymbol->IsExplicit() && !constructorSymbol->IsGeneratedFunction())
    {
        constructorSymbol->SetConversion();
        constructorSymbol->SetConversionSourceType(constructorSymbol->Parameters()[1]->GetType()->PlainType(context));
        constructorSymbol->SetConversionTargetType(constructorSymbol->Parameters()[0]->GetType()->RemovePointer(context)->PlainType(context));
        symbolTable.AddConversion(constructorSymbol);
    }
    if (constructorNode.Body())
    {
        if ((constructorSymbol->IsDefault() || constructorSymbol->IsSuppressed()) && !constructorSymbol->HasArtificialBody())
        {
            throw cmajor::symbols::Exception("default or suppressed constructor cannot have a body", constructorNode.GetFullSpan(), constructorSymbol->GetFullSpan());
        }
        constructorNode.Body()->Accept(*this);
    }
    else
    {
        if (!constructorSymbol->IsDefault() && !constructorSymbol->IsSuppressed() && !constructorSymbol->IsTemplateSpecialization())
        {
            throw cmajor::symbols::Exception("constructor has no body", constructorNode.GetFullSpan(), constructorSymbol->GetFullSpan());
        }
    }
    boundCompileUnit.GetAttributeBinder()->BindAttributes(constructorNode.GetAttributes(), symbol, boundCompileUnit, containerScope);
    containerScope = prevContainerScope;
    currentFunctionSymbol = prevFunctionSymbol;
}

void TypeBinder::Visit(cmajor::ast::DestructorNode& destructorNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&destructorNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::destructorSymbol, "destructor symbol expected"); 
    cmajor::symbols::DestructorSymbol* destructorSymbol = static_cast<cmajor::symbols::DestructorSymbol*>(symbol);
    if (destructorSymbol->IsBound()) return;
    destructorSymbol->SetBound();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(destructorNode.ClassId(), destructorSymbol);
    }
    cmajor::symbols::FunctionSymbol* prevFunctionSymbol = currentFunctionSymbol;
    currentFunctionSymbol = destructorSymbol;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = destructorSymbol->GetContainerScope();
    destructorSymbol->SetSpecifiers(destructorNode.GetSpecifiers());
    cmajor::symbols::Symbol* parent = destructorSymbol->Parent();
    if (parent->IsStatic())
    {
        throw cmajor::symbols::Exception("static class cannot contain a destructor", destructorNode.GetFullSpan(), parent->GetFullSpan());
    }
    if (parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        destructorSymbol->SetTemplateSpecialization();
        destructorSymbol->SetLinkOnceOdrLinkage();
    }
    Assert(parent->IsClassTypeSymbol(), "class type symbol expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
    classType->SetDestructor(destructorSymbol);
    if (!destructorSymbol->Constraint() && destructorNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        destructorSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(destructorNode.WhereConstraint()->Clone(cloneContext)));
    }
    destructorSymbol->ComputeName(context);
    cmajor::symbols::SymbolMap* symbolMap = cmajor::symbols::GetSymbolMap();
    if (symbolMap)
    {
        symbolMap->Add(destructorSymbol);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && destructorNode.WhereConstraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(destructorNode.WhereConstraint(), containerScope, boundCompileUnit);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && destructorSymbol->Constraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(destructorSymbol->Constraint(), containerScope, boundCompileUnit);
    }
    if (destructorNode.Body())
    {
        if (destructorSymbol->IsDefault() && !destructorSymbol->HasArtificialBody())
        {
            throw cmajor::symbols::Exception("default destructor cannot have a body", destructorNode.GetFullSpan());
        }
        destructorNode.Body()->Accept(*this);
    }
    else
    {
        if (!destructorSymbol->IsDefault() && !destructorSymbol->IsTemplateSpecialization())
        {
            throw cmajor::symbols::Exception("destructor has no body", destructorNode.GetFullSpan());
        }
    }
    boundCompileUnit.GetAttributeBinder()->BindAttributes(destructorNode.GetAttributes(), symbol, boundCompileUnit, containerScope);
    containerScope = prevContainerScope;
    currentFunctionSymbol = prevFunctionSymbol;
}

void TypeBinder::Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&memberFunctionNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::memberFunctionSymbol, "member function symbol expected"); 
    cmajor::symbols::MemberFunctionSymbol* memberFunctionSymbol = static_cast<cmajor::symbols::MemberFunctionSymbol*>(symbol);
    if (memberFunctionSymbol->IsBound()) return;
    memberFunctionSymbol->SetBound();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(&memberFunctionNode, memberFunctionSymbol);
    }
    cmajor::symbols::FunctionSymbol* prevFunctionSymbol = currentFunctionSymbol;
    currentFunctionSymbol = memberFunctionSymbol;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = memberFunctionSymbol->GetContainerScope();
    memberFunctionSymbol->SetSpecifiers(memberFunctionNode.GetSpecifiers());
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && memberFunctionSymbol->IsInline())
    {
        memberFunctionSymbol->CloneUsingNodes(usingNodes);
    }
    cmajor::symbols::Symbol* parent = memberFunctionSymbol->Parent();
    if (parent->IsStatic() && !memberFunctionSymbol->IsStatic())
    {
        throw cmajor::symbols::Exception("static class cannot contain nonstatic member functions", memberFunctionNode.GetFullSpan(), parent->GetFullSpan());
    }
    if (parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        memberFunctionSymbol->SetTemplateSpecialization();
        memberFunctionSymbol->SetLinkOnceOdrLinkage();
    }
    int n = memberFunctionNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = memberFunctionNode.Parameters()[i];
        cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
        cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(parameterNode);
        Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::parameterSymbol, "parameter symbol expected"); 
        cmajor::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::symbols::ParameterSymbol*>(symbol);
        parameterSymbol->SetType(parameterType);
        if (parameterType->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
        {
            cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(parameterType);
            if (interfaceTypeSymbol->IsProject())
            {
                cmajor::ast::Node* node = symbolTable.GetNode(interfaceTypeSymbol);
                Assert(node->GetNodeType() == cmajor::ast::NodeType::interfaceNode, "interface node expected"); 
                cmajor::ast::InterfaceNode* interfaceNode = static_cast<cmajor::ast::InterfaceNode*>(node);
                BindInterface(interfaceTypeSymbol, interfaceNode, false);
            }
        }
    }
    cmajor::symbols::TypeSymbol* returnType = ResolveType(memberFunctionNode.ReturnTypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    memberFunctionSymbol->SetReturnType(returnType);
    if (returnType->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
    {
        cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(returnType);
        if (interfaceTypeSymbol->IsProject())
        {
            cmajor::ast::Node* node = symbolTable.GetNode(interfaceTypeSymbol);
            Assert(node->GetNodeType() == cmajor::ast::NodeType::interfaceNode, "interface node expected"); 
            cmajor::ast::InterfaceNode* interfaceNode = static_cast<cmajor::ast::InterfaceNode*>(node);
            BindInterface(interfaceTypeSymbol, interfaceNode, false);
        }
    }
    if (!memberFunctionSymbol->Constraint() && memberFunctionNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        memberFunctionSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(memberFunctionNode.WhereConstraint()->Clone(cloneContext)));
    }
    memberFunctionSymbol->ComputeName(context);
    cmajor::symbols::SymbolMap* symbolMap = cmajor::symbols::GetSymbolMap();
    if (symbolMap)
    {
        symbolMap->Add(memberFunctionSymbol);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && memberFunctionNode.WhereConstraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(memberFunctionNode.WhereConstraint(), containerScope, boundCompileUnit);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && memberFunctionSymbol->Constraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(memberFunctionSymbol->Constraint(), containerScope, boundCompileUnit);
    }
    for (cmajor::symbols::ParameterSymbol* parameterSymbol : memberFunctionSymbol->Parameters())
    {
        parameterSymbol->ComputeMangledName(context);
    }
    if (memberFunctionSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(memberFunctionNode.ReturnTypeExpr()->GetSpan(), U"@return");
        returnParam->SetParent(memberFunctionSymbol);
        returnParam->SetType(returnType->AddPointer(context));
        memberFunctionSymbol->SetReturnParam(returnParam);
    }
    if (parent->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parent);
        if (memberFunctionSymbol->IsCopyAssignment(context))
        {
            classType->SetCopyAssignment(memberFunctionSymbol);
        }
        else if (memberFunctionSymbol->IsMoveAssignment(context))
        {
            classType->SetMoveAssignment(memberFunctionSymbol);
        }
    }
    if (memberFunctionNode.Body())
    {
        if ((memberFunctionSymbol->IsDefault() || memberFunctionSymbol->IsSuppressed()) && !memberFunctionSymbol->HasArtificialBody())
        {
            throw cmajor::symbols::Exception("default or suppressed member function cannot have a body", memberFunctionNode.GetFullSpan());
        }
        memberFunctionNode.Body()->Accept(*this);
    }
    else
    {
        if (memberFunctionSymbol->Parent()->GetSymbolType() != cmajor::symbols::SymbolType::interfaceTypeSymbol &&
            !memberFunctionSymbol->IsAbstract() &&
            !memberFunctionSymbol->IsDefault() &&
            !memberFunctionSymbol->IsSuppressed() &&
            !memberFunctionSymbol->IsTemplateSpecialization())
        {
            throw cmajor::symbols::Exception("member function has no body", memberFunctionNode.GetFullSpan());
        }
    }
    boundCompileUnit.GetAttributeBinder()->BindAttributes(memberFunctionNode.GetAttributes(), symbol, boundCompileUnit, containerScope);
    containerScope = prevContainerScope;
    currentFunctionSymbol = prevFunctionSymbol;
}

void TypeBinder::Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&conversionFunctionNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol, "conversion function symbol expected");
    cmajor::symbols::ConversionFunctionSymbol* conversionFunctionSymbol = static_cast<cmajor::symbols::ConversionFunctionSymbol*>(symbol);
    if (conversionFunctionSymbol->IsBound()) return;
    conversionFunctionSymbol->SetBound();
    cmajor::symbols::FunctionSymbol* prevFunctionSymbol = currentFunctionSymbol;
    currentFunctionSymbol = conversionFunctionSymbol;
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = conversionFunctionSymbol->GetContainerScope();
    conversionFunctionSymbol->SetSpecifiers(conversionFunctionNode.GetSpecifiers());
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && conversionFunctionSymbol->IsInline())
    {
        conversionFunctionSymbol->CloneUsingNodes(usingNodes);
    }
    const cmajor::symbols::Symbol* parent = conversionFunctionSymbol->Parent();
    if (parent->IsStatic())
    {
        throw cmajor::symbols::Exception("static class cannot contain conversion functions", conversionFunctionNode.GetFullSpan(), parent->GetFullSpan());
    }
    bool requireBody = true;
    if (parent->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        conversionFunctionSymbol->SetTemplateSpecialization();
        conversionFunctionSymbol->SetLinkOnceOdrLinkage();
        requireBody = false;
    }
    cmajor::symbols::TypeSymbol* returnType = ResolveType(conversionFunctionNode.ReturnTypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    conversionFunctionSymbol->SetReturnType(returnType);
    cmajor::symbols::TypeSymbol* parentTypeSymbol = const_cast<cmajor::symbols::TypeSymbol*>(static_cast<const cmajor::symbols::TypeSymbol*>(parent));
    conversionFunctionSymbol->SetConversionSourceType(parentTypeSymbol->PlainType(context));
    conversionFunctionSymbol->SetConversionTargetType(returnType->PlainType(context));
    if (!conversionFunctionSymbol->Constraint() && conversionFunctionNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        conversionFunctionSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(conversionFunctionNode.WhereConstraint()->Clone(cloneContext)));
    }
    conversionFunctionSymbol->ComputeName(context);
    cmajor::symbols::SymbolMap* symbolMap = cmajor::symbols::GetSymbolMap();
    if (symbolMap)
    {
        symbolMap->Add(conversionFunctionSymbol);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && conversionFunctionNode.WhereConstraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(conversionFunctionNode.WhereConstraint(), containerScope, boundCompileUnit);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc) && conversionFunctionSymbol->Constraint())
    {
        constraintSymbolBinder->BindConstraintSymbols(conversionFunctionSymbol->Constraint(), containerScope, boundCompileUnit);
    }
    if (conversionFunctionSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(conversionFunctionNode.ReturnTypeExpr()->GetSpan(), U"@return");
        returnParam->SetParent(conversionFunctionSymbol);
        returnParam->SetType(returnType->AddPointer(context));
        conversionFunctionSymbol->SetReturnParam(returnParam);
    }
    symbolTable.AddConversion(conversionFunctionSymbol);
    if (conversionFunctionNode.Body())
    {
        conversionFunctionNode.Body()->Accept(*this);
    }
    else
    {
        if (requireBody)
        {
            throw cmajor::symbols::Exception("conversion function has no body", conversionFunctionNode.GetFullSpan());
        }
    }
    boundCompileUnit.GetAttributeBinder()->BindAttributes(conversionFunctionNode.GetAttributes(), symbol, boundCompileUnit, containerScope);
    containerScope = prevContainerScope;
    currentFunctionSymbol = prevFunctionSymbol;
}

void TypeBinder::Visit(cmajor::ast::MemberVariableNode& memberVariableNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&memberVariableNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::memberVariableSymbol, "member variable symbol expected"); 
    cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = static_cast<cmajor::symbols::MemberVariableSymbol*>(symbol);
    if (memberVariableSymbol->IsBound()) return;
    memberVariableSymbol->SetBound();
    memberVariableSymbol->SetSpecifiers(memberVariableNode.GetSpecifiers());
    memberVariableSymbol->ComputeMangledName(context);
    const cmajor::symbols::Symbol* parent = memberVariableSymbol->Parent();
    if (parent->IsStatic() && !memberVariableSymbol->IsStatic())
    {
        throw cmajor::symbols::Exception("static class cannot contain instance variables", memberVariableNode.GetFullSpan(), parent->GetFullSpan());
    }
    cmajor::symbols::TypeSymbol* memberVariableType = ResolveType(memberVariableNode.TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    memberVariableSymbol->SetType(memberVariableType);
    if (memberVariableType->IsClassTypeSymbol() && memberVariableType->IsProject() && !memberVariableType->IsBound() && !cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::info))
    {
        cmajor::symbols::ClassTypeSymbol* memberVariableClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(memberVariableType);
        if (memberVariableClassType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
        {
            boundCompileUnit.GetClassTemplateRepository().BindClassTemplateSpecialization(static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(memberVariableClassType),
                containerScope, &memberVariableNode);
        }
        cmajor::ast::Node* node = symbolTable.GetNode(memberVariableClassType);
        Assert(node->GetNodeType() == cmajor::ast::NodeType::classNode, "class node expected");
        cmajor::ast::ClassNode* classNode = static_cast<cmajor::ast::ClassNode*>(node);
        BindClass(memberVariableClassType, classNode, false);
    }
    boundCompileUnit.GetAttributeBinder()->BindAttributes(memberVariableNode.GetAttributes(), symbol, boundCompileUnit, containerScope);
}

void TypeBinder::Visit(cmajor::ast::InterfaceNode& interfaceNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&interfaceNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol, "interface type symbol expected");
    cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(symbol);
    BindInterface(interfaceTypeSymbol, &interfaceNode, true);
}

void TypeBinder::BindInterface(cmajor::symbols::InterfaceTypeSymbol* interfaceTypeSymbol, cmajor::ast::InterfaceNode* interfaceNode, bool fromOwnCompileUnit)
{
    if (interfaceTypeSymbol->IsBound()) return;
    interfaceTypeSymbol->SetBound();
    interfaceTypeSymbol->SetSpecifiers(interfaceNode->GetSpecifiers());
    if (!fromOwnCompileUnit)
    {
        AddUsingNodesToCurrentCompileUnit(interfaceNode);
    }
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = interfaceTypeSymbol->GetContainerScope();
    int nm = interfaceNode->Members().Count();
    if (nm == 0)
    {
        throw cmajor::symbols::Exception("interface must have at least one member function", interfaceNode->GetFullSpan());
    }
    for (int i = 0; i < nm; ++i)
    {
        cmajor::ast::Node* member = interfaceNode->Members()[i];
        member->Accept(*this);
    }
    cmajor::symbols::GetObjectPtrFromInterface* getObjectPtrFromInterface = new cmajor::symbols::GetObjectPtrFromInterface(interfaceTypeSymbol, context);
    symbolTable.SetFunctionIdFor(getObjectPtrFromInterface);
    interfaceTypeSymbol->AddMember(getObjectPtrFromInterface, context);
    cmajor::symbols::InterfaceTypeDefaultConstructor* defaultConstructor = new cmajor::symbols::InterfaceTypeDefaultConstructor(interfaceTypeSymbol, context);
    symbolTable.SetFunctionIdFor(defaultConstructor);
    interfaceTypeSymbol->AddMember(defaultConstructor, context);
    cmajor::symbols::InterfaceTypeCopyConstructor* copyConstructor = new cmajor::symbols::InterfaceTypeCopyConstructor(interfaceTypeSymbol, context);
    symbolTable.SetFunctionIdFor(copyConstructor);
    interfaceTypeSymbol->AddMember(copyConstructor, context);
    cmajor::symbols::InterfaceTypeMoveConstructor* moveConstructor = new cmajor::symbols::InterfaceTypeMoveConstructor(interfaceTypeSymbol, context);
    symbolTable.SetFunctionIdFor(moveConstructor);
    interfaceTypeSymbol->AddMember(moveConstructor, context);
    cmajor::symbols::InterfaceTypeCopyAssignment* copyAssignment = new cmajor::symbols::InterfaceTypeCopyAssignment(interfaceTypeSymbol, 
        symbolTable.GetTypeByName(U"void"), context);
    symbolTable.SetFunctionIdFor(copyAssignment);
    interfaceTypeSymbol->AddMember(copyAssignment, context);
    cmajor::symbols::InterfaceTypeMoveAssignment* moveAssignment = new cmajor::symbols::InterfaceTypeMoveAssignment(interfaceTypeSymbol, 
        symbolTable.GetTypeByName(U"void"), context);
    symbolTable.SetFunctionIdFor(moveAssignment);
    interfaceTypeSymbol->AddMember(moveAssignment, context);
    cmajor::symbols::InterfaceTypeEqual* equality = new cmajor::symbols::InterfaceTypeEqual(interfaceTypeSymbol, 
        symbolTable.GetTypeByName(U"bool"), context);
    symbolTable.SetFunctionIdFor(equality);
    interfaceTypeSymbol->Ns(boundCompileUnit.GetContext())->AddMember(equality, context);
    cmajor::symbols::InterfaceTypeLess* less = new cmajor::symbols::InterfaceTypeLess(interfaceTypeSymbol, 
        symbolTable.GetTypeByName(U"bool"), context);
    symbolTable.SetFunctionIdFor(less);
    interfaceTypeSymbol->Ns(boundCompileUnit.GetContext())->AddMember(less, context);
    boundCompileUnit.GetAttributeBinder()->BindAttributes(interfaceNode->GetAttributes(), interfaceTypeSymbol, boundCompileUnit, containerScope);
    containerScope = prevContainerScope;
}

void TypeBinder::Visit(cmajor::ast::DelegateNode& delegateNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&delegateNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol, "delegate type symbol expected"); 
    cmajor::symbols::DelegateTypeSymbol* delegateTypeSymbol = static_cast<cmajor::symbols::DelegateTypeSymbol*>(symbol);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(delegateNode.Id(), delegateTypeSymbol);
    }
    delegateTypeSymbol->SetSpecifiers(delegateNode.GetSpecifiers());
    delegateTypeSymbol->ComputeMangledName(context);
    int n = delegateNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = delegateNode.Parameters()[i];
        cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
        cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(parameterNode);
        Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::parameterSymbol, "parameter symbol expected"); 
        cmajor::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::symbols::ParameterSymbol*>(symbol);
        parameterSymbol->SetType(parameterType);
    }
    cmajor::symbols::TypeSymbol* returnType = ResolveType(delegateNode.ReturnTypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    delegateTypeSymbol->SetReturnType(returnType);
    if (delegateTypeSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(delegateNode.ReturnTypeExpr()->GetSpan(), U"@return");
        returnParam->SetParent(delegateTypeSymbol);
        returnParam->SetType(returnType->AddPointer(context));
        delegateTypeSymbol->SetReturnParam(returnParam);
    }
    for (cmajor::symbols::ParameterSymbol* parameterSymbol : delegateTypeSymbol->Parameters())
    {
        parameterSymbol->ComputeMangledName(context);
    }
    cmajor::symbols::DelegateTypeDefaultConstructor* defaultConstructor = new cmajor::symbols::DelegateTypeDefaultConstructor(delegateTypeSymbol, context);
    symbolTable.SetFunctionIdFor(defaultConstructor);
    delegateTypeSymbol->AddMember(defaultConstructor, context);
    cmajor::symbols::DelegateTypeCopyConstructor* copyConstructor = new cmajor::symbols::DelegateTypeCopyConstructor(delegateTypeSymbol, context);
    symbolTable.SetFunctionIdFor(copyConstructor);
    delegateTypeSymbol->AddMember(copyConstructor, context);
    cmajor::symbols::DelegateTypeMoveConstructor* moveConstructor = new cmajor::symbols::DelegateTypeMoveConstructor(delegateTypeSymbol, context);
    symbolTable.SetFunctionIdFor(moveConstructor);
    delegateTypeSymbol->AddMember(moveConstructor, context);
    cmajor::symbols::DelegateTypeCopyAssignment* copyAssignment = new cmajor::symbols::DelegateTypeCopyAssignment(delegateTypeSymbol, 
        symbolTable.GetTypeByName(U"void"), context);
    symbolTable.SetFunctionIdFor(copyAssignment);
    delegateTypeSymbol->AddMember(copyAssignment, context);
    cmajor::symbols::DelegateTypeMoveAssignment* moveAssignment = new cmajor::symbols::DelegateTypeMoveAssignment(delegateTypeSymbol, 
        symbolTable.GetTypeByName(U"void"), context);
    symbolTable.SetFunctionIdFor(moveAssignment);
    delegateTypeSymbol->AddMember(moveAssignment, context);
    cmajor::symbols::DelegateTypeReturn* returnFun = new cmajor::symbols::DelegateTypeReturn(delegateTypeSymbol, context);
    symbolTable.SetFunctionIdFor(returnFun);
    delegateTypeSymbol->AddMember(returnFun, context);
    cmajor::symbols::DelegateTypeEquality* equality = new cmajor::symbols::DelegateTypeEquality(delegateTypeSymbol, 
        symbolTable.GetTypeByName(U"bool"), context);
    symbolTable.SetFunctionIdFor(equality);
    delegateTypeSymbol->Ns(boundCompileUnit.GetContext())->AddMember(equality, context);
    cmajor::symbols::DelegateTypeLess* less = new cmajor::symbols::DelegateTypeLess(delegateTypeSymbol, 
        symbolTable.GetTypeByName(U"bool"), context);
    symbolTable.SetFunctionIdFor(less);
    delegateTypeSymbol->Ns(boundCompileUnit.GetContext())->AddMember(less, context);
}

void TypeBinder::Visit(cmajor::ast::ClassDelegateNode& classDelegateNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&classDelegateNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol, "class delegate type symbol expected");
    cmajor::symbols::ClassDelegateTypeSymbol* classDelegateTypeSymbol = static_cast<cmajor::symbols::ClassDelegateTypeSymbol*>(symbol);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(classDelegateNode.Id(), classDelegateTypeSymbol);
    }
    classDelegateTypeSymbol->SetSpecifiers(classDelegateNode.GetSpecifiers());
    classDelegateTypeSymbol->ComputeMangledName(context);
    cmajor::symbols::DelegateTypeSymbol* memberDelegateType = new cmajor::symbols::DelegateTypeSymbol(classDelegateNode.GetSpan(), U"delegate_type");
    memberDelegateType->SetModule(module);
    symbolTable.SetTypeIdFor(memberDelegateType);
    cmajor::symbols::ParameterSymbol* objectParam = new cmajor::symbols::ParameterSymbol(classDelegateNode.GetSpan(), U"@obj");
    cmajor::symbols::TypeSymbol* voidPtrType = symbolTable.GetTypeByName(U"void")->AddPointer(context);
    objectParam->SetType(voidPtrType);
    memberDelegateType->AddMember(objectParam, context);
    int n = classDelegateNode.Parameters().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::ParameterNode* parameterNode = classDelegateNode.Parameters()[i];
        cmajor::symbols::TypeSymbol* parameterType = ResolveType(parameterNode->TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
        cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(parameterNode);
        Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::parameterSymbol, "parameter symbol expected"); 
        cmajor::symbols::ParameterSymbol* parameterSymbol = static_cast<cmajor::symbols::ParameterSymbol*>(symbol);
        parameterSymbol->SetType(parameterType);
        cmajor::symbols::ParameterSymbol* memberParam = new cmajor::symbols::ParameterSymbol(classDelegateNode.GetSpan(), util::ToUtf32("@p" + std::to_string(i)));
        memberParam->SetType(parameterType);
        memberDelegateType->AddMember(memberParam, context);
    }
    cmajor::symbols::TypeSymbol* returnType = ResolveType(classDelegateNode.ReturnTypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    classDelegateTypeSymbol->SetReturnType(returnType);
    if (classDelegateTypeSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(classDelegateNode.ReturnTypeExpr()->GetSpan(), U"@return");
        returnParam->SetParent(classDelegateTypeSymbol);
        returnParam->SetType(returnType->AddPointer(context));
        classDelegateTypeSymbol->SetReturnParam(returnParam);
        cmajor::symbols::ParameterSymbol* memberReturnParam = new cmajor::symbols::ParameterSymbol(classDelegateNode.ReturnTypeExpr()->GetSpan(), U"@return");
        memberReturnParam->SetParent(memberDelegateType);
        memberReturnParam->SetType(returnType->AddPointer(context));
        memberDelegateType->SetReturnParam(memberReturnParam);
    }
    memberDelegateType->SetReturnType(returnType);
    for (cmajor::symbols::ParameterSymbol* parameterSymbol : classDelegateTypeSymbol->Parameters())
    {
        parameterSymbol->ComputeMangledName(context);
    }
    classDelegateTypeSymbol->AddMember(memberDelegateType, context);
    cmajor::symbols::ClassTypeSymbol* objectDelegatePairType = new cmajor::symbols::ClassTypeSymbol(classDelegateNode.GetSpan(), U"@objectDelegatePairType");
    objectDelegatePairType->SetModule(context->RootModule());
    objectDelegatePairType->SetAccess(cmajor::symbols::SymbolAccess::public_);
    objectDelegatePairType->SetGroupName(U"@objectDelegatePairType");
    cmajor::symbols::MemberVariableSymbol* objVar = new cmajor::symbols::MemberVariableSymbol(classDelegateNode.GetSpan(), U"obj");
    objVar->SetAccess(cmajor::symbols::SymbolAccess::public_);
    objVar->SetType(voidPtrType);
    cmajor::symbols::MemberVariableSymbol* dlgVar = new cmajor::symbols::MemberVariableSymbol(classDelegateNode.GetSpan(), U"dlg");
    dlgVar->SetAccess(cmajor::symbols::SymbolAccess::public_);
    dlgVar->SetType(memberDelegateType);
    objectDelegatePairType->AddMember(objVar, context);
    objectDelegatePairType->AddMember(dlgVar, context);
    symbolTable.SetTypeIdFor(objectDelegatePairType);
    objectDelegatePairType->InitVmt(context);
    objectDelegatePairType->InitImts(context);
    objectDelegatePairType->CreateLayouts(context);
    objectDelegatePairType->SetBound();
    classDelegateTypeSymbol->AddMember(objectDelegatePairType, context);
    cmajor::symbols::ClassDelegateTypeDefaultConstructor* defaultConstructor = new cmajor::symbols::ClassDelegateTypeDefaultConstructor(classDelegateTypeSymbol, context);
    symbolTable.SetFunctionIdFor(defaultConstructor);
    classDelegateTypeSymbol->AddMember(defaultConstructor, context);
    cmajor::symbols::ClassDelegateTypeCopyConstructor* copyConstructor = new cmajor::symbols::ClassDelegateTypeCopyConstructor(classDelegateTypeSymbol, context);
    symbolTable.SetFunctionIdFor(copyConstructor);
    classDelegateTypeSymbol->AddMember(copyConstructor, context);
    cmajor::symbols::ClassDelegateTypeMoveConstructor* moveConstructor = new cmajor::symbols::ClassDelegateTypeMoveConstructor(classDelegateTypeSymbol, context);
    symbolTable.SetFunctionIdFor(moveConstructor);
    classDelegateTypeSymbol->AddMember(moveConstructor, context);
    cmajor::symbols::ClassDelegateTypeCopyAssignment* copyAssignment = new cmajor::symbols::ClassDelegateTypeCopyAssignment(classDelegateTypeSymbol, 
        symbolTable.GetTypeByName(U"void"), context);
    symbolTable.SetFunctionIdFor(copyAssignment);
    classDelegateTypeSymbol->AddMember(copyAssignment, context);
    cmajor::symbols::ClassDelegateTypeMoveAssignment* moveAssignment = new cmajor::symbols::ClassDelegateTypeMoveAssignment(classDelegateTypeSymbol, 
        symbolTable.GetTypeByName(U"void"), context);
    symbolTable.SetFunctionIdFor(moveAssignment);
    classDelegateTypeSymbol->AddMember(moveAssignment, context);
    cmajor::symbols::ClassDelegateTypeEquality* equality = new cmajor::symbols::ClassDelegateTypeEquality(classDelegateTypeSymbol, 
        symbolTable.GetTypeByName(U"bool"), context);
    symbolTable.SetFunctionIdFor(equality);
    classDelegateTypeSymbol->Ns(boundCompileUnit.GetContext())->AddMember(equality, context);
    cmajor::symbols::ClassDelegateTypeLess* less = new cmajor::symbols::ClassDelegateTypeLess(classDelegateTypeSymbol, 
        symbolTable.GetTypeByName(U"bool"), context);
    symbolTable.SetFunctionIdFor(less);
    classDelegateTypeSymbol->Ns(boundCompileUnit.GetContext())->AddMember(less, context);
}

void TypeBinder::Visit(cmajor::ast::ConceptNode& conceptNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&conceptNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::conceptSymbol, "concept symbol expected"); 
    cmajor::symbols::ConceptSymbol* conceptSymbol = static_cast<cmajor::symbols::ConceptSymbol*>(symbol);
    BindConcept(conceptSymbol, &conceptNode);
}

void TypeBinder::BindConcept(cmajor::symbols::ConceptSymbol* conceptSymbol, cmajor::ast::ConceptNode* conceptNode)
{
    if (conceptSymbol->IsBound()) return;
    conceptSymbol->SetBound();
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = conceptSymbol->GetContainerScope();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(conceptNode->Id(), conceptSymbol);
        int n = conceptNode->TypeParameters().Count();
        for (int i = 0; i < n; ++i)
        {
            symbolTable.MapSymbol(conceptNode->TypeParameters()[i], conceptSymbol->TemplateParameters()[i]);
        }
        constraintSymbolBinder->BindConstraintSymbols(conceptNode, containerScope, boundCompileUnit);
    }
    conceptSymbol->SetSpecifiers(conceptNode->GetSpecifiers());
    conceptSymbol->ComputeName(context);
    if (conceptNode->Refinement())
    {
        cmajor::ast::ConceptIdNode* refinedConceptIdNode = conceptNode->Refinement();
        cmajor::symbols::ConceptSymbol* refinedConceptSymbol = ResolveConceptId(refinedConceptIdNode, boundCompileUnit, containerScope);
        if (refinedConceptSymbol->IsProject())
        {
            cmajor::ast::Node* node = symbolTable.GetNode(refinedConceptSymbol);
            Assert(node->GetNodeType() == cmajor::ast::NodeType::conceptNode, "concept node expected"); 
            cmajor::ast::ConceptNode* refinedConceptNode = static_cast<cmajor::ast::ConceptNode*>(node);
            BindConcept(refinedConceptSymbol, refinedConceptNode);
        }
        conceptSymbol->SetRefinedConcept(refinedConceptSymbol);
    }
    containerScope = prevContainerScope;
}

void TypeBinder::Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&compoundStatementNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::declarationBlock, "declaration block expected");
    cmajor::symbols::DeclarationBlock* declarationBlock = static_cast<cmajor::symbols::DeclarationBlock*>(symbol);
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = declarationBlock->GetContainerScope();
    int n = compoundStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statementNode = compoundStatementNode.Statements()[i];
        statementNode->Accept(*this);
    }
    containerScope = prevContainerScope;
}

void TypeBinder::Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode)
{
    labeledStatementNode.Stmt()->Accept(*this);
}

void TypeBinder::Visit(cmajor::ast::IfStatementNode& ifStatementNode)
{
    ifStatementNode.ThenS()->Accept(*this);
    if (ifStatementNode.ElseS())
    {
        ifStatementNode.ElseS()->Accept(*this);
    }
}

void TypeBinder::Visit(cmajor::ast::WhileStatementNode& whileStatementNode)
{
    whileStatementNode.Statement()->Accept(*this);
}

void TypeBinder::Visit(cmajor::ast::DoStatementNode& doStatementNode)
{
    doStatementNode.Statement()->Accept(*this);
}

void TypeBinder::Visit(cmajor::ast::ForStatementNode& forStatementNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&forStatementNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::declarationBlock, "declaration block expected");
    cmajor::symbols::DeclarationBlock* declarationBlock = static_cast<cmajor::symbols::DeclarationBlock*>(symbol);
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = declarationBlock->GetContainerScope();
    forStatementNode.InitS()->Accept(*this);
    forStatementNode.ActionS()->Accept(*this);
    containerScope = prevContainerScope;
}

void TypeBinder::Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&constructionStatementNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::localVariableSymbol, "local variable symbol expected"); 
    cmajor::symbols::LocalVariableSymbol* localVariableSymbol = static_cast<cmajor::symbols::LocalVariableSymbol*>(symbol);
    cmajor::symbols::TypeSymbol* type = ResolveType(constructionStatementNode.TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    localVariableSymbol->SetType(type);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::trace))
    {
        if (localVariableSymbol->Name() == U"@tracer" && constructionStatementNode.Arguments().Count() == 1)
        {
            cmajor::ast::Node* argumentNode = constructionStatementNode.Arguments()[0];
            if (argumentNode->GetNodeType() == cmajor::ast::NodeType::intLiteralNode)
            {
                cmajor::ast::IntLiteralNode* intLiteralNode = static_cast<cmajor::ast::IntLiteralNode*>(argumentNode);
                if (currentFunctionSymbol)
                {
                    int32_t traceFunctionId = cmajor::symbols::GetTraceFunctionId(util::ToUtf8(currentFunctionSymbol->FullName()));
                    intLiteralNode->SetValue(traceFunctionId);
                }
            }
        }
    }
}

void TypeBinder::Visit(cmajor::ast::SwitchStatementNode& switchStatementNode)
{
    int n = switchStatementNode.Cases().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::CaseStatementNode* caseStatement = switchStatementNode.Cases()[i];
        caseStatement->Accept(*this);
    }
    if (switchStatementNode.Default())
    {
        switchStatementNode.Default()->Accept(*this);
    }
}

void TypeBinder::Visit(cmajor::ast::CaseStatementNode& caseStatementNode)
{
    int n = caseStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statementNode = caseStatementNode.Statements()[i];
        statementNode->Accept(*this);
    }
}

void TypeBinder::Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode)
{
    int n = defaultStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statementNode = defaultStatementNode.Statements()[i];
        statementNode->Accept(*this);
    }
}

void TypeBinder::Visit(cmajor::ast::TryStatementNode& tryStatementNode)
{
    currentFunctionSymbol->SetHasTry();
    tryStatementNode.TryBlock()->Accept(*this);
    int n = tryStatementNode.Catches().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::CatchNode* catchNode = tryStatementNode.Catches()[i];
        catchNode->Accept(*this);
    }
}

void TypeBinder::Visit(cmajor::ast::CatchNode& catchNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&catchNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::declarationBlock, "declaration block expected"); 
    cmajor::symbols::DeclarationBlock* declarationBlock = static_cast<cmajor::symbols::DeclarationBlock*>(symbol);
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = declarationBlock->GetContainerScope();
    if (catchNode.Id())
    {
        cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(catchNode.Id());
        Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::localVariableSymbol, "local variable symbol expected");
        cmajor::symbols::LocalVariableSymbol* exceptionVarSymbol = static_cast<cmajor::symbols::LocalVariableSymbol*>(symbol);
        cmajor::symbols::TypeSymbol* type = ResolveType(catchNode.TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
        if (type->BaseType()->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* exceptionVarClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type->BaseType());
            cmajor::ast::IdentifierNode systemExceptionNode(catchNode.GetSpan(), U"System.Exception");
            cmajor::symbols::TypeSymbol* systemExceptionType = ResolveType(&systemExceptionNode, boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
            Assert(systemExceptionType->IsClassTypeSymbol(), "System.Exception not of class type"); 
            cmajor::symbols::ClassTypeSymbol* systemExceptionClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(systemExceptionType);
            if (exceptionVarClassType->IsProject())
            {
                cmajor::ast::Node* exceptionVarNode = symbolTable.GetNode(exceptionVarClassType);
                Assert(exceptionVarNode->GetNodeType() == cmajor::ast::NodeType::classNode, "class node expected"); 
                cmajor::ast::ClassNode* exceptionVarClassNode = static_cast<cmajor::ast::ClassNode*>(exceptionVarNode);
                BindClass(exceptionVarClassType, exceptionVarClassNode, false);
            }
            if (exceptionVarClassType == systemExceptionClassType || exceptionVarClassType->HasBaseClass(systemExceptionClassType))
            {
                exceptionVarSymbol->SetType(type);
            }
            else
            {
                throw cmajor::symbols::Exception("exception variable must be of class type equal to System.Exception class or derive from it",
                    catchNode.GetFullSpan(), catchNode.TypeExpr()->GetFullSpan());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("exception variable must be of class type equal to System.Exception class or derive from it", 
                catchNode.GetFullSpan(), catchNode.TypeExpr()->GetFullSpan());
        }
    }
    catchNode.CatchBlock()->Accept(*this);
    containerScope = prevContainerScope;
}

void TypeBinder::Visit(cmajor::ast::ConditionalCompilationPartNode& conditionalCompilationPartNode)
{
    conditionalCompilationPartNode.Expr()->Accept(*this);
}

void TypeBinder::Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode)
{
    conditionalCompilationDisjunctionNode.Left()->Accept(*this);
    bool left = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationDisjunctionNode.Right()->Accept(*this);
    bool right = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationStack.push(left || right);
}

void TypeBinder::Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode)
{
    conditionalCompilationConjunctionNode.Left()->Accept(*this);
    bool left = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationConjunctionNode.Right()->Accept(*this);
    bool right = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationStack.push(left && right);
}

void TypeBinder::Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode)
{
    conditionalCompilationNotNode.Expr()->Accept(*this);
    bool operand = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationStack.push(!operand);
}

void TypeBinder::Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode)
{
    bool defined = module->IsSymbolDefined(conditionalCompilationPrimaryNode.Symbol());
    conditionalCompilationStack.push(defined);
}

void TypeBinder::Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode)
{
    conditionalCompilationStatementNode.IfPart()->Accept(*this);
    bool defined = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    if (defined)
    {
        int n = conditionalCompilationStatementNode.IfPart()->Statements().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::StatementNode* statement = conditionalCompilationStatementNode.IfPart()->Statements()[i];
            statement->Accept(*this);
        }
    }
    else
    {
        bool executed = false;
        int n = conditionalCompilationStatementNode.ElifParts().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::ConditionalCompilationPartNode* elifPart = conditionalCompilationStatementNode.ElifParts()[i];
            elifPart->Accept(*this);
            bool defined = conditionalCompilationStack.top();
            conditionalCompilationStack.pop();
            if (defined)
            {
                int n = elifPart->Statements().Count();
                for (int i = 0; i < n; ++i)
                {
                    cmajor::ast::StatementNode* statement = elifPart->Statements()[i];
                    statement->Accept(*this);
                }
                executed = true;
                break;
            }
        }
        if (!executed)
        {
            cmajor::ast::ConditionalCompilationPartNode* elsePart = conditionalCompilationStatementNode.ElsePart();
            if (elsePart)
            {
                int n = elsePart->Statements().Count();
                for (int i = 0; i < n; ++i)
                {
                    cmajor::ast::StatementNode* statement = elsePart->Statements()[i];
                    statement->Accept(*this);
                }
            }
        }
    }
}

void TypeBinder::Visit(cmajor::ast::FullInstantiationRequestNode& fullInstantiationRequestNode)
{
    cmajor::symbols::TypeSymbol* type = ResolveType(fullInstantiationRequestNode.TemplateId(), boundCompileUnit, containerScope, TypeResolverFlags::doNotBind);
    if (type->GetSymbolType() != cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        throw cmajor::symbols::Exception("full instantiation request expects subject template identifier to be a class template specialization",
            fullInstantiationRequestNode.GetFullSpan());
    }
    cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
    specialization->SetHasFullInstantiation();
}

void TypeBinder::Visit(cmajor::ast::TypedefNode& typedefNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&typedefNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::aliasTypeSymbol, "alias type symbol expected");
    cmajor::symbols::AliasTypeSymbol* aliasTypeSymbol = static_cast<cmajor::symbols::AliasTypeSymbol*>(symbol);
    BindTypedef(aliasTypeSymbol, &typedefNode, true);
}

void TypeBinder::BindTypedef(cmajor::symbols::AliasTypeSymbol* aliasTypeSymbol, cmajor::ast::TypedefNode* typedefNode, bool fromOwnCompileUnit)
{
    if (aliasTypeSymbol->IsBound()) return;
    aliasTypeSymbol->SetBound();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(typedefNode->Id(), aliasTypeSymbol);
    }
    aliasTypeSymbol->SetSpecifiers(typedefNode->GetSpecifiers());
    aliasTypeSymbol->ComputeMangledName(context);
    if (!fromOwnCompileUnit)
    {
        AddUsingNodesToCurrentCompileUnit(typedefNode);
    }
    cmajor::symbols::TypeSymbol* typeSymbol = ResolveType(typedefNode->TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    aliasTypeSymbol->SetType(typeSymbol);
}

void TypeBinder::BindAlias(cmajor::symbols::AliasTypeSymbol* aliasTypeSymbol, cmajor::ast::AliasNode* aliasNode, bool fromOwnCompileUnit)
{
    if (aliasTypeSymbol->IsBound()) return;
    aliasTypeSymbol->SetBound();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(aliasNode->Id(), aliasTypeSymbol);
    }
    aliasTypeSymbol->SetSpecifiers(aliasNode->GetSpecifiers());
    aliasTypeSymbol->ComputeMangledName(context);
    cmajor::symbols::TypeSymbol* typeSymbol = ResolveType(aliasNode->TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    aliasTypeSymbol->SetType(typeSymbol);
    if (!fromOwnCompileUnit)
    {
        AddUsingNodesToCurrentCompileUnit(aliasNode);
    }
}

void TypeBinder::Visit(cmajor::ast::ConstantNode& constantNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&constantNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::constantSymbol, "constant symbol expected");
    cmajor::symbols::ConstantSymbol* constantSymbol = static_cast<cmajor::symbols::ConstantSymbol*>(symbol);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(constantNode.Id(), constantSymbol);
    }
    constantSymbol->SetSpecifiers(constantNode.GetSpecifiers());
    constantSymbol->ComputeMangledName(context);
    cmajor::symbols::TypeSymbol* typeSymbol = ResolveType(constantNode.TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    constantSymbol->SetType(typeSymbol);
    constantSymbol->SetEvaluating();
    std::unique_ptr<cmajor::symbols::Value> value;
    value = Evaluate(constantNode.Value(), typeSymbol, containerScope, boundCompileUnit, false, nullptr);
    cmajor::symbols::Value* val = value.get();
    if (val)
    {
        constantSymbol->SetType(value->GetType(&symbolTable, context));
        constantSymbol->SetValue(value.release());
    }
    constantSymbol->ResetEvaluating();
    if (val)
    {
        if (val->GetValueType() == cmajor::symbols::ValueType::arrayValue)
        {
            boundCompileUnit.AddConstantArray(constantSymbol);
        }
        else if (val->GetValueType() == cmajor::symbols::ValueType::structuredValue)
        {
            boundCompileUnit.AddConstantStructure(constantSymbol);
        }
    }
}

void TypeBinder::Visit(cmajor::ast::EnumTypeNode& enumTypeNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&enumTypeNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::enumTypeSymbol, "enumerated type symbol expected"); 
    cmajor::symbols::EnumTypeSymbol* enumTypeSymbol = static_cast<cmajor::symbols::EnumTypeSymbol*>(symbol);
    if (enumTypeSymbol->IsBound()) return;
    enumTypeSymbol->SetBound();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(enumTypeNode.Id(), enumTypeSymbol);
    }
    cmajor::symbols::EnumTypeSymbol* prevEnumType = enumType;
    enumType = enumTypeSymbol;
    enumTypeSymbol->SetSpecifiers(enumTypeNode.GetSpecifiers());
    enumTypeSymbol->ComputeMangledName(context);
    cmajor::symbols::TypeSymbol* underlyingType = symbolTable.GetTypeByName(U"int");
    if (enumTypeNode.GetUnderlyingType())
    {
        underlyingType = ResolveType(enumTypeNode.GetUnderlyingType(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    }
    enumTypeSymbol->SetUnderlyingType(underlyingType);
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = enumTypeSymbol->GetContainerScope();
    int n = enumTypeNode.Constants().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::EnumConstantNode* enumConstantNode = enumTypeNode.Constants()[i];
        enumConstantNode->Accept(*this);
    }
    cmajor::symbols::AliasTypeSymbol* underlyingAliasType = new cmajor::symbols::AliasTypeSymbol(enumTypeNode.GetSpan(), U"UnderlyingType");
    underlyingAliasType->SetModule(module);
    underlyingAliasType->SetAccess(cmajor::symbols::SymbolAccess::public_);
    underlyingAliasType->SetType(underlyingType);
    underlyingAliasType->SetBound();
    enumTypeSymbol->AddMember(underlyingAliasType, context);
    cmajor::symbols::EnumTypeDefaultConstructor* defaultConstructor = new cmajor::symbols::EnumTypeDefaultConstructor(enumTypeSymbol, context);
    symbolTable.SetFunctionIdFor(defaultConstructor);
    enumTypeSymbol->AddMember(defaultConstructor, context);
    cmajor::symbols::EnumTypeCopyConstructor* copyConstructor = new cmajor::symbols::EnumTypeCopyConstructor(enumTypeSymbol, context);
    symbolTable.SetFunctionIdFor(copyConstructor);
    enumTypeSymbol->AddMember(copyConstructor, context);
    cmajor::symbols::EnumTypeMoveConstructor* moveConstructor = new cmajor::symbols::EnumTypeMoveConstructor(enumTypeSymbol, context);
    symbolTable.SetFunctionIdFor(moveConstructor);
    enumTypeSymbol->AddMember(moveConstructor, context);
    cmajor::symbols::EnumTypeCopyAssignment* copyAssignment = new cmajor::symbols::EnumTypeCopyAssignment(enumTypeSymbol, 
        symbolTable.GetTypeByName(U"void"), context);
    symbolTable.SetFunctionIdFor(copyAssignment);
    enumTypeSymbol->AddMember(copyAssignment, context);
    cmajor::symbols::EnumTypeMoveAssignment* moveAssignment = new cmajor::symbols::EnumTypeMoveAssignment(enumTypeSymbol, 
        symbolTable.GetTypeByName(U"void"), context);
    symbolTable.SetFunctionIdFor(moveAssignment);
    enumTypeSymbol->AddMember(moveAssignment, context);
    cmajor::symbols::EnumTypeReturn* returnFun = new cmajor::symbols::EnumTypeReturn(enumTypeSymbol, context);
    symbolTable.SetFunctionIdFor(returnFun);
    enumTypeSymbol->AddMember(returnFun, context);
    cmajor::symbols::EnumTypeEqualityOp* equality = new cmajor::symbols::EnumTypeEqualityOp(enumTypeSymbol, 
        symbolTable.GetTypeByName(U"bool"), context);
    symbolTable.SetFunctionIdFor(equality);
    enumTypeSymbol->Ns(boundCompileUnit.GetContext())->AddMember(equality, context);
    cmajor::symbols::EnumTypeToUnderlyingTypeConversion* enum2underlying = new cmajor::symbols::EnumTypeToUnderlyingTypeConversion(
        enumTypeNode.GetSpan(), U"enum2underlying", enumTypeSymbol, underlyingType, context);
    symbolTable.SetFunctionIdFor(enum2underlying);
    enum2underlying->SetParent(enumTypeSymbol);
    symbolTable.AddConversion(enum2underlying);
    enumTypeSymbol->AddMember(enum2underlying, context);
    cmajor::symbols::UnderlyingTypeToEnumTypeConversion* underlying2enum = new cmajor::symbols::UnderlyingTypeToEnumTypeConversion(
        enumTypeNode.GetSpan(), U"underlying2enum", underlyingType, enumTypeSymbol, context);
    symbolTable.SetFunctionIdFor(underlying2enum);
    underlying2enum->SetParent(enumTypeSymbol);
    symbolTable.AddConversion(underlying2enum);
    enumTypeSymbol->AddMember(underlying2enum, context);
    containerScope = prevContainerScope;
    enumType = prevEnumType;
}

void TypeBinder::Visit(cmajor::ast::EnumConstantNode& enumConstantNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&enumConstantNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::enumConstantSymbol, "enumeration constant symbol expected");
    cmajor::symbols::EnumConstantSymbol* enumConstantSymbol = static_cast<cmajor::symbols::EnumConstantSymbol*>(symbol);
    if (enumConstantSymbol->IsBound()) return;
    enumConstantSymbol->SetBound();
    enumConstantSymbol->ComputeMangledName(context);
    enumConstantSymbol->SetEvaluating();
    std::unique_ptr<cmajor::symbols::Value> value = Evaluate(enumConstantNode.GetValue(), enumType->UnderlyingType(), containerScope, boundCompileUnit, false, nullptr);
    enumConstantSymbol->SetValue(value.release());
    if (enumConstantNode.StrValue().empty())
    {
        enumConstantSymbol->SetStrValue(enumConstantSymbol->GetValue()->ToString());
    }
    else
    {
        enumConstantSymbol->SetStrValue(enumConstantNode.StrValue());
    }
    enumConstantSymbol->ResetEvaluating();
}

void TypeBinder::Visit(cmajor::ast::GlobalVariableNode& globalVariableNode)
{
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&globalVariableNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::globalVariableSymbol, "global variable symbol expected");
    cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol = static_cast<cmajor::symbols::GlobalVariableSymbol*>(symbol);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
    {
        symbolTable.MapSymbol(globalVariableNode.Id(), globalVariableSymbol);
    }
    if (globalVariableSymbol->IsBound()) return;
    globalVariableSymbol->SetBound();
    globalVariableSymbol->SetSpecifiers(globalVariableNode.GetSpecifiers());
    globalVariableSymbol->ComputeMangledName(context);
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    containerScope = globalVariableSymbol->GetContainerScope();
    cmajor::symbols::TypeSymbol* typeSymbol = ResolveType(globalVariableNode.TypeExpr(), boundCompileUnit, containerScope, typeResolverFlags, currentClassTypeSymbol);
    globalVariableSymbol->SetType(typeSymbol);
    if (globalVariableNode.Initializer())
    {
        std::unique_ptr<cmajor::symbols::Value> value;
        value = Evaluate(globalVariableNode.Initializer(), typeSymbol, containerScope, boundCompileUnit, false, nullptr);
        cmajor::symbols::Value* val = value.get();
        if (val)
        {
            globalVariableSymbol->SetType(value->GetType(&symbolTable, context));
            globalVariableSymbol->SetInitializer(std::move(value));
        }
    }
    boundGlobalVariable = new BoundGlobalVariable(globalVariableNode.GetSpan(), globalVariableSymbol);
    boundCompileUnit.AddBoundNode(std::unique_ptr<BoundNode>(boundGlobalVariable));
    containerScope = prevContainerScope;
}

void TypeBinder::CreateMemberSymbols()
{
    typeResolverFlags = typeResolverFlags | TypeResolverFlags::createMemberSymbols;
}

void BindClass(cmajor::symbols::ClassTypeSymbol* classType, void* boundCompileUnit)
{
    BoundCompileUnit* compileUnit = static_cast<BoundCompileUnit*>(boundCompileUnit);
    TypeBinder binder(*compileUnit);
    cmajor::symbols::SymbolTable& symbolTable = compileUnit->GetSymbolTable();
    cmajor::ast::ClassNode* classNode = static_cast<cmajor::ast::ClassNode*>(symbolTable.GetNode(classType));
    binder.BindClass(classType, classNode, true);
}

} // namespace cmajor::binder
