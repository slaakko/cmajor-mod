// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.machine_x64.reg;

import cmajor.sbin.machine_x64.emitter;

namespace cmajor::sbin::machine_x64 {

uint8_t Reg64Value(Register reg, Emitter& emitter, const soul::ast::Span& span)
{
    switch (reg)
    {
        case Register::rax:
        case Register::rbx:
        case Register::rcx:
        case Register::rdx:
        case Register::rsi:
        case Register::rdi:
        case Register::rbp:
        case Register::rsp:
        {
            return static_cast<uint8_t>(RegValue(reg));
        }
        case Register::r8:
        case Register::r9:
        case Register::r10:
        case Register::r11:
        case Register::r12:
        case Register::r13:
        case Register::r14:
        case Register::r15:
        {
            return static_cast<uint8_t>(RegValue(reg)) - static_cast<uint8_t>(RegValue(Register::r8));
        }
    }
    emitter.ThrowError("invalid reg/64 register", span);
    return 0u;
}

uint8_t EncodeScale(uint8_t scaleFactor, Emitter& emitter, const soul::ast::Span& span)
{
    switch (scaleFactor)
    {
        case 1u:
        {
            return 0x0;
        }
        case 2u:
        {
            return 0x1;
        }
        case 4u:
        {
            return 0x2;
        }
        case 8u:
        {
            return 0x3;
        }
    }
    emitter.ThrowError("invalid scale factor", span);
    return 0x0;
}

Rex RexRBitReg64(Register reg64, Emitter& emitter, const soul::ast::Span& span)
{
    switch (reg64)
    {
        case Register::rax:
        case Register::rbx:
        case Register::rcx:
        case Register::rdx:
        case Register::rsi:
        case Register::rdi:
        case Register::rbp:
        case Register::rsp:
        {
            return Rex::none;
        }
        case Register::r8:
        case Register::r9:
        case Register::r10:
        case Register::r11:
        case Register::r12:
        case Register::r13:
        case Register::r14:
        case Register::r15:
        {
            return Rex::r_bit;
        }
    }
    emitter.ThrowError("64-bit register expected", span);
    return Rex::none;
}

Rex RexRBitReg32(Register reg32, Emitter& emitter, const soul::ast::Span& span)
{
    switch (reg32)
    {
        case Register::eax:
        case Register::ebx:
        case Register::ecx:
        case Register::edx:
        case Register::esi:
        case Register::edi:
        case Register::ebp:
        case Register::esp:
        {
            return Rex::none;
        }
        case Register::r8d:
        case Register::r9d:
        case Register::r10d:
        case Register::r11d:
        case Register::r12d:
        case Register::r13d:
        case Register::r14d:
        case Register::r15d:
        {
            return Rex::r_bit;
        }
    }
    emitter.ThrowError("32-bit register expected", span);
    return Rex::none;
}

Rex RexRBitReg16(Register reg16, Emitter& emitter, const soul::ast::Span& span)
{
    switch (reg16)
    {
        case Register::ax:
        case Register::bx:
        case Register::cx:
        case Register::dx:
        case Register::si:
        case Register::di:
        case Register::bp:
        case Register::sp:
        {
            return Rex::none;
        }
        case Register::r8w:
        case Register::r9w:
        case Register::r10w:
        case Register::r11w:
        case Register::r12w:
        case Register::r13w:
        case Register::r14w:
        case Register::r15w:
        {
            return Rex::r_bit;
        }
    }
    emitter.ThrowError("16-bit register expected", span);
    return Rex::none;
}

Rex RexRBitReg8(Register reg8, Emitter& emitter, const soul::ast::Span& span)
{
    switch (reg8)
    {
        case Register::al:
        case Register::bl:
        case Register::cl:
        case Register::dl:
        case Register::sil:
        case Register::dil:
        case Register::bpl:
        case Register::spl:
        {
            return Rex::none;
        }
        case Register::r8b:
        case Register::r9b:
        case Register::r10b:
        case Register::r11b:
        case Register::r12b:
        case Register::r13b:
        case Register::r14b:
        case Register::r15b:
        {
            return Rex::r_bit;
        }
    }
    emitter.ThrowError("8-bit register expected", span);
    return Rex::none;
}

Rex RexXBitReg64(Register reg64, Emitter& emitter, const soul::ast::Span& span)
{
    switch (reg64)
    {
        case Register::rax:
        case Register::rbx:
        case Register::rcx:
        case Register::rdx:
        case Register::rsi:
        case Register::rdi:
        case Register::rbp:
        case Register::rsp:
        {
            return Rex::none;
        }
        case Register::r8:
        case Register::r9:
        case Register::r10:
        case Register::r11:
        case Register::r12:
        case Register::r13:
        case Register::r14:
        case Register::r15:
        {
            return Rex::x_bit;
        }
    }
    emitter.ThrowError("64-bit register expected", span);
    return Rex::none;
}

Rex RexXBitReg8(Register reg8, Emitter& emitter, const soul::ast::Span& span)
{
    switch (reg8)
    {
        case Register::al:
        case Register::bl:
        case Register::cl:
        case Register::dl:
        case Register::sil:
        case Register::dil:
        case Register::bpl:
        case Register::spl:
        {
            return Rex::none;
        }
        case Register::r8b:
        case Register::r9b:
        case Register::r10b:
        case Register::r11b:
        case Register::r12b:
        case Register::r13b:
        case Register::r14b:
        case Register::r15b:
        {
            return Rex::x_bit;
        }
    }
    emitter.ThrowError("8-bit register expected", span);
    return Rex::none;
}

Rex RexBBitReg64(Register reg64, Emitter& emitter, const soul::ast::Span& span)
{
    switch (reg64)
    {
        case Register::rax:
        case Register::rbx:
        case Register::rcx:
        case Register::rdx:
        case Register::rsi:
        case Register::rdi:
        case Register::rbp:
        case Register::rsp:
        {
            return Rex::none;
        }
        case Register::r8:
        case Register::r9:
        case Register::r10:
        case Register::r11:
        case Register::r12:
        case Register::r13:
        case Register::r14:
        case Register::r15:
        {
            return Rex::b_bit;
        }
    }
    emitter.ThrowError("64-bit register expected", span);
    return Rex::none;
}

Rex RexBBitReg8(Register reg8, Emitter& emitter, const soul::ast::Span& span)
{
    switch (reg8)
    {
        case Register::al:
        case Register::bl:
        case Register::cl:
        case Register::dl:
        case Register::sil:
        case Register::dil:
        case Register::bpl:
        case Register::spl:
        {
            return Rex::none;
        }
        case Register::r8b:
        case Register::r9b:
        case Register::r10b:
        case Register::r11b:
        case Register::r12b:
        case Register::r13b:
        case Register::r14b:
        case Register::r15b:
        {
            return Rex::b_bit;
        }
    }
    emitter.ThrowError("8-bit register expected", span);
    return Rex::none;
}

uint8_t RegValue7(Register reg)
{
    switch (reg)
    {
        case Register::rax:
        case Register::eax:
        case Register::ax:
        case Register::al:
        case Register::xmm0:
        {
            return 0x0;
        }
        case Register::rcx:
        case Register::ecx:
        case Register::cx:
        case Register::cl:
        case Register::xmm1:
        {
            return 0x1;
        }
        case Register::rdx:
        case Register::edx:
        case Register::dx:
        case Register::dl:
        case Register::xmm2:
        {
            return 0x2;
        }
        case Register::rbx:
        case Register::ebx:
        case Register::bx:
        case Register::bl:
        case Register::xmm3:
        {
            return 0x3;
        }
        case Register::ah:
        case Register::rsp:
        case Register::esp:
        case Register::sp:
        case Register::spl:
        case Register::xmm4:
        {
            return 0x4;
        }
        case Register::ch:
        case Register::rbp:
        case Register::ebp:
        case Register::bp:
        case Register::bpl:
        case Register::xmm5:
        {
            return 0x5;
        }
        case Register::dh:
        case Register::rsi:
        case Register::esi:
        case Register::si:
        case Register::sil:
        case Register::xmm6:
        {
            return 0x6;
        }
        case Register::bh:
        case Register::rdi:
        case Register::edi:
        case Register::di:
        case Register::dil:
        case Register::xmm7:
        {
            return 0x7;
        }
    }
    return 0x0;
}

} // namespace cmajor::sbin::machine_x64
