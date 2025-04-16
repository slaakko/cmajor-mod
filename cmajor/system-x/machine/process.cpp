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

std::string ProcessStateStr(ProcessState state)
{
    switch (state)
    {
        case ProcessState::created: return "created";
        case ProcessState::exec: return "exec";
        case ProcessState::asleep: return "asleep";
        case ProcessState::runnableInUser: return "runnableInUser";
        case ProcessState::runnableInKernel: return "runnableInKernel";
        case ProcessState::running: return "running";
        case ProcessState::runningKernel: return "runningKernel";
        case ProcessState::zombie: return "zombie";
    }
    return std::string();
}

void UserProcess::Sleep(std::unique_lock<std::recursive_mutex>& lock)
{
    SetState(cmajor::systemx::machine::ProcessState::asleep);
    SetStartSleepTime();
    if (lock.owns_lock())
    {
        lock.unlock();
    }
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
    SetState(cmajor::systemx::machine::ProcessState::runningKernel);
}

} // namespace cmajor::systemx::machine
