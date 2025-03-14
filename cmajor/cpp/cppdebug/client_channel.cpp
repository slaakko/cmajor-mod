// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.client.channel;

import soul.xml.dom.parser;
import soul.xml.dom;
import util;

namespace cmajor::debug {

std::string ToString(const std::unique_ptr<soul::xml::Document>& messageDoc)
{
    std::stringstream strStream;
    util::CodeFormatter formatter(strStream);
    messageDoc->Write(formatter);
    return strStream.str();
}

ClientChannelUser::~ClientChannelUser()
{
}

void RunChannel(ClientChannel* channel, ClientChannelUser* user)
{
    try
    {
        channel->Run();
    }
    catch (const std::exception& ex)
    {
        user->ClientChannelError(ex.what());
    }
}

ClientChannel::ClientChannel(ClientChannelUser* user_, util::TcpSocket& socket_, int timeoutMs_) : user(user_), socket(socket_), timeoutMs(timeoutMs_), sendIdleMessages(false)
{
    thread = std::thread{ RunChannel, this, user };
}

void ClientChannel::StartSendingIdleMessages()
{
    sendIdleMessages = true;
}

void ClientChannel::StopSendingIdleMessages()
{
    sendIdleMessages = false;
}

void ClientChannel::Run()
{
    try
    {
        std::unique_ptr<soul::xml::Document> message = GetMessage();
        while (message)
        {
            std::string replyStr;
            if (user->IsIdleChannelMessage(message->DocumentElement()))
            {
                if (sendIdleMessages)
                {
                    std::string messageStr = ToString(message);
                    Write(socket, messageStr);
                    replyStr = ReadStr(socket);
                }
                else
                {
                    message = GetMessage();
                    continue;
                }
            }
            else
            {
                std::string messageStr = ToString(message);
                Write(socket, messageStr);
                replyStr = ReadStr(socket);
            }
            std::unique_ptr<soul::xml::Document> reply = soul::xml::ParseXmlContent(replyStr, "socket");
            user->ProcessReceivedClientChannelMessage(reply->DocumentElement());
            message = GetMessage();
        }
    }
    catch (const std::exception& ex)
    {
        user->ClientChannelError(ex.what());
    }
}

std::unique_ptr<soul::xml::Document> ClientChannel::GetMessage()
{
    std::unique_lock<std::mutex> lock(messageQueueMtx);
    if (user->SendIdleChannelMessages())
    {
        if (!messageEnqueued.wait_for(lock, std::chrono::milliseconds{ timeoutMs }, [this] { return !messageQueue.empty(); }))
        {
            std::unique_ptr<soul::xml::Element> idleChannelMessage = user->GetIdleClientChannelMessage();
            std::unique_ptr<soul::xml::Document> message(new soul::xml::Document());
            message->AppendChild(idleChannelMessage.release());
            return message;
        }
    }
    else
    {
        messageEnqueued.wait(lock, [this] { return !messageQueue.empty(); });
    }
    std::unique_ptr<soul::xml::Document> message(std::move(messageQueue.front()));
    messageQueue.pop_front();
    return message;
}

void ClientChannel::SendMessage(soul::xml::Element* message)
{
    std::lock_guard<std::mutex> lock(messageQueueMtx);
    if (message)
    {
        std::unique_ptr<soul::xml::Document> doc(new soul::xml::Document());
        doc->AppendChild(message);
        messageQueue.push_back(std::move(doc));
    }
    else
    {
        messageQueue.push_back(std::unique_ptr<soul::xml::Document>());
    }
    messageEnqueued.notify_one();
}

ClientChannel::~ClientChannel()
{
    try
    {
        SendMessage(nullptr);
        thread.join();
    }
    catch (const std::exception& ex)
    {
        user->ClientChannelError(ex.what());
    }
}

} // namespace cmajor::debug
