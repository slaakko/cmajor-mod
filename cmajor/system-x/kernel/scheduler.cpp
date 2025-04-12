// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.scheduler;

import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.debug.help;
import cmajor.systemx.machine;
import util;

namespace cmajor::systemx::kernel {

std::unique_ptr<Scheduler> Scheduler::instance;

void Scheduler::Init()
{
    instance.reset(new Scheduler());
}

void Scheduler::Done()
{
    instance.reset();
}

Scheduler::Scheduler() : machine(nullptr)
{
}

void Scheduler::SetMachine(cmajor::systemx::machine::Machine* machine_)
{
    machine = machine_;
}

cmajor::systemx::machine::UserProcess* Scheduler::GetRunnableProcess(cmajor::systemx::machine::Processor* processor)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), SCHEDULER, 0, NO_LOCK | GET_RUNNABLE_PROCESS);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    queueNotEmptyOrExiting.wait(lock, [this] { return machine->Exiting() || !runnableProcesses.empty() || machine->HasRunnableKernelProcess();  });
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), SCHEDULER, 0, HAS_LOCK | GET_RUNNABLE_PROCESS);
#endif 
    while (!machine->Exiting() && !runnableProcesses.empty())
    {
        cmajor::systemx::machine::UserProcess* runnableProcess = runnableProcesses.front();
        runnableProcesses.pop_front();
        if (runnableProcess->State() == cmajor::systemx::machine::ProcessState::runnableInKernel)
        {
            runnableProcess->KernelProcessor()->AddRunnableKernelProcess(runnableProcess);
            return nullptr;
        }
        else if (runnableProcess->State() == cmajor::systemx::machine::ProcessState::runnableInUser)
        {
            return runnableProcess;
        }
    }
    return nullptr;
}

void Scheduler::AddRunnableProcess(cmajor::systemx::machine::UserProcess* runnableProcess, cmajor::systemx::machine::ProcessState processState)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), SCHEDULER, runnableProcess->Id(), NO_LOCK | ADD_RUNNABLE_PROCESS);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), SCHEDULER, runnableProcess->Id(), HAS_LOCK | ADD_RUNNABLE_PROCESS);
#endif 
    if (runnableProcess->State() != cmajor::systemx::machine::ProcessState::zombie)
    {
        runnableProcess->SetState(processState);
        runnableProcesses.push_back(runnableProcess);
        queueNotEmptyOrExiting.notify_all();
    }
}

void Scheduler::CheckRunnable()
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), SCHEDULER, 0, NO_LOCK | CHECK_RUNNABLE);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), SCHEDULER, 0, HAS_LOCK | CHECK_RUNNABLE);
#endif 
    if (!runnableProcesses.empty() || machine->HasRunnableKernelProcess())
    {
        queueNotEmptyOrExiting.notify_all();
    }
}

void Scheduler::Start()
{
    machine->SetScheduler(this);
    runnableProcesses.clear();
}

void Scheduler::Stop()
{
    queueNotEmptyOrExiting.notify_all();
}

class ClockInterruptHandler : public cmajor::systemx::machine::InterruptHandler
{
public:
    ClockInterruptHandler();
    void HandleInterrupt(cmajor::systemx::machine::Processor& processor) override;
};

ClockInterruptHandler::ClockInterruptHandler()
{
}

void ClockInterruptHandler::HandleInterrupt(cmajor::systemx::machine::Processor& processor)
{
    cmajor::systemx::machine::UserProcess* currentProcess = processor.CurrentProcess();
    if (currentProcess && currentProcess->State() != cmajor::systemx::machine::ProcessState::zombie && 
        currentProcess->State() != cmajor::systemx::machine::ProcessState::asleep)
    {
        std::lock_guard<std::recursive_mutex> lock(processor.GetMachine()->Lock());
        cmajor::systemx::machine::UserProcess* currentProcess = processor.CurrentProcess();
        processor.ResetCurrentProcess(true, currentProcess->DoSaveContext(), false);
        Scheduler::Instance().AddRunnableProcess(currentProcess, cmajor::systemx::machine::ProcessState::runnableInUser);
    }
}

void InitScheduler()
{
    Scheduler::Init();
    cmajor::systemx::machine::SetInterruptHandler(cmajor::systemx::machine::irq_clock, new ClockInterruptHandler());
}

void DoneScheduler()
{
    Scheduler::Done();
    cmajor::systemx::machine::SetInterruptHandler(cmajor::systemx::machine::irq_clock, nullptr);
}

} // namespace cmajor::systemx::kernel
