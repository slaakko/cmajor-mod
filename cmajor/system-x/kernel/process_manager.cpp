// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.process.manager;

import cmajor.systemx.kernel.debug.help;
import cmajor.systemx.kernel.error;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

std::unique_ptr<ProcessManager> ProcessManager::instance;

void ProcessManager::Init()
{
    instance.reset(new ProcessManager());
}

void ProcessManager::Done()
{
    instance.reset();
}

ProcessManager::ProcessManager() : machine(nullptr), maxProcs(cmajor::systemx::machine::MaxProcs()), numProcessTableSlots(maxProcs + 1), nextProcessId(1), numRunnableProcesses(0)
{
    if (maxProcs <= 0 || maxProcs > 64 * 1024)
    {
        throw SystemError(EPARAM, "invalid 'maxProcs' (" + std::to_string(maxProcs) + ") in '" + cmajor::systemx::machine::ConfigFilePath() +
            "': value should be in range 1..." + std::to_string(64 * 1024), __FUNCTION__);
    }
    processTable.resize(numProcessTableSlots);
}

ProcessManager::~ProcessManager()
{
}

Process* ProcessManager::GetProcess(int32_t pid) const
{
    if (pid < 1 || pid > maxProcs + 1)
    {
        throw SystemError(EPARAM, "invalid pid " + std::to_string(pid), __FUNCTION__);
    }
    return processTable[pid].get();
}

void ProcessManager::Start()
{
    processTable.clear();
    processTable.resize(numProcessTableSlots);
    nextProcessId = 1;
}

void ProcessManager::Stop()
{
    processTable.clear();
}

Process* ProcessManager::CreateProcess()
{
    if (!machine)
    {
        throw SystemError(EFAIL, "machine not set in process manager", __FUNCTION__);
    }
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), PROCESS_MANAGER, 0, NO_LOCK | CREATE_PROCESS);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), PROCESS_MANAGER, 0, HAS_LOCK | CREATE_PROCESS);
#endif 
    if (nextProcessId > maxProcs)
    {
        nextProcessId = 1;
    }
    while (nextProcessId <= maxProcs && processTable[nextProcessId])
    {
        if (processTable[nextProcessId]->State() == cmajor::systemx::machine::ProcessState::zombie)
        {
            DeleteProcess(nextProcessId);
            break;
        }
        ++nextProcessId;
    }
    if (nextProcessId > maxProcs)
    {
        throw SystemError(ELIMITEXCEEDED, "all process table entries in use", __FUNCTION__);
    }
    int32_t processId = nextProcessId++;
    Process* process = new Process(processId);
    processTable[processId].reset(process);
    return process;
}

void ProcessManager::SetMachine(cmajor::systemx::machine::Machine* machine_)
{
    machine = machine_;
    if (machine)
    {
        machine->AddObserver(this);
    }
}

void ProcessManager::DeleteProcess(int32_t pid)
{
    if (!machine)
    {
        throw SystemError(EFAIL, "machine not set in process manager", __FUNCTION__);
    }
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), PROCESS_MANAGER, pid, NO_LOCK | DELETE_PROCESS);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), PROCESS_MANAGER, pid, HAS_LOCK | DELETE_PROCESS);
#endif 
    Process* process = GetProcess(pid);
    if (process)
    {
        if (process->RV() != static_cast<uint64_t>(-1))
        {
            machine->Mem().FreeMemory(process->RV());
        }
        process->RemoveFromParent();
        process->DeleteKernelFiber();
        processTable[pid].reset();
    }
    else
    {
        throw SystemError(ENOTFOUND, "process with id " + std::to_string(pid) + " not found", __FUNCTION__);
    }
}

void ProcessManager::IncrementRunnableProcesses()
{
    ++numRunnableProcesses;
}

void ProcessManager::DecrementRunnableProcesses()
{
    --numRunnableProcesses;
    if (numRunnableProcesses == 0)
    {
        processesExitOrMachineStateChanged.notify_all();
    }
}

void ProcessManager::WaitForProcessesExit()
{
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), PROCESS_MANAGER, 0, NO_LOCK | WAIT_FOR_EXIT);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    processesExitOrMachineStateChanged.wait(lock, [this] { return numRunnableProcesses == 0 || machine->Exiting() || machine->HasException(); });
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), PROCESS_MANAGER, 0, HAS_LOCK | WAIT_FOR_EXIT);
#endif 
}

void ProcessManager::MachineStateChanged()
{
    if (machine->HasException() || machine->Exiting())
    {
        processesExitOrMachineStateChanged.notify_all();
    }
}

std::vector<Process*> ProcessManager::GetProcesses() const
{
    std::vector<Process*> processes;
    for (const auto& process : processTable)
    {
        if (process)
        {
            processes.push_back(process.get());
        }
    }
    return processes;
}

void InitProcessManager()
{
    ProcessManager::Init();
}

void DoneProcessManager()
{
    ProcessManager::Done();
}

} // namespace cmajor::systemx::kernel
