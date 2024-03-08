// =================================
// Copyright (c) 2024 Seppo Laakko
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
    rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, max
};

enum class RegisterGroupKind
{
    rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, max
};

class Register : public Value
{
public:
    Register();
    Register(RegisterKind kind_, RegisterGroupKind group_, int size_);
    RegisterKind Kind() const { return kind; }
    RegisterGroupKind Group() const { return group; }
    int Size() const { return size; }
private:
    RegisterKind kind;
    RegisterGroupKind group;
    int size;
};

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
    bool IsVolatile() const { return !nonvolatile; }
    bool IsNonvolatile() const { return nonvolatile; }
    void SetNonvolatile(bool nonvolatile_) { nonvolatile = nonvolatile_; }
private:
    RegisterGroupKind kind;
    Register regs[8];
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
    RegisterPool(Registers& registers);
    void AddLocalRegisterGroup(RegisterGroup* regGroup);
    RegisterGroup* GetLocalRegisterGroup();
    RegisterGroup* GetGlobalRegisterGroup(RegisterGroupKind regGroupKind);
    int LocalRegisterCount() const { return localRegisterCount; }
    int NumFreeLocalRegisters() const { return localRegisterPool.size(); }
    const std::set<RegisterGroup*, RegisterGroupLess>& UsedLocalRegs() const { return usedLocalRegs; }
    const std::set<RegisterGroup*, RegisterGroupLess>& UsedNonvolatileRegs() const { return usedNonvolatileRegs; }
private:
    int localRegisterCount;
    std::set<RegisterGroup*, RegisterGroupLess> localRegisterPool;
    std::map<RegisterGroupKind, RegisterGroup*> globalRegisterMap;
    std::set<RegisterGroup*, RegisterGroupLess> usedLocalRegs;
    std::set<RegisterGroup*, RegisterGroupLess> usedNonvolatileRegs;
};

} // namespace cmajor::masm::assembly
