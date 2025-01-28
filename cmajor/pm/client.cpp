// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.portmap.client;

import cmajor.portmap.server;
import cmajor.portmap.message;
import soul.xml.dom;
import soul.xml.dom.parser;
import util;

namespace cmajor::portmap {

class PortMapClient
{
public:
    static PortMapClient& Instance();
    void Start(int portMapServicePort, const std::vector<int>& portNumbers, const std::string& processName, int pid);
    void Stop();
    void Run();
private:
    static std::unique_ptr<PortMapClient> instance;
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

void PortMapClient::ExtendPortLease()
{
    try
    {
        util::TcpSocket socket("localhost", std::to_string(portMapServicePort));
        ExtendPortLeaseRequest request;
        request.processName = processName;
        request.pid = pid;
        for (int port : portNumbers)
        {
            request.portNumbers.push_back(port);
        }
        std::unique_ptr<soul::xml::Element> requestValue(request.ToXml("extendPortLeaseRequest"));
        std::stringstream strStream;
        util::CodeFormatter formatter(strStream);
        requestValue->Write(formatter);
        std::string requestStr = strStream.str();
        Write(socket, requestStr);
        std::string replyStr = ReadStr(socket);
        std::unique_ptr<soul::xml::Document> replyDoc = soul::xml::ParseXmlContent(replyStr, "socket");
        soul::xml::Element* replyElement = replyDoc->DocumentElement();
        if (replyElement->Name() == "extendPortLeaseReply")
        {
            ExtendPortLeaseReply reply;
            reply.FromXml(replyElement);
        }
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

void StartPortMapClient(int portMapServicePort, const std::vector<int>& portNumbers, const std::string& processName, int pid)
{
    PortMapClient::Instance().Start(portMapServicePort, portNumbers, processName, pid);
}

void StopPortMapClient()
{
    PortMapClient::Instance().Stop();
}

} // namespace cmajor::portmap
