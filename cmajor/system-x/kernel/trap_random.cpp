// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#define _CRT_RAND_S
#include <stdlib.h>

module cmajor.systemx.kernel.trap.random;

import cmajor.systemx.kernel.trap;
import cmajor.systemx.kernel.error;
import cmajor.systemx.kernel.process;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

class TrapRandomSeedHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_random_seed"; }
};

uint64_t TrapRandomSeedHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    try
    {
        unsigned int seed = 0;
        errno_t error = rand_s(&seed);
        if (error == 0)
        {
            return static_cast<uint64_t>(seed);
        }
        else
        {
            throw SystemError(EFAIL, "rand_s failed", __FUNCTION__);
        }
    }
    catch (const SystemError& error)
    {
        Process* process = static_cast<Process*>(processor.CurrentProcess());
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

void InitTrapRandom()
{
    SetTrapHandler(trap_random_seed, new TrapRandomSeedHandler());
}

void DoneTrapRandom()
{
    SetTrapHandler(trap_random_seed, nullptr);
}

} // namespace cmajor::systemx::kernel
