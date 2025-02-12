// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.machine_x64.opcode;

import std.core;

export namespace cmajor::sbin::machine_x64 {

enum class OpCode : uint8_t
{
    ADD, ADDSD, ADDSS, AND, CALL, CBW, CDQ, CMP, COMISD, COMISS, CQO, CVTSD2SS, CVTSI2SD, CVTSI2SS, CVTSS2SD, CVTTSD2SI, CVTTSS2SI,
    CWD, DIV, DIVSD, DIVSS, IDIV, IMUL, JAE, JE, JMP, JNZ, JZ, LEA, MOV, MOVSD, MOVSS, MOVSX, MOVSXD, MOVZX, MUL, MULSD, MULSS,
    NEG, NOP, NOT, OR, POP, PUSH, RET, SAR, SETC, SETE, SETL, SHL, SHR, SUB, SUBSD, SUBSS, UCOMISD, UCOMISS, XOR
};

const uint8_t OPCODE_PUSH_REG64 = 0x50u;

} // namespace cmajor::sbin::machine_x64
