// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.build;

import cmcode.config;
import cmcode.configuration;
import cmajor.service;

namespace cmcode {

void StartBuild(const std::string& backend, const std::string& config, const std::string& filePath, BuildRequestKind requestKind)
{
    cmajor::service::BuildServiceStartParams serviceStartParams = cmajor::service::BuildServiceStartParams().
        ProcessName("cmcode").DebugServer(UseDebugServers()).Log(ServerLogging()).Wait(DebugWait());
    bs::BuildRequest buildRequest;
    const BuildSettings& buildSettings = GetBuildSettings();
    buildRequest.backend = backend;
    buildRequest.config = config;
    buildRequest.optimizationLevel = "default";
    buildRequest.filePath = filePath;
    buildRequest.singleThreadedCompile = buildSettings.singleThreadedCompile;
    buildRequest.emitIR = buildSettings.generateIntermediateCodeFiles;
    buildRequest.verbose = true;
    if ((requestKind & BuildRequestKind::clean) != BuildRequestKind::none)
    {
        buildRequest.clean = true;
    }
    if ((requestKind & BuildRequestKind::rebuild) != BuildRequestKind::none)
    {
        buildRequest.rebuild = true;
    }
    if ((requestKind & BuildRequestKind::buildDependencies) != BuildRequestKind::none)
    {
        buildRequest.buildAllDependencies = true;
    }
    std::unique_ptr<cmajor::service::StartBuildRequest> startBuildRequest(new cmajor::service::StartBuildRequest(serviceStartParams, buildRequest));
    cmajor::service::PutRequest(startBuildRequest.release());
}

void StopBuild()
{
    std::unique_ptr<cmajor::service::StopBuildRequest> stopBuildRequest(new cmajor::service::StopBuildRequest());
    PutRequest(stopBuildRequest.release());
}

void StartGetDefinitionRequest(const bs::GetDefinitionRequest& getDefinitionRequest)
{
    cmajor::service::BuildServiceStartParams serviceStartParams = cmajor::service::BuildServiceStartParams().
        ProcessName("cmcode").DebugServer(UseDebugServers()).Log(ServerLogging()).Wait(DebugWait());
    PutRequest(new cmajor::service::GotoDefinitionRequest(serviceStartParams, getDefinitionRequest));
}

} // namespace cmcode
