// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.masm.optimizer.identity_operation_remover;

namespace cmajor::masm::optimizer {

bool IsIdentityOperation(cmajor::masm::intermediate::Function* function)
{
    bool isRvalue = function->Name().starts_with("function_Rvalue");
    cmajor::masm::intermediate::FunctionType* functionType = function->GetType();
    if (functionType->IsUnaryOperationType())
    {
        if (function->NumBasicBlocks() == 1)
        {
            cmajor::masm::intermediate::BasicBlock* bb = function->FirstBasicBlock();
            cmajor::masm::intermediate::Instruction* inst0 = bb->FirstInstruction();
            if (inst0 && inst0->IsParamInstruction())
            {
                cmajor::masm::intermediate::Instruction* inst1 = inst0->Next();
                if (inst1 && inst1->IsLocalInstruction())
                {
                    cmajor::masm::intermediate::Instruction* inst2 = inst1->Next();
                    if (inst2 && inst2->IsStoreInstruction())
                    {
                        cmajor::masm::intermediate::Instruction* inst3 = inst2->Next();
                        if (inst3 && inst3->IsLoadInstruction())
                        {
                            cmajor::masm::intermediate::Instruction* inst4 = inst3->Next();
                            if (inst4 && inst4->IsRetInstruction())
                            {
                                if (!inst4->Next())
                                {
                                    cmajor::masm::intermediate::Type* paramType = functionType->ParamType(0);
                                    cmajor::masm::intermediate::ParamInstruction* paramInst = static_cast<cmajor::masm::intermediate::ParamInstruction*>(inst0);
                                    if (paramInst->Result()->GetType() == paramType)
                                    {
                                        cmajor::masm::intermediate::LocalInstruction* localInst = static_cast<cmajor::masm::intermediate::LocalInstruction*>(inst1);
                                        if (localInst->LocalType() == paramType)
                                        {
                                            cmajor::masm::intermediate::StoreInstruction* storeInst = static_cast<cmajor::masm::intermediate::StoreInstruction*>(inst2);
                                            if (storeInst->GetValue()->GetInstruction() == paramInst && storeInst->GetPtr()->GetInstruction() == localInst)
                                            {
                                                cmajor::masm::intermediate::LoadInstruction* loadInst = static_cast<cmajor::masm::intermediate::LoadInstruction*>(inst3);
                                                if (loadInst->Ptr()->GetInstruction() == localInst)
                                                {
                                                    cmajor::masm::intermediate::Value* loadedValue = loadInst->Result();
                                                    cmajor::masm::intermediate::RetInstruction* retInst = static_cast<cmajor::masm::intermediate::RetInstruction*>(inst4);
                                                    if (retInst->ReturnValue() == loadedValue)
                                                    {
                                                        return true;
                                                    }
                                                    else
                                                    {
                                                        if (isRvalue)
                                                        {
                                                            int x = 0;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    if (isRvalue)
                                                    {
                                                        int x = 0;
                                                    }
                                                }

                                            }
                                            else
                                            {
                                                if (isRvalue)
                                                {
                                                    int x = 0;
                                                }
                                            }

                                        }
                                        else
                                        {
                                            if (isRvalue)
                                            {
                                                int x = 0;
                                            }
                                        }

                                    }
                                    else
                                    {
                                        if (isRvalue)
                                        {
                                            int x = 0;
                                        }
                                    }

                                }
                                else
                                {
                                    if (isRvalue)
                                    {
                                        int x = 0;
                                    }
                                }

                            }
                            else
                            {
                                if (isRvalue)
                                {
                                    int x = 0;
                                }
                            }

                        }
                        else
                        {
                            if (isRvalue)
                            {
                                int x = 0;
                            }
                        }

                    }
                    else
                    {
                        if (isRvalue)
                        {
                            int x = 0;
                        }
                    }

                }
                else
                {
                    if (isRvalue)
                    {
                        int x = 0;
                    }
                }

            }
            else
            {
                if (isRvalue)
                {
                    int x = 0;
                }
            }

        }
        else
        {
            if (isRvalue)
            {
                int x = 0;
            }
        }
    }
    else
    {
        if (isRvalue)
        {
            int x = 0;
        }
    }
    return false;
}

void RemoveIdentityFunctionCall(cmajor::masm::intermediate::FunctionCallInstruction* identityFunctionCallInst, cmajor::masm::intermediate::Context* context)
{
    if (identityFunctionCallInst->Args().size() != 1)
    {
        Error("one arg expected", identityFunctionCallInst->Span(), context);
    }
    cmajor::masm::intermediate::Instruction* prev = identityFunctionCallInst->Prev();
    if (!prev || !prev->IsArgInstruction())
    {
        Error("arg prev instruction expected", identityFunctionCallInst->Span(), context);
    }
    cmajor::masm::intermediate::ArgInstruction* argInst = static_cast<cmajor::masm::intermediate::ArgInstruction*>(prev);
    identityFunctionCallInst->ReplaceUsesWith(argInst->Arg());
    cmajor::masm::intermediate::Instruction* inst = argInst->Arg()->GetInstruction();
    if (inst)
    {
        inst->RemoveUser(argInst);
        inst->RemoveUser(identityFunctionCallInst);
    }
    argInst->Parent()->RemoveInstruction(argInst);
    identityFunctionCallInst->Parent()->RemoveInstruction(identityFunctionCallInst);
}

void RemoveIdentityFunctionCalls(cmajor::masm::intermediate::Function* function)
{
    bool removed = false;
    cmajor::masm::intermediate::Context* context = function->Parent()->GetContext();
    cmajor::masm::intermediate::BasicBlock* bb = function->FirstBasicBlock();
    while (bb)
    {
        cmajor::masm::intermediate::Instruction* inst = bb->FirstInstruction();
        while (inst)
        {
            cmajor::masm::intermediate::Instruction* next = inst->Next();
            if (inst->IsFunctionCallInstruction())
            {
                cmajor::masm::intermediate::FunctionCallInstruction* functionCallInst = static_cast<cmajor::masm::intermediate::FunctionCallInstruction*>(inst);
                cmajor::masm::intermediate::Function* callee = functionCallInst->CalleeFn();
                if (callee && IsIdentityOperation(callee))
                {
                    RemoveIdentityFunctionCall(functionCallInst, context);
                    removed = true;
                }
            }
            inst = next;
        }
        bb = bb->Next();
    }
    if (removed)
    {
        function->SetNumbers();
    }
}

} // cmajor::masm::optimizer
