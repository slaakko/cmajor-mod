// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.service.build.service.request;

namespace cmajor::service {

StartBuildRequest::StartBuildRequest(const BuildServiceStartParams& serviceStartParams_, const bs::BuildRequest& buildRequest_) : 
    serviceStartParams(serviceStartParams_), buildRequest(buildRequest_)
{
}

void StartBuildRequest::Execute()
{
    if (!BuildServiceRunning())
    {
        StartBuildService(serviceStartParams);
    }
    EnqueueBuildServiceRequest(new RunBuildRequest(buildRequest));
}

StopBuildRequest::StopBuildRequest()
{
}

void StopBuildRequest::Execute()
{
    StopBuildService(true);
    PutServiceMessage(new StopBuildServiceMessage());
}

GotoDefinitionRequest::GotoDefinitionRequest(const BuildServiceStartParams& serviceStartParams_, const bs::GetDefinitionRequest& getDefinitionRequest_) :
    serviceStartParams(serviceStartParams_), getDefinitionRequest(getDefinitionRequest_)
{
}

void GotoDefinitionRequest::Execute()
{
    if (!BuildServiceRunning())
    {
        StartBuildService(serviceStartParams);
    }
    EnqueueBuildServiceRequest(new RunGetDefinitionRequest(getDefinitionRequest));
}

} // namespace cmajor::service
