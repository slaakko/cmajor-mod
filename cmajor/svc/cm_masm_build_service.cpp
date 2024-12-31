// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.cm_masm_build.service;

import soul.xml.xpath;
import soul.xml.dom;
import soul.xml.dom.parser;
import util;

namespace cmajor::service {

int defaultOPort = 53000;
int defaultIPort = 53001;

std::unique_ptr<soul::xml::Document> MakeStopRequest()
{
    std::unique_ptr<soul::xml::Document> stopRequest = soul::xml::MakeDocument();
    soul::xml::Element* rootElement = soul::xml::MakeElement("request");
    rootElement->SetAttribute("kind", "stop");
    stopRequest->AppendChild(rootElement);
    return stopRequest;
}

std::unique_ptr<soul::xml::Document> MakeExitRequest()
{
    std::unique_ptr<soul::xml::Document> exitRequest = soul::xml::MakeDocument();
    soul::xml::Element* rootElement = soul::xml::MakeElement("request");
    rootElement->SetAttribute("kind", "exit");
    exitRequest->AppendChild(rootElement);
    return exitRequest;
}

std::unique_ptr<soul::xml::Document> MakeExitReply()
{
    std::unique_ptr<soul::xml::Document> exitReply = soul::xml::MakeDocument();
    soul::xml::Element* rootElement = soul::xml::MakeElement("reply");
    rootElement->SetAttribute("kind", "exit");
    exitReply->AppendChild(rootElement);
    return exitReply;
}

std::unique_ptr<soul::xml::Document> MakeLogReply()
{
    std::unique_ptr<soul::xml::Document> logReply = soul::xml::MakeDocument();
    soul::xml::Element* rootElement = soul::xml::MakeElement("reply");
    rootElement->SetAttribute("kind", "log");
    logReply->AppendChild(rootElement);
    return logReply;
}

std::unique_ptr<soul::xml::Document> MakeBuildResultReply()
{
    std::unique_ptr<soul::xml::Document> buildResultReply = soul::xml::MakeDocument();
    soul::xml::Element* rootElement = soul::xml::MakeElement("reply");
    rootElement->SetAttribute("kind", "buildResult");
    buildResultReply->AppendChild(rootElement);
    return buildResultReply;
}

cmajor::info::bs::BuildResult ReadBuildResultRequest(soul::xml::Element* buildResultRequest)
{
    cmajor::info::bs::BuildResult buildResult;
    buildResult.success = buildResultRequest->GetAttribute("success") == "true";
    std::unique_ptr<soul::xml::xpath::NodeSet> compileErrors = soul::xml::xpath::EvaluateToNodeSet("compileError", buildResultRequest);
    int n = compileErrors->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* compileErrorNode = compileErrors->GetNode(i);
        if (compileErrorNode->IsElementNode())
        {
            cmajor::info::bs::CompileError compileError;
            soul::xml::Element* compileErrorElement = static_cast<soul::xml::Element*>(compileErrorNode);
            compileError.message = compileErrorElement->GetAttribute("message");
            compileError.project = compileErrorElement->GetAttribute("project");
            compileError.file = compileErrorElement->GetAttribute("file");
            std::string line = compileErrorElement->GetAttribute("line");
            if (!line.empty())
            {
                compileError.line = std::stoi(line);
            }
            std::string scol = compileErrorElement->GetAttribute("scol");
            if (!scol.empty())
            {
                compileError.scol = std::stoi(scol);
            }
            std::string ecol = compileErrorElement->GetAttribute("ecol");
            if (!ecol.empty())
            {
                compileError.ecol = std::stoi(ecol);
            }
            buildResult.errors.push_back(compileError);
        }
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> warnings = soul::xml::xpath::EvaluateToNodeSet("warning", buildResultRequest);
    int nw = warnings->Count();
    for (int i = 0; i < nw; ++i)
    {
        soul::xml::Node* warningNode = warnings->GetNode(i);
        if (warningNode->IsElementNode())
        {
            cmajor::info::bs::Warning warning;
            soul::xml::Element* warningElement = static_cast<soul::xml::Element*>(warningNode);
            std::string number = warningElement->GetAttribute("number");
            if (!number.empty())
            {
                warning.number = std::stoi(number);
            }
            warning.message = warningElement->GetAttribute("message");
            warning.project = warningElement->GetAttribute("project");
            warning.file = warningElement->GetAttribute("file");
            std::string line = warningElement->GetAttribute("line");
            if (!line.empty())
            {
                warning.line = std::stoi(line);
            }
            std::string scol = warningElement->GetAttribute("scol");
            if (!scol.empty())
            {
                warning.scol = std::stoi(scol);
            }
            std::string ecol = warningElement->GetAttribute("ecol");
            if (!ecol.empty())
            {
                warning.ecol = std::stoi(ecol);
            }
            buildResult.warnings.push_back(warning);
        }
    }
    return buildResult;
}

class CmMasmBuildServer
{
public:
    static CmMasmBuildServer& Instance();
    void Start(int iport_, int oport_);
    void Stop();
    void Run();
    void ClientRun(util::TcpSocket&& socket_);
    void WaitRunning();
    void WaitResultOrExit();
    bool ResultAvailableOrExiting() const { return resultAvailable || exiting; }
    cmajor::info::bs::BuildResult GetResult() { return std::move(result); }
    cmajor::info::bs::BuildResult& Result() { return result; }
    void SetResultAvailable() { resultAvailable = true; }
    void NotifyVar() { var.notify_one(); }
private:
    CmMasmBuildServer();
    int iport;
    int oport;
    bool exiting;
    bool running;
    bool resultAvailable;
    std::jthread thread;
    std::vector<std::jthread> clientThreads;
    std::mutex mtx;
    std::condition_variable var;
    std::condition_variable runningVar;
    std::condition_variable exitedVar;
    cmajor::info::bs::BuildResult result;
};

CmMasmBuildServer& CmMasmBuildServer::Instance()
{
    static CmMasmBuildServer instance;
    return instance;
}

void RunServer(CmMasmBuildServer* server)
{
    try
    {
        server->Run();
    }
    catch (const std::exception& ex)
    {
        cmajor::info::bs::CompileError error;
        error.message = util::PlatformStringToUtf8(ex.what());
        server->Result().errors.push_back(error);
        server->SetResultAvailable();
        server->NotifyVar();
    }
}

CmMasmBuildServer::CmMasmBuildServer() : iport(-1), oport(-1), exiting(false), running(false), resultAvailable(false)
{
}

void CmMasmBuildServer::WaitRunning()
{
    std::unique_lock<std::mutex> lock(mtx);
    runningVar.wait(lock, [this] { return running; });
}

void CmMasmBuildServer::WaitResultOrExit()
{
    std::unique_lock<std::mutex> lock(mtx);
    var.wait(lock, [this]{ return ResultAvailableOrExiting(); });
}

void CmMasmBuildServer::Start(int iport_, int oport_)
{
    exiting = false;
    running = false;
    resultAvailable = false;
    iport = iport_;
    oport = oport_;
    thread = std::jthread{ RunServer, this };
}

void CmMasmBuildServer::Stop()
{
    try
    {
        if (running)
        {
            util::TcpSocket socket("127.0.0.1", std::to_string(oport));
            std::unique_ptr<soul::xml::Document> exitRequest = MakeExitRequest();
            soul::xml::SendDocument(socket, *exitRequest);
            std::unique_lock<std::mutex> lock(mtx);
            exitedVar.wait(lock, [this] { return !running; });
            socket.Close();
        }
    }
    catch (const std::exception& ex)
    {
        cmajor::info::bs::CompileError error;
        error.message = util::PlatformStringToUtf8(ex.what());
        result.errors.push_back(error);
        resultAvailable = true;
        var.notify_one();
    }
}

void CmMasmBuildServer::Run()
{
    util::TcpSocket socket;
    socket.Bind(oport);
    socket.Listen(10);
    bool first = true;
    while (!exiting)
    {
        if (first)
        {
            running = true;
            runningVar.notify_one();
            first = false;
        }
        try
        {
            util::TcpSocket clientSocket = socket.Accept();
            ClientRun(std::move(clientSocket));
        }
        catch (const std::exception& ex)
        {
            cmajor::info::bs::CompileError error;
            error.message = util::PlatformStringToUtf8(ex.what());
            result.errors.push_back(error);
            resultAvailable = true;
            var.notify_one();
        }
    }
    socket.Close();
    running = false;
    exitedVar.notify_one();
}

void CmMasmBuildServer::ClientRun(util::TcpSocket&& socket_)
{
    util::TcpSocket socket(std::move(socket_));
    while (!exiting)
    {
        std::unique_ptr<soul::xml::Document> request = soul::xml::ReceiveDocument(socket);
        if (!request)
        {
            break;
        }
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/request", request.get());
        if (nodeSet->Count() > 0)
        {
            soul::xml::Node* rootNode = nodeSet->GetNode(0);
            if (rootNode->IsElementNode())
            {
                soul::xml::Element* requestElement = static_cast<soul::xml::Element*>(rootNode);
                std::string kind = requestElement->GetAttribute("kind");
                if (kind == "exit")
                {
                    exiting = true;
                    var.notify_one();
                }
                else if (kind == "log")
                {
                    std::string line = requestElement->GetAttribute("line");
                    util::LogMessage(-1, line);
                    std::unique_ptr<soul::xml::Document> logReply = MakeLogReply();
                    soul::xml::SendDocument(socket, *logReply);
                }
                else if (kind == "buildResult")
                {
                    result = ReadBuildResultRequest(requestElement);
                    std::unique_ptr<soul::xml::Document> buildResultReply = MakeBuildResultReply();
                    soul::xml::SendDocument(socket, *buildResultReply);
                    resultAvailable = true;
                    var.notify_one();
                }
            }
        }
    }
    socket.Close();
}

void RunCmMasmCompiler(const cmajor::info::bs::BuildCommand& buildCommand)
{
    std::string commandLine("cmcmc");
    commandLine.append(" --client");
    //commandLine.append(" --wait");
    if (buildCommand.verbose)
    {
        commandLine.append(" --verbose");
    }
    commandLine.append(" --config=").append(buildCommand.config);
    commandLine.append(" --opt=").append(buildCommand.optimizationLevel);
    commandLine.append(" --iport=").append(std::to_string(defaultIPort));
    commandLine.append(" --oport=").append(std::to_string(defaultOPort));
    if (buildCommand.buildAllDependencies)
    {
        commandLine.append(" --all");
    }
    if (buildCommand.clean)
    {
        commandLine.append(" --clean");
    }
    if (buildCommand.rebuild)
    {
        commandLine.append(" --rebuild");
    }
    for (const auto& define : buildCommand.defines)
    {
        commandLine.append(" --define ").append(define);
    }
    if (buildCommand.singleThreadedCompile)
    {
        commandLine.append(" --single-threaded");
    }
    commandLine.append(" ").append(buildCommand.filePath);
    util::ExecuteResult executeResult = util::Execute(commandLine);
}

cmajor::info::bs::BuildResult BuildWithCmMasmCompiler(const cmajor::info::bs::BuildCommand& buildCommand)
{
    CmMasmBuildServer::Instance().Start(defaultIPort, defaultOPort);
    CmMasmBuildServer::Instance().WaitRunning();
    std::jthread thread = std::jthread{ RunCmMasmCompiler, buildCommand };
    CmMasmBuildServer::Instance().WaitResultOrExit();
    cmajor::info::bs::BuildResult result = CmMasmBuildServer::Instance().GetResult();
    CmMasmBuildServer::Instance().Stop(); 
    return result;
}

void SendStopBuildRequest()
{
    try
    {
        util::TcpSocket socket("127.0.0.1", std::to_string(defaultIPort));
        std::unique_ptr<soul::xml::Document> stopRequest = MakeStopRequest();
        soul::xml::SendDocument(socket, *stopRequest);
        std::unique_ptr<soul::xml::Document> reply = soul::xml::ReceiveDocument(socket);
        socket.Close();
    }
    catch (...)
    {
        int x = 0;
    }
}

void StopCmMasmBuild()
{
    SendStopBuildRequest();
    CmMasmBuildServer::Instance().Stop();
}

} // cmajor::service

