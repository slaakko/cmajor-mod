// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.service;

import cmajor.service.message;
import cmajor.service.breakpoint;
import cmajor.debug.message;
import cmajor.common.message;
import std.core;

export namespace cmajor::service {

struct DebugServiceStartParams
{
    DebugServiceStartParams();
    DebugServiceStartParams& ProcessName(const std::string& processName_);
    DebugServiceStartParams& Pid(int pid_);
    DebugServiceStartParams& Backend(const std::string& backend_);
    DebugServiceStartParams& Config(const std::string& config_);
    DebugServiceStartParams& ProjectFilePath(const std::string& projectFilePath_);
    DebugServiceStartParams& ExecutableName(const std::string& executableName_);
    DebugServiceStartParams& ProgramArguments(const std::string& programArguments_);
    DebugServiceStartParams& DebugServer(bool debugServer_);
    DebugServiceStartParams& Log(bool log_);
    DebugServiceStartParams& Wait(bool wait_);
    DebugServiceStartParams& Verbose(bool verbose_);
    std::string processName;
    int pid;
    std::string backend;
    std::string config;
    std::string projectFilePath;
    std::string executableName;
    std::string programArguments;
    bool debugServer;
    bool log;
    bool wait;
    bool verbose;
};

class DebugServiceRequest
{
public:
    virtual ~DebugServiceRequest();
    virtual void Execute() = 0;
    virtual std::string Name() const = 0;
    virtual void Failed(const std::string& error) = 0;
};

class RunStartDebugServiceRequest : public DebugServiceRequest
{
public:
    RunStartDebugServiceRequest();
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
};

class StartReplyServiceMessage : public ServiceMessage
{
public:
    StartReplyServiceMessage(const db::StartDebugReply& startReply_);
    const db::StartDebugReply& GetStartDebugReply() const { return startReply; }
private:
    db::StartDebugReply startReply;
};

class StartErrorServiceMessage : public ServiceMessage
{
public:
    StartErrorServiceMessage(const std::string& error_);
    const std::string& Error() const { return error; }
private:
    std::string error;
};

class RunStopDebugServiceRequest : public DebugServiceRequest
{
public:
    RunStopDebugServiceRequest();
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
};

class TargetRunningServiceMessage : public ServiceMessage
{
public:
    TargetRunningServiceMessage();
};

class TargetInputServiceMessage : public ServiceMessage
{
public:
    TargetInputServiceMessage();
};

class TargetOutputServiceMessage : public ServiceMessage
{
public:
    TargetOutputServiceMessage(const db::TargetOutputRequest& targetOutputRequest_);
    const db::TargetOutputRequest& GetTargetOutputRequest() const { return targetOutputRequest; }
private:
    db::TargetOutputRequest targetOutputRequest;
};

class RunContinueDebugServiceRequest : public DebugServiceRequest
{
public:
    RunContinueDebugServiceRequest();
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
};

class ContinueReplyServiceMessage : public ServiceMessage
{
public:
    ContinueReplyServiceMessage(const db::ContinueReply& continueReply_);
    const db::ContinueReply& GetContinueReply() const { return continueReply; }
private:
    db::ContinueReply continueReply;
};

class RunNextDebugServiceRequest : public DebugServiceRequest
{
public:
    RunNextDebugServiceRequest();
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
};

class NextReplyServiceMessage : public ServiceMessage
{
public:
    NextReplyServiceMessage(const db::NextReply& nextReply_);
    const db::NextReply& GetNextReply() const { return nextReply; }
private:
    db::NextReply nextReply;
};

class RunStepDebugServiceRequest : public DebugServiceRequest
{
public:
    RunStepDebugServiceRequest();
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
};

class StepReplyServiceMessage : public ServiceMessage
{
public:
    StepReplyServiceMessage(const db::StepReply& stepReply_);
    const db::StepReply& GetStepReply() const { return stepReply; }
private:
    db::StepReply stepReply;
};

class RunFinishDebugServiceRequest : public DebugServiceRequest
{
public:
    RunFinishDebugServiceRequest();
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
};

class FinishReplyServiceMessage : public ServiceMessage
{
public:
    FinishReplyServiceMessage(const db::FinishReply& finishReply_);
    const db::FinishReply& GetFinishReply() const { return finishReply; }
private:
    db::FinishReply finishReply;
};

class RunUntilDebugServiceRequest : public DebugServiceRequest
{
public:
    RunUntilDebugServiceRequest(const common::SourceLoc& sourceLocation_);
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
private:
    common::SourceLoc sourceLocation;
};

class UntilReplyServiceMessage : public ServiceMessage
{
public:
    UntilReplyServiceMessage(const db::UntilReply& untilReply_);
    const db::UntilReply& GetUntilReply() const { return untilReply; }
private:
    db::UntilReply untilReply;
};

class RunBreakDebugServiceRequest : public DebugServiceRequest
{
public:
    RunBreakDebugServiceRequest(cmajor::service::Breakpoint* breakpoint_);
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
private:
    cmajor::service::Breakpoint* breakpoint;
};

class BreakReplyServiceMessage : public ServiceMessage
{
public:
    BreakReplyServiceMessage(const db::BreakReply& breakReply_);
    const db::BreakReply& GetBreakReply() const { return breakReply; }
private:
    db::BreakReply breakReply;
};

class RunDeleteDebugServiceRequest : public DebugServiceRequest
{
public:
    RunDeleteDebugServiceRequest(const std::string& breakpointId_);
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
private:
    std::string breakpointId;
};

class DeleteReplyServiceMessage : public ServiceMessage
{
public:
    DeleteReplyServiceMessage(const db::DeleteReply& deleteReply_);
    const db::DeleteReply& GetDeleteReply() const { return deleteReply; }
private:
    db::DeleteReply deleteReply;
};

class RunDepthDebugServiceRequest : public DebugServiceRequest
{
public:
    RunDepthDebugServiceRequest();
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
};

class DepthReplyServiceMessage : public ServiceMessage
{
public:
    DepthReplyServiceMessage(const db::DepthReply& depthReply_);
    const db::DepthReply& GetDepthReply() const { return depthReply; }
private:
    db::DepthReply depthReply;
};

class RunFramesDebugServiceRequest : public DebugServiceRequest
{
public:
    RunFramesDebugServiceRequest(int lowFrame_, int highFrame_);
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
private:
    int lowFrame;
    int highFrame;
};

class FramesReplyServiceMessage : public ServiceMessage
{
public:
    FramesReplyServiceMessage(const db::FramesReply& framesReply_);
    const db::FramesReply& GetFramesReply() const { return framesReply; }
private:
    db::FramesReply framesReply;
};

class RunEvaluateDebugServiceRequest : public DebugServiceRequest
{
public:
    RunEvaluateDebugServiceRequest(const std::string& expression_, int requestId_);
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
private:
    std::string expression;
    int requestId;
};

class EvaluateReplyServiceMessage : public ServiceMessage
{
public:
    EvaluateReplyServiceMessage(const db::EvaluateReply& evaluateReply_, int requestId_);
    const db::EvaluateReply& GetEvaluateReply() const { return evaluateReply; }
    int RequestId() const { return requestId; }
private:
    db::EvaluateReply evaluateReply;
    int requestId;
};

class RunCountDebugServiceRequest : public DebugServiceRequest
{
public:
    RunCountDebugServiceRequest(const std::string& expression_);
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
private:
    std::string expression;
};

class CountReplyServiceMessage : public ServiceMessage
{
public:
    CountReplyServiceMessage(const db::CountReply& countReply_);
    const db::CountReply& GetCountReply() const { return countReply; }
private:
    db::CountReply countReply;
};

class RunEvaluateChildRequest : public DebugServiceRequest
{
public:
    RunEvaluateChildRequest(const std::string& expression_, int start_, int count_);
    void Execute() override;
    std::string Name() const override;
    void Failed(const std::string& error) override;
private:
    std::string expression;
    int start;
    int count;
};

class EvaluateChildReplyServiceMessage : public ServiceMessage
{
public:
    EvaluateChildReplyServiceMessage(const db::EvaluateChildReply& evaluateChildReply_);
    const db::EvaluateChildReply& GetEvaluateChildReply() const { return evaluateChildReply; }
private:
    db::EvaluateChildReply evaluateChildReply;
};

class DebugServiceStoppedServiceMessage : public ServiceMessage
{
public:
    DebugServiceStoppedServiceMessage();
};

void InitDebugService();
void DoneDebugService();
void StartDebugService(DebugServiceStartParams& startParams, const std::vector<Breakpoint*>& breakpoints);
void StopDebugService();
void Continue();
void Next();
void Step();
void Finish();
void Until(const common::SourceLoc& sourceLocation);
void SetTargetInputEof();
void PutTargetInputLine(const std::string& targetInputLine);
bool DebugRequestInProgress(std::string& requestName);
void Break(Breakpoint* breakpoint);
void Delete(const std::string& breakpointId);
void Depth();
void Frames(int lowFrame, int highFrame);
void Evaluate(const std::string& expression, int requestId);
void Count(const std::string& expression);
void EvaluateChild(const std::string& expression, int start, int count);

} // namespace cmajor::service
