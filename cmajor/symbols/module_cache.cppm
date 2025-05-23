// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.module_cache;

import std.core;
import soul.ast.span;
import util.uuid;
import cmajor.ast;

export namespace cmajor::symbols {

class Context;
class Module;

class ModuleCache
{
public:
    ModuleCache();
    ModuleCache(const ModuleCache&) = delete;
    ModuleCache(ModuleCache&&) = delete;
    ModuleCache& operator=(const ModuleCache&) = delete;
    ModuleCache& operator=(ModuleCache&&) = delete;
    static std::unique_ptr<ModuleCache> Release();
    static void Init();
    static void Done();
    static ModuleCache& Instance() { return *instance; }
    Module* GetModule(const std::string& moduleFilePath);
    Module* GetModule(const util::uuid& moduleId) const;
    void MapModule(Module* module);
    Module* GetCachedModule(const std::string& moduleFilePath) const;
    void PutModule(std::unique_ptr<Module>&& module);
    Module* ResetModule(const std::string& moduleFilePath);
    void ResetCacheEntries(Module* module);
    void RemoveModule(const std::string& moduleFilePath);
    void Restore(ModuleCache* prevCache);
    bool HasModule(const std::string& moduleFilePath) const;
    void SetModule(const std::string& moduleFilePath, std::unique_ptr<Module>&& module);
    void MoveNonSystemModulesTo(ModuleCache* cache);
    void Update();
private:
    static std::unique_ptr<ModuleCache> instance;
    std::unordered_map<std::string, int> moduleMap;
    std::vector<std::unique_ptr<Module>> modules;
    std::unordered_map<util::uuid, Module*, util::UuidHash> moduleIdMap;
    void CollectModuleIndices(Module* module, std::unordered_set<int>& moduleIndeces);
};

void PrepareModuleForCompilation(Context* context, const std::vector<std::string>& references, cmajor::ast::Target target, const soul::ast::Span& rootSpan, int32_t rootFileIndex,
    cmajor::ast::CompileUnitNode* rootCompileUnit);
Module* GetModuleFromModuleCache(const std::string& moduleFilePath);
void PutModuleToModuleCache(std::unique_ptr<Module>&& module);
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
void SetCacheModule(const std::string& moduleFilePath, std::unique_ptr<Module>&& module);
void RemoveModuleFromCache(const std::string& moduleFilePath);
void MoveNonSystemModulesTo(std::unique_ptr<ModuleCache>& cachePtr);
Module* GetModuleById(const util::uuid& moduleId);
void MapModule(Module* module);
void UpdateModuleCache();

} // namespace cmajor::symbols
