// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module bpm.server;

import util;
import std.filesystem;
import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import cmajor.binary.portmap.message;
import cmajor.binary.message.protocol;

namespace bpm {

const char* binaryPortMapServerVersion = "5.0.0";

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

std::string BinaryPortMapConfigFilePath()
{
    return util::GetFullPath(util::Path::Combine(CmajorConfigDir(), "bpm.config.xml"));
}

std::string CmajorLogDir()
{
    std::string logDir = util::Path::Combine(CmajorRootDir(), "log");
    std::filesystem::create_directories(logDir);
    return logDir;
}

std::string BinaryPortMapLogFilePath()
{
    return util::GetFullPath(util::Path::Combine(CmajorLogDir(), "bpms.log"));
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
    util::DateTime leaseStartTime;
    util::DateTime leaseRenewalTime;
    std::chrono::time_point<std::chrono::steady_clock> leaseTimePoint;
};

class BinaryPortMapServer
{
public:
    static BinaryPortMapServer& Instance();
    void Run();
private:
    BinaryPortMapServer();
    int portMapServicePort;
    int startPortRange;
    int endPortRange;
    int nextPortNumber;
    bool exiting;
    std::map<int, PortEntry> portEntryMap;
    util::TcpSocket listenSocket;
    int GetNextPortNumber(const std::string& processName, int pid);
    PortEntry* GetPortEntry(int portNumber) const;
    bpm::GetFreePortNumberReply ProcessGetFreePortNumberRequest(const bpm::GetFreePortNumberRequest& request);
    bpm::ExtendPortLeaseReply ProcessExtendPortLeaseRequest(const bpm::ExtendPortLeaseRequest& request);
    bpm::ViewPortLeaseReply ProcessViewPortLeaseRequest(const bpm::ViewPortLeaseRequest& request);
    bpm::StopPortMapServerReply ProcessStopPortMapServerRequest(const bpm::StopPortMapServerRequest& request);
};

BinaryPortMapServer& BinaryPortMapServer::Instance()
{
    static BinaryPortMapServer instance;
    return instance;
}

BinaryPortMapServer::BinaryPortMapServer() : portMapServicePort(54421), startPortRange(54400), endPortRange(54420), nextPortNumber(-1), exiting(false)
{
    std::string configFilePath = BinaryPortMapConfigFilePath();
    std::unique_ptr<soul::xml::Document> configDoc;
    if (std::filesystem::exists(configFilePath))
    {
        configDoc = soul::xml::ParseXmlFile(configFilePath);
    }
    else
    {
        configDoc.reset(new soul::xml::Document());
        soul::xml::Element* rootElement = soul::xml::MakeElement("bpm");
        configDoc->AppendChild(rootElement);
        rootElement->SetAttribute("portMapServicePort", std::to_string(portMapServicePort));
        rootElement->SetAttribute("startPortRange", std::to_string(startPortRange));
        rootElement->SetAttribute("endPortRange", std::to_string(endPortRange));
        std::ofstream configFile(configFilePath);
        util::CodeFormatter formatter(configFile);
        formatter.SetIndentSize(1);
        configDoc->Write(formatter);
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/bpm", configDoc.get());
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
                throw std::runtime_error("'portMapServicePort' attribute not found from 'bpm' element of file '" + configFilePath + "'");
            }
            std::string startPortRangeAttribute = element->GetAttribute("startPortRange");
            if (!startPortRangeAttribute.empty())
            {
                startPortRange = std::stoi(startPortRangeAttribute);
            }
            else
            {
                throw std::runtime_error("'startPortRange' attribute not found from 'bpm' element of file '" + configFilePath + "'");
            }
            std::string endPortRangeAttribute = element->GetAttribute("endPortRange");
            if (!endPortRangeAttribute.empty())
            {
                endPortRange = std::stoi(endPortRangeAttribute);
            }
            else
            {
                throw std::runtime_error("'endPortRange' attribute not found from 'bpm' element of file '" + configFilePath + "'");
            }
            nextPortNumber = startPortRange;
        }
        else
        {
            throw std::runtime_error("single 'bpm' element expected to be found from '" + configFilePath + "'");
        }
    }
    else
    {
        throw std::runtime_error("single 'bpm' element expected to be found from '" + configFilePath + "'");
    }
}

void BinaryPortMapServer::Run()
{
    listenSocket.Bind(portMapServicePort);
    listenSocket.Listen(10);
    std::cout << "binary-port-map-server-ready" << std::endl;
    while (!exiting)
    {
        util::TcpSocket socket = listenSocket.Accept();
        if (exiting) return;
        try
        {
            std::unique_ptr<cmajor::bmp::BinaryMessage> request(cmajor::bmp::ReadMessage(socket));
            if (request)
            {
                switch (request->Id())
                {
                    case bpm::bmpHelloBinaryPortMapServerRequestId:
                    {
                        HelloBinaryPortMapServerReply reply;
                        reply.version = binaryPortMapServerVersion;
                        cmajor::bmp::WriteMessage(socket, &reply);
                        break;
                    }
                    case bpm::bmpGetFreePortNumberRequestId:
                    {
                        bpm::GetFreePortNumberRequest* req = static_cast<bpm::GetFreePortNumberRequest*>(request.get());
                        GetFreePortNumberReply reply = ProcessGetFreePortNumberRequest(*req);
                        cmajor::bmp::WriteMessage(socket, &reply);
                        break;
                    }
                    case bpm::bmpExtendPortLeaseRequestId:
                    {
                        bpm::ExtendPortLeaseRequest* req = static_cast<bpm::ExtendPortLeaseRequest*>(request.get());
                        ExtendPortLeaseReply reply = ProcessExtendPortLeaseRequest(*req);
                        cmajor::bmp::WriteMessage(socket, &reply);
                        break;
                    }
                    case bpm::bmpStopPortMapServerRequestId:
                    {
                        bpm::StopPortMapServerRequest* req = static_cast<bpm::StopPortMapServerRequest*>(request.get());
                        StopPortMapServerReply reply = ProcessStopPortMapServerRequest(*req);
                        cmajor::bmp::WriteMessage(socket, &reply);
                        break;
                    }
                    case bpm::bmpViewPortLeaseRequestId:
                    {
                        bpm::ViewPortLeaseRequest* req = static_cast<bpm::ViewPortLeaseRequest*>(request.get());
                        ViewPortLeaseReply reply = ProcessViewPortLeaseRequest(*req);
                        cmajor::bmp::WriteMessage(socket, &reply);
                        break;
                    }
                }
            }
            else
            {
                throw std::runtime_error("bpms: unknown message received");
            }
        }
        catch (const std::exception& ex)
        {
            util::LogFileWriter writer(BinaryPortMapLogFilePath());
            writer.WriteCurrentDateTime();
            writer << "bpms got exception '" << ex.what() << "'" << std::endl;
        }
    }
}

int BinaryPortMapServer::GetNextPortNumber(const std::string& processName, int pid)
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
            entry->leaseRenewalTime = util::DateTime();
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
                    entry->leaseRenewalTime = util::DateTime();
                    entry->leaseTimePoint = now;
                    return entry->portNumber;
                }
            }
            ++nextPortNumber;
        }
        return -1;
    }
}

PortEntry* BinaryPortMapServer::GetPortEntry(int portNumber) const
{
    auto it = portEntryMap.find(portNumber);
    if (it != portEntryMap.cend())
    {
        const PortEntry& portEntry = it->second;
        return const_cast<PortEntry*>(&portEntry);
    }
    return nullptr;
}

bpm::GetFreePortNumberReply BinaryPortMapServer::ProcessGetFreePortNumberRequest(const bpm::GetFreePortNumberRequest& request)
{
    int portNumber = GetNextPortNumber(request.processName, request.pid);
    bpm::GetFreePortNumberReply reply;
    reply.portNumber = portNumber;
    return reply;
}

bpm::ExtendPortLeaseReply BinaryPortMapServer::ProcessExtendPortLeaseRequest(const bpm::ExtendPortLeaseRequest& request)
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
    bpm::ExtendPortLeaseReply reply;
    return reply;
}

bpm::ViewPortLeaseReply BinaryPortMapServer::ProcessViewPortLeaseRequest(const bpm::ViewPortLeaseRequest& request)
{
    bpm::ViewPortLeaseReply reply;
    for (const std::pair<int, PortEntry>& p : portEntryMap)
    {
        const PortEntry& portEntry = p.second;
        bpm::PortLease portLease;
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

bpm::StopPortMapServerReply BinaryPortMapServer::ProcessStopPortMapServerRequest(const bpm::StopPortMapServerRequest& request)
{
    bpm::StopPortMapServerReply reply;
    exiting = true;
    return reply;
}

void RunPortMapServer()
{
    BinaryPortMapServer::Instance().Run();
}

} // namespace bpm
