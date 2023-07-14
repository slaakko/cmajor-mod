// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.systemx.backend.codegen;

import cmajor.systemx.intermediate;
import cmajor.systemx.assembler;
import util;

namespace cmajor::systemx::backend {

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

SystemXCodeGenerator::SystemXCodeGenerator(cmajor::ir::Emitter* emitter_) : 
    emitter(emitter_), symbolTable(nullptr), module(nullptr), compileUnit(nullptr),
    nativeCompileUnit(nullptr), function(nullptr), entryBasicBlock(nullptr), lastInstructionWasRet(false), destructorCallGenerated(false), genJumpingBoolCode(false),
    trueBlock(nullptr), falseBlock(nullptr), breakTarget(nullptr), continueTarget(nullptr), sequenceSecond(nullptr), currentFunction(nullptr), currentBlock(nullptr),
    breakTargetBlock(nullptr), continueTargetBlock(nullptr), lastAlloca(nullptr), currentClass(nullptr), basicBlockOpen(false), defaultDest(nullptr), currentCaseMap(nullptr),
    generateLineNumbers(false), currentTryBlockId(-1), nextTryBlockId(0), currentTryNextBlock(nullptr), handlerBlock(nullptr), cleanupBlock(nullptr), newCleanupNeeded(false),
    inTryBlock(false), prevWasTerminator(false)
{
    emitter->SetEmittingDelegate(this);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    std::string intermediateFilePath = util::Path::ChangeExtension(boundCompileUnit.ObjectFilePath(), ".i");
    NativeModule nativeModule(emitter, intermediateFilePath);
    compileUnitId = boundCompileUnit.Id();
    emitter->SetCompileUnitId(compileUnitId);
    generateLineNumbers = false;
    symbolTable = &boundCompileUnit.GetSymbolTable();
    module = &boundCompileUnit.GetModule();
    compileUnit = &boundCompileUnit;
    nativeCompileUnit = static_cast<cmajor::systemx::ir::CompileUnit*>(nativeModule.module);
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
    cmajor::systemx::intermediate::Context intermediateContext;
    cmajor::systemx::intermediate::Parse(boundCompileUnit.GetModule().LogStreamId(), intermediateFilePath, intermediateContext, 
        cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
    cmajor::systemx::intermediate::Verify(intermediateContext);
    std::string pass = cmajor::symbols::Pass();
    if (pass.empty())
    {
        int optimizationLevel = emitter->EmittingContext()->OptimizationLevel();
        pass = "opt-" + std::to_string(optimizationLevel);
    }
    cmajor::systemx::intermediate::PassManager::Instance().Run(boundCompileUnit.GetModule().LogStreamId(), &intermediateContext, pass, 
        cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
    std::string objectFilePath = boundCompileUnit.ObjectFilePath();
    std::string assemblyFilePath = util::Path::ChangeExtension(objectFilePath, ".s");
    cmajor::systemx::assembler::Assemble(boundCompileUnit.GetModule().LogStreamId(), assemblyFilePath, objectFilePath, 
        cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundNamespace& boundNamespace)
{
    int n = boundNamespace.Members().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundNode* node = boundNamespace.Members()[i].get();
        node->Accept(*this);
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundClass& boundClass)
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

void SystemXCodeGenerator::Visit(cmajor::binder::BoundFunction& boundFunction)
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
    newCleanupNeeded = false;
    labeledStatementMap.clear();
    cleanups.clear();
    if (functionSymbol->HasSource())
    {
        generateLineNumbers = true;
        emitter->SetCurrentSourcePos(boundFunction.Body()->GetSourcePos().line, 0, 0); 
    }
    else
    {
        generateLineNumbers = false;
        emitter->SetCurrentSourcePos(0, 0, 0); 
    }
    function = emitter->GetOrInsertFunction(util::ToUtf8(functionSymbol->MangledName()), functionType, functionSymbol->DontThrow());
    if (functionSymbol->HasSource())
    {
        void* mdStruct = emitter->CreateMDStruct();
        emitter->AddMDItem(mdStruct, "nodeType", emitter->CreateMDLong(funcInfoNodeType));
        emitter->AddMDItem(mdStruct, "fullName", emitter->CreateMDString(util::ToUtf8(functionSymbol->FullName())));
        void* mdFile = emitter->GetMDStructRefForSourceFile(module->GetFilePath(functionSymbol->GetSourcePos().file));
        emitter->AddMDItem(mdStruct, "sourceFile", mdFile);
        int mdId = emitter->GetMDStructId(mdStruct);
        emitter->SetFunctionMdId(function, mdId);
    }
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
    int32_t fileIndex = -1;
    util::uuid functionId;
    if (functionSymbol->HasSource())
    {
        fileIndex = functionSymbol->GetSourcePos().file;
        functionId = functionSymbol->FunctionId();
    }
    emitter->SetFunction(function, fileIndex, functionSymbol->SourceModuleId(), functionId);
    void* entryBlock = emitter->CreateBasicBlock("entry");
    entryBasicBlock = entryBlock;
    emitter->SetCurrentBasicBlock(entryBlock);
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
        lastAlloca = allocaInst;
    }
    if (functionSymbol->ReturnParam())
    {
        cmajor::symbols::ParameterSymbol* parameter = functionSymbol->ReturnParam();
        void* allocaInst = emitter->CreateAlloca(parameter->GetType()->IrType(*emitter));
        emitter->SetIrObject(parameter, allocaInst);
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
            copyConstructor->GenerateCall(*emitter, copyCtorArgs, cmajor::ir::OperationFlags::none, boundFunction.Body()->GetSourcePos(), boundFunction.Body()->ModuleId());
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
            paramValue.SetType(interfaceType->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
            copyCtorArgs.push_back(&paramValue);
            cmajor::ir::NativeValue argumentValue(arg);
            argumentValue.SetType(interfaceType->AddPointer(soul::ast::SourcePos(), util::nil_uuid()));
            copyCtorArgs.push_back(&argumentValue);
            copyConstructor->GenerateCall(*emitter, copyCtorArgs, cmajor::ir::OperationFlags::none, boundFunction.Body()->GetSourcePos(), boundFunction.Body()->ModuleId());
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
            emitter->CreateRet(defaultValue);
            lastInstructionWasRet = true;
        }
        else
        {
            emitter->CreateRetVoid();
            lastInstructionWasRet = true;
        }
    }
    GenerateCodeForCleanups();
    emitter->FinalizeFunction(function, functionSymbol->HasCleanup());
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundCompoundStatement& boundCompoundStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundCompoundStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundCompoundStatement);
    cmajor::binder::BoundCompoundStatement* prevBlock = currentBlock;
    currentBlock = &boundCompoundStatement;
    blockDestructionMap[currentBlock] = std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>();
    blocks.push_back(currentBlock);
    int n = boundCompoundStatement.Statements().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundStatement* statement = boundCompoundStatement.Statements()[i].get();
        statement->Accept(*this);
    }
    ExitBlocks(prevBlock);
    blocks.pop_back();
    currentBlock = prevBlock;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundSequenceStatement& boundSequenceStatement)
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

void SystemXCodeGenerator::Visit(cmajor::binder::BoundReturnStatement& boundReturnStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundReturnStatement.GetSourcePos().line, 0, 0);
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
        emitter->CreateRet(returnValue);
        lastInstructionWasRet = true;
    }
    else
    {
        ExitBlocks(nullptr);
        emitter->CreateRetVoid();
        lastInstructionWasRet = true;
    }
    cmajor::binder::BoundCompoundStatement* body = currentFunction->Body();
    cmajor::binder::BoundStatement* lastStatement = nullptr;
    if (!body->Statements().empty())
    {
        lastStatement = body->Statements().back().get();
    }
    if (lastStatement && lastStatement != &boundReturnStatement)
    {
        void* nextBlock = emitter->CreateBasicBlock("next");
        emitter->SetCurrentBasicBlock(nextBlock);
        basicBlockOpen = true;
        lastInstructionWasRet = false;
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundGotoCaseStatement& boundGotoCaseStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundGotoCaseStatement.GetSourcePos().line, 0, 0);
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
        emitter->CreateBr(caseDest);
    }
    else
    {
        throw cmajor::symbols::Exception("case not found", boundGotoCaseStatement.GetSourcePos(), boundGotoCaseStatement.ModuleId());
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundGotoDefaultStatement& boundGotoDefaultStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundGotoDefaultStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundGotoDefaultStatement);
    Assert(breakTargetBlock, "break target not set");
    ExitBlocks(breakTargetBlock);
    if (defaultDest)
    {
        emitter->CreateBr(defaultDest);
    }
    else
    {
        throw cmajor::symbols::Exception("no default destination", boundGotoDefaultStatement.GetSourcePos(), boundGotoDefaultStatement.ModuleId());
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundBreakStatement& boundBreakStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundBreakStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundBreakStatement);
    Assert(breakTarget && breakTargetBlock, "break target not set");
    ExitBlocks(breakTargetBlock);
    emitter->CreateBr(breakTarget);
    if (!currentCaseMap) // not in switch
    {
        void* nextBlock = emitter->CreateBasicBlock("next");
        emitter->SetCurrentBasicBlock(nextBlock);
        basicBlockOpen = true;
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundContinueStatement& boundContinueStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundContinueStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    prevWasTerminator = false;
    SetTarget(&boundContinueStatement);
    Assert(continueTarget && continueTargetBlock, "continue target not set");
    ExitBlocks(continueTargetBlock);
    emitter->CreateBr(continueTarget);
    void* nextBlock = emitter->CreateBasicBlock("next");
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundGotoStatement& boundGotoStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundGotoStatement.GetSourcePos().line, 0, 0);
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
        emitter->CreateBr(target);
    }
    else
    {
        throw cmajor::symbols::Exception("goto target not found", boundGotoStatement.GetSourcePos(), boundGotoStatement.ModuleId());
    }
    void* nextBlock = emitter->CreateBasicBlock("next");
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundIfStatement& boundIfStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundIfStatement.GetSourcePos().line, 0, 0);
    }
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
    boundIfStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    boundIfStatement.ThenS()->Accept(*this);
    emitter->CreateBr(nextBlock);
    if (boundIfStatement.ElseS())
    {
        emitter->SetCurrentBasicBlock(falseBlock);
        boundIfStatement.ElseS()->Accept(*this);
        emitter->CreateBr(nextBlock);
    }
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundWhileStatement& boundWhileStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundWhileStatement.GetSourcePos().line, 0, 0);
    }
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
    boundWhileStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    boundWhileStatement.Statement()->Accept(*this);
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(falseBlock);
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    continueTarget = prevContinueTarget;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundDoStatement& boundDoStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundDoStatement.GetSourcePos().line, 0, 0);
    }
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
    boundDoStatement.Statement()->Accept(*this);
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    boundDoStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(falseBlock);
    basicBlockOpen = true;
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    continueTarget = prevContinueTarget;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundForStatement& boundForStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundForStatement.GetSourcePos().line, 0, 0);
    }
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
    boundForStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(actionBlock);
    boundForStatement.ActionS()->Accept(*this);
    emitter->CreateBr(loopBlock);
    emitter->SetCurrentBasicBlock(loopBlock);
    boundForStatement.LoopS()->Accept(*this);
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(falseBlock);
    basicBlockOpen = true;
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    continueTarget = prevContinueTarget;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundSwitchStatement& boundSwitchStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundSwitchStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundSwitchStatement);
    void* prevBreakTarget = breakTarget;
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    breakTargetBlock = currentBlock;
    boundSwitchStatement.Condition()->Accept(*this);
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
        caseS->Accept(*this);
        if (basicBlockOpen)
        {
            emitter->CreateBr(next);
            basicBlockOpen = false;
        }
    }
    if (boundSwitchStatement.DefaultStatement())
    {
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

void SystemXCodeGenerator::Visit(cmajor::binder::BoundCaseStatement& boundCaseStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundCaseStatement.GetSourcePos().line, 0, 0);
    }
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
            if (boundCaseStatement.CompoundStatement())
            {
                boundCaseStatement.CompoundStatement()->Accept(*this);
            }
        }
        else
        {
            throw cmajor::symbols::Exception("case not found", boundCaseStatement.GetSourcePos(), boundCaseStatement.ModuleId());
        }
    }
    else
    {
        throw cmajor::symbols::Exception("no cases", boundCaseStatement.GetSourcePos(), boundCaseStatement.ModuleId());
    }

}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundDefaultStatement& boundDefaultStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundDefaultStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundDefaultStatement);
    if (defaultDest)
    {
        emitter->SetCurrentBasicBlock(defaultDest);
        if (boundDefaultStatement.CompoundStatement())
        {
            boundDefaultStatement.CompoundStatement()->Accept(*this);
        }
    }
    else
    {
        throw cmajor::symbols::Exception("no default destination", boundDefaultStatement.GetSourcePos(), boundDefaultStatement.ModuleId());
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundConstructionStatement& boundConstructionStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundConstructionStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundConstructionStatement);
    boundConstructionStatement.ConstructorCall()->Accept(*this);
    if (!boundConstructionStatement.ConstructorCall()->GetFunctionSymbol()->IsBasicTypeOperation())
    {
        int n = boundConstructionStatement.ConstructorCall()->Arguments().size();
        if (n > 0)
        {
            const std::unique_ptr<cmajor::binder::BoundExpression>& firstArgument = boundConstructionStatement.ConstructorCall()->Arguments()[0];
            cmajor::symbols::TypeSymbol* firstArgumentBaseType = firstArgument->GetType()->BaseType();
            if (firstArgumentBaseType->IsClassTypeSymbol())
            {
                if (firstArgument->GetType()->IsPointerType() && firstArgument->GetType()->RemovePointer(boundConstructionStatement.GetSourcePos(), boundConstructionStatement.ModuleId())->IsClassTypeSymbol())
                {
                    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(firstArgumentBaseType);
                    if (classType->Destructor())
                    {
                        newCleanupNeeded = true;
                        std::unique_ptr<cmajor::binder::BoundExpression> classPtrArgument(firstArgument->Clone());
                        std::unique_ptr<cmajor::binder::BoundFunctionCall> destructorCall(new cmajor::binder::BoundFunctionCall(currentBlock->EndSourcePos(), currentBlock->ModuleId(), classType->Destructor()));
                        destructorCall->AddArgument(std::move(classPtrArgument));
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
                    }
                }
            }
        }
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundAssignmentStatement& boundAssignmentStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundAssignmentStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundAssignmentStatement);
    boundAssignmentStatement.AssignmentCall()->Accept(*this);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundExpressionStatement& boundExpressionStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundExpressionStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundExpressionStatement);
    boundExpressionStatement.Expression()->Accept(*this);
    if (boundExpressionStatement.Expression()->HasValue())
    {
        emitter->Stack().Pop();
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundInitializationStatement& boundInitializationStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundInitializationStatement.GetSourcePos().line, 0, 0);
    }
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

void SystemXCodeGenerator::Visit(cmajor::binder::BoundEmptyStatement& boundEmptyStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundEmptyStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundEmptyStatement);
    // todo
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundSetVmtPtrStatement& boundSetVmtPtrStatement)
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

void SystemXCodeGenerator::Visit(cmajor::binder::BoundThrowStatement& boundThrowStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundThrowStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundThrowStatement);
    boundThrowStatement.ThrowCallExpr()->Accept(*this);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundTryStatement& boundTryStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundTryStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundTryStatement);
    void* prevHandlerBlock = handlerBlock;
    void* prevCleanupBlock = cleanupBlock;
    handlerBlock = emitter->CreateBasicBlock("handlers");
    cleanupBlock = nullptr;
    int64_t parentTryBlockId = currentTryBlockId;
    currentTryBlockId = nextTryBlockId++;
    void* nop1 = emitter->CreateNop();
    void* beginTry = emitter->CreateMDStruct();
    emitter->AddMDItem(beginTry, "nodeType", emitter->CreateMDLong(beginTryNodeType));
    emitter->AddMDItem(beginTry, "tryBlockId", emitter->CreateMDLong(currentTryBlockId));
    emitter->AddMDItem(beginTry, "parentTryBlockId", emitter->CreateMDLong(parentTryBlockId));
    int beginTryId = emitter->GetMDStructId(beginTry);
    void* beginTryMdRef = emitter->CreateMDStructRef(beginTryId);
    emitter->SetMetadataRef(nop1, beginTryMdRef);
    bool prevInTryBlock = inTryBlock;
    inTryBlock = true;
    boundTryStatement.TryBlock()->Accept(*this);
    inTryBlock = prevInTryBlock;
    void* nop2 = emitter->CreateNop();
    void* endTry = emitter->CreateMDStruct();
    emitter->AddMDItem(endTry, "nodeType", emitter->CreateMDLong(endTryNodeType));
    emitter->AddMDItem(endTry, "tryBlockId", emitter->CreateMDLong(currentTryBlockId));
    int endTryId = emitter->GetMDStructId(endTry);
    void* endTryMdRef = emitter->CreateMDStructRef(endTryId);
    emitter->SetMetadataRef(nop2, endTryMdRef);
    void* tryNextBlock = emitter->CreateBasicBlock("tryNext");
    emitter->CreateBr(tryNextBlock);
    void* prevTryNextBlock = currentTryNextBlock;
    currentTryNextBlock = tryNextBlock;
    handlerBlock = prevHandlerBlock;
    for (const auto& c : boundTryStatement.Catches())
    {
        c->Accept(*this);
    }
    emitter->SetCurrentBasicBlock(tryNextBlock);
    currentTryBlockId = parentTryBlockId;
    currentTryNextBlock = prevTryNextBlock;
    cleanupBlock = prevCleanupBlock;
    basicBlockOpen = true;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundCatchStatement& boundCatchStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundCatchStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundCatchStatement);
    void* catchBlock = emitter->CreateBasicBlock("catch");
    emitter->SetCurrentBasicBlock(catchBlock);
    void* nop1 = emitter->CreateNop();
    void* catch_ = emitter->CreateMDStruct();
    emitter->AddMDItem(catch_, "nodeType", emitter->CreateMDLong(catchNodeType));
    emitter->AddMDItem(catch_, "tryBlockId", emitter->CreateMDLong(currentTryBlockId));
    emitter->AddMDItem(catch_, "catchBlockId", emitter->CreateMDBasicBlockRef(catchBlock));
    const util::uuid& uuid = compileUnit->GetUuid(boundCatchStatement.CatchTypeUuidId());
    std::string uuidStr = util::ToString(uuid);
    emitter->AddMDItem(catch_, "caughtTypeId", emitter->CreateMDString(uuidStr));
    int catchId = emitter->GetMDStructId(catch_);
    void* catchMdRef = emitter->CreateMDStructRef(catchId);
    emitter->SetMetadataRef(nop1, catchMdRef);
    boundCatchStatement.CatchBlock()->Accept(*this);
    emitter->CreateBr(currentTryNextBlock);
    emitter->SetCurrentBasicBlock(currentTryNextBlock);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundRethrowStatement& boundRethrowStatement)
{
    if (generateLineNumbers)
    {
        emitter->SetCurrentSourcePos(boundRethrowStatement.GetSourcePos().line, 0, 0);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundRethrowStatement);
    void* resumeFunctionType = emitter->GetIrTypeForFunction(emitter->GetIrTypeForVoid(), std::vector<void*>());
    void* callee = emitter->GetOrInsertFunction("resume", resumeFunctionType, false);
    emitter->CreateCall(resumeFunctionType, callee, std::vector<void*>());
    if (currentFunction->GetFunctionSymbol()->ReturnType() && currentFunction->GetFunctionSymbol()->ReturnType()->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol &&
        !currentFunction->GetFunctionSymbol()->ReturnsClassInterfaceOrClassDelegateByValue())
    {
        void* defaultValue = currentFunction->GetFunctionSymbol()->ReturnType()->CreateDefaultIrValue(*emitter);
        emitter->CreateRet(defaultValue);
        lastInstructionWasRet = true;
    }
    else
    {
        emitter->CreateRetVoid();
        lastInstructionWasRet = true;
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundParameter& boundParameter)
{
    boundParameter.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundLocalVariable& boundLocalVariable)
{
    boundLocalVariable.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundMemberVariable& boundMemberVariable)
{
    boundMemberVariable.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundConstant& boundConstant)
{
    boundConstant.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundEnumConstant& boundEnumConstant)
{
    boundEnumConstant.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundLiteral& boundLiteral)
{
    boundLiteral.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundTemporary& boundTemporary)
{
    boundTemporary.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundSizeOfExpression& boundSizeOfExpression)
{
    boundSizeOfExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundAddressOfExpression& boundAddressOfExpression)
{
    boundAddressOfExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundDereferenceExpression& boundDereferenceExpression)
{
    boundDereferenceExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundReferenceToPointerExpression& boundReferenceToPointerExpression)
{
    boundReferenceToPointerExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundFunctionCall& boundFunctionCall)
{
    boundFunctionCall.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundDelegateCall& boundDelegateCall)
{
    boundDelegateCall.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundClassDelegateCall& boundClassDelegateCall)
{
    boundClassDelegateCall.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundConversion& boundConversion)
{
    boundConversion.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundConstructExpression& boundConstructExpression)
{
    boundConstructExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundConstructAndReturnTemporaryExpression& boundConstructAndReturnTemporaryExpression)
{
    boundConstructAndReturnTemporaryExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundClassOrClassDelegateConversionResult& boundClassOrClassDelegateConversionResult)
{
    boundClassOrClassDelegateConversionResult.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundIsExpression& boundIsExpression)
{
    boundIsExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundAsExpression& boundAsExpression)
{
    boundAsExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundTypeNameExpression& boundTypeNameExpression)
{
    boundTypeNameExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundBitCast& boundBitCast)
{
    boundBitCast.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundFunctionPtr& boundFunctionPtr)
{
    boundFunctionPtr.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundDisjunction& boundDisjunction)
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

void SystemXCodeGenerator::Visit(cmajor::binder::BoundConjunction& boundConjunction)
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

void SystemXCodeGenerator::Visit(cmajor::binder::BoundGlobalVariable& boundGlobalVariable)
{
    cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol = boundGlobalVariable.GetGlobalVariableSymbol();
    globalVariableSymbol->CreateIrObject(*emitter);
}

void SystemXCodeGenerator::GenJumpingBoolCode()
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

void SystemXCodeGenerator::SetTarget(cmajor::binder::BoundStatement* labeledStatement)
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
        throw cmajor::symbols::Exception("target for labeled statement not found", labeledStatement->GetSourcePos(), labeledStatement->ModuleId());
    }
}

void SystemXCodeGenerator::ExitBlocks(cmajor::binder::BoundCompoundStatement* targetBlock)
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
    if (lastStatement && currentBlockLastStatement && lastStatement == currentBlockLastStatement &&
        currentBlockLastStatement->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundReturnStatement)
    {
        createBasicBlock = true;
    }
    if (currentBlockLastStatement && currentBlockLastStatement->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundRethrowStatement)
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
                    newCleanupNeeded = true;
                }
            }
        }
    }
}

void* SystemXCodeGenerator::GetGlobalStringPtr(int stringId)
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

void* SystemXCodeGenerator::GetGlobalWStringConstant(int stringId, void*& arrayType)
{
    auto it = utf16stringMap.find(stringId);
    if (it != utf16stringMap.cend())
    {
        return it->second;
    }
    else
    {
        const std::u16string& str = compileUnit->GetUtf16String(stringId);
        uint64_t length = str.length();
        std::vector<void*> wcharConstants;
        for (char16_t c : str)
        {
            wcharConstants.push_back(emitter->CreateIrValueForUShort(static_cast<uint16_t>(c)));
        }
        wcharConstants.push_back(emitter->CreateIrValueForUShort(static_cast<uint16_t>(0)));
        void* arrayType = emitter->GetIrTypeForArrayType(emitter->GetIrTypeForUShort(), length + 1);
        void* stringObject = emitter->GetOrInsertGlobal("wstring" + std::to_string(stringId) + "_" + compileUnitId, emitter->GetIrTypeForUShort());
        void* stringGlobal = stringObject;
        emitter->SetPrivateLinkage(stringGlobal);
        void* constant = emitter->CreateIrValueForConstantArray(arrayType, wcharConstants, "w");
        emitter->SetInitializer(stringGlobal, constant);
        void* stringValue = stringGlobal;
        utf16stringMap[stringId] = stringValue;
        return stringValue;
    }
}

void* SystemXCodeGenerator::GetGlobalUStringConstant(int stringId, void*& arrayType)
{
    auto it = utf32stringMap.find(stringId);
    if (it != utf32stringMap.cend())
    {
        return it->second;
    }
    else
    {
        const std::u32string& str = compileUnit->GetUtf32String(stringId);
        uint64_t length = str.length();
        std::vector<void*> ucharConstants;
        for (char32_t c : str)
        {
            ucharConstants.push_back(emitter->CreateIrValueForUInt(static_cast<uint32_t>(c)));
        }
        ucharConstants.push_back(emitter->CreateIrValueForUInt(static_cast<uint32_t>(0)));
        void* arrayType = emitter->GetIrTypeForArrayType(emitter->GetIrTypeForUInt(), length + 1);
        void* stringObject = emitter->GetOrInsertGlobal("ustring" + std::to_string(stringId) + "_" + compileUnitId, emitter->GetIrTypeForUInt());
        void* stringGlobal = stringObject;
        emitter->SetPrivateLinkage(stringGlobal);
        void* constant = emitter->CreateIrValueForConstantArray(arrayType, ucharConstants, "u");
        emitter->SetInitializer(stringGlobal, constant);
        void* stringValue = stringGlobal;
        utf32stringMap[stringId] = stringValue;
        return stringValue;
    }
}

void* SystemXCodeGenerator::GetGlobalUuidConstant(int uuidId)
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
        void* uuidObject = emitter->GetOrInsertGlobal("uuid" + std::to_string(uuidId) + "_" + compileUnitId, emitter->GetIrTypeForByte());
        void* uuidGlobal = uuidObject;
        emitter->SetPrivateLinkage(uuidGlobal);
        void* constant = emitter->CreateIrValueForConstantArray(arrayType, byteConstants, "b");
        emitter->SetInitializer(uuidGlobal, constant);
        void* uuidValue = uuidGlobal;
        uuidMap[uuidId] = uuidValue;
        return uuidValue;
    }
}

void* SystemXCodeGenerator::HandlerBlock()
{
    return handlerBlock;
}

void* SystemXCodeGenerator::CleanupBlock()
{
    return cleanupBlock;
}

bool SystemXCodeGenerator::NewCleanupNeeded()
{
    return newCleanupNeeded;
}

bool SystemXCodeGenerator::InTryBlock() const
{
    return inTryBlock;
}

int SystemXCodeGenerator::CurrentTryBlockId() const
{
    return currentTryBlockId;
}

int SystemXCodeGenerator::Install(const std::string& str)
{
    return compileUnit->Install(str);
}

int SystemXCodeGenerator::Install(const std::u16string& str)
{
    return compileUnit->Install(str);
}

int SystemXCodeGenerator::Install(const std::u32string& str)
{
    return compileUnit->Install(str);
}

void SystemXCodeGenerator::CreateCleanup()
{
    cleanupBlock = emitter->CreateBasicBlock("cleanup");
    cmajor::binder::BoundCompoundStatement* targetBlock = nullptr;
    cmajor::binder::BoundStatement* parent = currentBlock->Parent();
    while (parent && parent->GetBoundNodeType() != cmajor::binder::BoundNodeType::boundTryStatement)
    {
        parent = parent->Parent();
    }
    if (parent)
    {
        targetBlock = parent->Block();
    }
    Cleanup* cleanup = new Cleanup(cleanupBlock);
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
                    cleanup->destructors.push_back(std::unique_ptr<cmajor::binder::BoundFunctionCall>(static_cast<cmajor::binder::BoundFunctionCall*>(destructorCall->Clone())));
                }
            }
        }
    }
    cleanups.push_back(std::unique_ptr<Cleanup>(cleanup));
    newCleanupNeeded = false;
}

void SystemXCodeGenerator::GenerateCodeForCleanups()
{
    for (const std::unique_ptr<Cleanup>& cleanup : cleanups)
    {
        emitter->SetCurrentBasicBlock(cleanup->cleanupBlock);
        for (const std::unique_ptr<cmajor::binder::BoundFunctionCall>& destructorCall : cleanup->destructors)
        {
            destructorCall->Accept(*this);
        }
        void* resumeFunctionType = emitter->GetIrTypeForFunction(emitter->GetIrTypeForVoid(), std::vector<void*>());
        void* callee = emitter->GetOrInsertFunction("resume", resumeFunctionType, false);
        emitter->CreateCall(resumeFunctionType, callee, std::vector<void*>());
        if (currentFunction->GetFunctionSymbol()->ReturnType() && currentFunction->GetFunctionSymbol()->ReturnType()->GetSymbolType() != cmajor::symbols::SymbolType::voidTypeSymbol &&
            !currentFunction->GetFunctionSymbol()->ReturnsClassInterfaceOrClassDelegateByValue())
        {
            void* defaultValue = currentFunction->GetFunctionSymbol()->ReturnType()->CreateDefaultIrValue(*emitter);
            emitter->CreateRet(defaultValue);
            lastInstructionWasRet = true;
        }
        else
        {
            emitter->CreateRetVoid();
            lastInstructionWasRet = true;
        }
    }
}

} // namespace cmajor::systemx::backend
