// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.build.install;

import cmajor.symbols;
import std.filesystem;
import util;

namespace cmajor::masm::build {

void Install(cmajor::ast::Project* project)
{
    std::string libExePath = util::Path::ChangeExtension(project->ModuleFilePath(), ".exe");
    if (std::filesystem::exists(libExePath))
    {
        std::string binExePath = project->ExecutableFilePath();
        std::string dirPath = util::Path::GetDirectoryName(binExePath);
        std::filesystem::create_directories(dirPath);
        std::filesystem::path from = libExePath;
        std::filesystem::path to = binExePath;
        if (std::filesystem::exists(to))
        {
            std::filesystem::remove(to);
        }
        std::filesystem::copy(from, to);
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(project->LogStreamId(), from.generic_string() + " -> " + to.generic_string());
        }
    }
    else
    {
        throw std::runtime_error(libExePath + " does not exist");
    }
}

} // namespace cmajor::masm::build
