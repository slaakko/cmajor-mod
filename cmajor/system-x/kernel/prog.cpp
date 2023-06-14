// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.prog;

import cmajor.systemx.kernel.error;
import cmajor.systemx.kernel.process;
import cmajor.systemx.kernel.io;
import cmajor.systemx.machine;
import util;

namespace cmajor::systemx::kernel {

class Prog
{
public:
    Prog(util::Process* process_, const std::string& name_, int portNumber_);
    virtual ~Prog();
    virtual bool Stop(std::string& error) = 0;
    util::Process* GetProcess() const { return process.get(); }
    int PortNumber() const { return portNumber; }
    const std::string& Name() const { return name; }
private:
    std::unique_ptr<util::Process> process;
    int portNumber;
    std::string name;
};

Prog::Prog(util::Process* process_, const std::string& name_, int portNumber_) : process(process_), name(name_), portNumber(portNumber_)
{
}

Prog::~Prog()
{
    try
    {
        process->Terminate();
        process.reset();
    }
    catch (...)
    {
    }
}

class SxbsProg : public Prog
{
public:
    SxbsProg(util::Process* process);
    bool Stop(std::string& error) override;
};

SxbsProg::SxbsProg(util::Process* process) : Prog(process, "sxbs", sxbsPortNumber)
{
}

bool SxbsProg::Stop(std::string& error)
{
    util::TcpSocket socket;
    try
    {
        socket = util::TcpSocket("127.0.0.1", std::to_string(PortNumber()));
    }
    catch (const std::exception& ex)
    {
        error = ex.what();
        return false;
    }
    catch (...)
    {
        error = "unknown error";
    }
    try
    {
        util::Write(socket, "stop server request");
        std::string replyStr = util::ReadStr(socket);
        if (replyStr == "stop server reply")
        {
            socket.Close();
            return true;
        }
    }
    catch (const std::exception& ex)
    {
        error = ex.what();
        return false;
    }
    catch (...)
    {
        error = "unknown error";
    }
    if (error.empty())
    {
        error = "unknown error";
    }
    return false;
}

class ProgTable
{
public:
    static void Init();
    static void Done();
    static ProgTable& Instance() { return *instance; }
    int32_t AddProg(Prog* prog);
    void RemoveProg(int32_t prog);
private:
    static std::unique_ptr<ProgTable> instance;
    std::vector<std::unique_ptr<Prog>> progs;
};

std::unique_ptr<ProgTable> ProgTable::instance;

void ProgTable::Init()
{
    instance.reset(new ProgTable());
}

void ProgTable::Done()
{
    instance.reset();
}

int32_t ProgTable::AddProg(Prog* prog)
{
    int32_t progId = progs.size();
    progs.push_back(std::unique_ptr<Prog>(prog));
    return progId;
}

void ProgTable::RemoveProg(int32_t progId)
{
    if (progId >= 0 && progId < progs.size())
    {
        std::string errorMsg;
        if (!progs[progId]->Stop(errorMsg))
        {
            progs[progId].reset();
            throw SystemError(EHOST, "prog id " + std::to_string(progId) + " (" + progs[progId]->Name() + ") failed to stop : " + errorMsg, __FUNCTION__);
        }
        progs[progId].reset();
    }
    else
    {
        throw SystemError(EPARAM, "bad prog id " + std::to_string(progId), __FUNCTION__);
    }
}

int32_t Start(Process* process, int64_t progAddr)
{
    if (progAddr == 0)
    {
        throw SystemError(EPARAM, "prog is null", __FUNCTION__);
    }
    cmajor::systemx::machine::Memory& mem = process->GetProcessor()->GetMachine()->Mem();
    std::string progName = ReadString(process, progAddr, mem);
    if (progName == "sx.bs")
    {
        Prog* prog = new SxbsProg(new util::Process("sxbs",
            util::Process::Redirections::processStdIn |
            util::Process::Redirections::processStdOut |
            util::Process::Redirections::processStdErr));
        std::string line = prog->GetProcess()->ReadLine(util::Process::StdHandle::stdOut);
        if (line == "sx.bs.ready")
        {
            return ProgTable::Instance().AddProg(prog);
        }
        else if (line == "sx.bs.error")
        {
            std::string line = prog->GetProcess()->ReadLine(util::Process::StdHandle::stdOut);
            throw SystemError(EPARAM, "error starting '" + progName + "': " + line, __FUNCTION__);
        }
        else
        {
            throw SystemError(EPARAM, "error starting '" + progName + "': process exited prematurely", __FUNCTION__);
        }
    }
    else
    {
        throw SystemError(EPARAM, "unknown prog name '" + progName + "'", __FUNCTION__);
    }
}

void Stop(int32_t prog)
{
    ProgTable::Instance().RemoveProg(prog);
}

void InitProg()
{
    ProgTable::Init();
}

void DoneProg()
{
    ProgTable::Done();
}

} // namespace cmajor::systemx::kernel
