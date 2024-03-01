// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.logger.service;

import cmajor.service.message;
import util;

namespace cmajor::service {

class LoggerService
{
public:
    static LoggerService& Instance();
    void Start();
    void Run();
    void Stop();
private:
    LoggerService();
    std::thread thread;
    bool exit;
};

LoggerService& LoggerService::Instance()
{
    static LoggerService instance;
    return instance;
}

LoggerService::LoggerService() : exit(false)
{
    util::SetLogMode(util::LogMode::queue);
    util::StartLog();
}

void RunLogger(LoggerService* loggerService)
{
    loggerService->Run();
}

void LoggerService::Start()
{
    thread = std::thread(RunLogger, this);
}

void LoggerService::Stop()
{
    util::EndLog();
    thread.join();
}

void LoggerService::Run()
{
    while (!exit)
    {
        bool timeout = false;
        std::string message = util::FetchLogMessage(exit, 0, timeout);
        if (!exit)
        {
            PutOutputServiceMessage(message);
        }
    }
}

void StartLoggerService()
{
    LoggerService::Instance().Start();
}

void StopLoggerService()
{
    LoggerService::Instance().Stop();
}

} // namespace cmajor::service
