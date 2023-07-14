// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmbs.build.server;

import soul.lexer;
import cmbs.logging;
import cmajor.build.server.message;
import cmajor.build;
import cmajor.bmp;
import cmajor.symbols;
import cmajor.backend;
import util;

namespace cmbs {

bs::CompileError ToError(const soul::lexer::ParsingException& ex)
{
    bs::CompileError error;
    error.message = ex.Message();
    error.project = ex.Project();
    error.file = ex.FileName();
    if (ex.GetSourcePos().IsValid())
    {
        error.line = ex.GetSourcePos().line;
        error.scol = ex.GetSourcePos().col;
    }
    return error;
}

class BuildServer
{
public:
    static BuildServer& Instance();
    void Start(int port_, std::condition_variable* exitVar_, bool* exiting_, bool logging_, bool progress_);
    void Stop();
    void Run();
    void RunLogger();
    bool StopRequested() const { return stopRequested; }
    bool Logging() const { return logging; }
private:
    BuildServer();
    void HandleRequest(cmajor::bmp::BinaryMessage* requestMessage, util::LogFileWriter& logWriter);
    void HandleBuildRequest(bs::BuildRequest* request, util::LogFileWriter& logWriter);
    void HandleStopBuildRequest(util::LogFileWriter& logWriter);
    void StartLogging();
    void EndLogging();
    void StartLoggerThread();
    void StopLoggerThread();
    int port;
    std::condition_variable* exitVar;
    bool* exiting;
    bool stopRequested;
    std::thread serverThread;
    std::thread loggerThread;
    bool running;
    bool exit;
    bool logging;
    std::exception_ptr runException;
    util::TcpSocket listenSocket;
    util::TcpSocket socket;
    bool progress;
    int progressIntervalMs;
};

BuildServer& BuildServer::Instance()
{
    static BuildServer instance;
    return instance;
}

BuildServer::BuildServer() : port(-1), exitVar(nullptr), exiting(nullptr), stopRequested(false), running(false), exit(false), logging(false), 
    progress(false), progressIntervalMs(0)
{
}

void RunServer(BuildServer* server)
{
    server->Run();
}

void BuildServer::Start(int port_, std::condition_variable* exitVar_, bool* exiting_, bool logging_, bool progress_)
{
    port = port_; 
    exitVar = exitVar_;
    exiting = exiting_;
    logging = logging_;
    progress = progress_;
    if (progress)
    {
        progressIntervalMs = 250;
    }
    serverThread = std::thread{ RunServer, this };
    while (!running)
    {
        std::this_thread::sleep_for(std::chrono::seconds{ 1 });
    }
    if (logging)
    {
        util::LogFileWriter logWriter(LogFilePath());
        logWriter << "Build server version " << cmajor::symbols::GetCompilerVersion() << " started" << std::endl;
    }
}

void BuildServer::Stop()
{
    exit = true;
    try
    {
        if (running)
        {
            cmajor::build::StopBuild();
            if (runException)
            {
                serverThread.join();
                std::rethrow_exception(runException);
            }
            else
            {
                util::TcpSocket socket("localhost", std::to_string(port));
            }
        }
        if (serverThread.joinable())
        {
            serverThread.join();
        }
    }
    catch (const std::runtime_error& ex)
    {
        std::cerr << "exception from build server Stop: " << ex.what() << std::endl;
        throw;
    }
}

void BuildServer::Run()
{
    try
    {
        running = true;
        util::LogFileWriter logWriter(LogFilePath());
        listenSocket.Bind(port);
        listenSocket.Listen(10);
        while (!exit)
        {
            std::cout << "build-server-ready" << std::endl;
            socket = listenSocket.Accept();
            if (exit)
            {
                if (logging)
                {
                    logWriter << "exiting..." << std::endl;
                }
                return;
            }
            std::unique_ptr<cmajor::bmp::BinaryMessage> requestMessage(cmajor::bmp::ReadMessage(socket));
            if (requestMessage)
            {
                HandleRequest(requestMessage.get(), logWriter);
            }
            else
            {
                throw std::runtime_error("null message received");
            }
        }
    }
    catch (const std::exception& ex)
    {
        running = false;
        std::cout << "build-server-error" << std::endl;
        std::cout << ex.what() << std::endl;
        if (logging)
        {
            util::LogFileWriter logWriter(LogFilePath());
            logWriter.WriteLine("================================================================================");
            logWriter.WriteCurrentDateTime();
            logWriter << "error: build  server got exception: " << ex.what() << std::endl;
        }
        runException = std::current_exception();
    }
}

void BuildServer::HandleRequest(cmajor::bmp::BinaryMessage* requestMessage, util::LogFileWriter& logWriter)
{
    switch (requestMessage->Id())
    {
        case bs::bmpBuildRequestId:
        {
            HandleBuildRequest(static_cast<bs::BuildRequest*>(requestMessage), logWriter);
            break;
        }
        case bs::bmpStopBuildRequestId:
        {
            HandleStopBuildRequest(logWriter);
            break;
        }
    }
}

void BuildServer::HandleBuildRequest(bs::BuildRequest* request, util::LogFileWriter& logWriter)
{
    bs::BuildReply buildReply;
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    try
    {
        if (logging)
        {
            logWriter.WriteLine("================================================================================");
            logWriter.WriteCurrentDateTime();
            logWriter << "build request received" << std::endl;
        }
        cmajor::symbols::BackEnd backend = cmajor::symbols::BackEnd::cpp;
        if (request->backend.empty())
        {
            throw std::runtime_error("backend required");
        }
        else if (request->backend == "cpp")
        {
            backend = cmajor::symbols::BackEnd::cpp;
        }
        else if (request->backend == "llvm")
        {
            backend = cmajor::symbols::BackEnd::llvm;
        }
        else
        {
            throw std::runtime_error("unsupported backend '" + request->backend + "'");
        }
        if (request->filePath.empty())
        {
            throw std::runtime_error("file path required");
        }
        else if (!std::filesystem::exists(request->filePath))
        {
            throw std::runtime_error("file '" + request->filePath + "' does not exist");
        }
        cmajor::build::ResetStopBuild();
        cmajor::symbols::ResetGlobalFlags();
        cmajor::symbols::SetBackEnd(backend);
        if (request->config == "release")
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::release);
        }
        else if (!request->config.empty() && request->config != "debug")
        {
            throw std::runtime_error("unknown configuration '" + request->config);
        }
        if (request->verbose)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
        }
        if (request->quiet)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::quiet);
        }
        if (request->clean)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::clean);
        }
        if (request->rebuild)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild);
        }
        if (request->emitIR)
        { 
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::emitLlvm);
        }
        if (request->emitOptIR)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::emitOptLlvm);
        }
        if (request->linkWithDebugRuntime)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime);
        }
        if (request->singleThreadedCompile)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
        }
        if (request->buildAllDependencies)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::buildAll);
        }
        if (request->disableModuleCache)
        {
            cmajor::symbols::SetUseModuleCache(false);
        }
        else
        {
            cmajor::symbols::SetUseModuleCache(true);
        }
        for (const auto& define : request->defines)
        {
            cmajor::symbols::DefineCommandLineConditionalSymbol(util::ToUtf32(define));
        }
        if (!request->optimizationLevel.empty() && request->optimizationLevel != "default")
        {
            int optimizationLevel = std::stoi(request->optimizationLevel);
            if (optimizationLevel < 0 || optimizationLevel > 3)
            {
                throw std::runtime_error("optimization level out of range");
            }
            cmajor::symbols::SetOptimizationLevel(optimizationLevel);
        }
        buildReply.requestValid = true;
        StartLogging();
        switch (backend)
        {
            case cmajor::symbols::BackEnd::llvm:
            {
                cmajor::backend::SetCurrentBackEnd(cmajor::backend::BackEndKind::llvmBackEnd);
                util::LogMessage(-1, "Cmajor with LLVM backend compiler version " + cmajor::symbols::GetCompilerVersion() + " for Windows x64");
                break;
            }
            case cmajor::symbols::BackEnd::cpp:
            {
                cmajor::backend::SetCurrentBackEnd(cmajor::backend::BackEndKind::cppBackEnd);
                util::LogMessage(-1, "Cmajor with C++ backend compiler version " + cmajor::symbols::GetCompilerVersion() + " for Windows x64");
                break;
            }
        }
        std::unique_ptr<cmajor::symbols::Module> rootModule;
        std::vector<std::unique_ptr<cmajor::symbols::Module>> rootModules;
        std::set<std::string> builtProjects;
        try
        {
            if (request->filePath.ends_with(".cms"))
            {
                cmajor::build::BuildSolution(util::GetFullPath(request->filePath), rootModules);
            }
            else if (request->filePath.ends_with(".cmp"))
            {
                cmajor::build::BuildProject(util::GetFullPath(request->filePath), rootModule, builtProjects);
            }
            else
            {
                throw std::runtime_error("file path has unknown extension (not .cms or .cmp)");
            }
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            auto dur = end - start;
            long long totalSecs = std::chrono::duration_cast<std::chrono::seconds>(dur).count() + 1;
            int hours = static_cast<int>(totalSecs / 3600);
            int mins = static_cast<int>((totalSecs / 60) % 60);
            int secs = static_cast<int>(totalSecs % 60);
            std::string time =
                (hours > 0 ? std::to_string(hours) + " hour" + ((hours != 1) ? "s " : " ") : "") +
                (mins > 0 ? std::to_string(mins) + " minute" + ((mins != 1) ? "s " : " ") : "") +
                std::to_string(secs) + " second" + ((secs != 1) ? "s" : "");
            buildReply.time = time;
            buildReply.success = true;
            if (logging)
            {
                logWriter.WriteLine("================================================================================");
                logWriter.WriteCurrentDateTime();
                logWriter << "request succeeded: time = " << time << std::endl;
            }
        }
        catch (const soul::lexer::ParsingException& ex)
        {
            buildReply.success = false;
            buildReply.errors.push_back(ToError(ex));
            util::LogMessage(-1, ex.what());
            if (logging)
            {
                logWriter.WriteLine("================================================================================");
                logWriter.WriteCurrentDateTime();
                logWriter << "request failed: " << ex.what() << std::endl;
            }
        }
        catch (const cmajor::symbols::Exception& ex)
        {
            buildReply.success = false;
            buildReply.errors = ex.ToErrors();
            util::LogMessage(-1, ex.What());
            if (logging)
            {
                logWriter.WriteLine("================================================================================");
                logWriter.WriteCurrentDateTime();
                logWriter << "request failed: " << ex.What() << std::endl;
            }
        }
        catch (const std::exception& ex)
        {
            buildReply.success = false;
            bs::CompileError error;
            error.message = ex.what();
            buildReply.errors.push_back(error);
            util::LogMessage(-1, ex.what());
            if (logging)
            {
                logWriter.WriteLine("================================================================================");
                logWriter.WriteCurrentDateTime();
                logWriter << "request failed: " << ex.what() << std::endl;
            }
        }
    }
    catch (const std::exception& ex)
    {
        buildReply.requestValid = false;
        buildReply.requestErrorMessage = ex.what();
        if (logging)
        {
            logWriter << ex.what() << std::endl;
        }
    }
    EndLogging();
    cmajor::bmp::WriteMessage(socket, &buildReply);
}

void BuildServer::HandleStopBuildRequest(util::LogFileWriter& logWriter)
{
    if (logging)
    {
        logWriter.WriteLine("================================================================================");
        logWriter.WriteCurrentDateTime();
        logWriter << "stop build request received" << std::endl;
    }
    bs::StopBuildReply reply;
    cmajor::bmp::WriteMessage(socket, &reply);
    stopRequested = true;
    if (exitVar && exiting)
    {
        *exiting = true;
        exitVar->notify_one();
    }
}

void RunLoggerThread(BuildServer* server)
{
    try
    {
        server->RunLogger();
    }
    catch (const std::exception& ex)
    {
        if (server->Logging())
        {
            util::LogFileWriter logWriter(LogFilePath());
            logWriter.WriteLine("================================================================================");
            logWriter.WriteCurrentDateTime();
            logWriter << "logger thread received exception: " << ex.what() << std::endl;
        }
    }
}

void BuildServer::StartLogging()
{
    util::SetLogMode(util::LogMode::queue);
    util::StartLog();
    StartLoggerThread();
}

void BuildServer::EndLogging()
{
    util::EndLog();
    StopLoggerThread();
}

void BuildServer::StartLoggerThread()
{
    loggerThread = std::thread{ RunLoggerThread, this };
}

void BuildServer::RunLogger()
{
    while (true)
    {
        bool endOfLog = false;
        bool timeout = false;
        std::string message = util::FetchLogMessage(endOfLog, progressIntervalMs, timeout);
        if (endOfLog) return;
        if (timeout)
        {
            bs::BuildProgressMessage buildProgressMessage;
            cmajor::bmp::WriteMessage(socket, &buildProgressMessage);
            continue;
        }
        bs::LogBuildMessageRequest logBuildMessageRequest;
        logBuildMessageRequest.message = message;
        cmajor::bmp::WriteMessage(socket, &logBuildMessageRequest);
        std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(socket));
        if (replyMessage)
        {
            if (replyMessage->Id() == bs::bmpLogBuildMessageReplyId)
            {
                bs::LogBuildMessageReply* logBuildMessageReply = static_cast<bs::LogBuildMessageReply*>(replyMessage.get());
                if (!logBuildMessageReply->ok)
                {
                    throw std::runtime_error("log message refused by client");
                }
            }
            else
            {
                throw std::runtime_error("invalid log reply id " + std::to_string(replyMessage->Id()) + " received");
            }
        }
        else
        {
            throw std::runtime_error("null log reply received");
        }
    }
}

void BuildServer::StopLoggerThread()
{
    loggerThread.join();
}

void StartBuildServer(int port, std::condition_variable* exitVar, bool* exiting, bool logging, bool progress)
{
    BuildServer::Instance().Start(port, exitVar, exiting, logging, progress);
}

void StopBuildServer()
{
    BuildServer::Instance().Stop();
}

bool BuildServerStopRequested()
{
    return BuildServer::Instance().StopRequested();
}

} // namespace cmbs
