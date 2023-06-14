// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.process.manager;

import cmajor.systemx.kernel.process;
import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::kernel {

class ProcessManager : public cmajor::systemx::machine::MachineObserver
{
public:
    static void Init();
    static void Done();
    static ProcessManager& Instance() { return *instance; }
    ProcessManager(const ProcessManager&) = delete;
    ProcessManager& operator=(const ProcessManager&) = delete;
    ~ProcessManager();
    void SetMachine(cmajor::systemx::machine::Machine* machine_);
    cmajor::systemx::machine::Machine* GetMachine() const { return machine; }
    void Start();
    void Stop();
    Process* CreateProcess();
    Process* GetProcess(int32_t pid) const;
    void DeleteProcess(int32_t pid);
    void IncrementRunnableProcesses();
    void DecrementRunnableProcesses();
    void WaitForProcessesExit();
    void MachineStateChanged() override;
private:
    ProcessManager();
    static std::unique_ptr<ProcessManager> instance;
    cmajor::systemx::machine::Machine* machine;
    int maxProcs;
    int32_t nextProcessId;
    std::vector<std::unique_ptr<Process>> processTable;
    int32_t numProcessTableSlots;
    int numRunnableProcesses;
    std::condition_variable_any processesExitOrMachineStateChanged;
};

void InitProcessManager();
void DoneProcessManager();

} // namespace cmajor::systemx::kernel
