// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.debug;

import cmajor.systemx.kernel.trap;
import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.io;
import cmajor.systemx.machine;
import util;

namespace cmajor::systemx::kernel {

const int defaultDebugLogPort = 55000;

class DebugLogWriter
{
public:
    static void Init();
    static void Done();
    static DebugLogWriter& Instance() { return *instance; }
    void SetDebugMode(int debugMode_) { debugMode = debugMode_; }
    int GetDebugMode() const { return debugMode; }
    void SetPort(int port_) { port = port_; }
    void WriteDebugMessage(const std::string& debugMessage);
    void Connect();
    void CloseConnection();
private:
    DebugLogWriter();
    static std::unique_ptr<DebugLogWriter> instance;
    int debugMode;
    int port;
    bool connected;
    std::unique_ptr<util::TcpSocket> socket;
};

std::unique_ptr<DebugLogWriter> DebugLogWriter::instance;

void DebugLogWriter::Init()
{
    instance.reset(new DebugLogWriter());
}

void DebugLogWriter::Done()
{
    instance.reset();
}

DebugLogWriter::DebugLogWriter() : debugMode(0), port(defaultDebugLogPort), connected(false)
{
}

void DebugLogWriter::Connect()
{
    try
    {
        connected = true;
        socket.reset(new util::TcpSocket("127.0.0.1", std::to_string(port)));
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("could not connect kernel debugger to port " + std::to_string(port) + ": " + ex.what());
    }
}

void DebugLogWriter::CloseConnection()
{
    connected = false;
    socket.reset();
}

void DebugLogWriter::WriteDebugMessage(const std::string& debugMessage)
{
    if (!connected)
    {
        Connect();
    }
    Write(*socket, debugMessage);
}

void SetDebugMode(int debugMode)
{
    DebugLogWriter::Instance().SetDebugMode(debugMode);
}

int GetDebugMode()
{
    return DebugLogWriter::Instance().GetDebugMode();
}

void SetDebugLogPort(int port)
{
    DebugLogWriter::Instance().SetPort(port);
}

void DebugWrite(const std::string& debugMessage)
{
    DebugLogWriter::Instance().WriteDebugMessage(debugMessage);
}

void StartDebug()
{
    if (GetDebugMode() != 0)
    {
        DebugWrite("kernel.debug: started kernel debugging in mode " + std::to_string(GetDebugMode()));
    }
}

void StopDebug()
{
    if (GetDebugMode() != 0)
    {
        DebugWrite("kernel.debug: stopped kernel debugging in mode " + std::to_string(GetDebugMode()));
    }
}

class TrapGetDebugModeHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_get_debug_mode"; }
};

uint64_t TrapGetDebugModeHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        return GetDebugMode();
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapSetDebugModeHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_set_debug_mode"; }
};

uint64_t TrapSetDebugModeHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int32_t debugMode = static_cast<int32_t>(processor.Regs().Get(cmajor::systemx::machine::regAX));
        SetDebugMode(debugMode);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

class TrapWriteDebugMessageHandler : public TrapHandler
{
public:
    uint64_t HandleTrap(cmajor::systemx::machine::Processor& processor) override;
    std::string TrapName() const { return "trap_write_debug_message"; }
};

uint64_t TrapWriteDebugMessageHandler::HandleTrap(cmajor::systemx::machine::Processor& processor)
{
    Process* process = static_cast<Process*>(processor.CurrentProcess());
    try
    {
        int64_t msgAddr = processor.Regs().Get(cmajor::systemx::machine::regAX);
        if (msgAddr == 0)
        {
            throw std::runtime_error("message pointer is null");
        }
        cmajor::systemx::machine::Memory& mem = process->GetProcessor()->GetMachine()->Mem();
        std::string msg = ReadString(process, msgAddr, mem);
        DebugWrite(msg);
        return 0;
    }
    catch (const SystemError& error)
    {
        process->SetError(error);
        return static_cast<uint64_t>(-1);
    }
}

void InitDebugTraps()
{
    SetTrapHandler(trap_get_debug_mode, new TrapGetDebugModeHandler());
    SetTrapHandler(trap_set_debug_mode, new TrapSetDebugModeHandler());
    SetTrapHandler(trap_write_debug_message, new TrapWriteDebugMessageHandler());
}

void DoneDebugTraps()
{
    SetTrapHandler(trap_write_debug_message, nullptr);
    SetTrapHandler(trap_set_debug_mode, nullptr);
    SetTrapHandler(trap_get_debug_mode, nullptr);
}

void InitDebug()
{
    DebugLogWriter::Init();
    InitDebugTraps();
}

void DoneDebug()
{
    DoneDebugTraps();
    DebugLogWriter::Done();
}

} // namespace cmajor::systemx::kernel
