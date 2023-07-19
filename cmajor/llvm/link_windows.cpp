// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#ifdef HAS_LLD
// todo
#endif

module cmajor.llvm.link.windows;

import util;
import std.filesystem;

namespace cmajor::llvm {

void LinkWindowsLLDLink(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
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
#ifdef HAS_LLD
    // todo
#else
    LinkWindowsLLDLink(project, rootModule);
#endif
}

} // cmajor::llvm
