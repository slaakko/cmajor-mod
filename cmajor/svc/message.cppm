// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

export module cmajor.service.message;

import wing;
import std.core;

export namespace cmajor::service {

const int SM_SERVICE_MESSAGE_AVAILABLE = WM_USER + 1;

enum class ServiceMessageKind : int
{
    clearOutput = 0, outputMessage = 1, buildResult = 2, buildStoppedMessage = 3, getDefinitionReply = 5, getDefinitionError = 6,
    startDebugReply = 7, startError = 8, execReply = 9, debugError = 10,
    breakReply = 16, deleteReply = 17, depthReply = 18, framesReply = 19, errorReply = 20,
    evaluateReply = 21, countReply = 22, evaluateChildReply = 23, targetRunning = 24, targetInput = 25, targetOutput = 26, debugServiceStopped = 27,
    processTerminated = 28, runServiceStopped = 29,
    loadEditModuleReply = 30, loadEditModuleError = 31, parseSourceReply = 32, parseSourceError = 33, resetEditModuleCacheReply = 34, resetEditModuleCacheError = 35,
    getCCListReply = 36, getCCListError = 37, getParamHelpListReply = 38, getParamHelpListError = 39, clearDebugLog = 40, debugLogMessage = 41
};

class ServiceMessage
{
public:
    ServiceMessage(ServiceMessageKind kind_);
    virtual ~ServiceMessage();
    ServiceMessageKind Kind() const { return kind; }
private:
    ServiceMessageKind kind;
};

class ClearOutputServiceMessage : public ServiceMessage
{
public:
    ClearOutputServiceMessage();
};

class OutputServiceMessage : public ServiceMessage
{
public:
    OutputServiceMessage(const std::string& text_);
    const std::string& Text() const { return text; }
private:
    std::string text;
};

class ClearDebugLogMessage : public ServiceMessage
{
public:
    ClearDebugLogMessage();
};

class DebugLogMessage : public ServiceMessage
{
public:
    DebugLogMessage(const std::string& text_);
    const std::string& Text() const { return text; }
private:
    std::string text;
};

void SetServiceMessageHandlerView(wing::Window* view);
void PutServiceMessage(ServiceMessage* message);
void PutClearOutputServiceMessage();
void PutOutputServiceMessage(const std::string& messageText);
bool ServiceMessageQueueEmpty();
std::unique_ptr<ServiceMessage> GetServiceMessage();

} // namespace cmajor::service
