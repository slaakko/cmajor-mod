// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.kernel.process;

import cmajor.systemx.kernel.error;
import cmajor.systemx.kernel.file;
import cmajor.systemx.kernel.region;
import cmajor.systemx.kernel.inode.manager;
import cmajor.systemx.kernel.msg.queue;
import cmajor.systemx.object;
import cmajor.systemx.machine;
import util;
import std.core;

export namespace cmajor::systemx::kernel {

class Process : public util::IntrusiveListNode<Process>, public cmajor::systemx::machine::UserProcess
{
public:
    Process(int32_t id_);
    int32_t Id() const override { return id; }
    int32_t CoreId() const override;
    uint64_t RV() const override { return rv; }
    void SetRV(uint64_t rv_) { rv = rv_; }
    uint64_t AXAddress() const { return axAddress; }
    uint64_t BXAddress() const { return bxAddress; }
    uint64_t CXAddress() const { return cxAddress; }
    std::string FilePath() const override { return filePath; }
    void SetFilePath(const std::string& filePath_);
    cmajor::systemx::machine::ProcessState State() const override { return state; }
    void SetState(cmajor::systemx::machine::ProcessState state_);
    int64_t EntryPoint() const { return entryPoint; }
    void SetEntryPoint(int64_t entryPoint_) { entryPoint = entryPoint_; }
    int64_t ArgumentsStartAddress() const { return argumentsStartAddress; }
    void SetArgumentsStartAddress(int64_t argumentsStartAddress_) { argumentsStartAddress = argumentsStartAddress_; }
    int64_t ArgumentsLength() const { return argumentsLength; }
    void SetArgumentsLength(int64_t argumentsLength_) { argumentsLength = argumentsLength_; }
    int64_t EnvironmentStartAddress() const { return environmentStartAddress; }
    void SetEnvironmentStartAddress(int64_t environmentStartAddress_) { environmentStartAddress = environmentStartAddress_; }
    int64_t EnvironmentLength() const { return environmentLength; }
    void SetEnvironmentLength(int64_t environmentLength_) { environmentLength = environmentLength_; }
    int64_t HeapStartAddress() const override { return heapStartAddress; }
    void SetHeapStartAddress(int64_t heapStartAddress_) { heapStartAddress = heapStartAddress_; }
    int64_t HeapLength() const override { return heapLength; }
    void SetHeapLength(int64_t heapLength_) override;
    void SetAddressesFrom(Process* parent);
    cmajor::systemx::object::SymbolTable* GetSymbolTable() const { return symbolTable.get(); }
    const std::shared_ptr<cmajor::systemx::object::SymbolTable>& GetSymbolTablePtr() const { return symbolTable; }
    void SetSymbolTable(const std::shared_ptr<cmajor::systemx::object::SymbolTable>& symbolTable_);
    cmajor::systemx::object::FunctionTable* GetFunctionTable();
    cmajor::systemx::object::TypeTable* GetTypeTable();
    void RemoveFromParent();
    uint8_t ExitCode() const { return exitCode; }
    void SetExitCode(uint8_t exitCode_) { exitCode = exitCode_; }
    void Exit(uint8_t exitCode_) override;
    void SaveContext(cmajor::systemx::machine::Machine& machine, cmajor::systemx::machine::Registers& regs) override;
    void RestoreContext(cmajor::systemx::machine::Machine& machine, cmajor::systemx::machine::Registers& regs) override;
    void SetRunning(cmajor::systemx::machine::Processor* processor_) override;
    void SetKernelProcessor(cmajor::systemx::machine::Processor* kernelProcessor_) override;
    cmajor::systemx::machine::Processor* KernelProcessor() const override;
    void ResetProcessor() override;
    cmajor::systemx::machine::Debugger* GetDebugger() const override;
    void SetDebugger(cmajor::systemx::machine::Debugger* debugger_) override;
    cmajor::systemx::machine::Processor* GetProcessor() const override { return processor; }
    void SetProcessor(cmajor::systemx::machine::Processor* processor_) override;
    void SetStartUserTime() override;
    void SetStartSleepTime() override;
    void SetStartSystemTime() override;
    void AddUserTime() override;
    void AddSleepTime() override;
    void AddSystemTime() override;
    void AddChildUserTime(const std::chrono::steady_clock::duration& childUserTime_);
    void AddChildSleepTime(const std::chrono::steady_clock::duration& childSleepTime_);
    void AddChildSystemTime(const std::chrono::steady_clock::duration& childSystemTime_);
    const std::chrono::steady_clock::duration& UserTime() const { return userTime; }
    const std::chrono::steady_clock::duration& SleepTime() const { return sleepTime; }
    const std::chrono::steady_clock::duration& SystemTime() const { return systemTime; }
    const std::chrono::steady_clock::duration& ChildUserTime() const { return childUserTime; }
    const std::chrono::steady_clock::duration& ChildSleepTime() const { return childSleepTime; }
    const std::chrono::steady_clock::duration& ChildSystemTime() const { return childSystemTime; }
    int32_t UID() const override { return uid; }
    void SetUID(int32_t uid_) override;
    int32_t EUID() const override { return euid; }
    void SetEUID(int32_t euid_);
    void SetEUIDTrusted(int32_t euid_);
    int32_t GID() const override { return gid; }
    void SetGID(int32_t gid_) override;
    int32_t EGID() const override { return egid; }
    void SetEGID(int32_t egid_);
    void SetEGIDTrusted(int32_t egid_);
    RegionTable& GetRegionTable() { return regionTable; }
    ProcessFileTable& GetFileTable() { return fileTable; }
    void SetError(const SystemError& error_);
    SystemError GetError() const { return error; }
    uint64_t CurrentExceptionAddress() const { return currentExceptionAddress; }
    void SetCurrentExceptionAddress(uint64_t exceptionAddress) { currentExceptionAddress = exceptionAddress; }
    uint64_t CurrentExceptionClassId() const { return currentExceptionClassId; }
    void SetCurrentExceptionClassId(uint64_t exceptionClassId) { currentExceptionClassId = exceptionClassId; }
    cmajor::systemx::object::TryRecord* CurrentTryRecord() const { return currentTryRecord; }
    void SetCurrentTryRecord(cmajor::systemx::object::TryRecord* tryRecord) { currentTryRecord = tryRecord; }
    void SetKernelFiber(void* kernelFiber_) override { kernelFiber = kernelFiber_; }
    void* KernelFiber() const override { return kernelFiber; }
    void SetMainFiber(void* mainFiber_) override { mainFiber = mainFiber_; }
    void* MainFiber() const { return mainFiber; }
    void DeleteKernelFiber();
    uint64_t GetINodeKeyOfWorkingDir() const override { return inodeKeyOfWorkingDirAsULong; }
    void SetINodeKeyOfWorkingDir(uint64_t inodeKeyOfWorkingDirAsULong_) override { inodeKeyOfWorkingDirAsULong = inodeKeyOfWorkingDirAsULong_; }
    void SetUMask(int32_t mask);
    int32_t UMask() const override { return umask; }
    void SetDirectoriesChanged() { directoriesChanged = true; }
    bool DirectoriesChanged() const { return directoriesChanged; }
    void ClearDirectoriesChanged() { directoriesChanged = false; }
    ProcessMessageQueues& MessageQueues() { return messageQueues; }
    void RemoveMessageQueue(int32_t md) override;
    cmajor::systemx::machine::Machine* GetMachine() override;
    cmajor::systemx::machine::InterruptHandler* GetInterruptHandler() const override { return interruptHandler;  }
    void SetInterruptHandler(cmajor::systemx::machine::InterruptHandler* interruptHandler_) override { interruptHandler = interruptHandler_; }
    bool InKernel() const override { return inKernel; }
    void SetInKernel() override;
    void SetNotInKernel() override;
    void WaitNotInKernel(bool enter) override;
    bool DoSaveContext() const override;
    void SetSaveContext(bool saveContext_) override;
    void SetRegAX(uint64_t regAX_) override;
    void SetUseRegAX() override;
private:
    int32_t id;
    uint64_t rv;
    uint64_t kernelSP;
    uint64_t axAddress;
    uint64_t bxAddress;
    uint64_t cxAddress;
    std::string filePath;
    cmajor::systemx::machine::ProcessState state;
    int64_t entryPoint;
    int64_t argumentsStartAddress;
    int64_t argumentsLength;
    int64_t environmentStartAddress;
    int64_t environmentLength;
    int64_t heapStartAddress;
    int64_t heapLength;
    int64_t stackStartAddress;
    uint8_t exitCode;
    int32_t uid;
    int32_t gid;
    int32_t euid;
    int32_t egid;
    int32_t umask;
    std::chrono::steady_clock::time_point startUserTime;
    std::chrono::steady_clock::time_point startSleepTime;
    std::chrono::steady_clock::time_point startSystemTime;
    std::chrono::steady_clock::duration userTime;
    std::chrono::steady_clock::duration sleepTime;
    std::chrono::steady_clock::duration systemTime;
    std::chrono::steady_clock::duration childUserTime;
    std::chrono::steady_clock::duration childSleepTime;
    std::chrono::steady_clock::duration childSystemTime;
    std::shared_ptr<cmajor::systemx::object::SymbolTable> symbolTable;
    cmajor::systemx::machine::Debugger* debugger;
    cmajor::systemx::machine::Processor* processor;
    cmajor::systemx::machine::Processor* kernelProcessor;
    cmajor::systemx::machine::InterruptHandler* interruptHandler;
    RegionTable regionTable;
    ProcessFileTable fileTable;
    std::unique_ptr<cmajor::systemx::object::FunctionTable> functionTable;
    std::unique_ptr<cmajor::systemx::object::TypeTable> typeTable;
    SystemError error;
    uint64_t currentExceptionAddress;
    uint64_t currentExceptionClassId;
    cmajor::systemx::object::TryRecord* currentTryRecord;
    void* kernelFiber;
    void* mainFiber;
    uint64_t inodeKeyOfWorkingDirAsULong;
    bool directoriesChanged;
    ProcessMessageQueues messageQueues;
    bool inKernel;
    std::condition_variable_any notInKernelVar;
    bool saveContext;
    uint64_t regAX;
    bool useRegAX;
};

int32_t Fork(Process* parent);
int32_t Wait(Process* parent, int32_t pid, int64_t childExitCodeAddress, SystemError& error);
void Exec(Process* process, int64_t filePathAddress, int64_t argvAddress, int64_t envpAddress);
void Kill(Process* parent, Process* process);

} // namespace cmajor::systemx::kernel
