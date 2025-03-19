// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.run; 

import cmajor.service;
import util;

namespace cmcode {

void RunProgram(const std::string& backend, const std::string& config, int optLevel, cmajor::ast::Project* project, const std::string& programArguments)
{
    std::string executableName = util::Path::Combine(project->OutdirBasePath().generic_string(), "bin");
    if (backend == "llvm")
    {
        if (config == "release")
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(
                util::Path::Combine(executableName, "llvm"), config), std::to_string(optLevel)),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
        else
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(executableName, "llvm"), config),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
    }
    else if (backend == "cpp")
    {
        if (config == "release")
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(
                util::Path::Combine(executableName, "cpp"), config), std::to_string(optLevel)),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
        else
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(executableName, "cpp"), config),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
    }
    else if (backend == "masm")
    {
        if (config == "release")
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(
                util::Path::Combine(executableName, "masm"), config), std::to_string(optLevel)),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
        else
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(executableName, "masm"), config),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
    }
    else if (backend == "cm")
    {
        if (config == "release")
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(
                util::Path::Combine(executableName, "cm"), config), std::to_string(optLevel)),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
        else
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(executableName, "cm"), config),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
    }
    else if (backend == "sbin")
    {
        if (config == "release")
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(
                util::Path::Combine(executableName, "sbin"), config), std::to_string(optLevel)),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
        else
        {
            executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(executableName, "sbin"), config),
                util::Path::GetFileName(project->FilePath())), ".exe"));
        }
    }
    cmajor::service::PutRequest(new cmajor::service::StartProgramRequest(executableName, programArguments, "cmcode"));
}

} // namespace cmcode
