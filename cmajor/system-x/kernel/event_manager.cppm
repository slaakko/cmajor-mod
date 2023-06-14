// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.event.manager;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class EventManager
{
public:
    static void Init();
    static void Done();
    static EventManager& Instance() { return *instance; }
    void SetMachine(cmajor::systemx::machine::Machine* machine_);
    void Start();
    void Stop();
    void SleepOn(const cmajor::systemx::machine::Event& evnt, cmajor::systemx::machine::Process* process, std::unique_lock<std::recursive_mutex>& lock);
    void Wakeup(const cmajor::systemx::machine::Event& evnt);
private:
    static std::unique_ptr<EventManager> instance;
    EventManager();
    cmajor::systemx::machine::Machine* machine;
    std::map<cmajor::systemx::machine::Event, cmajor::systemx::machine::ProcessList> sleepingProcesses;
};

void Sleep(const cmajor::systemx::machine::Event& evnt, cmajor::systemx::machine::Process* process, std::unique_lock<std::recursive_mutex>& lock);
void Wakeup(const cmajor::systemx::machine::Event& evnt);
void InitEventManager();
void DoneEventManager();

} // namespace cmajor::systemx::kernel
