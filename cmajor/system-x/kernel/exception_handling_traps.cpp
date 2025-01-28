// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.exception.handling.traps;

import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.exception.handling;
import cmajor.systemx.kernel.io;
import cmajor.systemx.kernel.trap;
import cmajor.systemx.machine;

namespace cmajor::systemx::kernel {

class TrapThrowHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_throw"; }
};

uint64_t TrapThrowHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        uint64_t exceptionAddress = processor.Regs().Get(cmajor::systemx::machine::regAX);
        if (exceptionAddress == 0)
        {
            throw SystemError(EPARAM, "exception is null", __FUNCTION__);
        }
        uint64_t exceptionClassId = processor.Regs().Get(cmajor::systemx::machine::regBX);
        if (exceptionClassId == 0)
        {
            throw SystemError(EPARAM, "exception class id is 0", __FUNCTION__);
        }
        uint64_t setPC = 0;
        uint64_t setFP = 0;
        uint64_t setSP = 0;
        DispatchException(exceptionAddress, exceptionClassId, process, setFP, setSP, setPC);
        processor.Regs().SetPC(setPC);
        processor.Regs().Set(cmajor::systemx::machine::regFP, setFP);
        processor.Regs().Set(cmajor::systemx::machine::regSP, setSP);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapCatchHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_catch"; }
};

uint64_t TrapCatchHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        if (process->CurrentExceptionAddress() == 0)
        {
            throw SystemError(EFAIL, "current exception not set", __FUNCTION__);
        }
        return process->CurrentExceptionAddress();
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(0);
    }
}

class TrapResumeHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_resume"; }
};

uint64_t TrapResumeHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        uint64_t setPC = 0;
        uint64_t setFP = 0;
        uint64_t setSP = 0;
        ContinueExceptionDispatch(process, setFP, setSP, setPC);
        processor.Regs().SetPC(setPC);
        processor.Regs().Set(cmajor::systemx::machine::regFP, setFP);
        processor.Regs().Set(cmajor::systemx::machine::regSP, setSP);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapStackTraceHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_stack_trace"; }
};

uint64_t TrapStackTraceHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        uint64_t bufferAddress = processor.Regs().Get(cmajor::systemx::machine::regAX);
        if (bufferAddress == 0)
        {
            throw SystemError(EPARAM, "buffer is null", __FUNCTION__);
        }
        int64_t count = processor.Regs().Get(cmajor::systemx::machine::regBX);
        if (count <= 0)
        {
            throw SystemError(EPARAM, "count is nonpositive", __FUNCTION__);
        }
        std::string stackTrace = GetStackTrace(process);
        std::vector<std::uint8_t> buffer = MakeStringBuffer(stackTrace, count);
        WriteProcessMemory(process, bufferAddress, buffer);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapGetSystemErrorHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_get_system_error"; }
};

uint64_t TrapGetSystemErrorHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        SystemError systemError = process->GetError();
        uint64_t errorCodeAddress = processor.Regs().Get(cmajor::systemx::machine::regAX);
        if (errorCodeAddress == 0)
        {
            throw SystemError(EPARAM, "error code pointer is null", __FUNCTION__);
        }
        processor.GetMachine()->Mem().WriteTetra(process->RV(), errorCodeAddress, systemError.ErrorCode(), cmajor::systemx::machine::Protection::write);
        uint64_t bufferAddress = processor.Regs().Get(cmajor::systemx::machine::regBX);
        if (bufferAddress == 0)
        {
            throw SystemError(EPARAM, "buffer pointer is null", __FUNCTION__);
        }
        uint64_t bufferSize = processor.Regs().Get(cmajor::systemx::machine::regCX);
        std::vector<uint8_t> messageBuffer = MakeStringBuffer(systemError.Message(), bufferSize);
        WriteProcessMemory(process, bufferAddress, messageBuffer);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

void InitExceptionHandlingTraps()
{
    SetTrapHandler(trap_throw, new TrapThrowHandler());
    SetTrapHandler(trap_catch, new TrapCatchHandler());
    SetTrapHandler(trap_resume, new TrapResumeHandler());
    SetTrapHandler(trap_stack_trace, new TrapStackTraceHandler());
    SetTrapHandler(trap_get_system_error, new TrapGetSystemErrorHandler());
}

void DoneExceptionHandlingTraps()
{
    SetTrapHandler(trap_get_system_error, nullptr);
    SetTrapHandler(trap_stack_trace, nullptr);
    SetTrapHandler(trap_resume, nullptr);
    SetTrapHandler(trap_catch, nullptr);
    SetTrapHandler(trap_throw, nullptr);
}

} // namespace cmajor::systemx::kernel
