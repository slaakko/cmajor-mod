// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.service.debug.service;

import cmajor.service.port.map.service;
import util;

namespace cmajor::service {

const int defaultDebugServerPort = 55003;
const int defaultSessionPort = 55004;

enum class DebugMessageKind : int
{
    startDebugRequest, startDebugReply, stopDebugRequest, stopDebugReply, genericDebugErrorReply,
    continueRequest, continueReply, nextRequest, nextReply, stepRequest, stepReply, finishRequest, finishReply, untilRequest, untilReply, breakRequest, breakReply, 
    deleteRequest, deleteReply,
    depthRequest, depthReply, framesRequest, framesReply, evaluateRequest, evaluateReply, countRequest, countReply, evaluateChildRequest, evaluateChildReply,
    targetRunningRequest, targetRunningReply, targetInputRequest, targetInputReply, targetOutputRequest, targetOutputReply, logDebugMessageRequest, logDebugMessageReply
};

DebugServiceStartParams::DebugServiceStartParams() : pid(0), debugServer(false), log(false), wait(false), verbose(false)
{
}

DebugServiceStartParams& DebugServiceStartParams::ProcessName(const std::string& processName_)
{
    processName = processName_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::Pid(int pid_)
{
    pid = pid_;
    return *this;
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

DebugServiceStartParams& DebugServiceStartParams::ProjectFilePath(const std::string& projectFilePath_)
{
    projectFilePath = projectFilePath_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::ExecutableName(const std::string& executableName_)
{
    executableName = executableName_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::ProgramArguments(const std::string& programArguments_)
{
    programArguments = programArguments_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::DebugServer(bool debugServer_)
{
    debugServer = debugServer_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::Log(bool log_)
{
    log = log_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::Wait(bool wait_)
{
    wait = wait_;
    return *this;
}

DebugServiceStartParams& DebugServiceStartParams::Verbose(bool verbose_)
{
    verbose = verbose_;
    return *this;
}

class DebugService
{
public:
    static void Init();
    static void Done();
    static DebugService& Instance() { return *instance; }
    void Start(const DebugServiceStartParams& startParams, const std::vector<Breakpoint*>& breakpoints_);
    void Run();
    void Stop();
    void PutRequest(DebugServiceRequest* request);
    void WriteMessage(cmajor::bmp::BinaryMessage* message);
    void ProcessMessage(cmajor::bmp::BinaryMessage* message);
    std::unique_ptr<cmajor::bmp::BinaryMessage> ReadReply(uint32_t replyMessageId);
    void RunStartRequest();
    void RunStopRequest();
    void SetTargetInputEof();
    void PutTargetInputLine(const std::string& targetInputLine);
    void Terminate();
    void RunContinueRequest();
    void RunStepRequest();
    void RunNextRequest();
    void RunFinishRequest();
    void RunUntilRequest(const common::SourceLoc& sourceLocation);
    void RunBreakRequest(Breakpoint* breakpoint);
    void RunDeleteRequest(const std::string& breakpointId);
    void RunDepthRequest();
    void RunFramesRequest(int lowFrame, int highFrame);
    void RunEvaluateRequest(const std::string& expression, int requestId);
    void RunCountRequest(const std::string& expression);
    void RunEvaluateChildRequest(const std::string& expression, int start, int count);
    void SetRequestInProgress(const std::string& requestName);
    void ResetRequestInProgress();
    bool RequestInProgress(std::string& requestName);
    void ClearRequestQueue();
    void SendKillRequest();
private:
    DebugService();
    void ProcessTargetRunningRequest(const db::TargetRunningRequest& targetRunningRequest);
    void ProcessTargetInputRequest(const db::TargetInputRequest& targetInputRequest);
    void ProcessTargetOutputRequest(const db::TargetOutputRequest& targetOutputRequest);
    void ProcessLogDebugMessageRequest(const db::LogDebugMessageRequest& logDebugMessageRequest);
    void ProcessGenericDebugErrorReply(const db::GenericDebugErrorReply& genericDebugErrorReply);
    void ProcessUnexpectedDebugMessage(uint32_t messageId);
    void MakeDebugServiceStartCommand(const DebugServiceStartParams& startParams);
    void ExecuteRequest(DebugServiceRequest* request);
    static std::unique_ptr<DebugService> instance;
    std::string startCommand;
    std::string startStatus;
    std::vector<Breakpoint*> breakpoints;
    std::thread serviceThread;
    std::unique_ptr<util::Process> serverProcess;
    std::condition_variable requestAvailableOrStopping;
    std::condition_variable targetInputLineAvailableOrEof;
    std::condition_variable stopped;
    std::mutex queueMutex;
    std::mutex inputMutex;
    std::list<std::unique_ptr<DebugServiceRequest>> requestQueue;
    std::list<std::string> targetInputLines;
    bool waitingForTargetInput;
    bool targetInputEof;
    int serverPort;
    int killPort;
    util::TcpSocket socket;
    bool started;
    bool running;
    bool stop;
    bool requestInProgress;
    bool terminated;
    std::string runningRequestName;
};

std::unique_ptr<DebugService> DebugService::instance;

DebugService::DebugService() : serverPort(0), killPort(0), waitingForTargetInput(false), targetInputEof(false), started(false), running(false), stop(false), terminated(false), requestInProgress(false)
{
}

void DebugService::ProcessMessage(cmajor::bmp::BinaryMessage* message)
{
    switch (message->Id())
    {
        case db::bmpTargetRunningRequestId:
        {
            db::TargetRunningRequest* targetRunningRequest = static_cast<db::TargetRunningRequest*>(message);
            ProcessTargetRunningRequest(*targetRunningRequest);
            break;
        }
        case db::bmpTargetInputRequestId:
        {
            db::TargetInputRequest* targetInputRequest = static_cast<db::TargetInputRequest*>(message);
            ProcessTargetInputRequest(*targetInputRequest);
            break;
        }
        case db::bmpTargetOutputRequestId:
        {
            db::TargetOutputRequest* targetOutputRequest = static_cast<db::TargetOutputRequest*>(message);
            ProcessTargetOutputRequest(*targetOutputRequest);
            break;
        }
        case db::bmpLogDebugMessageRequestId:
        {
            db::LogDebugMessageRequest* logDebugMessageRequest = static_cast<db::LogDebugMessageRequest*>(message);
            ProcessLogDebugMessageRequest(*logDebugMessageRequest);
            break;
        }
        case db::bmpGenericDebugErrorReplyId:
        {
            db::GenericDebugErrorReply* genericDebugErrorReply = static_cast<db::GenericDebugErrorReply*>(message);
            ProcessGenericDebugErrorReply(*genericDebugErrorReply);
            break;
        }
        default:
        {
            ProcessUnexpectedDebugMessage(message->Id());
            break;
        }
    }
}

void DebugService::ProcessTargetRunningRequest(const db::TargetRunningRequest& targetRunningRequest)
{
    PutServiceMessage(new TargetRunningServiceMessage());
    db::TargetRunningReply targetRunningReply;
    WriteMessage(&targetRunningReply);
}

void DebugService::ProcessTargetInputRequest(const db::TargetInputRequest& targetInputRequest)
{
    std::unique_lock<std::mutex> lock(inputMutex);
    if (!targetInputEof && targetInputLines.empty())
    {
        waitingForTargetInput = true;
        PutServiceMessage(new TargetInputServiceMessage());
        targetInputLineAvailableOrEof.wait(lock, [this] { return targetInputEof || !targetInputLines.empty(); });
    }
    waitingForTargetInput = false;
    if (targetInputEof)
    {
        db::TargetInputReply targetInputReply;
        targetInputReply.eof = true;
        WriteMessage(&targetInputReply);
    }
    else
    {
        std::string targetInputLine = std::move(targetInputLines.front());
        targetInputLines.pop_front();
        db::TargetInputReply targetInputReply;
        targetInputReply.eof = false;
        targetInputReply.line = targetInputLine;
        WriteMessage(&targetInputReply);
    }
}

void DebugService::ProcessTargetOutputRequest(const db::TargetOutputRequest& targetOutputRequest)
{
    PutServiceMessage(new TargetOutputServiceMessage(targetOutputRequest));
    db::TargetOutputReply targetOutputReply;
    WriteMessage(&targetOutputReply);
}

void DebugService::ProcessLogDebugMessageRequest(const db::LogDebugMessageRequest& logDebugMessageRequest)
{
    PutOutputServiceMessage(logDebugMessageRequest.logMessage);
    db::LogDebugMessageReply logDebugMessageReply;
    WriteMessage(&logDebugMessageReply);
}

void DebugService::ProcessGenericDebugErrorReply(const db::GenericDebugErrorReply& genericDebugErrorReply)
{
    PutOutputServiceMessage("error from debug server: " + genericDebugErrorReply.errorMessage);
}

void DebugService::ProcessUnexpectedDebugMessage(uint32_t messageId)
{
    PutOutputServiceMessage("error: unexpected message id '" + std::to_string(messageId) + "' received from debug server");
}

void DebugService::Init()
{
    instance.reset(new DebugService());
}

void DebugService::Done()
{
    instance.reset();
}

void DebugService::PutRequest(DebugServiceRequest* request)
{
    std::lock_guard<std::mutex> lock(queueMutex);
    requestQueue.push_back(std::unique_ptr<DebugServiceRequest>(request));
    requestAvailableOrStopping.notify_one();
}

void DebugService::WriteMessage(cmajor::bmp::BinaryMessage* message)
{
    cmajor::bmp::WriteMessage(socket, message);
}

std::unique_ptr<cmajor::bmp::BinaryMessage> DebugService::ReadReply(uint32_t replyMessageId)
{
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(socket));
    while (replyMessage && replyMessage->Id() != replyMessageId)
    {
        ProcessMessage(replyMessage.get());
        replyMessage.reset(cmajor::bmp::ReadMessage(socket));
    }
    return replyMessage;
}

std::string MakeExecutablePath(const DebugServiceStartParams& startParams)
{
    std::string executablePath = util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(
        util::Path::GetDirectoryName(startParams.projectFilePath), "bin"), startParams.backend), startParams.config), startParams.executableName);
    return executablePath;
}

void DebugService::MakeDebugServiceStartCommand(const DebugServiceStartParams& startParams)
{
    startCommand.clear();
    startStatus.clear();
    serverPort = 0;
    killPort = 0;
    if (startParams.debugServer)
    {
        startCommand.append("cmdbd");
        startStatus = "starting debug server (cmdbd)";
    }
    else
    {
        startCommand.append("cmdb");
        startStatus = "starting debug server (cmdb)";
    }
    startCommand.append(" --server");
    serverPort = GetFreePortNumber(startParams.processName);
    if (serverPort == -1)
    {
        serverPort = defaultDebugServerPort;
    }
    startCommand.append(" --port=").append(std::to_string(serverPort));
    int sessionPort = GetFreePortNumber(startParams.processName);
    if (sessionPort == -1)
    {
        sessionPort = defaultSessionPort;
    }
    startCommand.append(" --sessionPort=").append(std::to_string(sessionPort));
    killPort = GetFreePortNumber(startParams.processName);
    if (killPort != -1)
    {
        startCommand.append(" --killPort=").append(std::to_string(killPort));
    }
    int portMapServicePort = GetPortMapServicePortNumberFromConfig();
    if (portMapServicePort != -1)
    {
        startCommand.append(" --portMapServicePort=" + std::to_string(portMapServicePort));
    }
    if (startParams.log)
    {
        startCommand.append(" --log");
    }
    if (startParams.wait)
    {
        startCommand.append(" --wait");
    }
    if (startParams.verbose)
    {
        startCommand.append(" --verbose");
    }
    startCommand.append(" \"").append(MakeExecutablePath(startParams)).append("\"");
    if (!startParams.programArguments.empty())
    {
        startCommand.append(" ").append(startParams.programArguments);
    }
    startStatus.append("...");
}

void RunService(DebugService* service)
{
    service->Run();
}

void DebugService::Start(const DebugServiceStartParams& startParams, const std::vector<Breakpoint*>& breakpoints_)
{
    terminated = false;
    requestInProgress = false;
    targetInputEof = false;
    running = false;
    stop = false;
    MakeDebugServiceStartCommand(startParams);
    breakpoints = breakpoints_;
    serviceThread = std::thread(RunService, this);
    started = true;
}

void DebugService::Run()
{
    try
    {
        running = true;
        PutOutputServiceMessage(startStatus);
        serverProcess.reset(new util::Process(startCommand, 
            util::Process::Redirections::processStdIn | util::Process::Redirections::processStdOut | util::Process::Redirections::processStdErr));
        std::string status = util::Trim(serverProcess->ReadLine(util::Process::StdHandle::stdOut));
        if (status == "debug-server-ready")
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            PutOutputServiceMessage(status);
            PutOutputServiceMessage("debug server started");
            socket.Connect("localhost", std::to_string(serverPort));
            while (!stop)
            {
                std::unique_ptr<DebugServiceRequest> request;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    requestAvailableOrStopping.wait(lock, [this] { return stop || !requestQueue.empty(); });
                    if (stop)
                    {
                        break;
                    }
                    request = std::move(requestQueue.front());
                    requestQueue.pop_front();
                }
                ExecuteRequest(request.get());
            }
            terminated = true;
            stopped.notify_all();
        }
        else
        {
            std::string errorMessage = serverProcess->ReadLine(util::Process::StdHandle::stdOut);
            PutOutputServiceMessage("error: debug server status is: " + status + ": " + errorMessage);
            serverProcess->WaitForExit();
        }
    }
    catch (const std::exception& ex)
    {
        running = false;
        PutOutputServiceMessage("debug service: error: " + std::string(ex.what()));
    }
}

struct RequestGuard
{
    RequestGuard(DebugService* service_, DebugServiceRequest* request_) : service(service_), request(request_) { service->SetRequestInProgress(request->Name()); }
    ~RequestGuard() { service->ResetRequestInProgress(); }
    DebugService* service;
    DebugServiceRequest* request;
};

void DebugService::ExecuteRequest(DebugServiceRequest* request)
{
    try
    {
        RequestGuard requestGuard(this, request);
        request->Execute();
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage("debug service: error executing " + request->Name() + ": " + ex.what());
        request->Failed(ex.what());
    }
}

void DebugService::Stop()
{
    try
    {
        if (running)
        {
            if (waitingForTargetInput)
            {
                SetTargetInputEof();
            }
            if (!requestInProgress)
            {
                PutRequest(new RunStopDebugServiceRequest());
                std::cv_status status = std::cv_status::no_timeout;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    status = stopped.wait_for(lock, std::chrono::seconds(3));
                }
                if (status == std::cv_status::timeout)
                {
                    SendKillRequest();
                }
                requestInProgress = false;
            }
            stop = true;
            requestAvailableOrStopping.notify_one();
            if (requestInProgress)
            {
                std::cv_status status = std::cv_status::no_timeout;
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    status = stopped.wait_for(lock, std::chrono::seconds(3));
                }
                if (status == std::cv_status::timeout)
                {
                    if (requestInProgress)
                    {
                        SendKillRequest();
                    }
                }
            }
            DebugService::Terminate();
        }
        if (started)
        {
            started = false;
            serviceThread.join();
        }
        ClearRequestQueue();
        PutServiceMessage(new DebugServiceStoppedServiceMessage());
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage("error stopping debug service: " + std::string(ex.what()));
    }
}

void DebugService::RunStartRequest()
{
    db::StartDebugRequest request;
    for (Breakpoint* breakpoint : breakpoints)
    {
        common::SourceLoc breakpointLocation;
        if (!breakpoint->list)
        {
            throw std::runtime_error("source file path for the breakpoint not set");
        }
        breakpointLocation.path = breakpoint->list->FilePath();
        breakpointLocation.line = breakpoint->line;
        request.breakpointLocations.push_back(breakpointLocation);
    }
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpStartDebugReplyId);
    db::StartDebugReply* reply = static_cast<db::StartDebugReply*>(replyMessage.get());
    int n = std::min(reply->breakpointInfos.size(), breakpoints.size());
    for (int i = 0; i < n; ++i)
    {
        cmajor::service::Breakpoint* breakpoint = breakpoints[i];
        breakpoint->info = reply->breakpointInfos[i];
    }
    PutServiceMessage(new StartReplyServiceMessage(*reply));
}

void DebugService::RunStopRequest()
{
    db::StopDebugRequest request;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpStopDebugReplyId);
    db::StopDebugReply* reply = static_cast<db::StopDebugReply*>(replyMessage.get());
    serverProcess->WaitForExit();
    stopped.notify_all();
    stop = true;
}

void DebugService::SetTargetInputEof()
{
    std::unique_lock<std::mutex> lock(inputMutex);
    targetInputEof = true;
    if (waitingForTargetInput)
    {
        targetInputLineAvailableOrEof.notify_one();
    }
}

void DebugService::PutTargetInputLine(const std::string& targetInputLine)
{
    std::unique_lock<std::mutex> lock(inputMutex);
    targetInputLines.push_back(targetInputLine);
    if (waitingForTargetInput)
    {
        targetInputLineAvailableOrEof.notify_one();
    }
}

void DebugService::Terminate()
{
    if (!terminated)
    {
        serverProcess->Terminate();
    }
}

void DebugService::RunContinueRequest()
{
    PutServiceMessage(new TargetRunningServiceMessage());
    db::ContinueRequest request;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpContinueReplyId);
    db::ContinueReply* reply = static_cast<db::ContinueReply*>(replyMessage.get());
    PutServiceMessage(new ContinueReplyServiceMessage(*reply));
}

void DebugService::RunStepRequest()
{
    PutServiceMessage(new TargetRunningServiceMessage());
    db::StepRequest request;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpStepReplyId);
    db::StepReply* reply = static_cast<db::StepReply*>(replyMessage.get());
    PutServiceMessage(new StepReplyServiceMessage(*reply));
}

void DebugService::RunNextRequest()
{
    PutServiceMessage(new TargetRunningServiceMessage());
    db::NextRequest request;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpNextReplyId);
    db::NextReply* reply = static_cast<db::NextReply*>(replyMessage.get());
    PutServiceMessage(new NextReplyServiceMessage(*reply));
}

void DebugService::RunFinishRequest()
{
    PutServiceMessage(new TargetRunningServiceMessage());
    db::FinishRequest request;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpFinishReplyId);
    db::FinishReply* reply = static_cast<db::FinishReply*>(replyMessage.get());
    PutServiceMessage(new FinishReplyServiceMessage(*reply));
}

void DebugService::RunUntilRequest(const common::SourceLoc& sourceLocation)
{
    PutServiceMessage(new TargetRunningServiceMessage());
    db::UntilRequest request;
    request.sourceLoc = sourceLocation;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpUntilReplyId);
    db::UntilReply* reply = static_cast<db::UntilReply*>(replyMessage.get());
    PutServiceMessage(new UntilReplyServiceMessage(*reply));
}

void DebugService::RunBreakRequest(cmajor::service::Breakpoint* breakpoint)
{
    db::BreakRequest request;
    if (!breakpoint->list)
    {
        throw std::runtime_error("source file path for the breakpoint not set");
    }
    request.breakpointLocation.path = breakpoint->list->FilePath();
    request.breakpointLocation.line = breakpoint->line;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpBreakReplyId);
    db::BreakReply* reply = static_cast<db::BreakReply*>(replyMessage.get());
    breakpoint->info = reply->breakpointInfo;
    PutServiceMessage(new BreakReplyServiceMessage(*reply));
}

void DebugService::RunDeleteRequest(const std::string& breakpointId)
{
    db::DeleteRequest request;
    request.breakpointId = breakpointId;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpDeleteReplyId);
    db::DeleteReply* reply = static_cast<db::DeleteReply*>(replyMessage.get());
    PutServiceMessage(new DeleteReplyServiceMessage(*reply));
}

void DebugService::RunDepthRequest()
{
    db::DepthRequest request;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpDepthReplyId);
    db::DepthReply* reply = static_cast<db::DepthReply*>(replyMessage.get());
    PutServiceMessage(new DepthReplyServiceMessage(*reply));
}

void DebugService::RunFramesRequest(int lowFrame, int highFrame)
{
    db::FramesRequest request;
    request.lowFrame = lowFrame;
    request.highFrame = highFrame;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpFramesReplyId);
    db::FramesReply* reply = static_cast<db::FramesReply*>(replyMessage.get());
    PutServiceMessage(new FramesReplyServiceMessage(*reply));
}

void DebugService::RunEvaluateRequest(const std::string& expression, int requestId)
{
    db::EvaluateRequest request;
    request.expression = expression;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpEvaluateReplyId);
    db::EvaluateReply* reply = static_cast<db::EvaluateReply*>(replyMessage.get());
    PutServiceMessage(new EvaluateReplyServiceMessage(*reply, requestId));
}

void DebugService::RunCountRequest(const std::string& expression)
{
    db::CountRequest request;
    request.expression = expression;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpCountReplyId);
    db::CountReply* reply = static_cast<db::CountReply*>(replyMessage.get());
    PutServiceMessage(new CountReplyServiceMessage(*reply));
}

void DebugService::RunEvaluateChildRequest(const std::string& expression, int start, int count)
{
    db::EvaluateChildRequest request;
    request.expression = expression;
    request.start = start;
    request.count = count;
    WriteMessage(&request);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage = ReadReply(db::bmpEvaluateChildReplyId);
    db::EvaluateChildReply* reply = static_cast<db::EvaluateChildReply*>(replyMessage.get());
    PutServiceMessage(new EvaluateChildReplyServiceMessage(*reply));
}

void DebugService::SetRequestInProgress(const std::string& requestName)
{
    requestInProgress = true;
    runningRequestName = requestName;
}

void DebugService::ResetRequestInProgress()
{
    requestInProgress = false;
}

bool DebugService::RequestInProgress(std::string& requestName)
{
    if (requestInProgress)
    {
        requestName = runningRequestName;
        return true;
    }
    else
    {
        return false;
    }
}

void DebugService::ClearRequestQueue()
{
    requestQueue.clear();
}

void DebugService::SendKillRequest()
{
    try
    {
        if (killPort == -1)
        {
            PutOutputServiceMessage("debug service: error: no kill port set, use Task Manager to terminate GDB.");
        }
        db::KillRequest killRequest;
        util::TcpSocket socket("localhost", std::to_string(killPort));
        cmajor::bmp::WriteMessage(socket, &killRequest);
        std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(socket));
        if (replyMessage && replyMessage->Id() == db::bmpKillReplyId)
        {
            PutOutputServiceMessage("debug service: kill request sent to cmdb");
        }
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage("debug service: could not send kill request to cmdb: " + std::string(ex.what()));
    }
    catch (...)
    {
        PutOutputServiceMessage("debug service: could not send kill request to cmdb: unknown exception occurred");
    }
}

DebugServiceRequest::~DebugServiceRequest()
{
}

RunStartDebugServiceRequest::RunStartDebugServiceRequest()
{
}

void RunStartDebugServiceRequest::Execute()
{
    DebugService::Instance().RunStartRequest();
}

std::string RunStartDebugServiceRequest::Name() const
{
    return "runStartDebugServiceRequest";
}

void RunStartDebugServiceRequest::Failed(const std::string& error)
{
    PutServiceMessage(new StartErrorServiceMessage(error));
}

StartReplyServiceMessage::StartReplyServiceMessage(const db::StartDebugReply& startReply_) : ServiceMessage(ServiceMessageKind::startDebugReply), startReply(startReply_)
{
}

StartErrorServiceMessage::StartErrorServiceMessage(const std::string& error_) : ServiceMessage(ServiceMessageKind::startError), error(error_)
{
}

RunStopDebugServiceRequest::RunStopDebugServiceRequest()
{
}

void RunStopDebugServiceRequest::Execute()
{
    DebugService::Instance().RunStopRequest();
}

std::string RunStopDebugServiceRequest::Name() const
{
    return "runStopDebugServiceRequest";
}

void RunStopDebugServiceRequest::Failed(const std::string& error)
{
    DebugService::Instance().ClearRequestQueue();
    PutServiceMessage(new DebugServiceStoppedServiceMessage());
}

TargetRunningServiceMessage::TargetRunningServiceMessage() : ServiceMessage(ServiceMessageKind::targetRunning)
{
}

TargetInputServiceMessage::TargetInputServiceMessage() : ServiceMessage(ServiceMessageKind::targetInput)
{
}

TargetOutputServiceMessage::TargetOutputServiceMessage(const db::TargetOutputRequest& targetOutputRequest_) : 
    ServiceMessage(ServiceMessageKind::targetOutput), targetOutputRequest(targetOutputRequest_)
{
}

DebugServiceStoppedServiceMessage::DebugServiceStoppedServiceMessage() : ServiceMessage(ServiceMessageKind::debugServiceStopped)
{
}

RunContinueDebugServiceRequest::RunContinueDebugServiceRequest()
{
}

void RunContinueDebugServiceRequest::Execute()
{
    DebugService::Instance().RunContinueRequest();
}

std::string RunContinueDebugServiceRequest::Name() const
{
    return "runContinueDebugServiceRequest";
}

void RunContinueDebugServiceRequest::Failed(const std::string& error)
{
}

ContinueReplyServiceMessage::ContinueReplyServiceMessage(const db::ContinueReply& continueReply_) : 
    ServiceMessage(ServiceMessageKind::continueReply), continueReply(continueReply_)
{
}

RunNextDebugServiceRequest::RunNextDebugServiceRequest()
{
}

void RunNextDebugServiceRequest::Execute()
{
    DebugService::Instance().RunNextRequest();
}

std::string RunNextDebugServiceRequest::Name() const
{
    return "runNextDebugServiceRequest";
}

void RunNextDebugServiceRequest::Failed(const std::string& error)
{
}

NextReplyServiceMessage::NextReplyServiceMessage(const db::NextReply& nextReply_) : ServiceMessage(ServiceMessageKind::nextReply), nextReply(nextReply_)
{
}

RunStepDebugServiceRequest::RunStepDebugServiceRequest()
{
}

void RunStepDebugServiceRequest::Execute()
{
    DebugService::Instance().RunStepRequest();
}

std::string RunStepDebugServiceRequest::Name() const
{
    return "runStepDebugServiceRequest";
}

void RunStepDebugServiceRequest::Failed(const std::string& error)
{
}

StepReplyServiceMessage::StepReplyServiceMessage(const db::StepReply& stepReply_) : ServiceMessage(ServiceMessageKind::stepReply), stepReply(stepReply_)
{
}

RunFinishDebugServiceRequest::RunFinishDebugServiceRequest()
{
}

void RunFinishDebugServiceRequest::Execute()
{
    DebugService::Instance().RunFinishRequest();
}

std::string RunFinishDebugServiceRequest::Name() const
{
    return "runFinishDebugServiceRequest";
}

void RunFinishDebugServiceRequest::Failed(const std::string& error)
{
}

FinishReplyServiceMessage::FinishReplyServiceMessage(const db::FinishReply& finishReply_) : ServiceMessage(ServiceMessageKind::finishReply), finishReply(finishReply_)
{
}

RunUntilDebugServiceRequest::RunUntilDebugServiceRequest(const common::SourceLoc& sourceLocation_) : sourceLocation(sourceLocation_)
{
}

void RunUntilDebugServiceRequest::Execute()
{
    DebugService::Instance().RunUntilRequest(sourceLocation);
}

std::string RunUntilDebugServiceRequest::Name() const
{
    return "runUntilDebugServiceRequest";
}

void RunUntilDebugServiceRequest::Failed(const std::string& error)
{
}

UntilReplyServiceMessage::UntilReplyServiceMessage(const db::UntilReply& untilReply_) : ServiceMessage(ServiceMessageKind::untilReply), untilReply(untilReply_)
{
}

RunBreakDebugServiceRequest::RunBreakDebugServiceRequest(Breakpoint* breakpoint_) : breakpoint(breakpoint_)
{
}

void RunBreakDebugServiceRequest::Execute()
{
    DebugService::Instance().RunBreakRequest(breakpoint);
}

std::string RunBreakDebugServiceRequest::Name() const
{
    return "runBreakDebugServiceRequest";
}

void RunBreakDebugServiceRequest::Failed(const std::string& error)
{
}

BreakReplyServiceMessage::BreakReplyServiceMessage(const db::BreakReply& breakReply_) : ServiceMessage(ServiceMessageKind::breakReply), breakReply(breakReply_)
{
}

RunDeleteDebugServiceRequest::RunDeleteDebugServiceRequest(const std::string& breakpointId_) : breakpointId(breakpointId_)
{
}

void RunDeleteDebugServiceRequest::Execute()
{
    DebugService::Instance().RunDeleteRequest(breakpointId);
}

std::string RunDeleteDebugServiceRequest::Name() const
{
    return "runDeleteDebugServiceRequest";
}

void RunDeleteDebugServiceRequest::Failed(const std::string& error)
{
}

DeleteReplyServiceMessage::DeleteReplyServiceMessage(const db::DeleteReply& deleteReply_) : ServiceMessage(ServiceMessageKind::deleteReply), deleteReply(deleteReply_)
{
}

RunDepthDebugServiceRequest::RunDepthDebugServiceRequest()
{
}

void RunDepthDebugServiceRequest::Execute()
{
    DebugService::Instance().RunDepthRequest();
}

std::string RunDepthDebugServiceRequest::Name() const
{
    return "runDepthDebugServiceRequest";
}

void RunDepthDebugServiceRequest::Failed(const std::string& error)
{
}

DepthReplyServiceMessage::DepthReplyServiceMessage(const db::DepthReply& depthReply_) : ServiceMessage(ServiceMessageKind::depthReply), depthReply(depthReply_)
{
}

RunFramesDebugServiceRequest::RunFramesDebugServiceRequest(int lowFrame_, int highFrame_) : lowFrame(lowFrame_), highFrame(highFrame_)
{
}

void RunFramesDebugServiceRequest::Execute()
{
    DebugService::Instance().RunFramesRequest(lowFrame, highFrame);
}

std::string RunFramesDebugServiceRequest::Name() const
{
    return "runFrameDebugServiceRequest";
}

void RunFramesDebugServiceRequest::Failed(const std::string& error)
{
}

FramesReplyServiceMessage::FramesReplyServiceMessage(const db::FramesReply& framesReply_) : ServiceMessage(ServiceMessageKind::framesReply), framesReply(framesReply_)
{
}

RunEvaluateDebugServiceRequest::RunEvaluateDebugServiceRequest(const std::string& expression_, int requestId_) : expression(expression_), requestId(requestId_)
{
}

void RunEvaluateDebugServiceRequest::Execute()
{
    DebugService::Instance().RunEvaluateRequest(expression, requestId);
}

std::string RunEvaluateDebugServiceRequest::Name() const
{
    return "runEvaluateDebugServiceRequest";
}

void RunEvaluateDebugServiceRequest::Failed(const std::string& error)
{
}

EvaluateReplyServiceMessage::EvaluateReplyServiceMessage(const db::EvaluateReply& evaluateReply_, int requestId_) :
    ServiceMessage(ServiceMessageKind::evaluateReply), evaluateReply(evaluateReply_), requestId(requestId_)
{
}

RunCountDebugServiceRequest::RunCountDebugServiceRequest(const std::string& expression_) : expression(expression_)
{
}

void RunCountDebugServiceRequest::Execute()
{
    DebugService::Instance().RunCountRequest(expression);
}

std::string RunCountDebugServiceRequest::Name() const
{
    return "runCountDebugServiceRequest";
}

void RunCountDebugServiceRequest::Failed(const std::string& error)
{
}

CountReplyServiceMessage::CountReplyServiceMessage(const db::CountReply& countReply_) : ServiceMessage(ServiceMessageKind::countReply), countReply(countReply_)
{
}

RunEvaluateChildRequest::RunEvaluateChildRequest(const std::string& expression_, int start_, int count_) : expression(expression_), start(start_), count(count_)
{
}

void RunEvaluateChildRequest::Execute()
{
    DebugService::Instance().RunEvaluateChildRequest(expression, start, count);
}

std::string RunEvaluateChildRequest::Name() const
{
    return "runEvaluateChildRequest";
}

void RunEvaluateChildRequest::Failed(const std::string& error)
{
}

EvaluateChildReplyServiceMessage::EvaluateChildReplyServiceMessage(const db::EvaluateChildReply& evaluateChildReply_) :
    ServiceMessage(ServiceMessageKind::evaluateChildReply), evaluateChildReply(evaluateChildReply_)
{
}

void InitDebugService()
{
    DebugService::Init();
}

void DoneDebugService()
{
    DebugService::Done();
}

void StartDebugService(DebugServiceStartParams& startParams, const std::vector<Breakpoint*>& breakpoints)
{
    DebugService::Instance().Start(startParams, breakpoints);
    DebugService::Instance().PutRequest(new RunStartDebugServiceRequest());
}

void StopDebugService()
{
    DebugService::Instance().Stop();
}

void Continue()
{
    DebugService::Instance().PutRequest(new RunContinueDebugServiceRequest());
}

void Next()
{
    DebugService::Instance().PutRequest(new RunNextDebugServiceRequest());
}

void Step()
{
    DebugService::Instance().PutRequest(new RunStepDebugServiceRequest());
}

void Finish()
{
    DebugService::Instance().PutRequest(new RunFinishDebugServiceRequest());
}

void Until(const common::SourceLoc& sourceLocation)
{
    DebugService::Instance().PutRequest(new RunUntilDebugServiceRequest(sourceLocation));
}

void Break(Breakpoint* breakpoint)
{
    DebugService::Instance().PutRequest(new RunBreakDebugServiceRequest(breakpoint));
}

void Delete(const std::string& breakpointId)
{
    DebugService::Instance().PutRequest(new RunDeleteDebugServiceRequest(breakpointId));
}

void Depth()
{
    DebugService::Instance().PutRequest(new RunDepthDebugServiceRequest());
}

void Frames(int lowFrame, int highFrame)
{
    DebugService::Instance().PutRequest(new RunFramesDebugServiceRequest(lowFrame, highFrame));
}

void Evaluate(const std::string& expression, int requestId)
{
    DebugService::Instance().PutRequest(new RunEvaluateDebugServiceRequest(expression, requestId));
}

void Count(const std::string& expression)
{
    DebugService::Instance().PutRequest(new RunCountDebugServiceRequest(expression));
}

void EvaluateChild(const std::string& expression, int start, int count)
{
    DebugService::Instance().PutRequest(new RunEvaluateChildRequest(expression, start, count));
}

void SetTargetInputEof()
{
    DebugService::Instance().SetTargetInputEof();
}

void PutTargetInputLine(const std::string& targetInputLine)
{
    DebugService::Instance().PutTargetInputLine(targetInputLine);
}

bool DebugRequestInProgress(std::string& requestName)
{
    return DebugService::Instance().RequestInProgress(requestName);
}

} // namespace cmajor::service
