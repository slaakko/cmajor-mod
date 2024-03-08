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
    "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "rsp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", ""
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
    if (size >= 1 && size <= 8)
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
    if (size >= 1 && size <= 8)
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
    if (size >= 1 && size <= 8)
    {
        regs[size - 1] = reg;
    }
    else
    {
        throw std::runtime_error("otava.assembly.RegisterGroup::GetReg: invalid size");
    }
}

bool RegisterGroup::IsLocal() const
{
    if (kind >= RegisterGroupKind::r8 && kind <= RegisterGroupKind::r15)
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
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rax));

    RegisterGroup* rbx = new RegisterGroup(RegisterGroupKind::rbx);
    rbx->SetNonvolatile(true);
    rbx->SetReg(1, Register(RegisterKind::bl, RegisterGroupKind::rbx, 1));
    rbx->SetReg(2, Register(RegisterKind::bx, RegisterGroupKind::rbx, 2));
    rbx->SetReg(4, Register(RegisterKind::ebx, RegisterGroupKind::rbx, 4));
    rbx->SetReg(8, Register(RegisterKind::rbx, RegisterGroupKind::rbx, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rbx));

    RegisterGroup* rcx = new RegisterGroup(RegisterGroupKind::rcx);
    rcx->SetNonvolatile(false);
    rcx->SetReg(1, Register(RegisterKind::cl, RegisterGroupKind::rcx, 1));
    rcx->SetReg(2, Register(RegisterKind::cx, RegisterGroupKind::rcx, 2));
    rcx->SetReg(4, Register(RegisterKind::ecx, RegisterGroupKind::rcx, 4));
    rcx->SetReg(8, Register(RegisterKind::rcx, RegisterGroupKind::rcx, 8));
    regGroups.push_back(std::unique_ptr<RegisterGroup>(rcx));

    RegisterGroup* rdx = new RegisterGroup(RegisterGroupKind::rdx);
    rdx->SetNonvolatile(false);
    rdx->SetReg(1, Register(RegisterKind::dl, RegisterGroupKind::rdx, 1));
    rdx->SetReg(2, Register(RegisterKind::dx, RegisterGroupKind::rdx, 2));
    rdx->SetReg(4, Register(RegisterKind::edx, RegisterGroupKind::rdx, 4));
    rdx->SetReg(8, Register(RegisterKind::rdx, RegisterGroupKind::rdx, 8));
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
}

RegisterPool::RegisterPool(Registers& registers)
{
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::rsi));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::rdi));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r10));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r11));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r12));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r13));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r14));
    AddLocalRegisterGroup(registers.GetRegisterGroup(RegisterGroupKind::r15));
    localRegisterCount = localRegisterPool.size();
    globalRegisterMap[RegisterGroupKind::rax] = registers.GetRegisterGroup(RegisterGroupKind::rax);
    globalRegisterMap[RegisterGroupKind::rbx] = registers.GetRegisterGroup(RegisterGroupKind::rbx);
    globalRegisterMap[RegisterGroupKind::rcx] = registers.GetRegisterGroup(RegisterGroupKind::rcx);
    globalRegisterMap[RegisterGroupKind::rdx] = registers.GetRegisterGroup(RegisterGroupKind::rdx);
    globalRegisterMap[RegisterGroupKind::r8] = registers.GetRegisterGroup(RegisterGroupKind::r8);
    globalRegisterMap[RegisterGroupKind::r9] = registers.GetRegisterGroup(RegisterGroupKind::r9);
    globalRegisterMap[RegisterGroupKind::rbp] = registers.GetRegisterGroup(RegisterGroupKind::rbp);
    globalRegisterMap[RegisterGroupKind::rsp] = registers.GetRegisterGroup(RegisterGroupKind::rsp);
}

void RegisterPool::AddLocalRegisterGroup(RegisterGroup* regGroup)
{
    localRegisterPool.insert(regGroup);
}

RegisterGroup* RegisterPool::GetLocalRegisterGroup()
{
    if (localRegisterPool.empty())
    {
        throw std::runtime_error("register pool is empty");
    }
    RegisterGroup* regGroup = *localRegisterPool.begin();
    localRegisterPool.erase(regGroup);
    usedLocalRegs.insert(regGroup);
    return regGroup;
}

RegisterGroup* RegisterPool::GetGlobalRegisterGroup(RegisterGroupKind regGroupKind)
{
    auto it = globalRegisterMap.find(regGroupKind);
    if (it != globalRegisterMap.cend())
    {
        RegisterGroup* regGroup = it->second;
        if (regGroup->IsNonvolatile())
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
