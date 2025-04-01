// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.building;

import cmajor.build.action;
import cmajor.build.compiling;
import cmajor.build.flags;
import cmajor.build.config;
import cmajor.build.install;
import cmajor.build.parsing;
import cmajor.build.archiving;
import cmajor.build.linking;
import cmajor.build.resources;
import cmajor.build.main.unit;
import cmdoclib;
import cmajor.cpp.backend.codegen;
import cmajor.masm.build;
import cmajor.sbin.build;
import cmajor.binder;
import cmajor.ast;
import soul.lexer;
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
    int optLevel = cmajor::symbols::GetOptimizationLevel();
    cmajor::ast::BackEnd backend = cmajor::ast::BackEnd::llvm;
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::systemx)
    {
        backend = cmajor::ast::BackEnd::systemx;
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        backend = cmajor::ast::BackEnd::cpp;
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
    {
        backend = cmajor::ast::BackEnd::masm;
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin)
    {
        backend = cmajor::ast::BackEnd::sbin;
    }
    std::unique_ptr<cmajor::ast::Project> project = ParseProjectFile(projectFilePath, config, backend, optLevel);
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
    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm)
    {
        module->DefineSymbol(U"LLVM_BACKEND");
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
    {
        module->DefineSymbol(U"CPP_BACKEND");
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
    {
        module->DefineSymbol(U"MASM_BACKEND");
    }
    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin)
    {
        module->DefineSymbol(U"SBIN_BACKEND");
    }
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

void Preprocess(cmajor::ast::Project* project)
{
    for (std::unique_ptr<cmajor::ast::CompileUnitNode>& compileUnit : project->CompileUnits())
    {
        if (compileUnit->GlobalNs()->HasUnnamedNs())
        {
            cmajor::ast::AddNamespaceImportsForUnnamedNamespaces(*compileUnit);
        }
    }
}

void BuildProject(cmajor::ast::Project* project, std::unique_ptr<cmajor::symbols::Module>& rootModule, bool& stop, bool resetRootModule, std::set<std::string>& builtProjects)
{
    try
    {
        cmajor::cpp::backend::SetGXXPath(cmajor::build::GetGXXPathFromBuildConfig());
        Variables variables;
        std::string outDir = project->OutDir();
        if (!outDir.empty())
        {
            variables.AddVariable(new Variable("OUT_DIR", outDir));
        }
        variables.AddVariable(new Variable("PROJECT_DIR", project->ProjectDir()));
        variables.AddVariable(new Variable("LIBRARY_DIR", project->LibraryDir()));
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
            bool systemLibraryInstalled = false;
            std::string config = cmajor::symbols::GetConfig();
            std::string configOptLevel = config;
            int optLevel = cmajor::symbols::GetOptimizationLevel();
            if (configOptLevel == "release")
            {
                configOptLevel.append("/").append(std::to_string(optLevel));
            }
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
            else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
            {
                astBackEnd = cmajor::ast::BackEnd::masm;
            }
            else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin)
            {
                astBackEnd = cmajor::ast::BackEnd::sbin;
            }
            else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cm)
            {
                astBackEnd = cmajor::ast::BackEnd::cm;
            }
            if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild))
            {
                upToDate = project->IsUpToDate(cmajor::ast::CmajorSystemModuleFilePath(config, astBackEnd, optLevel));
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
                util::LogMessage(project->LogStreamId(), "===== Building project '" + util::ToUtf8(project->Name()) + "' (" + project->FilePath() + ") using " + configOptLevel + 
                    " configuration.");
            }
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
            {
                cmdoclib::SetEmptyLibraryPrefix(util::ToUtf8(project->Name()));
            }
            cmajor::symbols::Context context;
            rootModule.reset(new cmajor::symbols::Module(&context, project->Name(), project->ModuleFilePath(), project->GetTarget()));
            rootModule->SetRootModule();
            context.SetRootModule(rootModule.get());
            //cmajor::symbols::SetRootModuleForCurrentThread(rootModule.get());
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
                ParseSourceFiles(project, rootModule->FileMap(), flags, rootModule.get());
                int n = rootModule->FileMap().NextFileId();
                for (int fileId = 0; fileId < n; ++fileId)
                {
                    rootModule->GetFileTable().RegisterFilePath(rootModule->FileMap().GetFilePath(fileId));
                }
                bool prevPreparing = rootModule->Preparing();
                rootModule->SetPreparing(true);
                cmajor::symbols::PrepareModuleForCompilation(&context, project->References(), project->GetTarget(), project->RootSpan(), project->RootFileIndex(),
                    project->RootCompileUnit());
                Preprocess(project);
                CreateSymbols(&context, context.RootModule()->GetSymbolTable(), project, stop);
                if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
                {
                    util::LogMessage(project->LogStreamId(), "Binding types...");
                }
                cmajor::binder::AttributeBinder attributeBinder(&context);
                std::vector<std::unique_ptr<cmajor::binder::BoundCompileUnit>> boundCompileUnits = BindTypes(&context, project, &attributeBinder, stop);
                if (stop)
                {
                    return;
                }
                rootModule->SetPreparing(prevPreparing);
                std::vector<std::string> objectFilePaths;
                std::vector<std::string> asmFilePaths;
                std::vector<std::string> cppFilePaths;
                std::map<int, cmdoclib::File> docFileMap;
                Compile(project, &context, boundCompileUnits, objectFilePaths, asmFilePaths, docFileMap, stop);
                if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
                {
                    cmdoclib::GenerateSymbolTableXml(rootModule.get(), docFileMap);
                }
                for (const auto& warning : rootModule->WarningCollection().Warnings())
                {
                    cmajor::symbols::LogWarning(rootModule->LogStreamId(), warning);
                }
                AddResources(project, rootModule.get(), objectFilePaths);
                for (const auto& rcFilePath : project->ResourceScriptFilePaths())
                {
                    rootModule->AddResourceScriptFilePath(rcFilePath);
                }
                if (cmajor::symbols::GetBackEnd() != cmajor::symbols::BackEnd::llvm && cmajor::symbols::GetBackEnd() != cmajor::symbols::BackEnd::cpp)
                {
                    GenerateMainUnit(project, &context, objectFilePaths, cppFilePaths);
                }
                if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
                {
                    util::LogMessage(project->LogStreamId(), "Writing module file...");
                }
                cmajor::symbols::SymbolWriter writer(project->ModuleFilePath(), &context);
                rootModule->Write(writer);
                rootModule->ResetFlag(cmajor::symbols::ModuleFlags::compiling);
                project->SetModuleFilePath(rootModule->OriginalFilePath());
                project->SetLibraryFilePath(rootModule->LibraryFilePath());
                if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
                {
                    util::LogMessage(project->LogStreamId(), "==> " + project->ModuleFilePath());
                }
                RunBuildActions(*project, variables);
                if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm ||
                    cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin)
                {
                    std::vector<std::string> resourceScriptFiles;
                    resourceScriptFiles.push_back(util::GetFullPath(util::Path::Combine(util::Path::Combine(util::CmajorRoot(), "rc"), "soul.xml.xpath.lexer.classmap.rc")));
                    for (const auto& rcFilePath : rootModule->AllResourceScriptFilePaths())
                    {
                        resourceScriptFiles.push_back(rcFilePath);
                    }
                    std::string classIndexFilePath;
                    std::string traceDataFilePath;
                    if (project->GetTarget() == cmajor::ast::Target::program || 
                        project->GetTarget() == cmajor::ast::Target::winguiapp || 
                        project->GetTarget() == cmajor::ast::Target::winapp)
                    {
                        classIndexFilePath = util::Path::Combine(util::Path::GetDirectoryName(project->ModuleFilePath()), "class_index.bin");
                        cmajor::symbols::MakeClassIndexFile(rootModule->GetSymbolTable().PolymorphicClasses(), classIndexFilePath);
                        traceDataFilePath = util::Path::Combine(util::Path::GetDirectoryName(project->ModuleFilePath()), "trace_data.bin");
                        rootModule->WriteTraceData(traceDataFilePath);
                    }
                    if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::masm)
                    {
                        cmajor::masm::build::VSBuild(project, rootModule.get(), asmFilePaths, cppFilePaths, resourceScriptFiles, classIndexFilePath, traceDataFilePath,
                            cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
                        if (project->GetTarget() == cmajor::ast::Target::program ||
                            project->GetTarget() == cmajor::ast::Target::winguiapp ||
                            project->GetTarget() == cmajor::ast::Target::winapp)
                        {
                            cmajor::masm::build::Install(project);
                        }
                    }
                    else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::sbin)
                    {
                        bool program = project->GetTarget() == cmajor::ast::Target::program ||
                            project->GetTarget() == cmajor::ast::Target::winguiapp ||
                            project->GetTarget() == cmajor::ast::Target::winapp;
                        std::string libraryFilePath = cmajor::sbin::build::MakeLib(
                            project, rootModule.get(), objectFilePaths, program, cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
                        if (program)
                        {
                            std::string vsProjectFilePath = cmajor::masm::build::MakeVSProjectFile(
                                project, rootModule.get(), asmFilePaths, cppFilePaths, resourceScriptFiles, classIndexFilePath,
                                traceDataFilePath, libraryFilePath, cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
                            cmajor::masm::build::MSBuild(vsProjectFilePath, cmajor::symbols::GetConfig(), project->LogStreamId());
                            cmajor::masm::build::Install(project);
                        }
                    }
                }
                else if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
                {
                    if (project->GetTarget() == cmajor::ast::Target::program ||
                        project->GetTarget() == cmajor::ast::Target::winguiapp ||
                        project->GetTarget() == cmajor::ast::Target::winapp)
                    {
                        std::string classIndexFilePath = util::Path::Combine(util::Path::GetDirectoryName(project->ModuleFilePath()), "class_index.bin");
                        cmajor::symbols::MakeClassIndexFile(rootModule->GetSymbolTable().PolymorphicClasses(), classIndexFilePath);
                        std::string  traceDataFilePath = util::Path::Combine(util::Path::GetDirectoryName(project->ModuleFilePath()), "trace_data.bin");
                        rootModule->WriteTraceData(traceDataFilePath);
                        GenerateRuntimeResourceFile(project, rootModule.get(), classIndexFilePath, traceDataFilePath);
                        CompileResourceScriptFiles(project, rootModule.get());
                    }
                }
                if (!objectFilePaths.empty())
                {
                    Archive(project, objectFilePaths);
                }
                if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm || cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
                {
                    GenerateMainUnit(project, &context, objectFilePaths, cppFilePaths);
                }
                if (cmajor::symbols::GetBackEnd() != cmajor::symbols::BackEnd::llvm && cmajor::symbols::GetBackEnd() != cmajor::symbols::BackEnd::cpp)
                {
                    Link(project, rootModule.get());
                }
                if (cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::cpp)
                {
                    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
                    {
                        util::LogMessage(project->LogStreamId(), "Writing project debug info file...");
                    }
                    std::string pdiFilePath = util::Path::ChangeExtension(project->ModuleFilePath(), ".pdi");
                    rootModule->WriteProjectDebugInfoFile(pdiFilePath);
                    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
                    {
                        util::LogMessage(project->LogStreamId(), "==> " + pdiFilePath);
                    }
                    if (project->GetTarget() == cmajor::ast::Target::program || 
                        project->GetTarget() == cmajor::ast::Target::winguiapp || 
                        project->GetTarget() == cmajor::ast::Target::winapp)
                    {
                        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
                        {
                            util::LogMessage(project->LogStreamId(), "Writing debug information file...");
                        }
#ifdef _WIN32
                        std::string cmdbFilePath = util::Path::ChangeExtension(project->ExecutableFilePath(), ".cmdb");
#else
                        std::string cmdbFilePath = project->ExecutableFilePath() + ".cmdb";
#endif
                        rootModule->WriteCmdbFile(cmdbFilePath);
                        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
                        {
                            util::LogMessage(project->LogStreamId(), "==> " + cmdbFilePath);
                        }
                    }
                }
                if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
                {
                    util::LogMessage(project->LogStreamId(), std::to_string(rootModule->GetSymbolTable().NumSpecializations()) + " class template specializations, " +
                        std::to_string(rootModule->GetSymbolTable().NumSpecializationsNew()) + " new, " +
                        std::to_string(rootModule->GetSymbolTable().NumSpecializationsCopied()) + " copied.");
                    util::LogMessage(project->LogStreamId(), "Project '" + util::ToUtf8(project->Name()) + "' built successfully.");
                }
                if (rootModule->IsSystemModule())
                {
                    project->SetSystemProject();
                }
                if (rootModule->Name() == U"System.Install")
                {
                    if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
                    {
                        InstallSystemLibraries(rootModule.get());
                        systemLibraryInstalled = true;
                    }
                }
                else if (rootModule->Name() == U"System.Windows.Install")
                {
                    if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::cmdoc))
                    {
                        InstallSystemWindowsLibraries(rootModule.get());
                        systemLibraryInstalled = true;
                    }
                }
            }
            if (resetRootModule)
            {
                PutModuleToModuleCache(std::move(rootModule));
                rootModule.reset();
            }
            if (systemLibraryInstalled)
            {
                cmajor::symbols::ResetModuleCache();
            }
        }
    }
    catch (const soul::lexer::ParsingException&)
    {
        throw;
    }
    catch (cmajor::symbols::Exception& ex)
    {
        ex.SetProject(util::ToUtf8(project->Name()));
        throw ex;
    }
    catch (const std::exception& ex)
    {
        throw std::runtime_error("project: " + util::ToUtf8(project->Name()) + ": " + util::PlatformStringToUtf8(ex.what()));
    }
}

cmajor::ast::Solution* currentSolution = nullptr;

void CleanProject(cmajor::ast::Project* project)
{
    std::string config = cmajor::symbols::GetConfig();
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(project->LogStreamId(), "Cleaning project '" + util::ToUtf8(project->Name()) + "' (" + project->FilePath() + ") using " + config + " configuration...");
    }
    std::filesystem::path mfp = project->ModuleFilePath();
    cmajor::symbols::RemoveModuleFromCache(project->ModuleFilePath());
    mfp.remove_filename();
    std::filesystem::remove_all(mfp);
    if (project->GetTarget() == cmajor::ast::Target::program || project->GetTarget() == cmajor::ast::Target::winguiapp || project->GetTarget() == cmajor::ast::Target::winapp)
    {
        std::filesystem::path efp = project->ExecutableFilePath();
        efp.remove_filename();
        std::filesystem::remove_all(efp);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(project->LogStreamId(), "Project '" + util::ToUtf8(project->Name()) + "' cleaned successfully.");
    }
}

void BuildProject(const std::string& projectFilePath, std::unique_ptr<cmajor::symbols::Module>& rootModule, std::set<std::string>& builtProjects)
{
    std::unique_ptr<cmajor::ast::Project> project = ReadProject(projectFilePath);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::clean))
    {
        if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::msbuild))
        {
            for (const std::string& referencedProjectFilePath : project->ReferencedProjectFilePaths())
            {
                std::unique_ptr<cmajor::ast::Project> referencedProject = ReadProject(referencedProjectFilePath);
                project->AddDependsOnId(referencedProject->Id());
                if (currentSolution == nullptr && cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::buildAll))
                {
                    BuildProject(referencedProjectFilePath, rootModule, builtProjects);
                }
            }
        }
        CleanProject(project.get());
    }
    else
    {
        stopBuild = false;
        BuildProject(project.get(), rootModule, stopBuild, true, builtProjects);
    }
}

struct BuildData
{
    BuildData(bool& stop_, util::SynchronizedQueue<cmajor::ast::Project*>& buildQueue_, util::SynchronizedQueue<cmajor::ast::Project*>& readyQueue_,
        std::vector<std::unique_ptr<cmajor::symbols::Module>>& rootModules_, bool& isSystemSolution_, std::set<std::string>& builtProjects_) :
        stop(stop_), buildQueue(buildQueue_), readyQueue(readyQueue_), rootModules(rootModules_), 
        isSystemSolution(isSystemSolution_), builtProjects(builtProjects_)
    {
    }
    std::mutex mtx;
    bool& stop;
    util::SynchronizedQueue<cmajor::ast::Project*>& buildQueue;
    util::SynchronizedQueue<cmajor::ast::Project*>& readyQueue;
    std::vector<std::unique_ptr<cmajor::symbols::Module>>& rootModules;
    bool& isSystemSolution;
    std::set<std::string>& builtProjects;
    std::vector<std::exception_ptr> exceptions;
};

void BuildThreadFunction(BuildData* buildData)
{
    try
    {
        cmajor::ast::Project* toBuild = buildData->buildQueue.Get();
        while (toBuild && !buildData->stop)
        {
            BuildProject(toBuild, buildData->rootModules[toBuild->Index()], buildData->stop, true, buildData->builtProjects);
            if (toBuild->IsSystemProject())
            {
                buildData->isSystemSolution = true;
            }
            buildData->readyQueue.Put(toBuild);
            toBuild = buildData->buildQueue.Get();
        }
    }
    catch (...)
    {
        std::lock_guard lock(buildData->mtx);
        buildData->exceptions.push_back(std::current_exception());
        buildData->buildQueue.Exit();
        buildData->readyQueue.Exit();
        buildData->stop = true;
    }
}

struct CurrentSolutionGuard
{
    CurrentSolutionGuard(cmajor::ast::Solution* currentSolution_)
    {
        currentSolution = currentSolution_;
    }
    ~CurrentSolutionGuard()
    {
        currentSolution = nullptr;
    }
};

void BuildSolution(const std::string& solutionFilePath, std::vector<std::unique_ptr<cmajor::symbols::Module>>& rootModules)
{
    std::string solutionName;
    std::vector<std::string> moduleNames;
    BuildSolution(solutionFilePath, rootModules, solutionName, moduleNames);
}

void BuildSolution(const std::string& solutionFilePath, std::vector<std::unique_ptr<cmajor::symbols::Module>>& rootModules, 
    std::string& solutionName, std::vector<std::string>& moduleNames)
{
    std::set<std::string> builtProjects;
    std::string config = cmajor::symbols::GetConfig();
    if (config == "release")
    {
        config.append("/").append(std::to_string(cmajor::symbols::GetOptimizationLevel()));
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::clean))
    {
        util::LogMessage(-1, "Cleaning solution '" + solutionFilePath + "' using " + config + " configuration.");
    }
    else
    {
        util::LogMessage(-1, "Building solution '" + solutionFilePath + "' using " + config + " configuration.");
    }
    std::unique_ptr<cmajor::ast::Solution> solution = ParseSolutionFile(solutionFilePath);
    solutionName = util::ToUtf8(solution->Name());
    CurrentSolutionGuard currentSolutionGuard(solution.get());
    int np = solution->ProjectFilePaths().size();
    for (int i = 0; i < np; ++i)
    {
        const std::string& projectFilePath = solution->ProjectFilePaths()[i];
        const std::string& relativeProjectFilePath = solution->RelativeProjectFilePaths()[i];
        std::unique_ptr<cmajor::ast::Project> project = ReadProject(projectFilePath);
        project->SetRelativeFilePath(relativeProjectFilePath);
        solution->AddProject(std::move(project));
    }
    std::vector<cmajor::ast::Project*> buildOrder = solution->CreateBuildOrder();
    std::vector<cmajor::ast::Project*> projectsToBuild;
    bool isSystemSolution = false;
    int n = buildOrder.size();
    for (int i = 0; i < n; ++i)
    {
        cmajor::ast::Project* project = buildOrder[i];
        project->SetLogStreamId(i);
        project->SetIndex(i);
        if (project->GetTarget() == cmajor::ast::Target::unitTest)
        {
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                util::LogMessage(-1, "skipping unit test project '" + util::ToUtf8(project->Name()) + "'");
            }
            continue;
        }
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::clean))
        {
            CleanProject(project);
        }
        else
        {
            projectsToBuild.push_back(project);
        }
    }
    if (!projectsToBuild.empty())
    {
        for (cmajor::ast::Project* project : projectsToBuild)
        {
            moduleNames.push_back(util::ToUtf8(project->Name()));
        }
        int numProjectsToBuild = projectsToBuild.size();
        int numThreads = 1;
        if (!cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile))
        {
            numThreads = std::min(numProjectsToBuild, int(std::thread::hardware_concurrency()));
        }
        rootModules.resize(numProjectsToBuild);
        if (numThreads <= 1)
        {
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                util::LogMessage(-1, "Building " + std::to_string(numProjectsToBuild) + " projects...");
            }
            for (int i = 0; i < numProjectsToBuild; ++i)
            {
                cmajor::ast::Project* project = projectsToBuild[i];
                stopBuild = false;
                BuildProject(project, rootModules[i], stopBuild, true, builtProjects);
            }
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                util::LogMessage(-1, "Solution '" + util::ToUtf8(solution->Name()) + "' built successfully.");
            }
        }
        else
        {
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                util::LogMessage(-1, "Building " + std::to_string(numProjectsToBuild) + " projects using " + std::to_string(numThreads) + " threads...");
            }
            stopBuild = false;
            util::SynchronizedQueue<cmajor::ast::Project*> buildQueue;
            util::SynchronizedQueue<cmajor::ast::Project*> readyQueue;
            BuildData buildData(stopBuild, buildQueue, readyQueue, rootModules, isSystemSolution, builtProjects);
            std::vector<std::thread> threads;
            for (int i = 0; i < numThreads; ++i)
            {
                threads.push_back(std::thread(BuildThreadFunction, &buildData));
                if (buildData.stop) break;
            }
            while (numProjectsToBuild > 0 && !stopBuild)
            {
                std::vector<cmajor::ast::Project*> building;
                for (cmajor::ast::Project* project : projectsToBuild)
                {
                    if (project->Ready())
                    {
                        building.push_back(project);
                        buildQueue.Put(project);
                    }
                }
                for (cmajor::ast::Project* project : building)
                {
                    projectsToBuild.erase(std::remove(projectsToBuild.begin(), projectsToBuild.end(), project), projectsToBuild.end());
                }
                cmajor::ast::Project* ready = readyQueue.Get();
                if (ready)
                {
                    ready->SetBuilt();
                    --numProjectsToBuild;
                }
            }
            buildQueue.Exit();
            readyQueue.Exit();
            stopBuild = true;
            int numStartedThreads = threads.size();
            for (int i = 0; i < numStartedThreads; ++i)
            {
                buildQueue.Put(nullptr);
            }
            for (int i = 0; i < numStartedThreads; ++i)
            {
                if (threads[i].joinable())
                {
                    threads[i].join();
                }
            }
            if (!buildData.exceptions.empty())
            {
                std::lock_guard lock(buildData.mtx);
                std::rethrow_exception(buildData.exceptions.front());
            }
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                util::LogMessage(-1, "Solution '" + util::ToUtf8(solution->Name()) + "' built successfully.");
            }
        }
    }
}

void Install(const std::string& projectFilePath)
{
    std::unique_ptr<cmajor::ast::Project> project = ReadProject(projectFilePath);
    cmajor::masm::build::MSBuild(util::Path::ChangeExtension(project->LibraryFilePath(), ".vcxproj"), cmajor::symbols::GetConfig(),
        cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
    cmajor::masm::build::Install(project.get());
}

} // namespace cmajor::build
