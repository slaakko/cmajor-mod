// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.process;

import cmajor.systemx.kernel.debug;
import cmajor.systemx.kernel.process.manager;
import cmajor.systemx.kernel.event.manager;
import cmajor.systemx.kernel.scheduler;
import cmajor.systemx.kernel.kernel;
import cmajor.systemx.kernel.mount;
import cmajor.systemx.kernel.fs;
import cmajor.systemx.kernel.io;
import cmajor.systemx.kernel.load;
import cmajor.systemx.kernel.debug.help;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

Process::Process(int32_t id_) :
    util::IntrusiveListNode<Process>(this), id(id_), rv(static_cast<uint64_t>(-1)), kernelSP(cmajor::systemx::machine::kernelBaseAddress), axAddress(0), bxAddress(0), cxAddress(0),
    state(cmajor::systemx::machine::ProcessState::created), entryPoint(-1), argumentsStartAddress(-1), argumentsLength(0), environmentStartAddress(-1), environmentLength(0),
    heapStartAddress(-1), heapLength(0), stackStartAddress(-1), startUserTime(), startSleepTime(), startSystemTime(), userTime(0), sleepTime(0), systemTime(0),
    exitCode(0), debugger(nullptr), processor(nullptr), kernelProcessor(nullptr), interruptHandler(nullptr), currentExceptionAddress(0), currentExceptionClassId(0), 
    currentTryRecord(nullptr), mainFiber(nullptr), kernelFiber(nullptr), inodeKeyOfWorkingDirAsULong(-1), uid(0), gid(0), euid(0), egid(0), umask(0), directoriesChanged(false),
    inKernel(false), saveContext(false), regAX(0), useRegAX(false)
{
    SetINodeKeyOfWorkingDir(Kernel::Instance().GetINodeKeyOfRootDir());
}

int32_t Process::CoreId() const
{
    int coreId = -1;
    if (processor)
    {
        coreId = processor->Id();
    }
    return coreId;
}

void Process::SetState(cmajor::systemx::machine::ProcessState state_)
{
    cmajor::systemx::machine::Machine* machine = Kernel::Instance().GetMachine();
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    state = state_;
    if (InDebugMode(debugStateMode))
    {
        std::string line;
        int coreId = CoreId();
        if (coreId != -1)
        {
            line.append("core=").append(std::to_string(coreId)).append(1, ' ');
        }
        line.append("pid=");
        line.append(util::Format(std::to_string(id), 3, util::FormatWidth::min, util::FormatJustify::right, '0'));
        line.append(" prog=").append(filePath).append(" state=").append(cmajor::systemx::machine::ProcessStateStr(state));
        DebugWrite(line);
    }
}

void Process::SetFilePath(const std::string& filePath_)
{
    filePath = filePath_;
#if (LOCK_DEBUG)
    DebugLock create(nullptr, PROCESS_MANAGER, Id(), SET_FILE_PATH_PROCESS, filePath);
#endif
}

void Process::SetHeapLength(int64_t heapLength_)
{
    heapLength = heapLength_;
    Region poolRegion = regionTable.GetRegion(RegionId::pool);
    if (poolRegion.Valid())
    {
        int64_t heapEnd = heapStartAddress + heapLength;
        int64_t totalPoolLength = heapEnd - poolRegion.Start();
        Region newPoolRegion(RegionId::pool, poolRegion.Start(), totalPoolLength);
        regionTable.SetRegion(newPoolRegion);
    }
}

void Process::SetAddressesFrom(Process* parent)
{
    entryPoint = parent->entryPoint;
    argumentsStartAddress = parent->argumentsStartAddress;
    argumentsLength = parent->argumentsLength;
    environmentStartAddress = parent->environmentStartAddress;
    environmentLength = parent->environmentLength;
    heapStartAddress = parent->heapStartAddress;
    heapLength = parent->heapLength;
    stackStartAddress = parent->stackStartAddress;
}

void Process::SetSymbolTable(const std::shared_ptr<cmajor::systemx::object::SymbolTable>& symbolTable_)
{
    symbolTable = symbolTable_;
}

cmajor::systemx::object::FunctionTable* Process::GetFunctionTable()
{
    if (!functionTable)
    {
        functionTable.reset(new cmajor::systemx::object::FunctionTable());
    }
    return functionTable.get();
}

void Process::RemoveFromParent()
{
    if (Parent())
    {
        Parent()->RemoveChild(this);
    }
}

void Process::SetStartUserTime()
{
    startUserTime = std::chrono::steady_clock::now();
}

void Process::SetStartSleepTime()
{
    startSleepTime = std::chrono::steady_clock::now();
}

void Process::SetStartSystemTime()
{
    startSystemTime = std::chrono::steady_clock::now();
}

void Process::AddUserTime()
{
    std::chrono::steady_clock::time_point endUserTime = std::chrono::steady_clock::now();
    userTime = userTime + (endUserTime - startUserTime);
}

void Process::AddSleepTime()
{
    std::chrono::steady_clock::time_point endSleepTime = std::chrono::steady_clock::now();
    sleepTime = sleepTime + (endSleepTime - startSleepTime);
}

void Process::AddSystemTime()
{
    std::chrono::steady_clock::time_point endSystemTime = std::chrono::steady_clock::now();
    systemTime = systemTime + (endSystemTime - startSystemTime);
}

void Process::AddChildUserTime(const std::chrono::steady_clock::duration& childUserTime_)
{
    childUserTime = childUserTime + childUserTime_;
}

void Process::AddChildSleepTime(const std::chrono::steady_clock::duration& childSleepTime_)
{
    childSleepTime = childSleepTime + childSleepTime_;
}

void Process::AddChildSystemTime(const std::chrono::steady_clock::duration& childSystemTime_)
{
    childSystemTime = childSystemTime + childSystemTime_;
}

void Process::SetError(const SystemError& error_)
{
    error = error_;
}

void Process::SetUID(int32_t uid_)
{
    if (uid == 0 || euid == 0)
    {
        uid = uid_;
        euid = uid_;
    }
    else
    {
        throw SystemError(EPERMISSION, "unauthorized", __FUNCTION__);
    }
}

void Process::SetEUID(int32_t euid_)
{
    if (uid == 0 || euid == 0)
    {
        euid = euid_;
    }
}

void Process::SetEUIDTrusted(int32_t euid_)
{
    euid = euid_;
}

void Process::SetGID(int32_t gid_)
{
    if (uid == 0 || euid == 0 || gid == 0 || egid == 0)
    {
        gid = gid_;
        egid = gid_;
    }
    else
    {
        throw SystemError(EPERMISSION, "unauthorized", __FUNCTION__);
    }
}

void Process::SetEGID(int32_t egid_)
{
    if (uid == 0 || euid == 0)
    {
        egid = egid_;
    }
}

void Process::SetEGIDTrusted(int32_t egid_)
{
    egid = egid_;
}

void Process::WaitNotInKernel(bool enter)
{
    cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    notInKernelVar.wait(lock, [&] { return !inKernel || machine->Exiting(); });
    if (enter)
    {
        inKernel = true;
    }
}

void Process::SetInKernel()
{
    cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    inKernel = true;
}

void Process::SetNotInKernel()
{ 
    cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    inKernel = false;
    notInKernelVar.notify_all();
}

bool Process::DoSaveContext() const
{
    cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    return saveContext;
}

void Process::SetSaveContext(bool saveContext_)
{
    cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    saveContext = saveContext_;
}

void Process::SetRegAX(uint64_t regAX_)
{
    cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    regAX = regAX_;
}

void Process::SetUseRegAX()
{
    cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
    useRegAX = true;
}

void Process::Exit(uint8_t exitCode_)
{
    cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), PROCESS_MANAGER, Id(), NO_LOCK | EXIT);
#endif 
    std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), PROCESS_MANAGER, Id(), HAS_LOCK | EXIT);
#endif 
    SetExitCode(exitCode_);
    SetState(cmajor::systemx::machine::ProcessState::zombie);
    if (debugger)
    {
        debugger->ProcessExit();
    }
    else
    {
        if (rv != static_cast<uint64_t>(-1))
        {
            uint64_t rv_ = rv;
            rv = static_cast<uint64_t>(-1);
            cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
            machine->Mem().FreeMemory(rv_);
        }
    }
    symbolTable.reset();
    functionTable.reset();
    fileTable.CloseFiles(this);
    RemoveDirChangeNotifications(id);
    messageQueues.CloseAll(this);
    Process* parent = Parent();
    if (parent)
    {
        cmajor::systemx::kernel::Wakeup(this, cmajor::systemx::machine::Event(cmajor::systemx::machine::EventKind::childExitEvent, parent->Id()));
    }
    ProcessManager::Instance().DecrementRunnableProcesses();
}

void Process::DeleteKernelFiber()
{
    if (kernelFiber)
    {
        util::DeleteFiber(kernelFiber);
        kernelFiber = nullptr;
    }
}

void Process::SaveContext(cmajor::systemx::machine::Machine& machine, cmajor::systemx::machine::Registers& regs)
{
    cmajor::systemx::machine::Memory& mem = machine.Mem();
    kernelSP = cmajor::systemx::machine::kernelBaseAddress;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regSP), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regFP), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetPC(), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    axAddress = kernelSP;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regAX), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    bxAddress = kernelSP;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regBX), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    cxAddress = kernelSP;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regCX), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regDX), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regEX), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regIX), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regP0), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regP1), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regP2), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regP3), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regP4), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regP5), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regP6), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.Get(cmajor::systemx::machine::regP7), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    int numLocalRegs = cmajor::systemx::machine::NumLocalRegs();
    for (int i = 0; i < numLocalRegs; ++i)
    {
        uint8_t regNum = static_cast<uint8_t>(i);
        mem.WriteOcta(rv, kernelSP, regs.Get(regNum), cmajor::systemx::machine::Protection::write);
        kernelSP = kernelSP + 8;
    }
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rA), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rB), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rD), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rE), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rH), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rP), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rR), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rW), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rX), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rY), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rZ), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rL), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
    mem.WriteOcta(rv, kernelSP, regs.GetSpecial(cmajor::systemx::machine::rG), cmajor::systemx::machine::Protection::write);
    kernelSP = kernelSP + 8;
}

void Process::RestoreContext(cmajor::systemx::machine::Machine& machine, cmajor::systemx::machine::Registers& regs)
{
    cmajor::systemx::machine::Memory& mem = machine.Mem();
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rG, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rL, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rZ, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rY, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rX, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rW, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rR, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rP, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rH, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rE, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rD, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rB, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.SetSpecial(cmajor::systemx::machine::rA, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    int numLocalRegs = cmajor::systemx::machine::NumLocalRegs();
    for (int i = numLocalRegs - 1; i >= 0; --i)
    {
        kernelSP = kernelSP - 8;
        uint8_t regNum = static_cast<uint8_t>(i);
        regs.Set(regNum, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    }
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regP7, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regP6, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regP5, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regP4, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regP3, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regP2, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regP1, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regP0, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regIX, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regEX, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regDX, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regCX, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regBX, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regAX, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    if (useRegAX)
    {
        regs.Set(cmajor::systemx::machine::regAX, regAX);
        useRegAX = false;
    }
    regs.SetPC(mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regFP, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    kernelSP = kernelSP - 8;
    regs.Set(cmajor::systemx::machine::regSP, mem.ReadOcta(rv, kernelSP, cmajor::systemx::machine::Protection::read));
    regs.SetSpecial(cmajor::systemx::machine::rV, rv);
}

void Process::SetRunning(cmajor::systemx::machine::Processor* processor_)
{
    cmajor::systemx::machine::Machine* machine = Kernel::Instance().GetMachine();
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    processor = processor_;
    SetState(cmajor::systemx::machine::ProcessState::running);
}

void Process::SetKernelProcessor(cmajor::systemx::machine::Processor* kernelProcessor_)
{
    cmajor::systemx::machine::Machine* machine = Kernel::Instance().GetMachine();
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    kernelProcessor = kernelProcessor_;
}

cmajor::systemx::machine::Processor* Process::KernelProcessor() const
{
    cmajor::systemx::machine::Machine* machine = Kernel::Instance().GetMachine();
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    return kernelProcessor;
}

void Process::ResetProcessor()
{
    cmajor::systemx::machine::Machine* machine = Kernel::Instance().GetMachine();
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    processor = nullptr;
}

void Process::SetProcessor(cmajor::systemx::machine::Processor* processor_) 
{ 
    cmajor::systemx::machine::Machine* machine = Kernel::Instance().GetMachine();
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    processor = processor_; 
}

cmajor::systemx::machine::Debugger* Process::GetDebugger() const
{
    cmajor::systemx::machine::Machine* machine = Kernel::Instance().GetMachine();
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    return debugger;
}

void Process::SetDebugger(cmajor::systemx::machine::Debugger* debugger_)
{
    cmajor::systemx::machine::Machine* machine = Kernel::Instance().GetMachine();
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    debugger = debugger_;
}

void Process::SetUMask(int32_t mask)
{
    umask = mask & 0777;
}

void Process::RemoveMessageQueue(int32_t md)
{
    messageQueues.Remove(md);
}

cmajor::systemx::machine::Machine* Process::GetMachine()
{
    return processor->GetMachine();
}

void SetupRegions(Process* parent, Process* child)
{
    Region textRegion = parent->GetRegionTable().GetRegion(RegionId::text);
    ShareRegion(textRegion, parent, child);
    Region dataRegion = parent->GetRegionTable().GetRegion(RegionId::data);
    CopyRegion(dataRegion, parent, child);
    Region stackRegion = parent->GetRegionTable().GetRegion(RegionId::stack);
    uint64_t sp = parent->GetProcessor()->Regs().Get(cmajor::systemx::machine::regSP);
    int64_t stackSize = sp - stackRegion.Start();
    stackRegion.SetLength(stackSize);
    CopyRegion(stackRegion, parent, child);
    Region poolRegion = parent->GetRegionTable().GetRegion(RegionId::pool);
    CopyRegion(poolRegion, parent, child);
}

int32_t Fork(Process* parent)
{
    cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
    cmajor::systemx::machine::Processor* processor = parent->GetProcessor();
#if (LOCK_DEBUG)
    DebugLock startDebugLock(&machine->Lock(), PROCESS_MANAGER, parent->Id(), NO_LOCK | FORK);
#endif 
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
    DebugLock hasDebugLock(&machine->Lock(), PROCESS_MANAGER, parent->Id(), HAS_LOCK | FORK);
#endif 
    Process* child = ProcessManager::Instance().CreateProcess();
    child->SetProcessor(processor);
    uint64_t rv = machine->Mem().AllocateTranslationMap();
    child->SetRV(rv);
    child->SetUID(parent->UID());
    child->SetEUID(parent->EUID());
    child->SetGID(parent->GID());
    child->SetEGID(parent->EGID());
    child->SetUMask(parent->UMask());
    TextSegmentWriteProtectionGuard guard(rv, machine->Mem());
    SetupRegions(parent, child);
    child->GetFileTable().CopyFrom(parent->GetFileTable());
    child->SetAddressesFrom(parent);
    child->SetSymbolTable(parent->GetSymbolTablePtr());
    child->SetINodeKeyOfWorkingDir(parent->GetINodeKeyOfWorkingDir());
    parent->AddChild(child);
    uint64_t regAX = processor->Regs().Get(cmajor::systemx::machine::regAX);
    processor->Regs().Set(cmajor::systemx::machine::regAX, 0);
    uint64_t regRV = processor->Regs().GetSpecial(cmajor::systemx::machine::rV);
    processor->Regs().SetSpecial(cmajor::systemx::machine::rV, rv);
    child->SaveContext(*machine, processor->Regs());
    processor->Regs().SetSpecial(cmajor::systemx::machine::rV, regRV);
    processor->Regs().Set(cmajor::systemx::machine::regAX, regAX);
    ProcessManager::Instance().IncrementRunnableProcesses();
    Scheduler::Instance().AddRunnableProcess(child, cmajor::systemx::machine::ProcessState::runnableInUser);
    return child->Id();
}

int32_t Wait(Process* parent, int32_t pid, int64_t childExitCodeAddress, SystemError& error)
{
    while (true)
    {
        cmajor::systemx::machine::Machine* machine = ProcessManager::Instance().GetMachine();
#if (LOCK_DEBUG)
        DebugLock startDebugLock(&machine->Lock(), PROCESS_MANAGER, parent->Id(), NO_LOCK | WAIT);
#endif 
        std::unique_lock<std::recursive_mutex> lock(machine->Lock());
#if (LOCK_DEBUG)
        DebugLock hasDebugLock(&machine->Lock(), PROCESS_MANAGER, parent->Id(), HAS_LOCK | WAIT);
#endif
        Process* child = parent->FirstChild();
        while (child)
        {
            if (child->State() == cmajor::systemx::machine::ProcessState::zombie)
            {
                if (pid == -1 || child->Id() == pid)
                {
                    child->RemoveFromParent();
                    uint8_t exitCode = child->ExitCode();
                    parent->GetProcessor()->GetMachine()->Mem().WriteByte(parent->RV(), childExitCodeAddress, exitCode, cmajor::systemx::machine::Protection::write);
                    parent->AddChildUserTime(child->UserTime());
                    parent->AddChildSleepTime(child->SleepTime());
                    parent->AddChildSystemTime(child->SystemTime());
                    return child->Id();
                }
            }
            child = child->NextSibling();
        }
        child = parent->FirstChild();
        if (child)
        {
            Sleep(cmajor::systemx::machine::Event(cmajor::systemx::machine::EventKind::childExitEvent, parent->Id()), parent, lock);
            lock.lock();
        }
        bool hasChildInZombieState = false;
        child = parent->FirstChild();
        while (child)
        {
            if (child->State() == cmajor::systemx::machine::ProcessState::zombie)
            {
                hasChildInZombieState = true;
                if (pid == -1 || child->Id() == pid)
                {
                    child->RemoveFromParent();
                    uint8_t exitCode = child->ExitCode();
                    machine->Mem().WriteByte(parent->RV(), childExitCodeAddress, exitCode, cmajor::systemx::machine::Protection::write);
                    parent->AddChildUserTime(child->UserTime());
                    parent->AddChildSleepTime(child->SleepTime());
                    parent->AddChildSystemTime(child->SystemTime());
                    return child->Id();
                }
            }
            child = child->NextSibling();
        }
        if (pid == -1 || !hasChildInZombieState)
        {
            break;
        }
    }
    error = SystemError(ENOCHILD, "no child in zombie state", __FUNCTION__);
    return -1;
}

void Exec(Process* process, int64_t filePathAddress, int64_t argvAddress, int64_t envpAddress)
{
    cmajor::systemx::machine::Machine* machine = process->GetProcessor()->GetMachine();
    cmajor::systemx::machine::Memory& mem = machine->Mem();
    std::string filePath = ReadString(process, filePathAddress, mem);
    Filesystem* fs = GetFs(rootFSNumber);
    INodePtr inodePtr = PathToINode(filePath, fs, process);
    INode* inode = inodePtr.Get();
    if (!inode)
    {
        throw SystemError(ENOTFOUND, "could not execute: path '" + filePath + "' not found", __FUNCTION__);
    }
    if (!filePath.starts_with("/mnt/sx/bin"))
    {
        CheckAccess(Access::execute, process->EUID(), process->EGID(), inode, "could not execute '" + filePath + "'");
    }
    if (inode->SetUIDBit())
    {
        process->SetEUIDTrusted(inode->UID());
    }
    if (inode->SetGIDBit())
    {
        process->SetEGIDTrusted(inode->GID());
    }
    process->SetFilePath(filePath);
    std::vector<uint8_t> content = ReadFile(process, filePathAddress);
    std::vector<std::string> args = ReadStringPointerArray(process, argvAddress, mem);
    std::vector<std::string> env = ReadStringPointerArray(process, envpAddress, mem);
    mem.FreeMemory(process->RV());
    mem.AllocateTranslationMap(process->RV());
    process->GetRegionTable().FreeRegions();
    process->SetState(cmajor::systemx::machine::ProcessState::exec);
    util::MemoryStream memoryStream(content.data(), content.size());
    util::BinaryStreamReader reader(memoryStream);
    std::unique_ptr<cmajor::systemx::object::BinaryFile> binaryFile(cmajor::systemx::object::ReadBinaryFile(reader, filePath));
    Load(process, binaryFile.get(), args, env, *machine, process->RV(), false);
}

void Kill(Process* parent, Process* process)
{
    bool allow = false;
    if (parent->EUID() == 0 || parent->UID() == 0)
    {
        allow = true;
    }
    else
    {
        Process* par = process->Parent();
        while (par && par != parent)
        {
            par = par->Parent();
        }
        if (par == parent)
        {
            allow = true;
        }
    }
    if (allow)
    {
        process->Exit(255);
    }
    else
    {
        throw SystemError(EPERMISSION, "unauthorized", __FUNCTION__);
    }
}

} // namespace cmajor::systemx::kernel
