// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.machine.process;

import cmajor.systemx.machine.processor;
import cmajor.systemx.machine.machine;
import util;

namespace cmajor::systemx::machine {

Process::~Process()
{
}

void UserProcess::Sleep()
{
    SetState(cmajor::systemx::machine::ProcessState::asleep);
    SetStartSleepTime();
}

void UserProcess::Wakeup(Scheduler* scheduler)
{
    AddSleepTime();
    scheduler->AddRunnableProcess(this, cmajor::systemx::machine::ProcessState::runnableInKernel);
}

void UserProcess::ReleaseProcessor()
{
    util::SwitchToFiber(MainFiber());
    SetStartSystemTime();
}

} // namespace cmajor::systemx::machine
