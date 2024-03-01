// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.service.config;

import util;

namespace cmajor::service {

std::string CmajorRootDir()
{
    char* e = getenv("CMAJOR_ROOT");
    if (e == nullptr || !*e)
    {
        throw std::runtime_error("please set 'CMAJOR_ROOT' environment variable to contain /path/to/cmajor directory.");
    }
    return util::GetFullPath(std::string(e));
}

std::string CmajorConfigDir()
{
    std::string configDir = util::GetFullPath(util::Path::Combine(CmajorRootDir(), "config"));
    std::filesystem::create_directories(configDir);
    return configDir;
}

std::string BinaryPortMapConfigFilePath()
{
    return util::GetFullPath(util::Path::Combine(CmajorConfigDir(), "bpm.config.xml"));
}

std::string CmajorLogDir()
{
    std::string logDir = util::Path::Combine(CmajorRootDir(), "log");
    std::filesystem::create_directories(logDir);
    return logDir;
}

std::string CmajorProjectsDir()
{
    std::string projectsDir = util::Path::Combine(CmajorRootDir(), "projects");
    std::filesystem::create_directories(projectsDir);
    return projectsDir;
}

std::string BinaryPortMapLogFilePath()
{
    return util::GetFullPath(util::Path::Combine(CmajorLogDir(), "cmbpms.log"));
}

} // namespace cmajor::service
