// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.build;

import cmcode.config;
import cmcode.configuration;
import cmajor.service;
import cmajor.info;

namespace cmcode {

void StartBuild(const std::string& backend, const std::string& config, int optLevel, 
    const std::string& filePath, const std::string& disabledWarnings, BuildRequestKind requestKind)
{
    std::unique_ptr<cmajor::info::bs::BuildCommand> buildCommand(new cmajor::info::bs::BuildCommand());
    const BuildSettings& buildSettings = GetBuildSettings();
    buildCommand->backend = backend;
    buildCommand->config = config;
    buildCommand->optimizationLevel = std::to_string(optLevel);
    buildCommand->disabledWarnings = disabledWarnings;
    buildCommand->filePath = filePath;
    buildCommand->singleThreadedCompile = buildSettings.singleThreadedCompile;
    buildCommand->emitIR = buildSettings.generateIntermediateCodeFiles;
    buildCommand->verbose = true;
    if ((requestKind & BuildRequestKind::clean) != BuildRequestKind::none)
    {
        buildCommand->clean = true;
    }
    if ((requestKind & BuildRequestKind::rebuild) != BuildRequestKind::none)
    {
        buildCommand->rebuild = true;
    }
    if ((requestKind & BuildRequestKind::buildDependencies) != BuildRequestKind::none)
    {
        buildCommand->buildAllDependencies = true;
    }
    std::unique_ptr<cmajor::service::StartBuildServiceRequest> startBuildRequest(new cmajor::service::StartBuildServiceRequest(buildCommand.release()));
    cmajor::service::PutRequest(startBuildRequest.release());
}

void StopBuild()
{
    std::unique_ptr<cmajor::service::StopBuildServiceRequest> stopBuildRequest(new cmajor::service::StopBuildServiceRequest());
    PutRequest(stopBuildRequest.release());
}

} // namespace cmcode
