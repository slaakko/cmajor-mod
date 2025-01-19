// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.optimizer.inline_expander;

import util;

namespace cmajor::masm::optimizer {

void ReplaceParamsWithArgs(cmajor::masm::intermediate::Instruction* callInst, cmajor::masm::intermediate::ArgInstruction* firstArg, 
    cmajor::masm::intermediate::Function* callee, cmajor::masm::intermediate::Function* originalCallee, cmajor::masm::intermediate::Context* context)
{
    cmajor::masm::intermediate::BasicBlock* bb = callee->FirstBasicBlock();
    while (bb)
    {
        cmajor::masm::intermediate::Instruction* inst = bb->FirstInstruction();
        while (inst)
        {
            cmajor::masm::intermediate::Instruction* next = inst->Next();
            if (inst->IsParamInstruction())
            {
                if (firstArg)
                {
                    inst->ReplaceUsesWith(firstArg->Arg());
                    cmajor::masm::intermediate::Instruction* nextArg = firstArg->Next();
                    firstArg->RemoveFromUses();
                    firstArg->Parent()->RemoveInstruction(firstArg);
                    if (nextArg && nextArg->IsArgInstruction())
                    {
                        firstArg = static_cast<cmajor::masm::intermediate::ArgInstruction*>(nextArg);
                    }
                    else
                    {
                        firstArg = nullptr;
                    }
                }
                else
                {
                    cmajor::masm::intermediate::Error("number of arguments and parameters do not match", callInst->Span(), context, originalCallee->Span());
                }
                inst->RemoveFromUses();
                inst->Parent()->RemoveInstruction(inst);
            }
            else
            {
                break;
            }
            inst = next;
        }
        bb = bb->Next();
    }
}

void MoveInstructions(cmajor::masm::intermediate::BasicBlock* fromBasicBlock, cmajor::masm::intermediate::Instruction* afterCallInst, 
    std::vector<cmajor::masm::intermediate::RetInstruction*>& rets)
{
    cmajor::masm::intermediate::Instruction* inst = fromBasicBlock->FirstInstruction();
    while (inst)
    {
        cmajor::masm::intermediate::Instruction* next = inst->Next();
        std::unique_ptr<cmajor::masm::intermediate::Instruction> instToMove = inst->Parent()->RemoveInstruction(inst);
        if (instToMove->IsRetInstruction())
        {
            rets.push_back(static_cast<cmajor::masm::intermediate::RetInstruction*>(instToMove.get()));
        }
        afterCallInst->Parent()->InsertInstructionAfter(instToMove.release(), afterCallInst);
        afterCallInst = afterCallInst->Next();
        inst = next;
    }
}

void MergeSingleBasicBlock(cmajor::masm::intermediate::Function* function, cmajor::masm::intermediate::BasicBlock* basicBlock, 
    cmajor::masm::intermediate::Instruction* callInst, cmajor::masm::intermediate::Context* context)
{
    std::vector<cmajor::masm::intermediate::RetInstruction*> rets;
    MoveInstructions(basicBlock, callInst, rets);
    cmajor::masm::intermediate::Function* callee = basicBlock->Parent();
    callee->MoveRegValues(function);
    function->SetNumbers();
    if (callInst->IsFunctionCallInstruction())
    {
        if (rets.size() == 1)
        {
            cmajor::masm::intermediate::RetInstruction* ret = rets.front();
            if (ret->ReturnValue())
            {
                callInst->ReplaceUsesWith(ret->ReturnValue());
            }
            else
            {
                Error("return value expected", ret->Span(), context);
            }
        }
        else
        {
            Error("single ret instruction expected", callee->Span(), context);
        }
    }
    for (cmajor::masm::intermediate::RetInstruction* ret : rets)
    {
        ret->RemoveFromUses();
        ret->Parent()->RemoveInstruction(ret);
    }
    callInst->RemoveFromUses();
    callInst->Parent()->RemoveInstruction(callInst);
}

void MergeManyBasicBlocks(cmajor::masm::intermediate::Function* function, cmajor::masm::intermediate::Function* callee, cmajor::masm::intermediate::Instruction* callInst, 
    cmajor::masm::intermediate::Context* context)
{
    cmajor::masm::intermediate::Instruction* lastInst = callInst;
    cmajor::masm::intermediate::LocalInstruction* localInst = nullptr;
    cmajor::masm::intermediate::Type* returnType = nullptr;
    cmajor::masm::intermediate::BasicBlock* targetBB = callInst->Parent()->SplitAfter(callInst);
    if (callInst->IsFunctionCallInstruction())
    {
        returnType = callee->GetType()->ReturnType();
        localInst = new cmajor::masm::intermediate::LocalInstruction(callInst->Span(), function->MakeNextRegValue(callInst->Span(), returnType->AddPointer(context), context), 
            returnType);
        localInst->Result()->SetInst(localInst);
        callInst->Parent()->InsertInstructionAfter(localInst, callInst);
        lastInst = localInst;
    }
    cmajor::masm::intermediate::BasicBlock* firstMergedBasicBlock = callee->FirstBasicBlock();
    cmajor::masm::intermediate::BasicBlock* toMerge = firstMergedBasicBlock;
    while (toMerge)
    {
        cmajor::masm::intermediate::BasicBlock* next = toMerge->Next();
        std::unique_ptr<cmajor::masm::intermediate::BasicBlock> removedBasicBlock = callee->RemoveBasicBlock(toMerge);
        removedBasicBlock->SetId(function->GetNextBasicBlockNumber());
        removedBasicBlock->SetContainer(function->BasicBlocks());
        cmajor::masm::intermediate::Instruction* lastInst = removedBasicBlock->LastInstruction();
        if (lastInst->IsRetInstruction())
        {
            cmajor::masm::intermediate::RetInstruction* ret = static_cast<cmajor::masm::intermediate::RetInstruction*>(lastInst);
            if (callInst->IsFunctionCallInstruction())
            {
                if (ret->ReturnValue())
                {
                    cmajor::masm::intermediate::StoreInstruction* storeInst = new cmajor::masm::intermediate::StoreInstruction(ret->Span(), ret->ReturnValue(), 
                        localInst->Result());
                    storeInst->AddToUses();
                    ret->Parent()->InsertInstructionAfter(storeInst, ret);
                    lastInst = storeInst;
                }
                else
                {
                    Error("return value expected", ret->Span(), context);
                }
            }
            cmajor::masm::intermediate::JmpInstruction* jmp = new cmajor::masm::intermediate::JmpInstruction(ret->Span(), targetBB->Id());
            jmp->SetTargetBasicBlock(targetBB);
            lastInst->Parent()->InsertInstructionAfter(jmp, lastInst);
            ret->RemoveFromUses();
            removedBasicBlock->RemoveInstruction(ret);
        }
        removedBasicBlock->SetContainer(nullptr);
        function->InsertBasicBlockBefore(removedBasicBlock.release(), targetBB);
        toMerge = next;
    }
    if (callInst->IsFunctionCallInstruction())
    {
        cmajor::masm::intermediate::LoadInstruction* loadInst = new cmajor::masm::intermediate::LoadInstruction(localInst->Span(),
            function->MakeNextRegValue(localInst->Span(), returnType, context), localInst->Result());
        loadInst->Result()->SetInst(loadInst);
        targetBB->InsertFront(loadInst);
        loadInst->AddToUses();
        callInst->ReplaceUsesWith(loadInst->Result());
    }
    cmajor::masm::intermediate::JmpInstruction* jmp = new cmajor::masm::intermediate::JmpInstruction(callInst->Span(), firstMergedBasicBlock->Id());
    jmp->SetTargetBasicBlock(firstMergedBasicBlock);
    callInst->Parent()->InsertInstructionAfter(jmp, lastInst);
    callInst->RemoveFromUses();
    callInst->Parent()->RemoveInstruction(callInst);
    callee->MoveRegValues(function);
}

void MergeBasicBlocks(cmajor::masm::intermediate::Function* function, cmajor::masm::intermediate::Function* callee, cmajor::masm::intermediate::Instruction* callInst, 
    cmajor::masm::intermediate::Context* context)
{
    if (callee->NumBasicBlocks() == 1)
    {
        MergeSingleBasicBlock(function, callee->FirstBasicBlock(), callInst, context);
    }
    else if (callee->NumBasicBlocks() > 1)
    {
        MergeManyBasicBlocks(function, callee, callInst, context);
    }
    else
    {
        Error("callee has no basic blocks", callee->Span(), context);
    }
}

void InlineExpand(cmajor::masm::intermediate::Function* function, cmajor::masm::intermediate::Instruction* callInst, cmajor::masm::intermediate::ArgInstruction* firstArg, 
    cmajor::masm::intermediate::Function* callee, cmajor::masm::intermediate::Context* context)
{
    std::unique_ptr<cmajor::masm::intermediate::Function> clonedCallee(callee->Clone());
    //clonedCallee->Check();
    ReplaceParamsWithArgs(callInst, firstArg, clonedCallee.get(), callee, context);
    MergeBasicBlocks(function, clonedCallee.get(), callInst, context);
    function->SetNumbers();
    //function->Check();
}

bool InlineExpand(cmajor::masm::intermediate::Function* function)
{
    //function->Check();
    bool inlineExpanded = false;
    cmajor::masm::intermediate::Context* context = function->Parent()->GetContext();
    int inlineDepth = context->InlineDepth();
    int inlineCount = 0;
    bool inlined = false;
    do
    {
        inlined = false;
        cmajor::masm::intermediate::ArgInstruction* firstArg = nullptr;
        cmajor::masm::intermediate::BasicBlock* bb = function->FirstBasicBlock();
        while (bb)
        {
            cmajor::masm::intermediate::Instruction* inst = bb->FirstInstruction();
            while (inst)
            {
                cmajor::masm::intermediate::Instruction* next = inst->Next();
                if (inst->IsArgInstruction())
                {
                    if (!firstArg)
                    {
                        firstArg = static_cast<cmajor::masm::intermediate::ArgInstruction*>(inst);
                    }
                }
                cmajor::masm::intermediate::Function* callee = nullptr;
                bool isCallInst = false;
                if (inst->IsProcedureCallInstruction())
                {
                    cmajor::masm::intermediate::ProcedureCallInstruction* procedureCallInst = static_cast<cmajor::masm::intermediate::ProcedureCallInstruction*>(inst);
                    callee = procedureCallInst->CalleeFn();
                    isCallInst = true;
                }
                else if (inst->IsFunctionCallInstruction())
                {
                    cmajor::masm::intermediate::FunctionCallInstruction* functionCallInst = static_cast<cmajor::masm::intermediate::FunctionCallInstruction*>(inst);
                    callee = functionCallInst->CalleeFn();
                    isCallInst = true;
                }
                if (callee)
                {
                    if (callee != function)
                    {
                        if (callee->IsInline())
                        {
                            InlineExpand(function, inst, firstArg, callee, context);
                            inlined = true;
                            inlineExpanded = true;
                        }
                    }
                }
                if (isCallInst)
                {
                    firstArg = nullptr;
                }
                inst = next;
            }
            bb = bb->Next();
        }
    } 
    while (inlined && ++inlineCount < inlineDepth);
    if (inlineCount > 1)
    {
        //std::cout << inlineCount << " inline count for " << function->Name() << "\n";
    }
    return inlineExpanded;
}

} // cmajor::masm::optimizer
