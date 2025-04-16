// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.machine.processor;

import cmajor.systemx.machine.machine;
import cmajor.systemx.machine.instruction;
import cmajor.systemx.machine.interrupt;
import cmajor.systemx.machine.debugger;
import cmajor.systemx.machine.config;
import util;

namespace cmajor::systemx::machine {

void RunKernel();

Scheduler::~Scheduler()
{
}

Processor::Processor() : 
    id(0), machine(nullptr), currentProcess(nullptr), kernelStackSize(KernelStackSize()), mainFiber(nullptr)
{
}

void RunProcessor(Processor* processor)
{
    processor->Run();
}

void Processor::Start()
{
    thread = std::thread(RunProcessor, this);
}

void Processor::Stop()
{
    thread.join();
}

void Processor::Run()
{
    try
    {
        mainFiber = util::ConvertThreadToFiber(this);
        while (!machine->Exiting())
        {
            UserProcess* kernelProcess = GetRunnableKernelProcess();
            if (kernelProcess)
            {
                void* kernelFiber = nullptr;
                {
                    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
                    kernelProcess->SetMainFiber(mainFiber);
                    kernelProcess->SetInterruptHandler(nullptr);
                    kernelProcess->SetProcessor(this);
                    kernelProcess->SetSaveContext(false);
                    currentProcess = kernelProcess;
                    kernelFiber = kernelProcess->KernelFiber();
                }
                if (kernelFiber)
                {
                    kernelProcess->WaitNotInKernel(true);
                    if (machine->Exiting()) continue;
                    util::SwitchToFiber(kernelFiber);
                    kernelProcess->SetNotInKernel();
                }
                continue;
            }
            Scheduler* scheduler = machine->GetScheduler();
            cmajor::systemx::machine::UserProcess* process = scheduler->GetRunnableProcess(this);
            if (!process)
            {
                continue;
            }
            {
                std::lock_guard<std::recursive_mutex> lock(machine->Lock());
                if (!process->KernelFiber())
                {
                    void* kernelFiber = util::CreateFiber(kernelStackSize, cmajor::systemx::machine::RunKernel, process);
                    process->SetKernelFiber(kernelFiber);
                }
                currentProcess = process;
            }
            process->WaitNotInKernel(false);
            ProcessState processState = ProcessState::runnableInUser;
            Debugger* debugger = process->GetDebugger();
            uint64_t pc = static_cast<uint64_t>(-1);
            {
                std::lock_guard<std::recursive_mutex> lock(machine->Lock());
                process->SetRunning(this);
                process->RestoreContext(*machine, registers);
                pc = registers.GetPC();
                process->SetStartUserTime();
                process->SetSaveContext(true);
                processState = process->State();
            }
            while (process && processState == ProcessState::running)
            {
                if (debugger)
                {
                    debugger->Intercept();
                }
                if (machine->Exiting()) break;
                uint64_t prevPC = pc;
                uint8_t x = 0;
                uint8_t y = 0;
                uint8_t z = 0;
                try
                {
                    Instruction* inst = FetchInstruction(pc, x, y, z);
                    inst->Execute(*this, x, y, z);
                    SetPC(inst, pc, prevPC);
                    if (CheckInterrupts())
                    {
                        process->SetNotInKernel();
                        process = nullptr;
                        processState = ProcessState::runnableInUser;
                        continue;
                    }
                    pc = registers.GetPC();
                }
                catch (const MemoryError& memoryError)
                {
                    throw MemoryError("memory error: " + std::string(memoryError.what()) +
                        std::string(": PID=" + std::to_string(currentProcess->Id()) + ", process='" + currentProcess->FilePath() +
                            "', PC = #" + util::ToHexString(registers.GetPC()) + ", rW = #" + util::ToHexString(registers.GetSpecial(rW))));
                }
            }
        }
    }
    catch (...)
    {
        exception = std::current_exception();
        machine->SetHasException();
    }
}

Instruction* Processor::FetchInstruction(uint64_t& pc, uint8_t& x, uint8_t& y, uint8_t& z)
{
    Memory& mem = machine->Mem();
    uint64_t rv = registers.GetSpecial(rV);
    uint8_t opCode = mem.ReadByte(rv, pc, Protection::execute);
    ++pc;
    x = mem.ReadByte(rv, pc, Protection::execute);
    ++pc;
    y = mem.ReadByte(rv, pc, Protection::execute);
    ++pc;
    z = mem.ReadByte(rv, pc, Protection::execute);
    ++pc;
    Instruction* inst = machine->GetInstruction(opCode);
    return inst;
}

void Processor::SetPC(Instruction* inst, uint64_t pc, uint64_t prevPC)
{
    uint64_t regsPC = registers.GetPC();
    if (!inst->IsJumpInstruction() && regsPC == prevPC)
    {
        registers.SetPC(pc);
    }
    registers.SetSpecial(rW, prevPC);
}

bool Processor::CheckInterrupts()
{
    uint64_t interruptBits = registers.GetInterruptBits();
    if (interruptBits)
    {
        for (int irq = 0; irq < 64; ++irq)
        {
            uint64_t irqBit = static_cast<uint64_t>(1) << irq;
            if (interruptBits & irqBit)
            {
                InterruptHandler* handler = GetInterruptHandler(irq);
                if (handler)
                {
                    void* kernelFiber = nullptr;
                    {
                        std::lock_guard<std::recursive_mutex> lock(machine->Lock());
                        currentProcess->AddUserTime();
                        currentProcess->SetMainFiber(mainFiber);
                        currentProcess->SetInterruptHandler(handler);
                        currentProcess->SetSaveContext(true);
                        kernelFiber = currentProcess->KernelFiber();
                    }
                    if (kernelFiber)
                    {
                        currentProcess->WaitNotInKernel(true);
                        util::SwitchToFiber(kernelFiber);
                        return true;
                    }
                }
                else
                {
                    throw std::runtime_error("no interrupt handler for IRQ " + std::to_string(irq));
                }
            }
        }
    }
    return false;
}

void Processor::EnableInterrupts()
{
    registers.SetSpecial(rK, ALL_INTERRUPT_BITS);
}

void Processor::ResetCurrentProcess(bool addSystemTime, bool saveContext, bool setKernelProcessor)
{
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    if (addSystemTime)
    {
        currentProcess->AddSystemTime();
    }
    if (saveContext)
    {
        currentProcess->SaveContext(*machine, registers);
        currentProcess->SetSaveContext(false);
    }
    if (setKernelProcessor)
    {
        currentProcess->SetKernelProcessor(this);
    }
    currentProcess->ResetProcessor();
    currentProcess = nullptr;
}

void Processor::CheckException()
{
    if (exception)
    {
        std::rethrow_exception(exception);
    }
}

void Processor::AddRunnableKernelProcess(UserProcess* runnableKernelProcess)
{
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    runnableKernelProcesses.push_back(runnableKernelProcess);
}

UserProcess* Processor::GetRunnableKernelProcess()
{
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    if (!runnableKernelProcesses.empty())
    {
        UserProcess* runnableKernelProcess = runnableKernelProcesses.front();
        runnableKernelProcesses.pop_front();
        return runnableKernelProcess;
    }
    return nullptr;
}

bool Processor::HasRunnableKernelProcess()
{
    std::lock_guard<std::recursive_mutex> lock(machine->Lock());
    return !runnableKernelProcesses.empty();
}

struct RunningKernelGuard
{
    RunningKernelGuard(UserProcess* process_) : process(process_) 
    {
        prevState = process->State();
        process->SetState(ProcessState::runningKernel);
    }
    ~RunningKernelGuard()
    {
        if (process->State() == ProcessState::runningKernel)
        {
            process->SetState(prevState);
        }
    }
    UserProcess* process;
    ProcessState prevState;
};

void RunKernel()
{
    void* fiberData = util::GetFiberData();
    UserProcess* process = static_cast<UserProcess*>(fiberData);
    Machine* machine = process->GetMachine();
    while (!machine->Exiting())
    {
        void* processMainFiber = nullptr;
        try
        {
            Processor* processor = nullptr;
            InterruptHandler* currentHandler = nullptr;
            {
                std::lock_guard<std::recursive_mutex> lock(machine->Lock());
                process->SetStartSystemTime();
                currentHandler = process->GetInterruptHandler();
                processor = process->GetProcessor();
                processMainFiber = process->MainFiber();
            }
            if (currentHandler && processor)
            {
                RunningKernelGuard guard(process);
                currentHandler->HandleInterrupt(*processor);
            }
            {
                std::lock_guard<std::recursive_mutex> lock(machine->Lock());
                process->AddSystemTime();
                if (process->State() != ProcessState::zombie && process->GetProcessor() == processor)
                {
                    bool saveContext = process->State() != ProcessState::exec && process->DoSaveContext();
                    processor->ResetCurrentProcess(false, saveContext, false);
                    machine->GetScheduler()->AddRunnableProcess(process, ProcessState::runnableInUser);
                }
            }
            machine->GetScheduler()->CheckRunnable();
            util::SwitchToFiber(processMainFiber);
        }
        catch (...)
        {
            Processor* processor = process->GetProcessor();
            if (processor)
            {
                processor->SetException(std::current_exception());
            }
            machine->SetHasException();
            util::SwitchToFiber(processMainFiber);
        }
    }
}

} // namespace cmajor::systemx::machine
