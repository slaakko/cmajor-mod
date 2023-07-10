// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.service.build.service;

import cmajor.service.port.map.service;
import cmajor.service.message;
import util;

namespace cmajor::service{

const int keepAliveIntervalSeconds = 60;
const int defaultBuildServerPort = 55001;
const int defaultKeepAliveServerPort = 55002;

BuildServiceStartParams::BuildServiceStartParams() : debugServer(false), log(false), wait(false)
{
}

BuildServiceStartParams& BuildServiceStartParams::ProcessName(const std::string& processName_)
{
    processName = processName_;
    return *this;
}

BuildServiceStartParams& BuildServiceStartParams::DebugServer(bool debugServer_)
{
    debugServer = debugServer_;
    return *this;
}

BuildServiceStartParams& BuildServiceStartParams::Log(bool log_)
{
    log = log_;
    return *this;
}

BuildServiceStartParams& BuildServiceStartParams::Wait(bool wait_)
{
    wait = wait_;
    return *this;
}

class BuildService
{
public:
    static BuildService& Instance();
    void Start(BuildServiceStartParams& startParams_);
    bool Running();
    void Stop();
    void Stop(bool log);
    void Run();
    void Quit();
    void KeepAlive();
    void Put(BuildServiceRequest* request);
    void ProcessRequests();
    void ExecuteRequest(BuildServiceRequest* request);
    void ProcessBuildRequest(bs::BuildRequest& buildRequest);
    void ProcessGetDefinitionRequest(bs::GetDefinitionRequest& getDefinitionRequest);
private:
    BuildService();
    BuildServiceStartParams startParams;
    std::string MakeBuildServerStartCommand(std::string& startStatus);
    void StartKeepAliveThread();
    void StopKeepAliveThread();
    void SendReceiveKeepAlive();
    bool running;
    bool exiting;
    bool keepAliveThreadStarted;
    bool keepAliveThreadRunning;
    bool stopKeepAlives;
    bool serviceThreadStarted;
    bool requestInProgress;
    bool buildServerProcessTerminated;
    int serverPort;
    int keepAliveServerPort;
    std::thread serviceThread;
    std::thread keepAliveThread;
    std::unique_ptr<util::Process> buildServerProcess;
    std::mutex keepAliveMutex;
    std::condition_variable stopKeepAliveVar;
    std::mutex requestMutex;
    std::condition_variable requestAvailableOrExiting;
    std::list<std::unique_ptr<BuildServiceRequest>> requestQueue;
};

BuildService& BuildService::Instance()
{
    static BuildService instance;
    return instance;
}

BuildService::BuildService() :
    running(false),
    exiting(false),
    requestInProgress(false),
    keepAliveThreadStarted(false),
    keepAliveThreadRunning(false),
    stopKeepAlives(false),
    serviceThreadStarted(false),
    buildServerProcessTerminated(false),
    serverPort(-1),
    keepAliveServerPort(-1)
{
}

std::string BuildService::MakeBuildServerStartCommand(std::string& startStatus)
{
    startStatus.clear();
    std::string startCommand = "cmbs";
    startStatus = "starting build server (cmbs):";
    if (startParams.debugServer)
    {
        startCommand = "cmbsd";
        startStatus = "starting build server (cmbsd):";
    }
    serverPort = GetFreePortNumber(startParams.processName);
    keepAliveServerPort = -1;
    if (serverPort == -1)
    {
        serverPort = defaultBuildServerPort;
        startStatus.append(" port=" + std::to_string(serverPort) + " (default)");
        keepAliveServerPort = defaultKeepAliveServerPort;
        startStatus.append(", keep alive port=" + std::to_string(keepAliveServerPort) + " (default)");
    }
    else
    {
        startStatus.append(" port=" + std::to_string(serverPort));
        keepAliveServerPort = GetFreePortNumber(startParams.processName);
    }
    if (keepAliveServerPort == -1)
    {
        keepAliveServerPort = defaultKeepAliveServerPort;
        startStatus.append(", keep alive port=" + std::to_string(keepAliveServerPort) + " (default)");
    }
    else
    {
        startStatus.append(", keep alive port=" + std::to_string(keepAliveServerPort));
    }
    startCommand.append(" --port=" + std::to_string(serverPort));
    startCommand.append(" --keepAliveServerPort=" + std::to_string(keepAliveServerPort));
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
    startStatus.append("...");
    return startCommand;
}

void BuildService::KeepAlive()
{
    try
    {
        keepAliveThreadRunning = true;
        while (!stopKeepAlives)
        {
            std::unique_lock<std::mutex> lock(keepAliveMutex);
            if (stopKeepAliveVar.wait_for(lock, std::chrono::seconds(keepAliveIntervalSeconds), [this] { return stopKeepAlives; }))
            {
                keepAliveThreadRunning = false;
                return;
            }
            SendReceiveKeepAlive();
        }
    }
    catch (const std::exception& ex)
    {
        keepAliveThreadRunning = false;
        PutOutputServiceMessage("error: build service keep alive: " + std::string(ex.what()));
    }
}

void BuildService::SendReceiveKeepAlive()
{
    try
    {
        util::TcpSocket socket("localhost", std::to_string(keepAliveServerPort));
        bs::KeepAliveBuildRequest keepAliveRequest;
        cmajor::bmp::WriteMessage(socket, &keepAliveRequest);
        std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(socket));
        if (replyMessage)
        {
            if (replyMessage->Id() != bs::bmpKeepAliveBuildReplyId)
            {
                throw std::runtime_error("bs::KeepAliveBuildReply expected, message id=" + std::to_string(replyMessage->Id()) + " received");
            }
        }
        else
        { 
            throw std::runtime_error("bs::KeepAliveBuildReply expected, null message received");
        }
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage("error: build service send/receive keep alive: " + std::string(ex.what()));
    }
}

void RunKeepAliveThread()
{
    BuildService::Instance().KeepAlive();
}

void BuildService::StartKeepAliveThread()
{
    if (!keepAliveThreadStarted)
    {
        keepAliveThreadStarted = true;
        stopKeepAlives = false;
        keepAliveThread = std::thread(RunKeepAliveThread);
    }
}

void BuildService::StopKeepAliveThread()
{
    try
    {
        if (keepAliveThreadRunning)
        {
            stopKeepAlives = true;
            stopKeepAliveVar.notify_one();
        }
        if (keepAliveThreadStarted)
        {
            keepAliveThread.join();
            keepAliveThreadStarted = false;
        }
    }
    catch (...)
    {
    }
}

void BuildService::Put(BuildServiceRequest* request)
{
    std::unique_lock<std::mutex> lock(requestMutex);
    requestQueue.push_back(std::unique_ptr<BuildServiceRequest>(request));
    requestAvailableOrExiting.notify_one();
}

void BuildService::ProcessRequests()
{
    try
    {
        while (!exiting)
        {
            std::unique_ptr<BuildServiceRequest> request;
            {
                std::unique_lock<std::mutex> lock(requestMutex);
                requestAvailableOrExiting.wait(lock, [this] { return exiting || !requestQueue.empty(); });
                if (exiting)
                {
                    running = false;
                    return;
                }
                request = std::move(requestQueue.front());
                requestQueue.pop_front();
            }
            if (request)
            {
                ExecuteRequest(request.get());
            }
        }
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage("error: build service process requests: " + std::string(ex.what()));
        running = false;
    }
}

struct BuildServiceRequestGuard
{
    BuildServiceRequestGuard(bool& requestInProgress_) : requestInProgress(requestInProgress_)
    {
        requestInProgress = true;
    }
    ~BuildServiceRequestGuard()
    {
        requestInProgress = false;
    }
    bool& requestInProgress;
};

void BuildService::ExecuteRequest(BuildServiceRequest* request)
{
    BuildServiceRequestGuard requestGuard(requestInProgress);
    try
    {
        request->Execute();
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage("build service: error executing " + request->Name() + ": " + std::string(ex.what()));
        request->Failed(ex.what());
        throw;
    }
}

void BuildService::ProcessBuildRequest(bs::BuildRequest& buildRequest)
{
    util::TcpSocket socket("localhost", std::to_string(serverPort));
    cmajor::bmp::WriteMessage(socket, &buildRequest);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(socket));
    while (replyMessage && replyMessage->Id() == bs::bmpLogBuildMessageRequestId)
    {
        bs::LogBuildMessageRequest* logRequest = static_cast<bs::LogBuildMessageRequest*>(replyMessage.get());
        PutOutputServiceMessage(logRequest->message);
        bs::LogBuildMessageReply logReply;
        logReply.ok = true;
        cmajor::bmp::WriteMessage(socket, &logReply);
        replyMessage.reset(cmajor::bmp::ReadMessage(socket));
    }
    if (replyMessage)
    {
        if (replyMessage->Id() == bs::bmpBuildReplyId)
        {
            bs::BuildReply* buildReply = static_cast<bs::BuildReply*>(replyMessage.get());
            PutServiceMessage(new BuildReplyServiceMessage(*buildReply));
        }
        else if (replyMessage->Id() == bs::bmpGenericBuildErrorReplyId)
        {
            bs::GenericBuildErrorReply* buildErrorReply = static_cast<bs::GenericBuildErrorReply*>(replyMessage.get());
            throw std::runtime_error("generic build error received: " + buildErrorReply->error);
        }
        else
        {
            throw std::runtime_error("unknown build message id '" + std::to_string(replyMessage->Id()) + "' received");
        }
    }
    else
    {
        throw std::runtime_error("build reply or log request expected, null message received");
    }
}

void BuildService::ProcessGetDefinitionRequest(bs::GetDefinitionRequest& getDefinitionRequest)
{
    util::TcpSocket socket("localhost", std::to_string(serverPort));
    cmajor::bmp::WriteMessage(socket, &getDefinitionRequest);
    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(socket));
    if (replyMessage)
    {
        if (replyMessage->Id() == bs::bmpGetDefinitionReplyId)
        {
            bs::GetDefinitionReply* reply = static_cast<bs::GetDefinitionReply*>(replyMessage.get());
            PutServiceMessage(new GetDefinitionReplyServiceMessage(*reply));
        }
        else
        {
            throw std::runtime_error("get definition reply expected, message id '" + std::to_string(replyMessage->Id()) + "' received");
        }
    }
    else
    {
        throw std::runtime_error("get definition reply expected, null message received");
    }
}

void BuildService::Run()
{
    try
    {
        running = true;
        StartKeepAliveThread();
        std::string startStatus;
        std::string buildServerStartCommand = MakeBuildServerStartCommand(startStatus);
        PutOutputServiceMessage(startStatus);
        buildServerProcessTerminated = false;
        buildServerProcess.reset(new util::Process(buildServerStartCommand, 
            util::Process::Redirections::processStdIn | util::Process::Redirections::processStdOut | util::Process::Redirections::processStdErr));
        try
        {
            if (!exiting)
            {
                std::string status = util::Trim(buildServerProcess->ReadLine(util::Process::StdHandle::stdOut));
                if (status == "build-server-ready")
                {
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    PutOutputServiceMessage(status);
                    PutOutputServiceMessage("build server started");
                    ProcessRequests();
                }
                else
                {
                    std::string errorMessage = buildServerProcess->ReadLine(util::Process::StdHandle::stdOut);
                    PutOutputServiceMessage("error: build server status is: " + status + ": " + errorMessage);
                }
            }
            if (!buildServerProcessTerminated)
            {
                buildServerProcessTerminated = true;
                buildServerProcess->Terminate();
            }
            buildServerProcess.reset();
        }
        catch (const std::exception& ex)
        {
            if (!buildServerProcessTerminated)
            {
                buildServerProcessTerminated = true;
                buildServerProcess->Terminate();
            }
            buildServerProcess.reset();
            PutOutputServiceMessage("error: build service run: " + std::string(ex.what()));
            StopKeepAliveThread();
            running = false;
        }
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage("error: could not start build server: " + std::string(ex.what()));
        StopKeepAliveThread();
        running = false;
    }
}

void RunBuildService()
{
    BuildService::Instance().Run();
}

void BuildService::Start(BuildServiceStartParams& startParams_)
{
    startParams = startParams_;
    exiting = false;
    running = false;
    serviceThread = std::thread(RunBuildService);
    serviceThreadStarted = true;
}

void BuildService::Quit()
{
    try
    {
        if (!buildServerProcess) return;
        if (buildServerProcessTerminated) return;
        buildServerProcessTerminated = true;
        buildServerProcess->Terminate();
        running = false;
    }
    catch (...)
    {
    }
}

void BuildService::Stop()
{
    Stop(false);
}

bool BuildService::Running()
{
    running = buildServerProcess && buildServerProcess->Running();
    return running;
}

void BuildService::Stop(bool log)
{
    try
    {
        if (log)
        {
            PutOutputServiceMessage("stopping build server...");
        }
        exiting = true;
        if (serviceThreadStarted)
        {
            if (running)
            {
                if (keepAliveThreadStarted)
                {
                    StopKeepAliveThread();
                }
                Quit();
                requestAvailableOrExiting.notify_one();
                running = false;
            }
            serviceThread.join();
            serviceThreadStarted = false;
        }
        else
        {
            running = false;
        }
        if (log)
        {
            PutOutputServiceMessage("build server stopped");
        }
    }
    catch (...)
    {
    }
}

BuildServiceRequest::~BuildServiceRequest()
{
}

RunBuildRequest::RunBuildRequest(const bs::BuildRequest& buildRequest_) : buildRequest(buildRequest_)
{
}

void RunBuildRequest::Execute()
{
    BuildService::Instance().ProcessBuildRequest(buildRequest);
}

void RunBuildRequest::Failed(const std::string& error)
{
    PutServiceMessage(new BuildErrorServiceMessage(error));
}

RunGetDefinitionRequest::RunGetDefinitionRequest(const bs::GetDefinitionRequest& getDefinitionRequest_) : getDefinitionRequest(getDefinitionRequest_)
{
}

void RunGetDefinitionRequest::Execute()
{
    BuildService::Instance().ProcessGetDefinitionRequest(getDefinitionRequest);
}

void RunGetDefinitionRequest::Failed(const std::string& error)
{
    PutServiceMessage(new GetDefinitionErrorServiceMessage(error));
}

BuildReplyServiceMessage::BuildReplyServiceMessage(const bs::BuildReply& buildReply_) : ServiceMessage(ServiceMessageKind::buildReply), buildReply(buildReply_)
{
}

BuildErrorServiceMessage::BuildErrorServiceMessage(const std::string& error_) : ServiceMessage(ServiceMessageKind::buildError), error(error_)
{
}

GetDefinitionReplyServiceMessage::GetDefinitionReplyServiceMessage(const bs::GetDefinitionReply& getDefinitionReply_) :
    ServiceMessage(ServiceMessageKind::getDefinitionReply), getDefinitionReply(getDefinitionReply_)
{
}

GetDefinitionErrorServiceMessage::GetDefinitionErrorServiceMessage(const std::string& error_) : ServiceMessage(ServiceMessageKind::getDefinitionError), error(error_)
{
}

StopBuildServiceMessage::StopBuildServiceMessage() : ServiceMessage(ServiceMessageKind::stopBuild)
{
}

void StartBuildService(BuildServiceStartParams& startParams)
{
    BuildService::Instance().Start(startParams);
}

void EnqueueBuildServiceRequest(BuildServiceRequest* request)
{
    BuildService::Instance().Put(request);
}

bool BuildServiceRunning()
{
    return BuildService::Instance().Running();
}

void StopBuildService(bool log)
{
    BuildService::Instance().Stop(log);
}

} // namespace cmajor::service
