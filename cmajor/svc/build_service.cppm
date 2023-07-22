// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.service;

import cmajor.command;
import cmajor.service.message;
import std.core;

export namespace cmajor::service {

class BuildResultMessage : public ServiceMessage
{
public:
    BuildResultMessage(const cmajor::command::BuildResult& result_);
    const cmajor::command::BuildResult& Result() const { return result; }
private:
    cmajor::command::BuildResult result;
};

class BuildStoppedMessage : public ServiceMessage
{
public:
    BuildStoppedMessage();
};

void StartBuildService();
void StopBuildService();
void ExecuteBuildCommand(cmajor::command::BuildCommand* command);
bool BuildInProgress();
void CancelBuild();

} // cmajor::service
