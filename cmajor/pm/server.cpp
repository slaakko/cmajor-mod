// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.portmap.server;

import cmajor.portmap.message;
import soul.xml.serialization;
import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import util;
import std.filesystem;

namespace cmajor::portmap {

const char* portMapServerVersion = "5.0.0";

std::string CmajorRootDir()
{
    char* e = getenv("CMAJOR_ROOT");
    if (e == nullptr || !*e)
    {
        throw std::runtime_error("please set 'CMAJOR_ROOT' environment variable to contain /path/to/cmajor directory.");
    }
    return util::GetFullPath(std::string(e));
}

std::string CmajorConfigDir()
{
    std::string configDir = util::GetFullPath(util::Path::Combine(CmajorRootDir(), "config"));
    std::filesystem::create_directories(configDir);
    return configDir;
}

std::string PortMapConfigFilePath()
{
    return util::GetFullPath(util::Path::Combine(CmajorConfigDir(), "pm.config.xml"));
}

std::string CmajorLogDir()
{
    std::string logDir = util::Path::Combine(CmajorRootDir(), "log");
    std::filesystem::create_directories(logDir);
    return logDir;
}

std::string PortMapLogFilePath()
{
    return util::GetFullPath(util::Path::Combine(CmajorLogDir(), "pms.log"));
}

struct PortEntry
{
    PortEntry() : portNumber(-1), processName(), pid(), leaseStartTime(), leaseRenewalTime(), leaseTimePoint()
    {
    }
    PortEntry(int portNumber_, const std::string& processName_, int pid_) :
        portNumber(portNumber_), processName(processName_), pid(pid_), leaseStartTime(util::GetCurrentDateTime()), leaseRenewalTime(), leaseTimePoint(std::chrono::steady_clock::now())
    {
    }
    int portNumber;
    std::string processName;
    int pid;
    soul::xml::serialization::datetime leaseStartTime;
    soul::xml::serialization::datetime leaseRenewalTime;
    std::chrono::time_point<std::chrono::steady_clock> leaseTimePoint;
};

class PortMapServer
{
public:
    static PortMapServer& Instance();
    void Run();
private:
    static std::unique_ptr<PortMapServer> instance;
    PortMapServer();
    int portMapServicePort;
    int startPortRange;
    int endPortRange;
    int nextPortNumber;
    std::map<int, PortEntry> portEntryMap;
    util::TcpSocket listenSocket;
    bool exiting;
    std::string GetMessage(soul::xml::Element* element);
    int GetNextPortNumber(const std::string& processName, int pid);
    PortEntry* GetPortEntry(int portNumber) const;
    GetFreePortNumberReply ProcessGetFreePortNumberRequest(const GetFreePortNumberRequest& request);
    ExtendPortLeaseReply ProcessExtendPortLeaseRequest(const ExtendPortLeaseRequest& request);
    ViewPortLeaseReply ProcessViewPortLeaseRequest(const ViewPortLeaseRequest& request);
    StopPortMapServerReply ProcessStopPortMapServerRequest(const StopPortMapServerRequest& request);
};

PortMapServer& PortMapServer::Instance()
{
    static PortMapServer instance;
    return instance;
}

PortMapServer::PortMapServer() : portMapServicePort(54321), startPortRange(54300), endPortRange(54320), nextPortNumber(-1), exiting(false)
{
    std::string configFilePath = PortMapConfigFilePath();
    std::unique_ptr<soul::xml::Document> configDoc;
    if (std::filesystem::exists(configFilePath))
    {
        configDoc = soul::xml::ParseXmlFile(configFilePath);
    }
    else
    {
        configDoc.reset(new soul::xml::Document());
        soul::xml::Element* rootElement = soul::xml::MakeElement("pm");
        configDoc->AppendChild(rootElement);
        rootElement->SetAttribute("portMapServicePort", std::to_string(portMapServicePort));
        rootElement->SetAttribute("startPortRange", std::to_string(startPortRange));
        rootElement->SetAttribute("endPortRange", std::to_string(endPortRange));
        std::ofstream configFile(configFilePath);
        util::CodeFormatter formatter(configFile);
        formatter.SetIndentSize(1);
        configDoc->Write(formatter);
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/pm", configDoc.get());
    if (nodeSet->Count() == 1)
    {
        soul::xml::Node* node = nodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string portMapServicePortAttribute = element->GetAttribute("portMapServicePort");
            if (!portMapServicePortAttribute.empty())
            {
                portMapServicePort = std::stoi(portMapServicePortAttribute);
            }
            else
            {
                throw std::runtime_error("'portMapServicePort' attribute not found from 'pm' element of file '" + configFilePath + "'");
            }
            std::string startPortRangeAttribute = element->GetAttribute("startPortRange");
            if (!startPortRangeAttribute.empty())
            {
                startPortRange = std::stoi(startPortRangeAttribute);
            }
            else
            {
                throw std::runtime_error("'startPortRange' attribute not found from 'pm' element of file '" + configFilePath + "'");
            }
            std::string endPortRangeAttribute = element->GetAttribute("endPortRange");
            if (!endPortRangeAttribute.empty())
            {
                endPortRange = std::stoi(endPortRangeAttribute);
            }
            else
            {
                throw std::runtime_error("'endPortRange' attribute not found from 'pm' element of file '" + configFilePath + "'");
            }
            nextPortNumber = startPortRange;
        }
        else
        {
            throw std::runtime_error("single 'pm' element expected to be found from '" + configFilePath + "'");
        }
    }
    else
    {
        throw std::runtime_error("single 'pm' element expected to be found from '" + configFilePath + "'");
    }
}

std::string PortMapServer::GetMessage(soul::xml::Element* element)
{
    return element->Name();
}

PortEntry* PortMapServer::GetPortEntry(int portNumber) const
{
    auto it = portEntryMap.find(portNumber);
    if (it != portEntryMap.cend())
    {
        const PortEntry& portEntry = it->second;
        return const_cast<PortEntry*>(&portEntry);
    }
    return nullptr;
}

int PortMapServer::GetNextPortNumber(const std::string& processName, int pid)
{
    if (nextPortNumber > endPortRange)
    {
        nextPortNumber = startPortRange;
    }
    PortEntry* entry = GetPortEntry(nextPortNumber);
    if (entry == nullptr)
    {
        int portNumber = nextPortNumber++;
        portEntryMap[portNumber] = PortEntry(portNumber, processName, pid);
        return portNumber;
    }
    else
    {
        std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - entry->leaseTimePoint).count() > leaseExpirationTimeSecs)
        {
            ++nextPortNumber;
            entry->processName = processName;
            entry->pid = pid;
            entry->leaseStartTime = util::GetCurrentDateTime();
            entry->leaseRenewalTime = soul::xml::serialization::datetime();
            entry->leaseTimePoint = now;
            return entry->portNumber;
        }
        int start = nextPortNumber++;
        while (nextPortNumber != start)
        {
            if (nextPortNumber > endPortRange)
            {
                nextPortNumber = startPortRange;
            }
            entry = GetPortEntry(nextPortNumber);
            if (entry == nullptr)
            {
                int portNumber = nextPortNumber++;
                portEntryMap[portNumber] = PortEntry(portNumber, processName, pid);
                return portNumber;
            }
            else
            {
                if (std::chrono::duration_cast<std::chrono::seconds>(now - entry->leaseTimePoint).count() > leaseExpirationTimeSecs)
                {
                    ++nextPortNumber;
                    entry->processName = processName;
                    entry->pid = pid;
                    entry->leaseStartTime = util::GetCurrentDateTime();
                    entry->leaseRenewalTime = soul::xml::serialization::datetime();
                    entry->leaseTimePoint = now;
                    return entry->portNumber;
                }
            }
            ++nextPortNumber;
        }
        return -1;
    }
}

GetFreePortNumberReply PortMapServer::ProcessGetFreePortNumberRequest(const GetFreePortNumberRequest& request)
{
    int portNumber = GetNextPortNumber(request.processName, request.pid);
    GetFreePortNumberReply reply;
    reply.portNumber = portNumber;
    return reply;
}

ExtendPortLeaseReply PortMapServer::ProcessExtendPortLeaseRequest(const ExtendPortLeaseRequest& request)
{
    for (int port : request.portNumbers)
    {
        PortEntry* entry = GetPortEntry(port);
        if (entry != nullptr)
        {
            std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
            entry->leaseRenewalTime = util::GetCurrentDateTime();
            entry->leaseTimePoint = now;
            entry->processName = request.processName;
            entry->pid = request.pid;
        }
    }
    ExtendPortLeaseReply reply;
    return reply;
}

ViewPortLeaseReply PortMapServer::ProcessViewPortLeaseRequest(const ViewPortLeaseRequest& request)
{
    ViewPortLeaseReply reply;
    for (const std::pair<int, PortEntry>& p : portEntryMap)
    {
        const PortEntry& portEntry = p.second;
        PortLease portLease;
        portLease.portNumber = portEntry.portNumber;
        portLease.processName = portEntry.processName;
        portLease.pid = portEntry.pid;
        portLease.leaseStartTime = portEntry.leaseStartTime;
        portLease.leaseRenewalTime = portEntry.leaseRenewalTime;
        std::string leaseStateStr = "valid";
        std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - portEntry.leaseTimePoint).count() > leaseExpirationTimeSecs)
        {
            leaseStateStr = "expired";
        }
        portLease.leaseState = leaseStateStr;
        reply.portLeases.push_back(portLease);
    }
    return reply;
}

StopPortMapServerReply PortMapServer::ProcessStopPortMapServerRequest(const StopPortMapServerRequest& request)
{
    StopPortMapServerReply reply;
    exiting = true;
    return reply;
}

void PortMapServer::Run()
{
    listenSocket.Bind(portMapServicePort);
    listenSocket.Listen(10);
    std::cout << "port-map-server-ready" << std::endl;
    while (!exiting)
    {
        util::TcpSocket socket = listenSocket.Accept();
        if (exiting) return;
        try
        {
            std::string requestStr = ReadStr(socket);
            std::unique_ptr<soul::xml::Document> requestDoc = soul::xml::ParseXmlContent(requestStr, "socket");
            std::string message = GetMessage(requestDoc->DocumentElement());
            if (!message.empty())
            {
                if (message == "helloPmsRequest")
                {
                    HelloPmsRequest request;
                    request.FromXml(requestDoc->DocumentElement());
                    HelloPmsReply reply;
                    reply.version = portMapServerVersion;
                    std::unique_ptr<soul::xml::Element> replyValue(reply.ToXml("helloPmsReply"));
                    std::stringstream strStream;
                    util::CodeFormatter formatter(strStream);
                    replyValue->Write(formatter);
                    std::string replyStr = strStream.str();
                    Write(socket, replyStr);
                }
                else if (message == "getFreePortNumberRequest")
                {
                    GetFreePortNumberRequest request;
                    request.FromXml(requestDoc->DocumentElement());
                    GetFreePortNumberReply reply = ProcessGetFreePortNumberRequest(request);
                    std::unique_ptr<soul::xml::Element> replyValue(reply.ToXml("getFreePortNumberReply"));
                    std::stringstream strStream;
                    util::CodeFormatter formatter(strStream);
                    replyValue->Write(formatter);
                    std::string replyStr = strStream.str();
                    Write(socket, replyStr);
                }
                else if (message == "extendPortLeaseRequest")
                {
                    ExtendPortLeaseRequest request;
                    request.FromXml(requestDoc->DocumentElement());
                    ExtendPortLeaseReply reply = ProcessExtendPortLeaseRequest(request);
                    std::unique_ptr<soul::xml::Element> replyValue(reply.ToXml("extendPortLeaseReply"));
                    std::stringstream strStream;
                    util::CodeFormatter formatter(strStream);
                    replyValue->Write(formatter);
                    std::string replyStr = strStream.str();
                    Write(socket, replyStr);
                }
                else if (message == "stopPortMapServerRequest")
                {
                    StopPortMapServerRequest request;
                    request.FromXml(requestDoc->DocumentElement());
                    StopPortMapServerReply reply = ProcessStopPortMapServerRequest(request);
                    std::unique_ptr<soul::xml::Element> replyValue(reply.ToXml("stopPortMapServerReply"));
                    std::stringstream strStream;
                    util::CodeFormatter formatter(strStream);
                    replyValue->Write(formatter);
                    std::string replyStr = strStream.str();
                    Write(socket, replyStr);
                }
                else if (message == "viewPortLeaseRequest")
                {
                    ViewPortLeaseRequest request;
                    request.FromXml(requestDoc->DocumentElement());
                    ViewPortLeaseReply reply = ProcessViewPortLeaseRequest(request);
                    std::unique_ptr<soul::xml::Element> replyValue(reply.ToXml("viewPortLeaseReply"));
                    std::stringstream strStream;
                    util::CodeFormatter formatter(strStream);
                    replyValue->Write(formatter);
                    std::string replyStr = strStream.str();
                    Write(socket, replyStr);
                }
            }
            else
            {
                throw std::runtime_error("unknown message received: '" + message + "'");
            }
        }
        catch (const std::exception& ex)
        {
            util::LogFileWriter writer(PortMapLogFilePath());
            writer.WriteCurrentDateTime();
            writer << "cmpms got exception '" << ex.what() << "'" << std::endl;
        }
    }
}

void RunPortMapServer()
{
    PortMapServer::Instance().Run();
}

} // namespace cmajor::cmpm
