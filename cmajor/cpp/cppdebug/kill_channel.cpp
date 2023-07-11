// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.kill.channel;

import cmajor.debug.gdb;
import cmajor.msg.cmdb.message;
import soul.xml.dom.parser;
import soul.xml.dom;
import util;

namespace cmajor::debug {

class KillChannel
{
public:
    static void Init();
    static void Done();
    static KillChannel& Instance() { return *instance; }
    void Start(int port_);
    void Run();
    void Stop();
    void KillGdb();
private:
    KillChannel();
    static std::unique_ptr<KillChannel> instance;
    std::thread thread;
    int port;
    util::TcpSocket listenSocket;
    bool exiting;
};

std::unique_ptr<KillChannel> KillChannel::instance;

void KillChannel::Init()
{
    instance.reset(new KillChannel());
}

void KillChannel::Done()
{
    instance.reset();
}

KillChannel::KillChannel() : exiting(false), port(0)
{
}

void RunChannel(KillChannel* channel)
{
    channel->Run();
}

void KillChannel::Start(int port_)
{
    port = port_;
    thread = std::thread(RunChannel, this);
}

void KillChannel::Run()
{
    try
    {
        while (!exiting)
        {
            listenSocket = util::TcpSocket();
            listenSocket.Bind(port);
            listenSocket.Listen(10);
            util::TcpSocket socket = listenSocket.Accept();
            if (exiting) return;
            std::string requestStr = ReadStr(socket);
            std::unique_ptr<soul::xml::Document> requestDoc = soul::xml::ParseXmlContent(requestStr, "socket");
            if (requestDoc->DocumentElement()->Name() == "killRequest")
            {
                KillRequest killRequest;
                killRequest.FromXml(requestDoc->DocumentElement());
                KillGdb();
                KillReply killReply;
                std::unique_ptr<soul::xml::Element> replyElement(killReply.ToXml("killReply"));
                soul::xml::Document replyDoc;
                replyDoc.AppendChild(replyElement.release());
                std::stringstream strStream;
                util::CodeFormatter formatter(strStream);
                replyDoc.Write(formatter);
                std::string replyStr = strStream.str();
                Write(socket, replyStr);
            }
        }
    }
    catch (...)
    {
    }
}

void KillChannel::KillGdb()
{
    try
    {
        TerminateGDB();
    }
    catch (...)
    {
    }
}

void KillChannel::Stop()
{
    try
    {
        exiting = true;
        util::TcpSocket socket("localhost", std::to_string(port));
        thread.join();
    }
    catch (...)
    {
    }
}

void StartKillChannel(int port)
{
    KillChannel::Instance().Start(port);
}

void StopKillChannel()
{
    KillChannel::Instance().Stop();
}

void InitKillChannel()
{
    KillChannel::Init();
}

void DoneKillChannel()
{
    KillChannel::Done();
}

} // namespace cmajor::debug
