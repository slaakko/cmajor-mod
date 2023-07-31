// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <llvm/Config/llvm-config.h>
#if LLVM_VERSION_MAJOR >= 16
#define HAS_LLD_LIBRARY 1
#endif
#if HAS_LLD_LIBRARY
#include <lld/Common/Driver.h>
#include <llvm/Support/raw_os_ostream.h>
#endif

module cmajor.llvm.link.windows;

import util;
import std.filesystem;

namespace cmajor::llvmlink {

#if HAS_LLD_LIBRARY

void LinkWindowsLLDLinkLibrary(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(rootModule->LogStreamId(), "Linking...");
    }
    rootModule->SetCurrentToolName(U"lld-link");
    std::filesystem::path bdp = project->ExecutableFilePath();
    bdp.remove_filename();
    std::filesystem::create_directories(bdp);
    std::vector<std::string> argStrings;
    argStrings.push_back("/machine:x64");
    argStrings.push_back("/entry:main");
    argStrings.push_back("/debug");
    argStrings.push_back("/out:" + util::QuotedPath(project->ExecutableFilePath()));
    argStrings.push_back("/stack:16777216");
    std::string rtLibName = "cmajor.rt.lib";
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime))
    {
        util::LogMessage(rootModule->LogStreamId(), "Note: linking with debug runtime (cmajor.rtd.dll).");
        rtLibName = "cmajor.rtd.lib";
    }
    argStrings.push_back(util::QuotedPath(util::GetFullPath(util::Path::Combine(util::Path::Combine(util::CmajorRoot(), "lib"), rtLibName))));
    int n = rootModule->LibraryFilePaths().size();
    for (int i = 0; i < n; ++i)
    {
        argStrings.push_back(util::QuotedPath(rootModule->LibraryFilePaths()[i]));
    }
    if (!rootModule->ResourceFilePath().empty())
    {
        argStrings.push_back(util::QuotedPath(rootModule->ResourceFilePath()));
    }
    std::vector<const char*> args;
    for (const auto& str : argStrings)
    {
        args.push_back(str.c_str());
    }
    std::error_code outErrorCode;
    std::string stdOutFilePath = util::Path::ChangeExtension(rootModule->LibraryFilePath(), ".out");
    llvm::raw_fd_ostream  stdOut(stdOutFilePath, outErrorCode);
    if (outErrorCode)
    {
        throw std::runtime_error("error linking: could not create output file: " + util::PlatformStringToUtf8(outErrorCode.message()));
    }
    std::error_code errErrorCode;
    std::string stdErrFilePath = util::Path::ChangeExtension(rootModule->LibraryFilePath(), ".err");
    llvm::raw_fd_ostream  stdErr(stdErrFilePath, errErrorCode);
    if (errErrorCode)
    {
        throw std::runtime_error("error linking: could not create output file: " + util::PlatformStringToUtf8(errErrorCode.message()));
    }
    if (lld::coff::link(args, stdOut, stdErr, true, false))
    {
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            stdOut.close();
            std::string output = util::ReadFile(stdOutFilePath);
            if (!output.empty())
            {
                util::LogMessage(rootModule->LogStreamId(), output);
            }
            util::LogMessage(rootModule->LogStreamId(), "==> " + project->ExecutableFilePath());
        }
    }
    else
    {
        stdErr.close();
        std::string errors = util::ReadFile(stdErrFilePath);
        throw std::runtime_error("error linking: " + errors);
    }
}

#endif

void LinkWindowsLLDLinkProcess(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(rootModule->LogStreamId(), "Linking...");
    }
    rootModule->SetCurrentToolName(U"lld-link");
    std::filesystem::path bdp = project->ExecutableFilePath();
    bdp.remove_filename();
    std::filesystem::create_directories(bdp);
    std::vector<std::string> args;
    args.push_back("/machine:x64");
    args.push_back("/entry:main");
    args.push_back("/debug");
    args.push_back("/out:" + util::QuotedPath(project->ExecutableFilePath()));
    args.push_back("/stack:16777216");
    std::string rtLibName = "cmajor.rt.lib";
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime))
    {
        rtLibName = "cmajor.rtd.lib";
    }
    args.push_back(util::QuotedPath(util::GetFullPath(util::Path::Combine(util::Path::Combine(util::CmajorRoot(), "lib"), rtLibName))));
    int n = rootModule->LibraryFilePaths().size();
    for (int i = 0; i < n; ++i)
    {
        args.push_back(util::QuotedPath(rootModule->LibraryFilePaths()[i]));
    }
    if (!rootModule->ResourceFilePath().empty())
    {
        args.push_back(util::QuotedPath(rootModule->ResourceFilePath()));
    }
    std::string errors;
    std::string linkCommandLine = "lld-link";
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::generateDebugInfo))
    {
        linkCommandLine.append(" -debug");
    }
    for (const std::string& arg : args)
    {
        linkCommandLine.append(1, ' ').append(arg);
    }
    util::Process::Redirections redirections = util::Process::Redirections::processStdErr;
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        redirections = redirections | util::Process::Redirections::processStdOut;
    }
    try
    {
        util::Process process(linkCommandLine, redirections);
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            while (!process.Eof(util::Process::StdHandle::stdOut))
            {
                std::string line = process.ReadLine(util::Process::StdHandle::stdOut);
                if (!line.empty())
                {
                    util::LogMessage(-1, util::PlatformStringToUtf8(line));
                }
            }
        }
        errors = process.ReadToEnd(util::Process::StdHandle::stdErr);
        process.WaitForExit();
        int exitCode = process.ExitCode();
        if (exitCode != 0)
        {
            throw std::runtime_error("executing '" + linkCommandLine + "' failed with exit code: " + std::to_string(exitCode));
        }
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("linking executable '" + project->ExecutableFilePath() + "' failed: " + ex.what() + ":\nerrors:\n" + errors);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(rootModule->LogStreamId(), "==> " + project->ExecutableFilePath());
    }
}

void LinkWindows(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
#ifdef HAS_LLD_LIBRARY
    LinkWindowsLLDLinkLibrary(project, rootModule);
#else
    LinkWindowsLLDLinkProcess(project, rootModule);
#endif
}

} // cmajor::llvmlink
