// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.kernel;

import cmajor.systemx.kernel.clock;
import cmajor.systemx.kernel.event.manager;
import cmajor.systemx.kernel.process.manager;
import cmajor.systemx.kernel.scheduler;
import cmajor.systemx.kernel.block.manager;
import cmajor.systemx.kernel.inode.manager;
import cmajor.systemx.kernel.host.file.manager;
import cmajor.systemx.kernel.io.manager;
import cmajor.systemx.kernel.mount;
import cmajor.systemx.kernel.root.fs;
import cmajor.systemx.kernel.host.fs;
import cmajor.systemx.kernel.dev.fs;
import cmajor.systemx.kernel.terminal;
import cmajor.systemx.kernel.debug;
import cmajor.systemx.kernel.error;

namespace cmajor::systemx::kernel {

void KernelProcess::Sleep()
{
    Kernel::Instance().Sleep();
}

void KernelProcess::Wakeup(cmajor::systemx::machine::Scheduler* scheduler)
{
    Kernel::Instance().Wakeup();
}

cmajor::systemx::machine::Processor* KernelProcess::GetProcessor() const
{
    return nullptr;
}

void KernelProcess::ReleaseProcessor(cmajor::systemx::machine::Processor* processor)
{
}

uint64_t KernelProcess::GetINodeKeyOfWorkingDir() const
{
    uint64_t rootDirINodeKey = Kernel::Instance().GetINodeKeyOfRootDir();
    return rootDirINodeKey;
}

void KernelProcess::SetINodeKeyOfWorkingDir(uint64_t inodeKeyAsULong)
{
    Kernel::Instance().SetINodeKeyOfRootDir(inodeKeyAsULong);
}

void KernelProcess::SetUID(int32_t uid)
{
    throw SystemError(EPERMISSION, "cannot set UID of kernel process", __FUNCTION__);
}

void KernelProcess::SetGID(int32_t gid)
{
    throw SystemError(EPERMISSION, "cannot set GID of kernel process", __FUNCTION__);
}

void KernelProcess::SetEUID(int32_t euid)
{
    throw SystemError(EPERMISSION, "cannot set EUID of kernel process", __FUNCTION__);
}

void KernelProcess::SetEGID(int32_t egid)
{
    throw SystemError(EPERMISSION, "cannot set EGID of kernel process", __FUNCTION__);
}

void KernelProcess::RemoveMessageQueue(int32_t md)
{
    // nothing todo
}

cmajor::systemx::machine::Machine* KernelProcess::GetMachine()
{
    return Kernel::Instance().GetMachine();
}

std::unique_ptr<Kernel> Kernel::instance;

void Kernel::Init()
{
    instance.reset(new Kernel());
}

void Kernel::Done()
{
    instance.reset();
}

Kernel::Kernel() : machine(nullptr), kernelProcess(new KernelProcess()), wakeup(false), inodeKeyOfRootDir()
{
}

cmajor::systemx::machine::Process* Kernel::GetKernelProcess() const
{
    return kernelProcess.get();
}

void Kernel::SetMachine(cmajor::systemx::machine::Machine* machine_)
{
    machine = machine_;
    Clock::Instance().SetMachine(machine);
    EventManager::Instance().SetMachine(machine);
    ProcessManager::Instance().SetMachine(machine);
    Scheduler::Instance().SetMachine(machine);
    SetBlockManagerMachine(machine);
    SetINodeManagerMachine(machine);
    SetHostFileManagerMachine(machine);
    SetIOManagerMachine(machine);
    SetTerminalMachine(machine);
}

void Kernel::Start()
{
    StartDebug();
    EventManager::Instance().Start();
    ProcessManager::Instance().Start();
    Scheduler::Instance().Start();
    Clock::Instance().Start();
    mountTable.reset(new MountTable(machine));
    RootFilesystem* rootFs = new RootFilesystem();
    mountTable->AddFilesystem(rootFs);
    HostFilesystem* hostFs = new HostFilesystem(hostFSNumber, std::string());
    mountTable->AddFilesystem(hostFs);
    DevFilesystem* devFs = new DevFilesystem();
    mountTable->AddFilesystem(devFs);
    StartBlockManager();
    StartINodeManager();
    StartHostFileManager();
    StartIOManager();
    hostFs->Initialize();
    rootFs->Initialize();
    StartTerminal();
}

void Kernel::Stop()
{
    StopTerminal();
    StopIOManager();
    StopHostFileManager();
    StopBlockManager();
    StopINodeManager();
    mountTable.reset();
    Clock::Instance().Stop();
    Scheduler::Instance().Stop();
    ProcessManager::Instance().Stop();
    EventManager::Instance().Stop();
    StopDebug();
}

void Kernel::Sleep()
{
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    sleepVar.wait(lock, [this] { return wakeup || machine->Exiting(); });
    wakeup = false;
}

void Kernel::Wakeup()
{
    wakeup = true;
    sleepVar.notify_one();
}

void InitKernel()
{
    Kernel::Init();
}

void DoneKernel()
{
    Kernel::Done();
}

} // namespace cmajor::systemx::kernel
