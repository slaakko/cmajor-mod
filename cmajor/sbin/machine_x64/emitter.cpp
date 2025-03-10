// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.machine_x64.emitter;

import cmajor.sbin.machine_x64.opcode;

namespace cmajor::sbin::machine_x64 {

Emitter::~Emitter()
{
}

void EmitReg64Inst(Emitter& emitter, uint8_t opCode, Register reg, const soul::ast::Span& span)
{
    Rex rexB = RexBBitReg64(reg, emitter, span);
    if (rexB != Rex::none)
    {
        emitter.EmitByte(static_cast<uint8_t>(Rex::rexId | rexB));
    }
    emitter.EmitByte(opCode + Reg64Value(reg, emitter, span));
}

void EmitPush(Emitter& emitter, Register reg, const soul::ast::Span& span)
{
    EmitReg64Inst(emitter, OPCODE_PUSH_REG64, reg, span);
}

void EmitPop(Emitter& emitter, Register reg, const soul::ast::Span& span)
{
    EmitReg64Inst(emitter, OPCODE_POP_REG64, reg, span);
}

void EmitRet(Emitter& emitter, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_RET_0);
}

void EmitRet(Emitter& emitter, int64_t immediate0, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_RET_IMM16);
    emitter.EmitWord(static_cast<uint16_t>(immediate0));
}

void EmitAddReg64Reg64(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_ADD_REG64_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7( reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitAddReg32Reg32(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitReg32(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_ADD_REG32_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitAddReg16Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexRBit = RexRBitReg16(reg0, emitter, span);
    Rex rexBBit = RexBBitReg16(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_ADD_REG16_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitAddReg8Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_ADD_REG8_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitSubReg64Reg64(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SUB_REG64_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitSubReg32Reg32(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitReg32(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SUB_REG32_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitSubReg16Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexRBit = RexRBitReg16(reg0, emitter, span);
    Rex rexBBit = RexBBitReg16(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SUB_REG16_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitSubReg8Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SUB_REG8_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitSubReg64Immediate(Emitter& emitter, Register reg0, int64_t immediate1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SUB_REG64_IMM32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SUB_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitDword(static_cast<uint32_t>(immediate1));
}

void EmitSubReg32Immediate(Emitter& emitter, Register reg0, int32_t immediate1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg32(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SUB_REG32_IMM32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SUB_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitDword(static_cast<uint32_t>(immediate1));
}

void EmitSubReg16Immediate(Emitter& emitter, Register reg0, int16_t immediate1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg16(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SUB_REG16_IMM16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SUB_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitWord(static_cast<uint16_t>(immediate1));
}

void EmitSubReg8Immediate(Emitter& emitter, Register reg0, int8_t immediate1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SUB_REG8_IMM8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SUB_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitByte(static_cast<uint8_t>(immediate1));
}

void EmitMulReg64(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MUL_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, MUL_4, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitMulReg32(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MUL_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, MUL_4, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitMulReg16(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MUL_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, MUL_4, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitMulReg8(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MUL_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, MUL_4, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitIMulReg64(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_IMUL_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, IMUL_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitIMulReg32(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_IMUL_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, IMUL_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitIMulReg16(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_IMUL_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, IMUL_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitIMulReg8(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_IMUL_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, IMUL_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitDivReg64(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_DIV_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, DIV_6, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitDivReg32(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_DIV_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, DIV_6, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitDivReg16(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_DIV_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, DIV_6, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitDivReg8(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_DIV_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, DIV_6, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitIDivReg64(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_IDIV_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, IDIV_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitIDivReg32(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_IDIV_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, IDIV_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitIDivReg16(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_IDIV_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, IDIV_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitIDivReg8(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_IDIV_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, IDIV_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitLeaReg64ContentDisp(Emitter& emitter, Register reg0, Register contentReg1, uint32_t disp, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_LEA_REG64);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    bool disp8Mode = false;
    uint8_t modRM = 0u;
    if (disp >= 0x00 && disp <= 0x7F)
    {
        disp8Mode = true;
        modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), rmBits);
    }
    else
    {
        modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg0), rmBits);
    }
    emitter.EmitByte(modRM);
    if (rmBits == SIB_PLUS_DISP)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
    if (disp8Mode)
    {
        emitter.EmitByte(static_cast<uint8_t>(disp));
    }
    else
    {
        emitter.EmitDword(disp);
    }
}

void EmitLeaReg64ContentRegs(Emitter& emitter, Register reg0, Register contentReg1, Register contentReg2, const soul::ast::Span& span)
{
    if (RexXBitReg64(contentReg1, emitter, span) == Rex::none && RexBBitReg64(contentReg2, emitter, span) != Rex::none)
    {
        std::swap(contentReg1, contentReg2);
    }
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | 
        Rex::w_bit | 
        RexRBitReg64(reg0, emitter, span) | 
        RexXBitReg64(contentReg1, emitter, span) | 
        RexBBitReg64(contentReg2, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_LEA_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), SIB);
    emitter.EmitByte(modRM);
    uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), RegValue7(contentReg1), RegValue7(contentReg2));
    emitter.EmitByte(sib);
}

void EmitLeaReg64ContentReg(Emitter& emitter, Register reg0, Register contentReg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_LEA_REG64);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    switch (contentReg1)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), RegValue7(contentReg1));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
}

void EmitMovReg8Imm8(Emitter& emitter, Register reg0, int8_t immediate1, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg8(reg0, emitter, span);
    if (rexBBit != Rex::none )
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVREG8_IMM8 + RegValue7(reg0));
    emitter.EmitByte(static_cast<uint8_t>(immediate1));
}

void EmitMovReg16Imm16(Emitter& emitter, Register reg0, int16_t immediate1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOV_REG16_IMM16 + RegValue7(reg0));
    emitter.EmitWord(immediate1);
}

void EmitMovReg32Imm32(Emitter& emitter, Register reg0, int32_t immediate1, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOV_REG32_IMM32 + RegValue7(reg0));
    emitter.EmitDword(static_cast<uint32_t>(immediate1));
}

void EmitMovReg32Reg32(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg32(reg0, emitter, span) | RexBBitReg32(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG32_REG_MEM32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovReg64Imm64(Emitter& emitter, Register reg0, int64_t immediate1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG64_IMM64 + RegValue7(reg0));
    emitter.EmitQword(static_cast<uint64_t>(immediate1));
}

void EmitMovReg64Reg64(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG64_REG_MEM64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovReg8Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG8_REGMEM8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovReg8ContentDisp(Emitter& emitter, Register reg0, Register contentReg1, int32_t displacement, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg0, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG8_REGMEM8);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg0), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovReg64ContentDisp(Emitter& emitter, Register reg0, Register contentReg1, int32_t displacement, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG64_REG_MEM64);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg0), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovReg64Content(Emitter& emitter, Register reg0, Register contentReg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG64_REG_MEM64);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    switch (contentReg1)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), RegValue7(contentReg1));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
}

void EmitMovRegImmediate(Emitter& emitter, Register reg0, int64_t immediate1, const soul::ast::Span& span)
{
    switch (RegKind(reg0))
    {
        case RegisterKind::quadword_reg:
        {
            EmitMovReg64Imm64(emitter, reg0, immediate1, span);
            break;
        }
        case RegisterKind::doubleword_reg:
        {
            EmitMovReg32Imm32(emitter, reg0, static_cast<int32_t>(immediate1), span);
            break;
        }
        case RegisterKind::word_reg:
        {
            EmitMovReg16Imm16(emitter, reg0, static_cast<int16_t>(immediate1), span);
            break;
        }
        case RegisterKind::byte_reg:
        {
            EmitMovReg8Imm8(emitter, reg0, static_cast<int8_t>(immediate1), span);
            break;
        }
        default:
        {
            emitter.ThrowError("MOV not implemented with these operands", span);
            break;
        }

    }
}

void EmitMovContentReg32(Emitter& emitter, Register contentReg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg1, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg0, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOV_REGMEM32_REG32);
    uint8_t rmBits = EncodeRMBits(contentReg0);
    switch (contentReg0)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), RegValue7(contentReg0));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg1), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg0));
        emitter.EmitByte(sib);
    }
}

void EmitMovContentDispReg32(Emitter& emitter, Register contentReg1, int32_t displacement, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg1, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOV_REGMEM32_REG32);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovContentReg64(Emitter& emitter, Register contentReg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg1, emitter, span) | RexBBitReg64(contentReg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REGMEM64_REG64);
    uint8_t rmBits = EncodeRMBits(contentReg0);
    switch (contentReg0)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), RegValue7(contentReg0));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg1), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg0));
        emitter.EmitByte(sib);
    }
}

void EmitMovContentDispReg64(Emitter& emitter, Register contentReg1, int32_t displacement, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg1, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REGMEM64_REG64);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovContentDispReg8(Emitter& emitter, Register contentReg1, int32_t displacement, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg1, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REGMEM8_REG8);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovContentDispReg16(Emitter& emitter, Register contentReg1, int32_t displacement, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexRBit = RexRBitReg16(reg1, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOV_REGMEM16_REG16);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovContentReg16(Emitter& emitter, Register contentReg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg16(reg1, emitter, span) | RexBBitReg64(contentReg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REGMEM16_REG16);
    uint8_t rmBits = EncodeRMBits(contentReg0);
    switch (contentReg0)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), RegValue7(contentReg0));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg1), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg0));
        emitter.EmitByte(sib);
    }
}

void EmitMovContentReg8(Emitter& emitter, Register contentReg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg1, emitter, span) | RexBBitReg64(contentReg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REGMEM8_REG8); 
    uint8_t rmBits = EncodeRMBits(contentReg0);
    switch (contentReg0)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), RegValue7(contentReg0));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg1), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg0));
        emitter.EmitByte(sib);
    }
}

void EmitMovReg8Content(Emitter& emitter, Register reg0, Register contentReg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg0, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG8_REGMEM8);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    switch (contentReg1)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), RegValue7(contentReg1));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
}

void EmitMovReg16ContentDisp(Emitter& emitter, Register reg0, Register contentReg1, int32_t displacement, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexRBit = RexRBitReg16(reg0, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOV_REG16_REG_MEM16);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg0), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovReg32ContentDisp(Emitter& emitter, Register reg0, Register contentReg1, int32_t displacement, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOV_REG32_REG_MEM32);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg0), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovReg32Content(Emitter& emitter, Register reg0, Register contentReg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg32(reg0, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG32_REG_MEM32);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    switch (contentReg1)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), RegValue7(contentReg1));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
}

void EmitMovReg16Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexRBit = RexRBitReg16(reg0, emitter, span);
    Rex rexBBit = RexBBitReg16(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOV_REG16_REGMEM16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovReg16Content(Emitter& emitter, Register reg0, Register contentReg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg16(reg0, emitter, span) | RexBBitReg64(contentReg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG16_REGMEM16);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    switch (contentReg1)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), RegValue7(contentReg1));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
}

void EmitMovSxReg64Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVSX_REG64_REG8_0);
    emitter.EmitByte(OPCODE_MOVSX_REG64_REG8_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovSxReg64Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg16(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVSX_REG64_REG16_0);
    emitter.EmitByte(OPCODE_MOVSX_REG64_REG16_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovSxdReg64Reg32(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg32(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVSXD_REG64_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovSxdReg64Sib(Emitter& emitter, Register reg0, uint8_t scale, Register indexReg, Register baseReg, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId |
        Rex::w_bit |
        RexRBitReg64(reg0, emitter, span) |
        RexXBitReg64(indexReg, emitter, span) |
        RexBBitReg64(baseReg, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVSXD_REG64_SIB);
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), SIB);
    emitter.EmitByte(modRM);
    uint8_t sib = EncodeSIBByte(EncodeScale(scale, emitter, span), RegValue7(indexReg), RegValue7(baseReg));
    emitter.EmitByte(sib);
}

void EmitMovSxReg32Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg32(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVSX_REG32_REG8_0);
    emitter.EmitByte(OPCODE_MOVSX_REG32_REG8_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovSxReg32Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitReg16(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSX_REG32_REG16_0);
    emitter.EmitByte(OPCODE_MOVSX_REG32_REG16_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovSxReg64Reg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    switch (RegKind(reg1))
    {
        case RegisterKind::byte_reg:
        {
            EmitMovSxReg64Reg8(emitter, reg0, reg1, span);
            break;
        }
        case RegisterKind::word_reg:
        {
            EmitMovSxReg64Reg16(emitter, reg0, reg1, span);
            break;
        }
    }
}

void EmitMovSxReg16Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg16(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVSX_REG16_REG8_0);
    emitter.EmitByte(OPCODE_MOVSX_REG16_REG8_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovZxReg64Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVZX_REG64_REG8_0);
    emitter.EmitByte(OPCODE_MOVZX_REG64_REG8_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovZxReg64Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg16(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVZX_REG64_REG16_0);
    emitter.EmitByte(OPCODE_MOVZX_REG64_REG16_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovZxReg16Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg16(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVZX_REG16_REG8_0);
    emitter.EmitByte(OPCODE_MOVZX_REG16_REG8_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovZxReg32Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg32(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOVZX_REG32_REG8_0);
    emitter.EmitByte(OPCODE_MOVZX_REG32_REG8_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovZxReg32Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitReg16(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVZX_REG32_REG16_0);
    emitter.EmitByte(OPCODE_MOVZX_REG32_REG16_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovSdContentDispXmmReg(Emitter& emitter, Register contentReg1, int32_t displacement, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSD_MEM64_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSD_MEM64_XMM_1);
    emitter.EmitByte(OPCODE_MOVSD_MEM64_XMM_2);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovSdXmmRegContentDisp(Emitter& emitter, Register reg0, Register contentReg1, int32_t displacement, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSD_XMM_MEM64_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSD_XMM_MEM64_1);
    emitter.EmitByte(OPCODE_MOVSD_XMM_MEM64_2);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    bool disp8Mode = false;
    uint8_t modRM = 0u;
    if (displacement == 0x00)
    {
        modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), rmBits);
    }
    else if (displacement >= -128 && displacement <= 127)
    {
        disp8Mode = true;
        modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), rmBits);
    }
    else
    {
        modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg0), rmBits);
    }
    emitter.EmitByte(modRM);
    if (rmBits == SIB_PLUS_DISP)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
    if (disp8Mode)
    {
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else if (displacement != 0x00)
    {
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovSdXmmRegContent(Emitter& emitter, Register reg0, Register contentReg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSD_XMM_MEM64_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSD_XMM_MEM64_1);
    emitter.EmitByte(OPCODE_MOVSD_XMM_MEM64_2);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    switch (contentReg1)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), RegValue7(contentReg1));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
}

void EmitMovSdContentXmmReg(Emitter& emitter, Register contentReg1, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSD_MEM64_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg1, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSD_MEM64_XMM_1);
    emitter.EmitByte(OPCODE_MOVSD_MEM64_XMM_2);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    switch (contentReg1)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), RegValue7(contentReg1));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg1), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
}

void EmitMovSdXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSD_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSD_XMM_XMM_1);
    emitter.EmitByte(OPCODE_MOVSD_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitAddSdXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_ADDSD_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_ADDSD_XMM_XMM_1);
    emitter.EmitByte(OPCODE_ADDSD_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitSubSdXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_SUBSD_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SUBSD_XMM_XMM_1);
    emitter.EmitByte(OPCODE_SUBSD_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMulSdXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MULSD_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MULSD_XMM_XMM_1);
    emitter.EmitByte(OPCODE_MULSD_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitDivSdXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_DIVSD_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_DIVSD_XMM_XMM_1);
    emitter.EmitByte(OPCODE_DIVSD_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMovSdXmmRegNear(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSD_XMM_MEM64_0);
    emitter.EmitByte(OPCODE_MOVSD_XMM_MEM64_1);
    emitter.EmitByte(OPCODE_MOVSD_XMM_MEM64_2);
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), DISP32);
    emitter.EmitByte(modRM);
}

void EmitMovSsXmmRegNear(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSS_XMM_MEM64_0);
    emitter.EmitByte(OPCODE_MOVSS_XMM_MEM64_1);
    emitter.EmitByte(OPCODE_MOVSS_XMM_MEM64_2);
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), DISP32);
    emitter.EmitByte(modRM);
}

void EmitMovSsXmmRegContentDisp(Emitter& emitter, Register reg0, Register contentReg1, int32_t displacement, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSS_XMM_MEM64_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSS_XMM_MEM64_1);
    emitter.EmitByte(OPCODE_MOVSS_XMM_MEM64_2);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    bool disp8Mode = false;
    uint8_t modRM = 0u;
    if (displacement >= -128 && displacement <= 127)
    {
        disp8Mode = true;
        modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), rmBits);
    }
    else
    {
        modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg0), rmBits);
    }
    emitter.EmitByte(modRM);
    if (rmBits == SIB_PLUS_DISP)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
    if (disp8Mode)
    {
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovSsContentDispXmmReg(Emitter& emitter, Register contentReg1, int32_t displacement, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSS_MEM64_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg1, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSS_MEM64_XMM_1);
    emitter.EmitByte(OPCODE_MOVSS_MEM64_XMM_2);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    if (displacement >= -128 && displacement <= 127)
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitByte(static_cast<uint8_t>(displacement));
    }
    else
    {
        uint8_t modRM = EncodeModRMByte(Mod::disp32Mode, RegValue7(reg1), rmBits);
        emitter.EmitByte(modRM);
        if (rmBits == SIB)
        {
            uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
            emitter.EmitByte(sib);
        }
        emitter.EmitDword(static_cast<uint32_t>(displacement));
    }
}

void EmitMovSsXmmRegContent(Emitter& emitter, Register reg0, Register contentReg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSS_XMM_MEM64_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSS_XMM_MEM64_1);
    emitter.EmitByte(OPCODE_MOVSS_XMM_MEM64_2);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    switch (contentReg1)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg0), RegValue7(contentReg1));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
}

void EmitMovSsContentXmmReg(Emitter& emitter, Register contentReg1, Register reg1, const soul::ast::Span& span) 
{
    emitter.EmitByte(OPCODE_MOVSS_MEM64_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg1, emitter, span);
    Rex rexBBit = RexBBitReg64(contentReg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSS_MEM64_XMM_1);
    emitter.EmitByte(OPCODE_MOVSS_MEM64_XMM_2);
    uint8_t rmBits = EncodeRMBits(contentReg1);
    switch (contentReg1)
    {
        case Register::rbp:
        case Register::r13:
        {
            uint8_t modRM = EncodeModRMByte(Mod::disp8Mode, RegValue7(reg1), RegValue7(contentReg1));
            emitter.EmitByte(modRM);
            emitter.EmitByte(0);
            return;
        }
    }
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg1), rmBits);
    emitter.EmitByte(modRM);
    if (rmBits == SIB)
    {
        uint8_t sib = EncodeSIBByte(EncodeScale(1u, emitter, span), SET_SCALE_INDEX_ZERO, RegValue7(contentReg1));
        emitter.EmitByte(sib);
    }
}

void EmitMovSsXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MOVSS_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MOVSS_XMM_XMM_1);
    emitter.EmitByte(OPCODE_MOVSS_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitAddSsXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_ADDSS_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_ADDSS_XMM_XMM_1);
    emitter.EmitByte(OPCODE_ADDSS_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitSubSsXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_SUBSS_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SUBSS_XMM_XMM_1);
    emitter.EmitByte(OPCODE_SUBSS_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitMulSsXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_MULSS_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_MULSS_XMM_XMM_1);
    emitter.EmitByte(OPCODE_MULSS_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitDivSsXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_DIVSS_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_DIVSS_XMM_XMM_1);
    emitter.EmitByte(OPCODE_DIVSS_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCallNear(Emitter& emitter)
{
    emitter.EmitByte(OPCODE_CALL_NEAR);
}

void EmitCallReg64(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CALL_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, CALL_2, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitLeaNear(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_LEA_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::contentMode, RegValue7(reg0), DISP32);
    emitter.EmitByte(modRM);
}

void EmitMovReg64Offset(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_MOV_REG64_IMM64 + RegValue7(reg0));
}

void EmitJmpNear(Emitter& emitter)
{
    emitter.EmitByte(OPCODE_JMP_OFFSET32);
}

void EmitJmpReg64(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_JMP_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, JMP_4, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitJeNear(Emitter& emitter)
{
    emitter.EmitByte(OPCODE_JE_OFFSET32_0);
    emitter.EmitByte(OPCODE_JE_OFFSET32_1);
}

void EmitJneNear(Emitter& emitter)
{
    emitter.EmitByte(OPCODE_JNE_OFFSET32_0);
    emitter.EmitByte(OPCODE_JNE_OFFSET32_1);
}

void EmitJaeNear(Emitter& emitter)
{
    emitter.EmitByte(OPCODE_JAE_OFFSET32_0);
    emitter.EmitByte(OPCODE_JAE_OFFSET32_1);
}

void EmitCmpReg64Reg64(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CMP_REG64_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCmpReg32Reg32(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitReg32(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_CMP_REG32_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCmpReg16Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexRBit = RexRBitReg16(reg0, emitter, span);
    Rex rexBBit = RexBBitReg16(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_CMP_REG16_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCmpReg8Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CMP_REG8_REGMEM8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCmpReg64Imm32(Emitter& emitter, Register reg0, int32_t immediate, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CMP_IMMEDIATE32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, CMP_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitDword(static_cast<uint32_t>(immediate));
}

void EmitCmpReg32Imm32(Emitter& emitter, Register reg0, int32_t immediate, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg32(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CMP_IMMEDIATE32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, CMP_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitDword(static_cast<uint32_t>(immediate));
}

void EmitCmpReg16Imm16(Emitter& emitter, Register reg0, int16_t immediate, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg16(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CMP_IMMEDIATE16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, CMP_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitWord(static_cast<uint16_t>(immediate));
}

void EmitCmpReg8Imm8(Emitter& emitter, Register reg0, int8_t immediate, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CMP_IMMEDIATE8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, CMP_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitByte(static_cast<uint8_t>(immediate));
}

void EmitComiSdXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_COMISD_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_COMISD_XMM_XMM_1);
    emitter.EmitByte(OPCODE_COMISD_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitUComiSdXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_UCOMISD_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_UCOMISD_XMM_XMM_1);
    emitter.EmitByte(OPCODE_UCOMISD_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitComiSsXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_COMISS_XMM_XMM_0);
    emitter.EmitByte(OPCODE_COMISS_XMM_XMM_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitUComiSsXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_UCOMISS_XMM_XMM_0);
    emitter.EmitByte(OPCODE_UCOMISS_XMM_XMM_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitSetEReg8(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SETE_REG8_0);
    emitter.EmitByte(OPCODE_SETE_REG8_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SETE_0, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitSetCReg8(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SETC_REG8_0);
    emitter.EmitByte(OPCODE_SETC_REG8_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SETC_0, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitSetLReg8(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SETL_REG8_0);
    emitter.EmitByte(OPCODE_SETL_REG8_1);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SETL_0, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitShlReg64Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SHL_REG64_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SHL_4, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitShlReg32Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SHL_REG32_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SHL_4, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitShlReg16Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SHL_REG16_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SHL_4, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitShlReg8Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SHL_REG8_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SHL_4, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitShrReg64Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SHR_REG64_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SHR_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitShrReg32Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SHR_REG32_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SHR_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitShrReg16Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SHR_REG16_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SHR_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitShrReg8Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SHR_REG8_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SHR_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitSarReg64Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SAR_REG64_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SAR_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitSarReg32Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SAR_REG32_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SAR_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitSarReg16Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SAR_REG16_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SAR_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitSarReg8Cl(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_SAR_REG8_CL);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SAR_7, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitShrReg16Imm8(Emitter& emitter, Register reg0, int8_t immediate, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_SHR_REG16_IMM8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, SHR_5, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitByte(static_cast<uint8_t>(immediate));
}

void EmitAndReg64Reg64(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_AND_REG64_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitAndReg32Reg32(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitReg32(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_AND_REG32_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitAndReg16Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexRBit = RexRBitReg16(reg0, emitter, span);
    Rex rexBBit = RexBBitReg16(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_AND_REG16_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitAndReg8Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_AND_REG8_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitOrReg64Reg64(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_OR_REG64_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitOrReg32Reg32(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitReg32(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_OR_REG32_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitOrReg16Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexRBit = RexRBitReg16(reg0, emitter, span);
    Rex rexBBit = RexBBitReg16(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_OR_REG16_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitOrReg8Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_OR_REG8_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitXorReg8Imm8(Emitter& emitter, Register reg0, int8_t immediate, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_XOR_REG8_IMM8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, XOR_6, RegValue7(reg0));
    emitter.EmitByte(modRM);
    emitter.EmitByte(static_cast<uint8_t>(immediate));
}

void EmitXorReg64Reg64(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitReg64(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_XOR_REG64_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitXorReg32Reg32(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitReg32(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_XOR_REG32_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitXorReg16Reg16(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexRBit = RexRBitReg16(reg0, emitter, span);
    Rex rexBBit = RexBBitReg16(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_XOR_REG16_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitXorReg8Reg8(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexRBitReg8(reg0, emitter, span) | RexBBitReg8(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_XOR_REG8_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitXorHighByteRegHighByteReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_XOR_REG8_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitNegReg64(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_NEG_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, NEG_3, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitNegReg32(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_NEG_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, NEG_3, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitNegReg16(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_NEG_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, NEG_3, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitNegReg8(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_NEG_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, NEG_3, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitNotReg64(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexBBitReg64(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_NOT_REG64);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, NOT_2, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitNotReg32(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    Rex rexBBit = RexBBitReg32(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_NOT_REG32);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, NOT_2, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitNotReg16(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    Rex rexBBit = RexBBitReg16(reg0, emitter, span);
    if (rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_NOT_REG16);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, NOT_2, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitNotReg8(Emitter& emitter, Register reg0, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | RexBBitReg8(reg0, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_NOT_REG8);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, NOT_2, RegValue7(reg0));
    emitter.EmitByte(modRM);
}

void EmitCbw(Emitter& emitter, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    emitter.EmitByte(OPCODE_CBW);
}

void EmitCwd(Emitter& emitter, const soul::ast::Span& span)
{
    emitter.EmitByte(ADDRSIZE_16BIT_PREFIX);
    emitter.EmitByte(OPCODE_CWD);
}

void EmitCdq(Emitter& emitter, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CDQ);
}

void EmitCqo(Emitter& emitter, const soul::ast::Span& span)
{
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit);
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CQO);
}

void EmitNop(Emitter& emitter, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_NOP);
}

void EmitCvtSi2SdReg64XmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTSI2SD_REG64_XMM_0);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitXmmReg(reg0, emitter, span) | RexBBitReg64(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CVTSI2SD_REG64_XMM_1);
    emitter.EmitByte(OPCODE_CVTSI2SD_REG64_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCvtSi2SdReg32XmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTSI2SD_REG32_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitReg32(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_CVTSI2SD_REG32_XMM_1);
    emitter.EmitByte(OPCODE_CVTSI2SD_REG32_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCvtTSd2SiReg64XmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTTSD2SI_REG64_XMM_0);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitXmmReg(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CVTTSD2SI_REG64_XMM_1);
    emitter.EmitByte(OPCODE_CVTTSD2SI_REG64_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCvtTSd2SiReg32XmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTTSD2SI_REG32_XMM_0);
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_CVTTSD2SI_REG32_XMM_1);
    emitter.EmitByte(OPCODE_CVTTSD2SI_REG32_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCvtSi2SsReg64XmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTSI2SS_REG64_XMM_0);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitXmmReg(reg0, emitter, span) | RexBBitReg64(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CVTSI2SS_REG64_XMM_1);
    emitter.EmitByte(OPCODE_CVTSI2SS_REG64_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCvtSi2SsReg32XmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTSI2SS_REG32_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitReg32(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_CVTSI2SS_REG32_XMM_1);
    emitter.EmitByte(OPCODE_CVTSI2SS_REG32_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCvtTSs2SiReg64XmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTTSS2SI_REG64_XMM_0);
    uint8_t rex = static_cast<uint8_t>(Rex::rexId | Rex::w_bit | RexRBitReg64(reg0, emitter, span) | RexBBitXmmReg(reg1, emitter, span));
    emitter.EmitByte(rex);
    emitter.EmitByte(OPCODE_CVTTSS2SI_REG64_XMM_1);
    emitter.EmitByte(OPCODE_CVTTSS2SI_REG64_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCvtTSs2SiReg32XmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTTSS2SI_REG32_XMM_0);
    Rex rexRBit = RexRBitReg32(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_CVTTSS2SI_REG32_XMM_1);
    emitter.EmitByte(OPCODE_CVTTSS2SI_REG32_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCvtSs2SdXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTSS2SD_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_CVTSS2SD_XMM_XMM_1);
    emitter.EmitByte(OPCODE_CVTSS2SD_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

void EmitCvtSd2SsXmmRegXmmReg(Emitter& emitter, Register reg0, Register reg1, const soul::ast::Span& span)
{
    emitter.EmitByte(OPCODE_CVTSD2SS_XMM_XMM_0);
    Rex rexRBit = RexRBitXmmReg(reg0, emitter, span);
    Rex rexBBit = RexBBitXmmReg(reg1, emitter, span);
    if (rexRBit != Rex::none || rexBBit != Rex::none)
    {
        uint8_t rex = static_cast<uint8_t>(Rex::rexId | rexRBit | rexBBit);
        emitter.EmitByte(rex);
    }
    emitter.EmitByte(OPCODE_CVTSD2SS_XMM_XMM_1);
    emitter.EmitByte(OPCODE_CVTSD2SS_XMM_XMM_2);
    uint8_t modRM = EncodeModRMByte(Mod::registerDirectMode, RegValue7(reg0), RegValue7(reg1));
    emitter.EmitByte(modRM);
}

} // namespace cmajor::sbin::machine_x64
