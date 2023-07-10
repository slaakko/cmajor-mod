// =================================
// Copyright (c) 2023 Seppo Laakko
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
    clearOutput = 0, outputMessage = 1, buildReply = 2, buildError = 3, stopBuild = 4, getDefinitionReply = 5, getDefinitionError = 6,
    startDebugReply = 7, startError = 8, continueReply = 9, nextReply = 10, stepReply = 11, finishReply = 12, untilReply = 13, breakReply = 14, deleteReply = 15, depthReply = 16, framesReply = 17,
    evaluateReply = 18, countReply = 19, evaluateChildReply = 20, targetRunning = 21, targetInput = 22, targetOutput = 23, debugServiceStopped = 24, processTerminated = 25, runServiceStopped = 26,
    loadEditModuleReply = 27, loadEditModuleError = 28, parseSourceReply = 29, parseSourceError = 30, resetEditModuleCacheReply = 31, resetEditModuleCacheError = 32,
    getCCListReply = 33, getCCListError = 34, getParamHelpListReply = 35, getParamHelpListError = 36
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

void SetServiceMessageHandlerView(wing::Window* view);
void PutServiceMessage(ServiceMessage* message);
void PutClearOutputServiceMessage();
void PutOutputServiceMessage(const std::string& messageText);
bool ServiceMessageQueueEmpty();
std::unique_ptr<ServiceMessage> GetServiceMessage();

} // namespace cmajor::service
