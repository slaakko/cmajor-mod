// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmbs.logging;

import util;


namespace cmbs {

std::string CmajorRootDir()
{
    char* e = getenv("CMAJOR_ROOT");
    if (e == nullptr || !*e)
    {
        throw std::runtime_error("please set 'CMAJOR_ROOT' environment variable to contain /path/to/cmajor directory.");
    }
    return std::string(e);
}

std::string CmajorLogDir()
{
    std::string logDir = util::Path::Combine(CmajorRootDir(), "log");
    std::filesystem::create_directories(logDir);
    return util::GetFullPath(logDir);
}

std::string LogFilePath()
{
    return util::Path::Combine(CmajorLogDir(), "cmbs.log");
}

} // namespace cmbs
