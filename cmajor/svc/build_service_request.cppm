// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.service.build.service.request;

import cmajor.service.request;
import cmajor.service.build.service;
import cmajor.build.server.message;
import std.core;

export namespace cmajor::service {

class StartBuildRequest : public Request
{
public:
    StartBuildRequest(const BuildServiceStartParams& serviceStartParams_, const bs::BuildRequest& buildRequest_);
    std::string Name() const override { return "startBuildRequest"; }
    void Execute() override;
private:
    BuildServiceStartParams serviceStartParams;
    bs::BuildRequest buildRequest;
};

class StopBuildRequest : public Request
{
public:
    StopBuildRequest();
    std::string Name() const override { return "stopBuildRequest"; }
    void Execute() override;
};

class GotoDefinitionRequest : public Request
{
public:
    GotoDefinitionRequest(const BuildServiceStartParams& serviceStartParams_, const bs::GetDefinitionRequest& getDefinitionRequest_);
    std::string Name() const override { return "gotoDefinitionRequest"; }
    void Execute() override;
private:
    BuildServiceStartParams serviceStartParams;
    bs::GetDefinitionRequest getDefinitionRequest;
};

} // namespace cmajor::service
