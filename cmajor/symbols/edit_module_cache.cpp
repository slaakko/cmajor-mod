// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.edit_module_cache;

import cmajor.symbols.context;
import cmajor.symbols.global.flags;
import cmajor.symbols.modules;
import cmajor.symbols.module_cache;
import cmajor.symbols.sources; 
import cmajor.ast;
import util;

namespace cmajor::symbols {

ReadProjectFunc readProjectFunction;

void SetReadProjectFunction(ReadProjectFunc readProjectFunc)
{
    readProjectFunction = readProjectFunc;
}

std::string MakeEditModuleKey(const std::string& projectFilePath, const std::string& backendStr, const std::string& configurationStr)
{
    std::string editModuleKey = projectFilePath;
    editModuleKey.append(":").append(backendStr).append(":").append(configurationStr);
    return editModuleKey;
}

bool toolChainsRead = false;

EditModuleCache::EditModuleCache()
{
}

EditModuleCache& EditModuleCache::Instance() 
{ 
    static EditModuleCache instance;
    return instance; 
}

LoadEditModuleResult EditModuleCache::LoadEditModule(const std::string& projectFilePath, const std::string& backendStr, const std::string& configurationStr)
{
    LoadEditModuleResult result;
    result.ok = true;
    result.error = std::string();
    result.startLoading = std::chrono::steady_clock::now();
    result.resultKind = LoadEditModuleResultKind::moduleLoaded;
    ResetGlobalFlags();
    std::string projectFileContent = util::ReadFile(projectFilePath);
    std::string projectFileHash = util::GetSha1MessageDigest(projectFileContent);
    int index = editModules.size();
    std::string editModuleKey = MakeEditModuleKey(projectFilePath, backendStr, configurationStr);
    result.key = editModuleKey;
    auto it = editModuleMap.find(editModuleKey);
    if (it != editModuleMap.cend())
    {
        const std::pair<Module*, std::string>& moduleHashPair = it->second;
        const std::string& prevHash = moduleHashPair.second;
        if (prevHash == projectFileHash)
        {
            result.resultKind = LoadEditModuleResultKind::moduleUpToDate;
            result.endLoading = std::chrono::steady_clock::now();
            return result;
        }
        Module* module = moduleHashPair.first;
        index = module->Index();
        result.resultKind = LoadEditModuleResultKind::moduleReloaded;
    }
/*  TODO
    if (!toolChainsRead)
    {
        toolChainsRead = true;
        ReadToolChains(GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose));
    }
    ResetToolChain();
*/
    cmajor::ast::BackEnd backend = cmajor::ast::BackEnd::llvm;
    if (backendStr == "cpp")
    {
        backend = cmajor::ast::BackEnd::cpp;
        SetBackEnd(cmajor::symbols::BackEnd::cpp);
    }
    else if (backendStr == "llvm")
    {
        backend = cmajor::ast::BackEnd::llvm;
        SetBackEnd(cmajor::symbols::BackEnd::llvm);
    }
    cmajor::ast::Config config = cmajor::ast::Config::debug;
    if (configurationStr == "release")
    {
        SetGlobalFlag(cmajor::symbols::GlobalFlags::release);
        config = cmajor::ast::Config::release;
    }
    if (!readProjectFunction)
    {
        result.ok = false;
        result.error = "read project function not set";
        return result;
    }
    std::unique_ptr<cmajor::ast::Project> project = readProjectFunction(projectFilePath);
    Context context;
    std::unique_ptr<Module> module(new Module(&context, project->Name(), project->ModuleFilePath(), project->GetTarget()));
    context.SetRootModule(module.get());
    module->SetIndex(index);
    module->SetRootModule();
    module->SetLogStreamId(project->LogStreamId());
    module->SetCurrentProjectName(project->Name());
    module->SetCurrentToolName(U"cmccs");
    module->SetFlag(cmajor::symbols::ModuleFlags::compiling);
    PrepareModuleForCompilation(&context, project->References(), project->GetTarget(), soul::ast::Span(), -1, nullptr);
    // TODO module->SetSources(new Sources(project->SourceFilePaths()));
    ParseResult parseResult = module->ParseSources();
    result.ok = parseResult.ok;
    result.numberOfErrors = parseResult.numberOfErrors;
    result.synchronized = parseResult.synchronized;
    result.error = parseResult.error;
    //result.startParsing = parseResult.start; TODO!!!
    //result.endParsing = parseResult.end; TODO!!!
    while (index >= editModules.size())
    {
        editModules.push_back(std::unique_ptr<Module>());
    }
    editModuleMap[editModuleKey] = std::make_pair(module.get(), projectFileHash);
    editModules[index].reset(module.release());
    result.endLoading = std::chrono::steady_clock::now();
    return result;
}

Module* EditModuleCache::GetEditModule(const std::string& projectFilePath, const std::string& backendStr, const std::string& configurationStr)
{
    std::string editModuleKey = MakeEditModuleKey(projectFilePath, backendStr, configurationStr);
    auto it = editModuleMap.find(editModuleKey);
    if (it != editModuleMap.cend())
    {
        return it->second.first;
    }
    else
    {
        return nullptr;
    }
}

void InitEditModuleCache()
{
    EditModuleCache::Init();
}

void DoneEditModuleCache()
{
    EditModuleCache::Done();
}

} // namespace cmajor::symbols

