// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.math.traps;

import cmajor.systemx.kernel.trap;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

class TrapPowHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_pow"; }
};

uint64_t TrapPowHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    int64_t lx = processor.Regs().Get(cmajor::systemx::machine::regAX);
    int64_t ly = processor.Regs().Get(cmajor::systemx::machine::regBX);
    double x = *static_cast<double*>(static_cast<void*>(&lx));
    double y = *static_cast<double*>(static_cast<void*>(&ly));
    double result = std::pow(x, y);
    int64_t r = *static_cast<int64_t*>(static_cast<void*>(&result));
    return static_cast<uint64_t>(r);
}

void InitMathTraps()
{
    SetTrapHandler(trap_pow, new TrapPowHandler());
}

void DoneMathTraps()
{
    SetTrapHandler(trap_pow, nullptr);
}

} // namespace cmajor::systemx::kernel

