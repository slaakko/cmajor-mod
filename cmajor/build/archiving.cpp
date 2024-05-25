// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.archiving;

import cmajor.build.config;
import cmajor.symbols;
import cmajor.llvm;
import cmajor.backend.systemx;
import cmajor.systemx.object;
import util;
import std.filesystem;

namespace cmajor::build {

void ArchiveCpp(cmajor::ast::Project* project, const std::vector<std::string>& objectFilePaths, bool verbose)
{
    if (verbose)
    {
        util::LogMessage(project->LogStreamId(), "Generating archive file...");
    }
    if (std::filesystem::exists(project->LibraryFilePath()))
    {
        std::filesystem::remove(project->LibraryFilePath());
    }
    std::string arPath = GetARPathFromBuildConfig();
    std::string archiveCommand = arPath + " rv " + util::QuotedPath(project->LibraryFilePath());
    for (const auto& objectFilePath : objectFilePaths)
    {
        archiveCommand.append(" ").append(util::QuotedPath(objectFilePath));
    }
    util::ExecuteResult executeResult = util::Execute(archiveCommand);
    if (executeResult.exitCode != 0)
    {
        throw std::runtime_error("archiving failed with error code " + std::to_string(executeResult.exitCode) + ": " + std::move(executeResult.output));
    }
    else
    {
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(project->LogStreamId(), executeResult.output);
        }
    }
/*
    std::string errors;
    try
    {
        util::Process::Redirections redirections = util::Process::Redirections::processStdErr;
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            redirections = redirections | util::Process::Redirections::processStdOut;
        }
        util::Process process(archiveCommand, redirections);
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            while (!process.Eof(util::Process::StdHandle::stdOut))
            {
                std::string line = process.ReadLine(util::Process::StdHandle::stdOut);
                if (!line.empty())
                {
                    util::LogMessage(project->LogStreamId(), util::PlatformStringToUtf8(line));
                }
            }
        }
        errors = process.ReadToEnd(util::Process::StdHandle::stdErr);
        process.WaitForExit();
        int exitCode = process.ExitCode();
        if (exitCode != 0)
        {
            throw std::runtime_error("executing '" + archiveCommand + "' failed with exit code: " + std::to_string(exitCode));
        }
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("generating archive '" + project->LibraryFilePath() + "' failed: " + ex.what() + ":\nerrors:\n" + errors);
    }
*/
    if (verbose)
    {
        util::LogMessage(project->LogStreamId(), "==> " + project->LibraryFilePath());
    }
}

void Archive(cmajor::ast::Project* project, const std::vector<std::string>& objectFilePaths)
{
    bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
    switch (cmajor::symbols::GetBackEnd())
    {
        case cmajor::symbols::BackEnd::llvm:
        {
            if (verbose)
            {
                util::LogMessage(project->LogStreamId(), "Generating library file...");
            }
            if (std::filesystem::exists(project->LibraryFilePath()))
            {
                std::filesystem::remove(project->LibraryFilePath());
            }
            cmajor::llvm::Archive(project->LibraryFilePath(), objectFilePaths);
            if (verbose)
            {
                util::LogMessage(project->LogStreamId(), "==> " + project->LibraryFilePath());
            }
            break;
        }
        case cmajor::symbols::BackEnd::systemx:
        {
            cmajor::systemx::object::CreateArchive(0, project->LibraryFilePath(), objectFilePaths, verbose);
            break;
        }
        case cmajor::symbols::BackEnd::cpp:
        {
            ArchiveCpp(project, objectFilePaths, verbose);
            break;
        }
    }
}

} // namespace cmajor::build
