// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.service;

import cmajor.cm_masm_build.service;
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

class GetDefinitionReplyServiceMessage : public ServiceMessage
{
public:
    GetDefinitionReplyServiceMessage(const cmajor::info::bs::GetDefinitionReply& reply_);
    const cmajor::info::bs::GetDefinitionReply& Reply() const { return reply; }
private:
    cmajor::info::bs::GetDefinitionReply reply;
};

void StartBuildService();
void StopBuildService();
void ExecuteBuildCommand(cmajor::info::bs::BuildCommand* command);
cmajor::info::bs::GetDefinitionReply GetDefinition(const cmajor::info::bs::GetDefinitionRequest& request);
bool BuildInProgress();
void CancelBuild();

} // cmajor::service
