// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.machine.machine;

import cmajor.systemx.machine.process;
import cmajor.systemx.machine.clock;
import cmajor.systemx.machine.memory;
import cmajor.systemx.machine.instruction;
import std.core;

export namespace cmajor::systemx::machine {

class Scheduler
{
public:
    virtual ~Scheduler();
    virtual void SetMachine(Machine* machine_) = 0;
    virtual UserProcess* GetRunnableProcess() = 0;
    virtual void AddRunnableProcess(UserProcess* process, ProcessState processState) = 0;
    virtual void CheckRunnable() = 0;
    virtual void Stop() = 0;
};

class MachineObserver
{
public:
    virtual ~MachineObserver();
    virtual void MachineStateChanged() = 0;
};

class Machine
{
public:
    Machine();
    Machine(const Machine&) = delete;
    Machine& operator=(const Machine&) = delete;
    ~Machine();
    Clock& GetClock() { return clock; }
    Memory& Mem() { return memory; }
    void SetScheduler(Scheduler* scheduler_) { scheduler = scheduler_; }
    Scheduler* GetScheduler() const { return scheduler; }
    Instruction* GetInstruction(uint8_t opCode) const { return insts[opCode]; }
    std::recursive_mutex& Lock() { return lock; }
    void Start();
    void Exit();
    void SetExiting();
    bool Exiting() const { return exiting; }
    bool HasException() const { return hasException; }
    void SetHasException();
    void SetException(std::exception_ptr&& exception_);
    std::vector<Processor>& Processors() { return processors; }
    void CheckExceptions();
    void AddObserver(MachineObserver* observer);
    void NotifyObservers();
private:
    void SetInstruction(Instruction* inst);
    std::vector<Processor> processors;
    Clock clock;
    Scheduler* scheduler;
    Memory memory;
    bool exiting;
    std::vector<std::unique_ptr<Instruction>> instructions;
    Instruction* insts[256];
    std::recursive_mutex lock;
    bool hasException;
    std::exception_ptr exception;
    std::vector<MachineObserver*> observers;
};

} // namespace cmajor::systemx::machine
