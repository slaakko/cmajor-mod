// =================================
// Copyright (c) 2024 Seppo Laakko
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
        if (buildCommand->config == "release")
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::release);
        }
        else if (buildCommand->config.empty() || buildCommand->config == "debug")
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::generateDebugInfo); 
        }
        else 
        {
            throw std::runtime_error("unknown configuration '" + buildCommand->config);
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
        if (buildCommand->linkWithDebugRuntime)
        {
            cmajor::symbols::SetGlobalFlag(cmajor::symbols::GlobalFlags::linkWithDebugRuntime);
        }
        if (buildCommand->singleThreadedCompile)
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
        if (!buildCommand->optimizationLevel.empty() && buildCommand->optimizationLevel != "default")
        {
            int optimizationLevel = std::stoi(buildCommand->optimizationLevel);
            if (optimizationLevel < 0 || optimizationLevel > 3)
            {
                throw std::runtime_error("optimization level out of range");
            }
            cmajor::symbols::SetOptimizationLevel(optimizationLevel);
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
        }
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
        cmajor::build::StopBuild();
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
        cmajor::ast::Config config = cmajor::ast::Config::debug;
        if (request.config == "release")
        {
            config = cmajor::ast::Config::release;
        }
        cmajor::symbols::Module* module = cmajor::symbols::GetModuleBySourceFile(backend, config, request.identifierLocation.file);
        if (!module)
        {
            std::u32string moduleName = util::ToUtf32(request.projectName);
            cmajor::ast::Project project(moduleName, request.projectFilePath, request.config, backend);
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
                soul::ast::LineColLen lineColLen(line, scol, 1);
                const std::vector<int>* lineStarts = module->FileMap().LineStartIndeces(fileIndex);
                if (!lineStarts)
                {
                    module->FileMap().ReadFile(fileIndex);
                    lineStarts = module->FileMap().LineStartIndeces(fileIndex);
                }
                if (lineStarts)
                {
                    soul::ast::Span span(soul::ast::LineColLenToPos(lineColLen, *lineStarts), 1);
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
                    const std::vector<int>* lineStarts = module->FileMap().LineStartIndeces(definitionLocation.fileIndex);
                    if (!lineStarts)
                    {
                        module->FileMap().ReadFile(definitionLocation.fileIndex);
                        lineStarts = module->FileMap().LineStartIndeces(definitionLocation.fileIndex);
                    }
                    if (lineStarts)
                    {
                        soul::ast::LineColLen lineColLen = soul::ast::SpanToLineColLen(definitionLocation.span, *lineStarts);
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
