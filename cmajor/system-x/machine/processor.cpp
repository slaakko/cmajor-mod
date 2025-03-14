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

Processor::Processor() : mainFiber(nullptr), id(0), machine(nullptr), currentProcess(nullptr), currentHandler(nullptr), kernelStackSize(KernelStackSize())
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
            Scheduler* scheduler = machine->GetScheduler();
            currentProcess = scheduler->GetRunnableProcess();
            if (!currentProcess)
            {
                break;
            }
            if (!currentProcess->KernelFiber())
            {
                void* kernelFiber = util::CreateFiber(kernelStackSize, cmajor::systemx::machine::RunKernel, this);
                currentProcess->SetKernelFiber(kernelFiber);
            }
            currentProcess->RestoreContext(*machine, registers);
            ProcessState processState = currentProcess->State();
            currentProcess->SetRunning(this);
            uint64_t pc = registers.GetPC();
            if (processState == ProcessState::runnableInKernel)
            {
                util::SwitchToFiber(currentProcess->KernelFiber());
                pc = registers.GetPC();
            }
            if (currentProcess)
            {
                currentProcess->SetStartUserTime();
            }
            while (currentProcess && currentProcess->State() == ProcessState::running)
            {
                Debugger* debugger = currentProcess->GetDebugger();
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
                    CheckInterrupts();
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

void Processor::CheckInterrupts()
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
                    if (currentProcess)
                    {
                        currentProcess->AddUserTime();
                        currentHandler = handler;
                        util::SwitchToFiber(currentProcess->KernelFiber());
                    }
                }
                else
                {
                    throw std::runtime_error("no interrupt handler for IRQ " + std::to_string(irq));
                }
            }
        }
    }
}

void Processor::EnableInterrupts()
{
    registers.SetSpecial(rK, ALL_INTERRUPT_BITS);
}

void Processor::ResetCurrentProcess(bool addSystemTime, bool saveContext)
{
    if (addSystemTime)
    {
        currentProcess->AddSystemTime();
    }
    if (saveContext)
    {
        currentProcess->SaveContext(*machine, registers);
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

void Processor::RunKernel()
{
    while (!machine->Exiting())
    {
        try
        {
            if (currentProcess)
            {
                currentProcess->SetStartSystemTime();
            }
            currentHandler->HandleInterrupt(*this);
            if (currentProcess)
            {
                currentProcess->AddSystemTime();
            }
            if (currentProcess && currentProcess->State() != ProcessState::zombie)
            {
                bool exec = currentProcess->State() == ProcessState::exec;
                machine->GetScheduler()->AddRunnableProcess(currentProcess, ProcessState::runnableInUser);
                ResetCurrentProcess(false, !exec);
            }
            machine->GetScheduler()->CheckRunnable();
        }
        catch (...)
        {
            exception = std::current_exception();
            machine->SetHasException();
        }
        util::SwitchToFiber(mainFiber);
    }
}

void RunKernel()
{
    void* fiberData = util::GetFiberData();
    Processor* processor = static_cast<Processor*>(fiberData);
    processor->RunKernel();
}

} // namespace cmajor::systemx::machine
