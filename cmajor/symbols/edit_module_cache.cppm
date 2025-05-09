// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.edit_module_cache;

import cmajor.ast.project;
import std.core;

export namespace cmajor::symbols {

class Module;
using ReadProjectFunc = std::unique_ptr<cmajor::ast::Project>(*)(const std::string&);

void SetReadProjectFunction(ReadProjectFunc readProjectFunc);

std::string MakeEditModuleKey(const std::string& projectFilePath, const std::string& backendStr, const std::string& configurationStr);

enum class LoadEditModuleResultKind
{
    moduleUpToDate, moduleLoaded, moduleReloaded
};

struct LoadEditModuleResult
{
    LoadEditModuleResult() : ok(true), numberOfErrors(), synchronized(false), error(), resultKind(LoadEditModuleResultKind::moduleUpToDate), startLoading(), startParsing(), endLoading()
    {
    }
    bool ok;
    int numberOfErrors;
    bool synchronized;
    std::string error;
    std::string key;
    LoadEditModuleResultKind resultKind;
    std::chrono::steady_clock::time_point startLoading;
    std::chrono::steady_clock::time_point startParsing;
    std::chrono::steady_clock::time_point endParsing;
    std::chrono::steady_clock::time_point endLoading;
};

class EditModuleCache
{
public:
    static void Init();
    static void Done();
    EditModuleCache(const EditModuleCache&) = delete;
    EditModuleCache(EditModuleCache&&) = delete;
    EditModuleCache& operator=(const EditModuleCache&) = delete;
    EditModuleCache& operator=(EditModuleCache&&) = delete;
    static EditModuleCache& Instance();
    LoadEditModuleResult LoadEditModule(const std::string& projectFilePath, const std::string& backendStr, const std::string& configurationStr);
    Module* GetEditModule(const std::string& projectFilePath, const std::string& backendStr, const std::string& configurationStr);
private:
    EditModuleCache();
    std::map<std::string, std::pair<Module*, std::string>> editModuleMap;
    std::vector<std::unique_ptr<Module>> editModules;
};

} // namespace cmajor::symbols
