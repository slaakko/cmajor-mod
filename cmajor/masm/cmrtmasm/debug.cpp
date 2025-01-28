// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.rt.debug;

import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import util;

namespace cmajor::masm::rt {

const std::int64_t sessionTimeoutSecs = 5 * 60;

class CmdbSessionServer
{
public:
    static void Init();
    static void Done();
    static CmdbSessionServer& Instance() { return *instance; }
    void Start(const std::string& skey_, const std::string& rkey_, int port_);
    void OpenSession(util::TcpSocket& socket);
    void CloseSession(util::TcpSocket& socket);
    bool IsSessionOpen() const { return sessionOpen; }
    void SendOutput(int fileHandle, const std::string& bytes);
    int64_t ReceiveInput(uint8_t* buffer, int64_t bufferSize);
    bool InputEof() const { return inputEof; }
private:
    static std::unique_ptr<CmdbSessionServer> instance;
    CmdbSessionServer();
    std::string skey;
    std::string rkey;
    int port;
    bool sessionOpen;
    bool inputEof;
    std::string inputHexByteBuffer;
};

std::unique_ptr<CmdbSessionServer> CmdbSessionServer::instance;

CmdbSessionServer::CmdbSessionServer() : skey(), rkey(), port(), sessionOpen(false), inputEof(false)
{
}

void CmdbSessionServer::Init()
{
    instance.reset(new CmdbSessionServer());
}

void CmdbSessionServer::Done()
{
    instance.reset();
}

void CmdbSessionServer::Start(const std::string& skey_, const std::string& rkey_, int port_)
{
    skey = skey_;
    rkey = rkey_;
    port = port_;
    sessionOpen = true;
}

void CmdbSessionServer::OpenSession(util::TcpSocket& socket)
{
    soul::xml::Document openSessionRequest;
    soul::xml::Element* cmdbOpenMessage = soul::xml::MakeElement("cmdbMessage");
    openSessionRequest.AppendChild(cmdbOpenMessage);
    cmdbOpenMessage->SetAttribute("kind", "openSessionRequest");
    cmdbOpenMessage->SetAttribute("key", util::GetSha1MessageDigest(this->skey));
    soul::xml::SendDocument(socket, openSessionRequest);
    bool validOpenResponseReceived = false;
    std::unique_ptr<soul::xml::Document> openSessionResponse = soul::xml::ReceiveDocument(socket);
    if (openSessionResponse)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet(soul::xml::xpath::EvaluateToNodeSet("/cmdbMessage", openSessionResponse.get()));
        if (nodeSet->Count() == 1)
        {
            soul::xml::Node* node = nodeSet->GetNode(0);
            if (node->IsElementNode())
            {
                soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                std::string kind = element->GetAttribute("kind");
                if (kind == "openSessionResponse")
                {
                    std::string key = util::ToUtf8(element->GetAttribute("key"));
                    if (key == util::GetSha1MessageDigest(this->rkey))
                    {
                        validOpenResponseReceived = true;
                    }
                    else
                    {
                        throw std::runtime_error("invalid CMDB session key");
                    }
                }
            }
        }
    }
    if (!validOpenResponseReceived)
    {
        throw std::runtime_error("no valid open session response received");
    }
}

void CmdbSessionServer::CloseSession(util::TcpSocket& socket)
{
    soul::xml::Document closeSessionRequest;
    soul::xml::Element* cmdbCloseMessage = soul::xml::MakeElement("cmdbMessage");
    closeSessionRequest.AppendChild(cmdbCloseMessage);
    cmdbCloseMessage->SetAttribute("kind", "closeSessionRequest");
    soul::xml::SendDocument(socket, closeSessionRequest);
}

void CmdbSessionServer::SendOutput(int fileHandle, const std::string& bytes)
{
    if (!sessionOpen)
    {
        throw std::runtime_error("no CMDB session open");
    }
    util::TcpSocket socket("localhost", std::to_string(port));
    OpenSession(socket);
    soul::xml::Document outputRequest;
    soul::xml::Element* cmdbOutputMessage = soul::xml::MakeElement("cmdbMessage");
    outputRequest.AppendChild(cmdbOutputMessage);
    cmdbOutputMessage->SetAttribute("kind", "outputRequest");
    cmdbOutputMessage->SetAttribute("handle", std::to_string(fileHandle));
    cmdbOutputMessage->SetAttribute("bytes", bytes);
    soul::xml::SendDocument(socket, outputRequest);
    bool outputResponseReceived = false;
    std::unique_ptr<soul::xml::Document> outputResponse = soul::xml::ReceiveDocument(socket);
    if (outputResponse)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdbMessage", outputResponse.get());
        if (nodeSet->Count() == 1)
        {
            soul::xml::Node* node = nodeSet->GetNode(0);
            if (node->IsElementNode())
            {
                soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                std::string kind = element->GetAttribute("kind");
                if (kind == "outputResponse")
                {
                    outputResponseReceived = true;
                }
            }
        }
        CloseSession(socket);
        socket.Shutdown(util::ShutdownMode::both);
        socket.Close();
    }
    if (!outputResponseReceived)
    {
        throw std::runtime_error("no valid output response received");
    }
}

int64_t CmdbSessionServer::ReceiveInput(uint8_t* buffer, int64_t bufferSize)
{
    if (inputHexByteBuffer.empty())
    {
        if (!sessionOpen)
        {
            throw std::runtime_error("no CMDB session open");
        }
        util::TcpSocket socket("localhost", std::to_string(port));
        OpenSession(socket);
        soul::xml::Document inputRequest;
        soul::xml::Element* cmdbInputMessage = soul::xml::MakeElement("cmdbMessage");
        inputRequest.AppendChild(cmdbInputMessage);
        cmdbInputMessage->SetAttribute("kind", "inputRequest");
        soul::xml::SendDocument(socket, inputRequest);
        bool inputResponseReceived = false;
        std::unique_ptr<soul::xml::Document> inputResponse = soul::xml::ReceiveDocument(socket);
        if (inputResponse)
        {
            std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdbMessage", inputResponse.get());
            if (nodeSet->Count() == 1)
            {
                soul::xml::Node* node = nodeSet->GetNode(0);
                if (node->IsElementNode())
                {
                    soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                    std::string kind = element->GetAttribute("kind");
                    if (kind == "inputResponse")
                    {
                        std::string value = element->GetAttribute("bytes");
                        if (value.empty())
                        {
                            inputEof = true;
                        }
                        inputHexByteBuffer = value;
                        inputResponseReceived = true;
                    }
                }
            }
        }
        CloseSession(socket);
        socket.Shutdown(util::ShutdownMode::both);
        socket.Close();
        if (!inputResponseReceived)
        {
            throw std::runtime_error("no valid input response received");
        }
    }
    int64_t bytesReceived = 0;
    uint8_t* p = buffer;
    int64_t bytesToReceive = bufferSize;
    while (inputHexByteBuffer.size() >= 2 && bytesToReceive > 0)
    {
        std::string hex;
        hex.append(1, inputHexByteBuffer[0]);
        hex.append(1, inputHexByteBuffer[1]);
        inputHexByteBuffer.erase(inputHexByteBuffer.begin());
        inputHexByteBuffer.erase(inputHexByteBuffer.begin());
        uint8_t byte = util::ParseHexByte(hex);
        *p++ = byte;
        --bytesToReceive;
        ++bytesReceived;
    }
    return bytesReceived;
}

void StartCmdbSession()
{
    std::string cmdbSessionFilePath;
    std::string exePath = util::GetFullPath(util::GetPathToExecutable());
    if (exePath.ends_with(".exe"))
    {
        cmdbSessionFilePath = util::Path::ChangeExtension(exePath, ".cmdbs");
    }
    else
    {
        cmdbSessionFilePath = exePath + ".cmdbs";
    }
    if (util::FileExists(cmdbSessionFilePath))
    {
        std::unique_ptr<soul::xml::Document> sessionDoc = soul::xml::ParseXmlFile(cmdbSessionFilePath);
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdbSession/timestamp", sessionDoc.get());
        if (nodeSet->Count() == 1)
        {
            soul::xml::Node* timestampNode = nodeSet->GetNode(0);
            if (timestampNode->IsElementNode())
            {
                soul::xml::Element* timestampElement = static_cast<soul::xml::Element*>(timestampNode);
                std::string timestampStr = util::ToUtf8(timestampElement->GetAttribute("value"));
                if (!timestampStr.empty())
                {
                    time_t timestamp = std::stoll(timestampStr);
                    time_t now;
                    time(&now);
                    if (now - timestamp >= 0 && now - timestamp < sessionTimeoutSecs)
                    {
                        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdbSession/skey", sessionDoc.get());
                        if (nodeSet->Count() == 1)
                        {
                            soul::xml::Node* keyNode = nodeSet->GetNode(0);
                            if (keyNode->IsElementNode())
                            {
                                soul::xml::Element* keyElement = static_cast<soul::xml::Element*>(keyNode);
                                std::string skeyStr = keyElement->GetAttribute("value");
                                if (!skeyStr.empty())
                                {
                                    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdbSession/rkey", sessionDoc.get());
                                    if (nodeSet->Count() == 1)
                                    {
                                        soul::xml::Node* keyNode = nodeSet->GetNode(0);
                                        if (keyNode->IsElementNode())
                                        {
                                            soul::xml::Element* keyElement = static_cast<soul::xml::Element*>(keyNode);
                                            std::string rkeyStr = keyElement->GetAttribute("value");
                                            if (!rkeyStr.empty())
                                            {
                                                std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdbSession/port", sessionDoc.get());
                                                if (nodeSet->Count() == 1)
                                                {
                                                    soul::xml::Node* portNode = nodeSet->GetNode(0);
                                                    if (portNode->IsElementNode())
                                                    {
                                                        soul::xml::Element* portElement = static_cast<soul::xml::Element*>(portNode);
                                                        std::string portStr = portElement->GetAttribute("value");
                                                        if (!portStr.empty())
                                                        {
                                                            int port = std::stoi(portStr);
                                                            CmdbSessionServer::Instance().Start(skeyStr, rkeyStr, port);
                                                        }
                                                        else
                                                        {
                                                            throw std::runtime_error("port is empty");
                                                        }
                                                    }
                                                }
                                            }
                                            else
                                            {
                                                throw std::runtime_error("key is empty");
                                            }
                                        }
                                    }
                                    else
                                    {
                                        throw std::runtime_error("key is empty");
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    throw std::runtime_error("timestamp is empty");
                }
            }
        }
    }
}

bool IsCmdbSessionOpen()
{
    return CmdbSessionServer::Instance().IsSessionOpen();
}

void WriteBytesToCmdbSession(int fileHandle, const uint8_t* buffer, int64_t count)
{
    std::string output;
    for (int64_t i = 0; i < count; ++i)
    {
        output.append(util::ToHexString(buffer[i]));
    }
    CmdbSessionServer::Instance().SendOutput(fileHandle, output);
}

int64_t ReadBytesFromCmdbSession(uint8_t* buffer, int64_t bufferSize)
{
    return CmdbSessionServer::Instance().ReceiveInput(buffer, bufferSize);
}

bool CmdbSessionEof()
{
    return CmdbSessionServer::Instance().InputEof();
}

void InitCmdbSession()
{
    CmdbSessionServer::Init();
}

void DoneCmdbSession()
{
    CmdbSessionServer::Done();
}

} // namespace cmajor::rt

