// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.service;

import cmajor.build;
import cmajor.symbols;
import cmajor.backend;
import soul.lexer;
import std.filesystem;
import util;

namespace cmajor::service {

cmajor::info::bs::CompileError ToError(const soul::lexer::ParsingException& ex)
{
    cmajor::info::bs::CompileError error;
    error.message = ex.Message();
    error.project = ex.Project();
    error.file = ex.FileName();
    if (ex.GetSourcePos().IsValid())
    {
        error.line = ex.GetSourcePos().line;
        error.scol = ex.GetSourcePos().col;
    }
    return error;
}

void AddWarnings(std::vector<cmajor::info::bs::Warning>& warnings)
{
    cmajor::symbols::CompileWarningCollection& warningCollection = cmajor::symbols::GetGlobalWarningCollection();
    for (const auto& symbolsWarning : warningCollection.Warnings())
    {
        cmajor::info::bs::Warning warning;
        warning.number = symbolsWarning.Number();
        warning.message = symbolsWarning.Message();
        warning.project = util::ToUtf8(symbolsWarning.Project());
        soul::ast::FullSpan defined = symbolsWarning.Defined();
        if (defined.IsValid())
        {
            cmajor::symbols::Module* module = cmajor::symbols::GetModuleById(defined.moduleId);
            if (module)
            {
                warning.file = module->GetFilePath(defined.fileIndex);
                soul::ast::LineColLen lineColLen = cmajor::symbols::GetLineColLen(defined);
                warning.line = lineColLen.line;
                warning.scol = lineColLen.col;
                warning.ecol = lineColLen.col + lineColLen.len;
            }
        }
        warnings.push_back(warning);
        for (const auto& reference : symbolsWarning.References())
        {
            cmajor::info::bs::Warning warning;
            warning.number = symbolsWarning.Number();
            warning.message = "see reference";
            warning.project = util::ToUtf8(symbolsWarning.Project());
            if (reference.IsValid())
            {
                cmajor::symbols::Module* module = cmajor::symbols::GetModuleById(reference.moduleId);
                if (module)
                {
                    warning.file = module->GetFilePath(reference.fileIndex);
                    soul::ast::LineColLen lineColLen = cmajor::symbols::GetLineColLen(reference);
                    warning.line = lineColLen.line;
                    warning.scol = lineColLen.col;
                    warning.ecol = lineColLen.col + lineColLen.len;
                    warnings.push_back(warning);
                }
            }
        }
    }
}

BuildResultMessage::BuildResultMessage(const cmajor::info::bs::BuildResult& result_) : ServiceMessage(ServiceMessageKind::buildResult), result(result_)
{
}

BuildStoppedMessage::BuildStoppedMessage() : ServiceMessage(ServiceMessageKind::buildStoppedMessage)
{
}

class BuildService
{
public:
    static BuildService& Instance();
    void Start();
    void Stop();
    void Run();
    bool Running() const { return running; }
    bool BuildInProgress() const { return buildInProgress; }
    void CancelBuild();
    void ExecuteBuildCommand(cmajor::info::bs::BuildCommand* command);
    bool CommandAvailableOrExiting() const { return buildCommand != nullptr || exit; }
private:
    BuildService();
    void ExecuteCommand();
    std::unique_ptr<cmajor::info::bs::BuildCommand> buildCommand;
    bool buildInProgress;
    bool exit;
    bool running;
    std::thread thread;
    std::mutex mtx;
    std::condition_variable buildCommandAvailable;
};

BuildService& BuildService::Instance()
{
    static BuildService instance;
    return instance;
}

BuildService::BuildService() : buildInProgress(false), exit(false), running(false)
{
}

void RunBuildService(BuildService* buildService)
{
    buildService->Run();
}

void BuildService::Start()
{
    thread = std::thread(RunBuildService, this);
}

void BuildService::ExecuteBuildCommand(cmajor::info::bs::BuildCommand* command)
{
    std::lock_guard<std::mutex> lock(mtx);
    buildCommand.reset(command);
    buildCommandAvailable.notify_one();
}

void BuildService::Run()
{
    running = true;
    while (!exit)
    {
        std::unique_lock<std::mutex> lock(mtx);
        buildCommandAvailable.wait(lock, [this] { return CommandAvailableOrExiting(); });
        if (exit)
        {
            running = false;
            return;
        }
        ExecuteCommand();
        buildCommand.reset();
    }
    running = false;
}

void BuildService::ExecuteCommand()
{
    cmajor::symbols::SetRootModuleForCurrentThread(nullptr);
    buildInProgress = true;
    std::unique_ptr<cmajor::symbols::Module> rootModule;
    std::vector<std::unique_ptr<cmajor::symbols::Module>> rootModules;
    std::set<std::string> builtProjects;
    int logStreamId = -1;
    cmajor::info::bs::BuildResult result;
    try
    {
        cmajor::symbols::BackEnd backend = cmajor::symbols::BackEnd::cpp;
        if (buildCommand->backend.empty())
        {
            throw std::runtime_error("backend required");
        }
        else if (buildCommand->backend == "cpp")
        {
            backend = cmajor::symbols::BackEnd::cpp;
        }
        else if (buildCommand->backend == "llvm")
        {
            backend = cmajor::symbols::BackEnd::llvm;
        }
        else if (buildCommand->backend == "masm")
        {
            backend = cmajor::symbols::BackEnd::masm;
        }
        else if (buildCommand->backend == "sbin")
        {
            backend = cmajor::symbols::BackEnd::sbin;
        }
        else if (buildCommand->backend == "cm")
        {
            backend = cmajor::symbols::BackEnd::cm;
            cmajor::symbols::SetBackEnd(backend);
            result = BuildWithCmMasmCompiler(*buildCommand);
            BuildResultMessage* resultMessage = new BuildResultMessage(result);
            PutServiceMessage(resultMessage);
            buildInProgress = false;
            return;
        }
        else
        {
            throw std::runtime_error("unsupported backend '" + buildCommand->backend + "'");
        }
        if (buildCommand->filePath.empty())
        {
            throw std::runtime_error("file path required");
        }
        else if (!std::filesystem::exists(buildCommand->filePath))
        {
            throw std::runtime_error("file '" + buildCommand->filePath + "' does not exist");
        }
        cmajor::build::ResetStopBuild();
        cmajor::symbols::ResetGlobalFlags();
        cmajor::symbols::SetBackEnd(backend);
        int optLevel = std::stoi(buildCommand->optimizationLevel);
        if (buildCommand->config == "release")
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::release);
            cmajor::symbols::SetOptimizationLevel(optLevel);
        }
        else if (buildCommand->config.empty() || buildCommand->config == "debug")
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::generateDebugInfo); 
        }
        else 
        {
            throw std::runtime_error("unknown configuration '" + buildCommand->config);
        }
        cmajor::symbols::ClearDisabledWarnings();
        if (!buildCommand->disabledWarnings.empty())
        {
            std::vector<std::string> disabledWarnings = util::Split(buildCommand->disabledWarnings, ';');
            for (const auto warning : disabledWarnings)
            {
                int warningNumber = std::stoi(warning);
                cmajor::symbols::DisableWarning(warningNumber);
            }
        }
        if (buildCommand->verbose)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
        }
        if (buildCommand->quiet)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::quiet);
        }
        if (buildCommand->clean)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::clean);
        }
        if (buildCommand->rebuild)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::rebuild);
        }
        if (buildCommand->emitIR)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::emitLlvm);
        }
        if (buildCommand->singleThreadedCompile || backend == cmajor::symbols::BackEnd::sbin)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::singleThreadedCompile);
        }
        if (buildCommand->buildAllDependencies)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::buildAll);
        }
        if (buildCommand->disableModuleCache)
        {
            cmajor::symbols::SetUseModuleCache(false);
        }
        else
        {
            cmajor::symbols::SetUseModuleCache(true);
        }
        for (const auto& define : buildCommand->defines)
        {
            cmajor::symbols::DefineCommandLineConditionalSymbol(util::ToUtf32(define));
        }
        switch (backend)
        {
            case cmajor::symbols::BackEnd::llvm:
            {
                cmajor::backend::SetCurrentBackEnd(cmajor::backend::BackEndKind::llvmBackEnd);
                util::LogMessage(-1, "Cmajor with LLVM backend compiler version " + cmajor::symbols::GetCompilerVersion() + " for Windows x64");
                break;
            }
            case cmajor::symbols::BackEnd::cpp:
            {
                cmajor::backend::SetCurrentBackEnd(cmajor::backend::BackEndKind::cppBackEnd);
                util::LogMessage(-1, "Cmajor with C++ backend compiler version " + cmajor::symbols::GetCompilerVersion() + " for Windows x64");
                break;
            }
            case cmajor::symbols::BackEnd::masm:
            {
                cmajor::backend::SetCurrentBackEnd(cmajor::backend::BackEndKind::masmBackEnd);
                util::LogMessage(-1, "Cmajor with MASM backend compiler version " + cmajor::symbols::GetCompilerVersion() + " for Windows x64");
                break;
            }
            case cmajor::symbols::BackEnd::sbin:
            {
                cmajor::backend::SetCurrentBackEnd(cmajor::backend::BackEndKind::sbinBackEnd);
                util::LogMessage(-1, "Cmajor with SBIN backend compiler version " + cmajor::symbols::GetCompilerVersion() + " for Windows x64");
                break;
            }
        }
        cmajor::symbols::ClearGlobalWarningCollection();
        cmajor::symbols::SetUseGlobalWarningCollection(true);
        if (buildCommand->filePath.ends_with(".cms"))
        {
            cmajor::build::BuildSolution(util::GetFullPath(buildCommand->filePath), rootModules);
        }
        else if (buildCommand->filePath.ends_with(".cmp"))
        {
            cmajor::build::BuildProject(util::GetFullPath(buildCommand->filePath), rootModule, builtProjects);
        }
        else
        {
            throw std::runtime_error("file path has unknown extension (not .cms or .cmp)");
        }
        result.success = true;
        AddWarnings(result.warnings);
    }
    catch (const soul::lexer::ParsingException& ex)
    {
        util::LogMessage(logStreamId, ex.what());
        result.success = false;
        result.errors.push_back(ToError(ex));
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        util::LogMessage(logStreamId, ex.What());
        result.success = false;
        result.errors = ex.ToErrors();
    }
    catch (const std::exception& ex)
    {
        util::LogMessage(logStreamId, ex.what());
        result.success = false;
        cmajor::info::bs::CompileError error;
        error.message = ex.what();
        result.errors.push_back(error);
    }
    BuildResultMessage* resultMessage = new BuildResultMessage(result);
    PutServiceMessage(resultMessage);
    buildInProgress = false;
}

void BuildService::Stop()
{
    exit = true;
    buildCommandAvailable.notify_one();
    CancelBuild();
    thread.join();
}

void BuildService::CancelBuild()
{
    if (buildInProgress)
    {
        if (buildCommand->backend == "cm")
        {
            StopCmMasmBuild();
        }
        else
        {
            cmajor::build::StopBuild();
        }
        buildCommand.reset();
        buildInProgress = false;
        PutServiceMessage(new BuildStoppedMessage());
    }
}

void StartBuildService()
{
    BuildService::Instance().Start();
}

void StopBuildService()
{
    if (BuildService::Instance().Running())
    {
        BuildService::Instance().Stop();
    }
}

void ExecuteBuildCommand(cmajor::info::bs::BuildCommand* command)
{
    if (BuildInProgress())
    {
        throw std::runtime_error("build in progress");
    }
    BuildService::Instance().ExecuteBuildCommand(command);
}

bool BuildInProgress()
{
    return BuildService::Instance().BuildInProgress();
}

void CancelBuild()
{
    if (BuildInProgress())
    {
        BuildService::Instance().CancelBuild();
    }
}

GetDefinitionReplyServiceMessage::GetDefinitionReplyServiceMessage(const cmajor::info::bs::GetDefinitionReply& reply_) :
    ServiceMessage(ServiceMessageKind::getDefinitionReply), reply(reply_)
{
}

cmajor::info::bs::GetDefinitionReply GetDefinition(const cmajor::info::bs::GetDefinitionRequest& request)
{
    cmajor::symbols::SetRootModuleForCurrentThread(nullptr);
    cmajor::info::bs::GetDefinitionReply reply;
    int len = util::ToUtf32(request.identifier).length();
    try
    {
        cmajor::ast::BackEnd backend = cmajor::ast::BackEnd::llvm;
        if (request.backend == "cpp")
        {
            backend = cmajor::ast::BackEnd::cpp;
        }
        else if (request.backend == "systemx")
        {
            backend = cmajor::ast::BackEnd::systemx;
        }
        else if (request.backend == "masm")
        {
            backend = cmajor::ast::BackEnd::masm;
        }
        else if (request.backend == "sbin")
        {
            backend = cmajor::ast::BackEnd::sbin;
        }
        cmajor::ast::Config config = cmajor::ast::Config::debug;
        if (request.config == "release")
        {
            config = cmajor::ast::Config::release;
        }
        cmajor::symbols::Module* module = cmajor::symbols::GetModuleBySourceFile(backend, config, request.identifierLocation.file);
        if (!module)
        {
            std::u32string moduleName = util::ToUtf32(request.projectName);
            int optLevel = std::stoi(request.optimizationLevel);
            cmajor::ast::Project project(moduleName, request.projectFilePath, request.config, backend, optLevel);
            std::string moduleFilePath = project.ModuleFilePath();
            bool readModule = false;
            if (!cmajor::symbols::IsModuleCached(moduleFilePath))
            {
                readModule = true;
            }
            else
            {
                module = cmajor::symbols::GetModuleBySourceFile(backend, config, request.identifierLocation.file);
                if (!module)
                {
                    readModule = true;
                }
            }
            if (!module && readModule)
            {
                std::unique_ptr<cmajor::symbols::ModuleCache> prevCache;
                try
                {
                    if (cmajor::symbols::IsSystemModule(moduleName))
                    {
                        prevCache = cmajor::symbols::ReleaseModuleCache();
                        cmajor::symbols::InitModuleCache();
                    }
                    else
                    {
                        cmajor::symbols::MoveNonSystemModulesTo(prevCache);
                        cmajor::symbols::UpdateModuleCache();
                    }
                    std::unique_ptr<cmajor::symbols::Module> module(new cmajor::symbols::Module(moduleFilePath, true));
                    cmajor::symbols::SetCacheModule(moduleFilePath, std::move(module));
                    cmajor::symbols::RestoreModulesFrom(prevCache.get());
                }
                catch (...)
                {
                    cmajor::symbols::RestoreModulesFrom(prevCache.get());
                    throw;
                }
            }
            module = cmajor::symbols::GetModuleBySourceFile(backend, config, request.identifierLocation.file);
        }
        if (module)
        {
            int32_t fileIndex = module->GetFileIndexForFilePath(request.identifierLocation.file);
            if (fileIndex != -1)
            {
                int32_t line = request.identifierLocation.line;
                int32_t scol = request.identifierLocation.scol;
                soul::ast::LineColLen lineColLen(line, scol, len);
                const std::vector<int>* lineStarts = module->FileMap().LineStartIndeces(fileIndex);
                if (!lineStarts)
                {
                    module->FileMap().ReadFile(fileIndex);
                    lineStarts = module->FileMap().LineStartIndeces(fileIndex);
                }
                if (lineStarts)
                {
                    soul::ast::Span span(soul::ast::LineColLenToPos(lineColLen, *lineStarts), len);
                    cmajor::symbols::SymbolLocation identifierLocation(module->Id(), fileIndex, span);
                    cmajor::symbols::SymbolLocation definitionLocation = module->GetSymbolTable().GetDefinitionLocation(identifierLocation);
                    std::string filePath = cmajor::symbols::GetSourceFilePath(definitionLocation.fileIndex, definitionLocation.moduleId);
                    if (filePath.empty())
                    {
                        std::string moduleName = "<unknown>";
                        cmajor::symbols::Module* m = cmajor::symbols::GetModuleById(definitionLocation.moduleId);
                        if (m)
                        {
                            moduleName = util::ToUtf8(m->Name());
                        }
                        throw std::runtime_error("file path for file index " + std::to_string(definitionLocation.fileIndex) + " not found from module '" + moduleName + "'");
                    }
                    reply.definitionLocation.file = filePath;
                    cmajor::symbols::Module* m = cmajor::symbols::GetModuleById(definitionLocation.moduleId);
                    if (m)
                    {
                        soul::ast::LineColLen lineColLen = m->GetLineColLen(definitionLocation.span, definitionLocation.fileIndex);
                        if (lineColLen.IsValid())
                        {
                            reply.definitionLocation.line = lineColLen.line;
                            reply.definitionLocation.scol = lineColLen.col;
                            reply.succeeded = true;
                        }
                        else
                        {
                            throw std::runtime_error("definition location of identifier '" + request.identifier + "' not found");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("definition location of identifier '" + request.identifier + "' not found");
                    }
                }
                else
                {
                    throw std::runtime_error("definition location of identifier '" + request.identifier + "' not found");
                }
            }
            else
            {
                throw std::runtime_error("source file '" + request.identifierLocation.file + "' not included by module '" + util::ToUtf8(module->Name()) + 
                    "' or any of its referenced modules");
            }
        }
        else
        {
            throw std::runtime_error("module for source file '" + request.identifierLocation.file + "' not in cache");
        }
    }
    catch (const std::exception& ex)
    {
        reply.succeeded = false;
        reply.error = ex.what();
    }
    return reply;
}

} // cmajor::service
