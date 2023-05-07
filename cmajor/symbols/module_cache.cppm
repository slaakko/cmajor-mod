// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.module_cache;

import cmajor.ast.project;
import std.core;
import util.uuid;

export namespace cmajor::symbols {

class Module;

class ModuleCache
{
public:
    ModuleCache();
    ModuleCache(const ModuleCache&) = delete;
    ModuleCache(ModuleCache&&) = delete;
    ModuleCache& operator=(const ModuleCache&) = delete;
    ModuleCache& operator=(ModuleCache&&) = delete;
    static void Init();
    static void Done();
    static std::unique_ptr<ModuleCache> Release();
    static ModuleCache& Instance() { return *instance; }
    Module* GetModule(const std::string& moduleFilePath);
    Module* GetModule(const util::uuid& moduleId) const;
    void MapModule(Module* module_);
    Module* GetCachedModule(const std::string& moduleFilePath) const;
    void PutModule(std::unique_ptr<Module>&& module_);
    Module* ResetModule(const std::string& moduleFilePath);
    void ResetCacheEntries(Module* module_);
    void RemoveModule(const std::string& moduleFilePath);
    void Restore(ModuleCache* prevCache);
    bool HasModule(const std::string& moduleFilePath) const;
    void SetModule(const std::string& moduleFilePath, std::unique_ptr<Module>&& module_);
    void MoveNonSystemModulesTo(ModuleCache* cache);
    void Update();
private:
    static std::unique_ptr<ModuleCache> instance;
    std::unordered_map<std::string, int> moduleMap;
    std::vector<std::unique_ptr<Module>> modules;
    std::map<util::uuid, Module*> moduleIdMap;
    void CollectModuleIndices(Module* module_, std::unordered_set<int>& moduleIndeces);
};

void PrepareModuleForCompilation(Module* rootModule, const std::vector<std::string>& references, cmajor::ast::Target target);
Module* GetModuleFromModuleCache(const std::string& moduleFilePath);
void PutModuleToModuleCache(std::unique_ptr<Module>&& module_);
Module* ResetCachedModule(const std::string& moduleFilePath);
void InitModuleCache();
void DoneModuleCache();
void ResetModuleCache();
void SetUseModuleCache(bool useModuleCache_);
bool UseModuleCache();
std::unique_ptr<ModuleCache> ReleaseModuleCache();
void RestoreModulesFrom(ModuleCache* prevCache);
bool IsModuleCached(const std::string& moduleFilePath);
Module* GetCachedModule(const std::string& moduleFilePath);
void SetCacheModule(const std::string& moduleFilePath, std::unique_ptr<Module>&& module_);
void RemoveModuleFromCache(const std::string& moduleFilePath);
void MoveNonSystemModulesTo(std::unique_ptr<ModuleCache>& cachePtr);
Module* GetModuleById(const util::uuid& moduleId);
void MapModule(Module* module_);
void UpdateModuleCache();

} // namespace cmajor::symbols
