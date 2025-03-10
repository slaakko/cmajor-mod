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
    CWD, DIV, DIVSD, DIVSS, IDIV, IMUL, JAE, JE, JMP, JNZ, JNE, JZ, LEA, MOV, MOVSD, MOVSS, MOVSX, MOVSXD, MOVZX, MUL, MULSD, MULSS,
    NEG, NOP, NOT, OR, POP, PUSH, RET, SAR, SETC, SETE, SETL, SHL, SHR, SUB, SUBSD, SUBSS, UCOMISD, UCOMISS, XOR
};

const uint8_t ADDRSIZE_16BIT_PREFIX = 0x66u;

const uint8_t OPCODE_PUSH_REG64 = 0x50u; // REX.B +rq
const uint8_t OPCODE_POP_REG64 = 0x58u; // REX.B +rq

const uint8_t OPCODE_RET_0 = 0xC3u;
const uint8_t OPCODE_RET_IMM16 = 0xC2u;

const uint8_t OPCODE_ADD_REG64_REG64 = 0x03u; // REX.W, REX.B /r
const uint8_t OPCODE_ADD_REG32_REG32 = 0x03u; // /r
const uint8_t OPCODE_ADD_REG16_REG16 = 0x03u; // 0x66 /r
const uint8_t OPCODE_ADD_REG8_REG8 = 0x02u; // REX.R, REX.B /r

const uint8_t SUB_5 = 5u;
const uint8_t OPCODE_SUB_REG8_IMM8 = 0x80u;
const uint8_t OPCODE_SUB_REG64_IMM32 = 0x81u; // REX.W, REX.B /5 id
const uint8_t OPCODE_SUB_REG64_REG64 = 0x2Bu; // REX.W, REX.B /r
const uint8_t OPCODE_SUB_REG32_IMM32 = 0x81u; // REX.W, REX.B /5 id
const uint8_t OPCODE_SUB_REG32_REG32 = 0x2Bu; // /r
const uint8_t OPCODE_SUB_REG16_IMM16 = 0x81u; // REX.W, REX.B /5 id
const uint8_t OPCODE_SUB_REG16_REG16 = 0x2Bu; // 0x66 /r
const uint8_t OPCODE_SUB_REG8_REG8 = 0x2Au; // REX.R, REX.B /r

const uint8_t MUL_4 = 4;
const uint8_t OPCODE_MUL_REG64 = 0xF7u; // REX.W /4
const uint8_t OPCODE_MUL_REG32 = 0xF7u; // /4
const uint8_t OPCODE_MUL_REG16 = 0xF7u; // 0x66 /4
const uint8_t OPCODE_MUL_REG8 = 0xF6u; // /4

const uint8_t IMUL_5 = 5;
const uint8_t OPCODE_IMUL_REG64 = 0xF7u; // REX.W /5
const uint8_t OPCODE_IMUL_REG32 = 0xF7u; // /5
const uint8_t OPCODE_IMUL_REG16 = 0xF7u; // 0x66 /5
const uint8_t OPCODE_IMUL_REG8 = 0xF6u; // /5

const uint8_t DIV_6 = 6;
const uint8_t OPCODE_DIV_REG64 = 0xF7u; // REX.w /6
const uint8_t OPCODE_DIV_REG32 = 0xF7u; // /6
const uint8_t OPCODE_DIV_REG16 = 0xF7u; // 0x66 /6
const uint8_t OPCODE_DIV_REG8 = 0xF6u; // /6

const uint8_t IDIV_7 = 7;
const uint8_t OPCODE_IDIV_REG64 = 0xF7u; // REX.w /7
const uint8_t OPCODE_IDIV_REG32 = 0xF7u; // /7
const uint8_t OPCODE_IDIV_REG16 = 0xF7u; // /7
const uint8_t OPCODE_IDIV_REG8 = 0xF6u; // /7

const uint8_t OPCODE_LEA_REG64 = 0x8Du;
const uint8_t SET_SCALE_INDEX_ZERO = 0x4;
const uint8_t SIB = 0x4;
const uint8_t SIB_PLUS_DISP = 0x4;
const uint8_t DISP32 = 0x5;

const uint8_t MOV_0 = 0u;
const uint8_t OPCODE_MOVREG8_IMM8 = 0x0B0; // +rb ib
const uint8_t OPCODE_MOV_REG16_IMM16 = 0xB8u; // +rw iw
const uint8_t OPCODE_MOV_REG32_IMM32 = 0xB8u; // +rd id
const uint8_t OPCODE_MOV_REG64_IMM32 = 0xC7u; // REX.W, REX.B /0 id
const uint8_t OPCODE_MOV_REG64_IMM64 = 0xB8u; // REX.W, REX.B +rq iq
const uint8_t OPCODE_MOV_REG64_REG_MEM64 = 0x8Bu; // REX.W, REX.B /r
const uint8_t OPCODE_MOV_REG32_REG_MEM32 = 0x8Bu; // REX.W, REX.B /r
const uint8_t OPCODE_MOV_REG16_REG_MEM16 = 0x8Bu; // 0x66u, /r
const uint8_t OPCODE_MOV_REGMEM64_REG64 = 0x89u; // /r
const uint8_t OPCODE_MOV_REGMEM32_REG32 = 0x89u; // /r
const uint8_t OPCODE_MOV_REGMEM16_REG16 = 0x89u; // /r
const uint8_t OPCODE_MOV_REGMEM16_REG8 = 0x89u; // 0x66u, REX.B, /r
const uint8_t OPCODE_MOV_REG8_REGMEM8 = 0x8Au; // /r 
const uint8_t OPCODE_MOV_REGMEM8_REG8 = 0x88u; // r
const uint8_t OPCODE_MOV_REG16_REGMEM16 = 0x8Bu; //  0x66u (REX.B?) /r

const uint8_t OPCODE_MOVSX_REG64_REG8_0 = 0x0Fu; // REX.W, REX.B /r
const uint8_t OPCODE_MOVSX_REG64_REG8_1 = 0xBEu; // REX.W, REX.B /r
const uint8_t OPCODE_MOVSX_REG64_REG16_0 = 0x0Fu; // REX.W, REX.B /r
const uint8_t OPCODE_MOVSX_REG64_REG16_1 = 0xBFu; // REX.W, REX.B /r
const uint8_t OPCODE_MOVSXD_REG64_REG32 = 0x63u; // REX.W, REX.B /r
const uint8_t OPCODE_MOVSXD_REG64_SIB = 0x63u; // REX.W, REX.B /r
const uint8_t OPCODE_MOVSX_REG32_REG8_0 = 0x0Fu; // /r
const uint8_t OPCODE_MOVSX_REG32_REG8_1 = 0xBEu; // /r
const uint8_t OPCODE_MOVSX_REG32_REG16_0 = 0x0Fu; // /r
const uint8_t OPCODE_MOVSX_REG32_REG16_1 = 0xBFu; // /r
const uint8_t OPCODE_MOVSX_REG16_REG8_0 = 0x0Fu;
const uint8_t OPCODE_MOVSX_REG16_REG8_1 = 0xBEu;

const uint8_t OPCODE_MOVZX_REG64_REG8_0 = 0x0Fu; // REX.W /r
const uint8_t OPCODE_MOVZX_REG64_REG8_1 = 0xB6u; // REX.W /r
const uint8_t OPCODE_MOVZX_REG64_REG16_0 = 0x0Fu; // REX.W /r
const uint8_t OPCODE_MOVZX_REG64_REG16_1 = 0xB7u; // REX.W /r
const uint8_t OPCODE_MOVZX_REG32_REG8_0 = 0x0Fu; // /r
const uint8_t OPCODE_MOVZX_REG32_REG8_1 = 0xB6u; // /r
const uint8_t OPCODE_MOVZX_REG32_REG16_0 = 0x0Fu; // /r 
const uint8_t OPCODE_MOVZX_REG32_REG16_1 = 0xB7u; // /r 
const uint8_t OPCODE_MOVZX_REG16_REG8_0 = 0x0Fu; // 0x66 /r
const uint8_t OPCODE_MOVZX_REG16_REG8_1 = 0xB6u; // 0x66 /r

const uint8_t OPCODE_MOVSD_XMM_MEM64_0 = 0xF2u; // /r
const uint8_t OPCODE_MOVSD_XMM_MEM64_1 = 0x0Fu; // /r
const uint8_t OPCODE_MOVSD_XMM_MEM64_2 = 0x10u; // /r
const uint8_t OPCODE_MOVSD_MEM64_XMM_0 = 0xF2u; // /r
const uint8_t OPCODE_MOVSD_MEM64_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_MOVSD_MEM64_XMM_2 = 0x11u; // /r

const uint8_t OPCODE_MOVSS_XMM_MEM64_0 = 0xF3u; // /r
const uint8_t OPCODE_MOVSS_XMM_MEM64_1 = 0x0Fu; // /r
const uint8_t OPCODE_MOVSS_XMM_MEM64_2 = 0x10u; // /r
const uint8_t OPCODE_MOVSS_XMM_XMM_0 = 0xF3u; // /r
const uint8_t OPCODE_MOVSS_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_MOVSS_XMM_XMM_2 = 0x10u; // /r
const uint8_t OPCODE_MOVSS_MEM64_XMM_0 = 0xF3u; // /r
const uint8_t OPCODE_MOVSS_MEM64_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_MOVSS_MEM64_XMM_2 = 0x11u; // /r
const uint8_t OPCODE_ADDSS_XMM_XMM_0 = 0xF3u; // /r
const uint8_t OPCODE_ADDSS_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_ADDSS_XMM_XMM_2 = 0x58u; // /r
const uint8_t OPCODE_SUBSS_XMM_XMM_0 = 0xF3u; // /r
const uint8_t OPCODE_SUBSS_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_SUBSS_XMM_XMM_2 = 0x5Cu; // /r
const uint8_t OPCODE_MULSS_XMM_XMM_0 = 0xF3u; // /r
const uint8_t OPCODE_MULSS_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_MULSS_XMM_XMM_2 = 0x59u; // /r
const uint8_t OPCODE_DIVSS_XMM_XMM_0 = 0xF3u; // /r
const uint8_t OPCODE_DIVSS_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_DIVSS_XMM_XMM_2 = 0x5Eu; // /r

const uint8_t OPCODE_MOVSD_XMM_XMM_0 = 0xF2u; // /r
const uint8_t OPCODE_MOVSD_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_MOVSD_XMM_XMM_2 = 0x10u; // /r
const uint8_t OPCODE_ADDSD_XMM_XMM_0 = 0xF2u; // /r
const uint8_t OPCODE_ADDSD_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_ADDSD_XMM_XMM_2 = 0x58u; // /r
const uint8_t OPCODE_SUBSD_XMM_XMM_0 = 0xF2u; // /r
const uint8_t OPCODE_SUBSD_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_SUBSD_XMM_XMM_2 = 0x5Cu; // /r
const uint8_t OPCODE_MULSD_XMM_XMM_0 = 0xF2u; // /r
const uint8_t OPCODE_MULSD_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_MULSD_XMM_XMM_2 = 0x59u; // /r
const uint8_t OPCODE_DIVSD_XMM_XMM_0 = 0xF2u; // /r
const uint8_t OPCODE_DIVSD_XMM_XMM_1 = 0x0Fu; // /r
const uint8_t OPCODE_DIVSD_XMM_XMM_2 = 0x5Eu; // /r

const uint8_t OPCODE_CMP_REG64_REG64 = 0x3Bu; // REX.W, REX:B /r
const uint8_t OPCODE_CMP_REG32_REG32 = 0x3Bu; // /r
const uint8_t OPCODE_CMP_REG16_REG16 = 0x3Bu; // 0x66u, /r
const uint8_t OPCODE_CMP_REG8_REGMEM8 = 0x3Au; // REX.W, REX.B /r
const uint8_t CMP_7 = 7;
const uint8_t OPCODE_CMP_IMMEDIATE32 = 0x81u;
const uint8_t OPCODE_CMP_IMMEDIATE16 = 0x81u;
const uint8_t OPCODE_CMP_IMMEDIATE8 = 0x80u;
const uint8_t OPCODE_COMISD_XMM_XMM_0 = 0x66u;
const uint8_t OPCODE_COMISD_XMM_XMM_1 = 0x0Fu;
const uint8_t OPCODE_COMISD_XMM_XMM_2 = 0x2Fu;
const uint8_t OPCODE_COMISS_XMM_XMM_0 = 0x0Fu;
const uint8_t OPCODE_COMISS_XMM_XMM_1 = 0x2Fu;

const uint8_t OPCODE_UCOMISD_XMM_XMM_0 = 0x66u;
const uint8_t OPCODE_UCOMISD_XMM_XMM_1 = 0x0Fu;
const uint8_t OPCODE_UCOMISD_XMM_XMM_2 = 0x2Eu;
const uint8_t OPCODE_UCOMISS_XMM_XMM_0 = 0x0Fu;
const uint8_t OPCODE_UCOMISS_XMM_XMM_1 = 0x2Eu;

const uint8_t CALL_2 = 2;
const uint8_t OPCODE_CALL_NEAR = 0xE8u;
const uint8_t OPCODE_CALL_REG64 = 0xFFu;

const uint8_t JMP_4 = 4;
const uint8_t OPCODE_JMP_OFFSET32 = 0xE9u;
const uint8_t OPCODE_JMP_REG64 = 0xFFu;
const uint8_t OPCODE_JE_OFFSET32_0 = 0x0Fu;
const uint8_t OPCODE_JE_OFFSET32_1 = 0x84u;
const uint8_t OPCODE_JNE_OFFSET32_0 = 0x0Fu;
const uint8_t OPCODE_JNE_OFFSET32_1 = 0x85u;
const uint8_t OPCODE_JAE_OFFSET32_0 = 0x0Fu;
const uint8_t OPCODE_JAE_OFFSET32_1 = 0x83u;

const uint8_t OPCODE_SETE_REG8_0 = 0x0Fu;
const uint8_t OPCODE_SETE_REG8_1 = 0x94;
const uint8_t SETE_0 = 0x00;

const uint8_t OPCODE_SETC_REG8_0 = 0x0Fu;
const uint8_t OPCODE_SETC_REG8_1 = 0x92;
const uint8_t SETC_0 = 0x00;

const uint8_t OPCODE_SETL_REG8_0 = 0x0Fu;
const uint8_t OPCODE_SETL_REG8_1 = 0x9Cu;
const uint8_t SETL_0 = 0x00;

const uint8_t SHL_4 = 4;
const uint8_t OPCODE_SHL_REG64_CL = 0xD3u; // REX.W /4
const uint8_t OPCODE_SHL_REG32_CL = 0xD3u; // /4
const uint8_t OPCODE_SHL_REG16_CL = 0xD3u; // 0x66 /4
const uint8_t OPCODE_SHL_REG8_CL = 0xD2u; // /4

const uint8_t SAR_7 = 7;
const uint8_t OPCODE_SAR_REG64_CL = 0xD3u; // REX.W /7
const uint8_t OPCODE_SAR_REG32_CL = 0xD3u; // /7
const uint8_t OPCODE_SAR_REG16_CL = 0xD3u; // 0x66 /7
const uint8_t OPCODE_SAR_REG8_CL = 0xD2u; // /7

const uint8_t SHR_5 = 5;
const uint8_t OPCODE_SHR_REG16_IMM8 = 0xC1u; // /5 ib
const uint8_t OPCODE_SHR_REG64_CL = 0xD3u; // REX.W /5 
const uint8_t OPCODE_SHR_REG32_CL = 0xD3u; // /5 
const uint8_t OPCODE_SHR_REG16_CL = 0xD3u; // 0x66 /5 
const uint8_t OPCODE_SHR_REG8_CL = 0xD2u; // /5

const uint8_t OPCODE_AND_REG64_REG64 = 0x23u; // REX.W /r
const uint8_t OPCODE_AND_REG32_REG32 = 0x23u; // /r
const uint8_t OPCODE_AND_REG16_REG16 = 0x23u; // 0x66 /r
const uint8_t OPCODE_AND_REG8_REG8 = 0x22u; // REX.R, REX.B /r

const uint8_t OPCODE_OR_REG64_REG64 = 0x0Bu; // REX.W /r
const uint8_t OPCODE_OR_REG32_REG32 = 0x0Bu; // /r
const uint8_t OPCODE_OR_REG16_REG16 = 0x0Bu; // 0x66 /r
const uint8_t OPCODE_OR_REG8_REG8 = 0x0Au; // REX.R, REX.B /r

const uint8_t OPCODE_XOR_REG8_IMM8 = 0x80u; // REX.R /6 ib
const uint8_t XOR_6 = 6;
const uint8_t OPCODE_XOR_REG64_REG64 = 0x33u; // REX.R, REX.B /r
const uint8_t OPCODE_XOR_REG32_REG32 = 0x33u; // /r
const uint8_t OPCODE_XOR_REG16_REG16 = 0x33u; // 0x66 /r
const uint8_t OPCODE_XOR_REG8_REG8 = 0x32u; // /r

const uint8_t NEG_3 = 3;
const uint8_t OPCODE_NEG_REG64 = 0xF7u; // REX.W /3
const uint8_t OPCODE_NEG_REG32 = 0xF7u; // /3
const uint8_t OPCODE_NEG_REG16 = 0xF7u; // 0x66 /3
const uint8_t OPCODE_NEG_REG8 = 0xF6u; // /3

const uint8_t NOT_2 = 2;
const uint8_t OPCODE_NOT_REG64 = 0xF7u; // REX.W /2
const uint8_t OPCODE_NOT_REG32 = 0xF7u; // /2
const uint8_t OPCODE_NOT_REG16 = 0xF7u; // 0x66 /2
const uint8_t OPCODE_NOT_REG8 = 0xF6u; // /2

const uint8_t OPCODE_CBW = 0x98u;
const uint8_t OPCODE_CWD = 0x99u;
const uint8_t OPCODE_CDQ = 0x99u;
const uint8_t OPCODE_CQO = 0x99u;

const uint8_t OPCODE_NOP = 0x90u;

const uint8_t OPCODE_CVTSI2SD_REG64_XMM_0 = 0xF2u;
const uint8_t OPCODE_CVTSI2SD_REG64_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTSI2SD_REG64_XMM_2 = 0x2Au;
const uint8_t OPCODE_CVTSI2SD_REG32_XMM_0 = 0xF2u;
const uint8_t OPCODE_CVTSI2SD_REG32_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTSI2SD_REG32_XMM_2 = 0x2Au;
const uint8_t OPCODE_CVTTSD2SI_REG64_XMM_0 = 0xF2u;
const uint8_t OPCODE_CVTTSD2SI_REG64_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTTSD2SI_REG64_XMM_2 = 0x2Cu;
const uint8_t OPCODE_CVTTSD2SI_REG32_XMM_0 = 0xF2u;
const uint8_t OPCODE_CVTTSD2SI_REG32_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTTSD2SI_REG32_XMM_2 = 0x2Cu;

const uint8_t OPCODE_CVTSI2SS_REG64_XMM_0 = 0xF3u;
const uint8_t OPCODE_CVTSI2SS_REG64_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTSI2SS_REG64_XMM_2 = 0x2Au;
const uint8_t OPCODE_CVTSI2SS_REG32_XMM_0 = 0xF3u;
const uint8_t OPCODE_CVTSI2SS_REG32_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTSI2SS_REG32_XMM_2 = 0x2Au;
const uint8_t OPCODE_CVTTSS2SI_REG64_XMM_0 = 0xF3u;
const uint8_t OPCODE_CVTTSS2SI_REG64_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTTSS2SI_REG64_XMM_2 = 0x2Cu;
const uint8_t OPCODE_CVTTSS2SI_REG32_XMM_0 = 0xF3u;
const uint8_t OPCODE_CVTTSS2SI_REG32_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTTSS2SI_REG32_XMM_2 = 0x2Cu;

const uint8_t OPCODE_CVTSS2SD_XMM_XMM_0 = 0xF3u;
const uint8_t OPCODE_CVTSS2SD_XMM_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTSS2SD_XMM_XMM_2 = 0x5Au;
const uint8_t OPCODE_CVTSD2SS_XMM_XMM_0 = 0xF2u;
const uint8_t OPCODE_CVTSD2SS_XMM_XMM_1 = 0x0Fu;
const uint8_t OPCODE_CVTSD2SS_XMM_XMM_2 = 0x5Au;

} // namespace cmajor::sbin::machine_x64
