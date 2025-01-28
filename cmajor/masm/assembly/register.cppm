// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.assembly.reg;

import std.core;
import cmajor.masm.assembly.value;

export namespace cmajor::masm::assembly {

enum class RegisterKind
{
    al, bl, cl, dl, sil, dil, bpl, spl, r8b, r9b, r10b, r11b, r12b, r13b, r14b, r15b,
    ax, bx, cx, dx, si, di, bp, sp, r8w, r9w, r10w, r11w, r12w, r13w, r14w, r15w,
    eax, ebx, ecx, edx, esi, edi, ebp, esp, r8d, r9d, r10d, r11d, r12d, r13d, r14d, r15d,
    rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, 
    ah, bh, ch, dh, 
    xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15,
    max
};

enum class RegisterGroupKind
{
    rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, 
    xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7, xmm8, xmm9, xmm10, xmm11, xmm12, xmm13, xmm14, xmm15,
    max
};

class Register : public Value
{
public:
    Register();
    Register(RegisterKind kind_, RegisterGroupKind group_, int size_);
    RegisterKind RegKind() const { return kind; }
    RegisterGroupKind Group() const { return group; }
    int Size() const { return size; }
    bool IsFloatingPointReg() const { return group >= RegisterGroupKind::xmm0 && group <= RegisterGroupKind::xmm15; }
private:
    RegisterKind kind;
    RegisterGroupKind group;
    int size;
};

const int highByteRegSize = 9;

class RegisterGroup
{
public:
    RegisterGroup();
    RegisterGroup(RegisterGroupKind kind_);
    RegisterGroupKind Kind() const { return kind; }
    const Register* GetReg(int64_t size) const;
    Register* GetReg(int64_t size);
    void SetReg(int64_t size, const Register& reg);
    bool IsLocal() const;
    bool IsFloatingPointReg() const { return kind >= RegisterGroupKind::xmm0 && kind <= RegisterGroupKind::xmm15; }
    bool IsVolatile() const { return !nonvolatile; }
    bool IsNonvolatile() const { return nonvolatile; }
    void SetNonvolatile(bool nonvolatile_) { nonvolatile = nonvolatile_; }
private:
    RegisterGroupKind kind;
    Register regs[16];
    bool nonvolatile;
};

class Registers
{
public:
    Registers();
    const RegisterGroup* GetRegisterGroup(RegisterGroupKind kind) const { return regGroups[int(kind)].get(); }
    RegisterGroup* GetRegisterGroup(RegisterGroupKind kind) { return regGroups[int(kind)].get(); }
private:
    std::vector<std::unique_ptr<RegisterGroup>> regGroups;
};

struct RegisterGroupLess
{
    bool operator()(RegisterGroup* left, RegisterGroup* right) const;
};

class RegisterPool
{
public:
    RegisterPool(Registers& registers_);
    void AddLocalRegisterGroup(RegisterGroup* regGroup);
    void AddLocalXMMRegisterGroup(RegisterGroup* regGroup);
    RegisterGroup* GetLocalRegisterGroup();
    RegisterGroup* GetLocalXMMRegisterGroup();
    RegisterGroup* GetRegisterGroup(RegisterGroupKind regGroupKind, bool used);
    int LocalRegisterCount() const { return localRegisterCount; }
    int LocalXMMRegisterCount() const { return localXMMRegisterCount; }
    int NumFreeLocalRegisters() const { return localRegisterPool.size(); }
    int NumFreeLocalXMMRegisters() const { return localXMMRegisterPool.size(); }
    const std::set<RegisterGroup*, RegisterGroupLess>& UsedLocalRegs() const { return usedLocalRegs; }
    const std::set<RegisterGroup*, RegisterGroupLess>& UsedNonvolatileRegs() const { return usedNonvolatileRegs; }
    const std::set<RegisterGroup*, RegisterGroupLess>& UsedLocalXMMRegs() const { return usedLocalXMMRegs; }
    const std::set<RegisterGroup*, RegisterGroupLess>& UsedNonvolatileXMMRegs() const { return usedNonvolatileXMMRegs; }
private:
    Registers& registers;
    int localRegisterCount;
    int localXMMRegisterCount;
    std::set<RegisterGroup*, RegisterGroupLess> localRegisterPool;
    std::set<RegisterGroup*, RegisterGroupLess> localXMMRegisterPool;
    std::map<RegisterGroupKind, RegisterGroup*> globalRegisterMap;
    std::set<RegisterGroup*, RegisterGroupLess> usedLocalRegs;
    std::set<RegisterGroup*, RegisterGroupLess> usedNonvolatileRegs;
    std::set<RegisterGroup*, RegisterGroupLess> usedLocalXMMRegs;
    std::set<RegisterGroup*, RegisterGroupLess> usedNonvolatileXMMRegs;
};

} // namespace cmajor::masm::assembly
