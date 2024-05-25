// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.llvm.codegen.windows;

import cmajor.symbols.exception;

namespace cmajor::llvm {

WindowsCodeGenerator::WindowsCodeGenerator(cmajor::ir::Emitter* emitter_) : LLVMCodeGenerator(emitter_)
{
}

void WindowsCodeGenerator::Visit(cmajor::binder::BoundReturnStatement& boundReturnStatement)
{
    //Emitter()->SetCurrentDebugLocation(boundReturnStatement.GetSourcePos()); TODO LLVM debug info
    DestructorCallGenerated() = false;
    LastInstructionWasRet() = false;
    BasicBlockOpen() = false;
    SetTarget(&boundReturnStatement);
    cmajor::ir::Pad* prevCurrentPad = CurrentPad();
    while (CurrentPad() != nullptr)
    {
        void* returnTarget = Emitter()->CreateBasicBlock("return");
        Emitter()->CreateCatchRet(CurrentPad()->value, returnTarget);
        Emitter()->SetCurrentBasicBlock(returnTarget);
        SetCurrentPad(CurrentPad()->parent);
    }
    cmajor::binder::BoundFunctionCall* returnFunctionCall = boundReturnStatement.ReturnFunctionCall();
    if (returnFunctionCall)
    {
        returnFunctionCall->Accept(*this);
        void* returnValue = Emitter()->Stack().Pop();
        if (SequenceSecond())
        {
            SequenceSecond()->SetGenerated();
            SequenceSecond()->Accept(*this);
        }
        ExitBlocks(nullptr);
        GenerateExitFunctionCode(*CurrentFunction());
        Emitter()->CreateRet(returnValue);
        LastInstructionWasRet() = true;
    }
    else
    {
        ExitBlocks(nullptr);
        GenerateExitFunctionCode(*CurrentFunction());
        Emitter()->CreateRetVoid();
        LastInstructionWasRet() = true;
    }
    cmajor::binder::BoundCompoundStatement* body = CurrentFunction()->Body();
    cmajor::binder::BoundStatement* lastStatement = nullptr;
    if (!body->Statements().empty())
    {
        lastStatement = body->Statements().back().get();
    }
    if (lastStatement && lastStatement != &boundReturnStatement)
    {
        void* nextBlock = Emitter()->CreateBasicBlock("next");
        Emitter()->SetCurrentBasicBlock(nextBlock);
        BasicBlockOpen() = true;
        LastInstructionWasRet() = false;
    }
    SetCurrentPad(prevCurrentPad);
}

void WindowsCodeGenerator::Visit(cmajor::binder::BoundGotoCaseStatement& boundGotoCaseStatement)
{
    // Emitter()->SetCurrentDebugLocation(boundGotoCaseStatement.GetSourcePos()); TODO LLVM debug info
    DestructorCallGenerated() = false;
    LastInstructionWasRet() = false;
    BasicBlockOpen() = false;
    SetTarget(&boundGotoCaseStatement);
    Assert(BreakTargetBlock(), "break target not set");
    cmajor::ir::Pad* prevCurrentPad = CurrentPad();
    cmajor::binder::BoundStatement* parent = CurrentBlock();
    while (CurrentPad() != nullptr && parent && parent != BreakTargetBlock())
    {
        if (parent->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundTryStatement)
        {
            void* fromCatchTarget = Emitter()->CreateBasicBlock("fromCatch");
            Emitter()->CreateCatchRet(CurrentPad()->value, fromCatchTarget);
            Emitter()->SetCurrentBasicBlock(fromCatchTarget);
            SetCurrentPad(CurrentPad()->parent);
        }
        parent = parent->StatementParent();
    }
    ExitBlocks(BreakTargetBlock());
    cmajor::symbols::IntegralValue integralCaseValue(boundGotoCaseStatement.CaseValue());
    auto it = CurrentCaseMap()->find(integralCaseValue);
    if (it != CurrentCaseMap()->cend())
    {
        void* caseDest = it->second;
        Emitter()->CreateBr(caseDest);
    }
    else
    {
        throw cmajor::symbols::Exception("case not found", boundGotoCaseStatement.GetFullSpan());
    }
    SetCurrentPad(prevCurrentPad);
}

void WindowsCodeGenerator::Visit(cmajor::binder::BoundGotoDefaultStatement& boundGotoDefaultStatement)
{
    // Emitter()->SetCurrentDebugLocation(boundGotoDefaultStatement.GetSourcePos()); TODO LLVM debug info
    DestructorCallGenerated() = false;
    LastInstructionWasRet() = false;
    BasicBlockOpen() = false;
    SetTarget(&boundGotoDefaultStatement);
    Assert(BreakTargetBlock(), "break target not set");
    cmajor::ir::Pad* prevCurrentPad = CurrentPad();
    cmajor::binder::BoundStatement* parent = CurrentBlock();
    while (CurrentPad() != nullptr && parent && parent != BreakTargetBlock())
    {
        if (parent->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundTryStatement)
        {
            void* fromCatchTarget = Emitter()->CreateBasicBlock("fromCatch");
            Emitter()->CreateCatchRet(CurrentPad()->value, fromCatchTarget);
            Emitter()->SetCurrentBasicBlock(fromCatchTarget);
            SetCurrentPad(CurrentPad()->parent);
        }
        parent = parent->StatementParent();
    }
    ExitBlocks(BreakTargetBlock());
    if (DefaultDest())
    {
        Emitter()->CreateBr(DefaultDest());
    }
    else
    {
        throw cmajor::symbols::Exception("no default destination", boundGotoDefaultStatement.GetFullSpan());
    }
    SetCurrentPad(prevCurrentPad);
}

void WindowsCodeGenerator::Visit(cmajor::binder::BoundBreakStatement& boundBreakStatement)
{
    // Emitter()->SetCurrentDebugLocation(boundBreakStatement.GetSourcePos()); TODO LLVM debug info
    DestructorCallGenerated() = false;
    LastInstructionWasRet() = false;
    BasicBlockOpen() = false;
    SetTarget(&boundBreakStatement);
    Assert(BreakTarget() && BreakTargetBlock(), "break target not set");
    cmajor::ir::Pad* prevCurrentPad = CurrentPad();
    cmajor::binder::BoundStatement* parent = CurrentBlock();
    while (CurrentPad() != nullptr && parent && parent != BreakTargetBlock())
    {
        if (parent->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundTryStatement)
        {
            void* fromCatchTarget = Emitter()->CreateBasicBlock("fromCatch");
            Emitter()->CreateCatchRet(CurrentPad()->value, fromCatchTarget);
            Emitter()->SetCurrentBasicBlock(fromCatchTarget);
            SetCurrentPad(CurrentPad()->parent);
        }
        parent = parent->StatementParent();
    }
    ExitBlocks(BreakTargetBlock());
    Emitter()->CreateBr(BreakTarget());
    void* nextBlock = Emitter()->CreateBasicBlock("next");
    Emitter()->SetCurrentBasicBlock(nextBlock);
    BasicBlockOpen() = true;
    SetCurrentPad(prevCurrentPad);
}

void WindowsCodeGenerator::Visit(cmajor::binder::BoundContinueStatement& boundContinueStatement)
{
    // Emitter()->SetCurrentDebugLocation(boundContinueStatement.GetSourcePos()); TODO LLVM debug info
    DestructorCallGenerated() = false;
    LastInstructionWasRet() = false;
    BasicBlockOpen() = false;
    SetTarget(&boundContinueStatement);
    Assert(ContinueTarget() && ContinueTargetBlock(), "continue target not set");
    cmajor::ir::Pad* prevCurrentPad = CurrentPad();
    cmajor::binder::BoundStatement* parent = CurrentBlock();
    while (CurrentPad() != nullptr && parent && parent != ContinueTargetBlock())
    {
        if (parent->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundTryStatement)
        {
            void* fromCatchTarget = Emitter()->CreateBasicBlock("fromCatch");
            Emitter()->CreateCatchRet(CurrentPad()->value, fromCatchTarget);
            Emitter()->SetCurrentBasicBlock(fromCatchTarget);
            SetCurrentPad(CurrentPad()->parent);
        }
        parent = parent->StatementParent();
    }
    ExitBlocks(ContinueTargetBlock());
    Emitter()->CreateBr(ContinueTarget());
    void* nextBlock = Emitter()->CreateBasicBlock("next");
    Emitter()->SetCurrentBasicBlock(nextBlock);
    BasicBlockOpen() = true;
    SetCurrentPad(prevCurrentPad);
}

void WindowsCodeGenerator::Visit(cmajor::binder::BoundGotoStatement& boundGotoStatement)
{
    //Emitter()->SetCurrentDebugLocation(boundGotoStatement.GetSourcePos()); TODO LLVM debug info
    DestructorCallGenerated() = false;
    LastInstructionWasRet() = false;
    BasicBlockOpen() = false;
    SetTarget(&boundGotoStatement);
    cmajor::ir::Pad* prevCurrentPad = CurrentPad();
    cmajor::binder::BoundStatement* parent = CurrentBlock();
    while (CurrentPad() != nullptr && parent && parent != boundGotoStatement.TargetBlock())
    {
        if (parent->GetBoundNodeType() == cmajor::binder::BoundNodeType::boundTryStatement)
        {
            void* fromCatchTarget = Emitter()->CreateBasicBlock("fromCatch");
            Emitter()->CreateCatchRet(CurrentPad()->value, fromCatchTarget);
            Emitter()->SetCurrentBasicBlock(fromCatchTarget);
            SetCurrentPad(CurrentPad()->parent);
        }
        parent = parent->StatementParent();
    }
    ExitBlocks(boundGotoStatement.TargetBlock());
    auto it = LabeledStatementMap().find(boundGotoStatement.TargetStatement());
    if (it != LabeledStatementMap().cend())
    {
        void* target = it->second;
        Emitter()->CreateBr(target);
    }
    else
    {
        throw cmajor::symbols::Exception("goto target not found", boundGotoStatement.GetFullSpan());
    }
    void* nextBlock = Emitter()->CreateBasicBlock("next");
    Emitter()->SetCurrentBasicBlock(nextBlock);
    BasicBlockOpen() = true;
    SetCurrentPad(prevCurrentPad);
}

void WindowsCodeGenerator::Visit(cmajor::binder::BoundTryStatement& boundTryStatement)
{
    DestructorCallGenerated() = false;
    LastInstructionWasRet() = false;
    BasicBlockOpen() = false;
    SetTarget(&boundTryStatement);
    void* prevHandlerBlock = HandlerBlock();
    void* prevCleanupBlock = CleanupBlock();
    SetHandlerBlock(Emitter()->CreateBasicBlock("handlers"));
    SetCleanupBlock(nullptr);
    boundTryStatement.TryBlock()->Accept(*this);
    void* nextTarget = Emitter()->CreateBasicBlock("next");
    Emitter()->CreateBr(nextTarget);
    Emitter()->SetCurrentBasicBlock(HandlerBlock());
    SetHandlerBlock(prevHandlerBlock);
    cmajor::ir::Pad* parentPad = CurrentPad();
    void* catchSwitch = nullptr;
    if (parentPad == nullptr)
    {
        catchSwitch = Emitter()->CreateCatchSwitch(prevHandlerBlock);
    }
    else
    {
        catchSwitch = Emitter()->CreateCatchSwitchWithParent(parentPad->value, prevHandlerBlock);
    }
    cmajor::ir::Pad* pad = new cmajor::ir::Pad();
    Pads().push_back(std::unique_ptr<cmajor::ir::Pad>(pad));
    pad->parent = parentPad;
    pad->value = catchSwitch;
    SetCurrentPad(pad);
    void* catchPadTarget = Emitter()->CreateBasicBlock("catchpad");
    Emitter()->AddHandlerToCatchSwitch(catchSwitch, catchPadTarget);
    Emitter()->SetCurrentBasicBlock(catchPadTarget);
    std::vector<void*> catchPadArgs;
    catchPadArgs.push_back(Emitter()->CreateDefaultIrValueForVoidPtrType());
    catchPadArgs.push_back(Emitter()->CreateIrValueForInt(64));
    catchPadArgs.push_back(Emitter()->CreateDefaultIrValueForVoidPtrType());
    void* catchPad = Emitter()->CreateCatchPad(CurrentPad()->value, catchPadArgs);
    CurrentPad()->value = catchPad;
    void* catchTarget = Emitter()->CreateBasicBlock("catch");
    void* resumeTarget = Emitter()->CreateBasicBlock("resume");
    Emitter()->CreateBr(catchTarget);
    int n = boundTryStatement.Catches().size();
    for (int i = 0; i < n; ++i)
    {
        const std::unique_ptr<cmajor::binder::BoundCatchStatement>& boundCatchStatement = boundTryStatement.Catches()[i];
        Emitter()->SetCurrentBasicBlock(catchTarget);
        std::vector<void*> handleExceptionParamTypes;
        handleExceptionParamTypes.push_back(Emitter()->GetIrTypeForVoidPtrType());
        void* handleExceptionFunctionType = Emitter()->GetIrTypeForFunction(Emitter()->GetIrTypeForBool(), handleExceptionParamTypes);
        std::vector<void*> handleExceptionArgs;
        cmajor::symbols::UuidValue uuidValue(boundCatchStatement->GetSpan(), boundCatchStatement->CatchTypeUuidId());
        void* catchTypeIdValue = uuidValue.IrValue(*Emitter());
        handleExceptionArgs.push_back(catchTypeIdValue);
        void* handleException = Emitter()->GetOrInsertFunction("RtHandleException", handleExceptionFunctionType, true);
        void* handleThisEx = nullptr;
        if (CurrentPad() == nullptr)
        {
            handleThisEx = Emitter()->CreateCall(handleExceptionFunctionType, handleException, handleExceptionArgs);
        }
        else
        {
            std::vector<void*> bundles;
            bundles.push_back(CurrentPad()->value);
            handleThisEx = Emitter()->CreateCallInst(handleExceptionFunctionType, handleException, handleExceptionArgs, bundles);
        }
        void* nextHandlerTarget = nullptr;
        if (i < n - 1)
        {
            catchTarget = Emitter()->CreateBasicBlock("catch");
            nextHandlerTarget = catchTarget;
        }
        else
        {
            nextHandlerTarget = resumeTarget;
        }
        void* thisHandlerTarget = Emitter()->CreateBasicBlock("handler");
        Emitter()->CreateCondBr(handleThisEx, thisHandlerTarget, nextHandlerTarget);
        Emitter()->SetCurrentBasicBlock(thisHandlerTarget);
        boundCatchStatement->CatchBlock()->Accept(*this);
        Emitter()->CreateCatchRet(CurrentPad()->value, nextTarget);
    }
    Emitter()->SetCurrentBasicBlock(resumeTarget);
    std::vector<void*> cxxThrowFunctionParamTypes;
    cxxThrowFunctionParamTypes.push_back(Emitter()->GetIrTypeForVoidPtrType());
    cxxThrowFunctionParamTypes.push_back(Emitter()->GetIrTypeForVoidPtrType());
    void* cxxThrowFunctionType = Emitter()->GetIrTypeForFunction(Emitter()->GetIrTypeForVoid(), cxxThrowFunctionParamTypes);
    void* cxxThrowFunction = Emitter()->GetOrInsertFunction("_CxxThrowException", cxxThrowFunctionType, false);
    std::vector<void*> rethrowArgs;
    rethrowArgs.push_back(Emitter()->CreateDefaultIrValueForVoidPtrType());
    rethrowArgs.push_back(Emitter()->CreateDefaultIrValueForVoidPtrType());
    std::vector<void*> bundles;
    bundles.push_back(CurrentPad()->value);
    Emitter()->CreateCallInstToBasicBlock(cxxThrowFunctionType, cxxThrowFunction, rethrowArgs, bundles, resumeTarget, soul::ast::LineColLen()); // TODO LLVM Debug info
    Emitter()->CreateBr(nextTarget);
    SetCurrentPad(parentPad);
    Emitter()->SetCurrentBasicBlock(nextTarget);
    BasicBlockOpen() = true;
    SetCleanupBlock(prevCleanupBlock);
}

void WindowsCodeGenerator::Visit(cmajor::binder::BoundRethrowStatement& boundRethrowStatement)
{
    //Emitter()->SetCurrentDebugLocation(boundRethrowStatement.GetSourcePos()); // TODO LLVM debug info
    DestructorCallGenerated() = false;
    LastInstructionWasRet() = false;
    BasicBlockOpen() = false;
    SetTarget(&boundRethrowStatement);
    boundRethrowStatement.ReleaseCall()->Accept(*this);
    if (Emitter()->DIBuilder())
    {
        //Emitter()->SetCurrentDebugLocation(boundRethrowStatement.GetSourcePos()); TODO LLVM debug info
    }
    std::vector<void*> cxxThrowFunctionParamTypes;
    cxxThrowFunctionParamTypes.push_back(Emitter()->GetIrTypeForVoidPtrType());
    cxxThrowFunctionParamTypes.push_back(Emitter()->GetIrTypeForVoidPtrType());
    void* cxxThrowFunctionType = Emitter()->GetIrTypeForFunction(Emitter()->GetIrTypeForVoid(), cxxThrowFunctionParamTypes);
    void* cxxThrowFunction = Emitter()->GetOrInsertFunction("_CxxThrowException", cxxThrowFunctionType, false);
    std::vector<void*> rethrowArgs;
    rethrowArgs.push_back(Emitter()->CreateDefaultIrValueForVoidPtrType());
    rethrowArgs.push_back(Emitter()->CreateDefaultIrValueForVoidPtrType());
    std::vector<void*> bundles;
    bundles.push_back(CurrentPad()->value);
    void* callInst = Emitter()->CreateCallInst(cxxThrowFunctionType, cxxThrowFunction, rethrowArgs, bundles);
}

void* WindowsCodeGenerator::GetPersonalityFunction() const
{
    void* personalityFunctionType = Emitter()->GetIrTypeForVariableParamFunction(Emitter()->GetIrTypeForInt());
    void* personalityFunction = Emitter()->GetOrInsertFunction("__CxxFrameHandler3", personalityFunctionType, false);
    return personalityFunction;
}

void WindowsCodeGenerator::GenerateCodeForCleanups()
{
    for (const std::unique_ptr<Cleanup>& cleanup : Cleanups())
    {
        Emitter()->SetCurrentBasicBlock(cleanup->cleanupBlock);
        cmajor::ir::Pad* parentPad = cleanup->currentPad;
        void* cleanupPad = nullptr;
        if (parentPad)
        {
            std::vector<void*> args;
            cleanupPad = Emitter()->CreateCleanupPadWithParent(parentPad->value, args);
        }
        else
        {
            std::vector<void*> args;
            cleanupPad = Emitter()->CreateCleanupPad(args);
        }
        cmajor::ir::Pad pad;
        pad.parent = parentPad;
        pad.value = cleanupPad;
        SetCurrentPad(&pad);
        for (const std::unique_ptr<cmajor::binder::BoundFunctionCall>& destructorCall : cleanup->destructors)
        {
            destructorCall->Accept(*this);
        }
        void* unwindTarget = cleanup->handlerBlock;
        Emitter()->CreateCleanupRet(cleanupPad, unwindTarget);
    }
}

void WindowsCodeGenerator::CreateCleanup()
{
    SetCleanupBlock(Emitter()->CreateBasicBlock("cleanup"));
    cmajor::binder::BoundCompoundStatement* targetBlock = nullptr;
    cmajor::binder::BoundNode* parent = CurrentBlock()->Parent();
    while (parent && parent->GetBoundNodeType() != cmajor::binder::BoundNodeType::boundTryStatement)
    {
        parent = parent->Parent();
    }
    if (parent)
    {
        cmajor::binder::BoundTryStatement* tryStatement = static_cast<cmajor::binder::BoundTryStatement*>(parent);
        targetBlock = tryStatement->Block();
    }
    Cleanup* cleanup = new Cleanup(CleanupBlock(), HandlerBlock(), CurrentPad());
    int n = Blocks().size();
    for (int i = n - 1; i >= 0; --i)
    {
        cmajor::binder::BoundCompoundStatement* block = Blocks()[i];
        if (block == targetBlock)
        {
            break;
        }
        auto it = BlockDestructionMap().find(block);
        if (it != BlockDestructionMap().cend())
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
    Cleanups().push_back(std::unique_ptr<Cleanup>(cleanup));
    SetNewCleanupNeeded(false);
}

} // namespace cmajor::llvm
