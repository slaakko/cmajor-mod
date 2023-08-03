// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.request;

import std.core;

export namespace cmajor::debugger {

enum class RequestKind
{
    run, cont, next, step, exit, breakInsert, depth, frames
};

class Request
{
public:
    Request(RequestKind kind_);
    RequestKind Kind() const { return kind; }
    virtual ~Request();
    virtual std::string ToString() const = 0;
    bool IsExecRequest() const;
    bool IsExitRequest() const { return kind == RequestKind::exit; }
private:
    RequestKind kind;
};

class RunRequest : public Request
{
public:
    RunRequest();
    std::string ToString() const override;
};

class ContinueRequest : public Request
{
public:
    ContinueRequest();
    std::string ToString() const override;
};

class NextRequest : public Request
{
public:
    NextRequest();
    std::string ToString() const override;
};

class StepRequest : public Request
{
public:
    StepRequest();
    std::string ToString() const override;
};

class ExitRequest : public Request
{
public:
    ExitRequest();
    std::string ToString() const override;
};

class BreakInsertRequest : public Request
{
public:
    BreakInsertRequest(const std::string& location_);
    std::string ToString() const override;
private:
    std::string location;
};

class DepthRequest : public Request
{
public:
    DepthRequest();
    std::string ToString() const override;
};

class FramesRequest : public Request
{
public:
    FramesRequest(int lowFrame_, int highFrame_);
    std::string ToString() const override;
private:
    int lowFrame;
    int highFrame;
};

} // namespace cmajor::debugger
