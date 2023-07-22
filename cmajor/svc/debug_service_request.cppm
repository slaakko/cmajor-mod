// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.service.request;

import cmajor.service.request;
import cmajor.service.breakpoint;
import cmajor.debug.service;
import cmajor.common.message;
import std.core;

export namespace cmajor::service {

class StartDebugServiceRequest : public Request
{
public:
    StartDebugServiceRequest(const DebugServiceStartParams& startParams_, const std::vector<Breakpoint*>& breakpoints_);
    void Execute() override;
    std::string Name() const override { return "startDebugServiceRequest"; }
private:
    DebugServiceStartParams startParams;
    std::vector<Breakpoint*> breakpoints;
};

class StopDebugServiceRequest : public Request
{
public:
    StopDebugServiceRequest();
    void Execute() override;
    std::string Name() const override { return "stopDebugServiceRequest"; }
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
    UntilDebugServiceRequest(const common::SourceLoc& sourceLocation_);
    void Execute() override;
    std::string Name() const override { return "untilDebugServiceRequest"; }
private:
    common::SourceLoc sourceLocation;
};

class BreakDebugServiceRequest : public Request
{
public:
    BreakDebugServiceRequest(Breakpoint* breakpoint_);
    void Execute() override;
    std::string Name() const override { return "breakDebugServiceRequest"; }
private:
    Breakpoint* breakpoint;
};

class DeleteDebugServiceRequest : public Request
{
public:
    DeleteDebugServiceRequest(const std::string& breakpointId_);
    void Execute() override;
    std::string Name() const override { return "deleteDebugServiceRequest"; }
private:
    std::string breakpointId;
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

class SetTargetInputEofRequest : public Request
{
public:
    SetTargetInputEofRequest();
    void Execute() override;
    std::string Name() const override { return "setTargetInputEofRequest"; }
};

class PutTargetInputLineRequest : public Request
{
public:
    PutTargetInputLineRequest(const std::string& targetInputLine_);
    void Execute() override;
    std::string Name() const override { return "putTargetInputLineRequest"; }
private:
    std::string targetInputLine;
};

} // namespace cmajor::service
