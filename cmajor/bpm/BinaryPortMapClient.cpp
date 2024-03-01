// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module bpm.client;

import bpm.server;
import cmajor.binary.message.protocol;
import cmajor.binary.portmap.message;
import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import std.filesystem;
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

int GetPortMapServicePortNumberFromConfig()
{
    std::string portMapConfigFilePath;
    try
    {
        portMapConfigFilePath = BinaryPortMapConfigFilePath();
        if (!std::filesystem::exists(portMapConfigFilePath))
        {
            soul::xml::Document portMapConfigDoc;
            soul::xml::Element* rootElement = soul::xml::MakeElement("bpm");
            portMapConfigDoc.AppendChild(rootElement);
            rootElement->SetAttribute("startPortRange", "54400");
            rootElement->SetAttribute("endPortRange", "54420");
            rootElement->SetAttribute("portMapServicePort", "54421");
            std::ofstream file(portMapConfigFilePath);
            util::CodeFormatter formatter(file);
            formatter.SetIndentSize(1);
            portMapConfigDoc.Write(formatter);
        }
        std::unique_ptr<soul::xml::Document> portMapConfigDoc = soul::xml::ParseXmlFile(portMapConfigFilePath);
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/bpm", portMapConfigDoc.get());
        if (nodeSet->Count() == 1)
        {
            soul::xml::Node* node = nodeSet->GetNode(0);
            if (node->IsElementNode())
            {
                soul::xml::Element* bpmElement = static_cast<soul::xml::Element*>(node);
                std::string portMapServicePortAttribute = bpmElement->GetAttribute("portMapServicePort");
                if (!portMapServicePortAttribute.empty())
                {
                    return std::stoi(portMapServicePortAttribute);
                }
            }
            else
            {
                throw std::runtime_error("element node expected");
            }
        }
        else
        {
            throw std::runtime_error("single element node expected");
        }
    }
    catch (const std::exception& ex)
    {
        throw ("error: could not get port map service port number from '" + portMapConfigFilePath + "': " + std::string(ex.what()));
    }
    return -1;
}

Logger::~Logger()
{
}

void Logger::LogMessage(const std::string& message)
{
    util::LogMessage(-1, message);
}

bool StartPortMapServer(Logger* logger)
{
    try
    {
        util::Process* portmapServerProcess = new util::Process("cmbpms", 
            util::Process::Redirections::processStdIn | util::Process::Redirections::processStdOut | util::Process::Redirections::processStdErr);
        std::string portmapServerStatus = util::Trim(portmapServerProcess->ReadLine(util::Process::StdHandle::stdOut));
        if (portmapServerStatus == "binary-port-map-server-ready")
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            return true;
        }
        else
        {
            std::string errorMessage = util::Trim(portmapServerProcess->ReadLine(util::Process::StdHandle::stdOut));
            if (logger)
            {
                logger->LogMessage("error: port map server status is: " + portmapServerStatus + ": " + errorMessage);
            }
        }
    }
    catch (const std::exception& ex)
    {
        if (logger)
        {
            logger->LogMessage("error: could not start port map server : " + std::string(ex.what()));
        }
    }
    return false;
}

bool StopPortMapServer(Logger* logger)
{
    try
    {
        int portMapServerPort = GetPortMapServicePortNumberFromConfig();
        if (portMapServerPort == -1)
        {
            throw std::runtime_error("could not get port map server port number from configuration file '" + BinaryPortMapConfigFilePath() + "'");
        }
        util::TcpSocket portMapServerConnection("localhost", std::to_string(portMapServerPort));
        if (logger)
        {
            logger->LogMessage("connection to port map server port " + std::to_string(portMapServerPort) + " established");
            logger->LogMessage("sending stop request...");
        }
        bpm::StopPortMapServerRequest request;
        cmajor::bmp::WriteMessage(portMapServerConnection, &request);
        std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(portMapServerConnection));
        if (replyMessage)
        {
            if (replyMessage->Id() == bpm::bmpStopPortMapServerReplyId)
            {
                if (logger)
                {
                    logger->LogMessage("stop reply received");
                }
                return true;
            }
            else
            {
                throw std::runtime_error("'bpm::StopPortMapServerReply' expected, message id=" + std::to_string(replyMessage->Id()));
            }
        }
        else
        {
            throw std::runtime_error("'bpm::StopPortMapServerReply' expected");
        }
    }
    catch (const std::exception& ex)
    {
        if (logger)
        {
            logger->LogMessage("error: could not stop port map server: " + std::string(ex.what()));
        }
    }
    return false;
}

int GetPortMapServicePortNumber(Logger* logger, bool& portMapServerStarted)
{
    portMapServerStarted = false;
    int portmapServicePortNumber = GetPortMapServicePortNumberFromConfig();
    if (portmapServicePortNumber == -1)
    {
        portMapServerStarted = StartPortMapServer(logger);
        portmapServicePortNumber = GetPortMapServicePortNumberFromConfig();
    }
    return portmapServicePortNumber;
}

int GetFreePortNumber(Logger* logger, const std::string& processName)
{
    try
    {
        bool portMapServerStarted = false;
        int portmapServicePortNumber = GetPortMapServicePortNumber(logger, portMapServerStarted);
        if (portmapServicePortNumber != -1)
        {
            for (int connectionAttempt = 1; connectionAttempt <= 2; ++connectionAttempt)
            {
                try
                {
                    int pid = util::GetPid();
                    util::TcpSocket portMapServerConnection("localhost", std::to_string(portmapServicePortNumber));
                    bpm::GetFreePortNumberRequest request;
                    request.processName = processName;
                    request.pid = pid;
                    cmajor::bmp::WriteMessage(portMapServerConnection, &request);
                    std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(portMapServerConnection));
                    if (replyMessage)
                    {
                        if (replyMessage->Id() == bpm::bmpGetFreePortNumberReplyId)
                        {
                            bpm::GetFreePortNumberReply* reply = static_cast<bpm::GetFreePortNumberReply*>(replyMessage.get());
                            return reply->portNumber;
                        }
                        else
                        {
                            throw std::runtime_error("'bpm::GetFreePortNumberReply' expected, reply messageId=" + std::to_string(replyMessage->Id()));
                        }
                    }
                    else
                    {
                        throw std::runtime_error("'bpm::GetFreePortNumberReply' expected");
                    }
                }
                catch (const std::exception& ex)
                {
                    if (connectionAttempt > 1)
                    {
                        throw ex;
                    }
                }
                if (connectionAttempt == 1 && !portMapServerStarted)
                {
                    portMapServerStarted = StartPortMapServer(logger);
                }
                else
                {
                    break;
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        if (logger)
        {
            logger->LogMessage("error: could not get free port number from port map server: " + std::string(ex.what()));
        }
    }
    return -1;
}

bool IsPortMapServerRunning(Logger* logger)
{
    bool logError = true;
    try
    {
        int portMapServerPort = GetPortMapServicePortNumberFromConfig();
        if (portMapServerPort == -1)
        {
            throw std::runtime_error("could not get binary port map server port number from configuration file '" + BinaryPortMapConfigFilePath() + "'");
        }
        logError = false;
        util::TcpSocket connection("localhost", std::to_string(portMapServerPort));
        logError = true;
        bpm::HelloBinaryPortMapServerRequest request;
        cmajor::bmp::WriteMessage(connection, &request);
        std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(connection));
        if (!replyMessage)
        {
            throw std::runtime_error("'bpm::HelloBinaryPortMapServerReply' expected, please stop cmbpms process from the Task Manager and use 'Server | Start' command to run updated version");
        }
        if (replyMessage->Id() ==  bpm::bmpHelloBinaryPortMapServerReplyId)
        {
            bpm::HelloBinaryPortMapServerReply* reply = static_cast<bpm::HelloBinaryPortMapServerReply*>(replyMessage.get());
            if (logger)
            {
                logger->LogMessage("binary port map server (cmbpms) version " + reply->version + " running");
            }
            return true;
        }
        else
        {
            throw std::runtime_error("'bpm::HelloBinaryPortMapServerReply' expected, please stop cmbpms process from the Task Manager and use 'Server | Start' command to run updated version");
        }
    }
    catch (const std::exception& ex)
    {
        if (logError)
        {
            logger->LogMessage("error: " + std::string(ex.what()));
        }
    }
    return false;
}

} // namespace bpm
