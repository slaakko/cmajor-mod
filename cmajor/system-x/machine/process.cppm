// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <boost/pool/pool_alloc.hpp>

export module cmajor.systemx.machine.process;

export namespace cmajor::systemx::machine {

class Machine;
class Processor;
class Registers;
class Scheduler;
class Debugger;

enum class ProcessState
{
    created, exec, asleep, runnableInUser, runnableInKernel, running, zombie
};

class Process
{
public:
    virtual ~Process();
    virtual int32_t Id() const = 0;
    virtual std::string FilePath() const = 0;
    virtual void Sleep() = 0;
    virtual void Wakeup(Scheduler* scheduler) = 0;
    virtual Processor* GetProcessor() const = 0;
    virtual void ReleaseProcessor(Processor* processor) = 0;
    virtual uint64_t GetINodeKeyOfWorkingDir() const = 0;
    virtual void SetINodeKeyOfWorkingDir(uint64_t inodeKeyAsULong) = 0;
    virtual int32_t UID() const = 0;
    virtual int32_t EUID() const = 0;
    virtual int32_t GID() const = 0;
    virtual int32_t EGID() const = 0;
    virtual void SetUID(int32_t uid) = 0;
    virtual void SetEUID(int32_t euid) = 0;
    virtual void SetGID(int32_t gid) = 0;
    virtual void SetEGID(int32_t egid) = 0;
    virtual int32_t UMask() const = 0;
    virtual Machine* GetMachine() = 0;
    virtual void RemoveMessageQueue(int32_t md) = 0;
};

using ProcessList = std::list<Process*, boost::fast_pool_allocator<Process*>>;

class UserProcess : public Process
{
public:
    void Sleep() override;
    void Wakeup(Scheduler* scheduler) override;
    void ReleaseProcessor(Processor* processor) override;
    virtual uint64_t RV() const = 0;
    virtual void SaveContext(Machine& machine, Registers& regs) = 0;
    virtual void RestoreContext(Machine& machine, Registers& regs) = 0;
    virtual ProcessState State() const = 0;
    virtual void SetState(ProcessState state) = 0;
    virtual void Exit(uint8_t exitCode) = 0;
    virtual void SetRunning(Processor* processor) = 0;
    virtual void ResetProcessor() = 0;
    virtual Debugger* GetDebugger() const = 0;
    virtual void SetDebugger(Debugger* debugger) = 0;
    virtual void SetStartUserTime() = 0;
    virtual void SetStartSleepTime() = 0;
    virtual void SetStartSystemTime() = 0;
    virtual void AddUserTime() = 0;
    virtual void AddSleepTime() = 0;
    virtual void AddSystemTime() = 0;
    virtual int64_t HeapStartAddress() const = 0;
    virtual int64_t HeapLength() const = 0;
    virtual void SetHeapLength(int64_t heapLength) = 0;
    virtual void* KernelFiber() const = 0;
    virtual void SetKernelFiber(void* kernelFiber) = 0;
};

using UserProcessList = std::list<UserProcess*, boost::fast_pool_allocator<UserProcess*>>;

} // namespace cmajor::systemx::machine
