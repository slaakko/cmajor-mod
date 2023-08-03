// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.service;

import cmajor.info.bs;
import cmajor.service.message;
import std.core;

export namespace cmajor::service {

class BuildResultMessage : public ServiceMessage
{
public:
    BuildResultMessage(const cmajor::info::bs::BuildResult& result_);
    const cmajor::info::bs::BuildResult& Result() const { return result; }
private:
    cmajor::info::bs::BuildResult result;
};

class BuildStoppedMessage : public ServiceMessage
{
public:
    BuildStoppedMessage();
};

void StartBuildService();
void StopBuildService();
void ExecuteBuildCommand(cmajor::info::bs::BuildCommand* command);
bool BuildInProgress();
void CancelBuild();

} // cmajor::service
