// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.debug;

import cmcode.config;
import util;

namespace cmcode {

cmajor::service::DebugServiceStartParams MakeDebugServiceStartParams(const std::string& backend, const std::string& config, cmajor::ast::Project* project,
    const std::string& programArguments)
{
    cmajor::service::DebugServiceStartParams startParams;
    std::string projectFilePath = project->FilePath();
    std::string executableName = util::ToUtf8(project->Name());
    executableName.append(".exe");
    std::string executableFilePath;
    if (backend == "llvm")
    {
        executableFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(
            util::Path::GetDirectoryName(projectFilePath), "bin"), "llvm"), config), executableName));
    }
    else if (backend == "cpp")
    {
        executableFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(
            util::Path::GetDirectoryName(projectFilePath), "bin"), "cpp"), config), executableName));
    }
    else if (backend == "masm")
    {
        executableFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(
            util::Path::GetDirectoryName(projectFilePath), "bin"), "masm"), config), executableName));
    }
    else if (backend == "sbin")
    {
        executableFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(
            util::Path::GetDirectoryName(projectFilePath), "bin"), "sbin"), config), executableName));
    }
    return startParams.Backend(backend).Config(config).ExecutableFilePath(executableFilePath).ProgramArguments(programArguments);
}

void StartDebugService(const std::string& backend, const std::string& config, cmajor::ast::Project* project, const std::string& programArguments, 
    const std::vector<cmajor::debugger::Breakpoint*>& breakpoints)
{
    cmajor::service::DebugServiceStartParams startParams = MakeDebugServiceStartParams(backend, config, project, programArguments);
    startParams.breakpoints = breakpoints;
    cmajor::service::PutRequest(new cmajor::service::StartDebugServiceRequest(startParams));
}

} // namespace cmcode
