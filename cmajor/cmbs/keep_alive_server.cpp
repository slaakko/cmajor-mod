// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmbs.keep.alive.server;

import cmbs.logging;
import cmajor.build.server.message;
import cmajor.bmp;
import util;

namespace cmbs {

class KeepAliveServer
{
public:
    static KeepAliveServer& Instance();
    void Start(int keepAliveServerPort_, std::condition_variable* exitVar_, bool* exiting_, bool logging_);
    bool Started() const { return started; }
    void Run();
    bool Timeout() const;
    void Stop();
private:
    KeepAliveServer();
    int keepAliveServerPort;
    std::condition_variable* exitVar;
    bool* exiting;
    std::thread thread;
    bool started;
    bool exit;
    bool logging;
    util::TcpSocket listenSocket;
    std::chrono::time_point<std::chrono::steady_clock> keepAliveReceivedTimePoint;
};

KeepAliveServer& KeepAliveServer::Instance()
{
    static KeepAliveServer instance;
    return instance;
}

KeepAliveServer::KeepAliveServer() : keepAliveServerPort(-1), exitVar(nullptr), exiting(nullptr), started(false), exit(false)
{
}

void RunServer(KeepAliveServer* server)
{
    server->Run();
}

void KeepAliveServer::Start(int keepAliveServerPort_, std::condition_variable* exitVar_, bool* exiting_, bool logging_)
{
    keepAliveServerPort = keepAliveServerPort_;
    exitVar = exitVar_;
    exiting = exiting_;
    logging = logging_;
    thread = std::thread{ RunServer, this };
}

void KeepAliveServer::Run()
{
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    keepAliveReceivedTimePoint = now;
    listenSocket.Bind(keepAliveServerPort);
    listenSocket.Listen(10);
    started = true;
    while (!exit)
    {
        util::TcpSocket socket = listenSocket.Accept();
        if (exit) return;
        try
        {
            std::unique_ptr<cmajor::bmp::BinaryMessage> message(cmajor::bmp::ReadMessage(socket));
            if (message)
            {
                if (message->Id() == bs::bmpKeepAliveBuildRequestId)
                {
                    bs::KeepAliveBuildReply reply;
                    cmajor::bmp::WriteMessage(socket, &reply);
                    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
                    keepAliveReceivedTimePoint = now;
                }
                else
                {
                    throw std::runtime_error("cmbs.keepAliverServer: error: unknown message " + std::to_string(message->Id()) + " received");
                }
            }
            else
            {
                throw std::runtime_error("cmbs.keepAliverServer: error: null message received");
            }
        }
        catch (const std::exception& ex)
        {
            if (logging)
            {
                util::LogFileWriter writer(LogFilePath());
                writer.WriteCurrentDateTime();
                writer << "cmbs keep alive server got exception '" << ex.what() << "'" << std::endl;
            }
        }
    }
}

void KeepAliveServer::Stop()
{
    try
    {
        exit = true;
        util::TcpSocket socket("localhost", std::to_string(keepAliveServerPort));
        thread.join();
    }
    catch (...)
    {
    }
}

bool KeepAliveServer::Timeout() const
{
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::seconds>(now - keepAliveReceivedTimePoint).count() > timeoutSecs)
    {
        if (exitVar && exiting)
        {
            *exiting = true;
            exitVar->notify_one();
        }
        return true;
    }
    else
    {
        return false;
    }
}

void StartKeepAliveServer(int keepAliveServerPort, std::condition_variable* exitVar, bool* exiting, bool logging)
{
    KeepAliveServer::Instance().Start(keepAliveServerPort, exitVar, exiting, logging);
    while (!KeepAliveServer::Instance().Started())
    {
        std::this_thread::sleep_for(std::chrono::seconds{ 1 });
    }
}

void StopKeepAliveServer()
{
    KeepAliveServer::Instance().Stop();
}

bool Timeout()
{
    return KeepAliveServer::Instance().Timeout();
}

} // namespace cmbs
