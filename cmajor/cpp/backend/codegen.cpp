// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.cpp.backend.codegen;

import cmajor.cpp.backend.emitting.context;
import cmajor.cpp.ir;
import cmajor.debug;
import util;
import std.filesystem;

namespace cmajor::cpp::backend {

std::string gxxPath = "g++";

void SetGXXPath(const std::string& gxxPath_)
{
    gxxPath = gxxPath_;
}

struct NativeModule
{
    NativeModule(cmajor::ir::Emitter* emitter_, const std::string& moduleFilePath_) : emitter(emitter_)
    {
        module = emitter->CreateModule(moduleFilePath_);
        emitter->SetModule(module);
    }
    ~NativeModule()
    {
        emitter->DestroyModule(module);
    }
    cmajor::ir::Emitter* emitter;
    void* module;
};

CppCodeGenerator::CppCodeGenerator(cmajor::ir::Emitter* emitter_) :
    emitter(emitter_), symbolTable(nullptr), module(nullptr), compileUnit(nullptr), fileIndex(-1),
    nativeCompileUnit(nullptr), function(nullptr), entryBasicBlock(nullptr), lastInstructionWasRet(false), destructorCallGenerated(false), genJumpingBoolCode(false),
    trueBlock(nullptr), falseBlock(nullptr), breakTarget(nullptr), continueTarget(nullptr), sequenceSecond(nullptr), currentFunction(nullptr), currentBlock(nullptr),
    breakTargetBlock(nullptr), continueTargetBlock(nullptr), lastAlloca(nullptr), currentClass(nullptr), basicBlockOpen(false), defaultDest(nullptr), currentCaseMap(nullptr),
    generateLineNumbers(false), currentTryBlockId(-1), nextTryBlockId(0), currentTryNextBlock(nullptr), handlerBlock(nullptr), cleanupBlock(nullptr), inTryBlock(false),
    prevWasTerminator(false), numTriesInCurrentBlock(0), tryIndex(0), prevLineNumber(0), prevControlFlowGraphNodeId(-1), continueTargetNodeId(-1), loopNodeId(-1),
    emittingContext(nullptr), inSetLineOrEntryCode(false)
{
    emitter->SetEmittingDelegate(this);
}

void CppCodeGenerator::Compile(const std::string& intermediateCodeFile)
{
    std::string outputDirectory = util::GetFullPath(util::Path::GetDirectoryName(intermediateCodeFile));
    std::filesystem::create_directories(outputDirectory);
    std::string intermediateCompileCommand = gxxPath;
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::generateDebugInfo))
    {
        intermediateCompileCommand.append(" -g");
    }
    intermediateCompileCommand.append(" -c ").append(util::QuotedPath(intermediateCodeFile));
    intermediateCompileCommand.append(" -o ").append(util::QuotedPath(compileUnit->ObjectFilePath()));
    intermediateCompileCommand.append(" -O").append(std::to_string(emitter->EmittingContext()->OptimizationLevel()));
    std::string errors;
    try
    {
        util::Process::Redirections redirections = util::Process::Redirections::processStdOut | util::Process::Redirections::processStdErr;
        util::Process process(intermediateCompileCommand, redirections);
        errors = process.ReadToEnd(util::Process::StdHandle::stdErr);
        process.WaitForExit();
        int exitCode = process.ExitCode();
        if (exitCode != 0)
        {
            throw std::runtime_error("executing '" + intermediateCompileCommand + "' failed with exit code: " + std::to_string(exitCode));
        }
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("compiling intermediate code '" + intermediateCodeFile + "' failed: " + ex.what() + ":\nerrors:\n" + errors);
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    fileIndex = boundCompileUnit.FileIndex();
    std::string intermediateFilePath = util::Path::ChangeExtension(boundCompileUnit.LLFilePath(), ".cpp");
    NativeModule nativeModule(emitter, intermediateFilePath);
    compileUnitId = boundCompileUnit.Id();
    symbolTable = &boundCompileUnit.GetSymbolTable();
    module = &boundCompileUnit.GetModule();
    compileUnit = &boundCompileUnit;
    emitter->SetBoundCompileUnit(compileUnit);
    if (!module->IsCore())
    {
        module->AddCompileUnitId(compileUnitId);
    }
    cmajor::symbols::TypeSymbol* longType = module->GetSymbolTable().GetTypeByName(U"long");
    module->GetTypeIndex().AddType(longType->TypeId(), longType, *emitter);
    cmajor::symbols::TypeSymbol* boolType = module->GetSymbolTable().GetTypeByName(U"bool");
    module->GetTypeIndex().AddType(boolType->TypeId(), boolType, *emitter);
    emitter->SetCompileUnitId(compileUnitId);
    emitter->SetCurrentSourcePos(0, 0, 0);
    generateLineNumbers = false;
    nativeCompileUnit = static_cast<cmajor::cpp::ir::CompileUnit*>(nativeModule.module);
    nativeCompileUnit->SetId(compileUnitId);
    nativeCompileUnit->SetSourceFilePath(boundCompileUnit.SourceFilePath());
    cmajor::binder::ConstantArrayRepository& constantArrayRepository = boundCompileUnit.GetConstantArrayRepository();
    for (cmajor::symbols::ConstantSymbol* constantSymbol : constantArrayRepository.ConstantArrays())
    {
        constantSymbol->ArrayIrObject(*emitter, true);
    }
    cmajor::binder::ConstantStructureRepository& constantStructureRepository = boundCompileUnit.GetConstantStructureRepository();
    for (cmajor::symbols::ConstantSymbol* constantSymbol : constantStructureRepository.ConstantStructures())
    {
        constantSymbol->StructureIrObject(*emitter, true);
    }
    int n = boundCompileUnit.BoundNodes().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundNode* node = boundCompileUnit.BoundNodes()[i].get();
        node->Accept(*this);
    }
    nativeCompileUnit->Write();
    if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::disableCodeGen))
    {
        Compile(intermediateFilePath);
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundNamespace& boundNamespace)
{
    int n = boundNamespace.Members().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundNode* node = boundNamespace.Members()[i].get();
        node->Accept(*this);
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundClass& boundClass)
{
    classStack.push(currentClass);
    currentClass = &boundClass;
    int n = boundClass.Members().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundNode* boundNode = boundClass.Members()[i].get();
        boundNode->Accept(*this);
    }
    currentClass = classStack.top();
    classStack.pop();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundFunction& boundFunction)
{
    if (!boundFunction.Body()) return;
    currentFunction = &boundFunction;
    cmajor::symbols::FunctionSymbol* functionSymbol = boundFunction.GetFunctionSymbol();
    if (compileUnit->CodeGenerated(functionSymbol)) return;
    compileUnit->SetCodeGenerated(functionSymbol);
    void* functionType = functionSymbol->IrType(*emitter);
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    lastAlloca = nullptr;
    handlerBlock = nullptr;
    cleanupBlock = nullptr;
    labeledStatementMap.clear();
    cleanups.clear();
    cleanupMap.clear();
    tryIndexCleanupTryBlockMap.clear();
    tryIndexMap.clear();
    numTriesInCurrentBlock = 0;
    tryIndex = 0;
    prevLineNumber = 0;
    prevControlFlowGraphNodeId = -1;
    if (functionSymbol->HasSource() && cmajor::symbols::GetConfig() != "release" && module->Name() != U"System.Runtime" && module->Name() != U"System.Core")
    {
        generateLineNumbers = true;
        emitter->SetGenerateLocationInfo(true);
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundFunction.Body()->GetSpan(), fileIndex); 
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
        fullSpan = functionSymbol->GetFullSpan();
    }
    else
    {
        generateLineNumbers = false;
        emitter->SetCurrentSourcePos(0, 0, 0);
        fullSpan = soul::ast::FullSpan();
    }
    function = emitter->GetOrInsertFunction(util::ToUtf8(functionSymbol->MangledName()), functionType, functionSymbol->DontThrow());
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && functionSymbol->IsInline())
    {
        emitter->AddInlineFunctionAttribute(function);
        functionSymbol->SetLinkOnceOdrLinkage();
    }
    else if (functionSymbol->IsGeneratedFunction())
    {
        emitter->AddInlineFunctionAttribute(function);
        functionSymbol->SetLinkOnceOdrLinkage();
    }
    if (functionSymbol->HasLinkOnceOdrLinkage())
    {
        void* comdat = emitter->GetOrInsertAnyFunctionComdat(util::ToUtf8(functionSymbol->MangledName()), function);
        emitter->SetFunctionLinkageToLinkOnceODRLinkage(function);
    }
    util::uuid functionId;
    if (functionSymbol->HasSource())
    {
        functionId = functionSymbol->FunctionId();
        module->GetFunctionIndex().AddFunction(functionId, functionSymbol);
        if (functionSymbol == module->GetSymbolTable().MainFunctionSymbol())
        {
            module->GetFunctionIndex().SetMainFunctionId(functionId);
        }
    }
    emitter->SetFunction(function, fileIndex, functionSymbol->ModuleId(), functionId);
    emitter->SetFunctionName(util::ToUtf8(functionSymbol->FullName()));
    void* entryBlock = emitter->CreateBasicBlock("entry");
    if (functionSymbol->HasSource())
    {
        emitter->BeginScope();
    }
    entryBasicBlock = entryBlock;
    emitter->SetCurrentBasicBlock(entryBlock);
    emitter->PushParentBlock();
    if (functionSymbol->HasSource())
    {
        emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::beginBrace));
        prevControlFlowGraphNodeId = emitter->AddControlFlowGraphNode();
        emitter->CreateNop();
        emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::beginBrace));
    }
    emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::entryCode));
    if (currentClass && !currentClass->IsInlineFunctionContainer())
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = currentClass->GetClassTypeSymbol();
        if (!emitter->IsVmtObjectCreated(classTypeSymbol))
        {
            classTypeSymbol->VmtObject(*emitter, true);
        }
        if (!emitter->IsStaticObjectCreated(classTypeSymbol))
        {
            classTypeSymbol->StaticObject(*emitter, true);
        }
    }
    int np = functionSymbol->Parameters().size();
    for (int i = 0; i < np; ++i)
    {
        cmajor::symbols::ParameterSymbol* parameter = functionSymbol->Parameters()[i];
        void* allocaInst = emitter->CreateAlloca(parameter->GetType()->IrType(*emitter));
        emitter->SetIrObject(parameter, allocaInst);
        if (functionSymbol->HasSource())
        {
            const util::uuid& typeId = parameter->GetType()->TypeId();
            module->GetTypeIndex().AddType(typeId, parameter->GetType(), *emitter);
            emitter->AddLocalVariable(util::ToUtf8(parameter->Name()), typeId, parameter->IrObject(*emitter));
        }
        lastAlloca = allocaInst;
    }
    if (functionSymbol->ReturnParam())
    {
        cmajor::symbols::ParameterSymbol* parameter = functionSymbol->ReturnParam();
        void* allocaInst = emitter->CreateAlloca(parameter->GetType()->IrType(*emitter));
        emitter->SetIrObject(parameter, allocaInst);
        if (functionSymbol->HasSource())
        {
            const util::uuid& typeId = parameter->GetType()->TypeId();
            module->GetTypeIndex().AddType(typeId, parameter->GetType(), *emitter);
            emitter->AddLocalVariable(util::ToUtf8(parameter->Name()), typeId, parameter->IrObject(*emitter));
        }
        lastAlloca = allocaInst;
    }
    int nlv = functionSymbol->LocalVariables().size();
    for (int i = 0; i < nlv; ++i)
    {
        cmajor::symbols::LocalVariableSymbol* localVariable = functionSymbol->LocalVariables()[i];
        void* allocaInst = emitter->CreateAlloca(localVariable->GetType()->IrType(*emitter));
        emitter->SetIrObject(localVariable, allocaInst);
        lastAlloca = allocaInst;
    }
    for (int i = 0; i < np; ++i)
    {
        void* arg = emitter->GetFunctionArgument(function, i);
        cmajor::symbols::ParameterSymbol* parameter = functionSymbol->Parameters()[i];
        if (parameter->GetType()->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(parameter->GetType());
            cmajor::symbols::FunctionSymbol* copyConstructor = classType->CopyConstructor();
            if (!copyConstructor)
            {
                copyConstructor = compileUnit->GetCopyConstructorFor(classType->TypeId());
            }
            void* copyCtorType = copyConstructor->IrType(*emitter);
            void* callee = emitter->GetOrInsertFunction(util::ToUtf8(copyConstructor->MangledName()), copyCtorType, copyConstructor->DontThrow());
            std::vector<void*> args;
            args.push_back(parameter->IrObject(*emitter));
            args.push_back(arg);
            emitter->CreateCall(copyCtorType, callee, args);
        }
        else if (parameter->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::classDelegateTypeSymbol)
        {
            cmajor::symbols::ClassDelegateTypeSymbol* classDelegateType = static_cast<cmajor::symbols::ClassDelegateTypeSymbol*>(parameter->GetType());
            cmajor::symbols::FunctionSymbol* copyConstructor = classDelegateType->CopyConstructor();
            if (!copyConstructor)
            {
                throw std::runtime_error("internal error: class delegate type has no copy constructor");
            }
            std::vector<cmajor::ir::GenObject*> copyCtorArgs;
            cmajor::ir::NativeValue paramValue(parameter->IrObject(*emitter));
            copyCtorArgs.push_back(&paramValue);
            cmajor::ir::NativeValue argumentValue(arg);
            copyCtorArgs.push_back(&argumentValue);
            copyConstructor->GenerateCall(*emitter, copyCtorArgs, cmajor::ir::OperationFlags::none);
        }
        else if (parameter->GetType()->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
        {
            cmajor::symbols::InterfaceTypeSymbol* interfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(parameter->GetType());
            cmajor::symbols::FunctionSymbol* copyConstructor = interfaceType->CopyConstructor();
            if (!copyConstructor)
            {
                copyConstructor = compileUnit->GetCopyConstructorFor(interfaceType->TypeId());
            }
            std::vector<cmajor::ir::GenObject*> copyCtorArgs;
            cmajor::ir::NativeValue paramValue(parameter->IrObject(*emitter));
            paramValue.SetType(interfaceType->AddPointer());
            copyCtorArgs.push_back(&paramValue);
            cmajor::ir::NativeValue argumentValue(arg);
            argumentValue.SetType(interfaceType->AddPointer());
            copyCtorArgs.push_back(&argumentValue);
            copyConstructor->GenerateCall(*emitter, copyCtorArgs, cmajor::ir::OperationFlags::none);
        }
        else
        {
            emitter->CreateStore(arg, parameter->IrObject(*emitter));
        }
    }
    if (functionSymbol->ReturnParam())
    {
        void* arg = emitter->GetFunctionArgument(function, np);
        emitter->CreateStore(arg, functionSymbol->ReturnParam()->IrObject(*emitter));
    }
    for (cmajor::binder::BoundStatement* labeledStatement : boundFunction.LabeledStatements())
    {
        void* target = emitter->CreateBasicBlock(util::ToUtf8(labeledStatement->Label()));
        labeledStatementMap[labeledStatement] = target;
    }
    emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::entryCode));
    cmajor::binder::BoundCompoundStatement* body = boundFunction.Body();
    body->Accept(*this);
    cmajor::binder::BoundStatement* lastStatement = nullptr;
    if (!body->Statements().empty())
    {
        lastStatement = body->Statements().back().get();
    }
    if (!lastStatement || lastStatement->GetBoundNodeType() != cmajor::binder::BoundNodeType::boundReturnStatement ||
        lastStatement->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundReturnStatement && destructorCallGenerated)
    {
        if (functionSymbol->ReturnType() && functionSymbol->ReturnType()->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol && !functionSymbol->ReturnsClassInterfaceOrClassDelegateByValue())
        {
            void* defaultValue = functionSymbol->ReturnType()->CreateDefaultIrValue(*emitter);
            if (generateLineNumbers)
            {
                cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), body->EndSpan(), fileIndex);
                emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
            }
            int16_t functionScopeId = emitter->GetCurrentScopeId();
            emitter->SetCurrentScopeId(scopeIdMap[body]);
            emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
            emitter->CreateRet(defaultValue);
            emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
            emitter->SetCurrentScopeId(functionScopeId);
            lastInstructionWasRet = true;
        }
        else
        {
            if (generateLineNumbers)
            {
                cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), body->EndSpan(), fileIndex);
                emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
            }
            int16_t functionScopeId = emitter->GetCurrentScopeId();
            emitter->SetCurrentScopeId(scopeIdMap[body]);
            emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
            emitter->CreateRetVoid();
            emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
            emitter->SetCurrentScopeId(functionScopeId);
            lastInstructionWasRet = true;
        }
    }
    emitter->PopParentBlock();
    emitter->FinalizeFunction(function, functionSymbol->HasCleanup());
    if (functionSymbol->HasSource())
    {
        emitter->EndScope();
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundCompoundStatement& boundCompoundStatement)
{
    if (generateLineNumbers)
    {
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundCompoundStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
        emitter->BeginScope();
        scopeIdMap[&boundCompoundStatement] = emitter->GetCurrentScopeId();
        if (&boundCompoundStatement != currentFunction->Body())
        {
            emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::beginBrace));
            int32_t nopNodeId = emitter->AddControlFlowGraphNode();
            emitter->CreateNop();
            if (prevControlFlowGraphNodeId != -1)
            {
                emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, nopNodeId);
            }
            prevControlFlowGraphNodeId = nopNodeId;
            emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::beginBrace));
        }
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    int prevNumTriesInCurrentBlock = numTriesInCurrentBlock;
    numTriesInCurrentBlock = 0;
    SetTarget(&boundCompoundStatement);
    cmajor::binder::BoundCompoundStatement* prevBlock = currentBlock;
    currentBlock = &boundCompoundStatement;
    blockDestructionMap[currentBlock] = std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>();
    blocks.push_back(currentBlock);
    if (!prevBlock)
    {
        GenerateEnterFunctionCode(*currentFunction);
    }
    if (generateLineNumbers)
    {
        SetSpan(boundCompoundStatement.GetSpan());
    }
    int n = boundCompoundStatement.Statements().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundStatement* statement = boundCompoundStatement.Statements()[i].get();
        statement->Accept(*this);
    }
    if (generateLineNumbers)
    {
        emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundCompoundStatement.EndSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
        int32_t nopNodeId = emitter->AddControlFlowGraphNode();
        emitter->CreateNop();
        if (prevControlFlowGraphNodeId != -1)
        {
            emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, nopNodeId);
        }
        prevControlFlowGraphNodeId = nopNodeId;
        emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
    }
    ExitBlocks(prevBlock);
    GenerateCodeForCleanups();
    for (int i = 0; i < numTriesInCurrentBlock; ++i)
    {
        emitter->PopParentBlock();
    }
    if (generateLineNumbers)
    {
        emitter->EndScope();
    }
    blocks.pop_back();
    currentBlock = prevBlock;
    numTriesInCurrentBlock = prevNumTriesInCurrentBlock;
    if (boundCompoundStatement.Parent() && boundCompoundStatement.Parent()->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundTryStatement)
    {
        emitter->CreateBr(currentTryNextBlock);
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundSequenceStatement& boundSequenceStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundSequenceStatement);
    cmajor::binder::BoundStatement* prevSequence = sequenceSecond;
    sequenceSecond = boundSequenceStatement.Second();
    boundSequenceStatement.First()->Accept(*this);
    sequenceSecond = prevSequence;
    if (!boundSequenceStatement.Second()->Generated())
    {
        boundSequenceStatement.Second()->Accept(*this);
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundReturnStatement& boundReturnStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundReturnStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundReturnStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundReturnStatement);
    cmajor::binder::BoundFunctionCall* returnFunctionCall = boundReturnStatement.ReturnFunctionCall();
    if (returnFunctionCall)
    {
        boundReturnStatement.ReturnFunctionCall()->Accept(*this);
        void* returnValue = emitter->Stack().Pop();
        if (sequenceSecond)
        {
            sequenceSecond->SetGenerated();
            sequenceSecond->Accept(*this);
        }
        ExitBlocks(nullptr);
        int32_t retNodeId = -1;
        if (generateLineNumbers)
        {
            emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
            cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), currentBlock->EndSpan(), fileIndex);
            emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
            retNodeId = emitter->AddControlFlowGraphNode();
        }
        emitter->CreateRet(returnValue);
        if (prevControlFlowGraphNodeId != -1)
        {
            emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, retNodeId);
        }
        prevControlFlowGraphNodeId = retNodeId;
        if (generateLineNumbers)
        {
            emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
        }
        lastInstructionWasRet = true;
    }
    else
    {
        ExitBlocks(nullptr);
        int32_t retNodeId = -1;
        if (generateLineNumbers)
        {
            emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
            cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), currentBlock->EndSpan(), fileIndex);
            emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
            retNodeId = emitter->AddControlFlowGraphNode();
        }
        emitter->CreateRetVoid();
        if (prevControlFlowGraphNodeId != -1)
        {
            emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, retNodeId);
        }
        prevControlFlowGraphNodeId = retNodeId;
        if (generateLineNumbers)
        {
            emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::endBrace));
        }
        lastInstructionWasRet = true;
    }
    cmajor::binder::BoundCompoundStatement* body = currentFunction->Body();
    cmajor::binder::BoundStatement* lastStatement = nullptr;
    if (!body->Statements().empty())
    {
        lastStatement = body->Statements().back().get();
    }
    if (lastStatement && !lastStatement->IsOrContainsBoundReturnStatement())
    {
        void* nextBlock = emitter->CreateBasicBlock("next");
        emitter->SetCurrentBasicBlock(nextBlock);
        basicBlockOpen = true;
        lastInstructionWasRet = false;
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundGotoCaseStatement& boundGotoCaseStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundGotoCaseStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundGotoCaseStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundGotoCaseStatement);
    Assert(breakTargetBlock, "break target not set");
    ExitBlocks(breakTargetBlock);
    cmajor::symbols::IntegralValue integralCaseValue(boundGotoCaseStatement.CaseValue());
    auto it = currentCaseMap->find(integralCaseValue);
    if (it != currentCaseMap->cend())
    {
        void* caseDest = it->second;
        int32_t brNodeId = -1;
        if (generateLineNumbers)
        {
            brNodeId = emitter->AddControlFlowGraphNode();
        }
        emitter->CreateBr(caseDest);
        if (prevControlFlowGraphNodeId != -1)
        {
            emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, brNodeId);
        }
        prevControlFlowGraphNodeId = brNodeId;
    }
    else
    {
        throw cmajor::symbols::Exception("case not found", boundGotoCaseStatement.GetFullSpan());
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundGotoDefaultStatement& boundGotoDefaultStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundGotoDefaultStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundGotoDefaultStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundGotoDefaultStatement);
    Assert(breakTargetBlock, "break target not set");
    ExitBlocks(breakTargetBlock);
    if (defaultDest)
    {
        int32_t brNodeId = -1;
        if (generateLineNumbers)
        {
            brNodeId = emitter->AddControlFlowGraphNode();
        }
        emitter->CreateBr(defaultDest);
        if (prevControlFlowGraphNodeId != -1)
        {
            emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, brNodeId);
        }
        prevControlFlowGraphNodeId = brNodeId;
    }
    else
    {
        throw cmajor::symbols::Exception("no default destination", boundGotoDefaultStatement.GetFullSpan());
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundBreakStatement& boundBreakStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundBreakStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundBreakStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundBreakStatement);
    Assert(breakTarget && breakTargetBlock, "break target not set");
    ExitBlocks(breakTargetBlock);
    int32_t brNodeId = -1;
    if (generateLineNumbers)
    {
        brNodeId = emitter->AddControlFlowGraphNode();
    }
    emitter->CreateBr(breakTarget);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, brNodeId);
    }
    prevControlFlowGraphNodeId = brNodeId;
    if (!currentCaseMap) // not in switch
    {
        void* nextBlock = emitter->CreateBasicBlock("next");
        emitter->SetCurrentBasicBlock(nextBlock);
        basicBlockOpen = true;
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundContinueStatement& boundContinueStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundContinueStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundContinueStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    prevWasTerminator = false;
    SetTarget(&boundContinueStatement);
    Assert(continueTarget && continueTargetBlock, "continue target not set");
    ExitBlocks(continueTargetBlock);
    int32_t brNodeId = -1;
    if (generateLineNumbers)
    {
        brNodeId = emitter->AddControlFlowGraphNode();
    }
    emitter->CreateBr(continueTarget);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, brNodeId);
        emitter->AddControlFlowGraphEdge(brNodeId, continueTargetNodeId);
    }
    prevControlFlowGraphNodeId = brNodeId;
    void* nextBlock = emitter->CreateBasicBlock("next");
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundGotoStatement& boundGotoStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundGotoStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundGotoStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    prevWasTerminator = false;
    SetTarget(&boundGotoStatement);
    ExitBlocks(boundGotoStatement.TargetBlock());
    auto it = labeledStatementMap.find(boundGotoStatement.TargetStatement());
    if (it != labeledStatementMap.cend())
    {
        void* target = it->second;
        int32_t brNodeId = -1;
        if (generateLineNumbers)
        {
            brNodeId = emitter->AddControlFlowGraphNode();
        }
        emitter->CreateBr(target);
        if (prevControlFlowGraphNodeId != -1)
        {
            emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, brNodeId);
        }
        prevControlFlowGraphNodeId = brNodeId;
    }
    else
    {
        throw cmajor::symbols::Exception("goto target not found", boundGotoStatement.GetFullSpan());
    }
    void* nextBlock = emitter->CreateBasicBlock("next");
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundIfStatement& boundIfStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundIfStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    trueBlock = emitter->CreateBasicBlock("true");
    void* nextBlock = emitter->CreateBasicBlock("next");
    if (boundIfStatement.ElseS())
    {
        falseBlock = emitter->CreateBasicBlock("false");
    }
    else
    {
        falseBlock = nextBlock;
    }
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    int32_t condNodeId = -1;
    if (generateLineNumbers)
    {
        SetSpan(boundIfStatement.GetSpan());
        condNodeId = emitter->AddControlFlowGraphNode();
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundIfStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    boundIfStatement.Condition()->Accept(*this);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, condNodeId);
    }
    prevControlFlowGraphNodeId = condNodeId;
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    boundIfStatement.ThenS()->Accept(*this);
    emitter->CreateBr(nextBlock);
    if (boundIfStatement.ElseS())
    {
        emitter->SetCurrentBasicBlock(falseBlock);
        prevControlFlowGraphNodeId = condNodeId;
        boundIfStatement.ElseS()->Accept(*this);
        emitter->CreateBr(nextBlock);
    }
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundWhileStatement& boundWhileStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundWhileStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    void* prevBreakTarget = breakTarget;
    void* prevContinueTarget = continueTarget;
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    cmajor::binder::BoundCompoundStatement* prevContinueTargetBlock = continueTargetBlock;
    breakTargetBlock = currentBlock;
    continueTargetBlock = currentBlock;
    trueBlock = emitter->CreateBasicBlock("true");
    falseBlock = emitter->CreateBasicBlock("next");
    breakTarget = falseBlock;
    void* condBlock = emitter->CreateBasicBlock("cond");
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    continueTarget = condBlock;
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    int32_t prevContinueTargetNodeId = continueTargetNodeId;
    int32_t condNodeId = -1;
    if (generateLineNumbers)
    {
        SetSpan(boundWhileStatement.GetSpan());
        condNodeId = emitter->AddControlFlowGraphNode();
        continueTargetNodeId = condNodeId;
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundWhileStatement.Condition()->GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    boundWhileStatement.Condition()->Accept(*this);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, condNodeId);
    }
    prevControlFlowGraphNodeId = condNodeId;
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    boundWhileStatement.Statement()->Accept(*this);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, condNodeId);
    }
    prevControlFlowGraphNodeId = condNodeId;
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(falseBlock);
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    continueTarget = prevContinueTarget;
    continueTargetNodeId = prevContinueTargetNodeId;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundDoStatement& boundDoStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundDoStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    void* prevBreakTarget = breakTarget;
    void* prevContinueTarget = continueTarget;
    void* doBlock = emitter->CreateBasicBlock("do");
    void* condBlock = emitter->CreateBasicBlock("cond");
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    cmajor::binder::BoundCompoundStatement* prevContinueTargetBlock = continueTargetBlock;
    breakTargetBlock = currentBlock;
    continueTargetBlock = currentBlock;
    trueBlock = doBlock;
    falseBlock = emitter->CreateBasicBlock("next");
    breakTarget = falseBlock;
    continueTarget = condBlock;
    emitter->CreateBr(doBlock);
    emitter->SetCurrentBasicBlock(doBlock);
    int32_t doNodeId = -1;
    if (generateLineNumbers)
    {
        SetSpan(boundDoStatement.GetSpan());
        doNodeId = emitter->AddControlFlowGraphNode();
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundDoStatement.Statement()->GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
        emitter->CreateNop();
        prevControlFlowGraphNodeId = doNodeId;
    }
    boundDoStatement.Statement()->Accept(*this);
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    int32_t prevContinueTargetNodeId = continueTargetNodeId;
    int32_t condNodeId = -1;
    if (generateLineNumbers)
    {
        condNodeId = emitter->AddControlFlowGraphNode();
        continueTargetNodeId = condNodeId;
        cmajor::debug::SourceSpan condSpan = cmajor::debug::MakeSourceSpan(module->FileMap(), boundDoStatement.Condition()->GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(condSpan.line, condSpan.scol, condSpan.ecol);
    }
    boundDoStatement.Condition()->Accept(*this);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, condNodeId);
        emitter->AddControlFlowGraphEdge(condNodeId, doNodeId);
    }
    prevControlFlowGraphNodeId = condNodeId;
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(falseBlock);
    basicBlockOpen = true;
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    continueTarget = prevContinueTarget;
    continueTargetNodeId = prevContinueTargetNodeId;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundForStatement& boundForStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundForStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    void* prevBreakTarget = breakTarget;
    void* prevContinueTarget = continueTarget;
    boundForStatement.InitS()->Accept(*this);
    void* condBlock = emitter->CreateBasicBlock("cond");
    void* actionBlock = emitter->CreateBasicBlock("action");
    void* loopBlock = emitter->CreateBasicBlock("loop");
    trueBlock = actionBlock;
    falseBlock = emitter->CreateBasicBlock("next");
    breakTarget = falseBlock;
    continueTarget = loopBlock;
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    cmajor::binder::BoundCompoundStatement* prevContinueTargetBlock = continueTargetBlock;
    breakTargetBlock = currentBlock;
    continueTargetBlock = currentBlock;
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    int32_t prevContinueTargetNodeId = continueTargetNodeId;
    int32_t condNodeId = -1;
    if (generateLineNumbers)
    {
        SetSpan(boundForStatement.GetSpan());
        condNodeId = emitter->AddControlFlowGraphNode();
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundForStatement.Condition()->GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    boundForStatement.Condition()->Accept(*this);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, condNodeId);
    }
    prevControlFlowGraphNodeId = condNodeId;
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(actionBlock);
    int32_t currentLoopNodeId = -1;
    if (generateLineNumbers)
    {
        currentLoopNodeId = emitter->AddControlFlowGraphNode();
        continueTargetNodeId = currentLoopNodeId;
    }
    boundForStatement.ActionS()->Accept(*this);
    emitter->CreateBr(loopBlock);
    emitter->SetCurrentBasicBlock(loopBlock);
    int32_t prevLoopNodeId = loopNodeId;
    loopNodeId = currentLoopNodeId;
    boundForStatement.LoopS()->Accept(*this);
    loopNodeId = prevLoopNodeId;
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, condNodeId);
    }
    prevControlFlowGraphNodeId = condNodeId;
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(falseBlock);
    basicBlockOpen = true;
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    continueTarget = prevContinueTarget;
    continueTargetNodeId = prevContinueTargetNodeId;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundSwitchStatement& boundSwitchStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundSwitchStatement);
    void* prevBreakTarget = breakTarget;
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    breakTargetBlock = currentBlock;
    int32_t condNodeId = -1;
    if (generateLineNumbers)
    {
        SetSpan(boundSwitchStatement.GetSpan());
        condNodeId = emitter->AddControlFlowGraphNode();
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundSwitchStatement.Condition()->GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    boundSwitchStatement.Condition()->Accept(*this);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, condNodeId);
    }
    prevControlFlowGraphNodeId = condNodeId;
    void* condition = emitter->Stack().Pop();
    void* prevDefaultDest = defaultDest;
    void* next = nullptr;
    if (boundSwitchStatement.DefaultStatement())
    {
        defaultDest = emitter->CreateBasicBlock("default");
        next = emitter->CreateBasicBlock("next");
    }
    else
    {
        defaultDest = emitter->CreateBasicBlock("next");
        next = defaultDest;
    }
    breakTarget = next;
    unsigned n = boundSwitchStatement.CaseStatements().size();
    void* switchInst = emitter->CreateSwitch(condition, defaultDest, n);
    std::unordered_map<cmajor::symbols::IntegralValue, void*, cmajor::symbols::IntegralValueHash>* prevCaseMap = currentCaseMap;
    std::unordered_map<cmajor::symbols::IntegralValue, void*, cmajor::symbols::IntegralValueHash> caseMap;
    currentCaseMap = &caseMap;
    for (unsigned i = 0; i < n; ++i)
    {
        const std::unique_ptr<cmajor::binder::BoundCaseStatement>& caseS = boundSwitchStatement.CaseStatements()[i];
        void* caseDest = emitter->CreateBasicBlock("case" + std::to_string(i));
        for (const std::unique_ptr<cmajor::symbols::Value>& caseValue : caseS->CaseValues())
        {
            cmajor::symbols::IntegralValue integralCaseValue(caseValue.get());
            caseMap[integralCaseValue] = caseDest;
            emitter->AddCase(switchInst, caseValue->IrValue(*emitter), caseDest);
        }
    }
    for (unsigned i = 0; i < n; ++i)
    {
        const std::unique_ptr<cmajor::binder::BoundCaseStatement>& caseS = boundSwitchStatement.CaseStatements()[i];
        prevControlFlowGraphNodeId = condNodeId;
        caseS->Accept(*this);
        if (basicBlockOpen)
        {
            emitter->CreateBr(next);
            basicBlockOpen = false;
        }
    }
    if (boundSwitchStatement.DefaultStatement())
    {
        prevControlFlowGraphNodeId = condNodeId;
        boundSwitchStatement.DefaultStatement()->Accept(*this);
        if (basicBlockOpen)
        {
            emitter->CreateBr(next);
            basicBlockOpen = false;
        }
    }
    emitter->SetCurrentBasicBlock(next);
    basicBlockOpen = true;
    currentCaseMap = prevCaseMap;
    defaultDest = prevDefaultDest;
    breakTargetBlock = prevBreakTargetBlock;
    breakTarget = prevBreakTarget;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundCaseStatement& boundCaseStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundCaseStatement);
    if (!boundCaseStatement.CaseValues().empty())
    {
        cmajor::symbols::IntegralValue integralCaseValue(boundCaseStatement.CaseValues().front().get());
        auto it = currentCaseMap->find(integralCaseValue);
        if (it != currentCaseMap->cend())
        {
            void* caseDest = it->second;
            emitter->SetCurrentBasicBlock(caseDest);
            if (generateLineNumbers)
            {
                SetSpan(boundCaseStatement.GetSpan());
                cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundCaseStatement.GetSpan(), fileIndex);
                emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
            }
            if (boundCaseStatement.CompoundStatement())
            {
                boundCaseStatement.CompoundStatement()->Accept(*this);
            }
        }
        else
        {
            throw cmajor::symbols::Exception("case not found", boundCaseStatement.GetFullSpan());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("no cases", boundCaseStatement.GetFullSpan());
    }

}

void CppCodeGenerator::Visit(cmajor::binder::BoundDefaultStatement& boundDefaultStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundDefaultStatement);
    if (defaultDest)
    {
        emitter->SetCurrentBasicBlock(defaultDest);
        if (generateLineNumbers)
        {
            SetSpan(boundDefaultStatement.GetSpan());
            cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundDefaultStatement.GetSpan(), fileIndex);
            emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
        }
        if (boundDefaultStatement.CompoundStatement())
        {
            boundDefaultStatement.CompoundStatement()->Accept(*this);
        }
    }
    else
    {
        throw cmajor::symbols::Exception("no default destination", boundDefaultStatement.GetFullSpan());
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundConstructionStatement& boundConstructionStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundConstructionStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundConstructionStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
        cmajor::symbols::LocalVariableSymbol* localVariable = boundConstructionStatement.GetLocalVariable();
        if (localVariable)
        {
            const util::uuid& typeId = localVariable->GetType()->TypeId();
            module->GetTypeIndex().AddType(typeId, localVariable->GetType(), *emitter);
            emitter->AddLocalVariable(util::ToUtf8(localVariable->Name()), typeId, localVariable->IrObject(*emitter));
        }
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundConstructionStatement);
    int32_t constructorNodeId = -1;
    if (generateLineNumbers)
    {
        constructorNodeId = emitter->AddControlFlowGraphNode();
    }
    boundConstructionStatement.ConstructorCall()->Accept(*this);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, constructorNodeId);
    }
    prevControlFlowGraphNodeId = constructorNodeId;
    if (!boundConstructionStatement.ConstructorCall()->GetFunctionSymbol()->IsBasicTypeOperation())
    {
        int n = boundConstructionStatement.ConstructorCall()->Arguments().size();
        if (n > 0)
        {
            const std::unique_ptr<cmajor::binder::BoundExpression>& firstArgument = boundConstructionStatement.ConstructorCall()->Arguments()[0];
            cmajor::symbols::TypeSymbol* firstArgumentBaseType = firstArgument->GetType()->BaseType();
            if (firstArgumentBaseType->IsClassTypeSymbol())
            {
                if (firstArgument->GetType()->IsPointerType() && firstArgument->GetType()->RemovePointer()->IsClassTypeSymbol())
                {
                    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(firstArgumentBaseType);
                    if (classType->Destructor())
                    {
                        void* cleanupTry = emitter->CreateBasicBlock("cleanupTry");
                        emitter->CreateBr(cleanupTry);
                        emitter->SetCurrentBasicBlock(cleanupTry);
                        emitter->PushParentBlock();
                        emitter->CreateBeginTry();
                        tryIndexMap[numTriesInCurrentBlock] = tryIndex;
                        tryIndexCleanupTryBlockMap[tryIndex] = cleanupTry;
                        std::unique_ptr<cmajor::binder::BoundExpression> classPtrArgument(firstArgument->Clone());
                        std::unique_ptr<cmajor::binder::BoundFunctionCall> destructorCall(new cmajor::binder::BoundFunctionCall(currentBlock->EndSpan(), classType->Destructor()));
                        destructorCall->AddArgument(std::move(classPtrArgument));
                        GenerateCleanup(tryIndex, destructorCall.get());
                        Assert(currentBlock, "current block not set");
                        auto it = blockDestructionMap.find(currentBlock);
                        if (it != blockDestructionMap.cend())
                        {
                            std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>& destructorCallVec = it->second;
                            destructorCallVec.push_back(std::move(destructorCall));
                        }
                        else
                        {
                            Assert(false, "block destruction not found");
                        }
                        ++numTriesInCurrentBlock;
                        ++tryIndex;
                    }
                }
            }
        }
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundAssignmentStatement& boundAssignmentStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundAssignmentStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundAssignmentStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundAssignmentStatement);
    int32_t assignmentNodeId = -1;
    if (generateLineNumbers)
    {
        if (boundAssignmentStatement.IsForLoopStatementNode())
        {
            assignmentNodeId = loopNodeId;
            emitter->SetCurrentControlFlowGraphNodeId(assignmentNodeId);
        }
        else
        {
            assignmentNodeId = emitter->AddControlFlowGraphNode();
        }
    }
    boundAssignmentStatement.AssignmentCall()->Accept(*this);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, assignmentNodeId);
    }
    prevControlFlowGraphNodeId = assignmentNodeId;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundExpressionStatement& boundExpressionStatement)
{
    if (generateLineNumbers && !boundExpressionStatement.IgnoreNode())
    {
        SetSpan(boundExpressionStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundExpressionStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundExpressionStatement);
    int32_t expressionNodeId = -1;
    if (generateLineNumbers && !boundExpressionStatement.IgnoreNode())
    {
        if (boundExpressionStatement.IsForLoopStatementNode())
        {
            expressionNodeId = loopNodeId;
            emitter->SetCurrentControlFlowGraphNodeId(expressionNodeId);
        }
        else
        {
            expressionNodeId = emitter->AddControlFlowGraphNode();
        }
    }
    boundExpressionStatement.Expression()->Accept(*this);
    if (!boundExpressionStatement.IgnoreNode())
    {
        if (prevControlFlowGraphNodeId != -1)
        {
            emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, expressionNodeId);
        }
        prevControlFlowGraphNodeId = expressionNodeId;
    }
    if (boundExpressionStatement.Expression()->HasValue())
    {
        emitter->Stack().Pop();
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundInitializationStatement& boundInitializationStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundInitializationStatement.GetSpan());
    }
    emitter->SetCurrentSourcePos(0, 0, 0);
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundInitializationStatement);
    boundInitializationStatement.InitializationExpression()->Accept(*this);
    if (boundInitializationStatement.InitializationExpression()->HasValue())
    {
        emitter->Stack().Pop();
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundEmptyStatement& boundEmptyStatement)
{
    if (generateLineNumbers && !boundEmptyStatement.IgnoreNode())
    {
        SetSpan(boundEmptyStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundEmptyStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundEmptyStatement);
    int32_t nopNodeId = -1;
    if (generateLineNumbers && !boundEmptyStatement.IgnoreNode())
    {
        if (boundEmptyStatement.IsForLoopStatementNode())
        {
            nopNodeId = loopNodeId;
            emitter->SetCurrentControlFlowGraphNodeId(nopNodeId);
        }
        else
        {
            nopNodeId = emitter->AddControlFlowGraphNode();
        }
    }
    emitter->CreateNop();
    if (!boundEmptyStatement.IgnoreNode())
    {
        if (prevControlFlowGraphNodeId != -1)
        {
            emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, nopNodeId);
        }
        prevControlFlowGraphNodeId = nopNodeId;
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundSetVmtPtrStatement& boundSetVmtPtrStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundSetVmtPtrStatement);
    cmajor::binder::BoundExpression* classPtr = boundSetVmtPtrStatement.ClassPtr();
    cmajor::symbols::TypeSymbol* type = classPtr->GetType()->BaseType();
    Assert(type->IsClassTypeSymbol(), "class type expected");
    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(type);
    int32_t vmtPtrIndex = classType->VmtPtrIndex();
    Assert(vmtPtrIndex != -1, "invalid vmt ptr index");
    classPtr->Accept(*this);
    void* classPtrValue = emitter->Stack().Pop();
    void* ptr = emitter->GetMemberVariablePtr(classType->IrType(*emitter), classPtrValue, vmtPtrIndex);
    void* vmtPtr = emitter->CreateBitCast(boundSetVmtPtrStatement.ClassType()->VmtObject(*emitter, false), emitter->GetIrTypeForVoidPtrType());
    emitter->CreateStore(vmtPtr, ptr);
}

void CppCodeGenerator::Visit(cmajor::binder::BoundThrowStatement& boundThrowStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundThrowStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundThrowStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundThrowStatement);
    int32_t throwNodeId = -1;
    if (generateLineNumbers)
    {
        throwNodeId = emitter->AddControlFlowGraphNode();
        emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::throwInst));
    }
    boundThrowStatement.ThrowCallExpr()->Accept(*this);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, throwNodeId);
    }
    if (generateLineNumbers)
    {
        emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::throwInst));
    }
    prevControlFlowGraphNodeId = throwNodeId;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundTryStatement& boundTryStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundTryStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundTryStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundTryStatement);
    void* prevHandlerBlock = handlerBlock;
    void* prevCleanupBlock = cleanupBlock;
    void* tryNextBlock = emitter->CreateBasicBlock("tryNext");
    void* prevTryNextBlock = currentTryNextBlock;
    currentTryNextBlock = tryNextBlock;
    cleanupBlock = nullptr;
    bool prevInTryBlock = inTryBlock;
    inTryBlock = true;
    void* tryBlock = emitter->CreateBasicBlock("try");
    emitter->CreateBr(tryBlock);
    emitter->SetCurrentBasicBlock(tryBlock);
    emitter->PushParentBlock();
    emitter->CreateBeginTry();
    boundTryStatement.TryBlock()->Accept(*this);
    int32_t tryBlockId = prevControlFlowGraphNodeId;
    inTryBlock = prevInTryBlock;
    emitter->SetCurrentBasicBlock(tryBlock);
    emitter->CreateEndTry(tryNextBlock);
    emitter->CreateBeginCatch();
    handlerBlock = emitter->CreateBasicBlock("handlers");
    emitter->CreateBr(handlerBlock);
    emitter->CreateIncludeBasicBlockInstruction(handlerBlock);
    emitter->SetHandlerBlock(tryBlock, handlerBlock);
    emitter->SetCurrentBasicBlock(handlerBlock);
    emitter->PushParentBlock();
    void* resumeBlock = nullptr;
    int n = boundTryStatement.Catches().size();
    for (int i = 0; i < n; ++i)
    {
        const std::unique_ptr<cmajor::binder::BoundCatchStatement>& boundCatchStatement = boundTryStatement.Catches()[i];
        std::vector<void*> handleExceptionParamTypes;
        handleExceptionParamTypes.push_back(emitter->GetIrTypeForVoidPtrType());
        void* handleExceptionFunctionType = emitter->GetIrTypeForFunction(emitter->GetIrTypeForBool(), handleExceptionParamTypes);
        std::vector<void*> handleExceptionArgs;
        cmajor::symbols::UuidValue uuidValue(boundCatchStatement->GetSpan(), boundCatchStatement->CatchTypeUuidId());
        void* catchTypeIdValue = uuidValue.IrValue(*emitter);
        handleExceptionArgs.push_back(catchTypeIdValue);
        void* handleException = emitter->GetOrInsertFunction("RtmHandleException", handleExceptionFunctionType, true);
        void* handleThisEx = emitter->CreateCall(handleExceptionFunctionType, handleException, handleExceptionArgs);
        void* nextHandlerTarget = nullptr;
        if (i < n - 1)
        {
            nextHandlerTarget = emitter->CreateBasicBlock("catch");
        }
        else
        {
            resumeBlock = emitter->CreateBasicBlock("resume");
            nextHandlerTarget = resumeBlock;
        }
        void* thisHandlerTarget = emitter->CreateBasicBlock("handler");
        emitter->CreateCondBr(handleThisEx, thisHandlerTarget, nextHandlerTarget);
        emitter->SetCurrentBasicBlock(thisHandlerTarget);
        prevControlFlowGraphNodeId = tryBlockId;
        if (generateLineNumbers)
        {
            SetSpan(boundCatchStatement->GetSpan());
            emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::catchInst));
        }
        boundCatchStatement->CatchBlock()->Accept(*this);
        if (generateLineNumbers)
        {
            emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::catchInst));
        }
        emitter->CreateBr(tryNextBlock);
    }
    emitter->SetCurrentBasicBlock(resumeBlock);
    emitter->CreateResume(nullptr);
    currentFunction->GetFunctionSymbol()->SetHasCleanup();
    emitter->CreateBr(tryNextBlock);
    emitter->PopParentBlock();
    emitter->SetCurrentBasicBlock(tryBlock);
    emitter->CreateEndCatch(nullptr);
    emitter->CreateBr(tryNextBlock);
    basicBlockOpen = false;
    emitter->PopParentBlock();
    emitter->SetCurrentBasicBlock(tryNextBlock);
    currentTryNextBlock = prevTryNextBlock;
    cleanupBlock = prevCleanupBlock;
    basicBlockOpen = true;
}

void CppCodeGenerator::Visit(cmajor::binder::BoundRethrowStatement& boundRethrowStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundRethrowStatement.GetSpan());
        cmajor::debug::SourceSpan span = cmajor::debug::MakeSourceSpan(module->FileMap(), boundRethrowStatement.GetSpan(), fileIndex);
        emitter->SetCurrentSourcePos(span.line, span.scol, span.ecol);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundRethrowStatement);
    boundRethrowStatement.ReleaseCall()->Accept(*this);
    int32_t resumeNodeId = -1;
    if (generateLineNumbers)
    {
        resumeNodeId = emitter->AddControlFlowGraphNode();
        emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::throwInst));
    }
    if (prevControlFlowGraphNodeId == -1)
    {

    }
    emitter->CreateResume(nullptr);
    if (prevControlFlowGraphNodeId != -1)
    {
        emitter->AddControlFlowGraphEdge(prevControlFlowGraphNodeId, resumeNodeId);
    }
    if (generateLineNumbers)
    {
        emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::throwInst));
    }
    prevControlFlowGraphNodeId = resumeNodeId;
    currentFunction->GetFunctionSymbol()->SetHasCleanup();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundParameter& boundParameter)
{
    boundParameter.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundLocalVariable& boundLocalVariable)
{
    boundLocalVariable.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundMemberVariable& boundMemberVariable)
{
    boundMemberVariable.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundConstant& boundConstant)
{
    boundConstant.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundEnumConstant& boundEnumConstant)
{
    boundEnumConstant.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundLiteral& boundLiteral)
{
    boundLiteral.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundTemporary& boundTemporary)
{
    boundTemporary.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundSizeOfExpression& boundSizeOfExpression)
{
    boundSizeOfExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void CppCodeGenerator::Visit(cmajor::binder::BoundAddressOfExpression& boundAddressOfExpression)
{
    boundAddressOfExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void CppCodeGenerator::Visit(cmajor::binder::BoundDereferenceExpression& boundDereferenceExpression)
{
    boundDereferenceExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundReferenceToPointerExpression& boundReferenceToPointerExpression)
{
    boundReferenceToPointerExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void CppCodeGenerator::Visit(cmajor::binder::BoundFunctionCall& boundFunctionCall)
{
    boundFunctionCall.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundDelegateCall& boundDelegateCall)
{
    boundDelegateCall.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundClassDelegateCall& boundClassDelegateCall)
{
    boundClassDelegateCall.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundConversion& boundConversion)
{
    boundConversion.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundConstructExpression& boundConstructExpression)
{
    boundConstructExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    cmajor::symbols::TypeSymbol* type = boundConstructExpression.GetType();
    module->GetTypeIndex().AddType(type->TypeId(), type, *emitter);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundConstructAndReturnTemporaryExpression& boundConstructAndReturnTemporaryExpression)
{
    boundConstructAndReturnTemporaryExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    cmajor::symbols::TypeSymbol* type = boundConstructAndReturnTemporaryExpression.GetType();
    module->GetTypeIndex().AddType(type->TypeId(), type, *emitter);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundClassOrClassDelegateConversionResult& boundClassOrClassDelegateConversionResult)
{
    boundClassOrClassDelegateConversionResult.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundIsExpression& boundIsExpression)
{
    boundIsExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    cmajor::symbols::TypeSymbol* exprType = boundIsExpression.Expr()->GetType();
    module->GetTypeIndex().AddType(exprType->TypeId(), exprType, *emitter);
    cmajor::symbols::TypeSymbol* rightType = boundIsExpression.RightClassType();
    module->GetTypeIndex().AddType(rightType->TypeId(), rightType, *emitter);
    GenJumpingBoolCode();
}

void CppCodeGenerator::Visit(cmajor::binder::BoundAsExpression& boundAsExpression)
{
    boundAsExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    cmajor::symbols::TypeSymbol* exprType = boundAsExpression.Expr()->GetType();
    module->GetTypeIndex().AddType(exprType->TypeId(), exprType, *emitter);
    cmajor::symbols::TypeSymbol* rightType = boundAsExpression.RightClassType();
    module->GetTypeIndex().AddType(rightType->TypeId(), rightType, *emitter);
}

void CppCodeGenerator::Visit(cmajor::binder::BoundTypeNameExpression& boundTypeNameExpression)
{
    boundTypeNameExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    cmajor::symbols::TypeSymbol* classPtrType = boundTypeNameExpression.ClassPtr()->GetType();
    module->GetTypeIndex().AddType(classPtrType->TypeId(), classPtrType, *emitter);
}

void CppCodeGenerator::Visit(cmajor::binder::BoundBitCast& boundBitCast)
{
    boundBitCast.Load(*emitter, cmajor::ir::OperationFlags::none);
    cmajor::symbols::TypeSymbol* exprType = boundBitCast.Expr()->GetType();
    module->GetTypeIndex().AddType(exprType->TypeId(), exprType, *emitter);
    cmajor::symbols::TypeSymbol* type = boundBitCast.GetType();
    module->GetTypeIndex().AddType(type->TypeId(), type, *emitter);
}

void CppCodeGenerator::Visit(cmajor::binder::BoundFunctionPtr& boundFunctionPtr)
{
    boundFunctionPtr.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void CppCodeGenerator::Visit(cmajor::binder::BoundDisjunction& boundDisjunction)
{
    if (genJumpingBoolCode)
    {
        Assert(trueBlock, "true block not set");
        Assert(falseBlock, "false block not set");
        void* rightBlock = emitter->CreateBasicBlock("right");
        void* prevFalseBlock = falseBlock;
        falseBlock = rightBlock;
        boundDisjunction.Left()->Accept(*this);
        emitter->SetCurrentBasicBlock(rightBlock);
        falseBlock = prevFalseBlock;
        boundDisjunction.Right()->Accept(*this);
        boundDisjunction.DestroyTemporaries(*emitter);
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundConjunction& boundConjunction)
{
    if (genJumpingBoolCode)
    {
        Assert(trueBlock, "true block not set");
        Assert(falseBlock, "false block not set");
        void* rightBlock = emitter->CreateBasicBlock("right");
        void* prevTrueBlock = trueBlock;
        trueBlock = rightBlock;
        boundConjunction.Left()->Accept(*this);
        trueBlock = prevTrueBlock;
        emitter->SetCurrentBasicBlock(rightBlock);
        boundConjunction.Right()->Accept(*this);
        boundConjunction.DestroyTemporaries(*emitter);
    }
}

void CppCodeGenerator::Visit(cmajor::binder::BoundGlobalVariable& boundGlobalVariable)
{
    cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol = boundGlobalVariable.GetGlobalVariableSymbol();
    globalVariableSymbol->CreateIrObject(*emitter);
}

void CppCodeGenerator::GenJumpingBoolCode()
{
    if (!genJumpingBoolCode) return;
    Assert(trueBlock, "true block not set");
    Assert(falseBlock, "false block not set");
    void* cond = emitter->Stack().Pop();
    if (sequenceSecond)
    {
        genJumpingBoolCode = false;
        sequenceSecond->SetGenerated();
        sequenceSecond->Accept(*this);
        genJumpingBoolCode = true;
    }
    emitter->CreateCondBr(cond, trueBlock, falseBlock);
}

void CppCodeGenerator::SetTarget(cmajor::binder::BoundStatement* labeledStatement)
{
    if (labeledStatement->Label().empty()) return;
    auto it = labeledStatementMap.find(labeledStatement);
    if (it != labeledStatementMap.cend())
    {
        void* target = it->second;
        emitter->CreateBr(target);
        emitter->SetCurrentBasicBlock(target);
    }
    else
    {
        throw cmajor::symbols::Exception("target for labeled statement not found", labeledStatement->GetFullSpan());
    }
}

void CppCodeGenerator::ExitBlocks(cmajor::binder::BoundCompoundStatement* targetBlock)
{
    bool createBasicBlock = false;
    cmajor::binder::BoundStatement* lastStatement = nullptr;
    if (!currentFunction->Body()->Statements().empty())
    {
        lastStatement = currentFunction->Body()->Statements().back().get();
    }
    cmajor::binder::BoundStatement* currentBlockLastStatement = nullptr;
    if (currentBlock && !currentBlock->Statements().empty())
    {
        currentBlockLastStatement = currentBlock->Statements().back().get();
    }
    if (lastStatement && currentBlockLastStatement && lastStatement == currentBlockLastStatement && currentBlockLastStatement->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundReturnStatement)
    {
        createBasicBlock = true;
    }
    int n = blocks.size();
    for (int i = n - 1; i >= 0; --i)
    {
        cmajor::binder::BoundCompoundStatement* block = blocks[i];
        if (block == targetBlock)
        {
            break;
        }
        auto it = blockDestructionMap.find(block);
        if (it != blockDestructionMap.cend())
        {
            std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>& destructorCallVec = it->second;
            int nd = destructorCallVec.size();
            for (int i = nd - 1; i >= 0; --i)
            {
                std::unique_ptr<cmajor::binder::BoundFunctionCall>& destructorCall = destructorCallVec[i];
                if (destructorCall)
                {
                    if (createBasicBlock)
                    {
                        void* nextBlock = emitter->CreateBasicBlock("next");
                        if (!lastInstructionWasRet)
                        {
                            emitter->CreateBr(nextBlock);
                        }
                        emitter->SetCurrentBasicBlock(nextBlock);
                        createBasicBlock = false;
                    }
                    destructorCall->Accept(*this);
                    destructorCallGenerated = true;
                }
            }
        }
    }
}

void* CppCodeGenerator::GetGlobalStringPtr(int stringId)
{
    auto it = utf8stringMap.find(stringId);
    if (it != utf8stringMap.cend())
    {
        return it->second;
    }
    else
    {
        void* stringValue = emitter->CreateGlobalStringPtr(compileUnit->GetUtf8String(stringId));
        utf8stringMap[stringId] = stringValue;
        return stringValue;
    }
}

void* CppCodeGenerator::GetGlobalWStringConstant(int stringId, void*& arrayType)
{
    auto it = utf16stringMap.find(stringId);
    if (it != utf16stringMap.cend())
    {
        return it->second;
    }
    else
    {
        const std::u16string& str = compileUnit->GetUtf16String(stringId);
        void* stringValue = emitter->CreateGlobalWStringPtr(str);
        utf16stringMap[stringId] = stringValue;
        return stringValue;
    }
}

void* CppCodeGenerator::GetGlobalUStringConstant(int stringId, void*& arrayType)
{
    auto it = utf32stringMap.find(stringId);
    if (it != utf32stringMap.cend())
    {
        return it->second;
    }
    else
    {
        const std::u32string& str = compileUnit->GetUtf32String(stringId);
        void* stringValue = emitter->CreateGlobalUStringPtr(str);
        utf32stringMap[stringId] = stringValue;
        return stringValue;
    }
}

void* CppCodeGenerator::GetGlobalUuidConstant(int uuidId)
{
    auto it = uuidMap.find(uuidId);
    if (it != uuidMap.cend())
    {
        return it->second;
    }
    else
    {
        const util::uuid& uuid = compileUnit->GetUuid(uuidId);
        uint64_t length = uuid.static_size();
        std::vector<void*> byteConstants;
        for (util::uuid::value_type x : uuid)
        {
            byteConstants.push_back(emitter->CreateIrValueForByte(static_cast<int8_t>(x)));
        }
        void* arrayType = emitter->GetIrTypeForArrayType(emitter->GetIrTypeForByte(), length);
        void* uuidObject = emitter->GetOrInsertGlobal("uuid" + std::to_string(uuidId) + "_" + compileUnitId, arrayType);
        void* uuidGlobal = uuidObject;
        emitter->SetPrivateLinkage(uuidGlobal);
        void* constant = emitter->CreateIrValueForConstantArray(arrayType, byteConstants, "");
        emitter->SetInitializer(uuidGlobal, constant);
        void* uuidValue = uuidGlobal;
        uuidMap[uuidId] = uuidValue;
        return uuidValue;
    }
}

void* CppCodeGenerator::HandlerBlock()
{
    return handlerBlock;
}

void* CppCodeGenerator::CleanupBlock()
{
    return cleanupBlock;
}

bool CppCodeGenerator::InTryBlock() const
{
    return inTryBlock;
}

int CppCodeGenerator::CurrentTryBlockId() const
{
    return currentTryBlockId;
}

int CppCodeGenerator::Install(const std::string& str)
{
    return compileUnit->Install(str);
}

int CppCodeGenerator::Install(const std::u16string& str)
{
    return compileUnit->Install(str);
}

int CppCodeGenerator::Install(const std::u32string& str)
{
    return compileUnit->Install(str);
}

void CppCodeGenerator::GenerateCleanup(int tryIndex, cmajor::binder::BoundFunctionCall* destructorCall)
{
    cleanupBlock = emitter->CreateBasicBlock("cleanup");
    emitter->SetCleanupBlock(cleanupBlock);
    std::unique_ptr<Cleanup> cleanup(new Cleanup(cleanupBlock));
    cleanup->destructors.push_back(std::unique_ptr<cmajor::binder::BoundFunctionCall>(static_cast<cmajor::binder::BoundFunctionCall*>(destructorCall->Clone())));
    cleanupMap[tryIndex] = cleanup.get();
    cleanups.push_back(std::move(cleanup));
}

void CppCodeGenerator::GenerateCodeForCleanups()
{
    for (int i = 0; i < numTriesInCurrentBlock; ++i)
    {
        --tryIndex;
        auto cleanupIt = cleanupMap.find(tryIndex);
        if (cleanupIt != cleanupMap.cend())
        {
            Cleanup* cleanup = cleanupIt->second;
            auto it = tryIndexCleanupTryBlockMap.find(tryIndex);
            if (it != tryIndexCleanupTryBlockMap.cend())
            {
                void* cleanupTryBlock = it->second;
                void* prevBasicBlock = emitter->CurrentBasicBlock();
                emitter->SetCurrentBasicBlock(cleanupTryBlock);
                emitter->CreateEndTry(nullptr);
                emitter->SetCurrentBasicBlock(cleanupTryBlock);
                emitter->CreateBeginCatch();
                for (const std::unique_ptr<cmajor::binder::BoundFunctionCall>& destructorCall : cleanup->destructors)
                {
                    destructorCall->Accept(*this);
                }
                emitter->SetCurrentBasicBlock(cleanupTryBlock);
                emitter->CreateResume(nullptr);
                emitter->CreateEndCatch(nullptr);
                currentFunction->GetFunctionSymbol()->SetHasCleanup();
                emitter->SetCurrentBasicBlock(prevBasicBlock);
            }
            else
            {
                throw std::runtime_error("internal error: try index " + std::to_string(tryIndex) + " not found");
            }
        }
        else
        {
            throw std::runtime_error("internal error: cleanup for try index " + std::to_string(tryIndex) + " not found");
        }
    }
}

/*
void CppCodeGenerator::SetLineNumber(int32_t lineNumber)
{
    if (currentFunction->GetFunctionSymbol()->DontThrow()) return;
    if (prevLineNumber == lineNumber) return;
    prevLineNumber = lineNumber;
    cmajor::binder::BoundStatement* setLineNumberStatement = currentFunction->GetLineCode();
    if (setLineNumberStatement)
    {
        bool prevGenJumpingBoolCode = genJumpingBoolCode;
        genJumpingBoolCode = false;
        emitter->BeginSubstituteLineNumber(lineNumber);
        setLineNumberStatement->Accept(*this);
        emitter->EndSubstituteLineNumber();
        genJumpingBoolCode = prevGenJumpingBoolCode;
    }
}
*/

std::string CppCodeGenerator::GetSourceFilePath(int fileIndex, const util::uuid& moduleId)
{
    return cmajor::symbols::GetSourceFilePath(fileIndex, moduleId);
}

void CppCodeGenerator::GenerateEnterFunctionCode(cmajor::binder::BoundFunction& boundFunction)
{
    const std::vector<std::unique_ptr<cmajor::binder::BoundStatement>>& enterCode = boundFunction.EnterCode();
    if (enterCode.empty()) return;
    bool prevSetLineOrEntryCode = inSetLineOrEntryCode;
    inSetLineOrEntryCode = true;
    cmajor::symbols::LocalVariableSymbol* traceEntryVar = boundFunction.GetFunctionSymbol()->TraceEntryVar();
    void* traceEntryAlloca = emitter->CreateAlloca(traceEntryVar->GetType()->IrType(*emitter));
    emitter->SetIrObject(traceEntryVar, traceEntryAlloca);
    cmajor::symbols::LocalVariableSymbol* traceGuardVar = boundFunction.GetFunctionSymbol()->TraceGuardVar();
    void* traceGuardAlloca = emitter->CreateAlloca(traceGuardVar->GetType()->IrType(*emitter));
    emitter->SetIrObject(traceGuardVar, traceGuardAlloca);
    lastAlloca = traceGuardAlloca;
    for (const auto& statement : enterCode)
    {
        statement->Accept(*this);
    }
    inSetLineOrEntryCode = prevSetLineOrEntryCode;
}

void CppCodeGenerator::SetLineNumber(int32_t lineNumber)
{
    if (prevLineNumber == lineNumber) return;
    emitter->SetCurrentSourcePos(lineNumber, 0, 0);
    prevLineNumber = lineNumber;
    cmajor::binder::BoundStatement* setLineNumberStatement = currentFunction->GetLineCode();
    if (setLineNumberStatement)
    {
        bool prevGenJumpingBoolCode = genJumpingBoolCode;
        genJumpingBoolCode = false;
        emitter->BeginSubstituteLineNumber(lineNumber);
        bool prevSetLineOrEntryCode = inSetLineOrEntryCode;
        inSetLineOrEntryCode = true;
        setLineNumberStatement->Accept(*this);
        inSetLineOrEntryCode = prevSetLineOrEntryCode;
        emitter->EndSubstituteLineNumber();
        genJumpingBoolCode = prevGenJumpingBoolCode;
    }

}

void CppCodeGenerator::SetSpan(const soul::ast::Span& span)
{
    if (!span.IsValid()) return;
    if (inSetLineOrEntryCode) return;
    fullSpan.span = span;
    SetLineNumber(cmajor::symbols::GetLineNumber(fullSpan));
}

/*

void CppCodeGenerator::GenerateEnterFunctionCode(cmajor::binder::BoundFunction& boundFunction)
{
    const std::vector<std::unique_ptr<cmajor::binder::BoundStatement>>& enterCode = boundFunction.EnterCode();
    if (enterCode.empty()) return;
    compileUnitFunctions.insert(boundFunction.GetFunctionSymbol());
    cmajor::symbols::LocalVariableSymbol* prevUnwindInfoVar = boundFunction.GetFunctionSymbol()->PrevUnwindInfoVar();
    void* prevUnwindInfoAlloca = emitter->CreateAlloca(prevUnwindInfoVar->GetType()->IrType(*emitter));
    emitter->SetIrObject(prevUnwindInfoVar, prevUnwindInfoAlloca);
    cmajor::symbols::LocalVariableSymbol* unwindInfoVar = boundFunction.GetFunctionSymbol()->UnwindInfoVar();
    void* unwindInfoAlloca = emitter->CreateAlloca(unwindInfoVar->GetType()->IrType(*emitter));
    emitter->SetIrObject(unwindInfoVar, unwindInfoAlloca);
    lastAlloca = unwindInfoAlloca;
    for (const auto& statement : enterCode)
    {
        statement->Accept(*this);
    }
}

void CppCodeGenerator::GenerateExitFunctionCode(cmajor::binder::BoundFunction& boundFunction)
{
    const std::vector<std::unique_ptr<cmajor::binder::BoundStatement>>& exitCode = boundFunction.ExitCode();
    if (exitCode.empty()) return;
    emitter->BeginInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::exitCode));
    for (const auto& statement : exitCode)
    {
        statement->Accept(*this);
    }
    emitter->EndInstructionFlag(static_cast<int16_t>(cmajor::debug::InstructionFlags::exitCode));
}

void CppCodeGenerator::GenerateInitUnwindInfoFunction(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    cmajor::symbols::FunctionSymbol* initUnwindInfoFunctionSymbol = boundCompileUnit.GetInitUnwindInfoFunctionSymbol();
    if (!initUnwindInfoFunctionSymbol) return;
    if (compileUnitFunctions.empty()) return;
    cmajor::symbols::FunctionSymbol* addCompileUnitFunctionSymbol = boundCompileUnit.GetSystemRuntimeAddCompileUnitFunctionSymbol();
    if (!addCompileUnitFunctionSymbol) return;
    void* functionType = initUnwindInfoFunctionSymbol->IrType(*emitter);
    void* function = emitter->GetOrInsertFunction(util::ToUtf8(initUnwindInfoFunctionSymbol->MangledName()), functionType, true);
    emitter->SetFunction(function, -1, util::nil_uuid(), util::nil_uuid());
    emitter->SetFunctionName(util::ToUtf8(initUnwindInfoFunctionSymbol->FullName()));
    void* entryBlock = emitter->CreateBasicBlock("entry");
    emitter->SetCurrentBasicBlock(entryBlock);
    for (cmajor::symbols::FunctionSymbol* compileUnitFunction : compileUnitFunctions)
    {
        std::unique_ptr<cmajor::binder::BoundFunctionCall> boundFunctionCall(new cmajor::binder::BoundFunctionCall(
            compileUnitFunction->GetSpan(), addCompileUnitFunctionSymbol));
        cmajor::binder::BoundBitCast* functionPtrAsVoidPtr = new cmajor::binder::BoundBitCast(std::unique_ptr<cmajor::binder::BoundExpression>(
            new cmajor::binder::BoundFunctionPtr(compileUnitFunction->GetSpan(), compileUnitFunction, 
                symbolTable->GetTypeByName(U"void")->AddPointer())),
            symbolTable->GetTypeByName(U"void")->AddPointer());
        boundFunctionCall->AddArgument(std::unique_ptr<cmajor::binder::BoundExpression>(functionPtrAsVoidPtr));
        std::string functionName = util::ToUtf8(compileUnitFunction->FullName());
        int functionNameStringId = Install(functionName);
        cmajor::binder::BoundLiteral* boundFunctionNameLiteral = new cmajor::binder::BoundLiteral(
            std::unique_ptr<cmajor::symbols::Value>(new cmajor::symbols::StringValue(compileUnitFunction->GetSpan(),
            functionNameStringId, functionName)),
            symbolTable->GetTypeByName(U"char")->AddConst()->AddPointer());
        boundFunctionCall->AddArgument(std::unique_ptr<cmajor::binder::BoundExpression>(boundFunctionNameLiteral));
        std::string sourceFilePath = GetSourceFilePath(compileUnitFunction->ModuleId());
        int sourceFilePathStringId = Install(sourceFilePath);
        cmajor::binder::BoundLiteral* boundSourceFilePathLiteral = new cmajor::binder::BoundLiteral(std::unique_ptr<cmajor::symbols::Value>(
            new cmajor::symbols::StringValue(compileUnitFunction->GetSpan(), sourceFilePathStringId, sourceFilePath)),
            symbolTable->GetTypeByName(U"char")->AddConst()->AddPointer());
        boundFunctionCall->AddArgument(std::unique_ptr<cmajor::binder::BoundExpression>(boundSourceFilePathLiteral));
        boundFunctionCall->Accept(*this);
    }
    emitter->CreateRetVoid();
}

void CppCodeGenerator::GenerateInitCompileUnitFunction(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    generateLineNumbers = false;
    emitter->SetCurrentSourcePos(0, 0, 0);
    cmajor::symbols::FunctionSymbol* initCompileUnitFunctionSymbol = boundCompileUnit.GetInitCompileUnitFunctionSymbol();
    if (!initCompileUnitFunctionSymbol) return;
    soul::ast::Span span = initCompileUnitFunctionSymbol->GetSpan();
    util::uuid moduleId = initCompileUnitFunctionSymbol->ModuleId();
    void* functionType = initCompileUnitFunctionSymbol->IrType(*emitter);
    void* function = emitter->GetOrInsertFunction(util::ToUtf8(initCompileUnitFunctionSymbol->MangledName()), functionType, true);
    emitter->SetFunction(function, -1, util::nil_uuid(), util::nil_uuid());
    emitter->SetFunctionName(util::ToUtf8(initCompileUnitFunctionSymbol->FullName()));
    void* entryBlock = emitter->CreateBasicBlock("entry");
    emitter->SetCurrentBasicBlock(entryBlock);
    cmajor::symbols::FunctionSymbol* initUnwindInfoFunctionSymbol = boundCompileUnit.GetInitUnwindInfoFunctionSymbol();
    if (!initUnwindInfoFunctionSymbol)
    {
        emitter->CreateRetVoid();
        return;
    }
    cmajor::symbols::FunctionSymbol* pushCompileUnitUnwindInfoInitFunctionSymbol = boundCompileUnit.GetPushCompileUnitUnwindInfoInitFunctionSymbol();
    cmajor::symbols::TypeSymbol* initUnwindInfoDelegateType = boundCompileUnit.GetInitUnwindInfoDelegateType();
    cmajor::symbols::GlobalVariableSymbol* compileUnitUnwindInfoVarSymbol = boundCompileUnit.GetCompileUnitUnwindInfoVarSymbol();
    cmajor::binder::BoundGlobalVariable* boundCompileUnitUnwindInfoVar = new cmajor::binder::BoundGlobalVariable(span, compileUnitUnwindInfoVarSymbol);
    cmajor::binder::BoundAddressOfExpression* unwindInfoVarAddress = new cmajor::binder::BoundAddressOfExpression(std::unique_ptr<cmajor::binder::BoundExpression>(
        boundCompileUnitUnwindInfoVar), boundCompileUnitUnwindInfoVar->GetType()->AddPointer());
    cmajor::binder::BoundFunctionPtr* boundInitUnwindInfoFunction = new cmajor::binder::BoundFunctionPtr(span, initUnwindInfoFunctionSymbol, 
        initUnwindInfoDelegateType);
    std::unique_ptr<cmajor::binder::BoundFunctionCall> boundFunctionCall(new cmajor::binder::BoundFunctionCall(span, pushCompileUnitUnwindInfoInitFunctionSymbol));
    boundFunctionCall->AddArgument(std::unique_ptr<cmajor::binder::BoundExpression>(boundInitUnwindInfoFunction));
    boundFunctionCall->AddArgument(std::unique_ptr<cmajor::binder::BoundExpression>(unwindInfoVarAddress));
    boundFunctionCall->Accept(*this);
    emitter->CreateRetVoid();
}

void CppCodeGenerator::GenerateGlobalInitFunction(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    generateLineNumbers = false;
    emitter->SetCurrentSourcePos(0, 0, 0);
    cmajor::symbols::FunctionSymbol* globalInitFunctionSymbol = boundCompileUnit.GetGlobalInitializationFunctionSymbol();
    if (!globalInitFunctionSymbol) return;
    soul::ast::Span span = globalInitFunctionSymbol->GetSpan();
    util::uuid moduleId = globalInitFunctionSymbol->ModuleId();
    void* functionType = globalInitFunctionSymbol->IrType(*emitter);
    void* function = emitter->GetOrInsertFunction(util::ToUtf8(globalInitFunctionSymbol->MangledName()), functionType, true);
    emitter->SetFunction(function, -1, util::nil_uuid(), util::nil_uuid());
    emitter->SetFunctionName(util::ToUtf8(globalInitFunctionSymbol->FullName()));
    void* entryBlock = emitter->CreateBasicBlock("entry");
    emitter->SetCurrentBasicBlock(entryBlock);
    const std::vector<std::unique_ptr<cmajor::symbols::FunctionSymbol>>& allCompileUnitInitFunctionSymbols = boundCompileUnit.AllCompileUnitInitFunctionSymbols();
    for (const std::unique_ptr<cmajor::symbols::FunctionSymbol>& initCompileUnitFunctionSymbol : allCompileUnitInitFunctionSymbols)
    {
        std::unique_ptr<cmajor::binder::BoundFunctionCall> boundFunctionCall(new cmajor::binder::BoundFunctionCall(span, initCompileUnitFunctionSymbol.get()));
        boundFunctionCall->Accept(*this);
    }
    emitter->CreateRetVoid();
}
*/

} // namespace cmajor::cpp::backend::codegen
