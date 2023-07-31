// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.run.service.request;

import cmajor.run.service;

namespace cmajor::service{

StartProgramRequest::StartProgramRequest(const std::string& executableName_, const std::string& programArguments_, const std::string& processName_) :
    executableName(executableName_), programArguments(programArguments_), processName(processName_)
{
}

void StartProgramRequest::Execute()
{
    StartProgram(executableName, programArguments, processName);
}

std::string StartProgramRequest::Name() const
{
    return "startProgramRequest";
}

PutRunServiceProgramInputLineRequest::PutRunServiceProgramInputLineRequest(const std::string& inputLine_) : inputLine(inputLine_)
{
}

void PutRunServiceProgramInputLineRequest::Execute()
{
    PutRunServiceProgramTargetInputLine(inputLine);
}

std::string PutRunServiceProgramInputLineRequest::Name() const
{
    return "putProgramInputLineRequest";
}

SetRunServiceProgramEofRequest::SetRunServiceProgramEofRequest()
{
}

void SetRunServiceProgramEofRequest::Execute()
{
    SetRunServiceProgramTargetInputEof();
}

std::string SetRunServiceProgramEofRequest::Name() const
{
    return "setProgramEofRequest";
}

TerminateProcessRequest::TerminateProcessRequest()
{
}

void TerminateProcessRequest::Execute()
{
    TerminateProcess();
}

std::string TerminateProcessRequest::Name() const
{
    return "terminateProcessRequest";
}

StopRunServiceRequest::StopRunServiceRequest()
{
}

void StopRunServiceRequest::Execute()
{
    StopRunService();
}

std::string StopRunServiceRequest::Name() const
{
    return "stopRunServiceRequest";
}

} // namespace cmajor::service
