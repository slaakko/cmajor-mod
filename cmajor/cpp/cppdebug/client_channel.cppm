// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.client.channel;

import soul.xml.dom;
import util;
import std.core;

export namespace cmajor::debug {

class ClientChannelUser
{
public:
    virtual ~ClientChannelUser();
    virtual std::unique_ptr<soul::xml::Element> GetIdleClientChannelMessage() = 0;
    virtual bool SendIdleChannelMessages() const = 0;
    virtual bool IsIdleChannelMessage(soul::xml::Element* message) const = 0;
    virtual void ClientChannelError(const std::string& error) = 0;
    virtual void ProcessReceivedClientChannelMessage(soul::xml::Element* message) = 0;
};

class ClientChannel
{
public:
    ClientChannel(ClientChannelUser* user_, util::TcpSocket& socket_, int timeoutMs_);
    ~ClientChannel();
    void SendMessage(soul::xml::Element* message);
    void StartSendingIdleMessages();
    void StopSendingIdleMessages();
    void Run();
private:
    std::unique_ptr<soul::xml::Document> GetMessage();
    ClientChannelUser* user;
    util::TcpSocket& socket;
    int timeoutMs;
    std::thread thread;
    std::mutex messageQueueMtx;
    std::list<std::unique_ptr<soul::xml::Document>> messageQueue;
    std::condition_variable messageEnqueued;
    bool sendIdleMessages;
};

} // namespace cmajor::debug
