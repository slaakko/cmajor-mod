// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.service;

import cmajor.debuggers;
import cmajor.debug;
import cmajor.port.map.service;
import util;

namespace cmajor::service {

class DebugLogger : public cmajor::debugger::DebugLogger
{
public:
    void LogRequest(cmajor::debugger::Request* request) override;
    void LogReply(cmajor::debugger::Reply* reply) override;
};

void DebugLogger::LogRequest(cmajor::debugger::Request* request)
{
    DebugLogMessage* debugLogMessage = new DebugLogMessage("->" + request->ToString());
    PutServiceMessage(debugLogMessage);
}

void DebugLogger::LogReply(cmajor::debugger::Reply* reply)
{
    DebugLogMessage* debugLogMessage = new DebugLogMessage("<-" + reply->ToString());
    PutServiceMessage(debugLogMessage);
}

class MessageWriter : public cmajor::debugger::MessageWriter
{
public:
    void WriteMessage(const std::string& message) override;
};

void MessageWriter::WriteMessage(const std::string& message)
{
    PutOutputServiceMessage(message);
}

DebugServiceStartParams& DebugServiceStartParams::Backend(const std::string& backend_)
{
    backend = backend_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::Config(const std::string& config_)
{
    config = config_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::ExecutableFilePath(const std::string& executableFilePath_)
{
    executableFilePath = executableFilePath_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::ProgramArguments(const std::string& args)
{
    programArguments = args;
    return *this;
}

class DebugOutputWriter : public cmajor::debug::DebuggerOutputWriter
{
public:
    void WriteLogMessage(const std::string& message) override; 
    void WriteJsonValue(util::JsonValue* jsonValue) override;
    void WriteWarning(const std::string& warning) override;
};

void DebugOutputWriter::WriteLogMessage(const std::string& message)
{
    PutOutputServiceMessage(message);
}

void DebugOutputWriter::WriteJsonValue(util::JsonValue* jsonValue)
{
}

void DebugOutputWriter::WriteWarning(const std::string& warning)
{
}

class RequestGuard
{
public:
    RequestGuard(bool& requestInProgress_) : requestInProgress(requestInProgress_) {}
    ~RequestGuard() { requestInProgress = false; }
private:
    bool& requestInProgress;
};

class DebugCommand
{
public:
    DebugCommand();
    virtual ~DebugCommand();
    virtual void Execute() = 0;
    virtual std::string Name() = 0;
};

DebugCommand::DebugCommand()
{
}

DebugCommand::~DebugCommand()
{
}

class DebugService : public cmajor::debug::CmdbSessionClient
{
public:
    static DebugService& Instance();
    void Start(const DebugServiceStartParams& startParams);
    bool Started() const { return started; }
    void Stop();
    void Run();
    void Continue();
    void Next();
    void Step();
    void Finish();
    void Until(const cmajor::info::db::Location& loc);
    void Depth();
    void Frames(int lowFrame, int highFrame);
    void Count(const std::string& expression);
    void EvaluateChild(const std::string& expression, int start, int count);
    void Evaluate(const std::string& expression, int requestId);
    bool Running() const { return debugger.get() != nullptr; }
    void RunSession();
    void RunQueue();
    std::string GetTargetInputBytes() override;
    void WriteTargetOuput(int handle, const std::string& s) override;
    void SetInputEof();
    void PutInputLine(const std::string& inputLine);
    bool RequestInProgress() const { return requestInProgress; }
    void SetRequestInProgress(bool requestInProgress_) { requestInProgress = requestInProgress_; }
    void PutCommand(DebugCommand* command);
    std::unique_ptr<DebugCommand> GetCommand();
private:
    DebugService();
    std::unique_ptr<cmajor::debugger::Debugger> debugger;
    std::string cmdbSessionFilePath;
    int sessionPort;
    bool sessionThreadStarted;
    bool commandThreadStarted;
    bool inputEof;
    bool waitingForInput;
    bool exiting;
    bool started;
    bool requestInProgress;
    std::mutex mtx;
    std::condition_variable waitingForInputOrExitVar;
    std::list<std::string> inputLines;
    DebugOutputWriter outputWriter;
    std::thread sessionThread;
    std::thread commandThread;
    std::mutex commandQueueMutex;
    bool CommandAvailableOrExiting() const { return !commandQueue.empty() || exiting; }
    std::list<std::unique_ptr<DebugCommand>> commandQueue;
    std::condition_variable commandAvailableOrExiting;
};

void DebugService::PutCommand(DebugCommand* command)
{
    std::lock_guard<std::mutex> lock(commandQueueMutex);
    commandQueue.push_back(std::unique_ptr<DebugCommand>(command));
    commandAvailableOrExiting.notify_one();
}

std::unique_ptr<DebugCommand> DebugService::GetCommand()
{
    std::unique_lock<std::mutex> lock(commandQueueMutex);
    commandAvailableOrExiting.wait(lock, [this] { return CommandAvailableOrExiting(); });
    if (exiting) return std::unique_ptr<DebugCommand>();
    std::unique_ptr<DebugCommand> command = std::move(commandQueue.front());
    commandQueue.pop_front();
    return command;
}

void DebugService::RunQueue()
{
    while (!exiting)
    {
        std::unique_ptr<DebugCommand> command = GetCommand();
        if (exiting || !command) return;
        try
        {
            command->Execute();
        }
        catch (const std::exception& ex)
        {
            PutOutputServiceMessage("debugger: error executing " + command->Name() + ": " + std::string(ex.what()));
        }
    }
}

void RunCommandQueue()
{
    try
    {
        DebugService::Instance().RunQueue();
    }
    catch (...)
    {
    }
}

class RunCommand : public DebugCommand
{
public:
    void Execute() override
    {
        DebugService::Instance().Run();
    }
    std::string Name() override
    {
        return "run command";
    }
};

class ContinueCommand : public DebugCommand
{
public:
    void Execute() override
    {
        DebugService::Instance().Continue();
    }
    std::string Name() override
    {
        return "continue command";
    }
};

class NextCommand : public DebugCommand
{
public:
    void Execute() override
    {
        DebugService::Instance().Next();
    }
    std::string Name() override
    {
        return "next command";
    }
};

class StepCommand : public DebugCommand
{
public:
    void Execute() override
    {
        DebugService::Instance().Step();
    }
    std::string Name() override
    {
        return "step command";
    }
};

class FinishCommand : public DebugCommand
{
public:
    void Execute() override
    {
        DebugService::Instance().Finish();
    }
    std::string Name() override
    {
        return "finish command";
    }
};

class UntilCommand : public DebugCommand
{
public:
    UntilCommand(const cmajor::info::db::Location& loc_) : loc(loc_) 
    {
    }
    void Execute() override
    {
        DebugService::Instance().Until(loc);
    }
    std::string Name() override
    {
        return "until command";
    }
private:
    cmajor::info::db::Location loc;
};

class DepthCommand : public DebugCommand
{
public:
    void Execute() override
    {
        DebugService::Instance().Depth();
    }
    std::string Name() override
    {
        return "depth command";
    }
};

class FramesCommand : public DebugCommand
{
public:
    FramesCommand(int lowFrame_, int highFrame_) : lowFrame(lowFrame_), highFrame(highFrame_)
    {
    }
    void Execute() override
    {
        DebugService::Instance().Frames(lowFrame, highFrame);
    }
    std::string Name() override
    {
        return "frames command";
    }
private:
    int lowFrame;
    int highFrame;
};

class CountCommand : public DebugCommand
{
public:
    CountCommand(const std::string& expression_) : expression(expression_)
    {
    }
    void Execute() override
    {
        DebugService::Instance().Count(expression);
    }
    std::string Name() override
    {
        return "count command";
    }
private:
    std::string expression;
};

class EvaluateChildCommand : public DebugCommand
{
public:
    EvaluateChildCommand(const std::string& expression_, int start_, int count_) : expression(expression_), start(start_), count(count_)
    {
    }
    void Execute() override
    {
        DebugService::Instance().EvaluateChild(expression, start, count);
    }
    std::string Name() override
    {
        return "evaluate child command";
    }
private:
    std::string expression;
    int start;
    int count;
};

class EvaluateCommand : public DebugCommand
{
public:
    EvaluateCommand(const std::string& expression_, int requestId_) : expression(expression_), requestId(requestId_)
    {
    }
    void Execute() override
    {
        DebugService::Instance().Evaluate(expression, requestId);
    }
    std::string Name() override
    {
        return "evaluate command";
    }
private:
    std::string expression;
    int requestId;
};

std::string DebugService::GetTargetInputBytes()
{
    std::unique_lock<std::mutex> lock(mtx);
    if (inputEof)
    {
        return std::string();
    }
    std::string targetInputLine;
    if (!inputLines.empty())
    {
        targetInputLine = std::move(inputLines.front());
        inputLines.pop_front();
    }
    else
    {
        waitingForInput = true;
        waitingForInputOrExitVar.wait(lock, [this] { return !inputLines.empty() || inputEof || exiting; });
        if (exiting || inputEof)
        {
            return std::string();
        }
        targetInputLine = std::move(inputLines.front());
        inputLines.pop_front();
    }
    std::string targetInputBytes;
    for (char c : targetInputLine)
    {
        uint8_t x = static_cast<uint8_t>(c);
        targetInputBytes.append(util::ToHexString(x));
    }
    targetInputBytes.append(util::ToHexString(static_cast<uint8_t>('\n')));
    return targetInputBytes;
}

void DebugService::WriteTargetOuput(int handle, const std::string& s)
{
    cmajor::debugger::OutputRequest outputRequest;
    outputRequest.handle = handle;
    outputRequest.output = s;
    TargetOutputServiceMessage outputMessage(outputRequest);
    PutServiceMessage(new TargetOutputServiceMessage(outputMessage));
}

void DebugService::SetInputEof()
{
    inputEof = true;
    if (waitingForInput)
    {
        waitingForInputOrExitVar.notify_all();
    }
}

void DebugService::PutInputLine(const std::string& inputLine)
{
    std::lock_guard<std::mutex> lock(mtx);
    inputLines.push_back(inputLine);
    if (waitingForInput)
    {
        waitingForInputOrExitVar.notify_all();
    }
}

DebugService& DebugService::Instance()
{
    static DebugService instance;
    return instance;
}

void RunDebugSession(DebugService* service)
{
    service->RunSession();
}

void DebugService::RunSession()
{
    try
    {
        cmajor::debug::SetCmdbSessionPort(sessionPort);
        cmajor::debug::StartCmdbSession(cmdbSessionFilePath, &outputWriter, this, true);
        cmajor::debug::RemoveCmdbSessionFileGuard removeFileGuard(cmdbSessionFilePath);
        while (!exiting)
        {
            std::unique_lock<std::mutex> lock(mtx);
            waitingForInputOrExitVar.wait(lock, [this] { return inputEof || exiting; });
        }
        cmajor::debug::StopCmdbSession();
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage("debug service: error: " + std::string(ex.what()));
    }
}

DebugService::DebugService() : sessionPort(0), outputWriter(), sessionThreadStarted(false), commandThreadStarted(false), started(false), inputEof(false), 
    waitingForInput(false), exiting(false), requestInProgress(false)
{
}

void DebugService::Start(const DebugServiceStartParams& startParams)
{
    try
    {
        commandQueue.clear();
        RequestGuard requestGuard(requestInProgress);
        started = false;
        PutServiceMessage(new ClearDebugLogMessage());
        if (startParams.backend == "llvm")
        {
            debugger.reset(new cmajor::debugger::LLDBDebugger());
        }
        else if (startParams.backend == "cpp")
        {
            debugger.reset(new cmajor::debugger::GDBDebugger());
        }
        else
        {
            throw std::runtime_error("cannot start debugging: unknown backend '" + startParams.backend + "'");
        }
        debugger->SetDebugLogger(new DebugLogger());
        debugger->SetMessageWriter(new MessageWriter());
        exiting = false;
        inputEof = false;
        cmdbSessionFilePath = util::Path::ChangeExtension(startParams.executableFilePath, ".cmdbs");
        sessionPort = GetFreePortNumber("cmcode");
        if (sessionPort == -1)
        {
            sessionPort = 55005;
        }
        cmajor::debugger::DebuggerStartParams debuggerStartParams;
        debuggerStartParams.executableFilePath = startParams.executableFilePath;
        debuggerStartParams.programArguments = startParams.programArguments;
        debuggerStartParams.breakpoints = startParams.breakpoints;
        std::unique_ptr<cmajor::debugger::Reply> reply = debugger->Start(debuggerStartParams);
        PutServiceMessage(new StartDebugServiceReplyServiceMessage(reply.release()));
        sessionThreadStarted = false;
        sessionThread = std::thread(RunDebugSession, this);
        sessionThreadStarted = true;
        commandThread = std::thread(RunCommandQueue);
        commandThreadStarted = true;
        started = true;
    }
    catch (const std::exception& ex)
    {
        started = false;
        PutServiceMessage(new StartDebugServiceErrorServiceMessage(ex.what()));
    }
}

void DebugService::Stop()
{
    try
    {
        exiting = true;
        waitingForInputOrExitVar.notify_all();
        commandAvailableOrExiting.notify_all();
        if (Running())
        {
            debugger->Stop();
            debugger.reset();
        }
        if (sessionThreadStarted)
        {
            sessionThread.join();
            sessionThreadStarted = false;
        }
        if (commandThreadStarted)
        {
            commandThread.join();
            commandThreadStarted = false;
        }
        PutServiceMessage(new DebugServiceStoppedServiceMessage());
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage(ex.what());
        debugger.reset();
        PutServiceMessage(new DebugServiceStoppedServiceMessage());
    }
}

void DebugService::Run()
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        std::unique_ptr<cmajor::debugger::Reply> reply = debugger->Run();
        PutServiceMessage(new ExecDebugServiceReplyServiceMessage(reply.release()));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::Continue()
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        std::unique_ptr<cmajor::debugger::Reply> reply = debugger->Continue();
        PutServiceMessage(new ExecDebugServiceReplyServiceMessage(reply.release()));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::Next()
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        std::unique_ptr<cmajor::debugger::Reply> reply = debugger->Next();
        PutServiceMessage(new ExecDebugServiceReplyServiceMessage(reply.release()));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::Step()
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        std::unique_ptr<cmajor::debugger::Reply> reply = debugger->Step();
        PutServiceMessage(new ExecDebugServiceReplyServiceMessage(reply.release()));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::Finish()
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        std::unique_ptr<cmajor::debugger::Reply> reply = debugger->Finish();
        PutServiceMessage(new ExecDebugServiceReplyServiceMessage(reply.release()));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::Until(const cmajor::info::db::Location& loc)
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        std::unique_ptr<cmajor::debugger::Reply> reply = debugger->Until(loc);
        PutServiceMessage(new ExecDebugServiceReplyServiceMessage(reply.release()));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::Depth()
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        int depth = debugger->Depth();
        cmajor::info::db::DepthReply depthReply;
        depthReply.depth = depth;
        PutServiceMessage(new DepthDebugServiceReplyServiceMessage(depthReply));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::Frames(int lowFrame, int highFrame)
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        cmajor::info::db::FramesReply framesReply;
        framesReply.frames = debugger->Frames(lowFrame, highFrame);
        PutServiceMessage(new FramesDebugServiceReplyServiceMessage(framesReply));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::Count(const std::string& expression)
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        if (debugger && debugger->StoppedInstruction() != nullptr)
        {
            cmajor::info::db::CountRequest countRequest;
            countRequest.expression = expression;
            cmajor::info::db::CountReply countReply = debugger->Count(countRequest);
            PutServiceMessage(new CountDebugServiceReplyServiceMessage(countReply));
        }
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::EvaluateChild(const std::string& expression, int start, int count)
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        cmajor::info::db::EvaluateChildRequest request;
        request.expression = expression;
        request.start = start;
        request.count = count;
        cmajor::info::db::EvaluateChildReply reply = debugger->EvaluateChild(request);
        PutServiceMessage(new EvaluateChildDebugServiceReplyServiceMessage(reply));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

void DebugService::Evaluate(const std::string& expression, int requestId)
{
    try
    {
        if (!started) return;
        RequestGuard requestGuard(requestInProgress);
        cmajor::info::db::EvaluateReply reply = debugger->Evaluate(expression);
        PutServiceMessage(new EvaluateDebugServiceReplyServiceMessage(reply, requestId));
    }
    catch (const std::exception& ex)
    {
        PutServiceMessage(new DebugErrorServiceMessage(ex.what()));
    }
}

ExecDebugServiceReplyServiceMessage::ExecDebugServiceReplyServiceMessage(cmajor::debugger::Reply* reply_) :
    ServiceMessage(ServiceMessageKind::execReply), reply(reply_)
{
}

void StartDebugService(const DebugServiceStartParams& startParams)
{
    DebugService::Instance().Start(startParams);
}

void StopDebugService()
{
    DebugService::Instance().Stop();
}

StartDebugServiceRequest::StartDebugServiceRequest(const DebugServiceStartParams& startParams_) : startParams(startParams_)
{
}

void StartDebugServiceRequest::Execute()
{
    StartDebugService(startParams);
}

StopDebugServiceRequest::StopDebugServiceRequest()
{
}

void StopDebugServiceRequest::Execute()
{
    StopDebugService();
}

RunDebugServiceRequest::RunDebugServiceRequest()
{
}

void RunDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new RunCommand());
}

ContinueDebugServiceRequest::ContinueDebugServiceRequest()
{
}

void ContinueDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new ContinueCommand());
}

NextDebugServiceRequest::NextDebugServiceRequest()
{
}

void NextDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new NextCommand());
}

StepDebugServiceRequest::StepDebugServiceRequest()
{
}

void StepDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new StepCommand());
}

FinishDebugServiceRequest::FinishDebugServiceRequest()
{
}

void FinishDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new FinishCommand());
}

UntilDebugServiceRequest::UntilDebugServiceRequest(const cmajor::info::db::Location& loc_) : loc(loc_)
{
}

void UntilDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new UntilCommand(loc));
}

DepthDebugServiceRequest::DepthDebugServiceRequest()
{
}

void DepthDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new DepthCommand());
}

FramesDebugServiceRequest::FramesDebugServiceRequest(int lowFrame_, int highFrame_) : lowFrame(lowFrame_), highFrame(highFrame_)
{
}

void FramesDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new FramesCommand(lowFrame, highFrame));
}

CountDebugServiceRequest::CountDebugServiceRequest(const std::string& expression_) : expression(expression_)
{
}

void CountDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new CountCommand(expression));
}

EvaluateChildDebugServiceRequest::EvaluateChildDebugServiceRequest(const std::string& expression_, int start_, int count_) : expression(expression_), start(start_), count(count_)
{
}

void EvaluateChildDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new EvaluateChildCommand(expression, start, count));
}

EvaluateDebugServiceRequest::EvaluateDebugServiceRequest(const std::string& expression_, int requestId_) : expression(expression_), requestId(requestId_)
{
}

void EvaluateDebugServiceRequest::Execute()
{
    DebugService::Instance().PutCommand(new EvaluateCommand(expression, requestId));
}

PutDebugServiceProgramInputLineRequest::PutDebugServiceProgramInputLineRequest(const std::string& inputLine_) : inputLine(inputLine_)
{
}

void PutDebugServiceProgramInputLineRequest::Execute()
{
    PutDebugServiceProgramTargetInputLine(inputLine);
}

std::string PutDebugServiceProgramInputLineRequest::Name() const
{
    return "putProgramInputLineRequest";
}

SetDebugServiceProgramEofRequest::SetDebugServiceProgramEofRequest()
{
}

void SetDebugServiceProgramEofRequest::Execute()
{
    SetDebugServiceProgramTargetInputEof();
}

std::string SetDebugServiceProgramEofRequest::Name() const
{
    return "setProgramEofRequest";
}

StartDebugServiceReplyServiceMessage::StartDebugServiceReplyServiceMessage(cmajor::debugger::Reply* reply_) : 
    ServiceMessage(ServiceMessageKind::startDebugReply), reply(reply_)
{
}

StartDebugServiceErrorServiceMessage::StartDebugServiceErrorServiceMessage(const std::string& errorMessage_) : 
    ServiceMessage(ServiceMessageKind::startError), errorMessage(errorMessage_)
{
}

DebugServiceStoppedServiceMessage::DebugServiceStoppedServiceMessage() : ServiceMessage(ServiceMessageKind::debugServiceStopped)
{
}

DebugErrorServiceMessage::DebugErrorServiceMessage(const std::string& errorMessage_) : ServiceMessage(ServiceMessageKind::debugError), errorMessage(errorMessage_)
{
}

DepthDebugServiceReplyServiceMessage::DepthDebugServiceReplyServiceMessage(const cmajor::info::db::DepthReply& depthReply_) : 
    ServiceMessage(ServiceMessageKind::depthReply), depthReply(depthReply_)
{
}

FramesDebugServiceReplyServiceMessage::FramesDebugServiceReplyServiceMessage(const cmajor::info::db::FramesReply& framesReply_) :
    ServiceMessage(ServiceMessageKind::framesReply), framesReply(framesReply_)
{
}

CountDebugServiceReplyServiceMessage::CountDebugServiceReplyServiceMessage(const cmajor::info::db::CountReply& countReply_) :
    ServiceMessage(ServiceMessageKind::countReply), countReply(countReply_)
{
}

EvaluateChildDebugServiceReplyServiceMessage::EvaluateChildDebugServiceReplyServiceMessage(const cmajor::info::db::EvaluateChildReply& reply_) :
    ServiceMessage(ServiceMessageKind::evaluateChildReply), reply(reply_)
{
}

EvaluateDebugServiceReplyServiceMessage::EvaluateDebugServiceReplyServiceMessage(const cmajor::info::db::EvaluateReply& reply_, int requestId_) : 
    ServiceMessage(ServiceMessageKind::evaluateReply), reply(reply_), requestId(requestId_)
{
}

TargetOutputServiceMessage::TargetOutputServiceMessage(const cmajor::debugger::OutputRequest& outputRequest_) :
    ServiceMessage(ServiceMessageKind::targetOutput), outputRequest(outputRequest_)
{
}

TargetInputServiceMessage::TargetInputServiceMessage() : ServiceMessage(ServiceMessageKind::targetInput)
{
}

TargetRunningServiceMessage::TargetRunningServiceMessage() : ServiceMessage(ServiceMessageKind::targetRunning)
{
}

void SetDebugServiceProgramTargetInputEof()
{
    DebugService::Instance().SetInputEof();
}

void PutDebugServiceProgramTargetInputLine(const std::string& targetInputLine)
{
    DebugService::Instance().PutInputLine(targetInputLine);
}

bool DebugRequestInProgress()
{
    return DebugService::Instance().RequestInProgress();
}

} // cmajor::service