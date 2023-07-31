// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.run.service.request;

import cmajor.service.request;
import std.core;

export namespace cmajor::service {

class StartProgramRequest : public Request
{
public:
    StartProgramRequest(const std::string& executableName_, const std::string& programArguments_, const std::string& processName_);
    void Execute() override;
    std::string Name() const override;
private:
    std::string executableName;
    std::string programArguments;
    std::string processName;
};

class PutRunServiceProgramInputLineRequest : public Request
{
public:
    PutRunServiceProgramInputLineRequest(const std::string& inputLine_);
    void Execute() override;
    std::string Name() const override;
private:
    std::string inputLine;
};

class SetRunServiceProgramEofRequest : public Request
{
public:
    SetRunServiceProgramEofRequest();
    void Execute() override;
    std::string Name() const override;
};

class TerminateProcessRequest : public Request
{
public:
    TerminateProcessRequest();
    void Execute() override;
    std::string Name() const override;
};

class StopRunServiceRequest : public Request
{
public:
    StopRunServiceRequest();
    void Execute() override;
    std::string Name() const override;
};

} // namespace cmajor::service
