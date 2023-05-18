// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.building;

import cmajor.build.compiling;
import cmajor.build.flags;
import cmajor.build.parsing;
import cmajor.binder;
import cmajor.ast;
import std.filesystem;
import util;

namespace cmajor::build {

bool stopBuild = false;

void StopBuild()
{
    stopBuild = true;
}

void ResetStopBuild()
{
    stopBuild = false;
}

std::unique_ptr<cmajor::ast::Project> ReadProject(const std::string& projectFilePath)
{
    std::string config = cmajor::symbols::GetConfig();
    cmajor::ast::BackEnd backend = cmajor::ast::BackEnd::llvm;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        backend = cmajor::ast::BackEnd::systemx;
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        backend = cmajor::ast::BackEnd::cpp;
    }
    std::string toolchain = "llvm";
    std::unique_ptr<cmajor::ast::Project> project = ParseProjectFile(projectFilePath, config, backend, toolchain);
    return project;
}

void SetDefines(cmajor::symbols::Module* module, const std::string& definesFilePath)
{
    module->ClearDefines();
    if (cmajor::symbols::GetConfig() == "debug")
    {
        module->DefineSymbol(U"DEBUG");
    }
    else if (cmajor::symbols::GetConfig() == "release")
    {
        module->DefineSymbol(U"RELEASE");
    }
    else if (cmajor::symbols::GetConfig() == "profile")
    {
        module->DefineSymbol(U"RELEASE");
        module->DefineSymbol(U"PROFILE");
    }
#ifdef _WIN32
    module->DefineSymbol(U"WINDOWS");
#else
    module->DefineSymbol(U"LINUX");
#endif
    std::ifstream definesFile(definesFilePath);
    if (definesFile)
    {
        std::string line;
        while (std::getline(definesFile, line))
        {
            module->DefineSymbol(util::ToUtf32(line));
        }
    }
}

void BuildProject(cmajor::ast::Project* project, std::unique_ptr<cmajor::symbols::Module>& rootModule, bool& stop, bool resetRootModule, std::set<std::string>& builtProjects)
{
    if (!GetGlobalFlag(cmajor::symbols::GlobalFlags::msbuild))
    {
        if (builtProjects.find(project->FilePath()) != builtProjects.cend()) return;
        builtProjects.insert(project->FilePath());
        for (const std::string& referencedProjectFilePath : project->ReferencedProjectFilePaths())
        {
            std::unique_ptr<cmajor::ast::Project> referencedProject = ReadProject(referencedProjectFilePath);
            project->AddDependsOnId(referencedProject->Id());
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::buildAll))
            {
                if (builtProjects.find(referencedProjectFilePath) == builtProjects.cend())
                {
                    std::unique_ptr<cmajor::symbols::Module> module;
                    try
                    {
                        BuildProject(referencedProject.get(), module, stop, resetRootModule, builtProjects);
                    }
                    catch (...)
                    {
                        rootModule.reset(module.release());
                        throw;
                    }
                }
            }
        }
        std::string config = cmajor::symbols::GetConfig();
        bool isSystemModule = cmajor::symbols::IsSystemModule(project->Name());
        if (isSystemModule)
        {
            project->SetSystemProject();
        }
        bool upToDate = false;
        cmajor::ast::BackEnd astBackEnd = cmajor::ast::BackEnd::llvm;
        if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
        {
            astBackEnd = cmajor::ast::BackEnd::systemx;
        }
        else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
        {
            astBackEnd = cmajor::ast::BackEnd::cpp;
        }
        if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild))
        {
            upToDate = project->IsUpToDate(cmajor::ast::CmajorSystemModuleFilePath(config, astBackEnd, cmajor::ast::GetToolChain()));
        }
        if (upToDate)
        {
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                util::LogMessage(project->LogStreamId(), "===== Project '" + util::ToUtf8(project->Name()) + "' (" + project->FilePath() + ") is up-to-date.");
            }
            return;
        }
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(project->LogStreamId(), "===== Building project '" + util::ToUtf8(project->Name()) + "' (" + project->FilePath() + ") using " + config + " configuration.");
        }
        rootModule.reset(new cmajor::symbols::Module(project->Name(), project->ModuleFilePath(), project->GetTarget()));
        rootModule->SetRootModule();
        cmajor::symbols::SetRootModuleForCurrentThread(rootModule.get());
        {
            rootModule->SetLogStreamId(project->LogStreamId());
            rootModule->SetCurrentProjectName(project->Name());
            rootModule->SetCurrentToolName(U"cmc");
            std::filesystem::path libraryFilePath = project->LibraryFilePath();
            std::filesystem::path libDir = libraryFilePath.remove_filename();
            std::string definesFilePath = util::GetFullPath((libDir / std::filesystem::path("defines.txt")).generic_string());
            SetDefines(rootModule.get(), definesFilePath);
            rootModule->SetFlag(cmajor::symbols::ModuleFlags::compiling);
            Flags flags = Flags::none;
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile))
            {
                flags = flags | Flags::singleThreadedParse;
            }
            ParseSourceFiles(project, rootModule->FileMap(), flags);
            bool prevPreparing = rootModule->Preparing();
            rootModule->SetPreparing(true);
            cmajor::symbols::PrepareModuleForCompilation(rootModule.get(), project->References(), project->GetTarget());
            CreateSymbols(rootModule->GetSymbolTable(), project, stop);
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                util::LogMessage(project->LogStreamId(), "Binding types...");
            }
            cmajor::binder::AttributeBinder attributeBinder(rootModule.get());
            std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>> boundCompileUnits = BindTypes(rootModule.get(), project, &attributeBinder, stop);
            if (stop)
            {
                return;
            }
            rootModule->SetPreparing(prevPreparing);
            std::vector<std::string> objectFilePaths;
            Compile(project, rootModule.get(), boundCompileUnits, objectFilePaths, stop);
        }
    }
}

void BuildProject(const std::string& projectFilePath, std::unique_ptr<cmajor::symbols::Module>& rootModule, std::set<std::string>& builtProjects)
{
    std::unique_ptr<cmajor::ast::Project> project = ReadProject(projectFilePath);
    stopBuild = false;
    BuildProject(project.get(), rootModule, stopBuild, true, builtProjects);
}

} // namespace cmajor::build
