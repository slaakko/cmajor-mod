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

namespace cmajor::masm::intermediate {

void EmitLoad(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator);
cmajor::masm::assembly::Register* MakeRegOperand(Value* value, cmajor::masm::assembly::Register* reg, CodeGenerator& codeGenerator);
FrameLocation GetFrameLocation(Value* value, CodeGenerator& codeGenerator);
void EmitFrameLocationOperand(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator);

void StoreParamToHome(ParamInstruction* inst, CodeGenerator& codeGenerator)
{
    Type* type = inst->GetType();
    int64_t size = type->Size();
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::Register* paramReg = nullptr;
    switch (inst->Index())
    {
        case 0: paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rcx); break;
        case 1: paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rdx); break;
        case 2: paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r8); break;
        case 3: paramReg = assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r9); break;
    }
    if (paramReg)
    {
        cmajor::masm::assembly::Instruction* movToHomeInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        FrameLocation home = GetFrameLocation(inst, codeGenerator);
        if (home.Valid())
        {
            EmitFrameLocationOperand(size, home, movToHomeInst, codeGenerator);
            movToHomeInst->AddOperand(paramReg);
            codeGenerator.Emit(movToHomeInst);
        }
        else
        {
            codeGenerator.Error("error store to home: invalid home location for 'param' instruction not valid");
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

    cmajor::masm::assembly::Instruction* pushRbp = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::PUSH);
    pushRbp->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbp));
    codeGenerator.Emit(pushRbp);

    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedLocalRegs())
    {
        cmajor::masm::assembly::Instruction* pushReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::PUSH);
        pushReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(pushReg);
    }
    for (const auto& regGroup : assemblyContext->GetRegisterPool()->UsedNonvolatileRegs())
    {
        cmajor::masm::assembly::Instruction* pushReg = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::PUSH);
        pushReg->AddOperand(regGroup->GetReg(8));
        codeGenerator.Emit(pushReg);
    }

    Frame& frame = codeGenerator.RegAllocator()->GetFrame();

    cmajor::masm::assembly::Instruction* subRsp = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::SUB);
    subRsp->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp));
    subRsp->AddOperand(assemblyContext->MakeNumericLiteral(frame.Size(), 8));
    codeGenerator.Emit(subRsp);

    cmajor::masm::assembly::Instruction* leaRbp = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
    leaRbp->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbp));
    leaRbp->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
        assemblyContext->MakeBinaryExpr(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp),
            assemblyContext->MakeNumericLiteral(frame.LocalSize(), 8), cmajor::masm::assembly::Operator::add))));
    codeGenerator.Emit(leaRbp);
}
   

void EmitEpilogue(CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Instruction* leaderInst = nullptr;
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();

    Frame& frame = codeGenerator.RegAllocator()->GetFrame();

    cmajor::masm::assembly::Instruction* leaRsp = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::LEA);
    if (!leaderInst)
    {
        leaderInst = leaRsp;
        leaderInst->SetLabel("@" + std::to_string(codeGenerator.ExitLabelId()));
    }
    leaRsp->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rsp));
    leaRsp->AddOperand(assemblyContext->MakeSizePrefix(8, assemblyContext->MakeContent(
        assemblyContext->MakeBinaryExpr(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbp),
            assemblyContext->MakeNumericLiteral(frame.Size() - frame.LocalSize(), 8), cmajor::masm::assembly::Operator::add))));
    codeGenerator.Emit(leaRsp);

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
    ret->AddOperand(assemblyContext->MakeNumericLiteral(0, 1));
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
    cmajor::masm::assembly::NumericLiteral* frameLoc = assemblyContext->MakeNumericLiteral(frameLocation.ItemOffset(), 8);
    codeGenerator.AddFrameLocation(frameLoc);
    instruction->AddOperand(assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(
        assemblyContext->MakeBinaryExpr(reg, frameLoc, cmajor::masm::assembly::Operator::add))));
}

void EmitPtrOperand(int64_t size, Value* value, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
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
        leaInst->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbx));
        leaInst->AddOperand(assemblyContext->MakeSymbol(globalVar->Name()));
        codeGenerator.Emit(leaInst);
        instruction->AddOperand(assemblyContext->MakeSizePrefix(size, assemblyContext->MakeContent(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbx))));
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
                MakeRegOperand(value, assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator)));
            instruction->AddOperand(operand);
        }
    }
}

cmajor::masm::assembly::Register* MakeRegOperand(Value* value, cmajor::masm::assembly::Register* reg, CodeGenerator& codeGenerator)
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
                EmitPtrOperand(size, value, leaInst, codeGenerator);
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
                    cmajor::masm::assembly::RegisterGroup* regGroup = assemblyContext->GetRegisterPool()->GetGlobalRegisterGroup(reg->Group());
                    EmitLoad(size, frameLocation, regGroup, codeGenerator);
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
            inst->AddOperand(assemblyContext->MakeNumericLiteral(v->GetValue(), 1));
            break;
        }
        case ValueKind::sbyteValue:
        {
            SByteValue* v = static_cast<SByteValue*>(value);
            inst->AddOperand(assemblyContext->MakeNumericLiteral(v->GetValue(), 1));
            break;
        }
        case ValueKind::byteValue:
        {
            ByteValue* v = static_cast<ByteValue*>(value);
            inst->AddOperand(assemblyContext->MakeNumericLiteral(v->GetValue(), 1));
            break;
        }
        case ValueKind::shortValue:
        {
            ShortValue* v = static_cast<ShortValue*>(value);
            inst->AddOperand(assemblyContext->MakeNumericLiteral(v->GetValue(), 2));
            break;
        }
        case ValueKind::ushortValue:
        {
            UShortValue* v = static_cast<UShortValue*>(value);
            inst->AddOperand(assemblyContext->MakeNumericLiteral(v->GetValue(), 2));
            break;
        }
        case ValueKind::intValue:
        {
            IntValue* v = static_cast<IntValue*>(value);
            inst->AddOperand(assemblyContext->MakeNumericLiteral(v->GetValue(), 4));
            break;
        }
        case ValueKind::uintValue:
        {
            UIntValue* v = static_cast<UIntValue*>(value);
            inst->AddOperand(assemblyContext->MakeNumericLiteral(v->GetValue(), 4));
            break;
        }
        case ValueKind::longValue:
        {
            LongValue* v = static_cast<LongValue*>(value);
            inst->AddOperand(assemblyContext->MakeNumericLiteral(v->GetValue(), 8));
            break;
        }
        case ValueKind::ulongValue:
        {
            ULongValue* v = static_cast<ULongValue*>(value);
            inst->AddOperand(assemblyContext->MakeNumericLiteral(v->GetValue(), 8));
            break;
        }
        case ValueKind::floatValue:
        {
            FloatValue* v = static_cast<FloatValue*>(value);
            // todo
            throw std::runtime_error("floats not implemented");
        }
        case ValueKind::doubleValue:
        {
            DoubleValue* v = static_cast<DoubleValue*>(value);
            // todo
            throw std::runtime_error("floats not implemented");
        }
        case ValueKind::nullValue:
        {
            inst->AddOperand(assemblyContext->MakeNumericLiteral(0, 8));
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
        }
        }
    }
    return reg;
}

void EmitRet(RetInstruction& inst, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    if (inst.ReturnValue())
    {
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        int64_t size = inst.ReturnValue()->GetType()->Size();
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax));
        movInst->AddOperand(MakeRegOperand(inst.ReturnValue(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rbx), codeGenerator));
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
        cmajor::masm::assembly::Register* sourceReg = MakeRegOperand(
            inst.Operand(), assemblyContext->GetGlobalReg(operandSize, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator);
        movsxInst->AddOperand(sourceReg);
        codeGenerator.Emit(movsxInst);
    }
    else
    {
        codeGenerator.Error("error emitting sign extend instruction: reg group for inst not found");
    }
}

void EmitLoad(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator)
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

void EmitStore(int64_t size, const FrameLocation& frameLocation, cmajor::masm::assembly::RegisterGroup* regGroup, CodeGenerator& codeGenerator)
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

void EmitLoad(LoadInstruction& inst, CodeGenerator& codeGenerator)
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
        EmitPtrOperand(size, inst.Ptr(), instruction, codeGenerator);
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting load: invalid type kind");
    }
}

void EmitStore(StoreInstruction& inst, CodeGenerator& codeGenerator)
{
    Type* type = inst.GetValue()->GetType();
    if (type->IsFundamentalType() || type->IsPointerType())
    {
        int64_t size = type->Size();
        cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
        cmajor::masm::assembly::Instruction* instruction = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        EmitPtrOperand(size, inst.GetPtr(), instruction, codeGenerator);
        instruction->AddOperand(MakeRegOperand(inst.GetValue(), assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator));
        codeGenerator.Emit(instruction);
    }
    else
    {
        codeGenerator.Error("error emitting store: invalid type kind");
    }
}

void EmitParam(ParamInstruction& inst, CodeGenerator& codeGenerator)
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
            EmitLoad(size, frameLocation, regGroup, codeGenerator);
        }
        else
        {
            codeGenerator.Error("error emitting param: no frame location");
        }
    }
    }
}

void EmitArgOperand(Instruction* argInst, int64_t size, Value* arg, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
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
    instruction->AddOperand(MakeRegOperand(arg, assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rax), codeGenerator));
}

void EmitArgLocationOperand(int64_t size, const ArgLocation& argLocation, cmajor::masm::assembly::Instruction* instruction, CodeGenerator& codeGenerator)
{
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::Register* rbp = assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rbp);
    instruction->AddOperand(assemblyContext->MakeSizePrefix(size,
        assemblyContext->MakeContent(assemblyContext->MakeBinaryExpr(rbp, assemblyContext->MakeNumericLiteral(argLocation.ItemOffset(), 8), cmajor::masm::assembly::Operator::add))));
}

void EmitArg(ArgInstruction& inst, CallFrame* callFrame, int32_t index, CodeGenerator& codeGenerator)
{
    RegisterAllocationAction action = codeGenerator.RegAllocator()->Run(&inst);
    if (action == RegisterAllocationAction::spill)
    {
        for (const SpillData& spillData : codeGenerator.RegAllocator()->GetSpillData())
        {
            EmitStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
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
        EmitArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    case 1:
    {
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::rdx));
        EmitArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    case 2:
    {
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r8));
        EmitArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
        codeGenerator.Emit(movInst);
        break;
    }
    case 3:
    {
        cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
        movInst->AddOperand(assemblyContext->GetGlobalReg(size, cmajor::masm::assembly::RegisterGroupKind::r9));
        EmitArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
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
            EmitArgOperand(&inst, size, inst.Arg(), movInst, codeGenerator);
            codeGenerator.Emit(movInst);
            cmajor::masm::assembly::Instruction* pushToFrameInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
            ArgLocation argLocation = callFrame->GetArgLocation(index - 4);
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
    for (int i = 4; i < n; ++i)
    {
        int64_t size = args[i]->Arg()->GetType()->Size();
        callFrame.AllocateArgLocation(size);
    }
    for (int i = n - 1; i >= 0; --i)
    {
        EmitArg(*args[i], &callFrame, i, codeGenerator);
    }
}

cmajor::masm::assembly::Value* MakeCalleeOperand(Value* value, cmajor::masm::assembly::Register* reg, CodeGenerator& codeGenerator)
{
    if (value->Kind() == ValueKind::regValue)
    {
        return MakeRegOperand(value, reg, codeGenerator);
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
            EmitStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
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
            EmitStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, codeGenerator);
        }
    }
    cmajor::masm::assembly::Context* assemblyContext = codeGenerator.Ctx()->AssemblyContext();
    cmajor::masm::assembly::RegisterGroup* regGroup = codeGenerator.RegAllocator()->GetRegisterGroup(&inst);
    cmajor::masm::assembly::Register* reg = regGroup->GetReg(8);
    cmajor::masm::assembly::Instruction* callInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::CALL);
    callInst->AddOperand(MakeCalleeOperand(inst.Callee(), reg, codeGenerator));
    codeGenerator.Emit(callInst);
    cmajor::masm::assembly::Instruction* movInst = new cmajor::masm::assembly::Instruction(cmajor::masm::assembly::OpCode::MOV);
    movInst->AddOperand(reg);
    movInst->AddOperand(assemblyContext->GetGlobalReg(8, cmajor::masm::assembly::RegisterGroupKind::rax));
    codeGenerator.Emit(movInst);
}

CodeGenerator::CodeGenerator(Context* context_, const std::string& assemblyFilePath_) :
    Visitor(context_), context(context_), file(assemblyFilePath_), currentFunction(nullptr), currentInst(nullptr), assemblyFunction(nullptr), leader(false), 
    registerAllocator(nullptr)
{
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
        std::unique_ptr<RegisterAllocator> linearScanRregisterAllocator = CreateLinearScanRegisterAllocator(function, Ctx());
        registerAllocator = linearScanRregisterAllocator.get();
        assemblyFunction = file.GetCodeSection().CreateFunction(function.Name());
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
                    EmitStore(8, spillData.spillToFrameLocation, spillData.registerGroupToSpill, *this);
                }
            }
            inst->Accept(*this);
        }
        inst = inst->Next();
    }
}

void CodeGenerator::Visit(RetInstruction& inst)
{
    EmitRet(inst, *this);
}

void CodeGenerator::Visit(SignExtendInstruction& inst)
{
    EmitSignExtend(inst, *this);
}

void CodeGenerator::Visit(ParamInstruction& inst)
{
    EmitParam(inst, *this);
}

void CodeGenerator::Visit(LoadInstruction& inst)
{
    EmitLoad(inst, *this);
}

void CodeGenerator::Visit(StoreInstruction& inst)
{
    EmitStore(inst, *this);
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

void CodeGenerator::Error(const std::string& message)
{
    cmajor::masm::intermediate::Error(message, currentInst->Span(), context);
}

void CodeGenerator::AddFrameLocation(cmajor::masm::assembly::NumericLiteral* frameLoc)
{
    frameLocations.push_back(frameLoc);
}

void CodeGenerator::WriteOutputFile()
{
    file.Write();
}

} // cmajor::masm::intermediate
