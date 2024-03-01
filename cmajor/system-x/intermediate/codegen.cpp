// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.codegen;

import cmajor.systemx.intermediate.code;
import cmajor.systemx.intermediate.data;
import cmajor.systemx.intermediate.reg.allocator;
import cmajor.systemx.machine;
import cmajor.systemx.assembler;
import util;

namespace cmajor::systemx::intermediate {

CodeGenerator::~CodeGenerator()
{
}

cmajor::systemx::assembler::Node* MakeRegOperand(const Register& reg)
{
    switch (reg.kind)
    {
    case RegisterKind::local:
    {
        return cmajor::systemx::assembler::MakeLocalRegOperand(reg.number);
    }
    case RegisterKind::global:
    {
        return cmajor::systemx::assembler::MakeGlobalRegOperand(reg.number);
    }
    default:
    {
        throw std::runtime_error("invalid register");
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

int64_t GetOffset(Type* type, int64_t index, CodeGenerator& codeGen)
{
    StructureType* structureType = type->GetStructurePointeeType(codeGen.GetSourcePos(), codeGen.Ctx());
    return structureType->GetFieldOffset(index);
}

int64_t GetElementSize(Type* type, CodeGenerator& codeGen)
{
    ArrayType* arrayType = type->GetArrayPointeeType(codeGen.GetSourcePos(), codeGen.Ctx());
    return arrayType->ElementType()->Size();
}

int64_t GetPointeeSize(Type* type, CodeGenerator& codeGen)
{
    if (type->IsPointerType())
    {
        PointerType* pointerType = static_cast<PointerType*>(type);
        Type* pointeeType = pointerType->RemovePointer(codeGen.GetSourcePos(), codeGen.Ctx());
        return pointeeType->Size();
    }
    else
    {
        codeGen.Error("error getting pointee size: pointer type expected");
    }
    return -1;
}

FrameLocation GetFrameLocation(Value* value, CodeGenerator& codeGen)
{
    Instruction* inst = nullptr;
    if (value->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        inst = regValue->Inst();
        if (!inst)
        {
            codeGen.Error("error getting frame location: instruction for reg value not set");
        }
    }
    else if (value->IsInstruction())
    {
        inst = static_cast<Instruction*>(value);
    }
    if (inst)
    {
        Locations locs = codeGen.RegAllocator()->GetLocations(inst);
        if ((locs & Locations::frame) != Locations::none)
        {
            FrameLocation frameLocation = codeGen.RegAllocator()->GetFrameLocation(inst);
            if (frameLocation.Valid())
            {
                return frameLocation;
            }
            else
            {
                codeGen.Error("error getting frame location: frame location not valid");
            }
        }
        else
        {
            codeGen.Error("error getting frame location: value not in frame");
        }
    }
    else
    {
        codeGen.Error("error getting frame location: invalid value kind");
    }
    return FrameLocation();
}

void EmitFrameLocationOperand(const FrameLocation& frameLocation, cmajor::systemx::assembler::Instruction* instruction, CodeGenerator& codeGen)
{
    instruction->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regFP)));
    if (frameLocation.IsWithinImmediateRange())
    {
        instruction->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(frameLocation.offset));
    }
    else
    {
        cmajor::systemx::assembler::Instruction* setIxInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        setIxInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regIX));
        uint64_t value = frameLocation.offset;
        if (value <= std::numeric_limits<uint16_t>::max())
        {
            setIxInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint16_t>(value)));
        }
        else if (value <= std::numeric_limits<uint32_t>::max())
        {
            setIxInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint32_t>(value)));
        }
        else
        {
            setIxInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint64_t>(value)));
        }
        codeGen.Emit(setIxInst);
        instruction->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regIX));
    }
}

void EmitArgLocationOperand(cmajor::systemx::assembler::Instruction* instruction, CodeGenerator& codeGen)
{
    instruction->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regFP)));
    cmajor::systemx::assembler::Instruction* setIxInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setIxInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regIX));
    cmajor::systemx::assembler::HexadecimalConstant* node = new cmajor::systemx::assembler::HexadecimalConstant(soul::ast::SourcePos(), 0);
    codeGen.RegAllocator()->GetFrame().CurrentCallFrame()->NextArgLocation(node);
    setIxInst->AddOperand(node);
    codeGen.Emit(setIxInst);
    instruction->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regIX));
}

void EmitPtrOperand(Value* value, cmajor::systemx::assembler::Instruction* instruction, CodeGenerator& codeGen)
{
    Instruction* inst = nullptr;
    if (value->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        inst = regValue->Inst();
        if (!inst)
        {
            codeGen.Error("error emitting ptr operand: instruction for reg value not set");
        }
    }
    else if (value->IsInstruction())
    {
        inst = static_cast<Instruction*>(value);
    }
    else if (value->IsAddressValue())
    {
        AddressValue* addressValue = static_cast<AddressValue*>(value);
        GlobalVariable* globalVar = addressValue->Value();
        cmajor::systemx::assembler::Instruction* ldouInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::LDOU);
        ldouInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        ldouInst->AddOperand(cmajor::systemx::assembler::MakeGlobalSymbol(globalVar->Name()));
        codeGen.Emit(ldouInst);
        instruction->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        return;
    }
    if (inst)
    {
        if (inst->IsLocalInstruction())
        {
            FrameLocation frameLocation = GetFrameLocation(value, codeGen);
            EmitFrameLocationOperand(frameLocation, instruction, codeGen);
        }
        else
        {
            cmajor::systemx::assembler::Node* operand = MakeRegOperand(value, GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
            instruction->AddOperand(operand);
        }
    }
    else
    {
        codeGen.Error("error emitting ptr operand: invalid value kind");
    }
}

cmajor::systemx::assembler::Node* MakeRegOperand(Value* value, const Register& r, CodeGenerator& codeGen)
{
    if (value->Kind() == ValueKind::regValue)
    {
        RegValue* regValue = static_cast<RegValue*>(value);
        Instruction* inst = regValue->Inst();
        if (inst)
        {
            if (inst->IsLocalInstruction())
            {
                cmajor::systemx::assembler::Instruction* ldaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::LDA);
                ldaInst->AddOperand(MakeRegOperand(r));
                EmitPtrOperand(value, ldaInst, codeGen);
                codeGen.Emit(ldaInst);
                return MakeRegOperand(r);
            }
            Locations locs = codeGen.RegAllocator()->GetLocations(inst);
            if ((locs & Locations::reg) != Locations::none)
            {
                Register reg = codeGen.RegAllocator()->GetRegister(inst);
                if (reg.Valid())
                {
                    return MakeRegOperand(reg);
                }
                else
                {
                    codeGen.Error("error making reg operand: reg not valid");
                }
            }
            else if ((locs & Locations::frame) != Locations::none)
            {
                FrameLocation frameLocation = codeGen.RegAllocator()->GetFrameLocation(inst);
                if (frameLocation.Valid())
                {
                    EmitLoad(frameLocation, r, codeGen);
                }
                else
                {
                    codeGen.Error("error making reg operand: frame location not valid");
                }
            }
            else
            {
                codeGen.Error("error making reg operand: no locations for inst");
            }
        }
        else
        {
            codeGen.Error("error making reg operand: instruction for reg value not set");
        }
    }
    else if (value->IsAddressValue())
    {
        AddressValue* v = static_cast<AddressValue*>(value);
        cmajor::systemx::assembler::Instruction* ldouInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::LDOU);
        ldouInst->AddOperand(MakeRegOperand(r));
        ldouInst->AddOperand(cmajor::systemx::assembler::MakeGlobalSymbol(v->Value()->Name()));
        codeGen.Emit(ldouInst);
    }
    else
    {
        cmajor::systemx::assembler::Instruction* inst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        inst->AddOperand(MakeRegOperand(r));
        codeGen.Emit(inst);
        switch (value->Kind())
        {
        case ValueKind::boolValue:
        {
            BoolValue* v = static_cast<BoolValue*>(value);
            inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            break;
        }
        case ValueKind::sbyteValue:
        {
            SByteValue* v = static_cast<SByteValue*>(value);
            if (v->GetValue() < 0)
            {
                inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::machine::SignExtend(v->GetValue())));
            }
            else
            {
                inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            }
            break;
        }
        case ValueKind::byteValue:
        {
            ByteValue* v = static_cast<ByteValue*>(value);
            inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            break;
        }
        case ValueKind::shortValue:
        {
            ShortValue* v = static_cast<ShortValue*>(value);
            if (v->GetValue() < 0)
            {
                inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::machine::SignExtend(v->GetValue())));
            }
            else
            {
                inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            }
            break;
        }
        case ValueKind::ushortValue:
        {
            UShortValue* v = static_cast<UShortValue*>(value);
            inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            break;
        }
        case ValueKind::intValue:
        {
            IntValue* v = static_cast<IntValue*>(value);
            if (v->GetValue() < 0)
            {
                inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::machine::SignExtend(v->GetValue())));
            }
            else
            {
                inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            }
            break;
        }
        case ValueKind::uintValue:
        {
            UIntValue* v = static_cast<UIntValue*>(value);
            inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            break;
        }
        case ValueKind::longValue:
        {
            LongValue* v = static_cast<LongValue*>(value);
            if (v->GetValue() < 0)
            {
                inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::machine::SignExtend(v->GetValue())));
            }
            else
            {
                inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            }
            break;
        }
        case ValueKind::ulongValue:
        {
            ULongValue* v = static_cast<ULongValue*>(value);
            inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            break;
        }
        case ValueKind::floatValue:
        {
            FloatValue* v = static_cast<FloatValue*>(value);
            inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            break;
        }
        case ValueKind::doubleValue:
        {
            DoubleValue* v = static_cast<DoubleValue*>(value);
            inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(v->GetValue()));
            break;
        }
        case ValueKind::nullValue:
        {
            inst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint64_t>(0)));
            break;
        }
        case ValueKind::symbolValue:
        {
            SymbolValue* v = static_cast<SymbolValue*>(value);
            inst->AddOperand(cmajor::systemx::assembler::MakeGlobalSymbol(v->Symbol()));
            break;
        }
        default:
        {
            codeGen.Error("error making reg operand: not implemented for value kind " + value->KindStr());
        }
        }
    }
    return MakeRegOperand(r);
}

cmajor::systemx::assembler::Node* MakeCalleeOperand(Value* value, const Register& reg, CodeGenerator& codeGen)
{
    if (value->IsSymbolValue())
    {
        SymbolValue* symbolValue = static_cast<SymbolValue*>(value);
        return cmajor::systemx::assembler::MakeGlobalSymbol(symbolValue->Symbol());
    }
    else
    {
        return MakeRegOperand(value, reg, codeGen);
    }
}

cmajor::systemx::assembler::Node* MakeTrapOperand(Value* value, CodeGenerator& codeGen)
{
    if (value->IsSymbolValue())
    {
        SymbolValue* symbolValue = static_cast<SymbolValue*>(value);
        return cmajor::systemx::assembler::MakeGlobalSymbol(symbolValue->Symbol());
    }
    else
    {
        switch (value->Kind())
        {
        case ValueKind::sbyteValue:
        {
            SByteValue* v = static_cast<SByteValue*>(value);
            return cmajor::systemx::assembler::MakeConstantExpr(v->GetValue());
        }
        case ValueKind::byteValue:
        {
            ByteValue* v = static_cast<ByteValue*>(value);
            return cmajor::systemx::assembler::MakeConstantExpr(v->GetValue());
        }
        default:
        {
            codeGen.Error("error making trap operand: not implemented for value kind " + value->KindStr());
        }
        }
    }
    return nullptr;
}

void EmitParam(ParamInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting param: reg not valid");
    }
    Locations locs = codeGen.RegAllocator()->GetLocations(&inst);
    if ((locs & Locations::frame) != Locations::none)
    {
        FrameLocation frameLocation = codeGen.RegAllocator()->GetFrameLocation(&inst);
        EmitLoad(frameLocation, reg, codeGen);
    }
    else
    {
        codeGen.Error("error emitting param: no frame location");
    }
}

void EmitLoad(const FrameLocation& frameLocation, const Register& reg, CodeGenerator& codeGen)
{
    if (!frameLocation.Valid())
    {
        codeGen.Error("error emitting load: frame location not valid");
    }
    cmajor::systemx::assembler::Instruction* instruction = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::LDOU);
    instruction->AddOperand(MakeRegOperand(reg));
    instruction->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regFP)));
    if (frameLocation.IsWithinImmediateRange())
    {
        instruction->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(frameLocation.offset));
    }
    else
    {
        cmajor::systemx::assembler::Instruction* setIxInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        setIxInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regIX));
        setIxInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(frameLocation.offset));
        codeGen.Emit(setIxInst);
        instruction->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regIX));
    }
    codeGen.Emit(instruction);
}

void EmitStore(const FrameLocation& frameLocation, const Register& reg, CodeGenerator& codeGen)
{
    if (!frameLocation.Valid())
    {
        codeGen.Error("error emitting store: frame location not valid");
    }
    cmajor::systemx::assembler::Instruction* instruction = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::STOU);
    instruction->AddOperand(MakeRegOperand(reg));
    EmitFrameLocationOperand(frameLocation, instruction, codeGen);
    codeGen.Emit(instruction);
}

void EmitLoad(LoadInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting load: reg not valid");
    }
    Type* type = inst.Result()->GetType();
    int machineInst = -1;
    if (type->IsFundamentalType())
    {
        switch (type->Id())
        {
        case boolTypeId:
        case sbyteTypeId:
        {
            machineInst = cmajor::systemx::machine::LDB;
            break;
        }
        case byteTypeId:
        {
            machineInst = cmajor::systemx::machine::LDBU;
            break;
        }
        case shortTypeId:
        {
            machineInst = cmajor::systemx::machine::LDW;
            break;
        }
        case ushortTypeId:
        {
            machineInst = cmajor::systemx::machine::LDWU;
            break;
        }
        case intTypeId:
        {
            machineInst = cmajor::systemx::machine::LDT;
            break;
        }
        case uintTypeId:
        {
            machineInst = cmajor::systemx::machine::LDTU;
            break;
        }
        case longTypeId:
        {
            machineInst = cmajor::systemx::machine::LDO;
            break;
        }
        case ulongTypeId:
        {
            machineInst = cmajor::systemx::machine::LDOU;
            break;
        }
        case floatTypeId:
        {
            machineInst = cmajor::systemx::machine::LDSF;
            break;
        }
        case doubleTypeId:
        {
            machineInst = cmajor::systemx::machine::LDOU;
            break;
        }
        }
    }
    else if (type->IsPointerType())
    {
        machineInst = cmajor::systemx::machine::LDOU;
    }
    else
    {
        codeGen.Error("error emitting load: invalid type kind");
    }
    if (machineInst == -1)
    {
        codeGen.Error("error emitting load: invalid machine instruction");
    }
    cmajor::systemx::assembler::Instruction* instruction = new cmajor::systemx::assembler::Instruction(machineInst);
    instruction->AddOperand(MakeRegOperand(reg));
    EmitPtrOperand(inst.Ptr(), instruction, codeGen);
    codeGen.Emit(instruction);
}

void EmitStore(StoreInstruction& inst, CodeGenerator& codeGen)
{
    int machineInst = -1;
    Type* type = inst.GetValue()->GetType();
    if (type->IsFundamentalType())
    {
        switch (type->Id())
        {
        case boolTypeId:
        case sbyteTypeId:
        {
            machineInst = cmajor::systemx::machine::STB;
            break;
        }
        case byteTypeId:
        {
            machineInst = cmajor::systemx::machine::STBU;
            break;
        }
        case shortTypeId:
        {
            machineInst = cmajor::systemx::machine::STW;
            break;
        }
        case ushortTypeId:
        {
            machineInst = cmajor::systemx::machine::STWU;
            break;
        }
        case intTypeId:
        {
            machineInst = cmajor::systemx::machine::STT;
            break;
        }
        case uintTypeId:
        {
            machineInst = cmajor::systemx::machine::STTU;
            break;
        }
        case longTypeId:
        {
            machineInst = cmajor::systemx::machine::STO;
            break;
        }
        case ulongTypeId:
        {
            machineInst = cmajor::systemx::machine::STOU;
            break;
        }
        case floatTypeId:
        {
            machineInst = cmajor::systemx::machine::STSF;
            break;
        }
        case doubleTypeId:
        {
            machineInst = cmajor::systemx::machine::STOU;
            break;
        }
        }
    }
    else if (type->IsPointerType())
    {
        machineInst = cmajor::systemx::machine::STOU;
    }
    else
    {
        codeGen.Error("error emitting store: invalid type kind");
    }
    if (machineInst == -1)
    {
        codeGen.Error("error emitting store: invalid machine instruction");
    }
    cmajor::systemx::assembler::Instruction* instruction = new cmajor::systemx::assembler::Instruction(machineInst);
    instruction->AddOperand(MakeRegOperand(inst.GetValue(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen));
    EmitPtrOperand(inst.GetPtr(), instruction, codeGen);
    codeGen.Emit(instruction);
}

void EmitSignExtension(SignExtendInstruction& inst, CodeGenerator& codeGen)
{
    cmajor::systemx::assembler::Instruction* instruction = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting sign extension: reg not valid");
    }
    instruction->AddOperand(MakeRegOperand(reg));
    instruction->AddOperand(MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen));
    codeGen.Emit(instruction);
}

void EmitNot(NotInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting not: reg not valid");
    }
    if (inst.Operand()->GetType()->IsBooleanType())
    {
        cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
        cmajor::systemx::assembler::Instruction* zszInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::ZSZ);
        zszInst->AddOperand(MakeRegOperand(reg));
        zszInst->AddOperand(operandReg);
        zszInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint8_t>(1)));
        codeGen.Emit(zszInst);
    }
    else if (inst.Operand()->GetType()->IsIntegerType())
    {
        if (inst.Operand()->GetType()->Size() == 1)
        {
            cmajor::systemx::assembler::Instruction* xorInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::XOR);
            xorInst->AddOperand(MakeRegOperand(reg));
            cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
            xorInst->AddOperand(operandReg);
            xorInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint8_t>(0xFF)));
            codeGen.Emit(xorInst);
        }
        else
        {
            cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
            setInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regIX));
            int64_t operandSize = inst.Operand()->GetType()->Size();
            if (operandSize <= 2)
            {
                setInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint16_t>(0xFFFF)));
            }
            else if (operandSize <= 4)
            {
                setInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint32_t>(0xFFFFFFFF)));
            }
            else
            {
                setInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint32_t>(0xFFFFFFFFFFFFFFFF)));
            }
            codeGen.Emit(setInst);
            cmajor::systemx::assembler::Instruction* xorInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::XOR);
            xorInst->AddOperand(MakeRegOperand(reg));
            cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
            xorInst->AddOperand(operandReg);
            xorInst->AddOperand(cmajor::systemx::assembler::MakeGlobalRegOperand(cmajor::systemx::machine::regIX));
            codeGen.Emit(xorInst);
        }
    }
}

void EmitNeg(NegInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting neg: reg not valid");
    }
    cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    int machineInst = -1;
    switch (inst.Result()->GetType()->Id())
    {
    case sbyteTypeId: machineInst = cmajor::systemx::machine::NEG; break;
    case byteTypeId: machineInst = cmajor::systemx::machine::NEGU; break;
    case shortTypeId: machineInst = cmajor::systemx::machine::NEG; break;
    case ushortTypeId: machineInst = cmajor::systemx::machine::NEGU; break;
    case intTypeId: machineInst = cmajor::systemx::machine::NEG; break;
    case uintTypeId: machineInst = cmajor::systemx::machine::NEGU; break;
    case longTypeId: machineInst = cmajor::systemx::machine::NEG; break;
    case ulongTypeId: machineInst = cmajor::systemx::machine::NEGU; break;
    case doubleTypeId: machineInst = cmajor::systemx::machine::FSUB; break;
    default:
    {
        codeGen.Error("error emitting neg: invalid result type");
    }
    }
    cmajor::systemx::assembler::Instruction* negInst = new cmajor::systemx::assembler::Instruction(machineInst);
    negInst->AddOperand(MakeRegOperand(reg));
    if (machineInst == cmajor::systemx::machine::FSUB)
    {
        negInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(0.0));
    }
    negInst->AddOperand(operandReg);
    codeGen.Emit(negInst);
}


void EmitZeroExtension(ZeroExtendInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting zero extension: reg not valid");
    }
    cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX)));
    cmajor::systemx::assembler::Node* maskOperand = nullptr;
    switch (inst.Result()->GetType()->Id())
    {
    case boolTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0x01), true);
        break;
    }
    case byteTypeId:
    case sbyteTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0xFF), true);
        break;
    }
    case ushortTypeId:
    case shortTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0xFFFF), true);
        break;
    }
    case uintTypeId:
    case intTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0xFFFFFFFF), true);
        break;
    }
    case ulongTypeId:
    case longTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0xFFFFFFFFFFFFFFFF), true);
        break;
    }
    }
    if (!maskOperand)
    {
        codeGen.Error("error emitting zero extension: invalid result type");
    }
    setInst->AddOperand(maskOperand);
    codeGen.Emit(setInst);
    cmajor::systemx::assembler::Instruction* andInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::AND);
    andInst->AddOperand(MakeRegOperand(reg));
    andInst->AddOperand(operandReg);
    andInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX)));
    codeGen.Emit(andInst);
}

void EmitSwitch(SwitchInstruction& inst, CodeGenerator& codeGen)
{
    for (const auto& caseTarget : inst.CaseTargets())
    {
        cmajor::systemx::assembler::Node* caseReg = MakeRegOperand(caseTarget.caseValue, GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regCX), codeGen);
        cmajor::systemx::assembler::Instruction* cmpInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::CMP);
        cmpInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        cmpInst->AddOperand(MakeRegOperand(inst.Cond(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen));
        cmpInst->AddOperand(caseReg);
        codeGen.Emit(cmpInst);
        cmajor::systemx::assembler::Instruction* branchInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::BZ);
        branchInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        branchInst->AddOperand(cmajor::systemx::assembler::MakeLocalSymbol(caseTarget.targetBlock->Id()));
        codeGen.Emit(branchInst);
    }
    cmajor::systemx::assembler::Instruction* jmpInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::JMP);
    jmpInst->AddOperand(cmajor::systemx::assembler::MakeLocalSymbol(inst.DefaultTargetBlock()->Id()));
    codeGen.Emit(jmpInst);
}

void EmitBinOpInst(BinaryInstruction& inst, CodeGenerator& codeGen)
{
    int signedMachineInst = -1;
    int unsignedMachineInst = -1;
    int floatingMachineInst = -1;
    int machineInst = -1;
    bool mod = false;
    switch (inst.GetOpCode())
    {
    case OpCode::add:
    {
        signedMachineInst = cmajor::systemx::machine::ADD;
        unsignedMachineInst = cmajor::systemx::machine::ADDU;
        floatingMachineInst = cmajor::systemx::machine::FADD;
        break;
    }
    case OpCode::sub:
    {
        signedMachineInst = cmajor::systemx::machine::SUB;
        unsignedMachineInst = cmajor::systemx::machine::SUBU;
        floatingMachineInst = cmajor::systemx::machine::FSUB;
        break;
    }
    case OpCode::mul:
    {
        signedMachineInst = cmajor::systemx::machine::MUL;
        unsignedMachineInst = cmajor::systemx::machine::MULU;
        floatingMachineInst = cmajor::systemx::machine::FMUL;
        break;
    }
    case OpCode::div_:
    {
        signedMachineInst = cmajor::systemx::machine::DIV;
        unsignedMachineInst = cmajor::systemx::machine::DIVU;
        floatingMachineInst = cmajor::systemx::machine::FDIV;
        break;
    }
    case OpCode::mod:
    {
        signedMachineInst = cmajor::systemx::machine::DIV;
        unsignedMachineInst = cmajor::systemx::machine::DIVU;
        floatingMachineInst = cmajor::systemx::machine::FREM;
        mod = true;
        break;
    }
    case OpCode::and_:
    {
        signedMachineInst = cmajor::systemx::machine::AND;
        unsignedMachineInst = cmajor::systemx::machine::AND;
        break;
    }
    case OpCode::or_:
    {
        signedMachineInst = cmajor::systemx::machine::OR;
        unsignedMachineInst = cmajor::systemx::machine::OR;
        break;
    }
    case OpCode::xor_:
    {
        signedMachineInst = cmajor::systemx::machine::XOR;
        unsignedMachineInst = cmajor::systemx::machine::XOR;
        break;
    }
    case OpCode::shl:
    {
        signedMachineInst = cmajor::systemx::machine::SL;
        unsignedMachineInst = cmajor::systemx::machine::SLU;
        break;
    }
    case OpCode::shr:
    {
        signedMachineInst = cmajor::systemx::machine::SR;
        unsignedMachineInst = cmajor::systemx::machine::SRU;
        break;
    }
    }
    Type* type = inst.Result()->GetType();
    if (type->IsFundamentalType())
    {
        switch (type->Id())
        {
        case sbyteTypeId:
        case shortTypeId:
        case intTypeId:
        case longTypeId:
        {
            machineInst = signedMachineInst;
            break;
        }
        case boolTypeId:
        case byteTypeId:
        case ushortTypeId:
        case uintTypeId:
        case ulongTypeId:
        {
            machineInst = unsignedMachineInst;
            break;
        }
        case floatTypeId:
        case doubleTypeId:
        {
            machineInst = floatingMachineInst;
            break;
        }
        }
    }
    if (machineInst == -1)
    {
        codeGen.Error("error emitting binary operator: invalid machine instruction");
    }
    cmajor::systemx::assembler::Instruction* instruction = new cmajor::systemx::assembler::Instruction(machineInst);
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting binary operator: reg not valid");
    }
    instruction->AddOperand(MakeRegOperand(reg));
    instruction->AddOperand(MakeRegOperand(inst.Left(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen));
    instruction->AddOperand(MakeRegOperand(inst.Right(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen));
    codeGen.Emit(instruction);
    if (mod)
    {
        cmajor::systemx::assembler::Instruction* remInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::GET);
        remInst->AddOperand(MakeRegOperand(reg));
        remInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::machine::rR));
        codeGen.Emit(remInst);
    }
}

void EmitEqual(EqualInstruction& inst, CodeGenerator& codeGen)
{
    int machineInst = -1;
    Type* type = inst.Left()->GetType();
    if (type->IsFundamentalType())
    {
        switch (type->Id())
        {
        case sbyteTypeId:
        case shortTypeId:
        case intTypeId:
        case longTypeId:
        {
            machineInst = cmajor::systemx::machine::CMP;
            break;
        }
        case boolTypeId:
        case byteTypeId:
        case ushortTypeId:
        case uintTypeId:
        case ulongTypeId:
        {
            machineInst = cmajor::systemx::machine::CMPU;
            break;
        }
        case doubleTypeId:
        {
            machineInst = cmajor::systemx::machine::FCMP;
            break;
        }
        }
    }
    else if (type->IsPointerType())
    {
        machineInst = cmajor::systemx::machine::CMPU;
    }
    else
    {
        codeGen.Error("error emitting equal: invalid type kind");
    }
    if (machineInst == -1)
    {
        codeGen.Error("error emitting equal: invalid machine instruction");
    }
    cmajor::systemx::assembler::Instruction* cmpInst = new cmajor::systemx::assembler::Instruction(machineInst);
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting equal: reg not valid");
    }
    cmpInst->AddOperand(MakeRegOperand(reg));
    cmpInst->AddOperand(MakeRegOperand(inst.Left(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen));
    cmpInst->AddOperand(MakeRegOperand(inst.Right(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen));
    codeGen.Emit(cmpInst);
    cmajor::systemx::assembler::Instruction* zszInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::ZSZ);
    zszInst->AddOperand(MakeRegOperand(reg));
    zszInst->AddOperand(MakeRegOperand(reg));
    zszInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint8_t>(1)));
    codeGen.Emit(zszInst);
}

void EmitLess(LessInstruction& inst, CodeGenerator& codeGen)
{
    int machineInst = -1;
    Type* type = inst.Left()->GetType();
    if (type->IsFundamentalType())
    {
        switch (type->Id())
        {
        case sbyteTypeId:
        case shortTypeId:
        case intTypeId:
        case longTypeId:
        {
            machineInst = cmajor::systemx::machine::CMP;
            break;
        }
        case byteTypeId:
        case ushortTypeId:
        case uintTypeId:
        case ulongTypeId:
        {
            machineInst = cmajor::systemx::machine::CMPU;
            break;
        }
        case doubleTypeId:
        {
            machineInst = cmajor::systemx::machine::FCMP;
            break;
        }
        }
    }
    else if (type->IsPointerType())
    {
        machineInst = cmajor::systemx::machine::CMPU;
    }
    else
    {
        codeGen.Error("error emitting less: invalid type kind");
    }
    if (machineInst == -1)
    {
        codeGen.Error("error emitting less: invalid machine instruction");
    }
    cmajor::systemx::assembler::Instruction* cmpInst = new cmajor::systemx::assembler::Instruction(machineInst);
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting less: reg not valid");
    }
    cmpInst->AddOperand(MakeRegOperand(reg));
    cmpInst->AddOperand(MakeRegOperand(inst.Left(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen));
    cmpInst->AddOperand(MakeRegOperand(inst.Right(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen));
    codeGen.Emit(cmpInst);
    cmajor::systemx::assembler::Instruction* zsnInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::ZSN);
    zsnInst->AddOperand(MakeRegOperand(reg));
    zsnInst->AddOperand(MakeRegOperand(reg));
    zsnInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint8_t>(1)));
    codeGen.Emit(zsnInst);
}

void EmitJmp(JmpInstruction& inst, CodeGenerator& codeGen)
{
    cmajor::systemx::assembler::Instruction* jmpInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::JMP);
    jmpInst->AddOperand(cmajor::systemx::assembler::MakeLocalSymbol(inst.TargetLabelId()));
    codeGen.Emit(jmpInst);
}

void EmitBranch(BranchInstruction& inst, CodeGenerator& codeGen)
{
    cmajor::systemx::assembler::Instruction* branchInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::BNZ);
    branchInst->AddOperand(MakeRegOperand(inst.Cond(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen));
    branchInst->AddOperand(cmajor::systemx::assembler::MakeLocalSymbol(inst.TrueTargetLabelId()));
    codeGen.Emit(branchInst);
    cmajor::systemx::assembler::Instruction* jmpInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::JMP);
    jmpInst->AddOperand(cmajor::systemx::assembler::MakeLocalSymbol(inst.FalseTargetLabelId()));
    codeGen.Emit(jmpInst);
}

void EmitNop(NoOperationInstruction& inst, CodeGenerator& codeGen)
{
    cmajor::systemx::assembler::Instruction* swymInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::SWYM);
    codeGen.Emit(swymInst);
}

void EmitElemAddr(ElemAddrInstruction& inst, CodeGenerator& codeGen)
{
    ElemAddrKind elemAddrKind = inst.GetElemAddrKind(codeGen.Ctx());
    if (elemAddrKind == ElemAddrKind::array)
    {
        cmajor::systemx::assembler::Node* indexReg = MakeRegOperand(inst.Index(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen);
        int64_t indexFactor = GetElementSize(inst.Ptr()->GetType(), codeGen);
        bool indexTypeIsUnsignedType = inst.Index()->GetType()->IsUnsignedType();
        cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        if (indexTypeIsUnsignedType)
        {
            setInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint64_t>(indexFactor)));
        }
        else
        {
            setInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(indexFactor));
        }
        codeGen.Emit(setInst);
        int machineMulInst = cmajor::systemx::machine::MUL;
        if (indexTypeIsUnsignedType)
        {
            machineMulInst = cmajor::systemx::machine::MULU;
        }
        cmajor::systemx::assembler::Instruction* mulInst = new cmajor::systemx::assembler::Instruction(machineMulInst);
        mulInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regCX)));
        mulInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        mulInst->AddOperand(indexReg);
        codeGen.Emit(mulInst);
    }
    else if (elemAddrKind == ElemAddrKind::structure)
    {
        int64_t index = GetIndex(inst.Index(), codeGen);
        int64_t offset = GetOffset(inst.Ptr()->GetType(), index, codeGen);
        cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regCX)));
        setInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(offset));
        codeGen.Emit(setInst);
    }
    else
    {
        codeGen.Error("error emitting elemaddr: invalid elemaddr kind");
    }
    cmajor::systemx::assembler::Instruction* ldaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::LDA);
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting elemaddr: reg not valid");
    }
    ldaInst->AddOperand(MakeRegOperand(reg));
    if (elemAddrKind == ElemAddrKind::array)
    {
        cmajor::systemx::assembler::Instruction* primaryLdaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::LDA);
        primaryLdaInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        EmitPtrOperand(inst.Ptr(), primaryLdaInst, codeGen);
        codeGen.Emit(primaryLdaInst);
        ldaInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
    }
    else if (elemAddrKind == ElemAddrKind::structure)
    {
        cmajor::systemx::assembler::Instruction* primaryLdaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::LDA);
        primaryLdaInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        EmitPtrOperand(inst.Ptr(), primaryLdaInst, codeGen);
        codeGen.Emit(primaryLdaInst);
        ldaInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
    }
    ldaInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regCX)));
    codeGen.Emit(ldaInst);
}

void EmitPtrOffset(PtrOffsetInstruction& inst, CodeGenerator& codeGen)
{
    cmajor::systemx::assembler::Node* offsetReg = MakeRegOperand(inst.Offset(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen);
    int64_t offsetFactor = GetPointeeSize(inst.Ptr()->GetType(), codeGen);
    bool offsetTypeIsUnsignedType = inst.Ptr()->GetType()->RemovePointer(inst.GetSourcePos(), codeGen.Ctx())->IsUnsignedType();
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
    if (offsetTypeIsUnsignedType)
    {
        setInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint64_t>(offsetFactor)));
    }
    else
    {
        setInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::machine::SignExtend(offsetFactor)));
    }
    codeGen.Emit(setInst);
    int machineMulInst = cmajor::systemx::machine::MUL;
    if (offsetTypeIsUnsignedType)
    {
        machineMulInst = cmajor::systemx::machine::MULU;
    }
    cmajor::systemx::assembler::Instruction* mulInst = new cmajor::systemx::assembler::Instruction(machineMulInst);
    mulInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regIX)));
    mulInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
    mulInst->AddOperand(offsetReg);
    codeGen.Emit(mulInst);
    cmajor::systemx::assembler::Instruction* ldaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::LDA);
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting ptroffset: reg not valid");
    }
    ldaInst->AddOperand(MakeRegOperand(reg));
    cmajor::systemx::assembler::Node* operand = MakeRegOperand(inst.Ptr(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    ldaInst->AddOperand(operand);
    ldaInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regIX)));
    codeGen.Emit(ldaInst);
}

void EmitPtrDiff(PtrDiffInstruction& inst, CodeGenerator& codeGen)
{
    int64_t scaleFactor = GetPointeeSize(inst.LeftPtr()->GetType(), codeGen);
    cmajor::systemx::assembler::Instruction* diff = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::SUB);
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting ptrdiff: reg not valid");
    }
    diff->AddOperand(MakeRegOperand(reg));
    cmajor::systemx::assembler::Node* leftOperand = MakeRegOperand(inst.LeftPtr(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    diff->AddOperand(leftOperand);
    cmajor::systemx::assembler::Node* rightOperand = MakeRegOperand(inst.RightPtr(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX), codeGen);
    diff->AddOperand(rightOperand);
    codeGen.Emit(diff);
    cmajor::systemx::assembler::Instruction* scaledDiff = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::DIV);
    scaledDiff->AddOperand(MakeRegOperand(reg));
    scaledDiff->AddOperand(MakeRegOperand(reg));
    scaledDiff->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(scaleFactor));
    codeGen.Emit(scaledDiff);
}

void EmitBitcast(BitcastInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting bitcast: reg not valid");
    }
    cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setInst->AddOperand(MakeRegOperand(reg));
    setInst->AddOperand(operandReg);
    codeGen.Emit(setInst);
}

void EmitPtrToInt(PtrToIntInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting ptrtoint: reg not valid");
    }
    cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setInst->AddOperand(MakeRegOperand(reg));
    setInst->AddOperand(operandReg);
    codeGen.Emit(setInst);
}

void EmitIntToPtr(IntToPtrInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting inttoptr: reg not valid");
    }
    cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setInst->AddOperand(MakeRegOperand(reg));
    setInst->AddOperand(operandReg);
    codeGen.Emit(setInst);
}

void EmitFloatToInt(FloatToIntInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting floattoint: reg not valid");
    }
    cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    int machineInst = -1;
    if (inst.Result()->GetType()->IsUnsignedType())
    {
        machineInst = cmajor::systemx::machine::FIXU;
    }
    else
    {
        machineInst = cmajor::systemx::machine::FIX;
    }
    cmajor::systemx::assembler::Instruction* fixInst = new cmajor::systemx::assembler::Instruction(machineInst);
    fixInst->AddOperand(MakeRegOperand(reg));
    fixInst->AddOperand(operandReg);
    codeGen.Emit(fixInst);
}

void EmitIntToFloat(IntToFloatInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting inttofloat: reg not valid");
    }
    cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    int machineInst = -1;
    if (inst.Operand()->GetType()->IsUnsignedType())
    {
        if (inst.Result()->GetType()->IsFloatType())
        {
            machineInst = cmajor::systemx::machine::SFLOTU;
        }
        else if (inst.Result()->GetType()->IsDoubleType())
        {
            machineInst = cmajor::systemx::machine::FLOTU;
        }
        else
        {
            codeGen.Error("error emitting inttofloat: invalid result type");
        }
    }
    else
    {
        if (inst.Result()->GetType()->IsFloatType())
        {
            machineInst = cmajor::systemx::machine::SFLOT;
        }
        else if (inst.Result()->GetType()->IsDoubleType())
        {
            machineInst = cmajor::systemx::machine::FLOT;
        }
        else
        {
            codeGen.Error("error emitting inttofloat: invalid result type");
        }
    }
    cmajor::systemx::assembler::Instruction* flotInst = new cmajor::systemx::assembler::Instruction(machineInst);
    flotInst->AddOperand(MakeRegOperand(reg));
    flotInst->AddOperand(operandReg);
    codeGen.Emit(flotInst);
}

void EmitTruncate(TruncateInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting truncate: reg not valid");
    }
    cmajor::systemx::assembler::Node* operandReg = MakeRegOperand(inst.Operand(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen);
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    if (inst.Operand()->GetType()->Id() == doubleTypeId && inst.Result()->GetType()->Id() == floatTypeId)
    {
        setInst->AddOperand(MakeRegOperand(reg));
        setInst->AddOperand(operandReg);
        codeGen.Emit(setInst);
        return;
    }
    setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX)));
    cmajor::systemx::assembler::Node* maskOperand = nullptr;
    switch (inst.Result()->GetType()->Id())
    {
    case boolTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0x01), true);
        break;
    }
    case byteTypeId:
    case sbyteTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0xFF), true);
        break;
    }
    case ushortTypeId:
    case shortTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0xFFFF), true);
        break;
    }
    case uintTypeId:
    case intTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0xFFFFFFFF), true);
        break;
    }
    case ulongTypeId:
    case longTypeId:
    {
        maskOperand = cmajor::systemx::assembler::MakeConstantExpr(uint64_t(0xFFFFFFFFFFFFFFFF), true);
        break;
    }
    }
    if (!maskOperand)
    {
        codeGen.Error("error emitting truncate: invalid result type");
    }
    setInst->AddOperand(maskOperand);
    codeGen.Emit(setInst);
    cmajor::systemx::assembler::Instruction* andInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::AND);
    andInst->AddOperand(MakeRegOperand(reg));
    andInst->AddOperand(operandReg);
    andInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regBX)));
    codeGen.Emit(andInst);
}

void EmitArg(ArgInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting arg: reg not valid");
    }
    bool addrEmitted = false;
    if (inst.Arg()->IsRegValue())
    {
        RegValue* regValue = static_cast<RegValue*>(inst.Arg());
        Instruction* argInst = regValue->Inst();
        if (argInst->IsLocalInstruction())
        {
            cmajor::systemx::assembler::Instruction* ldaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::LDA);
            ldaInst->AddOperand(MakeRegOperand(reg));
            FrameLocation frameLocation = GetFrameLocation(inst.Arg(), codeGen);
            EmitFrameLocationOperand(frameLocation, ldaInst, codeGen);
            codeGen.Emit(ldaInst);
            addrEmitted = true;
        }
    }
    cmajor::systemx::assembler::Instruction* stouInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::STOU);
    if (addrEmitted)
    {
        stouInst->AddOperand(MakeRegOperand(reg));
    }
    else
    {
        stouInst->AddOperand(MakeRegOperand(inst.Arg(), reg, codeGen));
    }
    EmitArgLocationOperand(stouInst, codeGen);
    codeGen.Emit(stouInst);
}

void EmitProcedureCall(ProcedureCallInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting procedure call: reg not valid");
    }
    uint8_t saveNumLocals = 0;
    int lastActiveLocalReg = codeGen.RegAllocator()->LastActiveLocalReg();
    if (lastActiveLocalReg != -1)
    {
        saveNumLocals = static_cast<uint8_t>(lastActiveLocalReg + 1);
    }
    cmajor::systemx::assembler::Instruction* callInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::CALL);
    Frame& frame = codeGen.RegAllocator()->GetFrame();
    frame.CurrentCallFrame()->SetSaveNumLocals(saveNumLocals);
    callInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(saveNumLocals));
    callInst->AddOperand(MakeCalleeOperand(inst.Callee(), reg, codeGen));
    codeGen.Emit(callInst);
    frame.AddCallFrame();
}

void EmitFunctionCall(FunctionCallInstruction& inst, CodeGenerator& codeGen)
{
    Register reg = codeGen.RegAllocator()->GetRegister(&inst);
    if (!reg.Valid())
    {
        codeGen.Error("error emitting function call: reg not valid");
    }
    uint8_t saveNumLocals = 0;
    int lastActiveLocalReg = codeGen.RegAllocator()->LastActiveLocalReg();
    if (lastActiveLocalReg != -1)
    {
        saveNumLocals = lastActiveLocalReg + 1;
    }
    cmajor::systemx::assembler::Instruction* callInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::CALL);
    Frame& frame = codeGen.RegAllocator()->GetFrame();
    frame.CurrentCallFrame()->SetSaveNumLocals(saveNumLocals);
    callInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(saveNumLocals));
    callInst->AddOperand(MakeCalleeOperand(inst.Callee(), reg, codeGen));
    codeGen.Emit(callInst);
    cmajor::systemx::assembler::Instruction* setRetValInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setRetValInst->AddOperand(MakeRegOperand(reg));
    setRetValInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
    codeGen.Emit(setRetValInst);
    frame.AddCallFrame();
}

void EmitRet(RetInstruction& inst, CodeGenerator& codeGen)
{
    if (inst.ReturnValue())
    {
        cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX)));
        setInst->AddOperand(MakeRegOperand(inst.ReturnValue(), GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX), codeGen));
        codeGen.Emit(setInst);
    }
    int targetLabelId = codeGen.ExitLabelId();
    cmajor::systemx::assembler::Instruction* jmpInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::JMP);
    jmpInst->AddOperand(cmajor::systemx::assembler::MakeLocalSymbol(targetLabelId));
    codeGen.Emit(jmpInst);
}

void EmitTrap(TrapInstruction& inst, CodeGenerator& codeGen)
{
    int n = inst.Args().size();
    if (n > 4)
    {
        codeGen.Error("error emitting trap: too many arguments");
    }
    for (int i = 0; i < n; ++i)
    {
        Value* arg = inst.Args()[i];
        cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        uint8_t reg = cmajor::systemx::machine::regAX - i;
        setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), reg)));
        setInst->AddOperand(MakeRegOperand(arg, GetGlobalRegister(codeGen.Ctx(), reg), codeGen));
        codeGen.Emit(setInst);
    }
    cmajor::systemx::assembler::Instruction* trapInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::TRAP);
    trapInst->AddOperand(MakeTrapOperand(inst.Op1(), codeGen));
    trapInst->AddOperand(MakeTrapOperand(inst.Op2(), codeGen));
    trapInst->AddOperand(MakeTrapOperand(inst.Op3(), codeGen));
    codeGen.Emit(trapInst);
    Frame& frame = codeGen.RegAllocator()->GetFrame();
    frame.AddCallFrame();
    codeGen.RegAllocator()->AddRegisterLocation(&inst, GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regAX));
}

void EmitPrologue(CodeGenerator& codeGen)
{
    cmajor::systemx::assembler::Instruction* stoInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::STO);
    stoInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regFP)));
    stoInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regSP)));
    codeGen.Emit(stoInst);
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regFP)));
    setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regSP)));
    codeGen.Emit(setInst);
    Frame& frame = codeGen.RegAllocator()->GetFrame();
    if (frame.IsWithinWydeRange())
    {
        cmajor::systemx::assembler::Instruction* inclInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::INCL);
        inclInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regSP)));
        inclInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint16_t>(frame.Size())));
        codeGen.Emit(inclInst);
    }
    else
    {
        cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
        setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regIX)));
        setInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(static_cast<uint64_t>(frame.Size())));
        codeGen.Emit(setInst);
        cmajor::systemx::assembler::Instruction* addInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::ADD);
        addInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regSP)));
        addInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regSP)));
        addInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regIX)));
        codeGen.Emit(addInst);
    }
    frame.ResolveCallFrames();
}

void EmitEpilogue(CodeGenerator& codeGen)
{
    cmajor::systemx::assembler::Instruction* setInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::SET);
    setInst->SetLabel(cmajor::systemx::assembler::MakeLocalSymbol(codeGen.ExitLabelId()));
    setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regSP)));
    setInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regFP)));
    codeGen.Emit(setInst);
    cmajor::systemx::assembler::Instruction* ldoInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::LDO);
    ldoInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regFP)));
    ldoInst->AddOperand(MakeRegOperand(GetGlobalRegister(codeGen.Ctx(), cmajor::systemx::machine::regSP)));
    codeGen.Emit(ldoInst);
    cmajor::systemx::assembler::Instruction* retInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::machine::RET);
    codeGen.Emit(retInst);
}

void EmitBool(BoolValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitByte(static_cast<uint8_t>(value.GetValue()));
}

void EmitSByte(SByteValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitByte(static_cast<uint8_t>(value.GetValue()));
}

void EmitByte(ByteValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitByte(value.GetValue());
}

void EmitShort(ShortValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitWyde(static_cast<uint16_t>(value.GetValue()));
}

void EmitUShort(UShortValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitWyde(value.GetValue());
}

void EmitInt(IntValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitTetra(static_cast<uint32_t>(value.GetValue()));
}

void EmitUInt(UIntValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitTetra(value.GetValue());
}

void EmitLong(LongValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitOcta(static_cast<uint64_t>(value.GetValue()));
}

void EmitULong(ULongValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitOcta(value.GetValue());
}

void EmitFloat(FloatValue& value, CodeGenerator& codeGen)
{
    uint32_t v = *static_cast<uint32_t*>(static_cast<void*>(&value));
    codeGen.EmitTetra(v);
}

void EmitDouble(DoubleValue& value, CodeGenerator& codeGen)
{
    uint64_t v = *static_cast<uint64_t*>(static_cast<void*>(&value));
    codeGen.EmitOcta(v);
}

void EmitNull(CodeGenerator& codeGen)
{
    codeGen.EmitOcta(0);
}

void EmitAddress(AddressValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitSymbol(value.Value()->Name());
}

void EmitSymbol(SymbolValue& value, CodeGenerator& codeGen)
{
    codeGen.EmitSymbol(value.Symbol());
}

void EmitString(StringValue& value, CodeGenerator& codeGen)
{
    int state = 0;
    std::string hex;
    for (char c : value.Value())
    {
        switch (state)
        {
        case 0:
        {
            if (c == '\\')
            {
                hex.clear();
                state = 1;
            }
            else
            {
                codeGen.EmitByte(static_cast<uint8_t>(c));
            }
            break;
        }
        case 1:
        {
            hex.append(1, c);
            state = 2;
            break;
        }
        case 2:
        {
            hex.append(1, c);
            codeGen.EmitByte(util::ParseHexByte(hex));
            state = 0;
            break;
        }
        }
    }
    if (state == 1)
    {
        codeGen.Error("error emitting string: two hex characters expected");
    }
    else if (state == 2)
    {
        codeGen.Error("error emitting string: one hex character expected");
    }
}

void EmitClsId(const std::string& typeId, CodeGenerator& codeGen)
{
    codeGen.EmitClsId(typeId);
}

void EmitSourceFileNameDebugInfo(const std::string& sourceFileName, int64_t id, CodeGenerator& codeGen)
{
    codeGen.EmitDebugInfoInst(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BSPEC));
    cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::assembler::FILEINFO));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(sourceFileName));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(id));
    codeGen.EmitDebugInfoInst(octaInst);
    codeGen.EmitDebugInfoInst(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::ESPEC));
}

void EmitFunctionDebugInfo(Function* function, int64_t frameSize, CodeGenerator& codeGen)
{
    codeGen.EmitDebugInfoInst(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BSPEC));
    cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::assembler::FUNCINFO));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeGlobalSymbol(function->Name()));
    std::string functionFullName;
    MetadataRef* metadataRef = function->GetMetadataRef();
    if (metadataRef)
    {
        MetadataStruct* metadataStruct = metadataRef->GetMetadataStruct();
        if (metadataStruct)
        {
            MetadataItem* fullNameItem = metadataStruct->GetItem("fullName");
            if (fullNameItem && fullNameItem->Kind() == MetadataItemKind::metadataString)
            {
                MetadataString* fullNameStr = static_cast<MetadataString*>(fullNameItem);
                functionFullName = fullNameStr->Value();
            }
        }
    }
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(functionFullName));
    if (metadataRef)
    {
        MetadataStruct* metadataStruct = metadataRef->GetMetadataStruct();
        if (metadataStruct)
        {
            MetadataItem* sourceFileItem = metadataStruct->GetItem("sourceFile");
            if (sourceFileItem && sourceFileItem->Kind() == MetadataItemKind::metadataRef)
            {
                MetadataRef* mdRef = static_cast<MetadataRef*>(sourceFileItem);
                MetadataStruct* mdStruct = mdRef->GetMetadataStruct();
                if (mdStruct)
                {
                    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(mdStruct->Id()));
                }
            }
        }
    }
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(frameSize));
    codeGen.EmitDebugInfoInst(octaInst);
    codeGen.EmitDebugInfoInst(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::ESPEC));
}

void EmitLineNumberInfo(uint32_t currentLineNumber, CodeGenerator& codeGen)
{
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BSPEC));
    cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::assembler::LINEINFO));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(currentLineNumber));
    codeGen.Emit(octaInst);
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::ESPEC));
}

void EmitBeginTry(uint32_t tryBlockId, uint32_t parentTryBlockId, CodeGenerator& codeGen)
{
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BSPEC));
    cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::assembler::BEGINTRY));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(tryBlockId));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(parentTryBlockId));
    codeGen.Emit(octaInst);
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::ESPEC));
}

void EmitEndTry(uint32_t tryBlockId, CodeGenerator& codeGen)
{
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BSPEC));
    cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::assembler::ENDTRY));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(tryBlockId));
    codeGen.Emit(octaInst);
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::ESPEC));
}

void EmitCatch(uint32_t catchBlockId, uint32_t tryBlockId, uint64_t caughtTypeId1, uint64_t caughtTypeId2, CodeGenerator& codeGen)
{
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BSPEC));
    cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::assembler::CATCH));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(catchBlockId));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(tryBlockId));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(caughtTypeId1));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(caughtTypeId2));
    codeGen.Emit(octaInst);
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::ESPEC));
}

void EmitBeginCleanup(uint32_t cleanupBlockId, uint32_t tryBlockId, CodeGenerator& codeGen)
{
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BSPEC));
    cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::assembler::BEGINCLEANUP));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cleanupBlockId));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(tryBlockId));
    codeGen.Emit(octaInst);
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::ESPEC));
}

void EmitEndCleanup(uint32_t cleanupBlockId, CodeGenerator& codeGen)
{
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BSPEC));
    cmajor::systemx::assembler::Instruction* octaInst = new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cmajor::systemx::assembler::ENDCLEANUP));
    octaInst->AddOperand(cmajor::systemx::assembler::MakeConstantExpr(cleanupBlockId));
    codeGen.Emit(octaInst);
    codeGen.Emit(new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::ESPEC));
}

void ProcessInstructionMetadata(Instruction* inst, CodeGenerator& codeGen)
{
    MetadataRef* mdRef = inst->GetMetadataRef();
    if (mdRef)
    {
        MetadataStruct* mdStruct = mdRef->GetMetadataStruct();
        if (mdStruct)
        {
            MetadataItem* mdItem = mdStruct->GetItem("nodeType");
            if (mdItem->Kind() == MetadataItemKind::metadataLong)
            {
                MetadataLong* mdNodeTypeLong = static_cast<MetadataLong*>(mdItem);
                int64_t nodeType = mdNodeTypeLong->Value();
                switch (nodeType)
                {
                case cmajor::systemx::assembler::LINEINFO:
                {
                    MetadataItem* mdLineItem = mdStruct->GetItem("line");
                    if (mdLineItem && mdLineItem->Kind() == MetadataItemKind::metadataLong)
                    {
                        MetadataLong* mdLineLong = static_cast<MetadataLong*>(mdLineItem);
                        int64_t lineNumber = mdLineLong->Value();
                        codeGen.SetCurrentLineNumber(static_cast<uint32_t>(lineNumber));
                    }
                    break;
                }
                case cmajor::systemx::assembler::BEGINTRY:
                {
                    int64_t tryBlockId = -1;
                    int64_t parentTryBlockId = -1;
                    MetadataItem* tryBlockIdItem = mdStruct->GetItem("tryBlockId");
                    if (tryBlockIdItem && tryBlockIdItem->Kind() == MetadataItemKind::metadataLong)
                    {
                        MetadataLong* mdTryBlockIdLong = static_cast<MetadataLong*>(tryBlockIdItem);
                        tryBlockId = mdTryBlockIdLong->Value();
                    }
                    MetadataItem* parentTryBlockIdItem = mdStruct->GetItem("parentTryBlockId");
                    if (parentTryBlockIdItem && parentTryBlockIdItem->Kind() == MetadataItemKind::metadataLong)
                    {
                        MetadataLong* mdParentTryBlockIdLong = static_cast<MetadataLong*>(parentTryBlockIdItem);
                        parentTryBlockId = mdParentTryBlockIdLong->Value();
                    }
                    codeGen.BeginTry(static_cast<uint32_t>(tryBlockId), static_cast<uint32_t>(parentTryBlockId));
                    break;
                }
                case cmajor::systemx::assembler::ENDTRY:
                {
                    int64_t tryBlockId = -1;
                    MetadataItem* tryBlockIdItem = mdStruct->GetItem("tryBlockId");
                    if (tryBlockIdItem && tryBlockIdItem->Kind() == MetadataItemKind::metadataLong)
                    {
                        MetadataLong* mdTryBlockIdLong = static_cast<MetadataLong*>(tryBlockIdItem);
                        tryBlockId = mdTryBlockIdLong->Value();
                    }
                    codeGen.EndTry(static_cast<uint32_t>(tryBlockId));
                    break;
                }
                case cmajor::systemx::assembler::CATCH:
                {
                    int64_t catchBlockId = -1;
                    MetadataItem* catchBlockIdItem = mdStruct->GetItem("catchBlockId");
                    if (catchBlockIdItem && catchBlockIdItem->Kind() == MetadataItemKind::metadataLong)
                    {
                        MetadataLong* mdCatchBlockIdLong = static_cast<MetadataLong*>(catchBlockIdItem);
                        catchBlockId = mdCatchBlockIdLong->Value();
                    }
                    int64_t tryBlockId = -1;
                    MetadataItem* tryBlockIdItem = mdStruct->GetItem("tryBlockId");
                    if (tryBlockIdItem && tryBlockIdItem->Kind() == MetadataItemKind::metadataLong)
                    {
                        MetadataLong* mdTryBlockIdLong = static_cast<MetadataLong*>(tryBlockIdItem);
                        tryBlockId = mdTryBlockIdLong->Value();
                    }
                    std::string caughtTypeIdStr;
                    MetadataItem* caughtTypeIdItem = mdStruct->GetItem("caughtTypeId");
                    if (caughtTypeIdItem && caughtTypeIdItem->Kind() == MetadataItemKind::metadataString)
                    {
                        MetadataString* mdCaughtTypeIdString = static_cast<MetadataString*>(caughtTypeIdItem);
                        caughtTypeIdStr = mdCaughtTypeIdString->Value();
                    }
                    codeGen.Catch(static_cast<uint32_t>(catchBlockId), static_cast<uint32_t>(tryBlockId), caughtTypeIdStr);
                    break;
                }
                case cmajor::systemx::assembler::BEGINCLEANUP:
                {
                    int64_t cleanupBlockId = -1;
                    int64_t tryBlockId = -1;
                    MetadataItem* cleanupBlockIdItem = mdStruct->GetItem("cleanupBlockId");
                    if (cleanupBlockIdItem && cleanupBlockIdItem->Kind() == MetadataItemKind::metadataLong)
                    {
                        MetadataLong* mdCleanupBlockIdLong = static_cast<MetadataLong*>(cleanupBlockIdItem);
                        cleanupBlockId = mdCleanupBlockIdLong->Value();
                    }
                    MetadataItem* tryBlockIdItem = mdStruct->GetItem("tryBlockId");
                    if (tryBlockIdItem && tryBlockIdItem->Kind() == MetadataItemKind::metadataLong)
                    {
                        MetadataLong* mdTryBlockIdLong = static_cast<MetadataLong*>(tryBlockIdItem);
                        tryBlockId = mdTryBlockIdLong->Value();
                    }
                    codeGen.BeginCleanup(static_cast<uint32_t>(cleanupBlockId), static_cast<uint32_t>(tryBlockId));
                    break;
                }
                case cmajor::systemx::assembler::ENDCLEANUP:
                {
                    int64_t cleanupBlockId = -1;
                    MetadataItem* cleanupBlockIdItem = mdStruct->GetItem("cleanupBlockId");
                    if (cleanupBlockIdItem && cleanupBlockIdItem->Kind() == MetadataItemKind::metadataLong)
                    {
                        MetadataLong* mdCleanupBlockIdLong = static_cast<MetadataLong*>(cleanupBlockIdItem);
                        cleanupBlockId = mdCleanupBlockIdLong->Value();
                    }
                    codeGen.EndCleanup(static_cast<uint32_t>(cleanupBlockId));
                    break;
                }
                }
            }
        }
    }
}

} // cmajor::systemx::intermediate
