// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.event.manager;

import cmajor.systemx.kernel.scheduler;
import cmajor.systemx.kernel.debug;
import cmajor.systemx.kernel.debug.help;
import cmajor.systemx.machine;
import util;

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
    if (InDebugMode(debugStateMode))
    {
        std::string line = "sleep.on ";
        int coreId = process->CoreId();
        if (coreId != -1)
        {
            line.append("core=").append(std::to_string(coreId)).append(1, ' ');
        }
        line.append("pid=");
        line.append(util::Format(std::to_string(process->Id()), 3, util::FormatWidth::min, util::FormatJustify::right, '0'));
        line.append(" event=").append(cmajor::systemx::machine::EventKindStr(evnt.kind)).append(".").append(std::to_string(evnt.id));
        DebugWrite(line);
    }
    sleepingProcesses[evnt].push_back(process);
    process->Sleep(lock);
#if (LOCK_DEBUG)
    DebugLock startDebugLock(lock.mutex(), EVENT_MANAGER, process->Id(), NO_LOCK | SLEEP, evnt);
#endif
}

void EventManager::Wakeup(cmajor::systemx::machine::Process* process, const cmajor::systemx::machine::Event& evnt)
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), EVENT_MANAGER, 0, NO_LOCK | WAKEUP, evnt);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    cmajor::systemx::machine::ProcessList processes;
    sleepingProcesses[evnt].swap(processes);
    sleepingProcesses.erase(evnt);
    std::string line;
    if (InDebugMode(debugStateMode) && !processes.empty())
    {
        if (process)
        {
            int coreId = process->CoreId();
            if (coreId != -1)
            {
                line.append("core=").append(std::to_string(coreId)).append(1, ' ');
            }
            line.append("pid=");
            line.append(util::Format(std::to_string(process->Id()), 3, util::FormatWidth::min, util::FormatJustify::right, '0')).append(1, ' ');
        }
        line.append("wake.up ");
        line.append("event=");
        line.append(cmajor::systemx::machine::EventKindStr(evnt.kind)).append(".").append(std::to_string(evnt.id));
    }
    for (auto& processToWakeUp : processes)
    {
#if (LOCK_DEBUG)
        DebugLock hasDebugLock(&machine->Lock(), EVENT_MANAGER, process->Id(), HAS_LOCK | WAKEUP, evnt);
#endif 
        processToWakeUp->Wakeup(machine->GetScheduler());
        if (InDebugMode(debugStateMode))
        {
            line.append(" pid=");
            line.append(util::Format(std::to_string(processToWakeUp->Id()), 3, util::FormatWidth::min, util::FormatJustify::right, '0')).append(1, ' ');
        }
    }
    if (InDebugMode(debugStateMode) && !processes.empty())
    {
        DebugWrite(line);
    }
}

void Sleep(const cmajor::systemx::machine::Event& evnt, cmajor::systemx::machine::Process* process, std::unique_lock<std::recursive_mutex>& lock)
{
    cmajor::systemx::machine::Processor* processor = process->GetProcessor();
    if (processor)
    {
        processor->ResetCurrentProcess(true, process->DoSaveContext(), true);
    }
    Scheduler::Instance().CheckRunnable();
    EventManager::Instance().SleepOn(evnt, process, lock);
    process->ReleaseProcessor();
}

void Wakeup(cmajor::systemx::machine::Process* process, const cmajor::systemx::machine::Event& evnt)
{
    EventManager::Instance().Wakeup(process, evnt);
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
