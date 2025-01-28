// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.object.instruction.formatter;

import cmajor.systemx.machine.registers;
import cmajor.systemx.machine.opcode;
import cmajor.systemx.machine.instruction;

namespace cmajor::systemx::object {

InstructionFormatter::InstructionFormatter(cmajor::systemx::machine::Machine& machine_) :
    Formatter(), machine(machine_), absoluteAddress(-1), setAddressMap(nullptr)
{
}

void InstructionFormatter::FormatInstruction(uint64_t address, std::map<int64_t, int64_t>* setAddressMap_, uint8_t opc, uint8_t x, uint8_t y, uint8_t z)
{
    absoluteAddress = address;
    setAddressMap = setAddressMap_;
    FormatCurrentAddress(absoluteAddress);
    FormatAssembledBytes(opc, x, y, z);
    FormatLabel(absoluteAddress);
    cmajor::systemx::machine::Instruction* inst = machine.GetInstruction(opc);
    inst->Format(*this, x, y, z);
    FormatEol();
}

void InstructionFormatter::FormatCurrentAddress(uint64_t currentAddress)
{
}

void InstructionFormatter::FormatAssembledBytes(uint8_t opc, uint8_t x, uint8_t y, uint8_t z)
{
}

void InstructionFormatter::FormatLabel(uint64_t currentAddress)
{
}

void InstructionFormatter::FormatOpCode(const std::string& opCodeName)
{
}

void InstructionFormatter::FormatByteOperand(uint8_t operand)
{
}

void InstructionFormatter::FormatRegOperand(uint8_t reg)
{
}

void InstructionFormatter::FormatComma()
{
}

void InstructionFormatter::FormatColon()
{
}

void InstructionFormatter::FormatEol()
{
}

void InstructionFormatter::FormatUndefined()
{
}

void InstructionFormatter::FormatSpecialReg(uint8_t reg)
{
}

void InstructionFormatter::FormatWydeOperand(uint16_t wyde)
{
}

void InstructionFormatter::FormatAddress(uint64_t address)
{
}

void InstructionFormatter::FormatSetAddress(uint64_t saddr)
{
}

void InstructionFormatter::FormatTrapName(uint8_t trap)
{
}

void InstructionFormatter::FormatSpace()
{
}

void InstructionFormatter::FormatChar(uint8_t x)
{
}

void InstructionFormatter::FormatString(const std::string& s)
{
}

void InstructionFormatter::Format(cmajor::systemx::machine::Trap& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatByteOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatByteOperand(z);
    FormatTrapName(y);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Swym& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldbi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldw& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldwi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldt& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldti& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldo& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldoi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldbu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldbui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldwu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldwui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldtu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldtui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldou& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
    int64_t saddr = GetSetAddress(absoluteAddress - 4);
    if (saddr >= cmajor::systemx::machine::dataSegmentBaseAddress && saddr < cmajor::systemx::machine::poolSegmentBaseAddress)
    {
        FormatColon();
        FormatSetAddress(saddr);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldoui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
    int64_t saddr = GetSetAddress(absoluteAddress - 4);
    if (saddr >= cmajor::systemx::machine::dataSegmentBaseAddress && saddr < cmajor::systemx::machine::poolSegmentBaseAddress)
    {
        FormatColon();
        FormatSetAddress(saddr);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldht& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldhti& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldsf& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ldsfi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stbi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stw& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stwi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stt& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stti& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sto& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stoi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stbu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stbui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stwu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stwui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sttu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sttui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stou& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stoui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stht& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sthti& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stsf& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stsfi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stco& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatByteOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Stcoi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Add& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Addi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sub& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Subi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Mul& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Muli& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Div& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Divi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Addu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Addui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Subu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Subui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Mulu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Mului& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Divu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Divui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::I2Addu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::I2Addui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::I4Addu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::I4Addui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::I8Addu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::I8Addui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::I16Addu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::I16Addui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Neg& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Negi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Negu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Negui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sl& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sli& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Slu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Slui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sr& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sri& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sru& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Srui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Cmp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Cmpi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Cmpu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Cmpui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csni& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csz& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Cszi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Cspi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csod& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csodi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csnn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csnni& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csnz& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csnzi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csnp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csnpi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csev& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Csevi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatRegOperand(y);
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsni& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsz& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zszi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zspi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsod& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsodi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsnn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsnni& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsnz& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsnzi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsnp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsnpi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsev& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Zsevi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::And& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Andi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Or& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ori& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
    int64_t saddr = GetSetAddress(absoluteAddress - 4);
    if (saddr >= cmajor::systemx::machine::textSegmentBaseAddress && saddr < cmajor::systemx::machine::dataSegmentBaseAddress)
    {
        FormatColon();
        FormatSetAddress(saddr);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Xor& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Xori& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Andn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Andni& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Orn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Orni& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Nand& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Nandi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Nor& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Nori& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Nxor& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Nxori& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Mux& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Muxi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sadd& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Saddi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bdif& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bdifi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Wdif& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Wdifi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Tdif& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Tdifi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Odif& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Odifi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Fadd& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Fsub& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Fmul& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Fdiv& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Frem& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Fsqrt& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Fint& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Fcmp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Feql& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Fix& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Fixu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Flot& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Floti& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Flotu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Flotui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sflot& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sfloti& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sflotu& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Sflotui& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatByteOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Seth& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 48;
    SetSetAddress(absoluteAddress, value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Setmh& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 32;
    SetSetAddress(absoluteAddress, value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Setml& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 16;
    SetSetAddress(absoluteAddress, value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Setl& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 16;
    SetSetAddress(absoluteAddress, value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Inch& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 48;
    SetSetAddress(absoluteAddress, GetSetAddress(absoluteAddress - 4) + value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Incmh& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 32;
    SetSetAddress(absoluteAddress, GetSetAddress(absoluteAddress - 4) + value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Incml& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 16;
    SetSetAddress(absoluteAddress, GetSetAddress(absoluteAddress - 4) + value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Incl& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz);
    SetSetAddress(absoluteAddress, GetSetAddress(absoluteAddress - 4) + value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Orh& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 48;
    SetSetAddress(absoluteAddress, GetSetAddress(absoluteAddress - 4) | value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ormh& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 32;
    SetSetAddress(absoluteAddress, GetSetAddress(absoluteAddress - 4) | value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Orml& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz) << 16;
    SetSetAddress(absoluteAddress, GetSetAddress(absoluteAddress - 4) | value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Orl& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    uint64_t value = static_cast<uint64_t>(yz);
    SetSetAddress(absoluteAddress, GetSetAddress(absoluteAddress - 4) | value);
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Andnh& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Andnmh& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Andnml& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Andnl& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    uint16_t yz = (static_cast<uint16_t>(y) << 8) | (static_cast<uint16_t>(z));
    FormatWydeOperand(yz);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Jmp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    uint64_t ra = ((static_cast<uint64_t>(x) << 16) | (static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    if (x == 0xFF && y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Jmpb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    uint64_t ra = ((static_cast<uint64_t>(x) << 16) | (static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    if (x == 0xFF && y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Go& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Goi& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Call& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatByteOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatRegOperand(z);
    if (y == cmajor::systemx::machine::regEX)
    {
        FormatColon();
        FormatSetAddress(GetSetAddress(absoluteAddress - 4));
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Calli& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatByteOperand(x);
    FormatComma();
    FormatRegOperand(y);
    FormatComma();
    FormatByteOperand(z);
    if (y == cmajor::systemx::machine::regEX)
    {
        FormatColon();
        FormatSetAddress(GetSetAddress(absoluteAddress - 4));
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Ret& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bnb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bz& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bzb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bpb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bod& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bodb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bnn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bnnb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bnz& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bnzb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bnp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bnpb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bev& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Bevb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbnb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbz& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbzb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbpb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbod& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbodb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbnn& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbnnb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbnz& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbnzb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbnp& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbnpb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbev& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Pbevb& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Get& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    FormatSpecialReg(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Put& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatSpecialReg(x);
    FormatComma();
    FormatRegOperand(z);
}

void InstructionFormatter::Format(cmajor::systemx::machine::Geta& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress + ra);
    }
}

void InstructionFormatter::Format(cmajor::systemx::machine::Getab& inst, uint8_t x, uint8_t y, uint8_t z)
{
    FormatOpCode(cmajor::systemx::machine::GetOpCodeName(inst.OpCode()));
    FormatRegOperand(x);
    FormatComma();
    if (y == 0xFF && z == 0xFF)
    {
        FormatUndefined();
    }
    else
    {
        uint64_t ra = ((static_cast<uint64_t>(y) << 8) | static_cast<uint64_t>(z)) << static_cast<uint64_t>(2);
        FormatAddress(absoluteAddress - ra);
    }
}

int64_t InstructionFormatter::GetSetAddress(int64_t absoluteAddress) const
{
    if (setAddressMap)
    {
        auto it = setAddressMap->find(absoluteAddress);
        if (it != setAddressMap->cend())
        {
            return it->second;
        }
    }
    return -1;
}

void InstructionFormatter::SetSetAddress(int64_t absoluteAddress, int64_t setAddress)
{
    if (setAddressMap)
    {
        (*setAddressMap)[absoluteAddress] = setAddress;
    }
}

} // cmajor::systemx::object
