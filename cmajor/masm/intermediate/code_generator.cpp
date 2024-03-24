// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.code_generator;

import cmajor.masm.intermediate.code;
import cmajor.masm.intermediate.data;
import cmajor.masm.intermediate.types;
import cmajor.masm.intermediate.context;
import cmajor.masm.intermediate.location;
import cmajor.masm.intermediate.register_allocator;
import cmajor.masm.intermediate.linear_scan_register_allocator;
import cmajor.masm.intermediate.error;
import util;

namespace cmajor::masm::intermediate {

void EmitIntegerLoad(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator);
void EmitFloatingPointLoad(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator);
cmajor::masm::assembly::Register* MakeIntegerRegOperand(Value* value, cmajor::masm::assembly::Register* reg, CodeGenerator& codeGenerator);
cmajor::masm::assembly::Register* MakeFloatingPointRegOperand(Value* value, cmajor::masm::assembly::Register* reg, CodeGenerator& codeGenerator);
FrameLocation GetFrameLocation(Value* value, CodeGenerator& codeGenerator);
void EmitFrameLocationOperand(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator);

void StoreParamToHome(ParamInstruction* inst, CodeGenerator& codeGenerator)
{
    Type* type = inst->GetType();
    int64_t size = type->Size();
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::Register* paramReg = nullptr;
    if (!inst->IsFloatingPointInstruction())
    {
        switch (inst->Index())
        {
            case 0: paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rcx); break;
            case 1: paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rdx); break;
            case 2: paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r8); break;
            case 3: paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r9); break;
        }
    }
    if (paramReg)
    {
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::MOV;
        cmajor::masm::assembly::Instruction* movToHomeInst = new cmajor::masm::assembly::Instruction(opCode);
        FrameLocation home = GetFrameLocation(inst, codeGenerator);
        if (home.Valid())
        {
            EmitFrameLocationOperand(size, home, movToHomeInst, codeGenerator);
            movToHomeInst->AddOperand(paramReg);
            codeGenerator.Emit(movToHomeInst);
        }
        else
        {
            codeGenerator.Error("error store to home: invalid home location for 'param' instruction");
        }
    }
}

void EmitPrologue(CodeGenerator& codeGenerator)
{
    Function* function = codeGenerator.CurrentFunction();
    if (!function) return;
    BasicBlock* entryBlock = function->FirstBasicBlock();
    if (!entryBlock) return;
    Instruction* inst = entryBlock->FirstInstruction();
    if (!inst) return;
    while (inst)
    {
        if (inst->IsParamInstruction())
        {
            StoreParamToHome(static_cast<ParamInstruction*>(inst), codeGenerator);
        }
        else
        {
            break;
        }
        inst = inst->Next();
    }

    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();

    int numPushes = 0;
    cmajor::masm::assembly::Instruction* pushRbp = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::PUSH);
    pushRbp->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbp));
    codeGenerator.Emit(pushRbp);
    ++numPushes;

    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedLocalRegs())
    {
        cmajor::masm::assembly::Instruction* pushReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::PUSH);
        pushReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(pushReg);
        ++numPushes;
    }
    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedNonvolatileRegs())
    {
        cmajor::masm::assembly::Instruction* pushReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::PUSH);
        pushReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(pushReg);
        ++numPushes;
    }

    Frame& frame = codeGenerator.RegAllocator()->GetFrame();

    if ((numPushes % 2) == 1)
    {
        cmajor::masm::assembly::Instruction* pushReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::PUSH);
        pushReg->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbx, false));
        codeGenerator.Emit(pushReg);
        frame.SetRbxPushed();
    }

    int nxmmregs = assemblyContext->GetRegisterPool()->UsedLocalXMMRegs().size() + assemblyContext->GetRegisterPool()->UsedNonvolatileXMMRegs().size();
    frame.SetNumUsedXMMRegs(nxmmregs);
    frame.SetMacroValues(assemblyContext);

    cmajor::masm::assembly::Instruction* subRsp = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::SUB);
    subRsp->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp));
    subRsp->AddOperand(assemblyContext->MakeIntegerLiteral(frame.Size(), 16));
    codeGenerator.Emit(subRsp);

    if (nxmmregs > 0)
    {
        int xmmRegIndex = 0;
        for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedLocalXMMRegs())
        {
            cmajor::masm::assembly::Instruction* movReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOVSD);
            movReg->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(
                assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp), 
                assemblyContext->MakeIntegerLiteral(xmmRegIndex * 16, 8), cmajor::masm::assembly::Operator::add))));
            movReg->AddOperand(regGroup->GetReg(16));
            codeGenerator.Emit(movReg);
            ++xmmRegIndex;
        }
        for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedNonvolatileXMMRegs())
        {
            cmajor::masm::assembly::Instruction* movReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOVSD);
            movReg->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(
                assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp),
                assemblyContext->MakeIntegerLiteral(xmmRegIndex * 16, 8), cmajor::masm::assembly::Operator::add))));
            movReg->AddOperand(regGroup->GetReg(16));
            codeGenerator.Emit(movReg);
            ++xmmRegIndex;
        }
    }

    cmajor::masm::assembly::Instruction* leaRbp = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
    leaRbp->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbp));
    leaRbp->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
        assemblyContext->MakeBinaryExpr(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp),
            assemblyContext->MakeIntegerLiteral(frame.CalleeParamAreaSize() + frame.XMMSaveRegSize(), 8), cmajor::masm::assembly::Operator::add))));
    codeGenerator.Emit(leaRbp);
}

void EmitEpilogue(CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Instruction* leaderInst = nullptr;
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();

    Frame& frame = codeGenerator.RegAllocator()->GetFrame();

    int nxmmregs = frame.GetNumUsedXMMRegs(); 

    if (nxmmregs > 0)
    {
        int xmmRegIndex = nxmmregs - 1;

        std::vector<cmajor::masm::assembly::RegisterGroup*> usedNVXMMRegs;
        for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedNonvolatileXMMRegs())
        {
            usedNVXMMRegs.push_back(regGroup);
        }
        std::reverse(usedNVXMMRegs.begin(), usedNVXMMRegs.end());
        for (const auto& regGroup : usedNVXMMRegs)
        {
            cmajor::masm::assembly::Instruction* movReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOVSD);
            if (!leaderInst)
            {
                leaderInst = movReg;
                leaderInst->SetLabel("@" + std::to_string(codeGenerator.ExitLabelId()));
            }
            movReg->AddOperand(regGroup->GetReg(16));
            movReg->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(
                assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp),
                assemblyContext->MakeIntegerLiteral(xmmRegIndex * 16, 8), cmajor::masm::assembly::Operator::add))));
            codeGenerator.Emit(movReg);
            --xmmRegIndex;
        }

        std::vector<cmajor::masm::assembly::RegisterGroup*> usedLocalXMMRegs;
        for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedLocalXMMRegs())
        {
            usedLocalXMMRegs.push_back(regGroup);
        }
        std::reverse(usedLocalXMMRegs.begin(), usedLocalXMMRegs.end());
        for (const auto& regGroup : usedLocalXMMRegs)
        {
            cmajor::masm::assembly::Instruction* movReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOVSD);
            if (!leaderInst)
            {
                leaderInst = movReg;
                leaderInst->SetLabel("@" + std::to_string(codeGenerator.ExitLabelId()));
            }
            movReg->AddOperand(regGroup->GetReg(16));
            movReg->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(
                assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp),
                assemblyContext->MakeIntegerLiteral(xmmRegIndex * 16, 8), cmajor::masm::assembly::Operator::add))));
            codeGenerator.Emit(movReg);
            --xmmRegIndex;
        }
    }

    cmajor::masm::assembly::Instruction* leaRsp = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
    if (!leaderInst)
    {
        leaderInst = leaRsp;
        leaderInst->SetLabel("@" + std::to_string(codeGenerator.ExitLabelId()));
    }
    leaRsp->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp));
    leaRsp->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
        assemblyContext->MakeBinaryExpr(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbp),
            assemblyContext->MakeIntegerLiteral(frame.Size() - frame.CalleeParamAreaSize() - frame.XMMSaveRegSize(), 8), cmajor::masm::assembly::Operator::add))));
    codeGenerator.Emit(leaRsp);

    if (frame.RbxPushed())
    {
        cmajor::masm::assembly::Instruction* popReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::POP);
        popReg->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbx, false));
        codeGenerator.Emit(popReg);
    }

    std::vector<cmajor::masm::assembly::RegisterGroup*> usedNVRegs;
    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedNonvolatileRegs())
    {
        usedNVRegs.push_back(regGroup);
    }
    std::reverse(usedNVRegs.begin(), usedNVRegs.end());
    for (const auto& regGroup : usedNVRegs)
    {
        cmajor::masm::assembly::Instruction* popReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::POP);
        popReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(popReg);
    }

    std::vector<cmajor::masm::assembly::RegisterGroup*> usedLocalRegs;
    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedLocalRegs())
    {
        usedLocalRegs.push_back(regGroup);
    }
    std::reverse(usedLocalRegs.begin(), usedLocalRegs.end());
    for (const auto& regGroup : usedLocalRegs)
    {
        cmajor::masm::assembly::Instruction* popReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::POP);
        popReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(popReg);
    }

    cmajor::masm::assembly::Instruction* popRbp = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::POP);
    popRbp->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbp));
    codeGenerator.Emit(popRbp);

    cmajor::masm::assembly::Instruction* ret = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::RET);
    ret->AddOperand(assemblyContext->MakeIntegerLiteral(0, 1));
    codeGenerator.Emit(ret);
}

FrameLocation GetFrameLocation(Value* value, CodeGenerator& codeGenerator)
{
    Instruction* inst = nullptr;
    if (value->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        inst = regValue->Inst();
        if (!inst)
        {
            codeGenerator.Error("error getting frame location: instruction for reg value not set");
        }
    }
    else if (value->IsInstruction())
    {
        inst = static_cast<Instruction*>(value);
    }
    if (inst)
    {
        Locations locs = codeGenerator.RegAllocator()->GetLocations(inst);
        if ((locs & Locations::frame) != Locations::none)
        {
            FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(inst);
            if (frameLocation.Valid())
            {
                return frameLocation;
            }
            else
            {
                codeGenerator.Error("error getting frame location: frame location not valid");
            }
        }
        else
        {
            codeGenerator.Error("error getting frame location: value not in frame");
        }
    }
    else
    {
        codeGenerator.Error("error getting frame location: invalid value kind");
    }
    return FrameLocation();
}

void EmitFrameLocationOperand(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::Register* reg = assemblyContext->GetGlobalReg(8, frameLocation.reg);
    cmajor::masm::assembly::Value* frameLoc = assemblyContext->MakeIntegerLiteral(frameLocation.offset, 8);
    if (frameLocation.macro)
    {
        frameLoc = assemblyContext->MakeSymbol(frameLocation.macro->Name());
    }
    instruction->AddOperand(assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(
        assemblyContext->MakeBinaryExpr(reg, frameLoc, cmajor::masm::assembly::Operator::add))));
}

void EmitIntegerPtrOperand(int64_t size, Value* value, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    Instruction* inst = nullptr;
    if (value->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        inst = regValue->Inst();
        if (!inst)
        {
            codeGenerator.Error("error emitting ptr operand: instruction for reg value not set");
        }
    }
    else if (value->IsInstruction())
    {
        inst = static_cast<Instruction*>(value);
    }
    else if (value->IsAddressValue())
    {
        cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
        AddressValue* addressValue = static_cast<AddressValue*>(value);
        GlobalVariable* globalVar = addressValue->GetValue();
        cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
        leaInst->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rdx));
        leaInst->AddOperand(assemblyContext->MakeSymbol(globalVar->Name()));
        codeGenerator.Emit(leaInst);
        instruction->AddOperand(assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(
            assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rdx))));
        return;
    }
    if (inst)
    {
        if (inst->IsLocalInstruction())
        {
            FrameLocation frameLocation = GetFrameLocation(value, codeGenerator);
            EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
        }
        else
        {
            cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
            cmajor::masm::assembly::Value* operand = assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(
                MakeIntegerRegOperand(value, assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator)));
            instruction->AddOperand(operand);
        }
    }
}

void EmitFloatingPointPtrOperand(int64_t size, Value* value, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    Instruction* inst = nullptr;
    if (value->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        inst = regValue->Inst();
        if (!inst)
        {
            codeGenerator.Error("error emitting ptr operand: instruction for reg value not set");
        }
    }
    else if (value->IsInstruction())
    {
        inst = static_cast<Instruction*>(value);
    }
    else if (value->IsAddressValue())
    {
        cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
        AddressValue* addressValue = static_cast<AddressValue*>(value);
        GlobalVariable* globalVar = addressValue->GetValue();
        cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
        leaInst->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rdx));
        leaInst->AddOperand(assemblyContext->MakeSymbol(globalVar->Name()));
        codeGenerator.Emit(leaInst);
        instruction->AddOperand(assemblyContext->MakeSizePrefix(
            size, assemblyContext->MakeContent(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::xmm1))));
        return;
    }
    if (inst)
    {
        if (inst->IsLocalInstruction())
        {
            FrameLocation frameLocation = GetFrameLocation(value, codeGenerator);
            EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
        }
        else
        {
            cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
            cmajor::masm::assembly::Value* operand = assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(
                MakeFloatingPointRegOperand(value, assemblyContext->GetGlobalReg(16, cmajor::masm::assembly::RegisterGroupKind::xmm0), codeGenerator)));
            instruction->AddOperand(operand);
        }
    }
}

int64_t GetIndex(Value* index, CodeGenerator& codeGen)
{
    if (index->IsIntegerValue())
    {
        return index->GetIntegerValue();
    }
    else
    {
        codeGen.Error("error getting index: invalid index value kind");
    }
    return 0;
}

int64_t GetOffset(Type* type, int64_t index, CodeGenerator& codeGenerator)
{
    StructureType* structureType = type->GetStructurePointeeType(codeGenerator.Span(), codeGenerator.Ctx());
    return structureType->GetFieldOffset(index);
}

int64_t GetElementSize(Type* type, CodeGenerator& codeGenerator)
{
    ArrayType* arrayType = type->GetArrayPointeeType(codeGenerator.Span(), codeGenerator.Ctx());
    return arrayType->ElementType()->Size();
}

int64_t GetPointeeSize(Type* type, CodeGenerator& codeGenerator)
{
    if (type->IsPointerType())
    {
        PointerType* pointerType = static_cast<PointerType*>(type);
        Type* pointeeType = pointerType->RemovePointer(codeGenerator.Span(), codeGenerator.Ctx());
        return pointeeType->Size();
    }
    else
    {
        codeGenerator.Error("error getting pointee size: pointer type expected");
    }
    return -1;
}

cmajor::masm::assembly::Register* MakeIntegerRegOperand(Value* value, cmajor::masm::assembly::Register* reg, CodeGenerator& codeGenerator)
{
    int64_t size = std::min(value->GetType()->Size(), int64_t(reg->Size()));
    if (value->Kind() == ValueKind::regValue)
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        Instruction* inst = regValue->Inst();
        if (inst)
        {
            if (inst->IsLocalInstruction())
            {
                cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
                leaInst->AddOperand(reg);
                EmitIntegerPtrOperand(size, value, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                return reg;
            }
            Locations locs = codeGenerator.RegAllocator()->GetLocations(inst);
            if ((locs & Locations::reg) != Locations::none)
            {
                cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(inst);
                if (regGroup)
                {
                    return regGroup->GetReg(size);
                }
                else
                {
                    codeGenerator.Error("error making reg operand: reg group not valid");
                }
            }
            else if ((locs & Locations::frame) != Locations::none)
            {
                FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(inst);
                if (frameLocation.Valid())
                {
                    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
                    cmajor::masm::assembly::RegisterGroup* regGroup = assemblyContext->GetRegisterPool()->GetGlobalRegisterGroup(reg->Group(), true);
                    EmitIntegerLoad(size, frameLocation, regGroup, codeGenerator);
                }
                else
                {
                    codeGenerator.Error("error making reg operand: frame location not valid");
                }
            }
            else
            {
                codeGenerator.Error("error making reg operand: no locations for inst");
            }
        }
        else
        {
            codeGenerator.Error("error making reg operand: instruction for reg value not set");
        }
    }
    else if (value->IsAddressValue())
    {
        AddressValue* v = static_cast<AddressValue*>(value);
        cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
        leaInst->AddOperand(reg);
        cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
        leaInst->AddOperand(assemblyContext->MakeSymbol(v->GetValue()->Name()));
        codeGenerator.Emit(leaInst);
    }
    else
    {
        cmajor::masm::assembly::Instruction* inst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        inst->AddOperand(reg);
        codeGenerator.Emit(inst);
        cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
        switch (value->Kind())
        {
            case ValueKind::boolValue:
            {
                BoolValue* v = static_cast<BoolValue*>(value);
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 1));
                break;
            }
            case ValueKind::sbyteValue:
            {
                SByteValue* v = static_cast<SByteValue*>(value);
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 1));
                break;
            }
            case ValueKind::byteValue:
            {
                ByteValue* v = static_cast<ByteValue*>(value);
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 1));
                break;
            }
            case ValueKind::shortValue:
            {
                ShortValue* v = static_cast<ShortValue*>(value);
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 2));
                break;
            }
            case ValueKind::ushortValue:
            {
                UShortValue* v = static_cast<UShortValue*>(value);
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 2));
                break;
            }
            case ValueKind::intValue:
            {
                IntValue* v = static_cast<IntValue*>(value);
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 4));
                break;
            }
            case ValueKind::uintValue:
            {
                UIntValue* v = static_cast<UIntValue*>(value);
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 4));
                break;
            }
            case ValueKind::longValue:
            {
                LongValue* v = static_cast<LongValue*>(value);
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 8));
                break;
            }
            case ValueKind::ulongValue:
            {
                ULongValue* v = static_cast<ULongValue*>(value);
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(v->GetValue(), 8));
                break;
            }
            case ValueKind::nullValue:
            {
                inst->AddOperand(assemblyContext->MakeIntegerLiteral(0, 8));
                break;
            }
            case ValueKind::symbolValue:
            {
                SymbolValue* v = static_cast<SymbolValue*>(value);
                inst->AddOperand(assemblyContext->MakeSymbol(v->Symbol()));
                break;
            }
            default:
            {
                codeGenerator.Error("error making reg operand: not implemented for value kind " + value->KindStr());
                break;
            }
        }
    }
    return reg;
}

cmajor::masm::assembly::Register* MakeFloatingPointRegOperand(Value* value, cmajor::masm::assembly::Register* reg, CodeGenerator& codeGenerator)
{
    int64_t size = std::min(value->GetType()->Size(), int64_t(reg->Size()));
    if (value->Kind() == ValueKind::regValue)
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        Instruction* inst = regValue->Inst();
        if (inst)
        {
            if (inst->IsLocalInstruction())
            {
                cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
                leaInst->AddOperand(reg);
                EmitFloatingPointPtrOperand(size, value, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                return reg;
            }
            Locations locs = codeGenerator.RegAllocator()->GetLocations(inst);
            if ((locs & Locations::reg) != Locations::none)
            {
                cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(inst);
                if (regGroup)
                {
                    return regGroup->GetReg(size);
                }
                else
                {
                    codeGenerator.Error("error making reg operand: reg group not valid");
                }
            }
            else if ((locs & Locations::frame) != Locations::none)
            {
                FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(inst);
                if (frameLocation.Valid())
                {
                    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
                    cmajor::masm::assembly::RegisterGroup* regGroup = assemblyContext->GetRegisterPool()->GetGlobalRegisterGroup(reg->Group(), true);
                    EmitIntegerLoad(size, frameLocation, regGroup, codeGenerator);
                }
                else
                {
                    codeGenerator.Error("error making reg operand: frame location not valid");
                }
            }
            else
            {
                codeGenerator.Error("error making reg operand: no locations for inst");
            }
        }
        else
        {
            codeGenerator.Error("error making reg operand: instruction for reg value not set");
        }
    }
    else if (value->IsAddressValue())
    {
        AddressValue* v = static_cast<AddressValue*>(value);
        cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
        leaInst->AddOperand(reg);
        cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
        leaInst->AddOperand(assemblyContext->MakeSymbol(v->GetValue()->Name()));
        codeGenerator.Emit(leaInst);
    }
    else
    {
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error making reg operand: invalid value size");
        }
        cmajor::masm::assembly::Instruction* inst = new cmajor::masm::assembly::Instruction(opCode);
        inst->AddOperand(reg);
        codeGenerator.Emit(inst);
        cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
        switch (value->Kind())
        {
            case ValueKind::floatValue:
            {
                FloatValue* v = static_cast<FloatValue*>(value);
                inst->AddOperand(assemblyContext->MakeFloatLiteralSymbol(v->GetValue()));
                break;
            }
            case ValueKind::doubleValue:
            {
                DoubleValue* v = static_cast<DoubleValue*>(value);
                inst->AddOperand(assemblyContext->MakeDoubleLiteralSymbol(v->GetValue()));
                break;
            }
            default:
            {
                codeGenerator.Error("error making reg operand: invalid value kind " + value->KindStr());
                break;
            }
        }
    }
    return reg;
}

void EmitIntegerRet(RetInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    if (inst.ReturnValue())
    {
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        int64_t size = inst.ReturnValue()->GetType()->Size();
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax));
        movInst->AddOperand(MakeIntegerRegOperand(inst.ReturnValue(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator));
        codeGenerator.Emit(movInst);
    }
    int targetLabelId = codeGenerator.ExitLabelId();
    cmajor::masm::assembly::Instruction* jmpInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::JMP);
    jmpInst->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(targetLabelId)));
    codeGenerator.Emit(jmpInst);
}

void EmitFloatingPointRet(RetInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    if (inst.ReturnValue())
    {
        int64_t size = inst.ReturnValue()->GetType()->Size();
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSD;
        }
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(opCode);
        movInst->AddOperand(assemblyContext->GetGlobalReg(16, cmajor::masm::assembly::RegisterGroupKind::xmm0));
        movInst->AddOperand(MakeFloatingPointRegOperand(
            inst.ReturnValue(), assemblyContext->GetGlobalReg(16, cmajor::masm::assembly::RegisterGroupKind::xmm1), codeGenerator));
        codeGenerator.Emit(movInst);
    }
    int targetLabelId = codeGenerator.ExitLabelId();
    cmajor::masm::assembly::Instruction* jmpInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::JMP);
    jmpInst->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(targetLabelId)));
    codeGenerator.Emit(jmpInst);
}

void EmitSignExtend(SignExtendInstruction& inst, CodeGenerator& codeGenerator)
{
    int64_t operandSize = inst.Operand()->GetType()->Size();
    int64_t resultSize = inst.Result()->GetType()->Size();
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::MOVSX;
    if (operandSize == 4)
    {
        opCode = cmajor::masm::assembly::OpCode::MOVSXD;
    }
    cmajor::masm::assembly::Instruction* movsxInst = new cmajor::masm::assembly::Instruction(opCode);
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        cmajor::masm::assembly::Register* reg = regGroup->GetReg(resultSize);
        movsxInst->AddOperand(reg);
        cmajor::masm::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        movsxInst->AddOperand(sourceReg);
        codeGenerator.Emit(movsxInst);
    }
    else
    {
        codeGenerator.Error("error emitting sign extend instruction: reg group for inst not found");
    }
}

void EmitZeroExtend(ZeroExtendInstruction& inst, CodeGenerator& codeGenerator)
{
    int64_t operandSize = inst.Operand()->GetType()->Size();
    int64_t resultSize = inst.Result()->GetType()->Size();
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::MOV;
    if (operandSize <= 2 && operandSize != resultSize)
    {
        opCode = cmajor::masm::assembly::OpCode::MOVZX;
    }
    else if (resultSize == 8)
    {
        resultSize = 4;
    }
    cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(opCode);
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        cmajor::masm::assembly::Register* reg = regGroup->GetReg(resultSize);
        movInst->AddOperand(reg);
        cmajor::masm::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting zero extend instruction: reg group for inst not found");
    }
}

void EmitIntegerTruncate(TruncateInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        int64_t resultSize = inst.Result()->GetType()->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        cmajor::masm::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(resultSize, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting truncate instruction: reg group for inst not found");
    }
}

void EmitFloatingPointTruncate(TruncateInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        int64_t resultSize = inst.Result()->GetType()->Size();
        int64_t operandSize = inst.Operand()->GetType()->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        cmajor::masm::assembly::Register* sourceReg = MakeFloatingPointRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::xmm0), codeGenerator);
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (resultSize == 4 && operandSize == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::CVTSD2SS;
        }
        else 
        {
            codeGenerator.Error("error emitting truncate instruction: invalid floating-point truncation types");
        }
        cmajor::masm::assembly::Instruction* convertInst = new cmajor::masm::assembly::Instruction(opCode);
        convertInst->AddOperand(resultReg);
        convertInst->AddOperand(sourceReg);
        codeGenerator.Emit(convertInst);
    }
    else
    {
        codeGenerator.Error("error emitting truncate instruction: reg group for inst not found");
    }
}

void EmitIntegerBinOpInst(BinaryInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    int64_t size = inst.GetType()->Size();
    cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::ADD;
    bool smallMod = false;
    bool smallDiv = false;
    switch (inst.GetOpCode())
    {
    case OpCode::add:
    {
        opCode = cmajor::masm::assembly::OpCode::ADD;
        break;
    }
    case OpCode::sub:
    {
        opCode = cmajor::masm::assembly::OpCode::SUB;
        break;
    }
    case OpCode::mul:
    {
        if (inst.GetType()->IsUnsignedType())
        {
            opCode = cmajor::masm::assembly::OpCode::MUL;
        }
        else
        {
            opCode = cmajor::masm::assembly::OpCode::IMUL;
        }
        break;
    }
    case OpCode::div_:
    {
        if (inst.GetType()->IsUnsignedType())
        {
            opCode = cmajor::masm::assembly::OpCode::DIV;
        }
        else
        {
            opCode = cmajor::masm::assembly::OpCode::IDIV;
        }
        break;
    }
    case OpCode::mod:
    {
        if (inst.GetType()->IsUnsignedType())
        {
            opCode = cmajor::masm::assembly::OpCode::DIV;
        }
        else
        {
            opCode = cmajor::masm::assembly::OpCode::IDIV;
        }
        break;
    }
    case OpCode::and_:
    {
        opCode = cmajor::masm::assembly::OpCode::AND;
        break;
    }
    case OpCode::or_:
    {
        opCode = cmajor::masm::assembly::OpCode::OR;
        break;
    }
    case OpCode::xor_:
    {
        opCode = cmajor::masm::assembly::OpCode::XOR;
        break;
    }
    case OpCode::shl:
    {
        opCode = cmajor::masm::assembly::OpCode::SHL;
        break;
    }
    case OpCode::shr:
    {
        if (inst.GetType()->IsUnsignedType())
        {
            opCode = cmajor::masm::assembly::OpCode::SHR;
        }
        else
        {
            opCode = cmajor::masm::assembly::OpCode::SAR;
        }
        break;
    }
    }
    cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(opCode);
    cmajor::masm::assembly::Register* leftOperandReg = nullptr;
    cmajor::masm::assembly::Register* rightOperandReg = nullptr;
    cmajor::masm::assembly::Register* resultOperandReg = nullptr;
    switch (inst.GetOpCode())
    {
    case OpCode::add:
    case OpCode::sub:
    case OpCode::and_:
    case OpCode::or_:
    case OpCode::xor_:
    {
        leftOperandReg = MakeIntegerRegOperand(inst.Left(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        instruction->AddOperand(leftOperandReg);
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        resultOperandReg = leftOperandReg;
        break;
    }
    case OpCode::div_:
    {
        cmajor::masm::assembly::Register* rdx = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rdx);
        cmajor::masm::assembly::Instruction* xorInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::XOR);
        xorInst->AddOperand(rdx);
        xorInst->AddOperand(rdx);
        codeGenerator.Emit(xorInst);
        cmajor::masm::assembly::Register* rax = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax);
        leftOperandReg = MakeIntegerRegOperand(inst.Left(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        if (leftOperandReg->Group() != cmajor::masm::assembly::RegisterGroupKind::rax)
        {
            cmajor::masm::assembly::Instruction* movInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            movInstruction->AddOperand(rax);
            movInstruction->AddOperand(leftOperandReg);
            codeGenerator.Emit(movInstruction);
        }
        resultOperandReg = rax;
        if (size < 2)
        {
            smallDiv = true;
        }
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        break;
    }
    case OpCode::mul:
    {
        cmajor::masm::assembly::Register* rax = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax);
        leftOperandReg = MakeIntegerRegOperand(inst.Left(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        if (leftOperandReg->Group() != cmajor::masm::assembly::RegisterGroupKind::rax)
        {
            cmajor::masm::assembly::Instruction* movInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            movInstruction->AddOperand(rax);
            movInstruction->AddOperand(leftOperandReg);
            codeGenerator.Emit(movInstruction);
        }
        resultOperandReg = rax;
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        break;
    }
    case OpCode::mod:
    {
        cmajor::masm::assembly::Register* rdx = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rdx);
        cmajor::masm::assembly::Instruction* xorInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::XOR);
        xorInst->AddOperand(rdx);
        xorInst->AddOperand(rdx);
        codeGenerator.Emit(xorInst);
        cmajor::masm::assembly::Register* rax = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax);
        leftOperandReg = MakeIntegerRegOperand(inst.Left(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        if (leftOperandReg->Group() != cmajor::masm::assembly::RegisterGroupKind::rax)
        {
            cmajor::masm::assembly::Instruction* movInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            movInstruction->AddOperand(rax);
            movInstruction->AddOperand(leftOperandReg);
            codeGenerator.Emit(movInstruction);
        }
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        if (size > 1)
        {
            resultOperandReg = rdx;
        }
        else
        {
            resultOperandReg = rax;
            smallMod = true;
        }
        break;
    }
    case OpCode::shl:
    case OpCode::shr:
    {
        leftOperandReg = MakeIntegerRegOperand(inst.Left(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        cmajor::masm::assembly::Register* cl = assemblyContext->GetGlobalReg(1, cmajor::masm::assembly::RegisterGroupKind::rcx);
        rightOperandReg = MakeIntegerRegOperand(inst.Right(), assemblyContext->GetGlobalReg(1, cmajor::masm::assembly::RegisterGroupKind::rcx), codeGenerator);
        if (rightOperandReg->Group() != cmajor::masm::assembly::RegisterGroupKind::rcx)
        {
            cmajor::masm::assembly::Instruction* movInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            movInstruction->AddOperand(cl);
            movInstruction->AddOperand(rightOperandReg);
            codeGenerator.Emit(movInstruction);
        }
        instruction->AddOperand(leftOperandReg);
        instruction->AddOperand(cl);
        resultOperandReg = leftOperandReg;
        break;
    }
    }
    if (size == 1 && (smallMod || smallDiv) && inst.GetType()->IsSignedType())
    {
        cmajor::masm::assembly::Instruction* cbwInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CBW);
        codeGenerator.Emit(cbwInstruction);
    }
    else if (size == 2 && (inst.GetOpCode() == OpCode::div_ || inst.GetOpCode() == OpCode::mod) && inst.GetType()->IsSignedType())
    {
        cmajor::masm::assembly::Instruction* cwdInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CWD);
        codeGenerator.Emit(cwdInstruction);
    }
    else if (size == 4 && (inst.GetOpCode() == OpCode::div_ || inst.GetOpCode() == OpCode::mod) && inst.GetType()->IsSignedType())
    {
        cmajor::masm::assembly::Instruction* cdqInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CDQ);
        codeGenerator.Emit(cdqInstruction);
    }
    else if (size == 8 && (inst.GetOpCode() == OpCode::div_ || inst.GetOpCode() == OpCode::mod) && inst.GetType()->IsSignedType())
    {
        cmajor::masm::assembly::Instruction* cqoInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CQO);
        codeGenerator.Emit(cqoInstruction);
    }
    codeGenerator.Emit(instruction);
    if (smallMod)
    {
        cmajor::masm::assembly::Instruction* shrInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::SHR);
        shrInstruction->AddOperand(assemblyContext->GetGlobalReg(2, cmajor::masm::assembly::RegisterGroupKind::rax));
        shrInstruction->AddOperand(assemblyContext->MakeIntegerLiteral(8, 1));
        codeGenerator.Emit(shrInstruction);
    }
    cmajor::masm::assembly::Instruction* movInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        if (smallDiv)
        {
            cmajor::masm::assembly::Instruction* xorInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::XOR);
            cmajor::masm::assembly::Register* ah = assemblyContext->GetGlobalReg(cmajor::masm::assembly::highByteRegSize, cmajor::masm::assembly::RegisterGroupKind::rax);
            xorInstruction->AddOperand(ah);
            xorInstruction->AddOperand(ah);
            codeGenerator.Emit(xorInstruction);
        }
        movInstruction->AddOperand(regGroup->GetReg(size));
        movInstruction->AddOperand(resultOperandReg);
        codeGenerator.Emit(movInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting integer binary operation instruction: reg group for inst not found");
    }
}

void EmitFloatingPointBinOpInst(BinaryInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    int64_t size = inst.GetType()->Size();
    if (size != 4 && size != 8)
    {
        codeGenerator.Error("error emitting integer binary operation instruction: invalid size");
    }
    cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
    switch (inst.GetOpCode())
    {
        case OpCode::add:
        {
            if (size == 4)
            {
                opCode = cmajor::masm::assembly::OpCode::ADDSS;
            }
            else if (size == 8)
            {
                opCode = cmajor::masm::assembly::OpCode::ADDSD;
            }
            break;
        }
        case OpCode::sub:
        {
            if (size == 4)
            {
                opCode = cmajor::masm::assembly::OpCode::SUBSS;
            }
            else if (size == 8)
            {
                opCode = cmajor::masm::assembly::OpCode::SUBSD;
            }
            break;
        }
        case OpCode::mul:
        {
            if (size == 4)
            {
                opCode = cmajor::masm::assembly::OpCode::MULSS;
            }
            else if (size == 8)
            {
                opCode = cmajor::masm::assembly::OpCode::MULSD;
            }
            break;
        }
        case OpCode::div_:
        {
            if (size == 4)
            {
                opCode = cmajor::masm::assembly::OpCode::DIVSS;
            }
            else if (size == 8)
            {
                opCode = cmajor::masm::assembly::OpCode::DIVSD;
            }
            break;
        }
    }
    cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(opCode);
    cmajor::masm::assembly::Register* leftOperandReg = MakeFloatingPointRegOperand(
        inst.Left(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm0), codeGenerator);
    instruction->AddOperand(leftOperandReg);
    cmajor::masm::assembly::Register* rightOperandReg = MakeFloatingPointRegOperand(
        inst.Right(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm1), codeGenerator);
    instruction->AddOperand(rightOperandReg);
    cmajor::masm::assembly::Register* resultOperandReg = leftOperandReg;
    codeGenerator.Emit(instruction);
    cmajor::masm::assembly::OpCode movOpCode = cmajor::masm::assembly::OpCode::NOP;
    if (size == 4)
    {
        movOpCode = cmajor::masm::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        movOpCode = cmajor::masm::assembly::OpCode::MOVSD;
    }
    cmajor::masm::assembly::Instruction* movInstruction = new cmajor::masm::assembly::Instruction(movOpCode);
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        movInstruction->AddOperand(regGroup->GetReg(size));
        movInstruction->AddOperand(resultOperandReg);
        codeGenerator.Emit(movInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting floating-point binary operation instruction: reg group for inst not found");
    }
}

void EmitBitcast(BitcastInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        int64_t size = type->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(size);
        cmajor::masm::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting bitcast instruction: reg group for inst not found");
    }
}

void EmitIntToFloat(IntToFloatInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    Value* operand = inst.Operand();
    Type* operandType = operand->GetType();
    int64_t operandSize = operandType->Size();
    Type* resultType = inst.Result()->GetType();
    int64_t resultSize = resultType->Size();
    cmajor::masm::assembly::Register* sourceReg = MakeIntegerRegOperand(
        inst.Operand(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator);
    if (operandSize < 4)
    {
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::MOVSX;
        if (operandType->IsUnsignedType())
        {
            opCode = cmajor::masm::assembly::OpCode::MOVZX;
        }
        cmajor::masm::assembly::Instruction* extendInst = new cmajor::masm::assembly::Instruction(opCode);
        cmajor::masm::assembly::Register* rax = assemblyContext->GetGlobalReg(4, cmajor::masm::assembly::RegisterGroupKind::rax);
        extendInst->AddOperand(rax);
        extendInst->AddOperand(sourceReg);
        codeGenerator.Emit(extendInst);
        sourceReg = rax;
    }
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (resultSize == 4)
        {
            opCode = cmajor::masm::assembly::OpCode::CVTSI2SS;
        }
        else if (resultSize == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::CVTSI2SD;
        }
        else
        {
            codeGenerator.Error("error emitting inttofloat instruction: invalid result size");
        }
        cmajor::masm::assembly::Instruction* convertInstruction = new cmajor::masm::assembly::Instruction(opCode);
        convertInstruction->AddOperand(resultReg);
        convertInstruction->AddOperand(sourceReg);
        codeGenerator.Emit(convertInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting inttofloat instruction: reg group for inst not found");
    }
}

void EmitFloatToInt(FloatToIntInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    Value* operand = inst.Operand();
    Type* operandType = operand->GetType();
    int64_t operandSize = operandType->Size();
    Type* resultType = inst.Result()->GetType();
    int64_t resultSize = resultType->Size();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        cmajor::masm::assembly::Register* sourceReg = MakeFloatingPointRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::xmm0), codeGenerator);
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (operandSize == 4)
        {
            opCode = cmajor::masm::assembly::OpCode::CVTTSS2SI;
        }
        else if (operandSize == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::CVTTSD2SO;
        }
        else
        {
            codeGenerator.Error("error emitting floattoint instruction: invalid operand size");
        }
        cmajor::masm::assembly::Instruction* convertInstruction = new cmajor::masm::assembly::Instruction(opCode);
        convertInstruction->AddOperand(resultReg);
        convertInstruction->AddOperand(sourceReg);
        codeGenerator.Emit(convertInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting floattoint instruction: reg group for inst not found");
    }
}

void EmitIntToPtr(IntToPtrInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        int64_t size = type->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(size);
        cmajor::masm::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting inttoptr instruction: reg group for inst not found");
    }
}

void EmitPtrToInt(PtrToIntInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        int64_t size = type->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(size);
        cmajor::masm::assembly::Register* sourceReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(sourceReg);
        codeGenerator.Emit(movInst);
    }
    else
    {
        codeGenerator.Error("error emitting ptrtoint instruction: reg group for inst not found");
    }
}

void EmitIntegerEqual(EqualInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        int64_t resultSize = 1;
        int64_t size = inst.Left()->GetType()->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        cmajor::masm::assembly::Instruction* cmpInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CMP);
        cmajor::masm::assembly::Register* leftOperandReg = MakeIntegerRegOperand(
            inst.Left(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rcx), codeGenerator);
        cmpInstruction->AddOperand(leftOperandReg);
        cmajor::masm::assembly::Register* rightOperandReg = MakeIntegerRegOperand(
            inst.Right(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rdx), codeGenerator);
        cmpInstruction->AddOperand(rightOperandReg);
        codeGenerator.Emit(cmpInstruction);
        cmajor::masm::assembly::Instruction* setInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::SETE);
        setInst->AddOperand(resultReg);
        codeGenerator.Emit(setInst);
    }
    else
    {
        codeGenerator.Error("error emitting equal instruction: reg group for inst not found");
    }
}

void EmitFloatingPointEqual(EqualInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    Value* leftOperand = inst.Left();
    Type* leftOperandType = leftOperand->GetType();
    int64_t operandSize = leftOperandType->Size();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        int64_t resultSize = 1;
        int64_t size = inst.Left()->GetType()->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (operandSize == 4)
        {
            opCode = cmajor::masm::assembly::OpCode::UCOMISS;
        }
        else if (operandSize == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::UCOMISD;
        }
        else
        {
            codeGenerator.Error("error emitting equal instruction: invalid operand size");
        }
        cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(opCode);
        cmajor::masm::assembly::Register* leftOperandReg = MakeFloatingPointRegOperand(
            inst.Left(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::xmm1), codeGenerator);
        instruction->AddOperand(leftOperandReg);
        cmajor::masm::assembly::Register* rightOperandReg = MakeFloatingPointRegOperand(
            inst.Right(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::xmm2), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        codeGenerator.Emit(instruction);
        cmajor::masm::assembly::Instruction* setInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::SETE);
        setInst->AddOperand(resultReg);
        codeGenerator.Emit(setInst);
    }
    else
    {
        codeGenerator.Error("error emitting equal instruction: reg group for inst not found");
    }
}

void EmitIntegerLess(LessInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        int64_t resultSize = 1;
        int64_t size = inst.Left()->GetType()->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        cmajor::masm::assembly::Instruction* cmpInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CMP);
        cmajor::masm::assembly::Register* leftOperandReg = MakeIntegerRegOperand(
            inst.Left(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rcx), codeGenerator);
        cmpInstruction->AddOperand(leftOperandReg);
        cmajor::masm::assembly::Register* rightOperandReg = MakeIntegerRegOperand(
            inst.Right(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rdx), codeGenerator);
        cmpInstruction->AddOperand(rightOperandReg);
        codeGenerator.Emit(cmpInstruction);
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::SETL;
        if (inst.Left()->GetType()->IsUnsignedType())
        {
            opCode = cmajor::masm::assembly::OpCode::SETC;
        }
        cmajor::masm::assembly::Instruction* setInst = new cmajor::masm::assembly::Instruction(opCode);
        setInst->AddOperand(resultReg);
        codeGenerator.Emit(setInst);
    }
    else
    {
        codeGenerator.Error("error emitting less instruction: reg group for inst not found");
    }
}

void EmitFloatingPointLess(LessInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    Value* leftOperand = inst.Left();
    Type* leftOperandType = leftOperand->GetType();
    int64_t operandSize = leftOperandType->Size();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        int64_t resultSize = 1;
        int64_t size = inst.Left()->GetType()->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(resultSize);
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (operandSize == 4)
        {
            opCode = cmajor::masm::assembly::OpCode::COMISS;
        }
        else if (operandSize == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::COMISD;
        }
        else
        {
            codeGenerator.Error("error emitting less instruction: invalid operand size");
        }
        cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(opCode);
        cmajor::masm::assembly::Register* leftOperandReg = MakeFloatingPointRegOperand(
            inst.Left(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::xmm1), codeGenerator);
        instruction->AddOperand(leftOperandReg);
        cmajor::masm::assembly::Register* rightOperandReg = MakeFloatingPointRegOperand(
            inst.Right(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::xmm2), codeGenerator);
        instruction->AddOperand(rightOperandReg);
        codeGenerator.Emit(instruction);
        cmajor::masm::assembly::Instruction* setInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::SETL);
        setInst->AddOperand(resultReg);
        codeGenerator.Emit(setInst);
    }
    else
    {
        codeGenerator.Error("error emitting less instruction: reg group for inst not found");
    }
}

void EmitJmp(JmpInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::Instruction* jmpInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::JMP);
    jmpInstruction->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(inst.TargetBasicBlock()->Id())));
    codeGenerator.Emit(jmpInstruction);
}

void EmitBranch(BranchInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    int size = 4;
    cmajor::masm::assembly::Register* operandReg = MakeIntegerRegOperand(
        inst.Cond(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
    cmajor::masm::assembly::Instruction* orInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::OR);
    orInstruction->AddOperand(operandReg);
    orInstruction->AddOperand(operandReg);
    codeGenerator.Emit(orInstruction);
    cmajor::masm::assembly::Instruction* jnzInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::JNZ);
    jnzInstruction->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(inst.TrueTargetBasicBlock()->Id())));
    codeGenerator.Emit(jnzInstruction);
    cmajor::masm::assembly::Instruction* jmpInstruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::JMP);
    jmpInstruction->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(inst.FalseTargetBasicBlock()->Id())));
    codeGenerator.Emit(jmpInstruction);
}

void EmitNot(NotInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        int64_t size = type->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(size);
        cmajor::masm::assembly::Register* operandReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(operandReg);
        codeGenerator.Emit(movInst);
        if (type->IsBooleanType())
        {
            cmajor::masm::assembly::Instruction* xorInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::XOR);
            xorInst->AddOperand(resultReg);
            xorInst->AddOperand(assemblyContext->MakeIntegerLiteral(1, int(size)));
            codeGenerator.Emit(xorInst);
        }
        else if (type->IsIntegerType())
        {
            cmajor::masm::assembly::Instruction* notInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::NOT);
            notInst->AddOperand(resultReg);
            codeGenerator.Emit(notInst);
        }
        else
        {
            codeGenerator.Error("error emitting not instruction: integer or boolean type expected");
        }
    }
    else
    {
        codeGenerator.Error("error emitting not instruction: reg group for inst not found");
    }
}

void EmitIntegerNeg(NegInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        int64_t size = type->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(size);
        cmajor::masm::assembly::Register* operandReg = MakeIntegerRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(resultReg);
        movInst->AddOperand(operandReg);
        codeGenerator.Emit(movInst);
        cmajor::masm::assembly::Instruction* negInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::NEG);
        negInst->AddOperand(resultReg);
        codeGenerator.Emit(negInst);
    }
    else
    {
        codeGenerator.Error("error emitting neg instruction: reg group for inst not found");
    }
}

void EmitFloatingPointNeg(NegInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        Type* type = inst.Operand()->GetType();
        int64_t size = type->Size();
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(size);
        cmajor::masm::assembly::Register* operandReg = MakeFloatingPointRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm0), codeGenerator);
        cmajor::masm::assembly::OpCode mulOpCode = cmajor::masm::assembly::OpCode::NOP;
        std::unique_ptr<Value> value(nullptr);
        if (size == 4)
        {
            value.reset(new FloatValue(-1.0f, codeGenerator.Ctx()->GetTypes().GetFloatType()));
            mulOpCode = cmajor::masm::assembly::OpCode::MULSS;
        }
        else if (size == 8)
        {
            value.reset(new DoubleValue(-1.0, codeGenerator.Ctx()->GetTypes().GetDoubleType()));
            mulOpCode = cmajor::masm::assembly::OpCode::MULSD;
        }
        else
        {
            codeGenerator.Error("error emitting neg instruction: invalid operand size");
        }
        cmajor::masm::assembly::Register* minusOneReg = MakeFloatingPointRegOperand(
            value.get(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm1), codeGenerator);
        cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(mulOpCode);
        instruction->AddOperand(operandReg);
        instruction->AddOperand(minusOneReg);
        codeGenerator.Emit(instruction);
        cmajor::masm::assembly::OpCode movOpCode = cmajor::masm::assembly::OpCode::NOP;
        if (size == 4)
        {
            movOpCode = cmajor::masm::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            movOpCode = cmajor::masm::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error emitting neg instruction: invalid operand size");
        }
        cmajor::masm::assembly::Instruction* movInstruction = new cmajor::masm::assembly::Instruction(movOpCode);
        movInstruction->AddOperand(resultReg);
        movInstruction->AddOperand(operandReg);
        codeGenerator.Emit(movInstruction);
    }
    else
    {
        codeGenerator.Error("error emitting neg instruction: reg group for inst not found");
    }
}

void EmitNop(NoOperationInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Instruction* nopInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::NOP);
    codeGenerator.Emit(nopInst);
}

void EmitSwitch(SwitchInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    int64_t size = inst.Cond()->GetType()->Size();
    cmajor::masm::assembly::Register* condReg = MakeIntegerRegOperand(
        inst.Cond(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator);
    for (const auto& caseTarget : inst.CaseTargets())
    {
        int64_t size = caseTarget.caseValue->GetType()->Size();
        cmajor::masm::assembly::Register* caseReg = MakeIntegerRegOperand(
            caseTarget.caseValue, assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        cmajor::masm::assembly::Instruction* cmpInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CMP);
        cmpInst->AddOperand(caseReg);
        cmpInst->AddOperand(condReg);
        codeGenerator.Emit(cmpInst);
        cmajor::masm::assembly::Instruction* jeInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::JE);
        jeInst->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(caseTarget.targetBlock->Id())));
        codeGenerator.Emit(jeInst);
    }
    cmajor::masm::assembly::Instruction* jmpInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::JMP);
    jmpInst->AddOperand(assemblyContext->MakeSymbol("@" + std::to_string(inst.DefaultTargetBlock()->Id())));
    codeGenerator.Emit(jmpInst);
}

void EmitIntegerLoad(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator)
{
    if (!frameLocation.Valid())
    {
        codeGenerator.Error("error emitting load: frame location not valid");
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
    instruction->AddOperand(regGroup->GetReg(size));
    EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
    codeGenerator.Emit(instruction);
}

void EmitIntegerStore(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator)
{
    if (!frameLocation.Valid())
    {
        codeGenerator.Error("error emitting store: frame location not valid");
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
    EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
    instruction->AddOperand(regGroup->GetReg(size));
    codeGenerator.Emit(instruction);
}

void EmitFloatingPointLoad(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator)
{
    if (!frameLocation.Valid())
    {
        codeGenerator.Error("error emitting load: frame location not valid");
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
    if (size == 4)
    {
        opCode = cmajor::masm::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        opCode = cmajor::masm::assembly::OpCode::MOVSD;
    }
    else
    {
        codeGenerator.Error("error emitting load: invalid size");
    }
    cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(opCode);
    instruction->AddOperand(regGroup->GetReg(size));
    EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
    codeGenerator.Emit(instruction);
}

void EmitFloatingPointStore(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator)
{
    if (!frameLocation.Valid())
    {
        codeGenerator.Error("error emitting store: frame location not valid");
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
    if (size == 4)
    {
        opCode = cmajor::masm::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        opCode = cmajor::masm::assembly::OpCode::MOVSD;
    }
    else
    {
        codeGenerator.Error("error emitting load: invalid size");
    }
    cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(opCode);
    EmitFrameLocationOperand(size, frameLocation, instruction, codeGenerator);
    instruction->AddOperand(regGroup->GetReg(size));
    codeGenerator.Emit(instruction);
}

void EmitIntegerLoad(LoadInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (!regGroup)
    {
        codeGenerator.Error("error emitting load: reg group not valid");
    }
    Type* type = inst.Result()->GetType();
    if (type->IsFundamentalType() || type->IsPointerType())
    {
        int64_t size = type->Size();
        cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        instruction->AddOperand(regGroup->GetReg(size));
        EmitIntegerPtrOperand(size, inst.Ptr(), instruction, codeGenerator);
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting load: invalid type kind");
    }
}

void EmitIntegerStore(StoreInstruction& inst, CodeGenerator& codeGenerator)
{
    Type* type = inst.GetValue()->GetType();
    if (type->IsFundamentalType() || type->IsPointerType())
    {
        int64_t size = type->Size();
        cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
        cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        EmitIntegerPtrOperand(size, inst.GetPtr(), instruction, codeGenerator);
        instruction->AddOperand(MakeIntegerRegOperand(inst.GetValue(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator));
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting store: invalid type kind");
    }
}

void EmitFloatingPointLoad(LoadInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (!regGroup)
    {
        codeGenerator.Error("error emitting load: reg group not valid");
    }
    Type* type = inst.Result()->GetType();
    if (type->IsFloatingPointType())
    {
        int64_t size = type->Size();
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error emitting load: invalid size");
        }
        cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(opCode);
        instruction->AddOperand(regGroup->GetReg(size));
        EmitFloatingPointPtrOperand(size, inst.Ptr(), instruction, codeGenerator);
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting load: invalid type kind");
    }
}

void EmitFloatingPointStore(StoreInstruction& inst, CodeGenerator& codeGenerator)
{
    Type* type = inst.GetValue()->GetType();
    if (type->IsFloatingPointType())
    {
        int64_t size = type->Size();
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSD;
        }
        cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
        cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(opCode);
        EmitFloatingPointPtrOperand(size, inst.GetPtr(), instruction, codeGenerator);
        instruction->AddOperand(MakeFloatingPointRegOperand(
            inst.GetValue(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm0), codeGenerator));
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting store: invalid type kind");
    }
}

void EmitIntegerParam(ParamInstruction& inst, CodeGenerator& codeGenerator)
{
    Type* type = inst.GetType();
    int64_t size = type->Size();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    if (!regGroup)
    {
        codeGenerator.Error("error emitting param: reg group not valid");
    }
    switch (inst.Index())
    {
        case 0:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            cmajor::masm::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rcx);
            movInst->AddOperand(regGroup->GetReg(size));
            movInst->AddOperand(paramReg);
            codeGenerator.Emit(movInst);
            break;
        }
        case 1:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            cmajor::masm::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rdx);
            movInst->AddOperand(regGroup->GetReg(size));
            movInst->AddOperand(paramReg);
            codeGenerator.Emit(movInst);
            break;
        }
        case 2:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            cmajor::masm::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r8);
            movInst->AddOperand(regGroup->GetReg(size));
            movInst->AddOperand(paramReg);
            codeGenerator.Emit(movInst);
            break;
        }
        case 3:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            cmajor::masm::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r9);
            movInst->AddOperand(regGroup->GetReg(size));
            movInst->AddOperand(paramReg);
            codeGenerator.Emit(movInst);
            break;
        }
        default:
        {
            Locations locs = codeGenerator.RegAllocator()->GetLocations(&inst);
            if ((locs & Locations::frame) != Locations::none)
            {
                FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(&inst);
                EmitIntegerLoad(size, frameLocation, regGroup, codeGenerator);
            }
            else
            {
                codeGenerator.Error("error emitting param: no frame location");
            }
        }
    }
}

void EmitFloatingPointParam(ParamInstruction& inst, CodeGenerator& codeGenerator)
{
    Type* type = inst.GetType();
    int64_t size = type->Size();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    if (!regGroup)
    {
        codeGenerator.Error("error emitting param: reg group not valid");
    }
    cmajor::masm::assembly::OpCode movOpCode = cmajor::masm::assembly::OpCode::NOP;
    if (size == 4)
    {
        movOpCode = cmajor::masm::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        movOpCode = cmajor::masm::assembly::OpCode::MOVSD;
    }
    else
    {
        codeGenerator.Error("error emitting param: invalid operand size");
    }
    switch (inst.Index())
    {
        case 0:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(movOpCode);
            cmajor::masm::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm0);
            movInst->AddOperand(regGroup->GetReg(size));
            movInst->AddOperand(paramReg);
            codeGenerator.Emit(movInst);
            break;
        }
        case 1:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(movOpCode);
            cmajor::masm::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm1);
            movInst->AddOperand(regGroup->GetReg(size));
            movInst->AddOperand(paramReg);
            codeGenerator.Emit(movInst);
            break;
        }
        case 2:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(movOpCode);
            cmajor::masm::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm2);
            movInst->AddOperand(regGroup->GetReg(size));
            movInst->AddOperand(paramReg);
            codeGenerator.Emit(movInst);
            break;
        }
        case 3:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(movOpCode);
            cmajor::masm::assembly::Register* paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm3);
            movInst->AddOperand(regGroup->GetReg(size));
            movInst->AddOperand(paramReg);
            codeGenerator.Emit(movInst);
            break;
        }
        default:
        {
            Locations locs = codeGenerator.RegAllocator()->GetLocations(&inst);
            if ((locs & Locations::frame) != Locations::none)
            {
                FrameLocation frameLocation = codeGenerator.RegAllocator()->GetFrameLocation(&inst);
                EmitFloatingPointLoad(size, frameLocation, regGroup, codeGenerator);
            }
            else
            {
                codeGenerator.Error("error emitting param: no frame location");
            }
        }
    }
}

void EmitIntegerArgOperand(Instruction* argInst, int64_t size, Value* arg, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    if (arg->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(arg);
        Instruction* inst = regValue->Inst();
        if (inst->IsLocalInstruction())
        {
            cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(argInst);
            if (regGroup)
            {
                cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
                leaInst->AddOperand(regGroup->GetReg(8));
                FrameLocation frameLocation = GetFrameLocation(arg, codeGenerator);
                EmitFrameLocationOperand(1, frameLocation, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                instruction->AddOperand(regGroup->GetReg(8));
                return;
            }
            else
            {
                codeGenerator.Error("error emitting arg: reg group not valid");
            }
        }
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    instruction->AddOperand(MakeIntegerRegOperand(arg, assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator));
}

void EmitFloatingPointArgOperand(Instruction* argInst, int64_t size, Value* arg, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    if (arg->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(arg);
        Instruction* inst = regValue->Inst();
        if (inst->IsLocalInstruction())
        {
            cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(argInst);
            if (regGroup)
            {
                cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
                leaInst->AddOperand(regGroup->GetReg(8));
                FrameLocation frameLocation = GetFrameLocation(arg, codeGenerator);
                EmitFrameLocationOperand(1, frameLocation, leaInst, codeGenerator);
                codeGenerator.Emit(leaInst);
                instruction->AddOperand(regGroup->GetReg(8));
                return;
            }
            else
            {
                codeGenerator.Error("error emitting arg: reg group not valid");
            }
        }
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    instruction->AddOperand(MakeFloatingPointRegOperand(arg, assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm0), codeGenerator));
}

void EmitArgLocationOperand(int64_t size, const ArgLocation& argLocation, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::Register* reg = assemblyContext->GetGlobalReg(8, argLocation.reg);
    instruction->AddOperand(assemblyContext->MakeSizePrefix(size,
        assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(reg, assemblyContext->MakeIntegerLiteral(argLocation.offset, 8), 
            cmajor::masm::assembly::Operator::add))));
}

void EmitIntegerArg(ArgInstruction& inst, CallFrame* callFrame, int32_t index, CodeGenerator& codeGenerator)
{
    RegisterAllocationAction action = codeGenerator.RegAllocator()->Run(&inst);
    if (action == RegisterAllocationAction::spill)
    {
        for (const SpillData& spillData : codeGenerator.RegAllocator()->GetSpillData())
        {
            EmitIntegerStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
        }
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    int64_t size = inst.Arg()->GetType()->Size();
    switch (index)
    {
        case 0:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rcx));
            EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            break;
        }
        case 1:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rdx));
            EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            break;
        }
        case 2:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r8));
            EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            break;
        }
        case 3:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r9));
            EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            break;
        }
        default:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
            if (regGroup)
            {
                cmajor::masm::assembly::Register* reg = regGroup->GetReg(size);
                movInst->AddOperand(reg);
                EmitIntegerArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
                codeGenerator.Emit(movInst);
                cmajor::masm::assembly::Instruction* pushToFrameInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
                ArgLocation argLocation = callFrame->GetArgLocation(index);
                EmitArgLocationOperand(size, argLocation, pushToFrameInst, codeGenerator);
                pushToFrameInst->AddOperand(reg);
                codeGenerator.Emit(pushToFrameInst);
            }
            else
            {
                codeGenerator.Error("error emitting arg instruction: reg group for inst not found");
            }
            break;
        }
    }
}

void EmitFloatingPointArg(ArgInstruction& inst, CallFrame* callFrame, int32_t index, CodeGenerator& codeGenerator)
{
    RegisterAllocationAction action = codeGenerator.RegAllocator()->Run(&inst);
    if (action == RegisterAllocationAction::spill)
    {
        for (const SpillData& spillData : codeGenerator.RegAllocator()->GetSpillData())
        {
            EmitFloatingPointStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
        }
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    int64_t size = inst.Arg()->GetType()->Size();
    cmajor::masm::assembly::OpCode movOpCode = cmajor::masm::assembly::OpCode::NOP;
    if (size == 4)
    {
        movOpCode = cmajor::masm::assembly::OpCode::MOVSS;
    }
    else if (size == 8)
    {
        movOpCode = cmajor::masm::assembly::OpCode::MOVSD;
    }
    else
    {
        codeGenerator.Error("error emitting arg instruction: invalid operand size");
    }
    switch (index)
    {
        case 0:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(movOpCode);
            movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm0));
            EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            break;
        }
        case 1:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(movOpCode);
            movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm1));
            EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            break;
        }
        case 2:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(movOpCode);
            movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm2));
            EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            break;
        }
        case 3:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(movOpCode);
            movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm3));
            EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            break;
        }
        default:
        {
            cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(movOpCode);
            cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
            if (regGroup)
            {
                cmajor::masm::assembly::Register* reg = regGroup->GetReg(size);
                movInst->AddOperand(reg);
                EmitFloatingPointArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
                codeGenerator.Emit(movInst);
                cmajor::masm::assembly::Instruction* pushToFrameInst = new cmajor::masm::assembly::Instruction(movOpCode);
                ArgLocation argLocation = callFrame->GetArgLocation(index);
                EmitArgLocationOperand(size, argLocation, pushToFrameInst, codeGenerator);
                pushToFrameInst->AddOperand(reg);
                codeGenerator.Emit(pushToFrameInst);
            }
            else
            {
                codeGenerator.Error("error emitting arg instruction: reg group for inst not found");
            }
            break;
        }
    }
}

void EmitArgs(const std::vector<ArgInstruction*>& args, CodeGenerator& codeGenerator)
{
    CallFrame callFrame;
    int n = args.size();
    int64_t calleeParamAreaSize = n * 8;
    Frame& frame = codeGenerator.RegAllocator()->GetFrame();
    frame.SetCalleeParamAreaSize(std::max(frame.CalleeParamAreaSize(), calleeParamAreaSize));
    for (int i = 0; i < n; ++i)
    {
        int64_t size = args[i]->Arg()->GetType()->Size();
        callFrame.AllocateArgLocation(size);
    }
    for (int i = n - 1; i >= 0; --i)
    {
        ArgInstruction* arg = args[i];
        if (arg->IsFloatingPointInstruction())
        {
            EmitFloatingPointArg(*arg, &callFrame, i, codeGenerator); 
        }
        else
        {
            EmitIntegerArg(*arg, &callFrame, i, codeGenerator); 
        }
    }
}

cmajor::masm::assembly::Value* MakeCalleeOperand(Value* value, cmajor::masm::assembly::Register* reg, CodeGenerator& codeGenerator)
{
    if (value->Kind() == ValueKind::regValue)
    {
        return MakeIntegerRegOperand(value, reg, codeGenerator);
    }
    else
    {
        if (value->IsSymbolValue())
        {
            SymbolValue* symbolValue = static_cast<SymbolValue*>(value);
            cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
            return assemblyContext->MakeSymbol(symbolValue->Symbol());
        }
        else
        {
            codeGenerator.Error("error emitting callee operand: invalid value kind");
        }
    }
    return nullptr;
}

void EmitProcedureCall(ProcedureCallInstruction& inst, const std::vector<ArgInstruction*>& args, CodeGenerator& codeGenerator)
{
    EmitArgs(args, codeGenerator);
    RegisterAllocationAction action = codeGenerator.RegAllocator()->Run(&inst);
    if (action == RegisterAllocationAction::spill)
    {
        for (const SpillData& spillData : codeGenerator.RegAllocator()->GetSpillData())
        {
            if (spillData.registerGroupToSpill->IsFloatingPointReg())
            {
                EmitFloatingPointStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator); 
            }
            else
            {
                EmitIntegerStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator); 
            }
        }
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    cmajor::masm::assembly::Register* reg = regGroup->GetReg(8);
    cmajor::masm::assembly::Instruction* callInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CALL);
    callInst->AddOperand(MakeCalleeOperand(inst.Callee(), reg, codeGenerator));
    codeGenerator.Emit(callInst);
}

void EmitFunctionCall(FunctionCallInstruction& inst, const std::vector<ArgInstruction*>& args, CodeGenerator& codeGenerator)
{
    EmitArgs(args, codeGenerator);
    RegisterAllocationAction action = codeGenerator.RegAllocator()->Run(&inst);
    if (action == RegisterAllocationAction::spill)
    {
        for (const SpillData& spillData : codeGenerator.RegAllocator()->GetSpillData())
        {
            if (spillData.registerGroupToSpill->IsFloatingPointReg())
            {
                EmitFloatingPointStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
            }
            else
            {
                EmitIntegerStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
            }
        }
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    cmajor::masm::assembly::Instruction* callInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CALL);
    Type* type = inst.Result()->GetType();
    int size = static_cast<int>(type->Size());
    if (!type->IsFloatingPointType())
    {
        size = 8;
    }
    cmajor::masm::assembly::Register* reg = regGroup->GetReg(size);
    callInst->AddOperand(MakeCalleeOperand(inst.Callee(), reg, codeGenerator));
    codeGenerator.Emit(callInst);
    if (type->IsFloatingPointType())
    {
        cmajor::masm::assembly::OpCode opCode = cmajor::masm::assembly::OpCode::NOP;
        if (size == 4)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSS;
        }
        else if (size == 8)
        {
            opCode = cmajor::masm::assembly::OpCode::MOVSD;
        }
        else
        {
            codeGenerator.Error("error emitting function call: invalid result size");
        }
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(opCode);
        movInst->AddOperand(reg);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::xmm0));
        codeGenerator.Emit(movInst);
    }
    else
    {
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(reg);
        movInst->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax));
        codeGenerator.Emit(movInst);
    }
}

void EmitElemAddr(ElemAddrInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    ElemAddrKind elemAddrKind = inst.GetElemAddrKind(codeGenerator.Ctx());
    if (elemAddrKind == ElemAddrKind::array)
    {
        cmajor::masm::assembly::Register* indexReg = MakeIntegerRegOperand(
            inst.Index(), assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator);
        int64_t indexFactor = GetElementSize(inst.Ptr()->GetType(), codeGenerator);
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        cmajor::masm::assembly::Register* rax = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax);
        movInst->AddOperand(rax);
        movInst->AddOperand(assemblyContext->MakeIntegerLiteral(indexFactor, 8));
        codeGenerator.Emit(movInst);
        cmajor::masm::assembly::Instruction* mulInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MUL);
        mulInst->AddOperand(indexReg);
        codeGenerator.Emit(mulInst);
        cmajor::masm::assembly::Instruction* leaPtrInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
        cmajor::masm::assembly::Register* rcx = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rcx);
        leaPtrInst->AddOperand(rcx);
        EmitIntegerPtrOperand(8, inst.Ptr(), leaPtrInst, codeGenerator);
        codeGenerator.Emit(leaPtrInst);
        cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
        if (regGroup)
        {
            cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(8);
            cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
            leaInst->AddOperand(resultReg);
            leaInst->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(rax, rcx, cmajor::masm::assembly::Operator::add))));
            codeGenerator.Emit(leaInst);
        }
        else
        {
            codeGenerator.Error("error emitting elemaddr instruction: reg group for inst not found");
        }
    }
    else if (elemAddrKind == ElemAddrKind::structure)
    {
        cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
        if (regGroup)
        {
            cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(8);
            int64_t index = GetIndex(inst.Index(), codeGenerator);
            int64_t offset = GetOffset(inst.Ptr()->GetType(), index, codeGenerator);
            cmajor::masm::assembly::Instruction* movOffsetInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            cmajor::masm::assembly::Register* rbx = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbx);
            movOffsetInst->AddOperand(rbx);
            movOffsetInst->AddOperand(assemblyContext->MakeIntegerLiteral(offset, 8));
            codeGenerator.Emit(movOffsetInst);
            cmajor::masm::assembly::Instruction* leaPtrInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
            cmajor::masm::assembly::Register* rcx = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rcx);
            leaPtrInst->AddOperand(rcx);
            EmitIntegerPtrOperand(8, inst.Ptr(), leaPtrInst, codeGenerator);
            codeGenerator.Emit(leaPtrInst);
            cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
            leaInst->AddOperand(resultReg);
            leaInst->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(rbx, rcx, cmajor::masm::assembly::Operator::add))));
            codeGenerator.Emit(leaInst);
        }
        else
        {
            codeGenerator.Error("error emitting elemaddr instruction: reg group for inst not found");
        }
    }
    else
    {
        codeGenerator.Error("error emitting elemaddr: invalid elemaddr kind");
    }
}

void EmitPtrOffset(PtrOffsetInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::Register* offsetReg = MakeIntegerRegOperand(
        inst.Offset(), assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator);
    int64_t offsetFactor = GetPointeeSize(inst.Ptr()->GetType(), codeGenerator);
    cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
    cmajor::masm::assembly::Register* rax = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax);
    movInst->AddOperand(rax);
    movInst->AddOperand(assemblyContext->MakeIntegerLiteral(offsetFactor, 8));
    codeGenerator.Emit(movInst);
    cmajor::masm::assembly::Instruction* mulInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MUL);
    mulInst->AddOperand(offsetReg);
    codeGenerator.Emit(mulInst);
    cmajor::masm::assembly::Register* ptrReg = MakeIntegerRegOperand(
        inst.Ptr(), assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rcx), codeGenerator);
    cmajor::masm::assembly::Instruction* leaInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(8);
        leaInst->AddOperand(resultReg);
        cmajor::masm::assembly::Value* operand = assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
            assemblyContext->MakeBinaryExpr(rax, ptrReg, cmajor::masm::assembly::Operator::add)));
        leaInst->AddOperand(operand);
        codeGenerator.Emit(leaInst);
    }
    else
    {
        codeGenerator.Error("error emitting ptroffset instruction: reg group for inst not found");
    }
}

void EmitPtrDiff(PtrDiffInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    int64_t scaleFactor = GetPointeeSize(inst.LeftPtr()->GetType(), codeGenerator);
    cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
    cmajor::masm::assembly::Register* rcx = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rcx);
    movInst->AddOperand(rcx);
    movInst->AddOperand(assemblyContext->MakeIntegerLiteral(scaleFactor, 8));
    codeGenerator.Emit(movInst);
    cmajor::masm::assembly::Instruction* subInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::SUB);
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    if (regGroup)
    {
        cmajor::masm::assembly::Register* resultReg = regGroup->GetReg(8);
        cmajor::masm::assembly::Register* leftReg = MakeIntegerRegOperand(
            inst.LeftPtr(), assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        subInst->AddOperand(leftReg);
        cmajor::masm::assembly::Register* rightReg = MakeIntegerRegOperand(
            inst.RightPtr(), assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator);
        subInst->AddOperand(rightReg);
        codeGenerator.Emit(subInst);
        cmajor::masm::assembly::Register* rdx = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rdx);
        cmajor::masm::assembly::Instruction* xorInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::XOR);
        xorInst->AddOperand(rdx);
        xorInst->AddOperand(rdx);
        codeGenerator.Emit(xorInst);
        cmajor::masm::assembly::Register* rax = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax);
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(rax);
        movInst->AddOperand(leftReg);
        codeGenerator.Emit(movInst);
        cmajor::masm::assembly::Instruction* divInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::DIV);
        divInst->AddOperand(rcx);
        codeGenerator.Emit(divInst);
        cmajor::masm::assembly::Instruction* movResultInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movResultInst->AddOperand(resultReg);
        movResultInst->AddOperand(rax);
        codeGenerator.Emit(movResultInst);
    }
    else
    {
        codeGenerator.Error("error emitting ptrdiff instruction: reg group for inst not found");
    }
}

CodeGenerator::CodeGenerator(Context* context_, const std::string& assemblyFilePath_) :
    Visitor(context_), context(context_), file(assemblyFilePath_), currentFunction(nullptr), currentInst(nullptr), assemblyFunction(nullptr), leader(false),
    registerAllocator(nullptr), data(nullptr), label(), dataInstruction(nullptr), prevDataOpCode(cmajor::masm::assembly::OpCode::DB), currentOffset(0)
{
    context->AssemblyContext()->SetFile(&file);
}

const soul::ast::Span& CodeGenerator::Span() const
{
    return currentInst->Span();
}

int CodeGenerator::ExitLabelId() const
{
    return currentInst->Parent()->Parent()->LastBasicBlock()->Id() + 1;
}

void CodeGenerator::Emit(cmajor::masm::assembly::Instruction* assemblyInstruction)
{
    if (leader)
    {
        leader = false;
        assemblyInstruction->SetLabel("@" + std::to_string(currentInst->Parent()->Id()));
    }
    assemblyFunction->AddInstruction(assemblyInstruction);
}

void CodeGenerator::EmitDataValue(cmajor::masm::assembly::Value* dataValue, cmajor::masm::assembly::OpCode dataOpCode)
{
    if (prevDataOpCode != dataOpCode)
    {
        if (dataInstruction)
        {
            data->AddInstruction(dataInstruction);
            dataInstruction = nullptr;
        }
    }
    if (!dataInstruction)
    {
        dataInstruction = new cmajor::masm::assembly::Instruction(dataOpCode);
        dataInstruction->SetNoColon();
    }
    if (leader)
    {
        leader = false;
        if (!dataInstruction->Label().empty())
        {
            data->AddInstruction(dataInstruction);
            dataInstruction = new cmajor::masm::assembly::Instruction(dataOpCode);
            dataInstruction->SetNoColon();
        }
        dataInstruction->SetLabel(label);
        label.clear();
    }
    while (dataInstruction->Length() + dataValue->Length() > cmajor::masm::assembly::maxAssemblyLineLength)
    {
        if (dataValue->CanSplit())
        {
            cmajor::masm::assembly::Value* next = dataValue->Split(cmajor::masm::assembly::maxAssemblyLineLength - dataInstruction->Length());
            dataInstruction->AddOperand(dataValue);
            dataValue = next;
            if (dataValue->IsEmpty())
            {
                delete dataValue;
                return;
            }
        }
        data->AddInstruction(dataInstruction);
        dataInstruction = new cmajor::masm::assembly::Instruction(dataOpCode);
        dataInstruction->SetNoColon();
    }
    dataInstruction->AddOperand(dataValue);
    prevDataOpCode = dataOpCode;
}

void CodeGenerator::Visit(GlobalVariable& globalVariable)
{
    if (data && dataInstruction)
    {
        data->AddInstruction(dataInstruction);
        dataInstruction = nullptr;
    }
    if (globalVariable.Initializer())
    {
        file.GetDeclarationSection().AddPublicDataDeclaration(new cmajor::masm::assembly::PublicDataDeclaration(globalVariable.Name()));
        label = globalVariable.Name();
        data = new cmajor::masm::assembly::Data();
        file.GetDataSection().AddData(data);
        leader = true;
        currentOffset = 0;
        globalVariable.Initializer()->Accept(*this);
        ByteType* byteType = context->GetTypes().GetByteType();
        while ((currentOffset & 7) != 0)
        {
            ByteValue value(0u, byteType);
            Visit(value);
        }
    }
    else
    {
        file.GetDeclarationSection().AddExternalDataDeclaration(new cmajor::masm::assembly::ExternalDataDeclaration(globalVariable.Name()));
    }
}

void CodeGenerator::Visit(Function& function)
{
    if (!function.IsDefined())
    {
        file.GetDeclarationSection().AddFunctionDeclaration(new cmajor::masm::assembly::FunctionDeclaration(function.Name()));
    }
    else
    {
        file.GetDeclarationSection().AddPublicDataDeclaration(new cmajor::masm::assembly::PublicDataDeclaration(function.Name()));
        currentFunction = &function;
        context->AssemblyContext()->ResetRegisterPool();
        assemblyFunction = file.GetCodeSection().CreateFunction(function.Name());
        context->AssemblyContext()->SetCurrentFunction(assemblyFunction);
        std::unique_ptr<RegisterAllocator> linearScanRregisterAllocator = CreateLinearScanRegisterAllocator(function, Ctx());
        registerAllocator = linearScanRregisterAllocator.get();
        function.VisitBasicBlocks(*this);
        assemblyFunction->SetActiveFunctionPart(cmajor::masm::assembly::FunctionPart::prologue);
        EmitPrologue(*this);
        assemblyFunction->SetActiveFunctionPart(cmajor::masm::assembly::FunctionPart::epilogue);
        EmitEpilogue(*this);
    }
}

void CodeGenerator::Visit(BasicBlock& basicBlock)
{
    Instruction* inst = basicBlock.FirstInstruction();
    leader = true;
    while (inst)
    {
        currentInst = inst;
        if (inst->IsArgInstruction() || inst->IsProcedureCallInstruction() || inst->IsFunctionCallInstruction())
        {
            inst->Accept(*this);
        }
        else
        {
            RegisterAllocationAction action = registerAllocator->Run(inst);
            if (action == RegisterAllocationAction::spill)
            {
                for (const SpillData& spillData : registerAllocator->GetSpillData())
                {
                    if (spillData.registerGroupToSpill->IsFloatingPointReg())
                    {
                        EmitFloatingPointStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, *this);
                    }
                    else
                    {
                        EmitIntegerStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, *this);
                    }
                }
            }
            inst->Accept(*this);
        }
        inst = inst->Next();
    }
}

void CodeGenerator::Visit(RetInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointRet(inst, *this);
    }
    else
    {
        EmitIntegerRet(inst, *this);
    }
}

void CodeGenerator::Visit(SignExtendInstruction& inst)
{
    EmitSignExtend(inst, *this);
}

void CodeGenerator::Visit(ZeroExtendInstruction& inst)
{
    EmitZeroExtend(inst, *this);
}

void CodeGenerator::Visit(ParamInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointParam(inst, *this);
    }
    else
    {
        EmitIntegerParam(inst, *this);
    }
}

void CodeGenerator::Visit(LoadInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointLoad(inst, *this);
    }
    else
    {
        EmitIntegerLoad(inst, *this);
    }
}

void CodeGenerator::Visit(StoreInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointStore(inst, *this);
    }
    else
    {
        EmitIntegerStore(inst, *this);
    }
}

void CodeGenerator::Visit(ArgInstruction& inst)
{
    args.push_back(&inst);
}

void CodeGenerator::Visit(ProcedureCallInstruction& inst)
{
    EmitProcedureCall(inst, args, *this);
    args.clear();
}

void CodeGenerator::Visit(FunctionCallInstruction& inst)
{
    EmitFunctionCall(inst, args, *this);
    args.clear();
}

void CodeGenerator::Visit(ElemAddrInstruction& inst)
{
    EmitElemAddr(inst, *this);
}

void CodeGenerator::Visit(PtrOffsetInstruction& inst)
{
    EmitPtrOffset(inst, *this);
}

void CodeGenerator::Visit(PtrDiffInstruction& inst)
{
    EmitPtrDiff(inst, *this);
}

void CodeGenerator::Visit(TruncateInstruction& inst)
{
    if (inst.GetType()->IsIntegerType())
    {
        EmitIntegerTruncate(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointTruncate(inst, *this);
    }
}

void CodeGenerator::Visit(AddInstruction& inst)
{
    if (inst.GetType()->IsIntegerType())
    {
        EmitIntegerBinOpInst(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointBinOpInst(inst, *this);
    }
}

void CodeGenerator::Visit(SubInstruction& inst)
{
    if (inst.GetType()->IsIntegerType())
    {
        EmitIntegerBinOpInst(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointBinOpInst(inst, *this);
    }
}

void CodeGenerator::Visit(MulInstruction& inst)
{
    if (inst.GetType()->IsIntegerType())
    {
        EmitIntegerBinOpInst(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointBinOpInst(inst, *this);
    }
}

void CodeGenerator::Visit(DivInstruction& inst)
{
    if (inst.GetType()->IsIntegerType())
    {
        EmitIntegerBinOpInst(inst, *this);
    }
    else if (inst.GetType()->IsFloatingPointType())
    {
        EmitFloatingPointBinOpInst(inst, *this);
    }
}

void CodeGenerator::Visit(ModInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(AndInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(OrInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(XorInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(ShlInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(ShrInstruction& inst)
{
    EmitIntegerBinOpInst(inst, *this);
}

void CodeGenerator::Visit(BitcastInstruction& inst)
{
    EmitBitcast(inst, *this);
}

void CodeGenerator::Visit(IntToFloatInstruction& inst)
{
    EmitIntToFloat(inst, *this);
}

void CodeGenerator::Visit(FloatToIntInstruction& inst)
{
    EmitFloatToInt(inst, *this);
}

void CodeGenerator::Visit(IntToPtrInstruction& inst)
{
    EmitIntToPtr(inst, *this);
}

void CodeGenerator::Visit(PtrToIntInstruction& inst)
{
    EmitPtrToInt(inst, *this);
}

void CodeGenerator::Visit(EqualInstruction& inst)
{
    if (inst.Left()->GetType()->IsFloatingPointType())
    {
        EmitFloatingPointEqual(inst, *this);
    }
    else
    {
        EmitIntegerEqual(inst, *this);
    }
}

void CodeGenerator::Visit(LessInstruction& inst)
{
    if (inst.Left()->GetType()->IsFloatingPointType())
    {
        EmitFloatingPointLess(inst, *this);
    }
    else
    {
        EmitIntegerLess(inst, *this);
    }
}

void CodeGenerator::Visit(JmpInstruction& inst)
{
    EmitJmp(inst, *this);
}

void CodeGenerator::Visit(BranchInstruction& inst)
{
    EmitBranch(inst, *this);
}

void CodeGenerator::Visit(NotInstruction& inst)
{
    EmitNot(inst, *this);
}

void CodeGenerator::Visit(NegInstruction& inst)
{
    if (inst.IsFloatingPointInstruction())
    {
        EmitFloatingPointNeg(inst, *this);
    }
    else
    {
        EmitIntegerNeg(inst, *this);
    }
}

void CodeGenerator::Visit(NoOperationInstruction& inst)
{
    EmitNop(inst, *this);
}

void CodeGenerator::Visit(SwitchInstruction& inst)
{
    EmitSwitch(inst, *this);
}

void CodeGenerator::Visit(BoolValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value.ToInteger(), 1), cmajor::masm::assembly::OpCode::DB);
    currentOffset += 1;
}

void CodeGenerator::Visit(SByteValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value.GetValue(), 1), cmajor::masm::assembly::OpCode::DB);
    currentOffset += 1;
}

void CodeGenerator::Visit(ByteValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value.GetValue(), 1), cmajor::masm::assembly::OpCode::DB);
    currentOffset += 1;
}

void CodeGenerator::Visit(ShortValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value.GetValue(), 2), cmajor::masm::assembly::OpCode::DW);
    currentOffset += 2;
}

void CodeGenerator::Visit(UShortValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value.GetValue(), 2), cmajor::masm::assembly::OpCode::DW);
    currentOffset += 2;
}

void CodeGenerator::Visit(IntValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value.GetValue(), 4), cmajor::masm::assembly::OpCode::DD);
    currentOffset += 4;
}

void CodeGenerator::Visit(UIntValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value.GetValue(), 4), cmajor::masm::assembly::OpCode::DD);
    currentOffset += 4;
}

void CodeGenerator::Visit(LongValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value.GetValue(), 8), cmajor::masm::assembly::OpCode::DQ);
    currentOffset += 8;
}

void CodeGenerator::Visit(ULongValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value.GetValue(), 8), cmajor::masm::assembly::OpCode::DQ);
    currentOffset += 8;
}

void CodeGenerator::Visit(FloatValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::FloatLiteral(value.GetValue()), cmajor::masm::assembly::OpCode::REAL4);
    currentOffset += 4;
}

void CodeGenerator::Visit(DoubleValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::DoubleLiteral(value.GetValue()), cmajor::masm::assembly::OpCode::REAL8);
    currentOffset += 8;
}

void CodeGenerator::Visit(NullValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(0, 8), cmajor::masm::assembly::OpCode::DQ);
    currentOffset += 8;
}

void CodeGenerator::Visit(AddressValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::Symbol(value.GetValue()->Name()), cmajor::masm::assembly::OpCode::DQ);
    currentOffset += 8;
}

void CodeGenerator::Visit(ArrayValue& value)
{
    for (const auto& element : value.Elements())
    {
        element->Accept(*this);
    }
}

void CodeGenerator::Visit(StructureValue& value)
{
    for (const auto& fieldValue : value.FieldValues())
    {
        fieldValue->Accept(*this);
    }
}

void CodeGenerator::Visit(StringValue& value)
{
    std::string hexByteStr;
    std::string stringValue;
    int state = 0;
    for (char c : value.GetValue())
    {
        switch (state)
        {
            case 0:
            {
                if (c == '\\')
                {
                    if (!stringValue.empty())
                    {
                        EmitDataValue(new cmajor::masm::assembly::StringLiteral(stringValue), cmajor::masm::assembly::OpCode::DB);
                        stringValue.clear();
                    }
                    state = 1;
                }
                else
                {
                    stringValue.append(1, c);
                    currentOffset += 1;
                }
                break;
            }
            case 1:
            {
                hexByteStr.append(1, c);
                state = 2;
                break;
            }
            case 2:
            {
                hexByteStr.append(1, c);
                uint8_t value = util::ParseHexByte(hexByteStr);
                EmitDataValue(new cmajor::masm::assembly::IntegerLiteral(value, 1), cmajor::masm::assembly::OpCode::DB);
                currentOffset += 1;
                hexByteStr.clear();
                state = 0;
                break;
            }
        }
    }
    if (!stringValue.empty())
    {
        EmitDataValue(new cmajor::masm::assembly::StringLiteral(stringValue), cmajor::masm::assembly::OpCode::DB);
        stringValue.clear();
    }
}

void CodeGenerator::Visit(StringArrayValue& value)
{
    for (auto stringValue : value.Strings())
    {
        stringValue->Accept(*this);
    }
}

void CodeGenerator::Visit(ConversionValue& value)
{
    value.From()->Accept(*this);
}

void CodeGenerator::Visit(ClsIdValue& value)
{
    int x = 0; // TODO
}

void CodeGenerator::Visit(SymbolValue& value)
{
    EmitDataValue(new cmajor::masm::assembly::Symbol(value.Symbol()), cmajor::masm::assembly::OpCode::DQ);
}

void CodeGenerator::Error(const std::string& message)
{
    cmajor::masm::intermediate::Error(message, currentInst->Span(), context);
}

void CodeGenerator::WriteOutputFile()
{
    if (data && dataInstruction)
    {
        data->AddInstruction(dataInstruction);
    }
    file.Write();
}

} // cmajor::masm::intermediate
