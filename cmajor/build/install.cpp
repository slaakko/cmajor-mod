// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.install;

import std.filesystem;
import util;

namespace cmajor::build {

void InstallSystemLibraries(cmajor::symbols::Module* systemInstallModule)
{
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(systemInstallModule->LogStreamId(), "Installing system libraries...");
    }
    cmajor::ast::BackEnd backend = cmajor::ast::BackEnd::llvm;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        backend = cmajor::ast::BackEnd::systemx;
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        backend = cmajor::ast::BackEnd::cpp;
    }
    std::filesystem::path systemLibDir = cmajor::ast::CmajorSystemLibDir(cmajor::symbols::GetConfig(), backend);
    std::filesystem::create_directories(systemLibDir);
    for (cmajor::symbols::Module* systemModule : systemInstallModule->AllReferencedModules())
    {
        std::filesystem::path from = systemModule->OriginalFilePath();
        std::filesystem::path to = systemLibDir / from.filename();
        if (std::filesystem::exists(to))
        {
            std::filesystem::remove(to);
        }
        std::filesystem::copy(from, to);
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(systemInstallModule->LogStreamId(), from.generic_string() + " -> " + to.generic_string());
        }
        if (!systemModule->LibraryFilePath().empty() && !cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::disableCodeGen))
        {
            from = systemModule->LibraryFilePath();
            to = systemLibDir / from.filename();
            if (std::filesystem::exists(to))
            {
                std::filesystem::remove(to);
            }
            std::filesystem::copy(from, to);
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                util::LogMessage(systemInstallModule->LogStreamId(), from.generic_string() + " -> " + to.generic_string());
            }
        }
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(systemInstallModule->LogStreamId(), "System libraries installed.");
    }
}

void InstallSystemWindowsLibraries(cmajor::symbols::Module* systemInstallWindowsModule)
{
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(systemInstallWindowsModule->LogStreamId(), "Installing system libraries (Windows)...");
    }
    cmajor::ast::BackEnd backend = cmajor::ast::BackEnd::llvm;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        backend = cmajor::ast::BackEnd::systemx;
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        backend = cmajor::ast::BackEnd::cpp;
    }
    std::filesystem::path systemLibDir = cmajor::ast::CmajorSystemLibDir(cmajor::symbols::GetConfig(), backend);
    std::filesystem::create_directories(systemLibDir);
    for (cmajor::symbols::Module* systemModule : systemInstallWindowsModule->AllReferencedModules())
    {
        std::filesystem::path from = systemModule->OriginalFilePath();
        std::filesystem::path to = systemLibDir / from.filename();
        if (std::filesystem::exists(to))
        {
            std::filesystem::remove(to);
        }
        std::filesystem::copy(from, to);
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(systemInstallWindowsModule->LogStreamId(), from.generic_string() + " -> " + to.generic_string());
        }
        if (!systemModule->LibraryFilePath().empty() && !cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::disableCodeGen))
        {
            from = systemModule->LibraryFilePath();
            to = systemLibDir / from.filename();
            if (std::filesystem::exists(to))
            {
                std::filesystem::remove(to);
            }
            std::filesystem::copy(from, to);
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                util::LogMessage(systemInstallWindowsModule->LogStreamId(), from.generic_string() + " -> " + to.generic_string());
            }
        }
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(systemInstallWindowsModule->LogStreamId(), "System libraries installed.");
    }
}

} // namespace cmajor::build
