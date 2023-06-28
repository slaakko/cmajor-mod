// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module bpm.client;

import bpm.server;
import cmajor.binary.message.protocol;
import cmajor.binary.portmap.message;
import util;

namespace bpm {

class PortMapClient
{
public:
    static PortMapClient& Instance();
    void Start(int portMapServicePort_, const std::vector<int>& portNumbers_, const std::string& processName_, int pid_);
    void Stop();
    void Run();
private:
    PortMapClient();
    void ExtendPortLease();
    int portMapServicePort;
    std::vector<int> portNumbers;
    std::string processName;
    int pid;
    std::thread clientThread;
    std::condition_variable exitVar;
    bool exiting;
    std::mutex mtx;
    bool run;
};

PortMapClient& PortMapClient::Instance()
{
    static PortMapClient instance;
    return instance;
}

PortMapClient::PortMapClient() : portMapServicePort(-1), portNumbers(), processName(), pid(-1), exiting(false), run(false)
{
}

void PortMapClient::ExtendPortLease()
{
    try
    {
        util::TcpSocket socket("localhost", std::to_string(portMapServicePort));
        bpm::ExtendPortLeaseRequest request;
        request.processName = processName;
        request.pid = pid;
        for (int port : portNumbers)
        {
            request.portNumbers.push_back(port);
        }
        cmajor::bmp::WriteMessage(socket, &request);
        std::unique_ptr<cmajor::bmp::BinaryMessage> reply(cmajor::bmp::ReadMessage(socket));
    }
    catch (...)
    {
    }
}

void RunPortMapClient(PortMapClient* client)
{
    try
    {
        client->Run();
    }
    catch (...)
    {
    }
}

void PortMapClient::Start(int portMapServicePort_, const std::vector<int>& portNumbers_, const std::string& processName_, int pid_)
{
    portMapServicePort = portMapServicePort_;
    portNumbers = portNumbers_;
    processName = processName_;
    pid = pid_;
    clientThread = std::thread{ RunPortMapClient, this };
}

void PortMapClient::Stop()
{
    if (run)
    {
        exiting = true;
        exitVar.notify_one();
        clientThread.join();
    }
}

void PortMapClient::Run()
{
    run = true;
    while (!exiting)
    {
        std::unique_lock<std::mutex> lock(mtx);
        if (exitVar.wait_for(lock, std::chrono::seconds{ leaseRenewalTimeSecs }, [this] { return exiting; }))
        {
            return;
        }
        ExtendPortLease();
    }
}

void StartPortMapClient(int portMapServicePort, const std::vector<int>& portNumbers, const std::string& processName, int pid)
{
    PortMapClient::Instance().Start(portMapServicePort, portNumbers, processName, pid);
}

void StopPortMapClient()
{
    PortMapClient::Instance().Stop();
}

} // namespace bpm
