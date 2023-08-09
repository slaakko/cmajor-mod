// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.service.request;

import cmajor.service.request;
import cmajor.info.bs;

export namespace cmajor::service {

class StartBuildServiceRequest : public Request
{
public:
    StartBuildServiceRequest(cmajor::info::bs::BuildCommand* buildCommand_);
    void Execute() override;
    std::string Name() const override { return "startBuildServiceRequest"; }
private:
    std::unique_ptr<cmajor::info::bs::BuildCommand> buildCommand;
};

class StopBuildServiceRequest : public Request
{
public:
    StopBuildServiceRequest();
    void Execute() override;
    std::string Name() const override { return "stopBuildServiceRequest"; }
};

class GetDefinitionRequest : public Request
{
public:
    GetDefinitionRequest(const cmajor::info::bs::GetDefinitionRequest& request_);
    void Execute() override;
    std::string Name() const override { return "getDefinitionRequest"; }
private:
    cmajor::info::bs::GetDefinitionRequest request;
};

} // namespace cmajor::service
