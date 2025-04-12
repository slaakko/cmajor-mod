// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.scheduler;

import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class Scheduler : public cmajor::systemx::machine::Scheduler
{
public:
    static void Init();
    static void Done();
    static Scheduler& Instance() { return *instance; }
    void SetMachine(cmajor::systemx::machine::Machine* machine_) override;
    cmajor::systemx::machine::UserProcess* GetRunnableProcess(cmajor::systemx::machine::Processor* processor) override;
    void AddRunnableProcess(cmajor::systemx::machine::UserProcess* runnableProcess, cmajor::systemx::machine::ProcessState processState) override;
    void CheckRunnable() override;
    void Start();
    void Stop() override;
private:
    Scheduler();
    static std::unique_ptr<Scheduler> instance;
    cmajor::systemx::machine::Machine* machine;
    cmajor::systemx::machine::UserProcessList runnableProcesses;
    std::condition_variable_any queueNotEmptyOrExiting;
};

void InitScheduler();
void DoneScheduler();

} // namespace cmajor::systemx::kernel
