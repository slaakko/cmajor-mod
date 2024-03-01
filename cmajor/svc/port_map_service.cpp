// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.port.map.service;

import cmajor.service.logger;
import cmajor.service.message;
import cmajor.service.config;
import cmajor.bmp;
import bpm.client;
import util;

namespace cmajor::service {

int GetPortMapServicePortNumberFromConfig()
{
    return bpm::GetPortMapServicePortNumberFromConfig();
}

bool StartPortMapServer()
{
    ServiceLogger logger;
    return bpm::StartPortMapServer(&logger);
}

bool StopPortMapServer()
{
    ServiceLogger logger;
    return bpm::StopPortMapServer(&logger);
}

int GetPortMapServicePortNumber(bool& portMapServerStarted)
{
    ServiceLogger logger;
    return bpm::GetPortMapServicePortNumber(&logger, portMapServerStarted);
}

int GetFreePortNumber(const std::string& processName)
{
    ServiceLogger logger;
    return bpm::GetFreePortNumber(&logger, processName);
}

bool IsPortMapServerRunning()
{
    ServiceLogger logger;
    return bpm::IsPortMapServerRunning(&logger);
}

std::vector<bpm::PortLease> GetPortLeases()
{
    std::vector<bpm::PortLease> portLeases;
    try
    {
        int portMapServerPort = GetPortMapServicePortNumberFromConfig();
        if (portMapServerPort == -1)
        {
            throw std::runtime_error("could not get binary port map server port number from configuration file '" + BinaryPortMapConfigFilePath() + "'");
        }
        util::TcpSocket connection("localhost", std::to_string(portMapServerPort));
        bpm::ViewPortLeaseRequest request;
        cmajor::bmp::WriteMessage(connection, &request);
        std::unique_ptr<cmajor::bmp::BinaryMessage> replyMessage(cmajor::bmp::ReadMessage(connection));
        if (replyMessage)
        {
            if (replyMessage->Id() == bpm::bmpViewPortLeaseReplyId)
            {
                bpm::ViewPortLeaseReply* reply = static_cast<bpm::ViewPortLeaseReply*>(replyMessage.get());
                return reply->portLeases;
            }
            else
            {
                throw std::runtime_error("'bpm::ViewPortLeaseReply' expected, message id=" + std::to_string(replyMessage->Id()));
            }
        }
        else
        {
            throw std::runtime_error("'bpm::ViewPortLeaseReply' expected, reply is empty");
        }
    }
    catch (const std::exception& ex)
    {
        PutOutputServiceMessage("error: " + std::string(ex.what()));
    }
    return portLeases;
}

} // namespace cmajor::service
