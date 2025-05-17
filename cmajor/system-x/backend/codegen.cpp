// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.systemx.backend.codegen;

import cmajor.systemx.intermediate;
import cmajor.systemx.assembler;
import cmajor.systemx.optimizer;
import util;

namespace cmajor::systemx::backend {

SystemXCodeGenerator::SystemXCodeGenerator(cmajor::ir::Emitter* emitter_) : 
    emitter(emitter_), symbolTable(nullptr), context(nullptr), module(nullptr), compileUnit(nullptr), fullSpan(),
    nativeCompileUnit(nullptr), function(nullptr), entryBasicBlock(nullptr), lastInstructionWasRet(false), destructorCallGenerated(false), genJumpingBoolCode(false),
    trueBlock(nullptr), falseBlock(nullptr), breakTarget(nullptr), continueTarget(nullptr), sequenceSecond(nullptr), currentFunction(nullptr), currentBlock(nullptr),
    breakTargetBlock(nullptr), continueTargetBlock(nullptr), lastAlloca(nullptr), currentClass(nullptr), basicBlockOpen(false), defaultDest(nullptr), currentCaseMap(nullptr),
    generateLineNumbers(false), currentTryBlockId(-1), nextTryBlockId(0), currentTryNextBlock(nullptr), handlerBlock(nullptr), cleanupBlock(nullptr), newCleanupNeeded(false),
    inTryBlock(false), prevWasTerminator(false), beginLineColLen(), endLineColLen(), continueTargetIndex(-1), breakSourceIndex(-1)
{
    emitter->SetEmittingDelegate(this);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    context = boundCompileUnit.GetContext();
    emitter->SetContext(context);
    std::string intermediateFilePath = util::Path::ChangeExtension(boundCompileUnit.ObjectFilePath(), ".i");
    emitter->SetFilePath(intermediateFilePath);
    std::string optimizedIntermediateFilePath = util::Path::ChangeExtension(boundCompileUnit.ObjectFilePath(), ".opt.i");
    compileUnitId = boundCompileUnit.Id();
    emitter->SetCompileUnitId(compileUnitId);
    void* sourceFileMetadataRef = emitter->GetMDStructRefForSourceFile(boundCompileUnit.SourceFilePath());
    emitter->SetCompileUnitMetadataRef(sourceFileMetadataRef);
    generateLineNumbers = false;
    symbolTable = &boundCompileUnit.GetSymbolTable();
    module = &boundCompileUnit.GetModule();
    compileUnit = &boundCompileUnit;
    nativeCompileUnit = static_cast<cmajor::systemx::intermediate::CompileUnit*>(emitter->GetCompileUnit());
    cmajor::binder::ConstantArrayRepository& constantArrayRepository = boundCompileUnit.GetConstantArrayRepository();
    for (cmajor::symbols::ConstantSymbol* constantSymbol : constantArrayRepository.ConstantArrays())
    {
        constantSymbol->ArrayIrObject(*emitter, true, context);
    }
    cmajor::binder::ConstantStructureRepository& constantStructureRepository = boundCompileUnit.GetConstantStructureRepository();
    for (cmajor::symbols::ConstantSymbol* constantSymbol : constantStructureRepository.ConstantStructures())
    {
        constantSymbol->StructureIrObject(*emitter, true, context);
    }
    int n = boundCompileUnit.BoundNodes().size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::binder::BoundNode* node = boundCompileUnit.BoundNodes()[i].get();
        node->Accept(*this);
    }
    nativeCompileUnit->Write();
    cmajor::systemx::intermediate::Context intermediateContext;
    cmajor::systemx::intermediate::Context optimizationContext;
    cmajor::systemx::intermediate::Context* finalContext = &intermediateContext;
    cmajor::systemx::intermediate::Parse(boundCompileUnit.GetModule().LogStreamId(), intermediateFilePath, intermediateContext,
        cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
    cmajor::systemx::intermediate::Verify(intermediateContext);
    std::string objectFilePath = boundCompileUnit.ObjectFilePath();
    std::string assemblyFilePath = util::Path::ChangeExtension(objectFilePath, ".s");
    {
        std::unique_ptr<cmajor::systemx::intermediate::CodeGenerator> codeGenerator;
        {
            std::unique_ptr<cmajor::systemx::assembler::AssemblyFile> assemblyFile(new cmajor::systemx::assembler::AssemblyFile(assemblyFilePath));
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::release))
            {
                if (cmajor::systemx::optimizer::CurrentOptimizations() != cmajor::systemx::optimizer::Optimizations::o0)
                {
                    cmajor::systemx::optimizer::Optimize(&intermediateContext);
                    cmajor::systemx::intermediate::Write(intermediateContext, optimizedIntermediateFilePath);
                    cmajor::systemx::intermediate::Parse(boundCompileUnit.GetModule().LogStreamId(), optimizedIntermediateFilePath, optimizationContext,
                        cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
                    cmajor::systemx::intermediate::Verify(optimizationContext);
                    finalContext = &optimizationContext;
                }
                codeGenerator.reset(new cmajor::systemx::optimizer::OptimizingCodeGenerator(finalContext, assemblyFile.get()));
            }
            else
            {
                codeGenerator.reset(new cmajor::systemx::intermediate::SimpleAssemblyCodeGenerator(finalContext, assemblyFile.get()));
            }
            codeGenerator->GenerateCode();
            codeGenerator->GenerateDebugInfo();
            codeGenerator->WriteOutputFile();
        }
    }
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
    void* functionType = functionSymbol->IrType(*emitter, context);
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
        fullSpan = soul::ast::FullSpan(functionSymbol->ModuleId(), functionSymbol->FileIndex(), boundFunction.Body()->GetSpan());
        beginLineColLen = GetLineColLen(boundFunction.Body()->GetSpan());
    }
    else
    {
        generateLineNumbers = false;
        fullSpan = soul::ast::FullSpan();
        beginLineColLen = soul::ast::LineColLen();
        emitter->SetCurrentSourcePos(0, 0, 0); 
    }
    function = emitter->GetOrInsertFunction(util::ToUtf8(functionSymbol->MangledName()), functionType, functionSymbol->DontThrow());
    if (functionSymbol->HasSource())
    {
        void* mdStruct = emitter->CreateMDStruct();
        emitter->AddMDItem(mdStruct, "nodeType", emitter->CreateMDLong(funcInfoNodeType));
        emitter->AddMDItem(mdStruct, "fullName", emitter->CreateMDString(util::ToUtf8(functionSymbol->FullName())));
        util::uuid moduleId = functionSymbol->ModuleId();
        void* mdFile = emitter->GetMDStructRefForSourceFile(cmajor::symbols::GetSourceFilePath(functionSymbol->FileIndex(), moduleId));
        emitter->AddMDItem(mdStruct, "sourceFile", mdFile);
        emitter->AddMDItem(mdStruct, "main", emitter->CreateMDBool(functionSymbol->IsUserMain()));
        cfg = emitter->CreateMDArray();
        emitter->AddMDItem(mdStruct, "cfg", cfg);
        int mdId = emitter->GetMDStructId(mdStruct);
        emitter->SetFunctionMdId(function, mdId);
    }
    else
    {
        cfg = nullptr;
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
    util::uuid functionId;
    if (functionSymbol->HasSource())
    {
        functionId = functionSymbol->FunctionId();
    }
    emitter->SetFunction(function, functionSymbol->FileIndex(), functionSymbol->ModuleId(), functionId);
    if (generateLineNumbers)
    {
        emitter->SetCurrentLineColLen(beginLineColLen);
    }
    if (functionSymbol->GroupName() == U"main")
    {
        if (!functionSymbol->IsProgramMain())
        {
            emitter->SetCurrentFunctionMain();
        }
    }
    void* entryBlock = emitter->CreateBasicBlock("entry");
    entryBasicBlock = entryBlock;
    emitter->SetCurrentBasicBlock(entryBlock);
    if (currentClass && !currentClass->IsInlineFunctionContainer())
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = currentClass->GetClassTypeSymbol();
        if (!emitter->IsVmtObjectCreated(classTypeSymbol))
        {
            classTypeSymbol->VmtObject(*emitter, true, context);
        }
        if (!emitter->IsStaticObjectCreated(classTypeSymbol))
        {
            classTypeSymbol->StaticObject(*emitter, true, context);
        }
        CreateMetadataForClassType(classTypeSymbol);
    }
    int np = functionSymbol->Parameters().size();
    for (int i = 0; i < np; ++i)
    {
        cmajor::symbols::ParameterSymbol* parameter = functionSymbol->Parameters()[i];
        CreateMetadataForType(parameter->GetType());
        void* irType = parameter->GetType()->IrType(*emitter, context);
        void* allocaInst = emitter->CreateAlloca(irType);
        CreateLocalNode(allocaInst, util::ToUtf8(parameter->Name()), irType);
        emitter->SetIrObject(parameter, allocaInst);
        lastAlloca = allocaInst;
    }
    if (functionSymbol->ReturnParam())
    {
        cmajor::symbols::ParameterSymbol* parameter = functionSymbol->ReturnParam();
        void* allocaInst = emitter->CreateAlloca(parameter->GetType()->IrType(*emitter, context));
        emitter->SetIrObject(parameter, allocaInst);
        lastAlloca = allocaInst;
    }
    int nlv = functionSymbol->LocalVariables().size();
    for (int i = 0; i < nlv; ++i)
    {
        cmajor::symbols::LocalVariableSymbol* localVariable = functionSymbol->LocalVariables()[i];
        CreateMetadataForType(localVariable->GetType());
        void* irType = localVariable->GetType()->IrType(*emitter, context);
        void* allocaInst = emitter->CreateAlloca(irType);
        if (!localVariable->IsTemporary())
        {
            CreateLocalNode(allocaInst, util::ToUtf8(localVariable->Name()), irType);
        }
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
            void* copyCtorType = copyConstructor->IrType(*emitter, context);
            void* callee = emitter->GetOrInsertFunction(util::ToUtf8(copyConstructor->MangledName()), copyCtorType, copyConstructor->DontThrow());
            std::vector<void*> args;
            args.push_back(parameter->IrObject(*emitter, context));
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
            cmajor::ir::NativeValue paramValue(parameter->IrObject(*emitter, context));
            copyCtorArgs.push_back(&paramValue);
            cmajor::ir::NativeValue argumentValue(arg);
            copyCtorArgs.push_back(&argumentValue);
            copyConstructor->GenerateCall(*emitter, copyCtorArgs, cmajor::ir::OperationFlags::none, context);
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
            cmajor::ir::NativeValue paramValue(parameter->IrObject(*emitter, context));
            paramValue.SetType(interfaceType->AddPointer(context));
            copyCtorArgs.push_back(&paramValue);
            cmajor::ir::NativeValue argumentValue(arg);
            argumentValue.SetType(interfaceType->AddPointer(context));
            copyCtorArgs.push_back(&argumentValue);
            copyConstructor->GenerateCall(*emitter, copyCtorArgs, cmajor::ir::OperationFlags::none, context);
        }
        else
        {
            emitter->CreateStore(arg, parameter->IrObject(*emitter, context));
        }
    }
    if (functionSymbol->ReturnParam())
    {
        void* arg = emitter->GetFunctionArgument(function, np);
        emitter->CreateStore(arg, functionSymbol->ReturnParam()->IrObject(*emitter, context));
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
            void* defaultValue = functionSymbol->ReturnType()->CreateDefaultIrValue(*emitter, context);
            int lastIndex = emitter->GetLineColLenIndex(endLineColLen);
            endLineColLen  = GetLineColLen(boundFunction.Body()->EndSpan());
            emitter->SetCurrentLineColLen(endLineColLen);
            emitter->CreateRet(defaultValue);
            AddCFGItem(lastIndex, emitter->GetLineColLenIndex(endLineColLen));
            lastInstructionWasRet = true;
        }
        else
        {
            int lastIndex = emitter->GetLineColLenIndex(endLineColLen);
            endLineColLen  = GetLineColLen(boundFunction.Body()->EndSpan());
            emitter->SetCurrentLineColLen(endLineColLen);
            emitter->CreateRetVoid();
            AddCFGItem(lastIndex, emitter->GetLineColLenIndex(endLineColLen));
            lastInstructionWasRet = true;
        }
    }
    GenerateCodeForCleanups();
    emitter->FinalizeFunction(function, functionSymbol->HasCleanup());
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundCompoundStatement& boundCompoundStatement)
{
    soul::ast::LineColLen blockBeginLineColLen;
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundCompoundStatement.GetSpan());
        endLineColLen = beginLineColLen;
        blockBeginLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
        emitter->CreateNop();
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundCompoundStatement);
    cmajor::binder::BoundCompoundStatement* prevBlock = currentBlock;
    currentBlock = &boundCompoundStatement;
    blockDestructionMap[currentBlock] = std::vector<std::unique_ptr<cmajor::binder::BoundFunctionCall>>();
    blocks.push_back(currentBlock);
    bool lastWasBreakOrContinue = false;
    int n = boundCompoundStatement.Statements().size();
    for (int i = 0; i < n; ++i)
    {
        soul::ast::LineColLen prevLineColLen = endLineColLen;
        cmajor::binder::BoundStatement* statement = boundCompoundStatement.Statements()[i].get();
        if (statement->IsBreakStatement() || statement->IsContinueStatement())
        {
            lastWasBreakOrContinue = true;
        }
        else
        {
            lastWasBreakOrContinue = false;
        }
        statement->Accept(*this);
        soul::ast::LineColLen nextLineColLen = beginLineColLen;
        AddCFGItem(emitter->GetLineColLenIndex(prevLineColLen), emitter->GetLineColLenIndex(nextLineColLen));
    }
    ExitBlocks(prevBlock);
    blocks.pop_back();
    currentBlock = prevBlock;
    soul::ast::LineColLen lastLineColLen = endLineColLen;
    beginLineColLen = blockBeginLineColLen;
    cmajor::binder::BoundStatement* lastStatement = nullptr;
    if (!currentFunction->Body()->Statements().empty())
    {
        lastStatement = currentFunction->Body()->Statements().back().get();
    }
    if (prevBlock && (!lastStatement || lastStatement->GetBoundNodeType() != cmajor::binder::BoundNodeType::boundReturnStatement ||
        lastStatement->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundReturnStatement && destructorCallGenerated))
    {
        if (!currentCaseMap) // not in switch
        {
            endLineColLen = GetLineColLen(boundCompoundStatement.EndSpan());
            emitter->SetCurrentLineColLen(endLineColLen);
            emitter->CreateNop();
            AddCFGItem(emitter->GetLineColLenIndex(lastLineColLen), emitter->GetLineColLenIndex(endLineColLen));
            if (lastWasBreakOrContinue)
            {
                void* nextBlock = emitter->CreateBasicBlock("next");
                emitter->CreateBr(nextBlock);
                emitter->SetCurrentBasicBlock(nextBlock);
                basicBlockOpen = true;
            }
        }
    }
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
        beginLineColLen = GetLineColLen(boundReturnStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
        beginLineColLen = GetLineColLen(boundGotoCaseStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
        throw cmajor::symbols::Exception("case not found", boundGotoCaseStatement.GetFullSpan());
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundGotoDefaultStatement& boundGotoDefaultStatement)
{
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundGotoDefaultStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
        throw cmajor::symbols::Exception("no default destination", boundGotoDefaultStatement.GetFullSpan());
    }
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundBreakStatement& boundBreakStatement)
{
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundBreakStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundBreakStatement);
    Assert(breakTarget && breakTargetBlock, "break target not set");
    ExitBlocks(breakTargetBlock);
    emitter->CreateBr(breakTarget);
    breakSourceIndex = emitter->GetLineColLenIndex(beginLineColLen);
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
        beginLineColLen = GetLineColLen(boundContinueStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    prevWasTerminator = false;
    SetTarget(&boundContinueStatement);
    Assert(continueTarget && continueTargetBlock, "continue target not set");
    ExitBlocks(continueTargetBlock);
    emitter->CreateBr(continueTarget);
    AddCFGItem(emitter->GetLineColLenIndex(beginLineColLen), continueTargetIndex);
    void* nextBlock = emitter->CreateBasicBlock("next");
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundGotoStatement& boundGotoStatement)
{
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundGotoStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
        throw cmajor::symbols::Exception("goto target not found", boundGotoStatement.GetFullSpan());
    }
    void* nextBlock = emitter->CreateBasicBlock("next");
    emitter->SetCurrentBasicBlock(nextBlock);
    basicBlockOpen = true;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundIfStatement& boundIfStatement)
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
    soul::ast::LineColLen condLineColLen = GetLineColLen(boundIfStatement.Condition()->GetSpan());
    emitter->SetCurrentLineColLen(condLineColLen);
    boundIfStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    boundIfStatement.ThenS()->Accept(*this);
    emitter->CreateBr(nextBlock);
    AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(beginLineColLen));
    if (boundIfStatement.ElseS())
    {
        emitter->SetCurrentBasicBlock(falseBlock);
        boundIfStatement.ElseS()->Accept(*this);
        emitter->CreateBr(nextBlock);
        AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(beginLineColLen));
    }
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
    emitter->SetCurrentBasicBlock(nextBlock);
    soul::ast::LineColLen nextLineColLen;
    cmajor::binder::BoundCompoundStatement* block = boundIfStatement.Block();
    if (block)
    {
        nextLineColLen = GetLineColLen(block->NextSpan(&boundIfStatement));
    }
    if (nextLineColLen.IsValid())
    {
        emitter->SetCurrentLineColLen(nextLineColLen);
        emitter->CreateNop();
        AddCFGItem(emitter->GetLineColLenIndex(endLineColLen), emitter->GetLineColLenIndex(nextLineColLen));
        if (!boundIfStatement.ElseS())
        {
            AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(nextLineColLen));
        }
    }
    basicBlockOpen = true;
    beginLineColLen = condLineColLen;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundWhileStatement& boundWhileStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundWhileStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    void* prevBreakTarget = breakTarget;
    int prevBreakSourceIndex = breakSourceIndex;
    void* prevContinueTarget = continueTarget;
    int32_t prevContinueTargetIndex = continueTargetIndex;
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
    soul::ast::LineColLen condLineColLen = GetLineColLen(boundWhileStatement.Condition()->GetSpan());
    emitter->SetCurrentLineColLen(condLineColLen);
    continueTargetIndex = emitter->GetLineColLenIndex(condLineColLen);
    boundWhileStatement.Condition()->Accept(*this);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(trueBlock);
    boundWhileStatement.Statement()->Accept(*this);
    AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(beginLineColLen));
    AddCFGItem(emitter->GetLineColLenIndex(endLineColLen), emitter->GetLineColLenIndex(condLineColLen));
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(falseBlock);
    soul::ast::LineColLen nextLineColLen;
    cmajor::binder::BoundCompoundStatement* block = boundWhileStatement.Block();
    if (block)
    {
        nextLineColLen = GetLineColLen(block->NextSpan(&boundWhileStatement));
    }
    if (nextLineColLen.IsValid())
    {
        emitter->SetCurrentLineColLen(nextLineColLen);
        emitter->CreateNop();
        AddCFGItem(breakSourceIndex, emitter->GetLineColLenIndex(nextLineColLen));
        AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(nextLineColLen));
    }
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    breakSourceIndex = prevBreakSourceIndex;
    continueTarget = prevContinueTarget;
    continueTargetIndex = prevContinueTargetIndex;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
    beginLineColLen = condLineColLen;
    endLineColLen = condLineColLen;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundDoStatement& boundDoStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundDoStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    void* prevBreakTarget = breakTarget;
    int prevBreakSourceIndex = breakSourceIndex;
    void* prevContinueTarget = continueTarget;
    int32_t prevContinueTargetIndex = continueTargetIndex;
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
    soul::ast::LineColLen condLineColLen = GetLineColLen(boundDoStatement.Condition()->GetSpan());
    emitter->SetCurrentLineColLen(condLineColLen);
    continueTargetIndex = emitter->GetLineColLenIndex(condLineColLen);
    boundDoStatement.Statement()->Accept(*this);
    soul::ast::LineColLen doLineColLen = beginLineColLen;
    soul::ast::LineColLen doEndLineColLen = endLineColLen;
    soul::ast::LineColLen statementLineColLen = beginLineColLen;;
    emitter->CreateBr(condBlock);
    emitter->SetCurrentBasicBlock(condBlock);
    bool prevGenJumpingBoolCode = genJumpingBoolCode;
    genJumpingBoolCode = true;
    emitter->SetCurrentLineColLen(condLineColLen);
    boundDoStatement.Condition()->Accept(*this);
    AddCFGItem(emitter->GetLineColLenIndex(doEndLineColLen), emitter->GetLineColLenIndex(condLineColLen));
    AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(statementLineColLen));
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(falseBlock);
    soul::ast::LineColLen nextLineColLen;
    cmajor::binder::BoundCompoundStatement* block = boundDoStatement.Block();
    if (block)
    {
        nextLineColLen = GetLineColLen(block->NextSpan(&boundDoStatement));
    }
    if (nextLineColLen.IsValid())
    {
        emitter->SetCurrentLineColLen(nextLineColLen);
        emitter->CreateNop();
        AddCFGItem(breakSourceIndex, emitter->GetLineColLenIndex(nextLineColLen));
        AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(nextLineColLen));
    }
    basicBlockOpen = true;
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    breakSourceIndex = prevBreakSourceIndex;
    continueTarget = prevContinueTarget;
    continueTargetIndex = prevContinueTargetIndex;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
    beginLineColLen = doLineColLen;
    endLineColLen = condLineColLen;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundForStatement& boundForStatement)
{
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundForStatement);
    void* prevTrueBlock = trueBlock;
    void* prevFalseBlock = falseBlock;
    void* prevBreakTarget = breakTarget;
    int prevBreakSourceIndex = breakSourceIndex;
    void* prevContinueTarget = continueTarget;
    int32_t prevContinueTargetIndex = continueTargetIndex;
    boundForStatement.InitS()->Accept(*this);
    soul::ast::LineColLen forLineColLen = beginLineColLen;
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
    soul::ast::LineColLen condLineColLen = GetLineColLen(boundForStatement.Condition()->GetSpan());
    emitter->SetCurrentLineColLen(condLineColLen);
    boundForStatement.Condition()->Accept(*this);
    AddCFGItem(emitter->GetLineColLenIndex(forLineColLen), emitter->GetLineColLenIndex(condLineColLen));
    soul::ast::LineColLen loopLineColLen = GetLineColLen(boundForStatement.LoopS()->GetSpan());
    emitter->SetCurrentLineColLen(loopLineColLen);
    continueTargetIndex = emitter->GetLineColLenIndex(loopLineColLen);
    genJumpingBoolCode = prevGenJumpingBoolCode;
    emitter->SetCurrentBasicBlock(actionBlock);
    boundForStatement.ActionS()->Accept(*this);
    soul::ast::LineColLen actionLineColLen = beginLineColLen;
    AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(actionLineColLen));
    soul::ast::LineColLen actionEndLineColLen = endLineColLen;
    emitter->SetCurrentLineColLen(actionEndLineColLen);
    emitter->CreateBr(loopBlock);
    emitter->SetCurrentBasicBlock(loopBlock);
    boundForStatement.LoopS()->Accept(*this);
    emitter->CreateBr(condBlock);
    AddCFGItem(emitter->GetLineColLenIndex(actionEndLineColLen), emitter->GetLineColLenIndex(loopLineColLen));
    AddCFGItem(emitter->GetLineColLenIndex(loopLineColLen), emitter->GetLineColLenIndex(condLineColLen));
    emitter->SetCurrentBasicBlock(falseBlock);
    soul::ast::LineColLen nextLineColLen;
    cmajor::binder::BoundCompoundStatement* block = boundForStatement.Block();
    if (block)
    {
        nextLineColLen = GetLineColLen(block->NextSpan(&boundForStatement));
    }
    if (nextLineColLen.IsValid())
    {
        emitter->SetCurrentLineColLen(nextLineColLen);
        emitter->CreateNop();
        AddCFGItem(breakSourceIndex, emitter->GetLineColLenIndex(nextLineColLen));
        AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(nextLineColLen));
    }
    basicBlockOpen = true;
    breakTargetBlock = prevBreakTargetBlock;
    continueTargetBlock = prevContinueTargetBlock;
    breakTarget = prevBreakTarget;
    breakSourceIndex = prevBreakSourceIndex;
    continueTarget = prevContinueTarget;
    continueTargetIndex = prevContinueTargetIndex;
    trueBlock = prevTrueBlock;
    falseBlock = prevFalseBlock;
    beginLineColLen = forLineColLen;
    endLineColLen = condLineColLen;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundSwitchStatement& boundSwitchStatement)
{
    soul::ast::LineColLen switchLineColLen;
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundSwitchStatement.GetSpan());
        switchLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundSwitchStatement);
    void* prevBreakTarget = breakTarget;
    int prevBreakSourceIndex = breakSourceIndex;
    cmajor::binder::BoundCompoundStatement* prevBreakTargetBlock = breakTargetBlock;
    breakTargetBlock = currentBlock;
    emitter->SetCurrentLineColLen(GetLineColLen(boundSwitchStatement.Condition()->GetSpan()));
    boundSwitchStatement.Condition()->Accept(*this);
    soul::ast::LineColLen condLineColLen = beginLineColLen;
    AddCFGItem(emitter->GetLineColLenIndex(switchLineColLen), emitter->GetLineColLenIndex(condLineColLen));
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
            emitter->AddCase(switchInst, caseValue->IrValue(*emitter, context), caseDest);
        }
    }
    for (unsigned i = 0; i < n; ++i)
    {
        const std::unique_ptr<cmajor::binder::BoundCaseStatement>& caseS = boundSwitchStatement.CaseStatements()[i];
        caseS->Accept(*this);
        soul::ast::LineColLen caseLineColLen = beginLineColLen;
        AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(caseLineColLen));
        if (basicBlockOpen)
        {
            emitter->CreateBr(next);
            basicBlockOpen = false;
        }
    }
    if (boundSwitchStatement.DefaultStatement())
    {
        boundSwitchStatement.DefaultStatement()->Accept(*this);
        soul::ast::LineColLen defaultLineColLen = beginLineColLen;
        AddCFGItem(emitter->GetLineColLenIndex(condLineColLen), emitter->GetLineColLenIndex(defaultLineColLen));
        if (basicBlockOpen)
        {
            emitter->CreateBr(next);
            basicBlockOpen = false;
        }
    }
    emitter->SetCurrentBasicBlock(next);
    soul::ast::LineColLen nextLineColLen;
    cmajor::binder::BoundCompoundStatement* block = boundSwitchStatement.Block();
    if (block)
    {
        nextLineColLen = GetLineColLen(block->NextSpan(&boundSwitchStatement));
    }
    if (nextLineColLen.IsValid())
    {
        emitter->SetCurrentLineColLen(nextLineColLen);
        emitter->CreateNop();
        AddCFGItem(breakSourceIndex, emitter->GetLineColLenIndex(nextLineColLen));
    }
    basicBlockOpen = true;
    currentCaseMap = prevCaseMap;
    defaultDest = prevDefaultDest;
    breakTargetBlock = prevBreakTargetBlock;
    breakTarget = prevBreakTarget;
    breakSourceIndex = prevBreakSourceIndex;
    beginLineColLen = switchLineColLen;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundCaseStatement& boundCaseStatement)
{
    soul::ast::LineColLen caseLineColLen;
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundCaseStatement.GetSpan());
        caseLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
                AddCFGItem(emitter->GetLineColLenIndex(caseLineColLen), emitter->GetLineColLenIndex(beginLineColLen));
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
    beginLineColLen = caseLineColLen;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundDefaultStatement& boundDefaultStatement)
{
    soul::ast::LineColLen defaultLineColLen;
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundDefaultStatement.GetSpan());
        defaultLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
            AddCFGItem(emitter->GetLineColLenIndex(defaultLineColLen), emitter->GetLineColLenIndex(beginLineColLen));
        }
    }
    else
    {
        throw cmajor::symbols::Exception("no default destination", boundDefaultStatement.GetFullSpan());
    }
    beginLineColLen = defaultLineColLen;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundConstructionStatement& boundConstructionStatement)
{
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundConstructionStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
                if (firstArgument->GetType()->IsPointerType() && firstArgument->GetType()->RemovePointer(context)->IsClassTypeSymbol())
                {
                    cmajor::symbols::ClassTypeSymbol* classType = static_cast<cmajor::symbols::ClassTypeSymbol*>(firstArgumentBaseType);
                    if (classType->Destructor())
                    {
                        newCleanupNeeded = true;
                        std::unique_ptr<cmajor::binder::BoundExpression> classPtrArgument(firstArgument->Clone());
                        std::unique_ptr<cmajor::binder::BoundFunctionCall> destructorCall(new cmajor::binder::BoundFunctionCall(currentBlock->EndSpan(), classType->Destructor()));
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
        beginLineColLen = GetLineColLen(boundAssignmentStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
        beginLineColLen = GetLineColLen(boundExpressionStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
        beginLineColLen = GetLineColLen(boundInitializationStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
        beginLineColLen = GetLineColLen(boundEmptyStatement.GetSpan());
        endLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
    void* ptr = emitter->GetMemberVariablePtr(classType->IrType(*emitter, context), classPtrValue, vmtPtrIndex);
    void* vmtPtr = emitter->CreateBitCast(boundSetVmtPtrStatement.ClassType()->VmtObject(*emitter, false, context), emitter->GetIrTypeForVoidPtrType());
    emitter->CreateStore(vmtPtr, ptr);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundThrowStatement& boundThrowStatement)
{
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundThrowStatement.GetSpan());
        emitter->SetCurrentLineColLen(beginLineColLen);
    }
    destructorCallGenerated = false;
    lastInstructionWasRet = false;
    basicBlockOpen = false;
    SetTarget(&boundThrowStatement);
    boundThrowStatement.ThrowCallExpr()->Accept(*this);
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundTryStatement& boundTryStatement)
{
    soul::ast::LineColLen tryLineColLen;
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundTryStatement.GetSpan());
        tryLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
    beginLineColLen = tryLineColLen;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundCatchStatement& boundCatchStatement)
{
    soul::ast::LineColLen catchLineColLen;
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundCatchStatement.GetSpan());
        catchLineColLen = beginLineColLen;
        emitter->SetCurrentLineColLen(beginLineColLen);
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
    beginLineColLen = catchLineColLen;
}

void SystemXCodeGenerator::Visit(cmajor::binder::BoundRethrowStatement& boundRethrowStatement)
{
    if (generateLineNumbers)
    {
        beginLineColLen = GetLineColLen(boundRethrowStatement.GetSpan());
        emitter->SetCurrentLineColLen(beginLineColLen);
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
        void* defaultValue = currentFunction->GetFunctionSymbol()->ReturnType()->CreateDefaultIrValue(*emitter, context);
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
    globalVariableSymbol->CreateIrObject(*emitter, context);
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
        throw cmajor::symbols::Exception("target for labeled statement not found", labeledStatement->GetFullSpan());
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
            wcharConstants.push_back(emitter->CreateIrValueForWChar(c));
        }
        wcharConstants.push_back(emitter->CreateIrValueForWChar(static_cast<char16_t>(0)));
        void* arrayType = emitter->GetIrTypeForArrayType(emitter->GetIrTypeForWChar(), length + 1);
        void* stringObject = emitter->GetOrInsertGlobal("wstring" + std::to_string(stringId) + "_" + compileUnitId, emitter->GetIrTypeForWChar());
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
            ucharConstants.push_back(emitter->CreateIrValueForUChar(c));
        }
        ucharConstants.push_back(emitter->CreateIrValueForUChar(static_cast<char32_t>(0)));
        void* arrayType = emitter->GetIrTypeForArrayType(emitter->GetIrTypeForUChar(), length + 1);
        void* stringObject = emitter->GetOrInsertGlobal("ustring" + std::to_string(stringId) + "_" + compileUnitId, emitter->GetIrTypeForUChar());
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
    cmajor::binder::BoundStatement* parent = currentBlock->StatementParent();
    while (parent && parent->GetBoundNodeType() != cmajor::binder::BoundNodeType::boundTryStatement)
    {
        parent = parent->StatementParent();
    }
    if (parent && parent->IsBoundStatement())
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
            void* defaultValue = currentFunction->GetFunctionSymbol()->ReturnType()->CreateDefaultIrValue(*emitter, context);
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

soul::ast::LineColLen SystemXCodeGenerator::GetLineColLen(const soul::ast::Span& span)
{
    fullSpan.span = span;
    return cmajor::symbols::GetLineColLen(fullSpan);
}

void SystemXCodeGenerator::AddCFGItem(int prev, int next)
{
    if (!cfg) return;
    if (prev == -1 || next == -1) return;
    if (prev == next) return;
    void* mdStruct = emitter->CreateMDStruct();
    void* prevItem = emitter->CreateMDLong(prev);
    void* nextItem = emitter->CreateMDLong(next);
    emitter->AddMDItem(mdStruct, "nodeType", emitter->CreateMDLong(cfgNodeType)); 
    emitter->AddMDItem(mdStruct, "prev", prevItem);
    emitter->AddMDItem(mdStruct, "next", nextItem);
    int mdStructId = emitter->GetMDStructId(mdStruct);
    void* mdStructRef = emitter->CreateMDStructRef(mdStructId);
    emitter->AddMDArrayItem(cfg, mdStructRef);
}

void SystemXCodeGenerator::CreateMetadataForType(cmajor::symbols::TypeSymbol* type)
{
    cmajor::symbols::TypeSymbol* baseType = type->BaseType();
    if (baseType->IsClassTypeSymbol())
    {
        CreateMetadataForClassType(static_cast<cmajor::symbols::ClassTypeSymbol*>(baseType));
    }
    else if (baseType->IsArrayType())
    {
        CreateMetadataForArrayType(static_cast<cmajor::symbols::ArrayTypeSymbol*>(baseType));
    }
    else if (baseType->IsDelegateType())
    {
        CreateMetadataForDelegateType(static_cast<cmajor::symbols::DelegateTypeSymbol*>(baseType));
    }
    else if (baseType->IsClassDelegateType())
    {
        CreateMetadataForClassDelegateType(static_cast<cmajor::symbols::ClassDelegateTypeSymbol*>(baseType));
    }
}

void SystemXCodeGenerator::CreateMetadataForClassType(cmajor::symbols::ClassTypeSymbol* classTypeSymbol)
{
    void* irType = classTypeSymbol->IrType(*emitter, context);
    void* metadataRef = emitter->GetMetadataRefForStructType(irType);
    if (metadataRef) return;
    void* mdStruct = emitter->CreateMDStruct();
    emitter->AddMDItem(mdStruct, "nodeType", emitter->CreateMDLong(structInfoNodeType));
    emitter->AddMDItem(mdStruct, "typeId", emitter->CreateMDLong(emitter->GetTypeId(irType)));
    void* fullNameItem = emitter->CreateMDString(util::ToUtf8(classTypeSymbol->FullName()));
    emitter->AddMDItem(mdStruct, "fullName", fullNameItem);
    int mdStructId = emitter->GetMDStructId(mdStruct);
    void* mdStructRef = emitter->CreateMDStructRef(mdStructId);
    emitter->SetMetadataRefForStructType(irType, mdStructRef);
    void* fieldArray = emitter->CreateMDArray();
    int index = 0;
    if (classTypeSymbol->BaseClass())
    {
        void* fieldStruct = emitter->CreateMDStruct();
        emitter->AddMDItem(fieldStruct, "nodeType", emitter->CreateMDLong(fieldInfoNodeType));
        int fieldStructId = emitter->GetMDStructId(fieldStruct);
        void* fieldStructRef = emitter->CreateMDStructRef(fieldStructId);
        void* nameItem = emitter->CreateMDString("base");
        emitter->AddMDItem(fieldStruct, "name", nameItem);
        emitter->AddMDArrayItem(fieldArray, fieldStructRef);
        cmajor::symbols::TypeSymbol* baseClassType = classTypeSymbol->BaseClass();
        CreateMetadataForType(baseClassType);
        void* baseIrType = baseClassType->IrType(*emitter, context);
        int typeId = emitter->GetTypeId(baseIrType);
        if (typeId != -1)
        {
            void* typeItem = emitter->CreateMDLong(typeId);
            emitter->AddMDItem(fieldStruct, "typeId", typeItem);
        }
        int64_t offset = emitter->GetFieldOffset(irType, 0);
        emitter->AddMDItem(fieldStruct, "offset", emitter->CreateMDLong(offset));
        ++index;
    }
    else if (classTypeSymbol->IsPolymorphic())
    {
        void* fieldStruct = emitter->CreateMDStruct();
        emitter->AddMDItem(fieldStruct, "nodeType", emitter->CreateMDLong(fieldInfoNodeType));
        int fieldStructId = emitter->GetMDStructId(fieldStruct);
        void* fieldStructRef = emitter->CreateMDStructRef(fieldStructId);
        void* nameItem = emitter->CreateMDString("@vmtptr");
        emitter->AddMDItem(fieldStruct, "name", nameItem);
        emitter->AddMDArrayItem(fieldArray, fieldStructRef);
        void* vmtPtrIrType = context->RootModule()->GetSymbolTable().GetTypeByName(U"void")->AddPointer(context)->IrType(*emitter, context);
        int typeId = emitter->GetTypeId(vmtPtrIrType);
        void* typeItem = emitter->CreateMDLong(typeId);
        emitter->AddMDItem(fieldStruct, "typeId", typeItem);
        int64_t offset = emitter->GetFieldOffset(irType, 0);
        emitter->AddMDItem(fieldStruct, "offset", emitter->CreateMDLong(offset));
        ++index;
    }
    int n = static_cast<int>(classTypeSymbol->MemberVariables().size());
    if (index == 0 && n == 0)
    {
        void* fieldStruct = emitter->CreateMDStruct();
        emitter->AddMDItem(fieldStruct, "nodeType", emitter->CreateMDLong(fieldInfoNodeType));
        int fieldStructId = emitter->GetMDStructId(fieldStruct);
        void* fieldStructRef = emitter->CreateMDStructRef(fieldStructId);
        void* nameItem = emitter->CreateMDString("@first");
        emitter->AddMDItem(fieldStruct, "name", nameItem);
        emitter->AddMDArrayItem(fieldArray, fieldStructRef);
        void* byteIrType = context->RootModule()->GetSymbolTable().GetTypeByName(U"byte")->IrType(*emitter, context);
        int typeId = emitter->GetTypeId(byteIrType);
        void* typeItem = emitter->CreateMDLong(typeId);
        emitter->AddMDItem(fieldStruct, "typeId", typeItem);
        int64_t offset = emitter->GetFieldOffset(irType, 0);
        emitter->AddMDItem(fieldStruct, "offset", emitter->CreateMDLong(offset));
        ++index;
    }
    for (int i = 0; i  < n; ++i)
    {
        const auto& memberVar = classTypeSymbol->MemberVariables()[i];
        void* fieldStruct = emitter->CreateMDStruct();
        emitter->AddMDItem(fieldStruct, "nodeType", emitter->CreateMDLong(fieldInfoNodeType));
        int fieldStructId = emitter->GetMDStructId(fieldStruct);
        void* fieldStructRef = emitter->CreateMDStructRef(fieldStructId);
        void* nameItem = emitter->CreateMDString(util::ToUtf8(memberVar->Name()));
        emitter->AddMDItem(fieldStruct, "name", nameItem);
        emitter->AddMDArrayItem(fieldArray, fieldStructRef);
        cmajor::symbols::TypeSymbol* memberVarType = memberVar->GetType();
        CreateMetadataForType(memberVarType);
        void* memberVarIrType = memberVarType->IrType(*emitter, context);
        int typeId = emitter->GetTypeId(memberVarIrType);
        if (typeId != -1)
        {
            void* typeItem = emitter->CreateMDLong(typeId);
            emitter->AddMDItem(fieldStruct, "typeId", typeItem);
        }
        int64_t offset = emitter->GetFieldOffset(irType, index + i);
        emitter->AddMDItem(fieldStruct, "offset", emitter->CreateMDLong(offset));
    }
    emitter->AddMDItem(mdStruct, "fields", fieldArray);
}

void SystemXCodeGenerator::CreateMetadataForArrayType(cmajor::symbols::ArrayTypeSymbol* arrayTypeSymbol)
{
    void* irType = arrayTypeSymbol->IrType(*emitter, context);
    void* metadataRef = emitter->GetMetadataRefForArrayType(irType);
    if (metadataRef) return;
    void* mdStruct = emitter->CreateMDStruct();
    emitter->AddMDItem(mdStruct, "nodeType", emitter->CreateMDLong(structInfoNodeType));
    emitter->AddMDItem(mdStruct, "typeId", emitter->CreateMDLong(emitter->GetTypeId(irType)));
    void* fullNameItem = emitter->CreateMDString(util::ToUtf8(arrayTypeSymbol->FullName()));
    emitter->AddMDItem(mdStruct, "fullName", fullNameItem);
    void* elementIrType = elementIrType = arrayTypeSymbol->ElementType()->IrType(*emitter, context);
    emitter->AddMDItem(mdStruct, "elementTypeId", emitter->CreateMDLong(emitter->GetTypeId(elementIrType)));
    int mdStructId = emitter->GetMDStructId(mdStruct);
    void* mdStructRef = emitter->CreateMDStructRef(mdStructId);
    emitter->SetMetadataRefForArrayType(irType, mdStructRef);
}

void SystemXCodeGenerator::CreateMetadataForDelegateType(cmajor::symbols::DelegateTypeSymbol* delegateTypeSymbol)
{
    void* irType = delegateTypeSymbol->IrType(*emitter, context);
    void* metadataRef = emitter->GetMetadataRefForFunctionPointerType(irType);
    if (metadataRef) return;
    void* mdStruct = emitter->CreateMDStruct();
    emitter->AddMDItem(mdStruct, "nodeType", emitter->CreateMDLong(delegateInfoNodeType));
    emitter->AddMDItem(mdStruct, "typeId", emitter->CreateMDLong(emitter->GetBaseTypeId(irType)));
    void* fullNameItem = emitter->CreateMDString(util::ToUtf8(delegateTypeSymbol->FullName()));
    emitter->AddMDItem(mdStruct, "fullName", fullNameItem);
    int mdStructId = emitter->GetMDStructId(mdStruct);
    void* mdStructRef = emitter->CreateMDStructRef(mdStructId);
    emitter->SetMetadataRefForFunctionPointerType(irType, mdStructRef);
}

void SystemXCodeGenerator::CreateMetadataForClassDelegateType(cmajor::symbols::ClassDelegateTypeSymbol* classDelegateTypeSymbol)
{
    cmajor::symbols::ClassTypeSymbol* objectDelegatePairType = classDelegateTypeSymbol->ObjectDelegatePairType();
    CreateMetadataForClassType(objectDelegatePairType);
}

void SystemXCodeGenerator::CreateLocalNode(void* local, const std::string& name, void* type)
{
    void* mdStruct = emitter->CreateMDStruct();
    emitter->AddMDItem(mdStruct, "nodeType", emitter->CreateMDLong(localInfoNodeType));
    void* nameItem = emitter->CreateMDString(name);
    emitter->AddMDItem(mdStruct, "name", nameItem);
    int typeId = emitter->GetTypeId(type);
    emitter->AddMDItem(mdStruct, "typeId", emitter->CreateMDLong(typeId));
    int mdStructId = emitter->GetMDStructId(mdStruct);
    void* mdStructRef = emitter->CreateMDStructRef(mdStructId);
    emitter->SetLocalMetadataRef(local, mdStructRef);
}

} // namespace cmajor::systemx::backend
