// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.sbin.machine_x64.reg;

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

} // namespace cmajor::sbin::machine_x64
