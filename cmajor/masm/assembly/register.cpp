// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.assembly.reg;

namespace cmajor::masm::assembly {

std::string regName[] =
{
    "al", "bl", "cl", "dl", "sil", "dil", "bpl", "spl", "r8b", "r9b", "r10b", "r11b", "r12b", "r13b", "r14b", "r15b",
    "ax", "bx", "cx", "dx", "si", "di", "bp", "sp", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w",
    "eax", "ebx", "ecx", "edx", "esi", "edi", "ebp", "esp", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d",
    "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", 
    "ah", "bh", "ch", "dh",
    "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "xmm8", "xmm9", "xmm10", "xmm11", "xmm12", "xmm13", "xmm14", "xmm15",
    ""
};

Register::Register() : Value(std::string()), kind(), group()
{
}

Register::Register(RegisterKind kind_, RegisterGroupKind group_, int size_) : Value(regName[int(kind_)]), kind(kind_), group(group_), size(size_)
{
}

RegisterGroup::RegisterGroup() : kind(RegisterGroupKind::max), nonvolatile(false)
{
}

RegisterGroup::RegisterGroup(RegisterGroupKind kind_) : kind(kind_), nonvolatile(false)
{
}

const Register* RegisterGroup::GetReg(int64_t size) const
{
    if (size >= 1 && size <= 8 || size == highByteRegSize || size == 16)
    {
        return &regs[size - 1];
    }
    else
    {
        throw std::runtime_error("otava.assembly.RegisterGroup::GetReg: invalid size");
    }
}

Register* RegisterGroup::GetReg(int64_t size)
{
    if (size >= 1 && size <= 8 || size == highByteRegSize || size ==  16)
    {
        return &regs[size - 1];
    }
    else
    {
        throw std::runtime_error("otava.assembly.RegisterGroup::GetReg: invalid size");
    }
}

void RegisterGroup::SetReg(int64_t size, const Register& reg)
{
    if (size >= 1 && size <= 8 || size == highByteRegSize || size == 16)
    {
        regs[size - 1] = reg;
    }
    else
    {
        throw std::runtime_error("otava.assembly.RegisterGroup::SetReg: invalid size");
    }
}

bool RegisterGroup::IsLocal() const
{
    if (kind >= RegisterGroupKind::r8 && kind <= RegisterGroupKind::r15)
    {
        return true;
    }
    else if (kind >= RegisterGroupKind::xmm6 && kind <= RegisterGroupKind::xmm15)
    {
        return true;
    }
    return false;
}

Registers::Registers()
{
    RegisterGroup* rax = new RegisterGroup(RegisterGroupKind::rax);
    rax->SetNonvolatile(false);
    rax->SetReg(1, Register(RegisterKind::al, RegisterGroupKind::rax, 1));
    rax->SetReg(2, Register(RegisterKind::ax, RegisterGroupKind::rax, 2));
    rax->SetReg(4, Register(RegisterKind::eax, RegisterGroupKind::rax, 4));
    rax->SetReg(8, Register(RegisterKind::rax, RegisterGroupKind::rax, 8));
    rax->SetReg(highByteRegSize, Register(RegisterKind::ah, RegisterGroupKind::rax, highByteRegSize));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rax));

    RegisterGroup* rbx = new RegisterGroup(RegisterGroupKind::rbx);
    rbx->SetNonvolatile(true);
    rbx->SetReg(1, Register(RegisterKind::bl, RegisterGroupKind::rbx, 1));
    rbx->SetReg(2, Register(RegisterKind::bx, RegisterGroupKind::rbx, 2));
    rbx->SetReg(4, Register(RegisterKind::ebx, RegisterGroupKind::rbx, 4));
    rbx->SetReg(8, Register(RegisterKind::rbx, RegisterGroupKind::rbx, 8));
    rbx->SetReg(highByteRegSize, Register(RegisterKind::bh, RegisterGroupKind::rbx, highByteRegSize));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rbx));

    RegisterGroup* rcx = new RegisterGroup(RegisterGroupKind::rcx);
    rcx->SetNonvolatile(false);
    rcx->SetReg(1, Register(RegisterKind::cl, RegisterGroupKind::rcx, 1));
    rcx->SetReg(2, Register(RegisterKind::cx, RegisterGroupKind::rcx, 2));
    rcx->SetReg(4, Register(RegisterKind::ecx, RegisterGroupKind::rcx, 4));
    rcx->SetReg(8, Register(RegisterKind::rcx, RegisterGroupKind::rcx, 8));
    rcx->SetReg(highByteRegSize, Register(RegisterKind::ch, RegisterGroupKind::rcx, highByteRegSize));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rcx));

    RegisterGroup* rdx = new RegisterGroup(RegisterGroupKind::rdx);
    rdx->SetNonvolatile(false);
    rdx->SetReg(1, Register(RegisterKind::dl, RegisterGroupKind::rdx, 1));
    rdx->SetReg(2, Register(RegisterKind::dx, RegisterGroupKind::rdx, 2));
    rdx->SetReg(4, Register(RegisterKind::edx, RegisterGroupKind::rdx, 4));
    rdx->SetReg(8, Register(RegisterKind::rdx, RegisterGroupKind::rdx, 8));
    rdx->SetReg(highByteRegSize, Register(RegisterKind::dh, RegisterGroupKind::rdx, highByteRegSize));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rdx));

    RegisterGroup* rsi = new RegisterGroup(RegisterGroupKind::rsi);
    rsi->SetNonvolatile(true);
    rsi->SetReg(1, Register(RegisterKind::sil, RegisterGroupKind::rsi, 1));
    rsi->SetReg(2, Register(RegisterKind::si, RegisterGroupKind::rsi, 2));
    rsi->SetReg(4, Register(RegisterKind::esi, RegisterGroupKind::rsi, 4));
    rsi->SetReg(8, Register(RegisterKind::rsi, RegisterGroupKind::rsi, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rsi));

    RegisterGroup* rdi = new RegisterGroup(RegisterGroupKind::rdi);
    rdi->SetNonvolatile(true);
    rdi->SetReg(1, Register(RegisterKind::dil, RegisterGroupKind::rdi, 1));
    rdi->SetReg(2, Register(RegisterKind::di, RegisterGroupKind::rdi, 2));
    rdi->SetReg(4, Register(RegisterKind::edi, RegisterGroupKind::rdi, 4));
    rdi->SetReg(8, Register(RegisterKind::rdi, RegisterGroupKind::rdi, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rdi));

    RegisterGroup* rbp = new RegisterGroup(RegisterGroupKind::rbp);
    rbp->SetNonvolatile(false);
    rbp->SetReg(1, Register(RegisterKind::bpl, RegisterGroupKind::rbp, 1));
    rbp->SetReg(2, Register(RegisterKind::bp, RegisterGroupKind::rbp, 2));
    rbp->SetReg(4, Register(RegisterKind::ebp, RegisterGroupKind::rbp, 4));
    rbp->SetReg(8, Register(RegisterKind::rbp, RegisterGroupKind::rbp, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rbp));

    RegisterGroup* rsp = new RegisterGroup(RegisterGroupKind::rsp);
    rsp->SetNonvolatile(false);
    rsp->SetReg(1, Register(RegisterKind::spl, RegisterGroupKind::rsp, 1));
    rsp->SetReg(2, Register(RegisterKind::sp, RegisterGroupKind::rsp, 2));
    rsp->SetReg(4, Register(RegisterKind::esp, RegisterGroupKind::rsp, 4));
    rsp->SetReg(8, Register(RegisterKind::rsp, RegisterGroupKind::rsp, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rsp));

    RegisterGroup* r8 = new RegisterGroup(RegisterGroupKind::r8);
    r8->SetNonvolatile(false);
    r8->SetReg(1, Register(RegisterKind::r8b, RegisterGroupKind::r8, 1));
    r8->SetReg(2, Register(RegisterKind::r8w, RegisterGroupKind::r8, 2));
    r8->SetReg(4, Register(RegisterKind::r8d, RegisterGroupKind::r8, 4));
    r8->SetReg(8, Register(RegisterKind::r8, RegisterGroupKind::r8, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(r8));

    RegisterGroup* r9 = new RegisterGroup(RegisterGroupKind::r9);
    r9->SetNonvolatile(false);
    r9->SetReg(1, Register(RegisterKind::r9b, RegisterGroupKind::r9, 1));
    r9->SetReg(2, Register(RegisterKind::r9w, RegisterGroupKind::r9, 2));
    r9->SetReg(4, Register(RegisterKind::r9d, RegisterGroupKind::r9, 4));
    r9->SetReg(8, Register(RegisterKind::r9, RegisterGroupKind::r9, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(r9));

    RegisterGroup* r10 = new RegisterGroup(RegisterGroupKind::r10);
    r10->SetNonvolatile(false);
    r10->SetReg(1, Register(RegisterKind::r10b, RegisterGroupKind::r10, 1));
    r10->SetReg(2, Register(RegisterKind::r10w, RegisterGroupKind::r10, 2));
    r10->SetReg(4, Register(RegisterKind::r10d, RegisterGroupKind::r10, 4));
    r10->SetReg(8, Register(RegisterKind::r10, RegisterGroupKind::r10, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(r10));

    RegisterGroup* r11 = new RegisterGroup(RegisterGroupKind::r11);
    r11->SetNonvolatile(false);
    r11->SetReg(1, Register(RegisterKind::r11b, RegisterGroupKind::r11, 1));
    r11->SetReg(2, Register(RegisterKind::r11w, RegisterGroupKind::r11, 2));
    r11->SetReg(4, Register(RegisterKind::r11d, RegisterGroupKind::r11, 4));
    r11->SetReg(8, Register(RegisterKind::r11, RegisterGroupKind::r11, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(r11));

    RegisterGroup* r12 = new RegisterGroup(RegisterGroupKind::r12);
    r12->SetNonvolatile(true);
    r12->SetReg(1, Register(RegisterKind::r12b, RegisterGroupKind::r12, 1));
    r12->SetReg(2, Register(RegisterKind::r12w, RegisterGroupKind::r12, 2));
    r12->SetReg(4, Register(RegisterKind::r12d, RegisterGroupKind::r12, 4));
    r12->SetReg(8, Register(RegisterKind::r12, RegisterGroupKind::r12, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(r12));

    RegisterGroup* r13 = new RegisterGroup(RegisterGroupKind::r13);
    r13->SetNonvolatile(true);
    r13->SetReg(1, Register(RegisterKind::r13b, RegisterGroupKind::r13, 1));
    r13->SetReg(2, Register(RegisterKind::r13w, RegisterGroupKind::r13, 2));
    r13->SetReg(4, Register(RegisterKind::r13d, RegisterGroupKind::r13, 4));
    r13->SetReg(8, Register(RegisterKind::r13, RegisterGroupKind::r13, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(r13));

    RegisterGroup* r14 = new RegisterGroup(RegisterGroupKind::r14);
    r14->SetNonvolatile(true);
    r14->SetReg(1, Register(RegisterKind::r14b, RegisterGroupKind::r14, 1));
    r14->SetReg(2, Register(RegisterKind::r14w, RegisterGroupKind::r14, 2));
    r14->SetReg(4, Register(RegisterKind::r14d, RegisterGroupKind::r14, 4));
    r14->SetReg(8, Register(RegisterKind::r14, RegisterGroupKind::r14, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(r14));

    RegisterGroup* r15 = new RegisterGroup(RegisterGroupKind::r15);
    r15->SetNonvolatile(true);
    r15->SetReg(1, Register(RegisterKind::r15b, RegisterGroupKind::r15, 1));
    r15->SetReg(2, Register(RegisterKind::r15w, RegisterGroupKind::r15, 2));
    r15->SetReg(4, Register(RegisterKind::r15d, RegisterGroupKind::r15, 4));
    r15->SetReg(8, Register(RegisterKind::r15, RegisterGroupKind::r15, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(r15));

    RegisterGroup* xmm0 = new RegisterGroup(RegisterGroupKind::xmm0);
    xmm0->SetReg(4, Register(RegisterKind::xmm0, RegisterGroupKind::xmm0, 4));
    xmm0->SetReg(8, Register(RegisterKind::xmm0, RegisterGroupKind::xmm0, 8));
    xmm0->SetReg(16, Register(RegisterKind::xmm0, RegisterGroupKind::xmm0, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm0));

    RegisterGroup* xmm1 = new RegisterGroup(RegisterGroupKind::xmm1);
    xmm1->SetReg(4, Register(RegisterKind::xmm1, RegisterGroupKind::xmm1, 4));
    xmm1->SetReg(8, Register(RegisterKind::xmm1, RegisterGroupKind::xmm1, 8));
    xmm1->SetReg(16, Register(RegisterKind::xmm1, RegisterGroupKind::xmm1, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm1));

    RegisterGroup* xmm2 = new RegisterGroup(RegisterGroupKind::xmm2);
    xmm2->SetReg(4, Register(RegisterKind::xmm2, RegisterGroupKind::xmm2, 4));
    xmm2->SetReg(8, Register(RegisterKind::xmm2, RegisterGroupKind::xmm2, 8));
    xmm2->SetReg(16, Register(RegisterKind::xmm2, RegisterGroupKind::xmm2, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm2));

    RegisterGroup* xmm3 = new RegisterGroup(RegisterGroupKind::xmm3);
    xmm3->SetReg(4, Register(RegisterKind::xmm3, RegisterGroupKind::xmm3, 4));
    xmm3->SetReg(8, Register(RegisterKind::xmm3, RegisterGroupKind::xmm3, 8));
    xmm3->SetReg(16, Register(RegisterKind::xmm3, RegisterGroupKind::xmm3, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm3));

    RegisterGroup* xmm4 = new RegisterGroup(RegisterGroupKind::xmm4);
    xmm4->SetReg(4, Register(RegisterKind::xmm4, RegisterGroupKind::xmm4, 4));
    xmm4->SetReg(8, Register(RegisterKind::xmm4, RegisterGroupKind::xmm4, 8));
    xmm4->SetReg(16, Register(RegisterKind::xmm4, RegisterGroupKind::xmm4, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm4));

    RegisterGroup* xmm5 = new RegisterGroup(RegisterGroupKind::xmm5);
    xmm5->SetReg(4, Register(RegisterKind::xmm5, RegisterGroupKind::xmm5, 4));
    xmm5->SetReg(8, Register(RegisterKind::xmm5, RegisterGroupKind::xmm5, 8));
    xmm5->SetReg(16, Register(RegisterKind::xmm5, RegisterGroupKind::xmm5, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm5));

    RegisterGroup* xmm6 = new RegisterGroup(RegisterGroupKind::xmm6);
    xmm6->SetNonvolatile(true);
    xmm6->SetReg(4, Register(RegisterKind::xmm6, RegisterGroupKind::xmm6, 4));
    xmm6->SetReg(8, Register(RegisterKind::xmm6, RegisterGroupKind::xmm6, 8));
    xmm6->SetReg(16, Register(RegisterKind::xmm6, RegisterGroupKind::xmm6, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm6));

    RegisterGroup* xmm7 = new RegisterGroup(RegisterGroupKind::xmm7);
    xmm7->SetNonvolatile(true);
    xmm7->SetReg(4, Register(RegisterKind::xmm7, RegisterGroupKind::xmm7, 4));
    xmm7->SetReg(8, Register(RegisterKind::xmm7, RegisterGroupKind::xmm7, 8));
    xmm7->SetReg(16, Register(RegisterKind::xmm7, RegisterGroupKind::xmm7, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm7));

    RegisterGroup* xmm8 = new RegisterGroup(RegisterGroupKind::xmm8);
    xmm8->SetNonvolatile(true);
    xmm8->SetReg(4, Register(RegisterKind::xmm8, RegisterGroupKind::xmm8, 4));
    xmm8->SetReg(8, Register(RegisterKind::xmm8, RegisterGroupKind::xmm8, 8));
    xmm8->SetReg(16, Register(RegisterKind::xmm8, RegisterGroupKind::xmm8, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm8));

    RegisterGroup* xmm9 = new RegisterGroup(RegisterGroupKind::xmm9);
    xmm9->SetNonvolatile(true);
    xmm9->SetReg(4, Register(RegisterKind::xmm9, RegisterGroupKind::xmm9, 4));
    xmm9->SetReg(8, Register(RegisterKind::xmm9, RegisterGroupKind::xmm9, 8));
    xmm9->SetReg(16, Register(RegisterKind::xmm9, RegisterGroupKind::xmm9, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm9));

    RegisterGroup* xmm10 = new RegisterGroup(RegisterGroupKind::xmm10);
    xmm10->SetNonvolatile(true);
    xmm10->SetReg(4, Register(RegisterKind::xmm10, RegisterGroupKind::xmm10, 4));
    xmm10->SetReg(8, Register(RegisterKind::xmm10, RegisterGroupKind::xmm10, 8));
    xmm10->SetReg(16, Register(RegisterKind::xmm10, RegisterGroupKind::xmm10, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm10));

    RegisterGroup* xmm11 = new RegisterGroup(RegisterGroupKind::xmm11);
    xmm11->SetNonvolatile(true);
    xmm11->SetReg(4, Register(RegisterKind::xmm11, RegisterGroupKind::xmm11, 4));
    xmm11->SetReg(8, Register(RegisterKind::xmm11, RegisterGroupKind::xmm11, 8));
    xmm11->SetReg(16, Register(RegisterKind::xmm11, RegisterGroupKind::xmm11, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm11));

    RegisterGroup* xmm12 = new RegisterGroup(RegisterGroupKind::xmm12);
    xmm12->SetNonvolatile(true);
    xmm12->SetReg(4, Register(RegisterKind::xmm12, RegisterGroupKind::xmm12, 4));
    xmm12->SetReg(8, Register(RegisterKind::xmm12, RegisterGroupKind::xmm12, 8));
    xmm12->SetReg(16, Register(RegisterKind::xmm12, RegisterGroupKind::xmm12, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm12));

    RegisterGroup* xmm13 = new RegisterGroup(RegisterGroupKind::xmm13);
    xmm13->SetNonvolatile(true);
    xmm13->SetReg(4, Register(RegisterKind::xmm13, RegisterGroupKind::xmm13, 4));
    xmm13->SetReg(8, Register(RegisterKind::xmm13, RegisterGroupKind::xmm13, 8));
    xmm13->SetReg(16, Register(RegisterKind::xmm13, RegisterGroupKind::xmm13, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm13));

    RegisterGroup* xmm14 = new RegisterGroup(RegisterGroupKind::xmm14);
    xmm14->SetNonvolatile(true);
    xmm14->SetReg(4, Register(RegisterKind::xmm14, RegisterGroupKind::xmm14, 4));
    xmm14->SetReg(8, Register(RegisterKind::xmm14, RegisterGroupKind::xmm14, 8));
    xmm14->SetReg(16, Register(RegisterKind::xmm14, RegisterGroupKind::xmm14, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm14));

    RegisterGroup* xmm15 = new RegisterGroup(RegisterGroupKind::xmm15);
    xmm15->SetNonvolatile(true);
    xmm15->SetReg(4, Register(RegisterKind::xmm15, RegisterGroupKind::xmm15, 4));
    xmm15->SetReg(8, Register(RegisterKind::xmm15, RegisterGroupKind::xmm15, 8));
    xmm15->SetReg(16, Register(RegisterKind::xmm15, RegisterGroupKind::xmm15, 16));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(xmm15));
}

RegisterPool::RegisterPool(Registers& registers) : localRegisterCount(0), localXMMRegisterCount(0)
{
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::rsi));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::rdi));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r10));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r11));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r12));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r13));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r14));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r15));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm6));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm7));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm8));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm9));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm10));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm11));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm12));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm13));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm14));
    AddLocalXMMRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::xmm15));
    localRegisterCount = localRegisterPool.size();
    localXMMRegisterCount = localXMMRegisterPool.size();
    globalRegisterMap[RegisterGroupKind::rax] = registers.GetRegisterGroup(RegisterGroupKind::rax);
    globalRegisterMap[RegisterGroupKind::rbx] = registers.GetRegisterGroup(RegisterGroupKind::rbx);
    globalRegisterMap[RegisterGroupKind::rcx] = registers.GetRegisterGroup(RegisterGroupKind::rcx);
    globalRegisterMap[RegisterGroupKind::rdx] = registers.GetRegisterGroup(RegisterGroupKind::rdx);
    globalRegisterMap[RegisterGroupKind::r8] = registers.GetRegisterGroup(RegisterGroupKind::r8);
    globalRegisterMap[RegisterGroupKind::r9] = registers.GetRegisterGroup(RegisterGroupKind::r9);
    globalRegisterMap[RegisterGroupKind::rbp] = registers.GetRegisterGroup(RegisterGroupKind::rbp);
    globalRegisterMap[RegisterGroupKind::rsp] = registers.GetRegisterGroup(RegisterGroupKind::rsp);
    globalRegisterMap[RegisterGroupKind::xmm0] = registers.GetRegisterGroup(RegisterGroupKind::xmm0);
    globalRegisterMap[RegisterGroupKind::xmm1] = registers.GetRegisterGroup(RegisterGroupKind::xmm1);
    globalRegisterMap[RegisterGroupKind::xmm2] = registers.GetRegisterGroup(RegisterGroupKind::xmm2);
    globalRegisterMap[RegisterGroupKind::xmm3] = registers.GetRegisterGroup(RegisterGroupKind::xmm3);
    globalRegisterMap[RegisterGroupKind::xmm4] = registers.GetRegisterGroup(RegisterGroupKind::xmm4);
    globalRegisterMap[RegisterGroupKind::xmm5] = registers.GetRegisterGroup(RegisterGroupKind::xmm5);
}

void RegisterPool::AddLocalRegisterGroup(RegisterGroup* regGroup)
{
    localRegisterPool.insert(regGroup);
}

void RegisterPool::AddLocalXMMRegisterGroup(RegisterGroup* regGroup)
{
    localXMMRegisterPool.insert(regGroup);
}

RegisterGroup* RegisterPool::GetLocalRegisterGroup()
{
    if (localRegisterPool.empty())
    {
        throw std::runtime_error("local register pool is empty");
    }
    RegisterGroup* regGroup = *localRegisterPool.begin();
    localRegisterPool.erase(regGroup);
    usedLocalRegs.insert(regGroup);
    return regGroup;
}

RegisterGroup* RegisterPool::GetLocalXMMRegisterGroup()
{
    if (localXMMRegisterPool.empty())
    {
        throw std::runtime_error("local XMM register pool is empty");
    }
    RegisterGroup* regGroup = *localXMMRegisterPool.begin();
    localXMMRegisterPool.erase(regGroup);
    usedLocalXMMRegs.insert(regGroup);
    return regGroup;
}

RegisterGroup* RegisterPool::GetGlobalRegisterGroup(RegisterGroupKind regGroupKind, bool used)
{
    auto it = globalRegisterMap.find(regGroupKind);
    if (it != globalRegisterMap.cend())
    {
        RegisterGroup* regGroup = it->second;
        if (used && regGroup->IsNonvolatile())
        {
            usedNonvolatileRegs.insert(regGroup);
        }
        return regGroup;
    }
    else
    {
        throw std::runtime_error("invalid global register");
    }
}

bool RegisterGroupLess::operator()(RegisterGroup* left, RegisterGroup* right) const
{
    return int(left->Kind()) < int(right->Kind());
}

} // namespace cmajor::masm::assembly
