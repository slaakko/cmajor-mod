// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.machine_x64.reg;

import cmajor.sbin.machine_x64.opcode;
import soul.ast.span;
import std.core;

export namespace cmajor::sbin::machine_x64 {

enum class RegisterKind : uint8_t
{
    none = 0x00u,
    byte_reg = 0x01u,
    high_byte_reg = 0x02u,
    word_reg = 0x03u,
    doubleword_reg = 0x04u,
    quadword_reg = 0x05u,
    xmm_reg = 0x06u
};

constexpr uint8_t registerKindShift = 0x04u;

enum class RegisterValue : uint8_t
{
    rax = 0x00u, 
    rbx = 0x03u, 
    rcx = 0x01u, 
    rdx = 0x02u, 
    rsi = 0x06u,
    rdi = 0x07u,
    rbp = 0x05u,
    rsp = 0x04u,
    r8 = 0x08u,
    r9 = 0x09u,
    r10 = 0x0Au,
    r11 = 0x0Bu,
    r12 = 0x0Cu,
    r13 = 0x0Du,
    r14 = 0x0Eu,
    r15 = 0x0Fu,
    xmm0 = 0x00u,
    xmm1 = 0x01u,
    xmm2 = 0x02u,
    xmm3 = 0x03u,
    xmm4 = 0x04u,
    xmm5 = 0x05u,
    xmm6 = 0x06u,
    xmm7 = 0x07u,
    xmm8 = 0x08u,
    xmm9 = 0x09u,
    xmm10 = 0x0Au,
    xmm11 = 0x0Bu,
    xmm12 = 0x0Cu,
    xmm13 = 0x0Du,
    xmm14 = 0x0Eu,
    xmm15 = 0x0Fu
};

constexpr uint8_t EncodeReg(RegisterKind regKind, RegisterValue regValue)
{
    return (static_cast<uint8_t>(regKind) << registerKindShift) | static_cast<uint8_t>(regValue);
}

enum class Register : uint8_t
{
    none = 0,
    al = EncodeReg(RegisterKind::byte_reg, RegisterValue::rax),
    bl = EncodeReg(RegisterKind::byte_reg, RegisterValue::rbx),
    cl = EncodeReg(RegisterKind::byte_reg, RegisterValue::rcx),
    dl = EncodeReg(RegisterKind::byte_reg, RegisterValue::rdx),
    sil = EncodeReg(RegisterKind::byte_reg, RegisterValue::rsi),
    dil = EncodeReg(RegisterKind::byte_reg, RegisterValue::rdi),
    bpl = EncodeReg(RegisterKind::byte_reg, RegisterValue::rbp),
    spl = EncodeReg(RegisterKind::byte_reg, RegisterValue::rsp),
    r8b = EncodeReg(RegisterKind::byte_reg, RegisterValue::r8),
    r9b = EncodeReg(RegisterKind::byte_reg, RegisterValue::r9),
    r10b = EncodeReg(RegisterKind::byte_reg, RegisterValue::r10),
    r11b = EncodeReg(RegisterKind::byte_reg, RegisterValue::r11),
    r12b = EncodeReg(RegisterKind::byte_reg, RegisterValue::r12),
    r13b = EncodeReg(RegisterKind::byte_reg, RegisterValue::r13),
    r14b = EncodeReg(RegisterKind::byte_reg, RegisterValue::r14),
    r15b = EncodeReg(RegisterKind::byte_reg, RegisterValue::r15),
    ah = EncodeReg(RegisterKind::high_byte_reg, RegisterValue::rax),
    bh = EncodeReg(RegisterKind::high_byte_reg, RegisterValue::rbx),
    ch = EncodeReg(RegisterKind::high_byte_reg, RegisterValue::rcx),
    dh = EncodeReg(RegisterKind::high_byte_reg, RegisterValue::rdx),
    ax = EncodeReg(RegisterKind::word_reg, RegisterValue::rax),
    bx = EncodeReg(RegisterKind::word_reg, RegisterValue::rbx),
    cx = EncodeReg(RegisterKind::word_reg, RegisterValue::rcx),
    dx = EncodeReg(RegisterKind::word_reg, RegisterValue::rdx),
    si = EncodeReg(RegisterKind::word_reg, RegisterValue::rsi),
    di = EncodeReg(RegisterKind::word_reg, RegisterValue::rdi),
    bp = EncodeReg(RegisterKind::word_reg, RegisterValue::rbp),
    sp = EncodeReg(RegisterKind::word_reg, RegisterValue::rsp),
    r8w = EncodeReg(RegisterKind::word_reg, RegisterValue::r8),
    r9w = EncodeReg(RegisterKind::word_reg, RegisterValue::r9),
    r10w = EncodeReg(RegisterKind::word_reg, RegisterValue::r10),
    r11w = EncodeReg(RegisterKind::word_reg, RegisterValue::r11),
    r12w = EncodeReg(RegisterKind::word_reg, RegisterValue::r12),
    r13w = EncodeReg(RegisterKind::word_reg, RegisterValue::r13),
    r14w = EncodeReg(RegisterKind::word_reg, RegisterValue::r14),
    r15w = EncodeReg(RegisterKind::word_reg, RegisterValue::r15),
    eax = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::rax),
    ebx = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::rbx),
    ecx = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::rcx),
    edx = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::rdx),
    esi = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::rsi),
    edi = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::rdi),
    ebp = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::rbp),
    esp = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::rsp),
    r8d = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::r8),
    r9d = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::r9),
    r10d = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::r10),
    r11d = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::r11),
    r12d = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::r12),
    r13d = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::r13),
    r14d = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::r14),
    r15d = EncodeReg(RegisterKind::doubleword_reg, RegisterValue::r15),
    rax = EncodeReg(RegisterKind::quadword_reg, RegisterValue::rax),
    rbx = EncodeReg(RegisterKind::quadword_reg, RegisterValue::rbx),
    rcx = EncodeReg(RegisterKind::quadword_reg, RegisterValue::rcx),
    rdx = EncodeReg(RegisterKind::quadword_reg, RegisterValue::rdx),
    rsi = EncodeReg(RegisterKind::quadword_reg, RegisterValue::rsi),
    rdi = EncodeReg(RegisterKind::quadword_reg, RegisterValue::rdi),
    rbp = EncodeReg(RegisterKind::quadword_reg, RegisterValue::rbp),
    rsp = EncodeReg(RegisterKind::quadword_reg, RegisterValue::rsp),
    r8 = EncodeReg(RegisterKind::quadword_reg, RegisterValue::r8),
    r9 = EncodeReg(RegisterKind::quadword_reg, RegisterValue::r9),
    r10 = EncodeReg(RegisterKind::quadword_reg, RegisterValue::r10),
    r11 = EncodeReg(RegisterKind::quadword_reg, RegisterValue::r11),
    r12 = EncodeReg(RegisterKind::quadword_reg, RegisterValue::r12),
    r13 = EncodeReg(RegisterKind::quadword_reg, RegisterValue::r13),
    r14 = EncodeReg(RegisterKind::quadword_reg, RegisterValue::r14),
    r15 = EncodeReg(RegisterKind::quadword_reg, RegisterValue::r15),
    xmm0 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm0),
    xmm1 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm1),
    xmm2 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm2),
    xmm3 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm3),
    xmm4 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm4),
    xmm5 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm5),
    xmm6 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm6),
    xmm7 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm7),
    xmm8 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm8),
    xmm9 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm9),
    xmm10 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm10),
    xmm11 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm11),
    xmm12 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm12),
    xmm13 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm13),
    xmm14 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm14),
    xmm15 = EncodeReg(RegisterKind::xmm_reg, RegisterValue::xmm15)
};

constexpr RegisterKind RegKind(Register reg)
{
    return static_cast<RegisterKind>((static_cast<uint8_t>(reg) >> registerKindShift) & 0x0Fu);
}

constexpr RegisterValue RegValue(Register reg)
{
    return static_cast<RegisterValue>(static_cast<uint8_t>(reg) & 0x0Fu);
}

uint8_t RegValue7(Register reg);

class Emitter;

uint8_t Reg64Value(Register reg, Emitter& emitter, const soul::ast::Span& span);

// ModRM byte
//
// Follows the opcode byte
//
// ModRM.mod=xx
// 
// mod=00: content mode:
// 
// modRM r/m:
// 000 [rax]
// 001 [rcx]
// 010 [rdx]
// 011 [rbx]
// 100 SIB
// 101 DISP32
// 110 [rsi] 00 000 110 
// 111 [rdi]

// mod=01: DISP8 mode:
// 
// modRM r/m:
// 000 [rax+DISP8]
// 001 [rcx+DISP8]
// 010 [rdx+DISP8]
// 011 [rbx+DISP8]
// 100 SIB+DISP8
// 101 [rbp+DISP8]
// 110 [rsi+DISP8]
// 111 [rdi+DISP8]

// mod=10: DISP32 mode:
// 
// modRM r/m:
// 000 [rax+DISP32]
// 001 [rcx+DISP32]
// 010 [rdx+DISP32]
// 011 [rbx+DISP32]
// 100 SIB+DISP32
// 101 [rbp+DISP32]
// 110 [rsi+DISP32]
// 111 [rdi+DISP32]

// mod=11 register-direct addressing mode
//
// modRM r/m:
// 000 al, rax, xmm0
// 001 cl, rcx, xmm1
// 010 dl, rdx, xmm2
// 011 bl, rbx, xmm3
// 100 ah, spl, rsp, xmm4
// 101 ch, bpl, rbp, xmm5
// 110 dh, sil, rsi, xmm6
// 111 bh, dil, rdi, xmm7

// mod=11 = register-direct addressing mode
//
// REX.R 
// 0:
// modRM reg field:
// 000 rax, xmm0
// 001 rcx, xmm1
// 010 rdx, xmm2
// 011 rbx, xmm3
// 100 ah, rsp, xmm4
// 101 ch, rbp, xmm5
// 110 dh, rsi, xmm6
// 111 bh, rdi, xmm7
// 
// REX.R
// 1:
// modRM reg field:
// ...

enum class Mod : uint8_t
{
    contentMode = 0x0, disp8Mode = 0x1, disp32Mode = 0x2, registerDirectMode = 0x3
};

uint8_t EncodeModRMRegAndRegRMFieldRegDirect(Register reg, Emitter& emitter, const soul::ast::Span& span);

enum class ModRMRMField : uint8_t
{
//  mod=11:
    rax = 0x0u, al = 0x0u, xmm0 = 0x0,
    rcx = 0x1u, cl = 0x1u, xmm1 = 0x1,
    rdx = 0x2u, dl = 0x2u, xmm2 = 0x2,
    rbx = 0x3u, bl = 0x3u, xmm3 = 0x3,
    rsp = 0x4u, ah = 0x4u, spl = 0x4u, xmm4 = 0x4u,
    rbp = 0x5u, ch = 0x5u, bpl = 0x5u, xmm5 = 0x5u,
    rsi = 0x6u, dh = 0x6u, sil = 0x6u, xmm6 = 0x6u,
    rdi = 0x7u, bh = 0x7u, dil = 0x7u, xmm7 = 0x7u
};

constexpr uint8_t EncodeRMBits(Register contentReg)
{
    switch (contentReg)
    {
        case Register::rax: return 0u;
        case Register::rcx: return 1u;
        case Register::rdx: return 2u;
        case Register::rbx: return 3u;
        case Register::rbp: return 5u;
        case Register::rsi: return 6u;
        case Register::rdi: return 7u;
    }
    return SIB_PLUS_DISP;
}

constexpr uint8_t EncodeModRMByte(Mod mod, uint8_t reg, uint8_t rm) // mod=xx, reg=xxx, r/m=xxx
{
    return (static_cast<uint8_t>(mod) & 0x03) << 6u | (reg & 0x07) << 3u | rm & 0x07;
}

// SIB byte (scale, index, base)
// 
// May come after ModRM byte.
// 
// Effective address = scale * index + base + offet
// 
// SIB.scale field:
// 00 scale factor 1
// 01 scale factor 2
// 10 scale factor 4
// 11 scale factor 8
//
// SIB.index field:
// 000 [rax]
// 001 [rcx]
// 010 [rdx]
// 011 [rbx]
// 100 set scale*index to zero
// 101 [rbp]
// 110 [rsi]
// 111 [rdi]
//
// SIB.base field:
// 000 [rax]
// 001 [rcx]
// 010 [rdx]
// 011 [rbx]
// 100 [rsp]
// 101 [rbp], (none if mod=00)
// 110 dh, [rsi]
// 111 bh, [rdi]

// SIB.base field encoding for ModRM.r/m:
// mod: 00, 01, 10:
//      SIB.base
//      000 [rax]
//      001 [rcx]
//      010 [rdx]
//      011 [rbx]
//      100 [rsp]
// 00:  101 DISP32, 01: [rbp+DISP8], 10: [rbp+DISP32]
//      110 [rsi]
//      111 [rdi]
//  
// REX.X extends the SIB.index field
// REX.B extends the SIB.base field

uint8_t EncodeScale(uint8_t scaleFactor, Emitter& emitter, const soul::ast::Span& span);

constexpr uint8_t EncodeSIBByte(uint8_t scale, uint8_t index, uint8_t base) // scale = xx, index = xxx, base = xxx
{
    return ((scale & 0x3) << 6u) | (index & 0x7) << 3u | base & 0x07;
}

// REX prefix:
// 0x40 - 0x4F

enum class Rex : uint8_t 
{
    none = 0u,
    rexId = 0x40u, 
    w_bit = 0x1 << 3u,
    r_bit = 0x1 << 2u,
    x_bit = 0x1 << 1u,
    b_bit = 0x1 << 0u
};

constexpr Rex operator|(Rex left, Rex right)
{
    return Rex(uint8_t(left) | uint8_t(right));
}

constexpr Rex operator&(Rex left, Rex right)
{
    return Rex(uint8_t(left) & uint8_t(right));
}

constexpr Rex operator~(Rex rex)
{
    return Rex(~uint8_t(rex));
}

// REX.W: 0x48
// 0 default operand size
// 1 64-bit operand size

// REX.R: 0x44
// 0 ModRM.reg.msb=0
// 1 ModRM.reg.msb=1

// REX.X: 0x42
// 0 SIB.index.msb=0
// 1 SIB.index.msb=1

// REX.B: 0x41
// 0 ModRM.rm.msb=0, SIB.base.msb=0, opcode.reg.msb=0
// 1 ModRM.rm.msb=1, SIB.base.msb=1, opcode.reg.msb=1

// Instructions not requiring the REX prefix in 64-bit mode:
// 
// for example these instructions do not require REX prefix in 64-bit mode:
// CALL (near), Jcc, JMP (near), POP reg, PUSH reg, RET (near)

Rex RexRBitReg64(Register reg64, Emitter& emitter, const soul::ast::Span& span);
Rex RexRBitReg32(Register reg32, Emitter& emitter, const soul::ast::Span& span);
Rex RexRBitReg16(Register reg16, Emitter& emitter, const soul::ast::Span& span);
Rex RexXBitReg64(Register reg64, Emitter& emitter, const soul::ast::Span& span);
Rex RexBBitReg64(Register reg64, Emitter& emitter, const soul::ast::Span& span);
Rex RexRBitReg8(Register reg8, Emitter& emitter, const soul::ast::Span& span);
Rex RexXBitReg8(Register reg64, Emitter& emitter, const soul::ast::Span& span);
Rex RexBBitReg8(Register reg8, Emitter& emitter, const soul::ast::Span& span);

// ModRM.reg: /digit 0-7
// ModRM.reg and ModRM.rm: /r
// ib: value following the ModRM and SIB bytes is an immediate byte
// iw: value following the ModRM and SIB bytes is an immediate word
// id: value following the ModRM and SIB bytes is an immediate doubleword
// iq: value following the ModRM and SIB bytes is an immediate quadword
//
// REX.B: 0:
// 
// +rb:
// 0 AL
// 1 CL
// 2 DL
// 3 BL
// 4 AH, SPL
// 5 CH, BPL
// 6 DH, SIL
// 7 BH, DIL
// 
// +rw:
// 0 AX
// 1 CX
// 2 DX
// 3 BX
// 4 SP
// 5 BP
// 6 SI
// 7 DI
// 
// +rd:
// 0 EAX
// 1 ECX
// 2 EDX
// 3 EBX
// 4 ESP
// 5 EBP
// 6 ESI
// 7 EDI
// 
// +rq:
// 0 RAX
// 1 RCX
// 2 RDX
// 3 RBX
// 4 RSP
// 5 RBP
// 6 RSI
// 7 RDI
// 
// REX.B 1:
// 
// +rb:
// 0 R8B
// 1 R9B
// 2 R10B
// 3 R11B
// 4 R12B
// 5 R13B
// 6 R14B
// 7 R15B
//
// +rw:
// 0 R8W
// 1 R9W
// 2 R10W
// 3 R11W
// 4 R12W
// 5 R13W
// 6 R14W
// 7 R15W
// 
// +rd:
// 0 R8D
// 1 R9D
// 2 R10D
// 3 R11D
// 4 R12D
// 5 R13D
// 6 R14D
// 7 R15D
//
// +rq:
// 0 R8
// 1 R9
// 2 R10
// 3 R11
// 4 R12
// 5 R13
// 6 R14
// 7 R15

// DISP
// 1, 2 or 4 bytes
// diplacement bytes follow the opcode, ModRM, or SIB byte whichever comes last

// IMMEDIATE
// 1, 2, 4 or 8 bytes
// 

// ModRM reg field encoding, 32-bit and 64-bit addressing
// see AMD64 programmers manual page 548

} // namespace cmajor::sbin::machine_x64
