// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.service;

import cmajor.debuggers;
import cmajor.info;
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
    UntilDebugServiceRequest(const cmajor::info::db::Location& loc_);
    void Execute() override;
    std::string Name() const override { return "untilDebugServiceRequest"; }
private:
    cmajor::info::db::Location loc;
};

class DepthDebugServiceRequest : public Request
{
public:
    DepthDebugServiceRequest();
    void Execute() override;
    std::string Name() const override { return "depthDebugServiceRequest"; }
};

class FramesDebugServiceRequest : public Request
{
public:
    FramesDebugServiceRequest(int lowFrame_, int highFrame_);
    void Execute() override;
    std::string Name() const override { return "framesDebugServiceRequest"; }
private:
    int lowFrame;
    int highFrame;
};

class CountDebugServiceRequest : public Request
{
public:
    CountDebugServiceRequest(const std::string& expression_);
    void Execute() override;
    std::string Name() const override { return "countDebugServiceRequest"; }
private:
    std::string expression;
};

class EvaluateChildDebugServiceRequest : public Request
{
public:
    EvaluateChildDebugServiceRequest(const std::string& expression_, int start_, int count_);
    void Execute() override;
    std::string Name() const override { return "evaluateChildDebugServiceRequest"; }
private:
    std::string expression;
    int start;
    int count;
};

class EvaluateDebugServiceRequest : public Request
{
public:
    EvaluateDebugServiceRequest(const std::string& expression_, int requestId_);
    void Execute() override;
    std::string Name() const override { return "evaluateDebugServiceRequest"; }
private:
    std::string expression;
    int requestId;
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

class DepthDebugServiceReplyServiceMessage : public ServiceMessage
{
public:
    DepthDebugServiceReplyServiceMessage(const cmajor::info::db::DepthReply& depthReply_);
    const cmajor::info::db::DepthReply& DepthReply() const { return depthReply; }
private:
    cmajor::info::db::DepthReply depthReply;
};

class FramesDebugServiceReplyServiceMessage : public ServiceMessage
{
public:
    FramesDebugServiceReplyServiceMessage(const cmajor::info::db::FramesReply& framesReply_);
    const cmajor::info::db::FramesReply& FramesReply() const { return framesReply; }
private:
    cmajor::info::db::FramesReply framesReply;
};

class CountDebugServiceReplyServiceMessage : public ServiceMessage
{
public:
    CountDebugServiceReplyServiceMessage(const cmajor::info::db::CountReply& countReply_);
    const cmajor::info::db::CountReply& CountReply() const { return countReply; }
private:
    cmajor::info::db::CountReply countReply;
};

class EvaluateChildDebugServiceReplyServiceMessage : public ServiceMessage
{
public:
    EvaluateChildDebugServiceReplyServiceMessage(const cmajor::info::db::EvaluateChildReply& reply_);
    const cmajor::info::db::EvaluateChildReply& Reply() const { return reply; }
private:
    cmajor::info::db::EvaluateChildReply reply;
};

class EvaluateDebugServiceReplyServiceMessage : public  ServiceMessage
{
public:
    EvaluateDebugServiceReplyServiceMessage(const cmajor::info::db::EvaluateReply& reply_, int requestId_);
    const cmajor::info::db::EvaluateReply& Reply() const { return reply; }
    int RequestId() const { return requestId; }
private:
    cmajor::info::db::EvaluateReply reply;
    int requestId;
};

class TargetOutputServiceMessage : public ServiceMessage
{
public:
    TargetOutputServiceMessage(const cmajor::debugger::OutputRequest& outputRequest_);
    const cmajor::debugger::OutputRequest& GetOutputRequest() const { return outputRequest; }
private:
    cmajor::debugger::OutputRequest outputRequest;
};

class TargetInputServiceMessage : public ServiceMessage
{
public:
    TargetInputServiceMessage();
};

class TargetRunningServiceMessage : public ServiceMessage
{
public:
    TargetRunningServiceMessage();
};

void StartDebugService(const DebugServiceStartParams& startParams);
void StopDebugService();
bool DebugRequestInProgress();

void SetDebugServiceProgramTargetInputEof();
void PutDebugServiceProgramTargetInputLine(const std::string& targetInputLine);

} // cmajor::service

