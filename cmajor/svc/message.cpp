// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module cmajor.service.message;

import cmajor.service.request;

namespace cmajor::service {

wing::Window* serviceMessageHandlerView = nullptr;

class ServiceMessageQueue
{
public:
    static ServiceMessageQueue& Instance();
    bool IsEmpty();
    void Put(ServiceMessage* message);
    std::unique_ptr<ServiceMessage> Get();
private:
    static std::unique_ptr<ServiceMessageQueue> instance;
    std::list<std::unique_ptr<ServiceMessage>> messageQueue;
    std::recursive_mutex mtx;
};

ServiceMessageQueue& ServiceMessageQueue::Instance()
{
    static ServiceMessageQueue instance;
    return instance;
}

bool ServiceMessageQueue::IsEmpty()
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    return messageQueue.empty();
}

void ServiceMessageQueue::Put(ServiceMessage* message)
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    messageQueue.push_back(std::unique_ptr<ServiceMessage>(message));
}

std::unique_ptr<ServiceMessage> ServiceMessageQueue::Get()
{
    std::lock_guard<std::recursive_mutex> lock(mtx);
    if (!IsEmpty())
    {
        std::unique_ptr<ServiceMessage> message = std::move(messageQueue.front());
        messageQueue.pop_front();
        return message;
    }
    return std::unique_ptr<ServiceMessage>();
}

ServiceMessage::ServiceMessage(ServiceMessageKind kind_) : kind(kind_)
{
}

ServiceMessage::~ServiceMessage()
{
}

ClearOutputServiceMessage::ClearOutputServiceMessage() : ServiceMessage(ServiceMessageKind::clearOutput)
{
}

OutputServiceMessage::OutputServiceMessage(const std::string& text_) : ServiceMessage(ServiceMessageKind::outputMessage), text(text_)
{
}

ClearDebugLogMessage::ClearDebugLogMessage() : ServiceMessage(ServiceMessageKind::clearDebugLog)
{
}

DebugLogMessage::DebugLogMessage(const std::string& text_) : ServiceMessage(ServiceMessageKind::debugLogMessage), text(text_)
{
}

void SetServiceMessageHandlerView(wing::Window* view)
{
    serviceMessageHandlerView = view;
}

void PutServiceMessage(ServiceMessage* message)
{
    if (Exiting()) return;
    ServiceMessageQueue::Instance().Put(message);
    if (serviceMessageHandlerView)
    {
        SendMessage(serviceMessageHandlerView->Handle(), SM_SERVICE_MESSAGE_AVAILABLE, 0, 0);
    }
}

void PutClearOutputServiceMessage()
{
    PutServiceMessage(new ClearOutputServiceMessage());
}

void PutOutputServiceMessage(const std::string& messageText)
{
    PutServiceMessage(new OutputServiceMessage(messageText));
}

bool ServiceMessageQueueEmpty()
{
    return ServiceMessageQueue::Instance().IsEmpty();
}

std::unique_ptr<ServiceMessage> GetServiceMessage()
{
    std::unique_ptr<ServiceMessage> message = ServiceMessageQueue::Instance().Get();
    return message;
}

} // namespace cmajor::service
