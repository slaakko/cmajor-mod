// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.event.manager;

import cmajor.systemx.kernel.scheduler;
import cmajor.systemx.kernel.debug.help;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

const int eventManager = -1;

void EventManager::Init()
{
    instance.reset(new EventManager());
}

void EventManager::Done()
{
    instance.reset();
}

std::unique_ptr<EventManager> EventManager::instance;

EventManager::EventManager() : machine(nullptr)
{
}

void EventManager::SetMachine(cmajor::systemx::machine::Machine* machine_)
{
    machine = machine_;
}

void EventManager::Start()
{
    sleepingProcesses.clear();
}

void EventManager::Stop()
{
    sleepingProcesses.clear();
}

void EventManager::SleepOn(const cmajor::systemx::machine::Event& evnt, cmajor::systemx::machine::Process* process, std::unique_lock<std::recursive_mutex>& lock)
{
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(lock.mutex(), EVENT_MANAGER, process->Id(), HAS_LOCK | SLEEP, evnt);
#endif 
    sleepingProcesses[evnt].push_back(process);
    lock.unlock();
    process->Sleep();
#if (LOCK_DEBUG)
    DebugLock startDebugLock(lock.mutex(), EVENT_MANAGER, process->Id(), HAS_LOCK | SLEEP, evnt);
#endif
}

void EventManager::Wakeup(const cmajor::systemx::machine::Event& evnt)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), EVENT_MANAGER, 0, NO_LOCK | WAKEUP, evnt);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    cmajor::systemx::machine::ProcessList processes;
    sleepingProcesses[evnt].swap(processes);
    sleepingProcesses.erase(evnt);
    for (auto& process : processes)
    {
#if (LOCK_DEBUG)
        DebugLock hasDebugLock(&machine->Lock(), EVENT_MANAGER, process->Id(), HAS_LOCK | WAKEUP, evnt);
#endif 
        process->Wakeup(machine->GetScheduler());
    }
}

void Sleep(const cmajor::systemx::machine::Event& evnt, cmajor::systemx::machine::Process* process, std::unique_lock<std::recursive_mutex>& lock)
{
    cmajor::systemx::machine::Processor* processor = process->GetProcessor();
    if (processor)
    {
        processor->ResetCurrentProcess(true, true);
    }
    Scheduler::Instance().CheckRunnable();
    EventManager::Instance().SleepOn(evnt, process, lock);
    if (processor)
    {
        process->ReleaseProcessor(processor);
    }
}

void Wakeup(const cmajor::systemx::machine::Event& evnt)
{
    EventManager::Instance().Wakeup(evnt);
}

void InitEventManager()
{
    EventManager::Init();
}

void DoneEventManager()
{
    EventManager::Done();
}

} // namespace cmajor::systemx::kernel
