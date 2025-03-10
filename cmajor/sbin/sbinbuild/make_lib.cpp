// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.sbin.build.make_lib;

import cmajor.masm.build.config;
import util;
import std.filesystem;

namespace cmajor::sbin::build {

std::string MakeLib(cmajor::ast::Project* project, cmajor::symbols::Module* module, const std::vector<std::string>& objectFilePaths, bool program, bool verbose)
{
    std::string libraryFilePath = module->LibraryFilePath();
    if (program)
    {
        libraryFilePath = util::Path::Combine(util::Path::GetDirectoryName(module->LibraryFilePath()), "lib_" + util::Path::GetFileName(module->LibraryFilePath()));
    }
    std::string libraryDir = util::Path::GetDirectoryName(libraryFilePath);
    std::string vcvars64Path = cmajor::masm::build::GetVCVars64Path();
    if (vcvars64Path.empty())
    {
        throw std::runtime_error("vcvars64.bat not configured");
    }
    std::string makeLibBatPath = util::GetFullPath(util::Path::Combine(libraryDir, "makelib.bat"));
    {
        std::ofstream makelibBatFile(makeLibBatPath);
        util::CodeFormatter formatter(makelibBatFile);
        formatter.WriteLine("@echo off");
        formatter.WriteLine("call \"" + vcvars64Path + "\"");
        std::string objects;
        for (const auto& objectFilePath : objectFilePaths)
        {
            objects.append(" \"").append(objectFilePath).append("\"");
        }
        formatter.WriteLine("lib /MACHINE:X64 /VERBOSE /OUT:\"" + libraryFilePath + "\" " + objects + 
            " > \"" + libraryDir + "/makelib.out.log\" 2> \"" + libraryDir + "/makelib.error.log\"");
        formatter.WriteLine("exit %ERRORLEVEL%");
    }
    std::string commandLine = "cmd /C \"" + makeLibBatPath + "\"";
    std::pair<int, std::string> cmdResult = util::ExecuteWin(commandLine);
    if (std::filesystem::exists(libraryDir + "/makelib.out.log"))
    {
        std::string makelibOutLog = util::ReadFile(libraryDir + "/makelib.out.log");
        std::vector<std::string> lines = util::SplitTextIntoLines(makelibOutLog);
        for (const auto& line : lines)
        {
            util::LogMessage(project->LogStreamId(), util::PlatformStringToUtf8(line));
        }
    }
    if (std::filesystem::exists(libraryDir + "/makelib.error.log"))
    {
        std::string makelibErrorLog = util::ReadFile(libraryDir + "/makelib.error.log");
        std::vector<std::string> lines = util::SplitTextIntoLines(makelibErrorLog);
        for (const auto& line : lines)
        {
            util::LogMessage(project->LogStreamId(), util::PlatformStringToUtf8(line));
        }
    }
    if (cmdResult.first != 0)
    {
        throw std::runtime_error(cmdResult.second);
    }
    if (verbose)
    {
        util::LogMessage(project->LogStreamId(), "==> " + libraryFilePath);
    }
    return libraryFilePath;
}

} // namespace cmajor::sbin::build
