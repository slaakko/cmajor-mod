// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binary.message.test.server;

import cmajor.binary.message.protocol;
import bm_test_message;
import util;

class Server
{
public:
    Server();
    static Server& Instance();
    void Start();
    void Stop();
    void Run();
private:
    int port;
    bool exit;
    std::thread thread;
    util::TcpSocket socket;
};

Server::Server() : port(56789), exit(false)
{
}

Server& Server::Instance()
{
    static Server instance;
    return instance;
}

void RunServer(Server* server)
{
    server->Run();
}

void Server::Start()
{
    thread = std::thread{ RunServer, this };
}

void Server::Stop()
{
    try
    {
        if (!exit)
        {
            exit = true;
            std::unique_ptr<cmajor::bmp::BinaryMessage> message(new BmTest::StopServerRequest());
            util::TcpSocket client("127.0.0.1", std::to_string(port));
            cmajor::bmp::WriteMessage(client, message.get());
        }
        thread.join();
    }
    catch (const std::exception& ex)
    {
        // todo
    }
}

void Server::Run()
{
    try
    {
        socket.Bind(port);
        socket.Listen(10);
        while (!exit)
        {
            util::TcpSocket client = socket.Accept();
            std::unique_ptr<cmajor::bmp::BinaryMessage> message(cmajor::bmp::ReadMessage(client));
            while (!exit)
            {
                if (message)
                {
                    if (message->Id() == BmTest::bmpStopServerRequestId)
                    {
                        std::unique_ptr<cmajor::bmp::BinaryMessage> reply(new BmTest::StopServerReply());
                        cmajor::bmp::WriteMessage(client, reply.get());
                        exit = true;
                        break;
                    }
                    cmajor::bmp::WriteMessage(client, message.get());
                }
                else
                {
                    break;
                }
                message.reset(cmajor::bmp::ReadMessage(client));
            }
        }
    }
    catch (const std::exception& ex)
    {
        // todo
    }
}

void StartServer()
{
    Server::Instance().Start();
}

void StopServer()
{
    Server::Instance().Stop();
}
