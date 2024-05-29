// =================================
// Copyright (c) 2024 Seppo Laakko
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
    std::unique_ptr<cmajor::symbols::Value> value = Evaluate(node, boundCompileUnit.GetSymbolTable().GetTypeByName(U"bool"), containerScope, boundCompileUnit, true, nullptr);
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

void CheckFunctionReturnPaths(cmajor::symbols::FunctionSymbol* functionSymbol, cmajor::ast::CompoundStatementNode* bodyNode, cmajor::ast::Node* node,
    cmajor::symbols::ContainerScope* containerScope, BoundCompileUnit& boundCompileUnit);

void CheckFunctionReturnPaths(cmajor::symbols::FunctionSymbol* functionSymbol, cmajor::ast::FunctionNode& functionNode, cmajor::symbols::ContainerScope* containerScope, 
    BoundCompileUnit& boundCompileUnit)
{
    CheckFunctionReturnPaths(functionSymbol, functionNode.Body(), &functionNode, containerScope, boundCompileUnit);
}

void CheckFunctionReturnPaths(cmajor::symbols::FunctionSymbol* functionSymbol, cmajor::ast::CompoundStatementNode* bodyNode, cmajor::ast::Node* node,
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
        throw cmajor::symbols::Exception("not all control paths terminate in return or throw statement", node->GetFullSpan());
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
        boundCompileUnit.GenerateCompileUnitInitialization(compileUnitNode.GetSpan());
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
            if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
            {
                cmajor::symbols::DestructorSymbol* copy = static_cast<cmajor::symbols::DestructorSymbol*>(destructorSymbol->Copy());
                boundCompileUnit.GetSymbolTable().AddFunctionSymbol(std::unique_ptr<cmajor::symbols::FunctionSymbol>(copy));
                cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit.GetCompileUnitNode();
                if (compileUnitNode)
                {
                    copy->SetCompileUnitId(compileUnitNode->Id());
                    copy->ComputeMangledName();
                }
                destructorSymbol->SetInstantiatedName(copy->MangledName());
                destructorSymbol = copy;
            }
            GenerateDestructorImplementation(currentClass, destructorSymbol, boundCompileUnit, containerScope, currentFunction, &classNode);
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
                if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
                {
                    cmajor::symbols::DestructorSymbol* copy = static_cast<cmajor::symbols::DestructorSymbol*>(destructorSymbol->Copy());
                    boundCompileUnit.GetSymbolTable().AddFunctionSymbol(std::unique_ptr<cmajor::symbols::FunctionSymbol>(copy));
                    cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit.GetCompileUnitNode();
                    if (compileUnitNode)
                    {
                        copy->SetCompileUnitId(compileUnitNode->Id());
                        copy->ComputeMangledName();
                    }
                    destructorSymbol->SetInstantiatedName(copy->MangledName());
                    destructorSymbol = copy;
                }
                GenerateDestructorImplementation(boundClass.get(), destructorSymbol, boundCompileUnit, containerScope, currentFunction, &memberVariableNode);
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
            fullInstantiationRequestNode.GetFullSpan());
    }
    cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(type);
    util::LogMessage(module->LogStreamId(), "generating full instantiation of '" + util::ToUtf8(specialization->FullName()) + "'");
    GetBoundCompileUnit().GetClassTemplateRepository().InstantiateAll(specialization, containerScope, currentFunction, &fullInstantiationRequestNode);
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
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp ||
        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        if (cmajor::symbols::GetConfig() == "release") return;
        cmajor::symbols::Module& currentModule = boundFunction->GetBoundCompileUnit()->GetModule();
        if (currentModule.Name() == U"System.Core" || currentModule.Name() == U"System.Runtime") return;
        soul::ast::FullSpan functionSpan = boundFunction->GetFunctionSymbol()->GetFullSpan();
        if (!functionSpan.IsValid())
        {
            return;
        }
        std::string fullFunctionName = util::ToUtf8(boundFunction->GetFunctionSymbol()->FullName());
        std::string sourceFileName = cmajor::symbols::GetSourceFilePath(functionSpan.fileIndex, functionSpan.moduleId);
        if (sourceFileName.empty())
        {
            return;
        }
        int32_t functionId = currentModule.MakeFunctionId(fullFunctionName, sourceFileName);

        soul::ast::Span span = boundFunction->GetSpan();
        cmajor::symbols::TypeSymbol* traceEntryTypeSymbol = boundCompileUnit.GetSystemRuntimeTraceEntryTypeSymbol();
        if (!traceEntryTypeSymbol)
        {
            cmajor::ast::IdentifierNode systemRuntimeTraceEntryNode(span, U"System.Runtime.TraceEntry");
            traceEntryTypeSymbol = ResolveType(&systemRuntimeTraceEntryNode, boundCompileUnit, containerScope);
            boundCompileUnit.SetSystemRuntimeTraceEntryTypeSymbol(traceEntryTypeSymbol);
        }
        cmajor::symbols::TypeSymbol* traceGuardTypeSymbol = boundCompileUnit.GetSystemRuntimeTraceGuardTypeSymbol();
        if (!traceGuardTypeSymbol)
        {
            cmajor::ast::IdentifierNode systemRuntimeTraceGuardNode(span, U"System.Runtime.TraceGuard");
            traceGuardTypeSymbol = ResolveType(&systemRuntimeTraceGuardNode, boundCompileUnit, containerScope);
            boundCompileUnit.SetSystemRuntimeTraceGuardTypeSymbol(traceGuardTypeSymbol);
        }

        std::unique_ptr<cmajor::ast::IdentifierNode> traceEntryNode(new cmajor::ast::IdentifierNode(span, U"@traceEntry"));

        cmajor::ast::CloneContext cloneContext;

        // System.Runtime.TraceEntry @traceEntry;

        std::unique_ptr<cmajor::ast::ConstructionStatementNode> constructTraceEntry(new cmajor::ast::ConstructionStatementNode(span,
            new cmajor::ast::IdentifierNode(span, U"System.Runtime.TraceEntry"), static_cast<cmajor::ast::IdentifierNode*>(traceEntryNode->Clone(cloneContext))));
        symbolTable.BeginContainer(boundFunction->GetFunctionSymbol());
        symbolTable.AddLocalVariable(*constructTraceEntry);
        symbolTable.EndContainer();
        cmajor::symbols::Symbol* traceEntrySymbol = symbolTable.GetSymbol(constructTraceEntry.get());
        if (traceEntrySymbol && traceEntrySymbol->GetSymbolType() == cmajor::symbols::SymbolType::localVariableSymbol)
        {
            cmajor::symbols::LocalVariableSymbol* traceEntryVariableSymbol = static_cast<cmajor::symbols::LocalVariableSymbol*>(traceEntrySymbol);
            boundFunction->GetFunctionSymbol()->SetTraceEntryVar(traceEntryVariableSymbol);
            traceEntryVariableSymbol->SetType(traceEntryTypeSymbol);
        }
        constructTraceEntry->AddArgument(new cmajor::ast::IntLiteralNode(span, functionId));
        constructTraceEntry->Accept(*this);
        std::unique_ptr<BoundStatement> constructTraceEntryStatement(statement.release());

        // @traceEntry.line = LINE

        std::unique_ptr<cmajor::ast::DotNode> lineDotNode(new cmajor::ast::DotNode(span, traceEntryNode->Clone(cloneContext), new cmajor::ast::IdentifierNode(span, U"line")));
        std::unique_ptr<cmajor::ast::AssignmentStatementNode> assignTraceEntryLine(new cmajor::ast::AssignmentStatementNode(
            span, lineDotNode.release(), new cmajor::ast::IntLiteralNode(span, 0)));
        assignTraceEntryLine->Accept(*this);
        std::unique_ptr<BoundStatement> assignTraceEntryLineStatement(statement.release());
        boundFunction->SetLineCode(std::move(assignTraceEntryLineStatement));

        std::unique_ptr<cmajor::ast::IdentifierNode> traceGuardNode(new cmajor::ast::IdentifierNode(span, U"@traceGuard"));
  
        // System.Runtime.TraceGuard @traceGuard(&@traceEntry);

        std::unique_ptr<cmajor::ast::ConstructionStatementNode> constructTraceGuard(new cmajor::ast::ConstructionStatementNode(span,
            new cmajor::ast::IdentifierNode(span, U"System.Runtime.TraceGuard"), static_cast<cmajor::ast::IdentifierNode*>(traceGuardNode->Clone(cloneContext))));
        symbolTable.BeginContainer(boundFunction->GetFunctionSymbol());
        symbolTable.AddLocalVariable(*constructTraceGuard);
        symbolTable.EndContainer();
        cmajor::symbols::Symbol* traceGuardSymbol = symbolTable.GetSymbol(constructTraceGuard.get());
        if (traceGuardSymbol && traceGuardSymbol->GetSymbolType() == cmajor::symbols::SymbolType::localVariableSymbol)
        {
            cmajor::symbols::LocalVariableSymbol* traceGuardVariableSymbol = static_cast<cmajor::symbols::LocalVariableSymbol*>(traceGuardSymbol);
            boundFunction->GetFunctionSymbol()->SetTraceGuardVar(traceGuardVariableSymbol);
            traceGuardVariableSymbol->SetType(traceGuardTypeSymbol);
        }
        constructTraceGuard->AddArgument(new cmajor::ast::AddrOfNode(span, traceEntryNode->Clone(cloneContext)));
        constructTraceGuard->Accept(*this);
        std::unique_ptr<BoundStatement> constructTraceGuardStatement(statement.release());

        std::vector<std::unique_ptr<BoundStatement>> enterCode;
        enterCode.push_back(std::move(constructTraceEntryStatement));
        enterCode.push_back(std::move(constructTraceGuardStatement));
        boundFunction->SetEnterCode(std::move(enterCode));
    }
    else
    {
        soul::ast::Span span = boundFunction->GetSpan();
        if (boundFunction->GetFunctionSymbol()->DontThrow()) return;
        cmajor::symbols::TypeSymbol* systemRuntimeUnwindInfoSymbol = boundCompileUnit.GetSystemRuntimeUnwindInfoSymbol();
        if (systemRuntimeUnwindInfoSymbol == nullptr)
        {
            cmajor::ast::IdentifierNode systemRuntimeUnwindInfoNode(span, U"System.Runtime.UnwindInfo");
            systemRuntimeUnwindInfoSymbol = ResolveType(&systemRuntimeUnwindInfoNode, boundCompileUnit, containerScope);
            boundCompileUnit.SetSystemRuntimeUnwindInfoSymbol(systemRuntimeUnwindInfoSymbol);
        }
        cmajor::symbols::FunctionSymbol* initUnwindSymbol = boundCompileUnit.GetInitUnwindInfoFunctionSymbol();
        if (initUnwindSymbol == nullptr)
        {
            boundCompileUnit.GenerateInitUnwindInfoFunctionSymbol(span);
        }
        cmajor::symbols::LocalVariableSymbol* prevUnwindInfoVariableSymbol = new cmajor::symbols::LocalVariableSymbol(span, U"@prevUnwindInfo");
        containerScope->Install(prevUnwindInfoVariableSymbol);
        prevUnwindInfoVariableSymbol->SetType(systemRuntimeUnwindInfoSymbol->AddPointer());
        boundFunction->GetFunctionSymbol()->SetPrevUnwindInfoVar(prevUnwindInfoVariableSymbol);
        cmajor::ast::IdentifierNode* prevUnwindInfoNode1 = new cmajor::ast::IdentifierNode(span, U"@prevUnwindInfo");
        symbolTable.MapSymbol(prevUnwindInfoNode1, prevUnwindInfoVariableSymbol);
        symbolTable.MapNode(prevUnwindInfoNode1, prevUnwindInfoVariableSymbol);
        cmajor::ast::InvokeNode* pushUnwindInfo = new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"RtPushUnwindInfo"));
        cmajor::symbols::LocalVariableSymbol* unwindInfoVariableSymbol = new cmajor::symbols::LocalVariableSymbol(span, U"@unwindInfo");
        containerScope->Install(unwindInfoVariableSymbol);
        unwindInfoVariableSymbol->SetType(systemRuntimeUnwindInfoSymbol);
        boundFunction->GetFunctionSymbol()->SetUnwindInfoVar(unwindInfoVariableSymbol);
        cmajor::ast::IdentifierNode* unwindInfoNode1 = new cmajor::ast::IdentifierNode(span, U"@unwindInfo");
        symbolTable.MapSymbol(unwindInfoNode1, unwindInfoVariableSymbol);
        symbolTable.MapNode(unwindInfoNode1, unwindInfoVariableSymbol);
        pushUnwindInfo->AddArgument(new cmajor::ast::CastNode(span, new cmajor::ast::PointerNode(span, new cmajor::ast::VoidNode(span)),
            new cmajor::ast::AddrOfNode(span, unwindInfoNode1)));
        cmajor::ast::AssignmentStatementNode assignUnwindInfo(span, prevUnwindInfoNode1,
            new cmajor::ast::CastNode(span, new cmajor::ast::PointerNode(span, new cmajor::ast::IdentifierNode(span, U"System.Runtime.UnwindInfo")), pushUnwindInfo));
        assignUnwindInfo.Accept(*this);
        std::unique_ptr<BoundStatement> pushUnwindInfoStatement(statement.release());

        cmajor::ast::IdentifierNode* prevUnwindInfoNode2 = new cmajor::ast::IdentifierNode(span, U"@prevUnwindInfo");
        symbolTable.MapSymbol(prevUnwindInfoNode2, prevUnwindInfoVariableSymbol);
        symbolTable.MapNode(prevUnwindInfoNode2, prevUnwindInfoVariableSymbol);
        cmajor::ast::IdentifierNode* unwindInfoNode2 = new cmajor::ast::IdentifierNode(span, U"@unwindInfo");
        symbolTable.MapSymbol(unwindInfoNode2, unwindInfoVariableSymbol);
        symbolTable.MapNode(unwindInfoNode2, unwindInfoVariableSymbol);
        cmajor::ast::AssignmentStatementNode assignUnwindInfoNext(span, new cmajor::ast::DotNode(span, unwindInfoNode2, new cmajor::ast::IdentifierNode(span, U"next")),
            prevUnwindInfoNode2);
        assignUnwindInfoNext.Accept(*this);
        std::unique_ptr<BoundStatement> assignUnwindInfoNextStatement(statement.release());

        cmajor::ast::IdentifierNode* unwindInfoNode3 = new cmajor::ast::IdentifierNode(span, U"@unwindInfo");
        symbolTable.MapSymbol(unwindInfoNode3, unwindInfoVariableSymbol);
        symbolTable.MapNode(unwindInfoNode3, unwindInfoVariableSymbol);
        cmajor::ast::FunctionPtrNode* functionPtrNode = new cmajor::ast::FunctionPtrNode(span);
        BoundFunctionPtr* boundFunctionPtr = new BoundFunctionPtr(span, boundFunction->GetFunctionSymbol(), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer());
        BoundBitCast* boundBitCast = new BoundBitCast(std::unique_ptr<BoundExpression>(boundFunctionPtr), boundCompileUnit.GetSymbolTable().GetTypeByName(U"void")->AddPointer());
        std::unique_ptr<BoundExpression> boundFunctionPtrAsVoidPtr(boundBitCast);
        functionPtrNode->SetBoundExpression(boundFunctionPtrAsVoidPtr.get());
        cmajor::ast::AssignmentStatementNode assignFunctionPtr(span, new cmajor::ast::DotNode(span, unwindInfoNode3, new cmajor::ast::IdentifierNode(span, U"function")),
            new cmajor::ast::CastNode(span, new cmajor::ast::PointerNode(span, new cmajor::ast::VoidNode(span)), functionPtrNode));
        assignFunctionPtr.Accept(*this);
        std::unique_ptr<BoundStatement> assignFunctionPtrStatement(statement.release());

        cmajor::ast::IdentifierNode* unwindInfoNode4 = new cmajor::ast::IdentifierNode(span, U"@unwindInfo");
        symbolTable.MapSymbol(unwindInfoNode4, unwindInfoVariableSymbol);
        symbolTable.MapNode(unwindInfoNode4, unwindInfoVariableSymbol);
        cmajor::ast::AssignmentStatementNode assignUnwindInfoLine(span, new cmajor::ast::DotNode(span, unwindInfoNode4, new cmajor::ast::IdentifierNode(span, U"line")),
            new cmajor::ast::IntLiteralNode(span, 0));
        assignUnwindInfoLine.Accept(*this);
        std::unique_ptr<BoundStatement> assignUnwindInfoLineStatement(statement.release());

        cmajor::ast::IdentifierNode* prevUnwindInfoNode3 = new cmajor::ast::IdentifierNode(span, U"@prevUnwindInfo");
        symbolTable.MapSymbol(prevUnwindInfoNode3, prevUnwindInfoVariableSymbol);
        symbolTable.MapNode(prevUnwindInfoNode3, prevUnwindInfoVariableSymbol);
        cmajor::ast::InvokeNode* setPrevUnwindInfoListPtr = new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"RtPopUnwindInfo"));
        setPrevUnwindInfoListPtr->AddArgument(new cmajor::ast::CastNode(span, new cmajor::ast::PointerNode(span, new cmajor::ast::VoidNode(span)), prevUnwindInfoNode3));
        cmajor::ast::ExpressionStatementNode setPrevUnwindInfoList(span, setPrevUnwindInfoListPtr);
        setPrevUnwindInfoList.Accept(*this);
        std::unique_ptr<BoundStatement> setPrevUnwindInfoListStatement(statement.release());

        std::vector<std::unique_ptr<BoundStatement>> enterCode;
        enterCode.push_back(std::move(pushUnwindInfoStatement));
        enterCode.push_back(std::move(assignUnwindInfoNextStatement));
        enterCode.push_back(std::move(assignFunctionPtrStatement));
        enterCode.push_back(std::move(assignUnwindInfoLineStatement));
        boundFunction->SetEnterCode(std::move(enterCode));

        std::unique_ptr<BoundStatement> setLineCode;
        cmajor::ast::IdentifierNode* unwindInfoNode5 = new cmajor::ast::IdentifierNode(span, U"@unwindInfo");
        symbolTable.MapSymbol(unwindInfoNode5, unwindInfoVariableSymbol);
        cmajor::ast::AssignmentStatementNode setUnwindInfoLine(span, new cmajor::ast::DotNode(span, unwindInfoNode5, new cmajor::ast::IdentifierNode(span, U"line")),
            new cmajor::ast::IntLiteralNode(span, 0));
        setUnwindInfoLine.Accept(*this);
        std::unique_ptr<BoundStatement> setLineStatement(statement.release());
        boundFunction->SetLineCode(std::move(setLineStatement));

        std::vector<std::unique_ptr<BoundStatement>> exitCode;
        exitCode.push_back(std::move(setPrevUnwindInfoListStatement));
        boundFunction->SetExitCode(std::move(exitCode));
    }
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
        std::unique_ptr<BoundCompoundStatement> boundCompoundStatement(new BoundCompoundStatement(constructorNode.GetSpan()));
        GenerateClassInitialization(currentConstructorSymbol, currentConstructorNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this, true,
            &constructorNode);
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
        std::unique_ptr<BoundCompoundStatement> boundCompoundStatement(new BoundCompoundStatement(destructorNode.GetSpan()));
        GenerateClassTermination(currentDestructorSymbol, currentDestructorNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this,
            currentDestructorNode);
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
        std::unique_ptr<BoundCompoundStatement> boundCompoundStatement(new BoundCompoundStatement(memberFunctionNode.GetSpan()));
        GenerateClassAssignment(currentMemberFunctionSymbol, currentMemberFunctionNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this, true,
            &memberFunctionNode);
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
    std::unique_ptr<BoundCompoundStatement> boundCompoundStatement(new BoundCompoundStatement(compoundStatementNode.GetSpan(), compoundStatementNode.EndSpan()));
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
                cmajor::ast::ConstructionStatementNode constructFunctionProfiler(compoundStatementNode.GetSpan(),
                    new cmajor::ast::IdentifierNode(compoundStatementNode.GetSpan(), U"System.Runtime.FunctionProfiler"),
                    new cmajor::ast::IdentifierNode(compoundStatementNode.GetSpan(), U"@functionProfiler"));
                constructFunctionProfiler.AddArgument(new cmajor::ast::UuidLiteralNode(compoundStatementNode.GetSpan(), functionId));
                std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().GetLock());
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
            GenerateStaticClassInitialization(currentStaticConstructorSymbol, currentStaticConstructorNode, boundCompileUnit, boundCompoundStatement.get(), 
                currentFunction, containerScope, this, &compoundStatementNode);
        }
        else if (currentConstructorSymbol && currentConstructorNode)
        {
            GenerateClassInitialization(currentConstructorSymbol, currentConstructorNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this, false,
                &compoundStatementNode);
        }
        else if (currentMemberFunctionSymbol && currentMemberFunctionSymbol->GroupName() == U"operator=" && currentMemberFunctionNode)
        {
            GenerateClassAssignment(currentMemberFunctionSymbol, currentMemberFunctionNode, boundCompoundStatement.get(), currentFunction, boundCompileUnit, containerScope, this, false,
                &compoundStatementNode);
        }
        else if (currentMemberFunctionSymbol && currentMemberFunctionSymbol->IsStatic() && currentMemberFunctionNode)
        {
            if (currentClass->GetClassTypeSymbol()->StaticConstructor())
            {
                boundCompoundStatement->AddStatement(std::unique_ptr<BoundStatement>(new BoundInitializationStatement(std::unique_ptr<BoundExpression>(
                    new BoundFunctionCall(compoundStatementNode.GetSpan(), currentClass->GetClassTypeSymbol()->StaticConstructor())))));
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
            &compoundStatementNode);
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
            classReturnArgs.push_back(std::unique_ptr<BoundExpression>(new BoundParameter(returnStatementNode.GetSpan(), currentFunction->GetFunctionSymbol()->ReturnParam())));
            cmajor::symbols::TypeSymbol* returnType = currentFunction->GetFunctionSymbol()->ReturnType();
            bool returnClassDelegateType = returnType->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol;
            std::unique_ptr<BoundExpression> expression = BindExpression(returnStatementNode.Expression(), boundCompileUnit, currentFunction, containerScope, this, false, 
                returnClassDelegateType, returnClassDelegateType);
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
                        boundCompileUnit.GetConversion(exprType, returnType, containerScope, currentFunction, argumentMatch, &returnStatementNode)));
                }
                rvalueArguments.push_back(std::move(expression));
                std::unique_ptr<BoundExpression> rvalueExpr = ResolveOverload(U"System.Rvalue", containerScope, rvalueLookups, rvalueArguments, boundCompileUnit, currentFunction,
                    &returnStatementNode);
                expression = std::move(rvalueExpr);
            }
            classReturnArgs.push_back(std::move(expression));
            std::unique_ptr<BoundFunctionCall> constructorCall = ResolveOverload(U"@constructor", containerScope, classReturnLookups, classReturnArgs, boundCompileUnit, currentFunction,
                &returnStatementNode);
            std::unique_ptr<BoundStatement> constructStatement(new BoundInitializationStatement(std::move(constructorCall)));
            AddStatement(constructStatement.release());
            std::unique_ptr<BoundFunctionCall> returnFunctionCall;
            std::unique_ptr<BoundStatement> returnStatement(new BoundReturnStatement(std::move(returnFunctionCall), returnStatementNode.GetSpan()));
            if (exceptionCapture)
            {
                AddReleaseExceptionStatement(&returnStatementNode);
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
                BoundTypeExpression* boundTypeExpression = new BoundTypeExpression(returnStatementNode.GetSpan(), returnType);
                returnTypeArgs.push_back(std::unique_ptr<BoundTypeExpression>(boundTypeExpression));
                std::vector<FunctionScopeLookup> functionScopeLookups;
                functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
                functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, 
                    returnType->BaseType()->ClassInterfaceEnumDelegateOrNsScope()));
                functionScopeLookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
                std::unique_ptr<BoundFunctionCall> returnFunctionCall = ResolveOverload(U"@return", containerScope, functionScopeLookups, returnTypeArgs, boundCompileUnit, 
                    currentFunction, &returnStatementNode);
                std::unique_ptr<BoundExpression> expression = BindExpression(returnStatementNode.Expression(), boundCompileUnit, currentFunction, containerScope, this, false,
                    returnDelegateType || returnClassDelegateType, returnClassDelegateType);
                if (insideCatch && expression->ContainsExceptionCapture())
                {
                    exceptionCapture = true;
                }
                std::vector<std::unique_ptr<BoundExpression>> returnValueArguments;
                returnValueArguments.push_back(std::move(expression));
                FunctionMatch functionMatch(returnFunctionCall->GetFunctionSymbol());
                bool conversionFound = FindConversions(boundCompileUnit, returnFunctionCall->GetFunctionSymbol(), returnValueArguments, functionMatch, 
                    cmajor::symbols::ConversionType::implicit_, containerScope, currentFunction, &returnStatementNode);
                if (conversionFound)
                {
                    Assert(!functionMatch.argumentMatches.empty(), "argument match expected");
                    ArgumentMatch argumentMatch = functionMatch.argumentMatches[0];
                    if (argumentMatch.preReferenceConversionFlags != cmajor::ir::OperationFlags::none)
                    {
                        if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::addr)
                        {
                            cmajor::symbols::TypeSymbol* type = returnValueArguments[0]->GetType()->AddLvalueReference();
                            BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(returnValueArguments[0]), type);
                            returnValueArguments[0].reset(addressOfExpression);
                        }
                        else if (argumentMatch.preReferenceConversionFlags == cmajor::ir::OperationFlags::deref)
                        {
                            cmajor::symbols::TypeSymbol* type = returnValueArguments[0]->GetType()->RemoveReference();
                            BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(returnValueArguments[0]), type);
                            returnValueArguments[0].reset(dereferenceExpression);
                        }
                    }
                    cmajor::symbols::FunctionSymbol* conversionFun = argumentMatch.conversionFun;
                    if (conversionFun)
                    {
                        if (conversionFun->GetSymbolType() == cmajor::symbols::SymbolType::constructorSymbol)
                        {
                            BoundFunctionCall* constructorCall = new BoundFunctionCall(returnStatementNode.GetSpan(), conversionFun);
                            cmajor::symbols::LocalVariableSymbol* temporary = currentFunction->GetFunctionSymbol()->CreateTemporary(conversionFun->ConversionTargetType(),
                                returnStatementNode.GetSpan());
                            constructorCall->AddArgument(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(
                                new BoundLocalVariable(returnStatementNode.GetSpan(), temporary)),
                                conversionFun->ConversionTargetType()->AddPointer())));
                            constructorCall->AddArgument(std::move(returnValueArguments[0]));
                            BoundConstructAndReturnTemporaryExpression* conversion = new BoundConstructAndReturnTemporaryExpression(std::unique_ptr<BoundExpression>(constructorCall),
                                std::unique_ptr<BoundExpression>(new BoundLocalVariable(returnStatementNode.GetSpan(), temporary)));
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
                            cmajor::symbols::TypeSymbol* type = returnValueArguments[0]->GetType()->AddLvalueReference();
                            BoundAddressOfExpression* addressOfExpression = new BoundAddressOfExpression(std::move(returnValueArguments[0]), type);
                            returnValueArguments[0].reset(addressOfExpression);
                        }
                        else if (argumentMatch.postReferenceConversionFlags == cmajor::ir::OperationFlags::deref)
                        {
                            cmajor::symbols::TypeSymbol* type = returnValueArguments[0]->GetType()->RemoveReference();
                            BoundDereferenceExpression* dereferenceExpression = new BoundDereferenceExpression(std::move(returnValueArguments[0]), type);
                            returnValueArguments[0].reset(dereferenceExpression);
                        }
                    }
                    returnFunctionCall->SetArguments(std::move(returnValueArguments));
                }
                else
                {
                    throw cmajor::symbols::Exception("no implicit conversion from '" + util::ToUtf8(returnValueArguments[0]->GetType()->FullName()) + "' to '" +
                        util::ToUtf8(returnType->FullName()) + "' exists",
                        returnStatementNode.GetFullSpan(), currentFunction->GetFunctionSymbol()->GetFullSpan());
                }
                CheckAccess(currentFunction->GetFunctionSymbol(), returnFunctionCall->GetFunctionSymbol());
                if (exceptionCapture)
                {
                    AddReleaseExceptionStatement(&returnStatementNode);
                }
                AddStatement(new BoundReturnStatement(std::move(returnFunctionCall), returnStatementNode.GetSpan()));
            }
            else
            {
                if (returnType)
                {
                    throw cmajor::symbols::Exception("void function cannot return a value",
                        returnStatementNode.GetFullSpan(), currentFunction->GetFunctionSymbol()->GetFullSpan());
                }
                else
                {
                    throw cmajor::symbols::Exception("constructor or assignment function cannot return a value", 
                        returnStatementNode.GetFullSpan(), currentFunction->GetFunctionSymbol()->GetFullSpan());
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
            AddStatement(new BoundReturnStatement(std::move(returnFunctionCall), returnStatementNode.GetSpan()));
        }
        else
        {
            throw cmajor::symbols::Exception("nonvoid function must return a value", returnStatementNode.GetFullSpan(),
                currentFunction->GetFunctionSymbol()->GetFullSpan());
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
    if (!TypesEqual(symbolTable.GetTypeByName(U"bool"), condition->GetType()->PlainType()))
    {
        throw cmajor::symbols::Exception("condition of an if statement must be a Boolean expression", ifStatementNode.Condition()->GetFullSpan());
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
        AddReleaseExceptionStatement(&ifStatementNode);
    }
    AddStatement(new BoundIfStatement(ifStatementNode.GetSpan(), std::move(condition), std::unique_ptr<BoundStatement>(thenS), std::unique_ptr<BoundStatement>(elseS)));
}

void StatementBinder::Visit(cmajor::ast::WhileStatementNode& whileStatementNode)
{
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> condition = BindExpression(whileStatementNode.Condition(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && condition->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (!TypesEqual(symbolTable.GetTypeByName(U"bool"), condition->GetType()->PlainType()))
    {
        throw cmajor::symbols::Exception("condition of a while statement must be a Boolean expression", whileStatementNode.Condition()->GetFullSpan());
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
        AddReleaseExceptionStatement(&whileStatementNode);
    }
    AddStatement(new BoundWhileStatement(whileStatementNode.GetSpan(), std::move(condition), std::unique_ptr<BoundStatement>(stmt)));
}

void StatementBinder::Visit(cmajor::ast::DoStatementNode& doStatementNode)
{
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> condition = BindExpression(doStatementNode.Condition(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && condition->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (!TypesEqual(symbolTable.GetTypeByName(U"bool"), condition->GetType()->PlainType()))
    {
        throw cmajor::symbols::Exception("condition of a do statement must be a Boolean expression", doStatementNode.Condition()->GetFullSpan());
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
        AddReleaseExceptionStatement(&doStatementNode);
    }
    AddStatement(new BoundDoStatement(doStatementNode.GetSpan(), std::unique_ptr<BoundStatement>(stmt), std::move(condition)));
}

void StatementBinder::Visit(cmajor::ast::ForStatementNode& forStatementNode)
{
    cmajor::symbols::ContainerScope* prevContainerScope = containerScope;
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&forStatementNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::declarationBlock, "declaration block expected");
    cmajor::symbols::DeclarationBlock* declarationBlock = static_cast<cmajor::symbols::DeclarationBlock*>(symbol);
    containerScope = declarationBlock->GetContainerScope();
    forStatementNode.InitS()->Accept(*this);
    std::unique_ptr<BoundStatement> initS(statement.release());
    std::unique_ptr<BoundExpression> condition;
    if (forStatementNode.Condition())
    {
        condition = BindExpression(forStatementNode.Condition(), boundCompileUnit, currentFunction, containerScope, this);
    }
    else
    {
        cmajor::ast::BooleanLiteralNode trueNode(forStatementNode.GetSpan(), true);
        condition = BindExpression(&trueNode, boundCompileUnit, currentFunction, containerScope, this);
    }
    bool exceptionCapture = false;
    if (insideCatch && condition->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (!TypesEqual(symbolTable.GetTypeByName(U"bool"), condition->GetType()->PlainType()))
    {
        throw cmajor::symbols::Exception("condition of a for statement must be a Boolean expression", forStatementNode.Condition()->GetFullSpan());
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
    forStatementNode.LoopS()->Accept(*this);
    std::unique_ptr<BoundStatement> loopS(statement.release());
    loopS->SetForLoopStatementNode();
    forStatementNode.ActionS()->Accept(*this);
    std::unique_ptr<BoundStatement> actionS(statement.release());
    if (s)
    {
        AddStatement(s.release());
    }
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(&forStatementNode);
    }
    AddStatement(new BoundForStatement(forStatementNode.GetSpan(), 
        std::unique_ptr<BoundStatement>(initS.release()), 
        std::move(condition), 
        std::unique_ptr<BoundStatement>(loopS.release()), 
        std::unique_ptr<BoundStatement>(actionS.release())));
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
        throw cmajor::symbols::Exception("break statement must be enclosed in a while, do, for or switch statement", breakStatementNode.GetFullSpan());
    }
    AddStatement(new BoundBreakStatement(breakStatementNode.GetSpan()));
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
        throw cmajor::symbols::Exception("continue statement must be enclosed in a while, do or for statement", continueStatementNode.GetFullSpan());
    }
    AddStatement(new BoundContinueStatement(continueStatementNode.GetSpan()));
}

void StatementBinder::Visit(cmajor::ast::GotoStatementNode& gotoStatementNode)
{
    currentFunction->SetHasGotos();
    boundCompileUnit.SetHasGotos();
    AddStatement(new BoundGotoStatement(gotoStatementNode.GetSpan(), gotoStatementNode.Target()));
}

void StatementBinder::Visit(cmajor::ast::ConstructionStatementNode& constructionStatementNode)
{
    cmajor::symbols::Symbol* symbol = boundCompileUnit.GetSymbolTable().GetSymbol(&constructionStatementNode);
    Assert(symbol->GetSymbolType() == cmajor::symbols::SymbolType::localVariableSymbol, "local variable symbol expected");
    cmajor::symbols::LocalVariableSymbol* localVariableSymbol = static_cast<cmajor::symbols::LocalVariableSymbol*>(symbol);
    bool constructDelegateOrClassDelegateType =
        localVariableSymbol->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol ||
        localVariableSymbol->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol;
    if (localVariableSymbol->GetType()->BaseType()->IsAutoType())
    {
        int n = constructionStatementNode.Arguments().Count();
        if (n != 1)
        {
            throw cmajor::symbols::Exception("'auto' needs an initializer", constructionStatementNode.GetFullSpan());
        }
        cmajor::ast::Node* argumentNode = constructionStatementNode.Arguments()[0];
        std::unique_ptr<BoundExpression> argument = BindExpression(argumentNode, boundCompileUnit, currentFunction, containerScope, this, false, constructDelegateOrClassDelegateType);
        cmajor::symbols::TypeSymbol* initializerType = argument->GetType();
        cmajor::symbols::TypeDerivationRec derivations = localVariableSymbol->GetType()->DerivationRec();
        if (derivations.IsEmpty())
        {
            localVariableSymbol->SetType(initializerType);
        }
        else
        {
            initializerType = boundCompileUnit.GetSymbolTable().MakeDerivedType(initializerType->BaseType(), derivations);
            localVariableSymbol->SetType(initializerType);
        }
    }
    std::vector<std::unique_ptr<BoundExpression>> arguments;
    BoundExpression* localVariable = new BoundLocalVariable(constructionStatementNode.GetSpan(), localVariableSymbol);
    arguments.push_back(std::unique_ptr<BoundExpression>(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(localVariable),
        localVariable->GetType()->AddPointer())));
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
        &constructionStatementNode);
    cmajor::symbols::FunctionSymbol* functionSymbol = constructorCall->GetFunctionSymbol();
    CheckAccess(currentFunction->GetFunctionSymbol(), functionSymbol);
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(&constructionStatementNode);
    }
    if (functionSymbol->Parent()->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(functionSymbol->Parent());
        if (classType->Destructor() && classType->Destructor()->IsGeneratedFunction() && !cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::info))
        {
            cmajor::symbols::DestructorSymbol* destructorSymbol = classType->Destructor();
            if (!boundCompileUnit.IsGeneratedDestructorInstantiated(destructorSymbol))
            {
                boundCompileUnit.SetGeneratedDestructorInstantiated(destructorSymbol);
                std::unique_ptr<BoundClass> boundClass(new BoundClass(classType));
                if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
                {
                    cmajor::symbols::DestructorSymbol* copy = static_cast<cmajor::symbols::DestructorSymbol*>(destructorSymbol->Copy());
                    boundCompileUnit.GetSymbolTable().AddFunctionSymbol(std::unique_ptr<cmajor::symbols::FunctionSymbol>(copy));
                    cmajor::ast::CompileUnitNode* compileUnitNode = boundCompileUnit.GetCompileUnitNode();
                    if (compileUnitNode)
                    {
                        copy->SetCompileUnitId(compileUnitNode->Id());
                        copy->ComputeMangledName();
                    }
                    destructorSymbol->SetInstantiatedName(copy->MangledName());
                    destructorSymbol = copy;
                }
                GenerateDestructorImplementation(boundClass.get(), destructorSymbol, boundCompileUnit, containerScope, currentFunction, &constructionStatementNode);
                boundCompileUnit.AddBoundNode(std::move(boundClass));
            }
        }
    }
    BoundConstructionStatement* boundConstructionStatement = new BoundConstructionStatement(std::move(constructorCall), constructionStatementNode.GetSpan());
    boundConstructionStatement->SetLocalVariable(localVariableSymbol);
    AddStatement(boundConstructionStatement);
}

void StatementBinder::Visit(cmajor::ast::DeleteStatementNode& deleteStatementNode)
{
    soul::ast::Span span;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        span = deleteStatementNode.GetSpan();
    }
    bool exceptionCapture = false;
    std::unique_ptr<BoundExpression> ptr = BindExpression(deleteStatementNode.Expression(), boundCompileUnit, currentFunction, containerScope, this);
    if (insideCatch && ptr->ContainsExceptionCapture())
    {
        exceptionCapture = true;
    }
    if (false) // cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm
    {
        if (cmajor::symbols::GetConfig() == "debug")
        {
            std::vector<FunctionScopeLookup> lookups;
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, containerScope));
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::fileScopes, nullptr));
            std::vector<std::unique_ptr<BoundExpression>> arguments;
            arguments.push_back(std::move(std::unique_ptr<BoundExpression>(ptr->Clone())));
            std::unique_ptr<BoundFunctionCall> disposeCall = ResolveOverload(U"RtDispose", containerScope, lookups, arguments, boundCompileUnit, currentFunction, &deleteStatementNode);
            CheckAccess(currentFunction->GetFunctionSymbol(), disposeCall->GetFunctionSymbol());
            AddStatement(new BoundExpressionStatement(std::move(disposeCall), span));
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
        std::unique_ptr<BoundFunctionCall> destructorCall = ResolveOverload(U"@destructor", containerScope, lookups, arguments, boundCompileUnit, currentFunction, &deleteStatementNode);
        CheckAccess(currentFunction->GetFunctionSymbol(), destructorCall->GetFunctionSymbol());
        if (destructorCall->GetFunctionSymbol()->IsVirtualAbstractOrOverride())
        {
            destructorCall->SetFlag(BoundExpressionFlags::virtualCall);
        }
        AddStatement(new BoundExpressionStatement(std::move(destructorCall), span));
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
    if (false) // cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm
    {
        memFreeFunctionName = U"RtMemFree";
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        memFreeFunctionName = U"MemFree";
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp || 
        cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        memFreeFunctionName = U"RtmMemFree";
    }
    std::unique_ptr<BoundFunctionCall> memFreeCall = ResolveOverload(memFreeFunctionName, containerScope, lookups, arguments, boundCompileUnit, currentFunction, &deleteStatementNode);
    CheckAccess(currentFunction->GetFunctionSymbol(), memFreeCall->GetFunctionSymbol());
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(&deleteStatementNode);
    }
    AddStatement(new BoundExpressionStatement(std::move(memFreeCall), deleteStatementNode.GetSpan()));
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
        throw cmajor::symbols::Exception("destroy statement needs pointer type operand", destroyStatementNode.GetFullSpan());
    }
    cmajor::symbols::TypeSymbol* pointeeType = ptr->GetType()->RemovePointer();
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
            &destroyStatementNode);
        CheckAccess(currentFunction->GetFunctionSymbol(), destructorCall->GetFunctionSymbol());
        if (destructorCall->GetFunctionSymbol()->IsVirtualAbstractOrOverride())
        {
            destructorCall->SetFlag(BoundExpressionFlags::virtualCall);
        }
        if (exceptionCapture)
        {
            AddReleaseExceptionStatement(&destroyStatementNode);
        }
        AddStatement(new BoundExpressionStatement(std::move(destructorCall), destroyStatementNode.GetSpan()));
    }
    else
    {
        AddStatement(new BoundEmptyStatement(destroyStatementNode.GetSpan()));
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
    cmajor::symbols::TypeSymbol* targetPlainType = target->GetType()->PlainType();
    if ((targetPlainType->IsClassTypeSymbol() || targetPlainType->IsArrayType()) && target->GetType()->IsReferenceType())
    {
        cmajor::symbols::TypeSymbol* type = target->GetType()->RemoveReference()->AddPointer();
        target.reset(new BoundReferenceToPointerExpression(std::unique_ptr<BoundExpression>(target.release()), type));
    }
    else if (targetPlainType->IsPointerType() && target->GetType()->IsReferenceType())
    {
        cmajor::symbols::TypeSymbol* derefType = target->GetType()->RemoveReference();
        cmajor::symbols::TypeSymbol* addrOfType = derefType->AddPointer();
        target.reset(new BoundAddressOfExpression(std::unique_ptr<BoundExpression>(
            new BoundDereferenceExpression(std::unique_ptr<BoundExpression>(target.release()), derefType)), addrOfType));
    }
    else
    {
        target.reset(new BoundAddressOfExpression(std::move(target), target->GetType()->AddPointer()));
    }
    cmajor::symbols::TypeSymbol* targetType = target->GetType()->BaseType();
    bool assignDelegateOrClassDelegateType = targetType->GetSymbolType() == cmajor::symbols::SymbolType::delegateTypeSymbol || 
        targetType->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol;
    std::unique_ptr<BoundExpression> source = BindExpression(assignmentStatementNode.SourceExpr(), boundCompileUnit, currentFunction, containerScope, this, false, 
        assignDelegateOrClassDelegateType);
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
        &assignmentStatementNode);
    CheckAccess(currentFunction->GetFunctionSymbol(), assignmentCall->GetFunctionSymbol());
    currentFunction->MoveTemporaryDestructorCallsTo(*assignmentCall);
    if (exceptionCapture)
    {
        AddReleaseExceptionStatement(&assignmentStatementNode);
    }
    AddStatement(new BoundAssignmentStatement(std::move(assignmentCall), assignmentStatementNode.GetSpan()));
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
        AddReleaseExceptionStatement(&expressionStatementNode);
    }
    AddStatement(new BoundExpressionStatement(std::move(expression), expressionStatementNode.GetSpan()));
}

void StatementBinder::Visit(cmajor::ast::EmptyStatementNode& emptyStatementNode)
{
    AddStatement(new BoundEmptyStatement(emptyStatementNode.GetSpan()));
}

void StatementBinder::Visit(cmajor::ast::RangeForStatementNode& rangeForStatementNode)
{
    soul::ast::Span span;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        span = rangeForStatementNode.GetSpan();
    }
    if (rangeForStatementNode.Action()->GetNodeType() == cmajor::ast::NodeType::compoundStatementNode)
    {
        cmajor::ast::CompoundStatementNode* action = static_cast<cmajor::ast::CompoundStatementNode*>(rangeForStatementNode.Action());
    }
    soul::ast::Span initSpan = rangeForStatementNode.TypeExpr()->GetSpan();
    soul::ast::Span containerSpan = rangeForStatementNode.Container()->GetSpan();
    std::unique_ptr<BoundExpression> container = BindExpression(rangeForStatementNode.Container(), boundCompileUnit, currentFunction, containerScope, this);
    cmajor::symbols::TypeSymbol* plainContainerType = container->GetType()->PlainType();
    std::u32string plainContainerTypeFullName = plainContainerType->FullName();
    cmajor::parser::context::Context parsingContext;
    parsingContext.SetModuleId(module->Id());
    std::u32string content = plainContainerTypeFullName + U"\n";
    auto cmajorLexer = cmajor::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), "");
    // cmajorLexer.SetSeparatorChar('\n'); TODO
    using LexerType = decltype(cmajorLexer);
    std::unique_ptr<cmajor::ast::Node> containerTypeNode(cmajor::type::expr::parser::TypeExprParser<LexerType>::Parse(cmajorLexer, &parsingContext));
    std::unique_ptr<cmajor::ast::IdentifierNode> iteratorTypeNode = nullptr;
    if (container->GetType()->IsConstType())
    {
        iteratorTypeNode.reset(new cmajor::ast::IdentifierNode(span, U"ConstIterator"));
    }
    else
    {
        iteratorTypeNode.reset(new cmajor::ast::IdentifierNode(span, U"Iterator"));
    }
    cmajor::ast::CloneContext cloneContext;
    std::unique_ptr<cmajor::ast::CompoundStatementNode> compoundStatementNode(new cmajor::ast::CompoundStatementNode(span));
    if (rangeForStatementNode.Action()->GetNodeType() == cmajor::ast::NodeType::compoundStatementNode)
    {
        cmajor::ast::CompoundStatementNode* action = static_cast<cmajor::ast::CompoundStatementNode*>(rangeForStatementNode.Action());
    }
    compoundStatementNode->SetParent(rangeForStatementNode.Parent());
    cmajor::ast::ConstructionStatementNode* constructEndIteratorStatement = new cmajor::ast::ConstructionStatementNode(span,
        new cmajor::ast::DotNode(span, containerTypeNode->Clone(cloneContext), static_cast<cmajor::ast::IdentifierNode*>(iteratorTypeNode->Clone(cloneContext))), 
        new cmajor::ast::IdentifierNode(span, U"@end"));
    if (container->GetType()->IsConstType())
    {
        constructEndIteratorStatement->AddArgument(new cmajor::ast::InvokeNode(span, new cmajor::ast::DotNode(span, rangeForStatementNode.Container()->Clone(cloneContext),
            new cmajor::ast::IdentifierNode(span, U"CEnd"))));
    }
    else
    {
        constructEndIteratorStatement->AddArgument(new cmajor::ast::InvokeNode(span, new cmajor::ast::DotNode(span, rangeForStatementNode.Container()->Clone(cloneContext),
            new cmajor::ast::IdentifierNode(span, U"End"))));
    }
    compoundStatementNode->AddStatement(constructEndIteratorStatement);
    cmajor::ast::ConstructionStatementNode* constructIteratorStatement = new cmajor::ast::ConstructionStatementNode(initSpan,
        new cmajor::ast::DotNode(span, containerTypeNode->Clone(cloneContext), static_cast<cmajor::ast::IdentifierNode*>(iteratorTypeNode->Clone(cloneContext))), 
        new cmajor::ast::IdentifierNode(span, U"@it"));
    if (container->GetType()->IsConstType())
    {
        constructIteratorStatement->AddArgument(new cmajor::ast::InvokeNode(span, new cmajor::ast::DotNode(span, rangeForStatementNode.Container()->Clone(cloneContext),
            new cmajor::ast::IdentifierNode(span, U"CBegin"))));
    }
    else
    {
        constructIteratorStatement->AddArgument(new cmajor::ast::InvokeNode(span, new cmajor::ast::DotNode(span, rangeForStatementNode.Container()->Clone(cloneContext),
            new cmajor::ast::IdentifierNode(span, U"Begin"))));
    }
    cmajor::ast::Node* itNotEndCond = new cmajor::ast::NotEqualNode(span, new cmajor::ast::IdentifierNode(span, U"@it"), new cmajor::ast::IdentifierNode(span, U"@end"));
    cmajor::ast::StatementNode* incrementItStatement = new cmajor::ast::ExpressionStatementNode(containerSpan, new cmajor::ast::PrefixIncrementNode(containerSpan, 
        new cmajor::ast::IdentifierNode(containerSpan, U"@it")));
    cmajor::ast::CompoundStatementNode* actionStatement = new cmajor::ast::CompoundStatementNode(span);
    cmajor::ast::ConstructionStatementNode* constructLoopVarStatement = new cmajor::ast::ConstructionStatementNode(span,
        rangeForStatementNode.TypeExpr()->Clone(cloneContext), static_cast<cmajor::ast::IdentifierNode*>(rangeForStatementNode.Id()->Clone(cloneContext)));
    constructLoopVarStatement->AddArgument(new cmajor::ast::DerefNode(span, new cmajor::ast::IdentifierNode(span, U"@it")));
    actionStatement->AddStatement(constructLoopVarStatement);
    actionStatement->AddStatement(static_cast<cmajor::ast::StatementNode*>(rangeForStatementNode.Action()->Clone(cloneContext)));
    cmajor::ast::ForStatementNode* forStatement = new cmajor::ast::ForStatementNode(span, constructIteratorStatement, itNotEndCond, incrementItStatement, actionStatement);
    compoundStatementNode->AddStatement(forStatement);

    std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().GetLock());
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
        std::unique_ptr<BoundSwitchStatement> boundSwitchStatement(new BoundSwitchStatement(switchStatementNode.GetSpan(), std::move(condition)));
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
                throw cmajor::symbols::Exception("case not found", switchStatementNode.GetFullSpan(), gotoCaseStatement->GetFullSpan());
            }
        }
        if (!gotoDefaultStatements.empty() && !switchStatementNode.Default())
        {
            throw cmajor::symbols::Exception("switch does not have a default statement", switchStatementNode.GetFullSpan(), gotoDefaultStatements.front()->GetFullSpan());
        }
        currentGotoCaseStatements = prevGotoCaseStatements;
        currentGotoDefaultStatements = prevGotoDefaultStatements;
        currentCaseValueMap = prevCaseValueMap;
        if (exceptionCapture)
        {
            AddReleaseExceptionStatement(&switchStatementNode);
        }
        AddStatement(boundSwitchStatement.release());
        switchConditionType = prevSwitchConditionType;
    }
    else
    {
        throw cmajor::symbols::Exception("switch statement condition must be of integer, character, enumerated or Boolean type", switchStatementNode.Condition()->GetFullSpan());
    }
}

void StatementBinder::Visit(cmajor::ast::CaseStatementNode& caseStatementNode)
{
    std::unique_ptr<BoundCaseStatement> boundCaseStatement(new BoundCaseStatement(caseStatementNode.GetSpan()));
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
        throw cmajor::symbols::Exception("case must end in break, continue, return, throw, goto, goto case or goto default statement", caseStatementNode.GetFullSpan());
    }
    int ne = caseStatementNode.CaseExprs().Count();
    for (int i = 0; i < ne; ++i)
    {
        cmajor::ast::Node* caseExprNode = caseStatementNode.CaseExprs()[i];
        std::unique_ptr<cmajor::symbols::Value> caseValue = Evaluate(caseExprNode, switchConditionType, containerScope, boundCompileUnit, false, currentFunction);
        cmajor::symbols::IntegralValue integralCaseValue(caseValue.get());
        Assert(currentCaseValueMap, "current case value map not set"); 
        auto it = currentCaseValueMap->find(integralCaseValue);
        if (it != currentCaseValueMap->cend())
        {
            throw cmajor::symbols::Exception("case value already used", caseStatementNode.GetFullSpan(), caseExprNode->GetFullSpan());
        }
        (*currentCaseValueMap)[integralCaseValue] = &caseStatementNode;
        boundCaseStatement->AddCaseValue(std::move(caseValue));
    }
    AddStatement(boundCaseStatement.release());
}

void StatementBinder::Visit(cmajor::ast::DefaultStatementNode& defaultStatementNode)
{
    std::unique_ptr<BoundDefaultStatement> boundDefaultStatement(new BoundDefaultStatement(defaultStatementNode.GetSpan()));
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
        throw cmajor::symbols::Exception("default must end in break, continue, return, throw, goto, or goto case statement", defaultStatementNode.GetFullSpan());
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
        throw cmajor::symbols::Exception("goto case statement must be enclosed in a case or default statement", gotoCaseStatementNode.GetFullSpan());
    }
    cmajor::ast::Node* caseExprNode = gotoCaseStatementNode.CaseExpr();
    std::unique_ptr<cmajor::symbols::Value> caseValue = Evaluate(caseExprNode, switchConditionType, containerScope, boundCompileUnit, false, currentFunction);
    cmajor::symbols::Value* caseValuePtr = caseValue.get();
    BoundGotoCaseStatement* boundGotoCaseStatement = new BoundGotoCaseStatement(gotoCaseStatementNode.GetSpan(), std::move(caseValue));
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
        throw cmajor::symbols::Exception("goto default statement must be enclosed in a case statement", gotoDefaultStatementNode.GetFullSpan());
    }
    BoundGotoDefaultStatement* boundGotoDefaultStatement = new BoundGotoDefaultStatement(gotoDefaultStatementNode.GetSpan());
    Assert(currentGotoDefaultStatements, "current goto default statement vector not set"); 
    currentGotoDefaultStatements->push_back(boundGotoDefaultStatement);
    AddStatement(boundGotoDefaultStatement);
}

void StatementBinder::Visit(cmajor::ast::ThrowStatementNode& throwStatementNode)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
    {
        throw cmajor::symbols::Exception("MASM backend does not support exceptions", throwStatementNode.GetFullSpan());
    }
    bool prevCompilingThrow = compilingThrow;
    compilingThrow = true;
    if (currentFunction->GetFunctionSymbol()->DontThrow() && !currentFunction->GetFunctionSymbol()->HasTry())
    {
        throw cmajor::symbols::Exception("a nothrow function cannot contain a throw statement unless it handles exceptions",
            throwStatementNode.GetFullSpan(), currentFunction->GetFunctionSymbol()->GetFullSpan());
    }
    soul::ast::Span span = throwStatementNode.GetSpan();
    cmajor::ast::Node* exceptionExprNode = throwStatementNode.Expression();
    if (exceptionExprNode)
    {
        std::unique_ptr<BoundExpression> boundExceptionExpr = BindExpression(exceptionExprNode, boundCompileUnit, currentFunction, containerScope, this);
        if (boundExceptionExpr->GetType()->PlainType()->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* exceptionClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(boundExceptionExpr->GetType()->PlainType());
            cmajor::ast::IdentifierNode systemExceptionNode(span, U"System.Exception");
            cmajor::symbols::TypeSymbol* systemExceptionType = ResolveType(&systemExceptionNode, boundCompileUnit, containerScope);
            Assert(systemExceptionType->IsClassTypeSymbol(), "System.Exception not of class type");
            cmajor::symbols::ClassTypeSymbol* systemExceptionClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(systemExceptionType);
            if (exceptionClassType == systemExceptionClassType || exceptionClassType->HasBaseClass(systemExceptionClassType))
            {
                cmajor::ast::NewNode* newNode = new cmajor::ast::NewNode(span, new cmajor::ast::IdentifierNode(span, exceptionClassType->FullName()));
                cmajor::ast::CloneContext cloneContext;
                newNode->AddArgument(throwStatementNode.Expression()->Clone(cloneContext));
                if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
                {
                    cmajor::ast::InvokeNode invokeNode(span, new cmajor::ast::IdentifierNode(span, U"RtmThrowException"));
                    invokeNode.AddArgument(newNode);
                    invokeNode.AddArgument(new cmajor::ast::UuidLiteralNode(span, exceptionClassType->TypeId()));
                    std::unique_ptr<BoundExpression> throwCallExpr = BindExpression(&invokeNode, boundCompileUnit, currentFunction, containerScope, this);
                    AddStatement(new BoundThrowStatement(span, std::move(throwCallExpr)));
                }
                else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
                {
                    cmajor::ast::InvokeNode invokeNode(span, new cmajor::ast::DotNode(span, new cmajor::ast::IdentifierNode(span, U"System"), 
                        new cmajor::ast::IdentifierNode(span, U"Throw")));
                    invokeNode.AddArgument(newNode);
                    std::unique_ptr<BoundExpression> throwCallExpr = BindExpression(&invokeNode, boundCompileUnit, currentFunction, containerScope, this);
                    AddStatement(new BoundThrowStatement(span, std::move(throwCallExpr)));
                }
            }
            else
            {
                throw cmajor::symbols::Exception("exception class must be derived from System.Exception class", throwStatementNode.GetFullSpan());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("exception not of class type", throwStatementNode.GetFullSpan());
        }
    }
    else
    {
        if (insideCatch)
        {
            cmajor::ast::InvokeNode invokeNode(span, new cmajor::ast::DotNode(span, new cmajor::ast::IdentifierNode(span, U"@exPtr"), 
                new cmajor::ast::IdentifierNode(span, U"Release")));
            std::unique_ptr<BoundExpression> releaseCall = BindExpression(&invokeNode, boundCompileUnit, currentFunction, containerScope, this);
            AddStatement(new BoundRethrowStatement(span, std::move(releaseCall)));
        }
        else
        {
            throw cmajor::symbols::Exception("rethrow must occur inside a catch clause", throwStatementNode.GetFullSpan());
        }
    }
    compilingThrow = prevCompilingThrow;
}

void StatementBinder::Visit(cmajor::ast::TryStatementNode& tryStatementNode)
{
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
    {
        throw cmajor::symbols::Exception("MASM backend do not support exceptions", tryStatementNode.GetFullSpan());
    }
    BoundTryStatement* boundTryStatement = new BoundTryStatement(tryStatementNode.GetSpan());
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
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
    {
        throw cmajor::symbols::Exception("MASM backend do not support exceptions", catchNode.GetFullSpan());
    }
    bool prevInsideCatch = insideCatch;
    insideCatch = true;
    soul::ast::Span span;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        span = catchNode.GetSpan();
    }
    std::unique_ptr<BoundCatchStatement> boundCatchStatement(new BoundCatchStatement(span));
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
    cmajor::ast::CompoundStatementNode handlerBlock(span);
    handlerBlock.SetParent(catchNode.Parent());
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        cmajor::ast::ConstructionStatementNode* getExceptionAddr = new cmajor::ast::ConstructionStatementNode(span, new cmajor::ast::PointerNode(span, 
            new cmajor::ast::IdentifierNode(span, U"void")),
            new cmajor::ast::IdentifierNode(span, U"@exceptionAddr"));
        getExceptionAddr->AddArgument(new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"RtmGetException")));
        handlerBlock.AddStatement(getExceptionAddr);
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        cmajor::ast::ConstructionStatementNode* getExceptionAddr = new cmajor::ast::ConstructionStatementNode(span, new cmajor::ast::PointerNode(span, 
            new cmajor::ast::IdentifierNode(span, U"void")),
            new cmajor::ast::IdentifierNode(span, U"@exceptionAddr"));
        getExceptionAddr->AddArgument(new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"do_catch")));
        handlerBlock.AddStatement(getExceptionAddr);
    }
    cmajor::ast::PointerNode exceptionPtrTypeNode(span, new cmajor::ast::IdentifierNode(span, caughtType->BaseType()->FullName()));
    cmajor::ast::CloneContext cloneContext;
    cmajor::ast::ConstructionStatementNode* constructExceptionPtr = new cmajor::ast::ConstructionStatementNode(span, exceptionPtrTypeNode.Clone(cloneContext),
        new cmajor::ast::IdentifierNode(span, U"@exceptionPtr"));
    constructExceptionPtr->AddArgument(new cmajor::ast::CastNode(span, exceptionPtrTypeNode.Clone(cloneContext), new cmajor::ast::IdentifierNode(span, U"@exceptionAddr")));
    handlerBlock.AddStatement(constructExceptionPtr);
    cmajor::ast::TemplateIdNode* uniquePtrNode = new cmajor::ast::TemplateIdNode(span, new cmajor::ast::IdentifierNode(span, U"UniquePtr"));
    uniquePtrNode->AddTemplateArgument(new cmajor::ast::IdentifierNode(span, caughtType->BaseType()->FullName()));
    cmajor::ast::ConstructionStatementNode* constructUniquePtrException = new cmajor::ast::ConstructionStatementNode(span, uniquePtrNode, 
        new cmajor::ast::IdentifierNode(span, U"@exPtr"));
    constructUniquePtrException->AddArgument(new cmajor::ast::IdentifierNode(span, U"@exceptionPtr"));
    handlerBlock.AddStatement(constructUniquePtrException);
    if (catchVar)
    {
        cmajor::ast::ConstructionStatementNode* setExceptionVar = new cmajor::ast::ConstructionStatementNode(span, catchNode.TypeExpr()->Clone(cloneContext), 
            static_cast<cmajor::ast::IdentifierNode*>(catchNode.Id()->Clone(cloneContext)));
        setExceptionVar->AddArgument(new cmajor::ast::DerefNode(span, new cmajor::ast::IdentifierNode(span, U"@exPtr")));
        handlerBlock.AddStatement(setExceptionVar);
    }
    handlerBlock.AddStatement(static_cast<cmajor::ast::StatementNode*>(catchNode.CatchBlock()->Clone(cloneContext)));
    std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().GetLock());
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
        int32_t assertionLineNumber = cmajor::symbols::GetLineNumber(assertStatementNode.GetFullSpan());
        int32_t assertionIndex = cmajor::symbols::GetNextUnitTestAssertionNumber();
        cmajor::symbols::AddAssertionLineNumber(assertionLineNumber);
        std::u32string setAssertionResultFunctionName = U"RtSetUnitTestAssertionResult";
        if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp ||
            cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
        {
            setAssertionResultFunctionName = U"RtmSetUnitTestAssertionResult";
        }
        cmajor::ast::InvokeNode* invokeSetUnitTestAssertionResult = new cmajor::ast::InvokeNode(assertStatementNode.GetSpan(),
            new cmajor::ast::IdentifierNode(assertStatementNode.GetSpan(), setAssertionResultFunctionName));
        invokeSetUnitTestAssertionResult->AddArgument(new cmajor::ast::IntLiteralNode(assertStatementNode.GetSpan(), assertionIndex));
        cmajor::ast::CloneContext cloneContext;
        invokeSetUnitTestAssertionResult->AddArgument(assertStatementNode.AssertExpr()->Clone(cloneContext));
        invokeSetUnitTestAssertionResult->AddArgument(new cmajor::ast::IntLiteralNode(assertStatementNode.GetSpan(), assertionLineNumber));
        cmajor::ast::ExpressionStatementNode setUnitTestAssertionResult(assertStatementNode.GetSpan(), invokeSetUnitTestAssertionResult);
        std::lock_guard<std::recursive_mutex> lock(boundCompileUnit.GetModule().GetLock());
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
            AddStatement(new BoundEmptyStatement(assertStatementNode.GetSpan()));
        }
        else
        {
            soul::ast::FullSpan fullSpan = assertStatementNode.GetFullSpan();
            int lineNumber = cmajor::symbols::GetLineNumber(fullSpan);
            std::string sourceFilePath = cmajor::symbols::GetSourceFilePath(fullSpan.fileIndex, fullSpan.moduleId);
            std::vector<FunctionScopeLookup> lookups;
            lookups.push_back(FunctionScopeLookup(cmajor::symbols::ScopeLookup::this_and_base_and_parent, symbolTable.GlobalNs().GetContainerScope()));
            std::vector<std::unique_ptr<BoundExpression>> arguments;
            cmajor::symbols::TypeSymbol* constCharPtrType = symbolTable.GetTypeByName(U"char")->AddConst()->AddPointer();
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(
                new cmajor::symbols::StringValue(assertStatementNode.GetSpan(),
                boundCompileUnit.Install(assertStatementNode.AssertExpr()->ToString()), assertStatementNode.AssertExpr()->ToString())), constCharPtrType)));
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(
                new cmajor::symbols::StringValue(assertStatementNode.GetSpan(),
                boundCompileUnit.Install(util::ToUtf8(currentFunction->GetFunctionSymbol()->FullName())), 
                    util::ToUtf8(currentFunction->GetFunctionSymbol()->FullName()))), constCharPtrType)));
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(
                new cmajor::symbols::StringValue(assertStatementNode.GetSpan(),
                boundCompileUnit.Install(sourceFilePath), sourceFilePath)), constCharPtrType)));
            arguments.push_back(std::unique_ptr<BoundExpression>(new BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(
                new cmajor::symbols::IntValue(assertStatementNode.GetSpan(),
                lineNumber)), symbolTable.GetTypeByName(U"int"))));
            std::unique_ptr<BoundExpression> assertExpression = BindExpression(assertStatementNode.AssertExpr(), boundCompileUnit, currentFunction, containerScope, this);
            const char32_t* failAssertionFunctionName = U"";
            if (false) // cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm
            {
                failAssertionFunctionName = U"RtFailAssertion";
            }
            else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
            {
                failAssertionFunctionName = U"System.FailAssertion";
            }
            else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp ||
                cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
            {
                failAssertionFunctionName = U"RtmFailAssertion";
            }
            std::unique_ptr<BoundStatement> emptyStatement(new BoundEmptyStatement(assertStatementNode.GetSpan()));
            emptyStatement->SetIgnoreNode();
            std::unique_ptr<BoundStatement> failAssertionStatement(
                new BoundExpressionStatement(ResolveOverload(failAssertionFunctionName, containerScope, lookups, arguments,
                    boundCompileUnit, currentFunction, &assertStatementNode), assertStatementNode.GetSpan()));
            failAssertionStatement->SetIgnoreNode();
            std::unique_ptr<BoundStatement> ifStatement(new BoundIfStatement(assertStatementNode.GetSpan(), std::move(assertExpression),
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
            AddStatement(new BoundEmptyStatement(conditionalCompilationStatementNode.GetSpan()));
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
                    AddStatement(new BoundEmptyStatement(conditionalCompilationStatementNode.GetSpan()));
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
                    AddStatement(new BoundEmptyStatement(conditionalCompilationStatementNode.GetSpan()));
                }
            }
            else
            {
                AddStatement(new BoundEmptyStatement(conditionalCompilationStatementNode.GetSpan()));
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

void StatementBinder::AddReleaseExceptionStatement(cmajor::ast::Node* node)
{
    if (insideCatch && !compilingReleaseExceptionStatement)
    {
        compilingReleaseExceptionStatement = true;
        cmajor::ast::InvokeNode* invokeNode(new cmajor::ast::InvokeNode(node->GetSpan(), new cmajor::ast::DotNode(node->GetSpan(), 
            new cmajor::ast::IdentifierNode(node->GetSpan(), U"@exPtr"), new cmajor::ast::IdentifierNode(node->GetSpan(), U"Release"))));
        cmajor::ast::ExpressionStatementNode releaseExceptionStatement(invokeNode->GetSpan(), invokeNode);
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
            BoundSequenceStatement* sequenceStatement = new BoundSequenceStatement(boundStatement->GetSpan(), std::unique_ptr<BoundStatement>(boundStatement), std::move(statement));
            boundStatement = sequenceStatement;
        }
        else
        {
            BoundSequenceStatement* sequenceStatement = new BoundSequenceStatement(boundStatement->GetSpan(), std::move(statement), std::unique_ptr<BoundStatement>(boundStatement));
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
