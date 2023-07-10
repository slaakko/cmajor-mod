// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.debug;

import cmcode.config;
import util;

namespace cmcode {

cmajor::service::DebugServiceStartParams MakeDebugServiceStartParams(int pid, const std::string& backend, const std::string& config, cmajor::ast::Project* project, const std::string& programArguments)
{
    cmajor::service::DebugServiceStartParams startParams;
    return startParams.ProcessName("cmcode").Pid(pid).Backend(backend).Config(config).ProjectFilePath(project->FilePath()).ExecutableName(util::ToUtf8(project->Name())).
        ProgramArguments(programArguments).DebugServer(UseDebugServers()).Log(ServerLogging()).Wait(DebugWait()).Verbose(ServerVerbose());
}

void StartDebugService(int pid, const std::string& backend, const std::string& config, cmajor::ast::Project* project, const std::string& programArguments, 
    const std::vector<cmajor::service::Breakpoint*>& breakpoints)
{
    cmajor::service::DebugServiceStartParams startParams = MakeDebugServiceStartParams(pid, backend, config, project, programArguments);
    cmajor::service::PutRequest(new cmajor::service::StartDebugServiceRequest(startParams, breakpoints));
}

} // namespace cmcode
