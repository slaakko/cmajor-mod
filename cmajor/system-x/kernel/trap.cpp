// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.trap;

import cmajor.systemx.kernel.error;
import cmajor.systemx.machine;
import util;

namespace cmajor::systemx::kernel {

TrapHandler::~TrapHandler()
{
}

class SoftwareInterruptHandler : public cmajor::systemx::machine::InterruptHandler
{
public:
    SoftwareInterruptHandler();
    void HandleInterrupt(cmajor::systemx::machine::Processor& processor) override;
    void SetTrapHandler(uint8_t trap, TrapHandler* handler);
    TrapHandler* GetTrapHandler(uint8_t trap) const;
private:
    std::vector<std::unique_ptr<TrapHandler>> trapHandlers;
};

SoftwareInterruptHandler::SoftwareInterruptHandler()
{
    trapHandlers.resize(256);
}

void SoftwareInterruptHandler::SetTrapHandler(uint8_t trap, TrapHandler* handler)
{
    trapHandlers[trap].reset(handler);
}

TrapHandler* SoftwareInterruptHandler::GetTrapHandler(uint8_t trap) const
{
    return trapHandlers[trap].get();
}

void SoftwareInterruptHandler::HandleInterrupt(cmajor::systemx::machine::Processor& processor)
{
    uint64_t trap = processor.Regs().GetSpecial(cmajor::systemx::machine::rX);
    uint8_t trapZ = static_cast<uint8_t>(trap);
    trap = trap >> 8;
    uint8_t trapY = static_cast<uint8_t>(trap);
    trap = trap >> 8;
    uint8_t trapX = static_cast<uint8_t>(trap);
    if (trapX == 0 && trapZ == 0)
    {
        TrapHandler* trapHandler = trapHandlers[trapY].get();
        if (trapHandler)
        {
            uint64_t ax = trapHandler->HandleTrap(processor);
            cmajor::systemx::machine::Process* process = processor.CurrentProcess();
            if (process->DoSaveContext())
            {
                processor.Regs().Set(cmajor::systemx::machine::regAX, ax);
            }
            else
            {
                process->SetRegAX(ax);
                process->SetUseRegAX();
            }
        }
        else
        {
            throw SystemError(ENOTFOUND, "no handler for TRAP #" + util::ToHexString(trapY), __FUNCTION__);
        }
    }
    else
    {
        throw SystemError(EFAIL, "invalid TRAP #" + util::ToHexString(trap), __FUNCTION__);
    }
}

void SetTrapHandler(uint8_t trap, TrapHandler* handler)
{
    SoftwareInterruptHandler* softwareInterruptHandler = static_cast<SoftwareInterruptHandler*>(cmajor::systemx::machine::GetInterruptHandler(cmajor::systemx::machine::irq_software));
    softwareInterruptHandler->SetTrapHandler(trap, handler);
}

void AddTrapsToSymbolTable(cmajor::systemx::object::SymbolTable& symbolTable)
{
    SoftwareInterruptHandler* softwareInterruptHandler = static_cast<SoftwareInterruptHandler*>(cmajor::systemx::machine::GetInterruptHandler(cmajor::systemx::machine::irq_software));
    if (softwareInterruptHandler)
    {
        for (int i = 0; i < 256; ++i)
        {
            uint8_t trap = static_cast<uint8_t>(i);
            TrapHandler* trapHandler = softwareInterruptHandler->GetTrapHandler(trap);
            if (trapHandler)
            {
                symbolTable.AddTrapSymbol(new cmajor::systemx::object::Symbol(cmajor::systemx::object::SymbolKind::global, std::string(), trapHandler->TrapName(), cmajor::systemx::object::Value(trap)));
            }
        }
    }
}

void InitTrap()
{
    cmajor::systemx::machine::SetInterruptHandler(cmajor::systemx::machine::irq_software, new SoftwareInterruptHandler());
}

void DoneTrap()
{
    cmajor::systemx::machine::SetInterruptHandler(cmajor::systemx::machine::irq_software, nullptr);
}

} // namespace cmajor::systemx::kernel
