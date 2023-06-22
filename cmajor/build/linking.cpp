// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.linking;

import cmajor.llvm;
import cmajor.systemx.object;
import util;

namespace cmajor::build {

void LinkSystemX(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    switch (project->GetTarget())
    {
        case cmajor::ast::Target::program:
        {
            bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
            std::string clsIdFileName = util::Path::ChangeExtension(project->LibraryFilePath(), ".clsid");
            cmajor::symbols::MakeClassIdFile(rootModule->GetSymbolTable().PolymorphicClasses(), clsIdFileName);
            std::filesystem::path bdp = project->ExecutableFilePath();
            bdp.remove_filename();
            std::filesystem::create_directories(bdp);
            std::vector<std::string> binaryFiles;
            std::string systemMainObjectFilePath = util::GetFullPath(util::Path::Combine(util::CmajorRoot(), "system-x/system/Main/Main.o"));
            binaryFiles.push_back(systemMainObjectFilePath);
            for (const auto& libraryFile : rootModule->LibraryFilePaths())
            {
                binaryFiles.push_back(libraryFile);
            }
            cmajor::systemx::object::Link(rootModule->LogStreamId(), project->ExecutableFilePath(), binaryFiles, clsIdFileName, verbose);
            break;
        }
    }
}

const char* dynamicDebugRuntimeArg = "-lcmajor.cpp.rt.debug";
const char* dynamicReleaseRuntimeArg = "-lcmajor.cpp.rt.release";

void LinkCpp(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    switch (project->GetTarget())
    {
        case cmajor::ast::Target::program:
        case cmajor::ast::Target::winapp:
        case cmajor::ast::Target::winguiapp:
        {
            std::string cmajorLibDir = util::Path::Combine(cmajor::ast::CmajorRootDir(), "lib");
            std::string cmajorBinDir = util::Path::Combine(cmajor::ast::CmajorRootDir(), "bin");
            bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
            if (verbose)
            {
                util::LogMessage(project->LogStreamId(), "Linking...");
            }
            std::filesystem::path bdp = project->ExecutableFilePath();
            bdp.remove_filename();
            std::filesystem::create_directories(bdp);
            std::string linkCommand = "g++";
            linkCommand.append(" -L" + cmajorLibDir);
            linkCommand.append(" -L" + cmajorBinDir);
            linkCommand.append(" -Xlinker --start-group");
            for (const auto& libraryFilePath : rootModule->LibraryFilePaths())
            {
                linkCommand.append(" ").append(util::QuotedPath(libraryFilePath));
            }
            linkCommand.append(" -Xlinker --end-group");
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime))
            {
                linkCommand.append(" ").append(dynamicDebugRuntimeArg);
            }
            else
            {
                linkCommand.append(" ").append(dynamicReleaseRuntimeArg);
            }
            linkCommand.append(" -o " + util::QuotedPath(project->ExecutableFilePath()));
            switch (project->GetTarget())
            {
                case cmajor::ast::Target::program:
                {
                    linkCommand.append(" --entry=main");
                    break;
                }
                case cmajor::ast::Target::winapp:
                {
                    linkCommand.append(" --entry=main");
                    break;
                }
                case cmajor::ast::Target::winguiapp:
                {
                    linkCommand.append(" --entry=WinMain");
                    break;
                }
            }
            std::string errors;
            try
            {
                util::Process::Redirections redirections = util::Process::Redirections::processStdErr;
                if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
                {
                    redirections = redirections | util::Process::Redirections::processStdOut;
                }
                util::Process process(linkCommand, redirections);
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
                    throw std::runtime_error("executing '" + linkCommand + "' failed with exit code: " + std::to_string(exitCode));
                }
            }
            catch (const std::exception& ex)
            {
                throw std::runtime_error("linking executable '" + project->ExecutableFilePath() + "' failed: " + ex.what() + ":\nerrors:\n" + errors);
            }
            if (verbose)
            {
                util::LogMessage(project->LogStreamId(), "==> " + project->ExecutableFilePath());
            }
            break;
        }
    }
}

void Link(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule)
{
    switch (cmajor::symbols::GetBackEnd())
    {
        case cmajor::symbols::BackEnd::llvm:
        {
            cmajor::llvm::Link(project, rootModule);
            break;
        }
        case cmajor::symbols::BackEnd::systemx:
        {
            LinkSystemX(project, rootModule);
            break;
        }
        case cmajor::symbols::BackEnd::cpp:
        {
            LinkCpp(project, rootModule);
            break;
        }
    }
}

} // namespace cmajor::build
