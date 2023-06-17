// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.statement.binder;

import cmajor.binder.bound_namespace;
import cmajor.binder.bound_class;
import cmajor.binder.bound.function;
import cmajor.binder.bound.statement;
import cmajor.binder.bound.expression;
import cmajor.binder.overload.resolution;
import cmajor.binder.expression.binder;
import cmajor.binder.access;
import cmajor.binder.operation.repository;
import cmajor.binder.evaluator;
import cmajor.binder.operation.repository;
import cmajor.binder.evaluator;
import cmajor.binder.type.binder;
import cmajor.binder.type.resolver;
import cmajor.binder.attribute.binder;
import cmajor.binder.bound.enumerations;
import cmajor.binder.bound.compile.unit;
import cmajor.parsers;
import cmajor.lexers;
import util;

namespace cmajor::binder {

bool IsAlwaysTrue(cmajor::ast::Node* node, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    std::unique_ptr<cmajor::symbols::Value> value = Evaluate(node, boundCompileUnit.GetSymbolTable().GetTypeByName(U"bool"), containerScope, boundCompileUnit, true, nullptr, node->GetSourcePos(), node->ModuleId());
    if (value)
    {
        if (value->GetValueType() == cmajor::symbols::ValueType::boolValue)
        {
            cmajor::symbols::BoolValue* boolValue = static_cast<cmajor::symbols::BoolValue*>(value.get());
            return boolValue->GetValue() == true;
        }
    }
    return false;
}

bool TerminatesFunction(cmajor::ast::StatementNode* statement, bool inForEverLoop, cmajor::symbols::ContainerScope* containerScope, BoundCompileUnit& boundCompileUnit)
{
    switch (statement->GetNodeType())
    {
    case cmajor::ast::NodeType::compoundStatementNode:
    {
        cmajor::ast::CompoundStatementNode* compoundStatement = static_cast<cmajor::ast::CompoundStatementNode*>(statement);
        int n = compoundStatement->Statements().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::StatementNode* statement = compoundStatement->Statements()[i];
            if (TerminatesFunction(statement, inForEverLoop, containerScope, boundCompileUnit)) return true;
        }
        break;
    }
    case cmajor::ast::NodeType::ifStatementNode:
    {
        cmajor::ast::IfStatementNode* ifStatement = static_cast<cmajor::ast::IfStatementNode*>(statement);
        if (inForEverLoop || ifStatement->ElseS())
        {
            if (TerminatesFunction(ifStatement->ThenS(), inForEverLoop, containerScope, boundCompileUnit) &&
                inForEverLoop || (ifStatement->ElseS() && TerminatesFunction(ifStatement->ElseS(), inForEverLoop, containerScope, boundCompileUnit)))
            {
                return true;
            }
        }
        break;
    }
    case cmajor::ast::NodeType::whileStatementNode:
    {
        cmajor::ast::WhileStatementNode* whileStatement = static_cast<cmajor::ast::WhileStatementNode*>(statement);
        if (IsAlwaysTrue(whileStatement->Condition(), boundCompileUnit, containerScope))
        {
            if (TerminatesFunction(whileStatement->Statement(), true, containerScope, boundCompileUnit)) return true;
        }
        break;
    }
    case cmajor::ast::NodeType::doStatementNode:
    {
        cmajor::ast::DoStatementNode* doStatement = static_cast<cmajor::ast::DoStatementNode*>(statement);
        if (IsAlwaysTrue(doStatement->Condition(), boundCompileUnit, containerScope))
        {
            if (TerminatesFunction(doStatement->Statement(), true, containerScope, boundCompileUnit)) return true;
        }
        break;
    }
    case cmajor::ast::NodeType::forStatementNode:
    {
        cmajor::ast::ForStatementNode* forStatement = static_cast<cmajor::ast::ForStatementNode*>(statement);
        if (!forStatement->Condition() || IsAlwaysTrue(forStatement->Condition(), boundCompileUnit, containerScope))
        {
            if (TerminatesFunction(forStatement->ActionS(), true, containerScope, boundCompileUnit)) return true;
        }
        break;
    }
    default:
    {
        if (statement->IsFunctionTerminatingNode())
        {
            return true;
        }
        break;
    }
    }
    return false;
}

bool TerminatesCase(cmajor::ast::StatementNode* statementNode)
{
    if (statementNode->GetNodeType() == cmajor::ast::NodeType::ifStatementNode)
    {
        cmajor::ast::IfStatementNode* ifStatementNode = static_cast<cmajor::ast::IfStatementNode*>(statementNode);
        if (ifStatementNode->ElseS())
        {
            if (TerminatesCase(ifStatementNode->ThenS()) && TerminatesCase(ifStatementNode->ElseS()))
            {
                return true;
            }
        }
    }
    else if (statementNode->GetNodeType() == cmajor::ast::NodeType::compoundStatementNode)
    {
        cmajor::ast::CompoundStatementNode* compoundStatement = static_cast<cmajor::ast::CompoundStatementNode*>(statementNode);
        int n = compoundStatement->Statements().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::StatementNode* statementNode = compoundStatement->Statements()[i];
            if (TerminatesCase(statementNode))
            {
                return true;
            }
        }
    }
    else
    {
        return statementNode->IsCaseTerminatingNode();
    }
    return false;
}

bool TerminatesDefault(cmajor::ast::StatementNode* statementNode)
{
    if (statementNode->GetNodeType() == cmajor::ast::NodeType::ifStatementNode)
    {
        cmajor::ast::IfStatementNode* ifStatementNode = static_cast<cmajor::ast::IfStatementNode*>(statementNode);
        if (ifStatementNode->ElseS())
        {
            if (TerminatesDefault(ifStatementNode->ThenS()) && TerminatesDefault(ifStatementNode->ElseS()))
            {
                return true;
            }
        }
    }
    else if (statementNode->GetNodeType() == cmajor::ast::NodeType::compoundStatementNode)
    {
        cmajor::ast::CompoundStatementNode* compoundStatement = static_cast<cmajor::ast::CompoundStatementNode*>(statementNode);
        int n = compoundStatement->Statements().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::StatementNode* statementNode = compoundStatement->Statements()[i];
            if (TerminatesDefault(statementNode))
            {
                return true;
            }
        }
    }
    else
    {
        return statementNode->IsDefaultTerminatingNode();
    }
    return false;
}

void CheckFunctionReturnPaths(cmajor::symbols::FunctionSymbol* functionSymbol, cmajor::ast::CompoundStatementNode* bodyNode, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    cmajor::symbols::ContainerScope* containerScope, BoundCompileUnit& boundCompileUnit);

void CheckFunctionReturnPaths(cmajor::symbols::FunctionSymbol* functionSymbol, cmajor::ast::FunctionNode& functionNode, cmajor::symbols::ContainerScope* containerScope, BoundCompileUnit& boundCompileUnit)
{
    CheckFunctionReturnPaths(functionSymbol, functionNode.Body(), functionNode.GetSourcePos(), functionNode.ModuleId(), containerScope, boundCompileUnit);
}

void CheckFunctionReturnPaths(cmajor::symbols::FunctionSymbol* functionSymbol, cmajor::ast::CompoundStatementNode* bodyNode, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId,
    cmajor::symbols::ContainerScope* containerScope, BoundCompileUnit& boundCompileUnit)
{
    cmajor::symbols::TypeSymbol* returnType = functionSymbol->ReturnType();
    if (!returnType || returnType->GetSymbolType() == cmajor::symbols::SymbolType::voidTypeSymbol) return;
    if (functionSymbol->IsExternal()) return;
    if (functionSymbol->IsAbstract()) return;
    cmajor::ast::CompoundStatementNode* body = bodyNode;
    if (body)
    {
        int n = body->Statements().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::StatementNode* statement = body->Statements()[i];
            if (TerminatesFunction(statement, false, containerScope, boundCompileUnit)) return;
        }
        throw cmajor::symbols::Exception("not all control paths terminate in return or throw statement", sourcePos, moduleId);
    }
}

StatementBinder::StatementBinder(BoundCompileUnit& boundCompileUnit_) :
    boundCompileUnit(boundCompileUnit_), symbolTable(boundCompileUnit.GetSymbolTable()), module(&boundCompileUnit.GetModule()), containerScope(nullptr), statement(), compoundLevel(0), insideCatch(false),
    currentClass(nullptr), currentFunction(nullptr), currentStaticConstructorSymbol(nullptr), currentStaticConstructorNode(nullptr), currentConstructorSymbol(nullptr),
    currentConstructorNode(nullptr), currentDestructorSymbol(nullptr), currentDestructorNode(nullptr), currentMemberFunctionSymbol(nullptr), currentMemberFunctionNode(nullptr),
    switchConditionType(nullptr), currentCaseValueMap(nullptr), currentGotoCaseStatements(nullptr), currentGotoDefaultStatements(nullptr), postfix(false), compilingThrow(false),
    compilingReleaseExceptionStatement(false), dontCheckDuplicateFunctionSymbols(false)
{
}

void StatementBinder::Visit(cmajor::ast::CompileUnitNode& compileUnitNode)
{
    if (compileUnitNode.IsSynthesizedUnit())
    {
        dontCheckDuplicateFunctionSymbols = true;
    }
    compileUnitNode.GlobalNs()->Accept(*this);
    dontCheckDuplicateFunctionSymbols = false;
    cmajor::symbols::Symbol* symbol = symbolTable.GlobalNs().GetContainerScope()->Lookup(U"System.Runtime.AddCompileUnitFunction");
    if (symbol)
    {
        if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupSymbol)
        {
            cmajor::symbols::FunctionGroupSymbol* functionGroup = static_cast<cmajor::symbols::FunctionGroupSymbol*>(symbol);
            cmajor::symbols::FunctionSymbol* systemRuntimeAddCompileUnitFunctionSymbol = functionGroup->GetFunction();
            boundCompileUnit.SetSystemRuntimeAddCompileUnitFunctionSymbol(systemRuntimeAddCompileUnitFunctionSymbol);
        }
    }
    cmajor::symbols::FunctionSymbol* initCompileUnitSymbol = boundCompileUnit.GetInitCompileUnitFunctionSymbol();
    if (initCompileUnitSymbol == nullptr)
    {
        boundCompileUnit.GenerateCompileUnitInitialization();
    }
}

struct NamespaceVisitor
{
    NamespaceVisitor(BoundCompileUnit* cu_, BoundNamespace* ns_) : cu(cu_), ns(ns_)
    {
        cu->PushNamespace(ns);
    }
    ~NamespaceVisitor()
    {
        cu->PopNamespace();
    }
    BoundCompileUnit* cu;
    BoundNamespace* ns;
};

void StatementBinder::Visit(cmajor::ast::NamespaceNode& namespaceNode)
{
    std::unique_ptr<BoundNamespace> ns(new BoundNamespace(namespaceNode));
    boundCompileUnit.PushNamespace(ns.get());
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&namespaceNode);
    containerScope = symbol->GetContainerScope();
    int n = namespaceNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* member = namespaceNode.Members()[i];
        member->Accept(*this);
    }
    containerScope = prevContainerScope;
    boundCompileUnit.PopNamespace();
    boundCompileUnit.AddBoundNode(std::unique_ptr<BoundNode>(ns.release()));
}

void StatementBinder::Visit(cmajor::ast::EnumTypeNode& enumTypeNode)
{
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&enumTypeNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::enumTypeSymbol, "enum type symbols expected");
    cmajor::symbols::EnumTypeSymbol* enumTypeSymbol = static_cast<cmajor::symbols::EnumTypeSymbol*>(symbol);
    std::unique_ptr<BoundEnumTypeDefinition> boundEnum(new BoundEnumTypeDefinition(enumTypeSymbol));
    boundCompileUnit.AddBoundNode(std::move(boundEnum));
}

void StatementBinder::Visit(cmajor::ast::ClassNode& classNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&classNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
        symbol->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type symbol expected");
    cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol);
    if (classTypeSymbol->IsClassTemplate())
    {
        return;
    }
    containerScope = symbol->GetContainerScope();
    std::unique_ptr<BoundClass> boundClass(new BoundClass(classTypeSymbol));
    BoundClass* prevClass = currentClass;
    currentClass = boundClass.get();
    int n = classNode.Members().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* classMember = classNode.Members()[i];
        classMember->Accept(*this);
    }
    boundCompileUnit.GetAttributeBinder()->GenerateImplementation(classNode.GetAttributes(), symbol, this);
    boundCompileUnit.AddBoundNode(std::move(boundClass));
    cmajor::symbols::DestructorSymbol* destructorSymbol = classTypeSymbol->Destructor();
    if (destructorSymbol && destructorSymbol->IsGeneratedFunction() && !cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::info))
    {
        if (!boundCompileUnit.IsGeneratedDestructorInstantiated(destructorSymbol))
        {
            boundCompileUnit.SetGeneratedDestructorInstantiated(destructorSymbol);
            GenerateDestructorImplementation(currentClass, destructorSymbol, boundCompileUnit, containerScope, currentFunction, classNode.GetSourcePos(), classNode.ModuleId());
        }
    }
    currentClass = prevClass;
    containerScope = prevContainerScope;
}

void StatementBinder::Visit(cmajor::ast::MemberVariableNode& memberVariableNode)
{
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&memberVariableNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::memberVariableSymbol, "member variable symbol expected");
    cmajor::symbols::MemberVariableSymbol* memberVariableSymbol = static_cast<cmajor::symbols::MemberVariableSymbol*>(symbol);
    cmajor::symbols::TypeSymbol* typeSymbol = memberVariableSymbol->GetType();
    if (typeSymbol->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(typeSymbol);
        cmajor::symbols::DestructorSymbol* destructorSymbol = classType->Destructor();
        if (destructorSymbol && destructorSymbol->IsGeneratedFunction() && !cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::info))
        {
            if (!boundCompileUnit.IsGeneratedDestructorInstantiated(destructorSymbol))
            {
                boundCompileUnit.SetGeneratedDestructorInstantiated(destructorSymbol);
                std::unique_ptr<BoundClass> boundClass(new BoundClass(classType));
                GenerateDestructorImplementation(boundClass.get(), destructorSymbol, boundCompileUnit, containerScope, currentFunction, memberVariableNode.GetSourcePos(), memberVariableNode.ModuleId());
                boundCompileUnit.AddBoundNode(std::move(boundClass));
            }
        }
    }
}

void StatementBinder::Visit(cmajor::ast::FunctionNode& functionNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&functionNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::functionSymbol, "function symbol expected");
    cmajor::symbols::FunctionSymbol* functionSymbol = static_cast<cmajor::symbols::FunctionSymbol*>(symbol);
    if (!dontCheckDuplicateFunctionSymbols)
    {
        functionSymbol->FunctionGroup()->CheckDuplicateFunctionSymbols();
    }
    if (functionSymbol->IsFunctionTemplate())
    {
        return;
    }
    containerScope = symbol->GetContainerScope();
    std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, functionSymbol));
    BoundFunction* prevFunction = currentFunction;
    currentFunction = boundFunction.get();
    if (functionNode.Body())
    {
        GenerateEnterAndExitFunctionCode(currentFunction);
        compoundLevel = 0;
        functionNode.Body()->Accept(*this);
        BoundStatement* boundStatement = statement.release();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected"); 
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        CheckFunctionReturnPaths(functionSymbol, functionNode, containerScope, boundCompileUnit);
        boundCompileUnit.AddBoundNode(std::move(boundFunction));
    }
    boundCompileUnit.GetAttributeBinder()->GenerateImplementation(functionNode.GetAttributes(), symbol, this);
    currentFunction = prevFunction;
    containerScope = prevContainerScope;
}

void StatementBinder::Visit(cmajor::ast::FullInstantiationRequestNode& fullInstantiationRequestNode)
{
    cmajor::symbols::TypeSymbol* type = ResolveType(fullInstantiationRequestNode.TemplateId(), boundCompileUnit, containerScope);
    if (type->GetSymbolType() != cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
    {
        throw cmajor::symbols::Exception("full instantiation request expects subject template identifier to be a class template specialization",
            fullInstantiationRequestNode.GetSourcePos(),
            fullInstantiationRequestNode.ModuleId());
    }
    cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
    util::LogMessage(module->LogStreamId(), "generating full instantation of '" + util::ToUtf8(specialization->FullName()) + "'");
    GetBoundCompileUnit().GetClassTemplateRepository().InstantiateAll(specialization, containerScope, currentFunction, fullInstantiationRequestNode.GetSourcePos(), fullInstantiationRequestNode.ModuleId());
    specialization->SetHasFullInstantiation();
}

void StatementBinder::Visit(cmajor::ast::StaticConstructorNode& staticConstructorNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&staticConstructorNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::staticConstructorSymbol, "static constructor symbol expected");
    cmajor::symbols::StaticConstructorSymbol* staticConstructorSymbol = static_cast<cmajor::symbols::StaticConstructorSymbol*>(symbol);
    if (!dontCheckDuplicateFunctionSymbols)
    {
        staticConstructorSymbol->FunctionGroup()->CheckDuplicateFunctionSymbols();
    }
    cmajor::symbols::StaticConstructorSymbol* prevStaticConstructorSymbol = currentStaticConstructorSymbol;
    currentStaticConstructorSymbol = staticConstructorSymbol;
    containerScope = symbol->GetContainerScope();
    std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, staticConstructorSymbol));
    BoundFunction* prevFunction = currentFunction;
    currentFunction = boundFunction.get();
    if (staticConstructorNode.Body())
    {
        GenerateEnterAndExitFunctionCode(currentFunction);
        cmajor::ast::StaticConstructorNode* prevStaticConstructorNode = currentStaticConstructorNode;
        currentStaticConstructorNode = &staticConstructorNode;
        compoundLevel = 0;
        staticConstructorNode.Body()->Accept(*this);
        currentStaticConstructorNode = prevStaticConstructorNode;
        BoundStatement* boundStatement = statement.release();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        CheckFunctionReturnPaths(staticConstructorSymbol, staticConstructorNode, containerScope, boundCompileUnit);
        currentClass->AddMember(std::move(boundFunction));
    }
    boundCompileUnit.GetAttributeBinder()->GenerateImplementation(staticConstructorNode.GetAttributes(), symbol, this);
    currentFunction = prevFunction;
    containerScope = prevContainerScope;
    currentStaticConstructorSymbol = prevStaticConstructorSymbol;
}

void StatementBinder::GenerateEnterAndExitFunctionCode(BoundFunction* boundFunction)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx) return;
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId = util::nil_uuid();
    if (boundFunction->GetFunctionSymbol()->DontThrow()) return;
    cmajor::symbols::TypeSymbol* systemRuntimeUnwindInfoSymbol = boundCompileUnit.GetSystemRuntimeUnwindInfoSymbol();
    if (systemRuntimeUnwindInfoSymbol == nullptr)
    {
        cmajor::ast::IdentifierNode systemRuntimeUnwindInfoNode(sourcePos, moduleId, U"System.Runtime.UnwindInfo");
        systemRuntimeUnwindInfoSymbol = ResolveType(&systemRuntimeUnwindInfoNode, boundCompileUnit, containerScope);
        boundCompileUnit.SetSystemRuntimeUnwindInfoSymbol(systemRuntimeUnwindInfoSymbol);
    }
    cmajor::symbols::FunctionSymbol* initUnwindSymbol = boundCompileUnit.GetInitUnwindInfoFunctionSymbol();
    if (initUnwindSymbol == nullptr)
    {
        boundCompileUnit.GenerateInitUnwindInfoFunctionSymbol();
    }
    cmajor::symbols::LocalVariableSymbol* prevUnwindInfoVariableSymbol = new cmajor::symbols::LocalVariableSymbol(sourcePos, moduleId, U"@prevUnwindInfo");
    containerScope->Install(prevUnwindInfoVariableSymbol);
    prevUnwindInfoVariableSymbol->SetType(systemRuntimeUnwindInfoSymbol->AddPointer(sourcePos, moduleId));
    boundFunction->GetFunctionSymbol()->SetPrevUnwindInfoVar(prevUnwindInfoVariableSymbol);
    cmajor::ast::IdentifierNode* prevUnwindInfoNode1 = new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@prevUnwindInfo");
    symbolTable.MapSymbol(prevUnwindInfoNode1, prevUnwindInfoVariableSymbol);
    symbolTable.MapNode(prevUnwindInfoNode1, prevUnwindInfoVariableSymbol);
    cmajor::ast::InvokeNode* pushUnwindInfo = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"RtPushUnwindInfo"));
    cmajor::symbols::LocalVariableSymbol* unwindInfoVariableSymbol = new cmajor::symbols::LocalVariableSymbol(sourcePos, moduleId, U"@unwindInfo");
    containerScope->Install(unwindInfoVariableSymbol);
    unwindInfoVariableSymbol->SetType(systemRuntimeUnwindInfoSymbol);
    boundFunction->GetFunctionSymbol()->SetUnwindInfoVar(unwindInfoVariableSymbol);
    cmajor::ast::IdentifierNode* unwindInfoNode1 = new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@unwindInfo");
    symbolTable.MapSymbol(unwindInfoNode1, unwindInfoVariableSymbol);
    symbolTable.MapNode(unwindInfoNode1, unwindInfoVariableSymbol);
    pushUnwindInfo->AddArgument(new cmajor::ast::CastNode(sourcePos, moduleId, new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::VoidNode(sourcePos, moduleId)), 
        new cmajor::ast::AddrOfNode(sourcePos, moduleId, unwindInfoNode1)));
    cmajor::ast::AssignmentStatementNode assignUnwindInfo(sourcePos, moduleId, prevUnwindInfoNode1,
        new cmajor::ast::CastNode(sourcePos, moduleId, new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Runtime.UnwindInfo")), pushUnwindInfo));
    assignUnwindInfo.Accept(*this);
    std::unique_ptr<BoundStatement> pushUnwindInfoStatement(statement.release());

    cmajor::ast::IdentifierNode* prevUnwindInfoNode2 = new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@prevUnwindInfo");
    symbolTable.MapSymbol(prevUnwindInfoNode2, prevUnwindInfoVariableSymbol);
    symbolTable.MapNode(prevUnwindInfoNode2, prevUnwindInfoVariableSymbol);
    cmajor::ast::IdentifierNode* unwindInfoNode2 = new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@unwindInfo");
    symbolTable.MapSymbol(unwindInfoNode2, unwindInfoVariableSymbol);
    symbolTable.MapNode(unwindInfoNode2, unwindInfoVariableSymbol);
    cmajor::ast::AssignmentStatementNode assignUnwindInfoNext(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, unwindInfoNode2, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"next")), prevUnwindInfoNode2);
    assignUnwindInfoNext.Accept(*this);
    std::unique_ptr<BoundStatement> assignUnwindInfoNextStatement(statement.release());

    cmajor::ast::IdentifierNode* unwindInfoNode3 = new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@unwindInfo");
    symbolTable.MapSymbol(unwindInfoNode3, unwindInfoVariableSymbol);
    symbolTable.MapNode(unwindInfoNode3, unwindInfoVariableSymbol);
    cmajor::ast::FunctionPtrNode* functionPtrNode = new cmajor::ast::FunctionPtrNode(sourcePos, moduleId);
    BoundFunctionPtr* boundFunctionPtr = new BoundFunctionPtr(sourcePos, moduleId, boundFunction->GetFunctionSymbol(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId));
    BoundBitCast* boundBitCast = new BoundBitCast(std::unique_ptr<BoundExpression>(boundFunctionPtr), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer(sourcePos, moduleId));
    std::unique_ptr<BoundExpression> boundFunctionPtrAsVoidPtr(boundBitCast);
    functionPtrNode->SetBoundExpression(boundFunctionPtrAsVoidPtr.get());
    cmajor::ast::AssignmentStatementNode assignFunctionPtr(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, unwindInfoNode3, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"function")),
        new cmajor::ast::CastNode(sourcePos, moduleId, new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::VoidNode(sourcePos, moduleId)), functionPtrNode));
    assignFunctionPtr.Accept(*this);
    std::unique_ptr<BoundStatement> assignFunctionPtrStatement(statement.release());

    cmajor::ast::IdentifierNode* unwindInfoNode4 = new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@unwindInfo");
    symbolTable.MapSymbol(unwindInfoNode4, unwindInfoVariableSymbol);
    symbolTable.MapNode(unwindInfoNode4, unwindInfoVariableSymbol);
    cmajor::ast::AssignmentStatementNode assignUnwindInfoLine(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, unwindInfoNode4, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"line")),
        new cmajor::ast::IntLiteralNode(sourcePos, moduleId, 0));
    assignUnwindInfoLine.Accept(*this);
    std::unique_ptr<BoundStatement> assignUnwindInfoLineStatement(statement.release());

    cmajor::ast::IdentifierNode* prevUnwindInfoNode3 = new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@prevUnwindInfo");
    symbolTable.MapSymbol(prevUnwindInfoNode3, prevUnwindInfoVariableSymbol);
    symbolTable.MapNode(prevUnwindInfoNode3, prevUnwindInfoVariableSymbol);
    cmajor::ast::InvokeNode* setPrevUnwindInfoListPtr = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"RtPopUnwindInfo"));
    setPrevUnwindInfoListPtr->AddArgument(new cmajor::ast::CastNode(sourcePos, moduleId, new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::VoidNode(sourcePos, moduleId)), prevUnwindInfoNode3));
    cmajor::ast::ExpressionStatementNode setPrevUnwindInfoList(sourcePos, moduleId, setPrevUnwindInfoListPtr);
    setPrevUnwindInfoList.Accept(*this);
    std::unique_ptr<BoundStatement> setPrevUnwindInfoListStatement(statement.release());

    std::vector<std::unique_ptr<BoundStatement>> enterCode;
    enterCode.push_back(std::move(pushUnwindInfoStatement));
    enterCode.push_back(std::move(assignUnwindInfoNextStatement));
    enterCode.push_back(std::move(assignFunctionPtrStatement));
    enterCode.push_back(std::move(assignUnwindInfoLineStatement));
    boundFunction->SetEnterCode(std::move(enterCode));

    std::unique_ptr<BoundStatement> setLineCode;
    cmajor::ast::IdentifierNode* unwindInfoNode5 = new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@unwindInfo");
    symbolTable.MapSymbol(unwindInfoNode5, unwindInfoVariableSymbol);
    cmajor::ast::AssignmentStatementNode setUnwindInfoLine(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, unwindInfoNode5, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"line")),
        new cmajor::ast::IntLiteralNode(sourcePos, moduleId, 0));
    setUnwindInfoLine.Accept(*this);
    std::unique_ptr<BoundStatement> setLineStatement(statement.release());
    boundFunction->SetLineCode(std::move(setLineStatement));

    std::vector<std::unique_ptr<BoundStatement>> exitCode;
    exitCode.push_back(std::move(setPrevUnwindInfoListStatement));
    boundFunction->SetExitCode(std::move(exitCode));
}

void StatementBinder::Visit(cmajor::ast::ConstructorNode& constructorNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&constructorNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol, "constructor symbol expected"); 
    cmajor::symbols::ConstructorSymbol* constructorSymbol = static_cast<cmajor::symbols::ConstructorSymbol*>(symbol);
    if (!dontCheckDuplicateFunctionSymbols)
    {
        constructorSymbol->FunctionGroup()->CheckDuplicateFunctionSymbols();
    }
    cmajor::symbols::ConstructorSymbol* prevConstructorSymbol = currentConstructorSymbol;
    currentConstructorSymbol = constructorSymbol;
    containerScope = symbol->GetContainerScope();
    std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, constructorSymbol));
    BoundFunction* prevFunction = currentFunction;
    currentFunction = boundFunction.get();
    if (constructorNode.Body())
    {
        GenerateEnterAndExitFunctionCode(currentFunction);
        cmajor::ast::ConstructorNode* prevConstructorNode = currentConstructorNode;
        currentConstructorNode = &constructorNode;
        compoundLevel = 0;
        constructorNode.Body()->Accept(*this);
        currentConstructorNode = prevConstructorNode;
        BoundStatement* boundStatement = statement.release();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
    }
    else if (constructorSymbol->IsDefault())
    {
        cmajor::ast::ConstructorNode* prevConstructorNode = currentConstructorNode;
        currentConstructorNode = &constructorNode;
        std::unique_ptr<BoundCompoundStatement> boundCompoundStatement(new BoundCompoundStatement(constructorNode.GetSourcePos(), constructorNode.ModuleId()));
        GenerateClassInitialization(currentConstructorSymbol, currentConstructorNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this, true,
            constructorNode.GetSourcePos(), constructorNode.ModuleId());
        currentConstructorNode = prevConstructorNode;
        boundFunction->SetBody(std::move(boundCompoundStatement));
    }
    if (boundFunction->Body())
    {
        CheckFunctionReturnPaths(constructorSymbol, constructorNode, containerScope, boundCompileUnit);
        currentClass->AddMember(std::move(boundFunction));
    }
    boundCompileUnit.GetAttributeBinder()->GenerateImplementation(constructorNode.GetAttributes(), symbol, this);
    currentFunction = prevFunction;
    containerScope = prevContainerScope;
    currentConstructorSymbol = prevConstructorSymbol;
}

void StatementBinder::Visit(cmajor::ast::DestructorNode& destructorNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&destructorNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::destructorSymbol, "destructor symbol expected");
    cmajor::symbols::DestructorSymbol* destructorSymbol = static_cast<cmajor::symbols::DestructorSymbol*>(symbol);
    if (!dontCheckDuplicateFunctionSymbols)
    {
        destructorSymbol->FunctionGroup()->CheckDuplicateFunctionSymbols();
    }
    cmajor::symbols::DestructorSymbol* prevDestructorSymbol = currentDestructorSymbol;
    currentDestructorSymbol = destructorSymbol;
    containerScope = symbol->GetContainerScope();
    std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, destructorSymbol));
    BoundFunction* prevFunction = currentFunction;
    currentFunction = boundFunction.get();
    if (destructorNode.Body())
    {
        GenerateEnterAndExitFunctionCode(currentFunction);
        cmajor::ast::DestructorNode* prevDestructorNode = currentDestructorNode;
        currentDestructorNode = &destructorNode;
        compoundLevel = 0;
        destructorNode.Body()->Accept(*this);
        currentDestructorNode = prevDestructorNode;
        BoundStatement* boundStatement = statement.release();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
    }
    else if (destructorSymbol->IsDefault())
    {
        cmajor::ast::DestructorNode* prevDestructorNode = currentDestructorNode;
        currentDestructorNode = &destructorNode;
        std::unique_ptr<BoundCompoundStatement> boundCompoundStatement(new BoundCompoundStatement(destructorNode.GetSourcePos(), destructorNode.ModuleId()));
        GenerateClassTermination(currentDestructorSymbol, currentDestructorNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this,
            currentDestructorNode->GetSourcePos(), currentDestructorNode->ModuleId());
        currentDestructorNode = prevDestructorNode;
        boundFunction->SetBody(std::move(boundCompoundStatement));
    }
    if (boundFunction->Body())
    {
        CheckFunctionReturnPaths(destructorSymbol, destructorNode, containerScope, boundCompileUnit);
        currentClass->AddMember(std::move(boundFunction));
    }
    boundCompileUnit.GetAttributeBinder()->GenerateImplementation(destructorNode.GetAttributes(), symbol, this);
    currentFunction = prevFunction;
    containerScope = prevContainerScope;
    currentDestructorSymbol = prevDestructorSymbol;
}

void StatementBinder::Visit(cmajor::ast::MemberFunctionNode& memberFunctionNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&memberFunctionNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::memberFunctionSymbol, "member function symbol expected");
    cmajor::symbols::MemberFunctionSymbol* memberFunctionSymbol = static_cast<cmajor::symbols::MemberFunctionSymbol*>(symbol);
    if (!dontCheckDuplicateFunctionSymbols)
    {
        memberFunctionSymbol->FunctionGroup()->CheckDuplicateFunctionSymbols();
    }
    cmajor::symbols::MemberFunctionSymbol* prevMemberFunctionSymbol = currentMemberFunctionSymbol;
    currentMemberFunctionSymbol = memberFunctionSymbol;
    containerScope = symbol->GetContainerScope();
    std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, memberFunctionSymbol));
    BoundFunction* prevFunction = currentFunction;
    currentFunction = boundFunction.get();
    if (memberFunctionNode.Body())
    {
        GenerateEnterAndExitFunctionCode(currentFunction);
        cmajor::ast::MemberFunctionNode* prevMemberFunctionNode = currentMemberFunctionNode;
        currentMemberFunctionNode = &memberFunctionNode;
        compoundLevel = 0;
        memberFunctionNode.Body()->Accept(*this);
        currentMemberFunctionNode = prevMemberFunctionNode;
        BoundStatement* boundStatement = statement.release();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
    }
    else if (memberFunctionSymbol->IsDefault())
    {
        Assert(memberFunctionSymbol->GroupName() == U"operator=", "operator= expected");
        cmajor::ast::MemberFunctionNode* prevMemberFunctionNode = currentMemberFunctionNode;
        currentMemberFunctionNode = &memberFunctionNode;
        std::unique_ptr<BoundCompoundStatement> boundCompoundStatement(new BoundCompoundStatement(memberFunctionNode.GetSourcePos(), memberFunctionNode.ModuleId()));
        GenerateClassAssignment(currentMemberFunctionSymbol, currentMemberFunctionNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this, true,
            memberFunctionNode.GetSourcePos(), memberFunctionNode.ModuleId());
        currentMemberFunctionNode = prevMemberFunctionNode;
        boundFunction->SetBody(std::move(boundCompoundStatement));
    }
    if (boundFunction->Body())
    {
        CheckFunctionReturnPaths(memberFunctionSymbol, memberFunctionNode, containerScope, boundCompileUnit);
        currentClass->AddMember(std::move(boundFunction));
    }
    boundCompileUnit.GetAttributeBinder()->GenerateImplementation(memberFunctionNode.GetAttributes(), symbol, this);
    currentFunction = prevFunction;
    containerScope = prevContainerScope;
    currentMemberFunctionSymbol = prevMemberFunctionSymbol;
}

void StatementBinder::Visit(cmajor::ast::ConversionFunctionNode& conversionFunctionNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&conversionFunctionNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::conversionFunctionSymbol, "conversion function symbol expected");
    cmajor::symbols::ConversionFunctionSymbol* conversionFunctionSymbol = static_cast<cmajor::symbols::ConversionFunctionSymbol*>(symbol);
    if (!dontCheckDuplicateFunctionSymbols)
    {
        conversionFunctionSymbol->FunctionGroup()->CheckDuplicateFunctionSymbols();
    }
    containerScope = symbol->GetContainerScope();
    std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&boundCompileUnit, conversionFunctionSymbol));
    BoundFunction* prevFunction = currentFunction;
    currentFunction = boundFunction.get();
    if (conversionFunctionNode.Body())
    {
        GenerateEnterAndExitFunctionCode(currentFunction);
        compoundLevel = 0;
        conversionFunctionNode.Body()->Accept(*this);
        BoundStatement* boundStatement = statement.release();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        CheckFunctionReturnPaths(conversionFunctionSymbol, conversionFunctionNode, containerScope, boundCompileUnit);
        currentClass->AddMember(std::move(boundFunction));
    }
    boundCompileUnit.GetAttributeBinder()->GenerateImplementation(conversionFunctionNode.GetAttributes(), symbol, this);
    currentFunction = prevFunction;
    containerScope = prevContainerScope;
}

void StatementBinder::Visit(cmajor::ast::CompoundStatementNode& compoundStatementNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(&compoundStatementNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::declarationBlock, "declaration block expected");
    cmajor::symbols::DeclarationBlock* declarationBlock = static_cast<cmajor::symbols::DeclarationBlock*>(symbol);
    containerScope = declarationBlock->GetContainerScope();
    std::unique_ptr<BoundCompoundStatement> boundCompoundStatement(new BoundCompoundStatement(compoundStatementNode.GetSourcePos(), compoundStatementNode.EndSourcePos(), 
        compoundStatementNode.ModuleId()));
    if (compoundLevel == 0)
    {
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::profile))
        {
            bool profile = true;
            if (currentFunction->GetFunctionSymbol()->IsProgramMain())
            {
                profile = false;
            }
            else if (currentClass && currentClass->GetClassTypeSymbol()->FullName() == U"System.Runtime.FunctionProfiler")
            {
                profile = false;
            }
            if (profile)
            {
                util::uuid functionId = currentFunction->GetFunctionSymbol()->FunctionId();
                symbolTable.MapProfiledFunction(functionId, currentFunction->GetFunctionSymbol()->FullName());
                cmajor::ast::ConstructionStatementNode constructFunctionProfiler(compoundStatementNode.GetSourcePos(), compoundStatementNode.ModuleId(),
                    new cmajor::ast::IdentifierNode(compoundStatementNode.GetSourcePos(), compoundStatementNode.ModuleId(), U"System.Runtime.FunctionProfiler"),
                    new cmajor::ast::IdentifierNode(compoundStatementNode.GetSourcePos(), compoundStatementNode.ModuleId(), U"@functionProfiler"));
                constructFunctionProfiler.AddArgument(new cmajor::ast::UuidLiteralNode(compoundStatementNode.GetSourcePos(), functionId));
                symbolTable.SetCurrentFunctionSymbol(currentFunction->GetFunctionSymbol());
                symbolTable.BeginContainer(containerScope->Container());
                cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
                constructFunctionProfiler.Accept(symbolCreatorVisitor);
                symbolTable.EndContainer();
                TypeBinder typeBinder(boundCompileUnit);
                typeBinder.SetContainerScope(containerScope);
                typeBinder.SetCurrentFunctionSymbol(currentFunction->GetFunctionSymbol());
                constructFunctionProfiler.Accept(typeBinder);
                constructFunctionProfiler.Accept(*this);
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(ReleaseStatement()));
            }
        }
        if (currentStaticConstructorSymbol && currentStaticConstructorNode)
        {
            GenerateStaticClassInitialization(currentStaticConstructorSymbol, currentStaticConstructorNode, boundCompileUnit, boundCompoundStatement.get(), currentFunction, containerScope, this,
                boundCompoundStatement->GetSourcePos(), boundCompoundStatement->ModuleId());
        }
        else if (currentConstructorSymbol && currentConstructorNode)
        {
            GenerateClassInitialization(currentConstructorSymbol, currentConstructorNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this, false,
                boundCompoundStatement->GetSourcePos(), boundCompoundStatement->ModuleId());
        }
        else if (currentMemberFunctionSymbol && currentMemberFunctionSymbol->GroupName() == U"operator=" && currentMemberFunctionNode)
        {
            GenerateClassAssignment(currentMemberFunctionSymbol, currentMemberFunctionNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this, false,
                boundCompoundStatement->GetSourcePos(), boundCompoundStatement->ModuleId());
        }
        else if (currentMemberFunctionSymbol && currentMemberFunctionSymbol->IsStatic() && currentMemberFunctionNode)
        {
            if (currentClass->GetClassTypeSymbol()->StaticConstructor())
            {
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(
                    new BoundFunctionCall(boundCompoundStatement->GetSourcePos(), boundCompoundStatement->ModuleId(), currentClass->GetClassTypeSymbol()->StaticConstructor())))));
            }
        }
    }
    ++compoundLevel;
    int n = compoundStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statementNode = compoundStatementNode.Statements()[i];
        statementNode->Accept(*this);
        boundCompoundStatement->AddStatement(std::move(statement));
    }
    --compoundLevel;
    if (compoundLevel == 0 && currentDestructorSymbol && currentDestructorNode)
    {
        GenerateClassTermination(currentDestructorSymbol, currentDestructorNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this,
            boundCompoundStatement->EndSourcePos(), boundCompoundStatement->ModuleId());
    }
    AddStatement(boundCompoundStatement.release());
    containerScope = prevContainerScope;
}

void StatementBinder::Visit(cmajor::ast::LabeledStatementNode& labeledStatementNode)
{
    labeledStatementNode.Stmt()->Accept(*this);
    statement->SetLabel(labeledStatementNode.Label()->Label());
}

void StatementBinder::Visit(cmajor::ast::ReturnStatementNode& returnStatementNode)
{
    if (returnStatementNode.Expression())
    {
        if (currentFunction->GetFunctionSymbol()->ReturnsClassInterfaceOrClassDelegateByValue())
        {
            std::vector<FunctionScopeLookup> classReturnLookups;
            classReturnLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            classReturnLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, currentFunction->GetFunctionSymbol()->ReturnType()->ClassInterfaceEnumDelegateOrNsScope()));
            classReturnLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> classReturnArgs;
            classReturnArgs.push_back(std::unique_ptr<BoundExpression>(new BoundParameter(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId(), currentFunction->GetFunctionSymbol()->ReturnParam())));
            cmajor::symbols::TypeSymbol* returnType = currentFunction->GetFunctionSymbol()->ReturnType();
            bool returnClassDelegateType = returnType->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol;
            std::unique_ptr<BoundExpression> expression = BindExpression(returnStatementNode.Expression(), boundCompileUnit, currentFunction, containerScope, this, false, returnClassDelegateType,
                returnClassDelegateType);
            bool exceptionCapture = false;
            if (insideCatch && expression->ContainsExceptionCapture())
            {
                exceptionCapture = true;
            }
            if (expression->GetBoundNodeType() == BoundNodeType::boundLocalVariable)
            {
                std::vector<FunctionScopeLookup> rvalueLookups;
                rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                rvalueLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::vector<std::unique_ptr<BoundExpression>> rvalueArguments;
                if (returnClassDelegateType && expression->GetType()->BaseType()->GetSymbolType() == cmajor::symbols::SymbolType::functionGroupTypeSymbol)
                {
                    cmajor::symbols::TypeSymbol* exprType = expression->GetType();
                    ArgumentMatch argumentMatch;
                    expression.reset(new BoundConversion(std::move(expression),
                        boundCompileUnit.GetConversion(exprType, returnType, containerScope, currentFunction, returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId(), argumentMatch)));
                }
                rvalueArguments.push_back(std::move(expression));
                std::unique_ptr<BoundExpression> rvalueExpr = ResolveOverload(U"System.Rvalue", containerScope, rvalueLookups, rvalueArguments, boundCompileUnit, currentFunction,
                    returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
                expression = std::move(rvalueExpr);
            }
            classReturnArgs.push_back(std::move(expression));
            std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, classReturnLookups, classReturnArgs, boundCompileUnit, currentFunction,
                returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
            std::unique_ptr<BoundStatement> constructStatement(new BoundInitializationStatement(std::move(constructorCall)));
            AddStatement(constructStatement.release());
            std::unique_ptr<BoundFunctionCall> returnFunctionCall;
            std::unique_ptr<BoundStatement> returnStatement(new BoundReturnStatement(std::move(returnFunctionCall), returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId()));
            if (exceptionCapture)
            {
                AddReleaseExceptionStatement(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
            }
            AddStatement(returnStatement.release());
        }
        else
        {
            cmajor::symbols::TypeSymbol* returnType = currentFunction->GetFunctionSymbol()->ReturnType();
            bool returnDelegateType = false;
            bool returnClassDelegateType = false;
            bool exceptionCapture = false;
            if (returnType)
            {
                returnDelegateType = returnType->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol;
                returnClassDelegateType = returnType->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol;
            }
            if (returnType && returnType->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol)
            {
                std::vector<std::unique_ptr<BoundExpression>> returnTypeArgs;
                BoundTypeExpression* boundTypeExpression = new BoundTypeExpression(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId(), returnType);
                returnTypeArgs.push_back(std::unique_ptr<BoundTypeExpression>(boundTypeExpression));
                std::vector<FunctionScopeLookup> functionScopeLookups;
                functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, returnType->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
                functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::unique_ptr<BoundFunctionCall> returnFunctionCall = ResolveOverload(U"@return", containerScope, functionScopeLookups, returnTypeArgs, boundCompileUnit, currentFunction,
                    returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
                std::unique_ptr<BoundExpression> expression = BindExpression(returnStatementNode.Expression(), boundCompileUnit, currentFunction, containerScope, this, false,
                    returnDelegateType || returnClassDelegateType, returnClassDelegateType);
                if (insideCatch && expression->ContainsExceptionCapture())
                {
                    exceptionCapture = true;
                }
                std::vector<std::unique_ptr<BoundExpression>> returnValueArguments;
                returnValueArguments.push_back(std::move(expression));
                FunctionMatch functionMatch(returnFunctionCall->GetFunctionSymbol());
                bool conversionFound = FindConversions(boundCompileUnit, returnFunctionCall->GetFunctionSymbol(), returnValueArguments, functionMatch, cmajor::symbols::ConversionType::implicit_,
                    containerScope, currentFunction, returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
                if (conversionFound)
                {
                    Assert(!functionMatch.argumentMatches.empty(), "argument match expected");
                    ArgumentMatch argumentMatch = functionMatch.argumentMatches[0];
                    if (argumentMatch.preReferenceConversionFlags != cmajor::ir::OperationFlags::none)
                    {
                        if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::addr)
                        {
                            cmajor::symbols::TypeSymbol* type = returnValueArguments[0]->GetType()->AddLvalueReference(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
                            BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(returnValueArguments[0]), type);
                            returnValueArguments[0].reset(addressOfExpression);
                        }
                        else if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::deref)
                        {
                            cmajor::symbols::TypeSymbol* type = returnValueArguments[0]->GetType()->RemoveReference(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
                            BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(returnValueArguments[0]), type);
                            returnValueArguments[0].reset(dereferenceExpression);
                        }
                    }
                    cmajor::symbols::FunctionSymbol* conversionFun = argumentMatch.conversionFun;
                    if (conversionFun)
                    {
                        if (conversionFun->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol)
                        {
                            BoundFunctionCall* constructorCall = new BoundFunctionCall(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId(), conversionFun);
                            cmajor::symbols::LocalVariableSymbol* temporary = currentFunction->GetFunctionSymbol()->CreateTemporary(conversionFun->ConversionTargetType(),
                                returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
                            constructorCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(new BoundLocalVariable(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId(), temporary)),
                                conversionFun->ConversionTargetType()->AddPointer(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId()))));
                            constructorCall->AddArgument(std::move(returnValueArguments[0]));
                            BoundConstructAndReturnTemporaryExpression* conversion = new BoundConstructAndReturnTemporaryExpression(std::unique_ptr<BoundExpression>(constructorCall),
                                std::unique_ptr<BoundExpression>(new BoundLocalVariable(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId(), temporary)));
                            returnValueArguments[0].reset(conversion);
                        }
                        else
                        {
                            BoundConversion* boundConversion = new BoundConversion(std::unique_ptr<BoundExpression>(returnValueArguments[0].release()), conversionFun);
                            returnValueArguments[0].reset(boundConversion);
                        }
                    }
                    if (argumentMatch.postReferenceConversionFlags != cmajor::ir::OperationFlags::none)
                    {
                        if (argumentMatch.postReferenceConversionFlags == cmajor::ir::OperationFlags::addr)
                        {
                            cmajor::symbols::TypeSymbol* type = returnValueArguments[0]->GetType()->AddLvalueReference(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
                            BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(returnValueArguments[0]), type);
                            returnValueArguments[0].reset(addressOfExpression);
                        }
                        else if (argumentMatch.postReferenceConversionFlags == cmajor::ir::OperationFlags::deref)
                        {
                            cmajor::symbols::TypeSymbol* type = returnValueArguments[0]->GetType()->RemoveReference(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
                            BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(returnValueArguments[0]), type);
                            returnValueArguments[0].reset(dereferenceExpression);
                        }
                    }
                    returnFunctionCall->SetArguments(std::move(returnValueArguments));
                }
                else
                {
                    throw cmajor::symbols::Exception("no implicit conversion from '" + util::ToUtf8(returnValueArguments[0]->GetType()->FullName()) + "' to '" + util::ToUtf8(returnType->FullName()) + "' exists",
                        returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId(), currentFunction->GetFunctionSymbol()->GetSourcePos(), currentFunction->GetFunctionSymbol()->SourceModuleId());
                }
                CheckAccess(currentFunction->GetFunctionSymbol(), returnFunctionCall->GetFunctionSymbol());
                if (exceptionCapture)
                {
                    AddReleaseExceptionStatement(returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId());
                }
                AddStatement(new BoundReturnStatement(std::move(returnFunctionCall), returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId()));
            }
            else
            {
                if (returnType)
                {
                    throw cmajor::symbols::Exception("void function cannot return a value", returnStatementNode.Expression()->GetSourcePos(), returnStatementNode.Expression()->ModuleId(),
                        currentFunction->GetFunctionSymbol()->GetSourcePos(), currentFunction->GetFunctionSymbol()->SourceModuleId());
                }
                else
                {
                    throw cmajor::symbols::Exception("constructor or assignment function cannot return a value", returnStatementNode.Expression()->GetSourcePos(), returnStatementNode.Expression()->ModuleId(),
                        currentFunction->GetFunctionSymbol()->GetSourcePos(), currentFunction->GetFunctionSymbol()->SourceModuleId());
                }
            }
        }
    }
    else
    {
        cmajor::symbols::TypeSymbol* returnType = currentFunction->GetFunctionSymbol()->ReturnType();
        if (!returnType || returnType->GetSymbolType() == cmajor::symbols::SymbolType::voidTypeSymbol)
        {
            std::unique_ptr<BoundFunctionCall> returnFunctionCall;
            AddStatement(new BoundReturnStatement(std::move(returnFunctionCall), returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId()));
        }
        else
        {
            throw cmajor::symbols::Exception("nonvoid function must return a value", returnStatementNode.GetSourcePos(), returnStatementNode.ModuleId(),
                currentFunction->GetFunctionSymbol()->GetSourcePos(), currentFunction->GetFunctionSymbol()->SourceModuleId());
        }
    }
}

void StatementBinder::Visit(cmajor::ast::IfStatementNode& ifStatementNode)
{
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> condition = BindExpression(ifStatementNode.Condition(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && condition->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (!TypesEqual(symbolTable.GetTypeByName(U"bool"), condition->GetType()->PlainType(ifStatementNode.GetSourcePos(), ifStatementNode.ModuleId())))
    {
        throw cmajor::symbols::Exception("condition of an if statement must be a Boolean expression", ifStatementNode.Condition()->GetSourcePos(), ifStatementNode.Condition()->ModuleId());
    }
    if (condition->GetType()->IsReferenceType())
    {
        cmajor::symbols::TypeSymbol* baseType = condition->GetType()->BaseType();
        condition.reset(new BoundDereferenceExpression(std::move(condition), baseType));
    }
    std::unique_ptr<BoundStatement> s;
    if (statement)
    {
        s = std::move(statement);
    }
    ifStatementNode.ThenS()->Accept(*this);
    BoundStatement* thenS = statement.release();
    BoundStatement* elseS = nullptr;
    if (ifStatementNode.ElseS())
    {
        ifStatementNode.ElseS()->Accept(*this);
        elseS = statement.release();
    }
    if (s)
    {
        AddStatement(s.release());
    }
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(ifStatementNode.GetSourcePos(), ifStatementNode.ModuleId());
    }
    AddStatement(new BoundIfStatement(ifStatementNode.GetSourcePos(), ifStatementNode.ModuleId(), std::move(condition), std::unique_ptr<BoundStatement>(thenS), std::unique_ptr<BoundStatement>(elseS)));
}

void StatementBinder::Visit(cmajor::ast::WhileStatementNode& whileStatementNode)
{
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> condition = BindExpression(whileStatementNode.Condition(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && condition->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (!TypesEqual(symbolTable.GetTypeByName(U"bool"), condition->GetType()->PlainType(whileStatementNode.GetSourcePos(), whileStatementNode.ModuleId())))
    {
        throw cmajor::symbols::Exception("condition of a while statement must be a Boolean expression", whileStatementNode.Condition()->GetSourcePos(), whileStatementNode.Condition()->ModuleId());
    }
    if (condition->GetType()->IsReferenceType())
    {
        cmajor::symbols::TypeSymbol* baseType = condition->GetType()->BaseType();
        condition.reset(new BoundDereferenceExpression(std::move(condition), baseType));
    }
    std::unique_ptr<BoundStatement> s;
    if (statement)
    {
        s = std::move(statement);
    }
    whileStatementNode.Statement()->Accept(*this);
    BoundStatement* stmt = statement.release();
    if (s)
    {
        AddStatement(s.release());
    }
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(whileStatementNode.GetSourcePos(), whileStatementNode.ModuleId());
    }
    AddStatement(new BoundWhileStatement(whileStatementNode.GetSourcePos(), whileStatementNode.ModuleId(), std::move(condition), std::unique_ptr<BoundStatement>(stmt)));
}

void StatementBinder::Visit(cmajor::ast::DoStatementNode& doStatementNode)
{
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> condition = BindExpression(doStatementNode.Condition(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && condition->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (!TypesEqual(symbolTable.GetTypeByName(U"bool"), condition->GetType()->PlainType(doStatementNode.GetSourcePos(), doStatementNode.ModuleId())))
    {
        throw cmajor::symbols::Exception("condition of a do statement must be a Boolean expression", doStatementNode.Condition()->GetSourcePos(), doStatementNode.Condition()->ModuleId());
    }
    if (condition->GetType()->IsReferenceType())
    {
        cmajor::symbols::TypeSymbol* baseType = condition->GetType()->BaseType();
        condition.reset(new BoundDereferenceExpression(std::move(condition), baseType));
    }
    std::unique_ptr<BoundStatement> s;
    if (statement)
    {
        s = std::move(statement);
    }
    doStatementNode.Statement()->Accept(*this);
    BoundStatement* stmt = statement.release();
    if (s)
    {
        AddStatement(s.release());
    }
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(doStatementNode.GetSourcePos(), doStatementNode.ModuleId());
    }
    AddStatement(new BoundDoStatement(doStatementNode.GetSourcePos(), doStatementNode.ModuleId(), std::unique_ptr<BoundStatement>(stmt), std::move(condition)));
}

void StatementBinder::Visit(cmajor::ast::ForStatementNode& forStatementNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&forStatementNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::declarationBlock, "declaration block expected");
    cmajor::symbols::DeclarationBlock* declarationBlock = static_cast<cmajor::symbols::DeclarationBlock*>(symbol);
    containerScope = declarationBlock->GetContainerScope();
    std::unique_ptr<BoundExpression> condition;
    if (forStatementNode.Condition())
    {
        condition = BindExpression(forStatementNode.Condition(), boundCompileUnit, currentFunction, containerScope, this);
    }
    else
    {
        cmajor::ast::BooleanLiteralNode trueNode(forStatementNode.GetSourcePos(), forStatementNode.ModuleId(), true);
        condition = BindExpression(&trueNode, boundCompileUnit, currentFunction, containerScope, this);
    }
    bool exceptionCapture = false;
    if (insideCatch && condition->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (!TypesEqual(symbolTable.GetTypeByName(U"bool"), condition->GetType()->PlainType(forStatementNode.GetSourcePos(), forStatementNode.ModuleId())))
    {
        throw cmajor::symbols::Exception("condition of a for statement must be a Boolean expression", forStatementNode.Condition()->GetSourcePos(), forStatementNode.Condition()->ModuleId());
    }
    if (condition->GetType()->IsReferenceType())
    {
        cmajor::symbols::TypeSymbol* baseType = condition->GetType()->BaseType();
        condition.reset(new BoundDereferenceExpression(std::move(condition), baseType));
    }
    std::unique_ptr<BoundStatement> s;
    if (statement)
    {
        s = std::move(statement);
    }
    forStatementNode.InitS()->Accept(*this);
    BoundStatement* initS = statement.release();
    forStatementNode.LoopS()->Accept(*this);
    BoundStatement* loopS = statement.release();
    loopS->SetForLoopStatementNode();
    forStatementNode.ActionS()->Accept(*this);
    BoundStatement* actionS = statement.release();
    if (s)
    {
        AddStatement(s.release());
    }
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(forStatementNode.GetSourcePos(), forStatementNode.ModuleId());
    }
    AddStatement(new BoundForStatement(forStatementNode.GetSourcePos(), forStatementNode.ModuleId(), std::unique_ptr<BoundStatement>(initS), std::move(condition), std::unique_ptr<BoundStatement>(loopS),
        std::unique_ptr<BoundStatement>(actionS)));
    containerScope = prevContainerScope;
}

void StatementBinder::Visit(cmajor::ast::BreakStatementNode& breakStatementNode)
{
    const cmajor::ast::Node* parent = breakStatementNode.Parent();
    const cmajor::ast::StatementNode* parentStatement = nullptr;
    if (parent && parent->IsStatementNode())
    {
        parentStatement = static_cast<const cmajor::ast::StatementNode*>(parent);
    }
    while (parentStatement && !parentStatement->IsBreakEnclosingStatementNode())
    {
        parent = parentStatement->Parent();
        if (parent && parent->IsStatementNode())
        {
            parentStatement = static_cast<const cmajor::ast::StatementNode*>(parent);
        }
        else
        {
            parentStatement = nullptr;
        }
    }
    if (!parentStatement)
    {
        throw cmajor::symbols::Exception("break statement must be enclosed in a while, do, for or switch statement", breakStatementNode.GetSourcePos(), breakStatementNode.ModuleId());
    }
    AddStatement(new BoundBreakStatement(breakStatementNode.GetSourcePos(), breakStatementNode.ModuleId()));
}

void StatementBinder::Visit(cmajor::ast::ContinueStatementNode& continueStatementNode)
{
    const cmajor::ast::Node* parent = continueStatementNode.Parent();
    const cmajor::ast::StatementNode* parentStatement = nullptr;
    if (parent && parent->IsStatementNode())
    {
        parentStatement = static_cast<const cmajor::ast::StatementNode*>(parent);
    }
    while (parentStatement && !parentStatement->IsContinueEnclosingStatementNode())
    {
        parent = parentStatement->Parent();
        if (parent && parent->IsStatementNode())
        {
            parentStatement = static_cast<const cmajor::ast::StatementNode*>(parent);
        }
        else
        {
            parentStatement = nullptr;
        }
    }
    if (!parentStatement)
    {
        throw cmajor::symbols::Exception("continue statement must be enclosed in a while, do or for statement", continueStatementNode.GetSourcePos(), continueStatementNode.ModuleId());
    }
    AddStatement(new BoundContinueStatement(continueStatementNode.GetSourcePos(), continueStatementNode.ModuleId()));
}

void StatementBinder::Visit(cmajor::ast::GotoStatementNode& gotoStatementNode)
{
    currentFunction->SetHasGotos();
    boundCompileUnit.SetHasGotos();
    AddStatement(new BoundGotoStatement(gotoStatementNode.GetSourcePos(), gotoStatementNode.ModuleId(), gotoStatementNode.Target()));
}

void StatementBinder::Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode)
{
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&constructionStatementNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::localVariableSymbol, "local variable symbol expected");
    cmajor::symbols::LocalVariableSymbol* localVariableSymbol = static_cast<cmajor::symbols::LocalVariableSymbol*>(symbol);
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    BoundExpression* localVariable = new BoundLocalVariable(constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId(), localVariableSymbol);
    arguments.push_back(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(localVariable),
        localVariable->GetType()->AddPointer(constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId()))));
    bool constructDelegateOrClassDelegateType =
        localVariableSymbol->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol ||
        localVariableSymbol->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol;
    std::vector<FunctionScopeLookup> functionScopeLookups;
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, localVariableSymbol->GetType()->ClassInterfaceEnumDelegateOrNsScope()));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    bool exceptionCapture = false;
    int n = constructionStatementNode.Arguments().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Node* argumentNode = constructionStatementNode.Arguments()[i];
        std::unique_ptr<BoundExpression> argument = BindExpression(argumentNode, boundCompileUnit, currentFunction, containerScope, this, false, constructDelegateOrClassDelegateType);
        if (insideCatch && argument->ContainsExceptionCapture())
        {
            exceptionCapture = true;
        }
        arguments.push_back(std::move(argument));
    }
    std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, functionScopeLookups, arguments, boundCompileUnit, currentFunction,
        constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
    cmajor::symbols::FunctionSymbol* functionSymbol = constructorCall->GetFunctionSymbol();
    CheckAccess(currentFunction->GetFunctionSymbol(), functionSymbol);
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
    }
    if (functionSymbol->Parent()->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(functionSymbol->Parent());
        if (classType->Destructor() && classType->Destructor()->IsGeneratedFunction() && !cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::info))
        {
            if (!boundCompileUnit.IsGeneratedDestructorInstantiated(classType->Destructor()))
            {
                boundCompileUnit.SetGeneratedDestructorInstantiated(classType->Destructor());
                std::unique_ptr<BoundClass> boundClass(new BoundClass(classType));
                GenerateDestructorImplementation(boundClass.get(), classType->Destructor(), boundCompileUnit, containerScope, currentFunction,
                    constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
                boundCompileUnit.AddBoundNode(std::move(boundClass));
            }
        }
    }
    BoundConstructionStatement* boundConstructionStatement = new BoundConstructionStatement(std::move(constructorCall), constructionStatementNode.GetSourcePos(), constructionStatementNode.ModuleId());
    boundConstructionStatement->SetLocalVariable(localVariableSymbol);
    AddStatement(boundConstructionStatement);
}

void StatementBinder::Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode)
{
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId = util::nil_uuid();
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        sourcePos = deleteStatementNode.GetSourcePos();
        moduleId = deleteStatementNode.ModuleId();
    }
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> ptr = BindExpression(deleteStatementNode.Expression(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && ptr->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        if (cmajor::symbols::GetConfig() == "debug")
        {
            std::vector<FunctionScopeLookup> lookups;
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> arguments;
            arguments.push_back(std::move(std::unique_ptr<BoundExpression>(ptr->Clone())));
            std::unique_ptr<BoundFunctionCall> disposeCall = ResolveOverload(U"RtDispose", containerScope, lookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId);
            CheckAccess(currentFunction->GetFunctionSymbol(), disposeCall->GetFunctionSymbol());
            AddStatement(new BoundExpressionStatement(std::move(disposeCall), sourcePos, moduleId));
        }
    }
    std::unique_ptr<BoundExpression> memFreePtr;
    cmajor::symbols::TypeSymbol* baseType = ptr->GetType()->BaseType();
    if (baseType->HasNontrivialDestructor())
    {
        Assert(baseType->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
             baseType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type expected"); 
        cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(baseType);
        std::vector<FunctionScopeLookup> lookups;
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->ClassInterfaceOrNsScope()));
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
        std::vector<std::unique_ptr<BoundExpression>> arguments;
        arguments.push_back(std::move(ptr));
        std::unique_ptr<BoundFunctionCall> destructorCall = ResolveOverload(U"@destructor", containerScope, lookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId);
        CheckAccess(currentFunction->GetFunctionSymbol(), destructorCall->GetFunctionSymbol());
        if (destructorCall->GetFunctionSymbol()->IsVirtualAbstractOrOverride())
        {
            destructorCall->SetFlag(BoundExpressionFlags::virtualCall);
        }
        AddStatement(new BoundExpressionStatement(std::move(destructorCall), sourcePos, moduleId));
        memFreePtr = BindExpression(deleteStatementNode.Expression(), boundCompileUnit, currentFunction, containerScope, this);
        if (insideCatch && memFreePtr->ContainsExceptionCapture())
        {
            exceptionCapture = true;
        }
    }
    else
    {
        memFreePtr = std::move(ptr);
    }
    std::vector<FunctionScopeLookup> lookups;
    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    arguments.push_back(std::move(memFreePtr));
    const char32_t* memFreeFunctionName = U"";
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        memFreeFunctionName = U"RtMemFree";
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        memFreeFunctionName = U"MemFree";
    }
    std::unique_ptr<BoundFunctionCall> memFreeCall = ResolveOverload(memFreeFunctionName, containerScope, lookups, arguments, boundCompileUnit, currentFunction, sourcePos, moduleId);
    CheckAccess(currentFunction->GetFunctionSymbol(), memFreeCall->GetFunctionSymbol());
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(deleteStatementNode.GetSourcePos(), deleteStatementNode.ModuleId());
    }
    AddStatement(new BoundExpressionStatement(std::move(memFreeCall), deleteStatementNode.GetSourcePos(), deleteStatementNode.ModuleId()));
}

void StatementBinder::Visit(cmajor::ast::DestroyStatementNode& destroyStatementNode)
{
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> ptr = BindExpression(destroyStatementNode.Expression(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && ptr->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (!ptr->GetType()->IsPointerType())
    {
        throw cmajor::symbols::Exception("destroy statement needs pointer type operand", destroyStatementNode.GetSourcePos(), destroyStatementNode.ModuleId());
    }
    cmajor::symbols::TypeSymbol* pointeeType = ptr->GetType()->RemovePointer(destroyStatementNode.GetSourcePos(), destroyStatementNode.ModuleId());
    if (pointeeType->HasNontrivialDestructor())
    {
        Assert(pointeeType->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
            pointeeType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol, "class type expected");
        cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(pointeeType);
        std::vector<FunctionScopeLookup> lookups;
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, classType->ClassInterfaceOrNsScope()));
        lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
        std::vector<std::unique_ptr<BoundExpression>> arguments;
        arguments.push_back(std::move(ptr));
        std::unique_ptr<BoundFunctionCall> destructorCall = ResolveOverload(U"@destructor", containerScope, lookups, arguments, boundCompileUnit, currentFunction,
            destroyStatementNode.GetSourcePos(), destroyStatementNode.ModuleId());
        CheckAccess(currentFunction->GetFunctionSymbol(), destructorCall->GetFunctionSymbol());
        if (destructorCall->GetFunctionSymbol()->IsVirtualAbstractOrOverride())
        {
            destructorCall->SetFlag(BoundExpressionFlags::virtualCall);
        }
        if (exceptionCapture)
        {
            AddReleaseExceptionStatement(destroyStatementNode.GetSourcePos(), destroyStatementNode.ModuleId());
        }
        AddStatement(new BoundExpressionStatement(std::move(destructorCall), destroyStatementNode.GetSourcePos(), destroyStatementNode.ModuleId()));
    }
    else
    {
        AddStatement(new BoundEmptyStatement(destroyStatementNode.GetSourcePos(), destroyStatementNode.ModuleId()));
    }
}

void StatementBinder::Visit(cmajor::ast::AssignmentStatementNode& assignmentStatementNode)
{
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> target = BindExpression(assignmentStatementNode.TargetExpr(), boundCompileUnit, currentFunction, containerScope, this, true);
    if (insideCatch && target->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    cmajor::symbols::TypeSymbol* targetPlainType = target->GetType()->PlainType(assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId());
    if ((targetPlainType->IsClassTypeSymbol() || targetPlainType->IsArrayType()) && target->GetType()->IsReferenceType())
    {
        cmajor::symbols::TypeSymbol* type = target->GetType()->RemoveReference(assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId())->AddPointer(
            assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId());
        target.reset(new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(target.release()), type));
    }
    else if (targetPlainType->IsPointerType() && target->GetType()->IsReferenceType())
    {
        cmajor::symbols::TypeSymbol* derefType = target->GetType()->RemoveReference(assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId());
        cmajor::symbols::TypeSymbol* addrOfType = derefType->AddPointer(assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId());
        target.reset(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(
            new BoundDereferenceExpression(std::unique_ptr<BoundExpression>(target.release()), derefType)), addrOfType));
    }
    else
    {
        target.reset(new BoundAddressOfExpression(std::move(target), target->GetType()->AddPointer(assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId())));
    }
    cmajor::symbols::TypeSymbol* targetType = target->GetType()->BaseType();
    bool assignDelegateOrClassDelegateType = targetType->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol || targetType->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol;
    std::unique_ptr<BoundExpression> source = BindExpression(assignmentStatementNode.SourceExpr(), boundCompileUnit, currentFunction, containerScope, this, false, assignDelegateOrClassDelegateType);
    if (insideCatch && source->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    arguments.push_back(std::move(target));
    arguments.push_back(std::move(source));
    std::vector<FunctionScopeLookup> functionScopeLookups;
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_, targetType->ClassInterfaceEnumDelegateOrNsScope()));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
    functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
    std::unique_ptr<BoundFunctionCall> assignmentCall = ResolveOverload(U"operator=", containerScope, functionScopeLookups, arguments, boundCompileUnit, currentFunction,
        assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId());
    CheckAccess(currentFunction->GetFunctionSymbol(), assignmentCall->GetFunctionSymbol());
    currentFunction->MoveTemporaryDestructorCallsTo(*assignmentCall);
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId());
    }
    AddStatement(new BoundAssignmentStatement(std::move(assignmentCall), assignmentStatementNode.GetSourcePos(), assignmentStatementNode.ModuleId()));
}

void StatementBinder::Visit(cmajor::ast::ExpressionStatementNode& expressionStatementNode)
{
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> expression = BindExpression(expressionStatementNode.Expression(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && expression->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(expressionStatementNode.GetSourcePos(), expressionStatementNode.ModuleId());
    }
    AddStatement(new BoundExpressionStatement(std::move(expression), expressionStatementNode.GetSourcePos(), expressionStatementNode.ModuleId()));
}

void StatementBinder::Visit(cmajor::ast::EmptyStatementNode& emptyStatementNode)
{
    AddStatement(new BoundEmptyStatement(emptyStatementNode.GetSourcePos(), emptyStatementNode.ModuleId()));
}

void StatementBinder::Visit(cmajor::ast::RangeForStatementNode& rangeForStatementNode)
{
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId = util::nil_uuid();
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        sourcePos = rangeForStatementNode.GetSourcePos();
        moduleId = rangeForStatementNode.ModuleId();
    }
    if (rangeForStatementNode.Action()->GetNodeType() == cmajor::ast::NodeType::compoundStatementNode)
    {
        cmajor::ast::CompoundStatementNode* action = static_cast<cmajor::ast::CompoundStatementNode*>(rangeForStatementNode.Action());
    }
    soul::ast::SourcePos initSourcePos = rangeForStatementNode.TypeExpr()->GetSourcePos();
    soul::ast::SourcePos containerSpan = rangeForStatementNode.Container()->GetSourcePos();
    std::unique_ptr<BoundExpression> container = BindExpression(rangeForStatementNode.Container(), boundCompileUnit, currentFunction, containerScope, this);
    cmajor::symbols::TypeSymbol* plainContainerType = container->GetType()->PlainType(sourcePos, moduleId);
    std::u32string plainContainerTypeFullName = plainContainerType->FullName();
    cmajor::parser::context::Context parsingContext;
    parsingContext.SetModuleId(moduleId);
    std::u32string content = plainContainerTypeFullName + U"\n";
    auto cmajorLexer = cmajor::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), "");
    // cmajorLexer.SetSeparatorChar('\n'); TODO
    using LexerType = decltype(cmajorLexer);
    std::unique_ptr<cmajor::ast::Node> containerTypeNode(cmajor::type::expr::parser::TypeExprParser<LexerType>::Parse(cmajorLexer, &parsingContext));
    std::unique_ptr<cmajor::ast::IdentifierNode> iteratorTypeNode = nullptr;
    if (container->GetType()->IsConstType())
    {
        iteratorTypeNode.reset(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ConstIterator"));
    }
    else
    {
        iteratorTypeNode.reset(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Iterator"));
    }
    cmajor::ast::CloneContext cloneContext;
    std::unique_ptr<cmajor::ast::CompoundStatementNode> compoundStatementNode(new cmajor::ast::CompoundStatementNode(sourcePos, moduleId));
    if (rangeForStatementNode.Action()->GetNodeType() == cmajor::ast::NodeType::compoundStatementNode)
    {
        cmajor::ast::CompoundStatementNode* action = static_cast<cmajor::ast::CompoundStatementNode*>(rangeForStatementNode.Action());
    }
    compoundStatementNode->SetParent(rangeForStatementNode.Parent());
    cmajor::ast::ConstructionStatementNode* constructEndIteratorStatement = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId,
        new cmajor::ast::DotNode(sourcePos, moduleId, containerTypeNode->Clone(cloneContext), static_cast<cmajor::ast::IdentifierNode*>(iteratorTypeNode->Clone(cloneContext))), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@end"));
    if (container->GetType()->IsConstType())
    {
        constructEndIteratorStatement->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, rangeForStatementNode.Container()->Clone(cloneContext),
            new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"CEnd"))));
    }
    else
    {
        constructEndIteratorStatement->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, rangeForStatementNode.Container()->Clone(cloneContext),
            new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"End"))));
    }
    compoundStatementNode->AddStatement(constructEndIteratorStatement);
    cmajor::ast::ConstructionStatementNode* constructIteratorStatement = new cmajor::ast::ConstructionStatementNode(initSourcePos, moduleId,
        new cmajor::ast::DotNode(sourcePos, moduleId, containerTypeNode->Clone(cloneContext), static_cast<cmajor::ast::IdentifierNode*>(iteratorTypeNode->Clone(cloneContext))), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@it"));
    if (container->GetType()->IsConstType())
    {
        constructIteratorStatement->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, rangeForStatementNode.Container()->Clone(cloneContext),
            new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"CBegin"))));
    }
    else
    {
        constructIteratorStatement->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, rangeForStatementNode.Container()->Clone(cloneContext),
            new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Begin"))));
    }
    cmajor::ast::Node* itNotEndCond = new cmajor::ast::NotEqualNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@it"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@end"));
    cmajor::ast::StatementNode* incrementItStatement = new cmajor::ast::ExpressionStatementNode(containerSpan, moduleId, new cmajor::ast::PrefixIncrementNode(containerSpan, moduleId, new cmajor::ast::IdentifierNode(containerSpan, moduleId, U"@it")));
    cmajor::ast::CompoundStatementNode* actionStatement = new cmajor::ast::CompoundStatementNode(sourcePos, moduleId);
    cmajor::ast::ConstructionStatementNode* constructLoopVarStatement = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId,
        rangeForStatementNode.TypeExpr()->Clone(cloneContext), static_cast<cmajor::ast::IdentifierNode*>(rangeForStatementNode.Id()->Clone(cloneContext)));
    constructLoopVarStatement->AddArgument(new cmajor::ast::DerefNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@it")));
    actionStatement->AddStatement(constructLoopVarStatement);
    actionStatement->AddStatement(static_cast<cmajor::ast::StatementNode*>(rangeForStatementNode.Action()->Clone(cloneContext)));
    cmajor::ast::ForStatementNode* forStatement = new cmajor::ast::ForStatementNode(sourcePos, moduleId, constructIteratorStatement, itNotEndCond, incrementItStatement, actionStatement);
    compoundStatementNode->AddStatement(forStatement);

    symbolTable.BeginContainer(containerScope->Container());
    cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
    compoundStatementNode->Accept(symbolCreatorVisitor);
    symbolTable.EndContainer();
    TypeBinder typeBinder(boundCompileUnit);
    typeBinder.SetContainerScope(containerScope);
    typeBinder.SetCurrentFunctionSymbol(currentFunction->GetFunctionSymbol());
    compoundStatementNode->Accept(typeBinder);
    compoundStatementNode->Accept(*this);
}

void StatementBinder::Visit(cmajor::ast::SwitchStatementNode& switchStatementNode)
{
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> condition = BindExpression(switchStatementNode.Condition(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && condition->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    cmajor::symbols::TypeSymbol* conditionType = condition->GetType();
    if (conditionType->IsSwitchConditionType())
    {
        if (conditionType->GetSymbolType() == cmajor::symbols::SymbolType::enumTypeSymbol)
        {
            cmajor::symbols::EnumTypeSymbol* enumType = static_cast<cmajor::symbols::EnumTypeSymbol*>(conditionType);
            conditionType = enumType->UnderlyingType();
        }
        cmajor::symbols::TypeSymbol* prevSwitchConditionType = switchConditionType;
        switchConditionType = conditionType;
        std::unordered_map<cmajor::symbols::IntegralValue, cmajor::ast::CaseStatementNode*, cmajor::symbols::IntegralValueHash>* prevCaseValueMap = currentCaseValueMap;
        std::unordered_map<cmajor::symbols::IntegralValue, cmajor::ast::CaseStatementNode*, cmajor::symbols::IntegralValueHash> caseValueMap;
        currentCaseValueMap = &caseValueMap;
        std::vector<std::pair<BoundGotoCaseStatement*, cmajor::symbols::IntegralValue>>* prevGotoCaseStatements = currentGotoCaseStatements;
        std::vector<std::pair<BoundGotoCaseStatement*, cmajor::symbols::IntegralValue>> gotoCaseStatements;
        currentGotoCaseStatements = &gotoCaseStatements;
        std::vector<BoundGotoDefaultStatement*>* prevGotoDefaultStatements = currentGotoDefaultStatements;
        std::vector<BoundGotoDefaultStatement*> gotoDefaultStatements;
        currentGotoDefaultStatements = &gotoDefaultStatements;
        std::unique_ptr<BoundSwitchStatement> boundSwitchStatement(new BoundSwitchStatement(switchStatementNode.GetSourcePos(), switchStatementNode.ModuleId(), std::move(condition)));
        int n = switchStatementNode.Cases().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::CaseStatementNode* caseS = switchStatementNode.Cases()[i];
            caseS->Accept(*this);
            Assert(statement->GetBoundNodeType() == BoundNodeType::boundCaseStatement, "case statement expected");
            boundSwitchStatement->AddCaseStatement(std::unique_ptr<BoundCaseStatement>(static_cast<BoundCaseStatement*>(statement.release())));
        }
        if (switchStatementNode.Default())
        {
            switchStatementNode.Default()->Accept(*this);
            Assert(statement->GetBoundNodeType() == BoundNodeType::boundDefaultStatement, "default statement expected");
            boundSwitchStatement->SetDefaultStatement(std::unique_ptr<BoundDefaultStatement>(static_cast<BoundDefaultStatement*>(statement.release())));
        }
        for (const std::pair<BoundGotoCaseStatement*, cmajor::symbols::IntegralValue>& p : gotoCaseStatements)
        {
            BoundGotoCaseStatement* gotoCaseStatement = p.first;
            cmajor::symbols::IntegralValue integralCaseValue = p.second;
            auto it = caseValueMap.find(integralCaseValue);
            if (it == caseValueMap.cend())
            {
                throw cmajor::symbols::Exception("case not found", gotoCaseStatement->GetSourcePos(), gotoCaseStatement->ModuleId());
            }
        }
        if (!gotoDefaultStatements.empty() && !switchStatementNode.Default())
        {
            throw cmajor::symbols::Exception("switch does not have a default statement", gotoDefaultStatements.front()->GetSourcePos(), gotoDefaultStatements.front()->ModuleId());
        }
        currentGotoCaseStatements = prevGotoCaseStatements;
        currentGotoDefaultStatements = prevGotoDefaultStatements;
        currentCaseValueMap = prevCaseValueMap;
        if (exceptionCapture)
        {
            AddReleaseExceptionStatement(switchStatementNode.GetSourcePos(), switchStatementNode.ModuleId());
        }
        AddStatement(boundSwitchStatement.release());
        switchConditionType = prevSwitchConditionType;
    }
    else
    {
        throw cmajor::symbols::Exception("switch statement condition must be of integer, character, enumerated or Boolean type", switchStatementNode.Condition()->GetSourcePos(), switchStatementNode.Condition()->ModuleId());
    }
}

void StatementBinder::Visit(cmajor::ast::CaseStatementNode& caseStatementNode)
{
    std::unique_ptr<BoundCaseStatement> boundCaseStatement(new BoundCaseStatement(caseStatementNode.GetSourcePos(), caseStatementNode.ModuleId()));
    bool terminated = false;
    int n = caseStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statementNode = caseStatementNode.Statements()[i];
        if (TerminatesCase(statementNode))
        {
            terminated = true;
        }
        statementNode->Accept(*this);
        boundCaseStatement->AddStatement(std::move(statement));
    }
    if (!terminated)
    {
        throw cmajor::symbols::Exception("case must end in break, continue, return, throw, goto, goto case or goto default statement", caseStatementNode.GetSourcePos(), caseStatementNode.ModuleId());
    }
    int ne = caseStatementNode.CaseExprs().Count();
    for (int i = 0; i < ne; ++i)
    {
        cmajor::ast::Node* caseExprNode = caseStatementNode.CaseExprs()[i];
        std::unique_ptr<cmajor::symbols::Value> caseValue = Evaluate(caseExprNode, switchConditionType, containerScope, boundCompileUnit, false, currentFunction, caseExprNode->GetSourcePos(), caseExprNode->ModuleId());
        cmajor::symbols::IntegralValue integralCaseValue(caseValue.get());
        Assert(currentCaseValueMap, "current case value map not set"); 
        auto it = currentCaseValueMap->find(integralCaseValue);
        if (it != currentCaseValueMap->cend())
        {
            throw cmajor::symbols::Exception("case value already used", caseExprNode->GetSourcePos(), caseExprNode->ModuleId());
        }
        (*currentCaseValueMap)[integralCaseValue] = &caseStatementNode;
        boundCaseStatement->AddCaseValue(std::move(caseValue));
    }
    AddStatement(boundCaseStatement.release());
}

void StatementBinder::Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode)
{
    std::unique_ptr<BoundDefaultStatement> boundDefaultStatement(new BoundDefaultStatement(defaultStatementNode.GetSourcePos(), defaultStatementNode.ModuleId()));
    bool terminated = false;
    int n = defaultStatementNode.Statements().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::StatementNode* statementNode = defaultStatementNode.Statements()[i];
        if (TerminatesDefault(statementNode))
        {
            terminated = true;
        }
        statementNode->Accept(*this);
        boundDefaultStatement->AddStatement(std::move(statement));
    }
    if (!terminated)
    {
        throw cmajor::symbols::Exception("default must end in break, continue, return, throw, goto, or goto case statement", defaultStatementNode.GetSourcePos(), defaultStatementNode.ModuleId());
    }
    AddStatement(boundDefaultStatement.release());
}

void StatementBinder::Visit(cmajor::ast::GotoCaseStatementNode& gotoCaseStatementNode)
{
    const cmajor::ast::Node* parent = gotoCaseStatementNode.Parent();
    while (parent && parent->GetNodeType() != cmajor::ast::NodeType::caseStatementNode && parent->GetNodeType() != cmajor::ast::NodeType::defaultStatementNode)
    {
        parent = parent->Parent();
    }
    if (!parent)
    {
        throw cmajor::symbols::Exception("goto case statement must be enclosed in a case or default statement", gotoCaseStatementNode.GetSourcePos(), gotoCaseStatementNode.ModuleId());
    }
    cmajor::ast::Node* caseExprNode = gotoCaseStatementNode.CaseExpr();
    std::unique_ptr<cmajor::symbols::Value> caseValue = Evaluate(caseExprNode, switchConditionType, containerScope, boundCompileUnit, false, currentFunction, gotoCaseStatementNode.GetSourcePos(), gotoCaseStatementNode.ModuleId());
    cmajor::symbols::Value* caseValuePtr = caseValue.get();
    BoundGotoCaseStatement* boundGotoCaseStatement = new BoundGotoCaseStatement(gotoCaseStatementNode.GetSourcePos(), gotoCaseStatementNode.ModuleId(), std::move(caseValue));
    Assert(currentGotoCaseStatements, "current goto case statement vector not set");
    currentGotoCaseStatements->push_back(std::make_pair(boundGotoCaseStatement, cmajor::symbols::IntegralValue(caseValuePtr)));
    AddStatement(boundGotoCaseStatement);
}

void StatementBinder::Visit(cmajor::ast::GotoDefaultStatementNode& gotoDefaultStatementNode)
{
    const cmajor::ast::Node* parent = gotoDefaultStatementNode.Parent();
    while (parent && parent->GetNodeType() != cmajor::ast::NodeType::caseStatementNode)
    {
        parent = parent->Parent();
    }
    if (!parent)
    {
        throw cmajor::symbols::Exception("goto default statement must be enclosed in a case statement", gotoDefaultStatementNode.GetSourcePos(), gotoDefaultStatementNode.ModuleId());
    }
    BoundGotoDefaultStatement* boundGotoDefaultStatement = new BoundGotoDefaultStatement(gotoDefaultStatementNode.GetSourcePos(), gotoDefaultStatementNode.ModuleId());
    Assert(currentGotoDefaultStatements, "current goto default statement vector not set"); 
    currentGotoDefaultStatements->push_back(boundGotoDefaultStatement);
    AddStatement(boundGotoDefaultStatement);
}

void StatementBinder::Visit(cmajor::ast::ThrowStatementNode& throwStatementNode)
{
    bool prevCompilingThrow = compilingThrow;
    compilingThrow = true;
    if (currentFunction->GetFunctionSymbol()->DontThrow() && !currentFunction->GetFunctionSymbol()->HasTry())
    {
        throw cmajor::symbols::Exception("a nothrow function cannot contain a throw statement unless it handles exceptions",
            throwStatementNode.GetSourcePos(), throwStatementNode.ModuleId(), currentFunction->GetFunctionSymbol()->GetSourcePos(), currentFunction->GetFunctionSymbol()->SourceModuleId());
    }
    soul::ast::SourcePos sourcePos = throwStatementNode.GetSourcePos();
    util::uuid moduleId = throwStatementNode.ModuleId();
    cmajor::ast::Node* exceptionExprNode = throwStatementNode.Expression();
    if (exceptionExprNode)
    {
        std::unique_ptr<BoundExpression> boundExceptionExpr = BindExpression(exceptionExprNode, boundCompileUnit, currentFunction, containerScope, this);
        if (boundExceptionExpr->GetType()->PlainType(sourcePos, moduleId)->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* exceptionClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(boundExceptionExpr->GetType()->PlainType(sourcePos, moduleId));
            cmajor::ast::IdentifierNode systemExceptionNode(sourcePos, moduleId, U"System.Exception");
            cmajor::symbols::TypeSymbol* systemExceptionType = ResolveType(&systemExceptionNode, boundCompileUnit, containerScope);
            Assert(systemExceptionType->IsClassTypeSymbol(), "System.Exception not of class type");
            cmajor::symbols::ClassTypeSymbol* systemExceptionClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(systemExceptionType);
            if (exceptionClassType == systemExceptionClassType || exceptionClassType->HasBaseClass(systemExceptionClassType))
            {
                cmajor::ast::NewNode* newNode = new cmajor::ast::NewNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, exceptionClassType->FullName()));
                cmajor::ast::CloneContext cloneContext;
                newNode->AddArgument(throwStatementNode.Expression()->Clone(cloneContext));
                if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
                {
                    cmajor::ast::InvokeNode invokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"RtThrowException"));
                    invokeNode.AddArgument(newNode);
                    invokeNode.AddArgument(new cmajor::ast::UuidLiteralNode(sourcePos, moduleId, exceptionClassType->TypeId()));
                    std::unique_ptr<BoundExpression> throwCallExpr = BindExpression(&invokeNode, boundCompileUnit, currentFunction, containerScope, this);
                    AddStatement(new BoundThrowStatement(sourcePos, moduleId, std::move(throwCallExpr)));
                }
                else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
                {
                    cmajor::ast::InvokeNode invokeNode(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Throw")));
                    invokeNode.AddArgument(newNode);
                    std::unique_ptr<BoundExpression> throwCallExpr = BindExpression(&invokeNode, boundCompileUnit, currentFunction, containerScope, this);
                    AddStatement(new BoundThrowStatement(sourcePos, moduleId, std::move(throwCallExpr)));
                }
            }
            else
            {
                throw cmajor::symbols::Exception("exception class must be derived from System.Exception class", throwStatementNode.GetSourcePos(), throwStatementNode.ModuleId());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("exception not of class type", throwStatementNode.GetSourcePos(), throwStatementNode.ModuleId());
        }
    }
    else
    {
        if (insideCatch)
        {
            cmajor::ast::InvokeNode invokeNode(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@exPtr"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Release")));
            std::unique_ptr<BoundExpression> releaseCall = BindExpression(&invokeNode, boundCompileUnit, currentFunction, containerScope, this);
            AddStatement(new BoundRethrowStatement(sourcePos, moduleId, std::move(releaseCall)));
        }
        else
        {
            throw cmajor::symbols::Exception("rethrow must occur inside a catch clause", throwStatementNode.GetSourcePos(), throwStatementNode.ModuleId());
        }
    }
    compilingThrow = prevCompilingThrow;
}

void StatementBinder::Visit(cmajor::ast::TryStatementNode& tryStatementNode)
{
    BoundTryStatement* boundTryStatement = new BoundTryStatement(tryStatementNode.GetSourcePos(), tryStatementNode.ModuleId());
    tryStatementNode.TryBlock()->Accept(*this);
    boundTryStatement->SetTryBlock(std::move(statement));
    int n = tryStatementNode.Catches().Count();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::CatchNode* catchNode = tryStatementNode.Catches()[i];
        catchNode->Accept(*this);
        BoundStatement* s = statement.release();
        Assert(s->GetBoundNodeType() == BoundNodeType::boundCatchStatement, "catch statement expected");
        BoundCatchStatement* catchStatement = static_cast<BoundCatchStatement*>(s);
        boundTryStatement->AddCatch(std::unique_ptr<BoundCatchStatement>(catchStatement));
    }
    AddStatement(boundTryStatement);
}

void StatementBinder::Visit(cmajor::ast::CatchNode& catchNode)
{
    bool prevInsideCatch = insideCatch;
    insideCatch = true;
    soul::ast::SourcePos sourcePos;
    util::uuid moduleId = util::nil_uuid();
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        sourcePos = catchNode.GetSourcePos();
        moduleId = catchNode.ModuleId();
    }
    std::unique_ptr<BoundCatchStatement> boundCatchStatement(new BoundCatchStatement(sourcePos, moduleId));
    cmajor::symbols::TypeSymbol* caughtType = ResolveType(catchNode.TypeExpr(), boundCompileUnit, containerScope);
    boundCatchStatement->SetCaughtType(caughtType);
    boundCatchStatement->SetCatchTypeUuidId(boundCompileUnit.Install(caughtType->BaseType()->TypeId()));
    cmajor::symbols::LocalVariableSymbol* catchVar = nullptr;
    if (catchNode.Id())
    {
        cmajor::symbols::Symbol* symbol = symbolTable.GetSymbol(catchNode.Id());
        Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::localVariableSymbol, "local variable symbol expected"); 
        catchVar = static_cast<cmajor::symbols::LocalVariableSymbol*>(symbol);
        boundCatchStatement->SetCatchVar(catchVar);
        currentFunction->GetFunctionSymbol()->AddLocalVariable(catchVar);
    }
    cmajor::ast::CompoundStatementNode handlerBlock(catchNode.CatchBlock()->GetSourcePos(), moduleId);
    handlerBlock.SetParent(catchNode.Parent());
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        cmajor::ast::ConstructionStatementNode* getExceptionAddr = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"void")),
            new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@exceptionAddr"));
        getExceptionAddr->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"RtGetException")));
        handlerBlock.AddStatement(getExceptionAddr);
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        cmajor::ast::ConstructionStatementNode* getExceptionAddr = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"void")),
            new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@exceptionAddr"));
        getExceptionAddr->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"do_catch")));
        handlerBlock.AddStatement(getExceptionAddr);
    }
    cmajor::ast::PointerNode exceptionPtrTypeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, caughtType->BaseType()->FullName()));
    cmajor::ast::CloneContext cloneContext;
    cmajor::ast::ConstructionStatementNode* constructExceptionPtr = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, exceptionPtrTypeNode.Clone(cloneContext),
        new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@exceptionPtr"));
    constructExceptionPtr->AddArgument(new cmajor::ast::CastNode(sourcePos, moduleId, exceptionPtrTypeNode.Clone(cloneContext), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@exceptionAddr")));
    handlerBlock.AddStatement(constructExceptionPtr);
    cmajor::ast::TemplateIdNode* uniquePtrNode = new cmajor::ast::TemplateIdNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"UniquePtr"));
    uniquePtrNode->AddTemplateArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, caughtType->BaseType()->FullName()));
    cmajor::ast::ConstructionStatementNode* constructUniquePtrException = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, uniquePtrNode, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@exPtr"));
    constructUniquePtrException->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@exceptionPtr"));
    handlerBlock.AddStatement(constructUniquePtrException);
    if (catchVar)
    {
        cmajor::ast::ConstructionStatementNode* setExceptionVar = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, catchNode.TypeExpr()->Clone(cloneContext), static_cast<cmajor::ast::IdentifierNode*>(catchNode.Id()->Clone(cloneContext)));
        setExceptionVar->AddArgument(new cmajor::ast::DerefNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@exPtr")));
        handlerBlock.AddStatement(setExceptionVar);
    }
    handlerBlock.AddStatement(static_cast<cmajor::ast::StatementNode*>(catchNode.CatchBlock()->Clone(cloneContext)));
    symbolTable.BeginContainer(containerScope->Container());
    cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
    handlerBlock.Accept(symbolCreatorVisitor);
    symbolTable.EndContainer();
    TypeBinder typeBinder(boundCompileUnit);
    typeBinder.SetContainerScope(containerScope);
    typeBinder.SetCurrentFunctionSymbol(currentFunction->GetFunctionSymbol());
    handlerBlock.Accept(typeBinder);
    handlerBlock.Accept(*this);
    boundCatchStatement->SetCatchBlock(std::move(statement));
    AddStatement(boundCatchStatement.release());
    insideCatch = prevInsideCatch;
}

void StatementBinder::Visit(cmajor::ast::AssertStatementNode& assertStatementNode)
{
    if (!module)
    {
        throw std::runtime_error("module not set");
    }
    bool unitTesting = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::unitTest) && cmajor::symbols::InUnitTest();
    bool unitTestAssertion = false;
    if (unitTesting)
    {
        cmajor::symbols::FunctionSymbol* functionSymbol = currentFunction->GetFunctionSymbol();
        cmajor::ast::Node* node = symbolTable.GetNodeNoThrow(functionSymbol);
        if (node)
        {
            if (node->GetNodeType() == cmajor::ast::NodeType::functionNode)
            {
                cmajor::ast::FunctionNode* functionNode = static_cast<cmajor::ast::FunctionNode*>(node);
                if ((functionNode->GetSpecifiers() & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
                {
                    unitTestAssertion = true;
                }
            }
        }
    }
    if (unitTestAssertion)
    {
        int32_t assertionLineNumber = assertStatementNode.GetSourcePos().line;
        int32_t assertionIndex = cmajor::symbols::GetNextUnitTestAssertionNumber();
        cmajor::symbols::AddAssertionLineNumber(assertionLineNumber);
        cmajor::ast::InvokeNode* invokeSetUnitTestAssertionResult = new cmajor::ast::InvokeNode(assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId(),
            new cmajor::ast::IdentifierNode(assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId(), U"RtSetUnitTestAssertionResult"));
        invokeSetUnitTestAssertionResult->AddArgument(new cmajor::ast::IntLiteralNode(assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId(), assertionIndex));
        cmajor::ast::CloneContext cloneContext;
        invokeSetUnitTestAssertionResult->AddArgument(assertStatementNode.AssertExpr()->Clone(cloneContext));
        invokeSetUnitTestAssertionResult->AddArgument(new cmajor::ast::IntLiteralNode(assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId(), assertionLineNumber));
        cmajor::ast::ExpressionStatementNode setUnitTestAssertionResult(assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId(), invokeSetUnitTestAssertionResult);
        symbolTable.BeginContainer(containerScope->Container());
        cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
        setUnitTestAssertionResult.Accept(symbolCreatorVisitor);
        symbolTable.EndContainer();
        TypeBinder typeBinder(boundCompileUnit);
        typeBinder.SetContainerScope(containerScope);
        typeBinder.SetCurrentFunctionSymbol(currentFunction->GetFunctionSymbol());
        setUnitTestAssertionResult.Accept(typeBinder);
        setUnitTestAssertionResult.Accept(*this);
    }
    else
    {
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release))
        {
            AddStatement(new BoundEmptyStatement(assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId()));
        }
        else
        {
            std::vector<FunctionScopeLookup> lookups;
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, symbolTable.GlobalNs().GetContainerScope()));
            std::vector<std::unique_ptr<BoundExpression>> arguments;
            cmajor::symbols::TypeSymbol* constCharPtrType = symbolTable.GetTypeByName(U"char")->AddConst(soul::ast::SourcePos(), util::nil_uuid())->AddPointer(soul::ast::SourcePos(), util::nil_uuid());
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::StringValue(soul::ast::SourcePos(), util::nil_uuid(),
                boundCompileUnit.Install(assertStatementNode.AssertExpr()->ToString()), assertStatementNode.AssertExpr()->ToString())), constCharPtrType)));
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::StringValue(soul::ast::SourcePos(), util::nil_uuid(),
                boundCompileUnit.Install(util::ToUtf8(currentFunction->GetFunctionSymbol()->FullName())), util::ToUtf8(currentFunction->GetFunctionSymbol()->FullName()))), constCharPtrType)));
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::StringValue(soul::ast::SourcePos(), util::nil_uuid(),
                boundCompileUnit.Install(cmajor::symbols::GetSourceFilePath(assertStatementNode.GetSourcePos().file, assertStatementNode.ModuleId())),
                cmajor::symbols::GetSourceFilePath(assertStatementNode.GetSourcePos().file, assertStatementNode.ModuleId()))), constCharPtrType)));
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::IntValue(soul::ast::SourcePos(), util::nil_uuid(),
                assertStatementNode.GetSourcePos().line)), symbolTable.GetTypeByName(U"int"))));
            std::unique_ptr<BoundExpression> assertExpression = BindExpression(assertStatementNode.AssertExpr(), boundCompileUnit, currentFunction, containerScope, this);
            const char32_t* failAssertionFunctionName = U"";
            if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
            {
                failAssertionFunctionName = U"RtFailAssertion";
            }
            else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
            {
                failAssertionFunctionName = U"System.FailAssertion";
            }
            std::unique_ptr<BoundStatement> emptyStatement(new BoundEmptyStatement(soul::ast::SourcePos(), util::nil_uuid()));
            emptyStatement->SetIgnoreNode();
            std::unique_ptr<BoundStatement> failAssertionStatement(
                new BoundExpressionStatement(ResolveOverload(failAssertionFunctionName, containerScope, lookups, arguments,
                    boundCompileUnit, currentFunction, soul::ast::SourcePos(), util::nil_uuid()), soul::ast::SourcePos(), util::nil_uuid()));
            failAssertionStatement->SetIgnoreNode();
            std::unique_ptr<BoundStatement> ifStatement(new BoundIfStatement(assertStatementNode.GetSourcePos(), assertStatementNode.ModuleId(), std::move(assertExpression),
                std::unique_ptr<BoundStatement>(emptyStatement.release()),
                std::unique_ptr<BoundStatement>(failAssertionStatement.release())));
            ifStatement->SetAssertNode();
            AddStatement(ifStatement.release());
        }
    }
}

void StatementBinder::Visit(cmajor::ast::ConditionalCompilationPartNode& conditionalCompilationPartNode)
{
    conditionalCompilationPartNode.Expr()->Accept(*this);
}

void StatementBinder::Visit(cmajor::ast::ConditionalCompilationDisjunctionNode& conditionalCompilationDisjunctionNode)
{
    conditionalCompilationDisjunctionNode.Left()->Accept(*this);
    bool left = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationDisjunctionNode.Right()->Accept(*this);
    bool right = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationStack.push(left || right);
}

void StatementBinder::Visit(cmajor::ast::ConditionalCompilationConjunctionNode& conditionalCompilationConjunctionNode)
{
    conditionalCompilationConjunctionNode.Left()->Accept(*this);
    bool left = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationConjunctionNode.Right()->Accept(*this);
    bool right = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationStack.push(left && right);
}

void StatementBinder::Visit(cmajor::ast::ConditionalCompilationNotNode& conditionalCompilationNotNode)
{
    conditionalCompilationNotNode.Expr()->Accept(*this);
    bool operand = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    conditionalCompilationStack.push(!operand);
}

void StatementBinder::Visit(cmajor::ast::ConditionalCompilationPrimaryNode& conditionalCompilationPrimaryNode)
{
    bool defined = module->IsSymbolDefined(conditionalCompilationPrimaryNode.Symbol());
    conditionalCompilationStack.push(defined);
}

void StatementBinder::Visit(cmajor::ast::ConditionalCompilationStatementNode& conditionalCompilationStatementNode)
{
    conditionalCompilationStatementNode.IfPart()->Accept(*this);
    bool defined = conditionalCompilationStack.top();
    conditionalCompilationStack.pop();
    if (defined)
    {
        int n = conditionalCompilationStatementNode.IfPart()->Statements().Count();
        if (n > 0)
        {
            for (int i = 0; i < n; ++i)
            {
                cmajor::ast::StatementNode* statement = conditionalCompilationStatementNode.IfPart()->Statements()[i];
                statement->Accept(*this);
            }
        }
        else
        {
            AddStatement(new BoundEmptyStatement(conditionalCompilationStatementNode.GetSourcePos(), conditionalCompilationStatementNode.ModuleId()));
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
                if (n > 0)
                {
                    for (int i = 0; i < n; ++i)
                    {
                        cmajor::ast::StatementNode* statement = elifPart->Statements()[i];
                        statement->Accept(*this);
                    }
                }
                else
                {
                    AddStatement(new BoundEmptyStatement(conditionalCompilationStatementNode.GetSourcePos(), conditionalCompilationStatementNode.ModuleId()));
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
                if (n > 0)
                {
                    for (int i = 0; i < n; ++i)
                    {
                        cmajor::ast::StatementNode* statement = elsePart->Statements()[i];
                        statement->Accept(*this);
                    }
                }
                else
                {
                    AddStatement(new BoundEmptyStatement(conditionalCompilationStatementNode.GetSourcePos(), conditionalCompilationStatementNode.ModuleId()));
                }
            }
            else
            {
                AddStatement(new BoundEmptyStatement(conditionalCompilationStatementNode.GetSourcePos(), conditionalCompilationStatementNode.ModuleId()));
            }
        }
    }
}

void StatementBinder::CompileStatement(cmajor::ast::Node* statementNode, bool setPostfix)
{
    bool prevPostfix = postfix;
    postfix = setPostfix;
    statementNode->Accept(*this);
    postfix = prevPostfix;
}

void StatementBinder::SetCurrentConstructor(cmajor::symbols::ConstructorSymbol* currentConstructorSymbol_, cmajor::ast::ConstructorNode* currentConstructorNode_)
{
    currentConstructorSymbol = currentConstructorSymbol_;
    currentConstructorNode = currentConstructorNode_;
}

void StatementBinder::SetCurrentDestructor(cmajor::symbols::DestructorSymbol* currentDestructorSymbol_, cmajor::ast::DestructorNode* currentDestructorNode_)
{
    currentDestructorSymbol = currentDestructorSymbol_;
    currentDestructorNode = currentDestructorNode_;
}

void StatementBinder::SetCurrentMemberFunction(cmajor::symbols::MemberFunctionSymbol* currentMemberFunctionSymbol_, cmajor::ast::MemberFunctionNode* currentMemberFunctionNode_)
{
    currentMemberFunctionSymbol = currentMemberFunctionSymbol_;
    currentMemberFunctionNode = currentMemberFunctionNode_;
}

void StatementBinder::AddReleaseExceptionStatement(const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId)
{
    if (insideCatch && !compilingReleaseExceptionStatement)
    {
        compilingReleaseExceptionStatement = true;
        cmajor::ast::InvokeNode* invokeNode(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@exPtr"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Release"))));
        cmajor::ast::ExpressionStatementNode releaseExceptionStatement(sourcePos, moduleId, invokeNode);
        CompileStatement(&releaseExceptionStatement, true);
        compilingReleaseExceptionStatement = false;
    }
}

void StatementBinder::AddStatement(BoundStatement* boundStatement)
{
    if (postfix)
    {
        boundStatement->SetPostfix();
    }
    if (statement)
    {
        if (statement->Postfix())
        {
            BoundSequenceStatement* sequenceStatement = new BoundSequenceStatement(boundStatement->GetSourcePos(), boundStatement->ModuleId(), std::unique_ptr<BoundStatement>(boundStatement), std::move(statement));
            boundStatement = sequenceStatement;
        }
        else
        {
            BoundSequenceStatement* sequenceStatement = new BoundSequenceStatement(boundStatement->GetSourcePos(), boundStatement->ModuleId(), std::move(statement), std::unique_ptr<BoundStatement>(boundStatement));
            boundStatement = sequenceStatement;
        }
        if (postfix)
        {
            boundStatement->SetPostfix();
        }
    }
    statement.reset(boundStatement);
}

} // namespace cmajor::binder
