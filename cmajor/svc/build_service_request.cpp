// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.service.request;

import cmajor.build.service;

namespace cmajor::service {

StartBuildServiceRequest::StartBuildServiceRequest(cmajor::info::bs::BuildCommand* buildCommand_) : buildCommand(buildCommand_)
{
}

void StartBuildServiceRequest::Execute()
{
    ExecuteBuildCommand(buildCommand.release());
}

StopBuildServiceRequest::StopBuildServiceRequest()
{
}

void StopBuildServiceRequest::Execute()
{
    CancelBuild();
}

} // namespace cmajor::service
