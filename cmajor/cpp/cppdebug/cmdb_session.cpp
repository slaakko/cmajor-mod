// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.cmdb.session;

import soul.xml.dom;
import soul.xml.xpath;
import soul.xml.dom.parser;
import util;
import std.filesystem;

namespace cmajor::debug {

int port = 54322;

RemoveCmdbSessionFileGuard::RemoveCmdbSessionFileGuard(const std::string& cmdbSessionFilePath_) : cmdbSessionFilePath(cmdbSessionFilePath_)
{
}

RemoveCmdbSessionFileGuard::~RemoveCmdbSessionFileGuard()
{
    try
    {
        if (!cmdbSessionFilePath.empty())
        {
            std::filesystem::remove(cmdbSessionFilePath);
        }
    }
    catch (...)
    {
    }
}

CmdbSessionClient::~CmdbSessionClient()
{
}

class CmdbSession
{
public:
    static CmdbSession& Instance();
    void SetKeys(const std::string& skeyStr_, const std::string& rkeyStr_);
    void Start(CmdbSessionClient* client_);
    void Stop();
    void Run();
    void WaitForStarted();
    void Transact(util::TcpSocket& socket);
    void OpenSession(util::TcpSocket& socket, soul::xml::Element* request);
    void CloseSession();
    void GetInput(util::TcpSocket& socket);
    void PutOutput(util::TcpSocket& socket, soul::xml::Element* request);
private:
    static std::unique_ptr<CmdbSession> instance;
    std::string skeyStr;
    std::string rkeyStr;
    std::thread thread;
    util::TcpSocket listenSocket;
    std::mutex mtx;
    std::condition_variable started;
    std::atomic<bool> exiting;
    std::atomic<bool> sessionClosed;
    CmdbSessionClient* client;
    CmdbSession();
};

CmdbSession& CmdbSession::Instance()
{
    static CmdbSession instance;
    return instance;
}

CmdbSession::CmdbSession() : exiting(false), sessionClosed(false), client(nullptr)
{
}

void CmdbSession::SetKeys(const std::string& skeyStr_, const std::string& rkeyStr_)
{
    skeyStr = skeyStr_;
    rkeyStr = rkeyStr_;
}

void CmdbSession::Run()
{
    try
    {
        listenSocket = util::TcpSocket();
        listenSocket.Bind(port);
        listenSocket.Listen(10);
        started.notify_one();
        while (!exiting)
        {
            util::TcpSocket socket = listenSocket.Accept();
            if (exiting)
            {
                listenSocket.Close();
                return;
            }
            sessionClosed = false;
            while (!sessionClosed)
            {
                Transact(socket);
            }
        }
        listenSocket.Close();
    }
    catch (const std::exception& ex)
    {
        listenSocket.Close();
        if (!exiting)
        {
            std::cerr << "error from CMDB session run: " << ex.what() << std::endl;
        }
    }
}

void CmdbSession::WaitForStarted()
{
    std::unique_lock<std::mutex> lock(mtx);
    started.wait(lock);
}

void RunSession()
{
    CmdbSession::Instance().Run();
}

void CmdbSession::Start(CmdbSessionClient* client_)
{
    exiting = false;
    client = client_;
    thread = std::thread{ RunSession };
}

void CmdbSession::Stop()
{
    try
    {
        exiting = true;
        try
        {
            util::TcpSocket socket;
            socket.Connect("localhost", std::to_string(port));
        }
        catch (...)
        {
        }
        thread.join();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "error stopping CMDB session: " << ex.what() << std::endl;
    }
}

void CmdbSession::OpenSession(util::TcpSocket& socket, soul::xml::Element* request)
{
    std::string skeyValue = request->GetAttribute("key");
    if (!skeyValue.empty())
    {
        if (util::GetSha1MessageDigest(skeyStr) == skeyValue)
        {
            soul::xml::Document openSessionResponse;
            soul::xml::Element* responseMessage = soul::xml::MakeElement("cmdbMessage");
            responseMessage->SetAttribute("kind", "openSessionResponse");
            responseMessage->SetAttribute("key", util::GetSha1MessageDigest(rkeyStr));
            openSessionResponse.AppendChild(responseMessage);
            soul::xml::SendDocument(socket, openSessionResponse);
            return;
        }
    }
    throw std::runtime_error("invalid session key");
}

void CmdbSession::CloseSession()
{
    sessionClosed = true;
}

void CmdbSession::Transact(util::TcpSocket& socket)
{
    std::unique_ptr<soul::xml::Document> request = soul::xml::ReceiveDocument(socket);
    if (request)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdbMessage", request.get());
        if (nodeSet->Count() == 1)
        {
            soul::xml::Node* node = nodeSet->GetNode(0);
            if (node->IsElementNode())
            {
                soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                std::string kind = element->GetAttribute("kind");
                if (kind == "openSessionRequest")
                {
                    OpenSession(socket, element);
                }
                else if (kind == "closeSessionRequest")
                {
                    CloseSession();
                }
                else if (kind == "inputRequest")
                {
                    GetInput(socket);
                }
                else if (kind == "outputRequest")
                {
                    PutOutput(socket, element);
                }
                else
                {
                    throw std::runtime_error("invalid CMDB session request received: kind='" + kind + "'");
                }
            }
        }
    }
    else
    {
        CloseSession();
    }
}

void CmdbSession::GetInput(util::TcpSocket& socket)
{
    std::string bytes = client->GetTargetInputBytes();
    soul::xml::Document inputResponse;
    soul::xml::Element* inputResponseElement = soul::xml::MakeElement("cmdbMessage");
    inputResponseElement->SetAttribute("kind", "inputResponse");
    inputResponseElement->SetAttribute("bytes", bytes);
    inputResponse.AppendChild(inputResponseElement);
    soul::xml::SendDocument(socket, inputResponse);
}

void CmdbSession::PutOutput(util::TcpSocket& socket, soul::xml::Element* request)
{
    std::string handleValue = request->GetAttribute("handle");
    if (!handleValue.empty())
    {
        std::string handleStr = handleValue;
        int handle = std::stoi(handleStr);
        if (handle == 1 || handle == 2)
        {
            std::string bytesValue = request->GetAttribute("bytes");
            if (!bytesValue.empty())
            {
                std::string output;
                std::string bytesStr = bytesValue;
                int n = bytesStr.length() / 2;
                for (int i = 0; i < n; ++i)
                {
                    std::string hex;
                    hex.append(1, bytesStr[i * 2]);
                    hex.append(1, bytesStr[i * 2 + 1]);
                    uint8_t x = util::ParseHexByte(hex);
                    output.append(1, static_cast<char>(x));
                }
                client->WriteTargetOuput(handle, output);
                soul::xml::Document outputResponse;
                soul::xml::Element* outputResponseElement = soul::xml::MakeElement("cmdbMessage");
                outputResponseElement->SetAttribute("kind", "outputResponse");
                outputResponse.AppendChild(outputResponseElement);
                soul::xml::SendDocument(socket, outputResponse);
            }
        }
    }
}

void SetCmdbSessionPort(int port_)
{
    port = port_;
}

void StartCmdbSession(const std::string& cmdbSessionFilePath, DebuggerOutputWriter* outputWriter, CmdbSessionClient* client, bool verbose)
{
    if (verbose)
    {
        outputWriter->WriteLogMessage("starting CMDB session...");
    }
    std::ofstream cmdbSessionFile(cmdbSessionFilePath);
    util::CodeFormatter sessionFileFormatter(cmdbSessionFile);
    sessionFileFormatter.SetIndentSize(1);
    soul::xml::Document cmdbSessionDoc;
    soul::xml::Element* sessionElement = soul::xml::MakeElement("cmdbSession");
    soul::xml::Element* timestampElement = soul::xml::MakeElement("timestamp");
    time_t t;
    time(&t);
    timestampElement->SetAttribute("value", std::to_string(t));
    sessionElement->AppendChild(timestampElement);
    soul::xml::Element* skeyElement = soul::xml::MakeElement("skey");
    util::uuid skey = util::random_uuid();
    std::string skeyStr = util::ToString(skey);
    skeyElement->SetAttribute("value", skeyStr);
    sessionElement->AppendChild(skeyElement);
    soul::xml::Element* rkeyElement = soul::xml::MakeElement("rkey");
    util::uuid rkey = util::random_uuid();
    std::string rkeyStr = util::ToString(rkey);
    rkeyElement->SetAttribute("value", rkeyStr);
    sessionElement->AppendChild(rkeyElement);
    soul::xml::Element* portElement = soul::xml::MakeElement("port");
    portElement->SetAttribute("value", std::to_string(port));
    sessionElement->AppendChild(portElement);
    cmdbSessionDoc.AppendChild(sessionElement);
    cmdbSessionDoc.Write(sessionFileFormatter);
    CmdbSession::Instance().SetKeys(skeyStr, rkeyStr);
    CmdbSession::Instance().Start(client);
    CmdbSession::Instance().WaitForStarted();
}

void StopCmdbSession()
{
    CmdbSession::Instance().Stop();
}

} // namespace cmajor::debug

