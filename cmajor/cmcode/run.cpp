// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.run; 

import cmajor.service;
import util;

namespace cmcode {

void RunProgram(const std::string& backend, const std::string& config, cmajor::ast::Project* project, const std::string& programArguments)
{
    std::string executableName = util::Path::Combine(project->OutdirBasePath().generic_string(), "bin");
    if (backend == "llvm")
    {
        executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(executableName, config), 
            util::Path::GetFileName(project->FilePath())), ".exe"));
    }
    else if (backend == "cpp")
    {
        executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(executableName, "cpp"), config),
            util::Path::GetFileName(project->FilePath())), ".exe"));
    }
    else if (backend == "masm")
    {
        executableName = util::Path::Combine(project->OutdirBasePath().generic_string(), "lib");
        executableName = util::GetFullPath(util::Path::ChangeExtension(util::Path::Combine(util::Path::Combine(util::Path::Combine(executableName, "masm"), config),
            util::Path::GetFileName(project->FilePath())), ".exe"));
    }
    cmajor::service::PutRequest(new cmajor::service::StartProgramRequest(executableName, programArguments, "cmcode"));
}

} // namespace cmcode
