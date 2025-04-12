// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.machine.processor;

import cmajor.systemx.machine.registers;
import cmajor.systemx.machine.process;
import std.core;

export namespace cmajor::systemx::machine {

class Machine;
class UserProcess;
class Instruction;
class InterruptHandler;

class Processor
{
public:
    Processor();
    void SetId(int id_) { id = id_; }
    Machine* GetMachine() const { return machine; }
    void SetMachine(Machine* machine_) { machine = machine_; }
    Registers& Regs() { return registers; }
    void Start();
    void Stop();
    void Run();
    void EnableInterrupts();
    UserProcess* CurrentProcess() const { return currentProcess; }
    void ResetCurrentProcess(bool addSystemTime, bool saveContext, bool setKernelProcessor);
    void CheckException();
    void* MainFiber() const { return mainFiber; }
    void SetException(std::exception_ptr exception_) { exception = exception_; }
    void AddRunnableKernelProcess(UserProcess* runnableKernelProcess);
    UserProcess* GetRunnableKernelProcess();
    bool HasRunnableKernelProcess();
private:
    Instruction* FetchInstruction(uint64_t& pc, uint8_t& x, uint8_t& y, uint8_t& z);
    void SetPC(Instruction* inst, uint64_t pc, uint64_t prevPC);
    bool CheckInterrupts();
    int id;
    Machine* machine;
    Registers registers;
    UserProcess* currentProcess;
    std::thread thread;
    std::exception_ptr exception;
    int kernelStackSize;
    void* mainFiber;
    UserProcessList runnableKernelProcesses;
};

} // namespace cmajor::systemx::machine
