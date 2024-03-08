// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#ifdef _WIN32
#include <Windows.h>
#endif

module cmajor.masm.build.msbuild;

import cmajor.masm.build.config;
import util;
import std.filesystem;

namespace cmajor::masm::build {

void MSBuild(const std::string& vsProjectFilePath, const std::string& config, int logStreamId)
{
    std::string vcvars64Path = GetVCVars64Path();
    if (vcvars64Path.empty())
    {
        throw std::runtime_error("vcvars64.bat not configured");
    }
    std::string Config = "Debug";
    if (config == "release")
    {
        Config = "Release";
    }
    std::string projectDir = util::Path::GetDirectoryName(vsProjectFilePath);
    std::string buildBatPath = util::GetFullPath(util::Path::Combine(projectDir, "build.bat"));
    {
        std::ofstream buildBatFile(buildBatPath);
        util::CodeFormatter formatter(buildBatFile);
        formatter.WriteLine("@echo off");
        formatter.WriteLine("call \"" + vcvars64Path + "\"");
        formatter.WriteLine("msbuild -verbosity:minimal -t:Rebuild -p:Configuration=\"" + Config + "\" " + "\"" + vsProjectFilePath + "\"" + " > \"" + projectDir +
            "/build.out.log\" 2> \"" + projectDir + "/build.error.log\"");
        formatter.WriteLine("exit %ERRORLEVEL%");
    }
    std::string commandLine = "cmd /C \"" + buildBatPath + "\"";
    STARTUPINFOA startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    PROCESS_INFORMATION processInfo;
    ZeroMemory(&processInfo, sizeof(processInfo));
    bool succeeded = CreateProcessA(NULL, (LPSTR)commandLine.c_str(), NULL, NULL, false, 0, NULL, NULL, &startupInfo, &processInfo);
    if (!succeeded)
    {
        throw std::runtime_error("could not run build.bat");
    }
    WaitForSingleObject(processInfo.hProcess, INFINITE);
    DWORD buildExitCode = 0;
    GetExitCodeProcess(processInfo.hProcess, &buildExitCode);
    CloseHandle(processInfo.hProcess);
    CloseHandle(processInfo.hThread);
    if (std::filesystem::exists(projectDir + "/build.out.log"))
    {
        std::string buildOutLog = util::ReadFile(projectDir + "/build.out.log");
        std::vector<std::string> lines = util::SplitTextIntoLines(buildOutLog);
        for (const auto& line : lines)
        {
            util::LogMessage(logStreamId, util::PlatformStringToUtf8(line));
        }
    }
    if (std::filesystem::exists(projectDir + "/build.error.log"))
    {
        std::string buildErrorLog = util::ReadFile(projectDir + "/build.error.log");
        std::vector<std::string> lines = util::SplitTextIntoLines(buildErrorLog);
        for (const auto& line : lines)
        {
            util::LogMessage(logStreamId, util::PlatformStringToUtf8(line));
        }
    }
    if (buildExitCode != 0)
    {
        throw std::runtime_error("msbuild failed with exit code " + std::to_string(buildExitCode));
    }
}

} // namespace otava::build
