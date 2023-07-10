// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.service.build.service;

import cmajor.service.message;
import cmajor.build.server.message;
import std.core;

export namespace cmajor::service {

struct BuildServiceStartParams
{
    BuildServiceStartParams();
    BuildServiceStartParams& ProcessName(const std::string& processName_);
    BuildServiceStartParams& DebugServer(bool debugServer_);
    BuildServiceStartParams& Log(bool log_);
    BuildServiceStartParams& Wait(bool wait_);
    std::string processName;
    bool debugServer;
    bool log;
    bool wait;
};

class BuildServiceRequest
{
public:
    virtual ~BuildServiceRequest();
    virtual void Execute() = 0;
    virtual std::string Name() const = 0;
    virtual void Failed(const std::string& error) = 0;
};

class RunBuildRequest : public BuildServiceRequest
{
public:
    RunBuildRequest(const bs::BuildRequest& buildRequest_);
    std::string Name() const override { return "runBuildRequest"; }
    void Execute() override;
    void Failed(const std::string& error) override;
private:
    bs::BuildRequest buildRequest;
};

class BuildReplyServiceMessage : public ServiceMessage
{
public:
    BuildReplyServiceMessage(const bs::BuildReply& buildReply_);
    bs::BuildReply& GetBuildReply() { return buildReply; }
private:
    bs::BuildReply buildReply;
};

class BuildErrorServiceMessage : public ServiceMessage
{
public:
    BuildErrorServiceMessage(const std::string& error_);
    const std::string& Error() const { return error; }
private:
    std::string error;
};

class RunGetDefinitionRequest : public BuildServiceRequest
{
public:
    RunGetDefinitionRequest(const bs::GetDefinitionRequest& getDefinitionRequest_);
    std::string Name() const override { return "runGetDefinitionRequest"; }
    void Execute() override;
    void Failed(const std::string& error) override;
private:
    bs::GetDefinitionRequest getDefinitionRequest;
};

class GetDefinitionReplyServiceMessage : public ServiceMessage
{
public:
    GetDefinitionReplyServiceMessage(const bs::GetDefinitionReply& getDefinitionReply_);
    bs::GetDefinitionReply& GetGetDefinitionReply() { return getDefinitionReply; }
private:
    bs::GetDefinitionReply getDefinitionReply;
};

class GetDefinitionErrorServiceMessage : public ServiceMessage
{
public:
    GetDefinitionErrorServiceMessage(const std::string& error_);
    const std::string& Error() const { return error; }
private:
    std::string error;
};

class StopBuildServiceMessage : public ServiceMessage
{
public:
    StopBuildServiceMessage();
};

void StartBuildService(BuildServiceStartParams& startParams);
void EnqueueBuildServiceRequest(BuildServiceRequest* request);
void StopBuildService(bool log);
bool BuildServiceRunning();

} // namespace cmajor::service
