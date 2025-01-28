// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.run.service;

import cmajor.service.message;
import std.core;

export namespace cmajor::service {

class ProcessTerminatedServiceMessage : public ServiceMessage
{
public:
    ProcessTerminatedServiceMessage();
};

class RunServiceStoppedServiceMessage : public ServiceMessage
{
public:
    RunServiceStoppedServiceMessage();
};

void InitRunService();
void DoneRunService();
void StartProgram(const std::string& executableFilePath, const std::string& programArguments, const std::string& processName);
void TerminateProcess();
void StopRunService();
void SetRunServiceProgramTargetInputEof();
void PutRunServiceProgramTargetInputLine(const std::string& targetInputLine);

} // namespace cmajor::service
