// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.service;

import cmajor.debuggers;
import cmajor.service.request;
import cmajor.service.message;
import std.core;

export namespace cmajor::service {

struct DebugServiceStartParams
{
    DebugServiceStartParams& Backend(const std::string& backend_);
    DebugServiceStartParams& Config(const std::string& config_);
    DebugServiceStartParams& ExecutableFilePath(const std::string& executableFilePath_);
    DebugServiceStartParams& ProgramArguments(const std::string& args);
    std::string backend;
    std::string config;
    std::string executableFilePath;
    std::string programArguments;
    std::vector<cmajor::debugger::Breakpoint*> breakpoints;
};

class StartDebugServiceRequest : public Request
{
public:
    StartDebugServiceRequest(const DebugServiceStartParams& startParams_);
    const DebugServiceStartParams& StartParams() const { return startParams; }
    void Execute() override;
    std::string Name() const override { return "startDebugServiceRequest"; }
private:
    DebugServiceStartParams startParams;
};

class StopDebugServiceRequest : public Request
{
public:
    StopDebugServiceRequest();
    void Execute() override;
    std::string Name() const override { return "stopDebugServiceRequest"; }
};

class RunDebugServiceRequest : public Request
{
public:
    RunDebugServiceRequest();
    void Execute() override;
    std::string Name() const override { return "runDebugServiceRequest"; }
};

class ContinueDebugServiceRequest : public Request
{
public:
    ContinueDebugServiceRequest();
    void Execute() override;
    std::string Name() const override { return "continueDebugServiceRequest"; }
};

class NextDebugServiceRequest : public Request
{
public:
    NextDebugServiceRequest();
    void Execute() override;
    std::string Name() const override { return "nextDebugServiceRequest"; }
};

class StepDebugServiceRequest : public Request
{
public:
    StepDebugServiceRequest();
    void Execute() override;
    std::string Name() const override { return "stepDebugServiceRequest"; }
};

class FinishDebugServiceRequest : public Request
{
public:
    FinishDebugServiceRequest();
    void Execute() override;
    std::string Name() const override { return "finishDebugServiceRequest"; }
};

class UntilDebugServiceRequest : public Request
{
public:
    UntilDebugServiceRequest(const cmajor::debugger::Location& loc_);
    void Execute() override;
    std::string Name() const override { return "untilDebugServiceRequest"; }
private:
    cmajor::debugger::Location loc;
};

class PutDebugServiceProgramInputLineRequest : public Request
{
public:
    PutDebugServiceProgramInputLineRequest(const std::string& inputLine_);
    void Execute() override;
    std::string Name() const override;
private:
    std::string inputLine;
};

class SetDebugServiceProgramEofRequest : public Request
{
public:
    SetDebugServiceProgramEofRequest();
    void Execute() override;
    std::string Name() const override;
};

class StartDebugServiceReplyServiceMessage : public ServiceMessage
{
public:    
    StartDebugServiceReplyServiceMessage(cmajor::debugger::Reply* reply_);
    cmajor::debugger::Reply* Reply() const { return reply.get(); }
private:
    std::unique_ptr<cmajor::debugger::Reply> reply;
};

class StartDebugServiceErrorServiceMessage : public ServiceMessage
{
public:
    StartDebugServiceErrorServiceMessage(const std::string& errorMessage_);
    const std::string& ErrorMessage() const { return errorMessage; }
private:
    std::string errorMessage;
};

class DebugServiceStoppedServiceMessage : public ServiceMessage
{
public:
    DebugServiceStoppedServiceMessage();
};

class ExecDebugServiceReplyServiceMessage : public ServiceMessage
{
public:
    ExecDebugServiceReplyServiceMessage(cmajor::debugger::Reply* reply_);
    cmajor::debugger::Reply* Reply() const { return reply.get(); }
private:
    std::unique_ptr<cmajor::debugger::Reply> reply;
};

class DebugErrorServiceMessage : public ServiceMessage
{
public:
    DebugErrorServiceMessage(const std::string& errorMessage_);
    const std::string& ErrorMessage() const { return errorMessage; }
private:
    std::string errorMessage;
};

class TargetOutputServiceMessage : public ServiceMessage
{
public:
    TargetOutputServiceMessage(const cmajor::debugger::OutputRequest& outputRequest_);
    const cmajor::debugger::OutputRequest& GetOutputRequest() const { return outputRequest; }
private:
    cmajor::debugger::OutputRequest outputRequest;
};

void StartDebugService(const DebugServiceStartParams& startParams);
void StopDebugService();
bool DebugRequestInProgress();

void SetDebugServiceProgramTargetInputEof();
void PutDebugServiceProgramTargetInputLine(const std::string& targetInputLine);

} // cmajor::service
