// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.trap.exit;

import cmajor.systemx.kernel.trap;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

class TrapExitHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_exit"; }
};

uint64_t TrapExitHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    uint8_t exitCode = static_cast<uint8_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
    cmajor::systemx::machine::UserProcess* currentProcess = processor.CurrentProcess();
    currentProcess->Exit(exitCode);
    return 0;
}

void InitTrapExit()
{
    SetTrapHandler(trap_exit, new TrapExitHandler());
}

void DoneTrapExit()
{
    SetTrapHandler(trap_exit, nullptr);
}

} // namespace cmajor::systemx::kernel
