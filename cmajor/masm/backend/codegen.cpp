// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.masm.backend.codegen;

import cmajor.masm.intermediate;
import cmajor.masm.optimizer;
import cmajor.masm.assembly;
import util;

namespace cmajor::masm::backend {

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

MasmCodeGenerator::MasmCodeGenerator(cmajor::ir::Emitter* emitter_) : emitter(emitter_), symbolTable(nullptr), module(nullptr), compileUnit(nullptr), nativeCompileUnit(nullptr), 
    fileIndex(-1), generateLineNumbers(false), currentClass(nullptr), currentFunction(nullptr), function(nullptr), entryBasicBlock(nullptr), lastInstructionWasRet(false),
    prevWasTerminator(false), destructorCallGenerated(false), genJumpingBoolCode(false), trueBlock(nullptr), falseBlock(nullptr), breakTarget(nullptr), continueTarget(nullptr),
    basicBlockOpen(false), lastAlloca(nullptr), currentTryNextBlock(nullptr), handlerBlock(nullptr), cleanupBlock(nullptr), newCleanupNeeded(false), inTryBlock(false),
    sequenceSecond(nullptr), currentBlock(nullptr), breakTargetBlock(nullptr), continueTargetBlock(nullptr), defaultDest(nullptr),
    currentTryBlockId(-1), nextTryBlockId(0), currentCaseMap(nullptr), latestRet(nullptr), prevLineNumber(0), inSetLineOrEntryCode(false)
{
    emitter->SetEmittingDelegate(this);
}

void MasmCodeGenerator::GenJumpingBoolCode()
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

void MasmCodeGenerator::SetTarget(cmajor::binder::BoundStatement* labeledStatement)
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

void MasmCodeGenerator::ExitBlocks(cmajor::binder::BoundCompoundStatement* targetBlock)
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
                block->SetDestroyed();
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
                    bool prevSetLineOrEntryCode = inSetLineOrEntryCode;
                    inSetLineOrEntryCode = true;
                    destructorCall->Accept(*this);
                    inSetLineOrEntryCode = prevSetLineOrEntryCode;
                    destructorCallGenerated = true;
                    newCleanupNeeded = true;
                }
            }
        }
    }
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    fileIndex = boundCompileUnit.FileIndex();
    std::string intermediateFilePath = util::Path::ChangeExtension(boundCompileUnit.ObjectFilePath(), ".i");
    std::string optimizedIntermediateFilePath = util::Path::ChangeExtension(boundCompileUnit.ObjectFilePath(), ".opt.i");
    NativeModule nativeModule(emitter, intermediateFilePath);
    compileUnitId = boundCompileUnit.Id();
    emitter->SetCompileUnitId(compileUnitId);
    generateLineNumbers = false;
    symbolTable = &boundCompileUnit.GetSymbolTable();
    module = &boundCompileUnit.GetModule();
    compileUnit = &boundCompileUnit;
    nativeCompileUnit = static_cast<cmajor::masm::ir::CompileUnit*>(nativeModule.module);
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
    cmajor::masm::intermediate::Context intermediateContext;
    cmajor::masm::intermediate::Context optimizationContext;
    cmajor::masm::intermediate::Context* finalContext = &intermediateContext;
    cmajor::masm::intermediate::Parse(boundCompileUnit.GetModule().LogStreamId(), intermediateFilePath, intermediateContext,
        cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
    cmajor::masm::intermediate::Verify(intermediateContext);
    std::unique_ptr<cmajor::masm::intermediate::CodeGenerator> codeGenerator;
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release))
    {
        cmajor::masm::optimizer::Optimize(&intermediateContext);
        intermediateContext.Write(optimizedIntermediateFilePath);
        cmajor::masm::intermediate::Parse(boundCompileUnit.GetModule().LogStreamId(), optimizedIntermediateFilePath, optimizationContext,
            cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
        cmajor::masm::intermediate::Verify(optimizationContext);
        finalContext = &optimizationContext;
        codeGenerator.reset(new cmajor::masm::optimizer::OptimizingCodeGenerator(finalContext, boundCompileUnit.AsmFilePath()));
    }
    else
    {
        codeGenerator.reset(new cmajor::masm::intermediate::CodeGenerator(finalContext, boundCompileUnit.AsmFilePath()));
    }
    finalContext->GetData().VisitGlobalVariables(*codeGenerator);
    finalContext->GetCode().VisitFunctions(*codeGenerator);
    codeGenerator->WriteOutputFile();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release))
    {
        boundCompileUnit.SetTotal(intermediateContext.TotalFunctions());
        boundCompileUnit.SetInlined(intermediateContext.FunctionsInlined());
    }
    else
    {
        boundCompileUnit.SetTotal(finalContext->GetCode().TotalFunctions());
    }
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundNamespace& boundNamespace)
{
    int n = boundNamespace.Members().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundNode* node = boundNamespace.Members()[i].get();
        node->Accept(*this);
    }
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundClass& boundClass)
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundFunction& boundFunction)
{
    if (!boundFunction.Body()) return;
    currentFunction = &boundFunction;
    cmajor::symbols::FunctionSymbol* functionSymbol = boundFunction.GetFunctionSymbol();
    if (functionSymbol->MangledName() == U"function_EvaluateShiftLeft_A5F0E366587CE3348FEE8B6A4362E042D75F4128")
    {
        int x = 0;
    }
    if (functionSymbol->MangledName() == U"function_EvaluateShiftLeft_CA12133843227B43D2C38AD3F56F4E15EE936D30")
    {
        int x = 0;
    }
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
    latestRet = nullptr;
    if (functionSymbol->HasSource() && cmajor::symbols::GetConfig() != "release" && module->Name() != U"System.Runtime" && module->Name() != U"System.Core")
    {
        generateLineNumbers = true;
        emitter->SetGenerateLocationInfo(true);
        fullSpan = functionSymbol->GetFullSpan();
    }
    else
    {
        generateLineNumbers = false;
        emitter->SetGenerateLocationInfo(false);
        fullSpan = soul::ast::FullSpan();
    }
    function = emitter->GetOrInsertFunction(util::ToUtf8(functionSymbol->MangledName()), functionType, functionSymbol->DontThrow());
    void* mdStruct = emitter->CreateMDStruct();
    void* fullNameItem = emitter->CreateMDString(util::ToUtf8(functionSymbol->FullName()));
    emitter->AddMDItem(mdStruct, "fullName", fullNameItem);
    emitter->SetFunctionMdId(function, emitter->GetMDStructId(mdStruct));
    emitter->SetFunctionComment(function, util::ToUtf8(functionSymbol->FullName()));
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release) && functionSymbol->IsInline())
    {
        emitter->AddInlineFunctionAttribute(function);
        functionSymbol->SetLinkOnceOdrLinkage();
    }
    else if (functionSymbol->IsGeneratedFunction() && functionSymbol->CanInline())
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
    }
    emitter->SetFunction(function, fileIndex, functionSymbol->ModuleId(), functionId);
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
    emitter->FinalizeFunction(function, functionSymbol->HasCleanup());
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundCompoundStatement& boundCompoundStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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
    if (currentBlock->IsDestroyed() && n > 0 && boundCompoundStatement.Statements().back()->IsBreakStatement())
    {
        void* nextBlock = emitter->CreateBasicBlock("next");
        emitter->SetCurrentBasicBlock(nextBlock);
        basicBlockOpen = true;
    }
    ExitBlocks(prevBlock);
    blocks.pop_back();
    currentBlock = prevBlock;
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundSequenceStatement& boundSequenceStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundReturnStatement& boundReturnStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundReturnStatement.GetSpan());
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
    latestRet = &boundReturnStatement;
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundGotoCaseStatement& boundGotoCaseStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundGotoCaseStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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
        throw cmajor::symbols::Exception("case not found", boundGotoCaseStatement.GetFullSpan());
    }
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundGotoDefaultStatement& boundGotoDefaultStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundGotoDefaultStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
    SetTarget(&boundGotoDefaultStatement);
    Assert(breakTargetBlock, "break target not set");
    ExitBlocks(breakTargetBlock);
    if (defaultDest)
    {
        emitter->CreateBr(defaultDest);
    }
    else
    {
        throw cmajor::symbols::Exception("no default destination", boundGotoDefaultStatement.GetFullSpan());
    }
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundBreakStatement& boundBreakStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundBreakStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundBreakStatement);
    Assert(breakTarget && breakTargetBlock, "break target not set");
    ExitBlocks(breakTargetBlock);
    emitter->CreateBr(breakTarget);
/*
    if (!currentCaseMap) // not in switch
    {
        void* nextBlock = emitter->CreateBasicBlock("next");
        emitter->SetCurrentBasicBlock(nextBlock);
        basicBlockOpen = true;
    }
*/
//  if not in direct switch or last was unconditional return in its block:
    if (!InDirectSwitchStatement(&boundBreakStatement) || (latestRet && !latestRet->IsConditionalStatementInBlock(latestRet->Block()))) 
    {
        void* nextBlock = emitter->CreateBasicBlock("next");
        emitter->SetCurrentBasicBlock(nextBlock);
        basicBlockOpen = true;
    }
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundContinueStatement& boundContinueStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundContinueStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
    prevWasTerminator = false;
    SetTarget(&boundContinueStatement);
    Assert(continueTarget && continueTargetBlock, "continue target not set");
    ExitBlocks(continueTargetBlock);
    emitter->CreateBr(continueTarget);
    void* nextBlock = emitter->CreateBasicBlock("next");
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundGotoStatement& boundGotoStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundGotoStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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
        throw cmajor::symbols::Exception("goto target not found", boundGotoStatement.GetFullSpan());
    }
    void* nextBlock = emitter->CreateBasicBlock("next");
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundIfStatement& boundIfStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundIfStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundWhileStatement& boundWhileStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundWhileStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundDoStatement& boundDoStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundDoStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundForStatement& boundForStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundForStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundSwitchStatement& boundSwitchStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundSwitchStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundCaseStatement& boundCaseStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundDefaultStatement& boundDefaultStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
    SetTarget(&boundDefaultStatement);
    if (defaultDest)
    {
        emitter->SetCurrentBasicBlock(defaultDest);
        if (generateLineNumbers)
        {
            SetSpan(boundDefaultStatement.GetSpan());
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundConstructionStatement& boundConstructionStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundConstructionStatement.GetSpan());
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
                if (firstArgument->GetType()->IsPointerType() && firstArgument->GetType()->RemovePointer()->IsClassTypeSymbol())
                {
                    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(firstArgumentBaseType);
                    cmajor::symbols::DestructorSymbol* destructor = classType->Destructor();
                    if (destructor)
                    {
                        if (destructor->IsGeneratedFunction() || destructor->IsTemplateSpecialization())
                        {
                            destructor = static_cast<cmajor::symbols::DestructorSymbol*>(classType->Destructor()->Copy());
                            compileUnit->GetSymbolTable().AddFunctionSymbol(std::unique_ptr<cmajor::symbols::FunctionSymbol>(destructor));
                            cmajor::ast::CompileUnitNode* compileUnitNode = compileUnit->GetCompileUnitNode();
                            if (compileUnitNode)
                            {
                                destructor->SetCompileUnitId(compileUnitNode->Id());
                                destructor->ComputeMangledName();
                            }
                        }
                        newCleanupNeeded = true;
                        std::unique_ptr<cmajor::binder::BoundExpression> classPtrArgument(firstArgument->Clone());
                        std::unique_ptr<cmajor::binder::BoundFunctionCall> destructorCall(
                            new cmajor::binder::BoundFunctionCall(currentBlock->EndSpan(), destructor));
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundAssignmentStatement& boundAssignmentStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundAssignmentStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
    SetTarget(&boundAssignmentStatement);
    boundAssignmentStatement.AssignmentCall()->Accept(*this);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundExpressionStatement& boundExpressionStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundExpressionStatement.GetSpan());
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundInitializationStatement& boundInitializationStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundInitializationStatement.GetSpan());
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundEmptyStatement& boundEmptyStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundEmptyStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
    SetTarget(&boundEmptyStatement);
    emitter->CreateNop();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundSetVmtPtrStatement& boundSetVmtPtrStatement)
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundThrowStatement& boundThrowStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundThrowStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
    SetTarget(&boundThrowStatement);
    boundThrowStatement.ThrowCallExpr()->Accept(*this);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundTryStatement& boundTryStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundTryStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
    SetTarget(&boundTryStatement);
    void* prevHandlerBlock = handlerBlock;
    void* prevCleanupBlock = cleanupBlock;
    handlerBlock = emitter->CreateBasicBlock("handlers");
    cleanupBlock = nullptr;
    int parentTryBlockId = currentTryBlockId;
    currentTryBlockId = nextTryBlockId++;
    void* nop1 = emitter->CreateNop();
/*
    void* beginTry = emitter->CreateMDStruct();
    emitter->AddMDItem(beginTry, "nodeType", emitter->CreateMDLong(beginTryNodeType));
    emitter->AddMDItem(beginTry, "tryBlockId", emitter->CreateMDLong(currentTryBlockId));
    emitter->AddMDItem(beginTry, "parentTryBlockId", emitter->CreateMDLong(parentTryBlockId));
    int beginTryId = emitter->GetMDStructId(beginTry);
    void* beginTryMdRef = emitter->CreateMDStructRef(beginTryId);
    emitter->SetMetadataRef(nop1, beginTryMdRef);
*/
    bool prevInTryBlock = inTryBlock;
    inTryBlock = true;
    boundTryStatement.TryBlock()->Accept(*this);
    inTryBlock = prevInTryBlock;
    void* nop2 = emitter->CreateNop();
/*
    void* endTry = emitter->CreateMDStruct();
    emitter->AddMDItem(endTry, "nodeType", emitter->CreateMDLong(endTryNodeType));
    emitter->AddMDItem(endTry, "tryBlockId", emitter->CreateMDLong(currentTryBlockId));
    int endTryId = emitter->GetMDStructId(endTry);
    void* endTryMdRef = emitter->CreateMDStructRef(endTryId);
    emitter->SetMetadataRef(nop2, endTryMdRef);
*/
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundCatchStatement& boundCatchStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundCatchStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
    SetTarget(&boundCatchStatement);
    void* catchBlock = emitter->CreateBasicBlock("catch");
    emitter->SetCurrentBasicBlock(catchBlock);
    void* nop1 = emitter->CreateNop();
    boundCatchStatement.CatchBlock()->Accept(*this);
    emitter->CreateBr(currentTryNextBlock);
    emitter->SetCurrentBasicBlock(currentTryNextBlock);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundRethrowStatement& boundRethrowStatement)
{
    if (generateLineNumbers)
    {
        SetSpan(boundRethrowStatement.GetSpan());
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    latestRet = nullptr;
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundParameter& boundParameter)
{
    boundParameter.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundLocalVariable& boundLocalVariable)
{
    boundLocalVariable.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundMemberVariable& boundMemberVariable)
{
    boundMemberVariable.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundConstant& boundConstant)
{
    boundConstant.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundEnumConstant& boundEnumConstant)
{
    boundEnumConstant.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundLiteral& boundLiteral)
{
    boundLiteral.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundTemporary& boundTemporary)
{
    boundTemporary.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundSizeOfExpression& boundSizeOfExpression)
{
    boundSizeOfExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundAddressOfExpression& boundAddressOfExpression)
{
    boundAddressOfExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundDereferenceExpression& boundDereferenceExpression)
{
    boundDereferenceExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundReferenceToPointerExpression& boundReferenceToPointerExpression)
{
    boundReferenceToPointerExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundFunctionCall& boundFunctionCall)
{
    boundFunctionCall.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundDelegateCall& boundDelegateCall)
{
    boundDelegateCall.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundClassDelegateCall& boundClassDelegateCall)
{
    boundClassDelegateCall.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundConversion& boundConversion)
{
    boundConversion.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundConstructExpression& boundConstructExpression)
{
    boundConstructExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundConstructAndReturnTemporaryExpression& boundConstructAndReturnTemporaryExpression)
{
    boundConstructAndReturnTemporaryExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundClassOrClassDelegateConversionResult& boundClassOrClassDelegateConversionResult)
{
    boundClassOrClassDelegateConversionResult.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundIsExpression& boundIsExpression)
{
    boundIsExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
    GenJumpingBoolCode();
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundAsExpression& boundAsExpression)
{
    boundAsExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundTypeNameExpression& boundTypeNameExpression)
{
    boundTypeNameExpression.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundBitCast& boundBitCast)
{
    boundBitCast.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundFunctionPtr& boundFunctionPtr)
{
    boundFunctionPtr.Load(*emitter, cmajor::ir::OperationFlags::none);
}

void MasmCodeGenerator::Visit(cmajor::binder::BoundDisjunction& boundDisjunction)
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundConjunction& boundConjunction)
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

void MasmCodeGenerator::Visit(cmajor::binder::BoundGlobalVariable& boundGlobalVariable)
{
    cmajor::symbols::GlobalVariableSymbol* globalVariableSymbol = boundGlobalVariable.GetGlobalVariableSymbol();
    globalVariableSymbol->CreateIrObject(*emitter);
}

void* MasmCodeGenerator::GetGlobalStringPtr(int stringId)
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

void* MasmCodeGenerator::GetGlobalWStringConstant(int stringId, void*& arrayType)
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

void* MasmCodeGenerator::GetGlobalUStringConstant(int stringId, void*& arrayType)
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

void* MasmCodeGenerator::GetGlobalUuidConstant(int uuidId)
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
    return nullptr;
}

int MasmCodeGenerator::Install(const std::string& str)
{
    return compileUnit->Install(str);
}

int MasmCodeGenerator::Install(const std::u16string& str)
{
    return compileUnit->Install(str);
}

int MasmCodeGenerator::Install(const std::u32string& str)
{
    return compileUnit->Install(str);
}

void* MasmCodeGenerator::HandlerBlock()
{
    return handlerBlock;
}

void* MasmCodeGenerator::CleanupBlock()
{
    return cleanupBlock;
}

bool MasmCodeGenerator::NewCleanupNeeded()
{
    return newCleanupNeeded;
}

void MasmCodeGenerator::CreateCleanup()
{
    int x = 0;
}

void MasmCodeGenerator::GenerateCodeForCleanups()
{
    int x = 0;
}

bool MasmCodeGenerator::InTryBlock() const
{
    return inTryBlock;
}

int MasmCodeGenerator::CurrentTryBlockId() const
{
    return currentTryBlockId;
}

void MasmCodeGenerator::GenerateEnterFunctionCode(cmajor::binder::BoundFunction& boundFunction)
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

void MasmCodeGenerator::SetSpan(const soul::ast::Span& span)
{
    if (!span.IsValid()) return;
    if (inSetLineOrEntryCode) return;
    fullSpan.span = span;
    SetLineNumber(cmajor::symbols::GetLineNumber(fullSpan));
}

void MasmCodeGenerator::SetLineNumber(int32_t lineNumber)
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

} // namespace cmajor::masm::backend
