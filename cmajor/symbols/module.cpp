// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.modules;

import cmajor.symbols.symbol.table;
import cmajor.symbols.global.flags;
import cmajor.symbols.module_cache;
import cmajor.symbols.symbol.collector;
import cmajor.symbols.classes;
import cmajor.symbols.interfaces;
import cmajor.symbols.concepts;
import cmajor.symbols.source.file.module_map;
import cmajor.ast.project;
import util;
import std.filesystem;

namespace cmajor::symbols {

class SystemModuleSet
{
public:
    static SystemModuleSet& Instance();
    bool IsSystemModule(const std::u32string& moduleName) const;
private:
    static std::unique_ptr<SystemModuleSet> instance;
    std::unordered_set<std::u32string> systemModuleNames;
    SystemModuleSet();
};

SystemModuleSet& SystemModuleSet::Instance()
{
    static SystemModuleSet instance;
    return instance;
}

SystemModuleSet::SystemModuleSet()
{
    systemModuleNames.insert(U"System.Core");
    systemModuleNames.insert(U"System.Runtime");
    systemModuleNames.insert(U"System.Base");
    systemModuleNames.insert(U"System.Lex");
    systemModuleNames.insert(U"System.Parsing");
    systemModuleNames.insert(U"System.Net.Sockets");
    systemModuleNames.insert(U"System.Net.Http");
    systemModuleNames.insert(U"System.Json");
    systemModuleNames.insert(U"System.Xml");
    systemModuleNames.insert(U"System.Dom");
    systemModuleNames.insert(U"System.Xml.Serialization");
    systemModuleNames.insert(U"System.Numerics.Multiprecision");
    systemModuleNames.insert(U"System.IO.Compression");
    systemModuleNames.insert(U"System.RegularExpressions");
    systemModuleNames.insert(U"System.XPath");
    systemModuleNames.insert(U"System.Init");
    systemModuleNames.insert(U"System.Screen");
    systemModuleNames.insert(U"System");
    systemModuleNames.insert(U"System.Install");
    systemModuleNames.insert(U"System.Windows");
    systemModuleNames.insert(U"System.Windows.Install");
}

bool SystemModuleSet::IsSystemModule(const std::u32string& moduleName) const
{
    auto it = systemModuleNames.find(moduleName);
    if (it != systemModuleNames.cend())
    {
        return true;
    }
    return false;
}

bool IsSystemModule(const std::u32string& moduleName)
{
    return SystemModuleSet::Instance().IsSystemModule(moduleName);
}

class ContainerClassTemplateMap
{
public:
    ContainerClassTemplateMap& Instance();
    // cmajor::debug::ContainerClassTemplateKind GetContainerClassTemplateKind(const std::u32string& fullClassName) const; TODO
private:
    static std::unique_ptr<ContainerClassTemplateMap> instance;
    // std::unordered_map<std::u32string, cmajor::debug::ContainerClassTemplateKind> containerClassTemplateMap; TODO
    ContainerClassTemplateMap();
};

ContainerClassTemplateMap& ContainerClassTemplateMap::Instance()
{
    static ContainerClassTemplateMap instance;
    return instance;
}

ContainerClassTemplateMap::ContainerClassTemplateMap()
{
    /* TODO
    containerClassTemplateMap[U"System.Collections.ForwardList<T>"] = cmajor::debug::ContainerClassTemplateKind::forwardList;
    containerClassTemplateMap[U"System.Collections.HashMap<K, T, H, C>"] = cmajor::debug::ContainerClassTemplateKind::hashMap;
    containerClassTemplateMap[U"System.Collections.HashSet<T, H, C>"] = cmajor::debug::ContainerClassTemplateKind::hashSet;
    containerClassTemplateMap[U"System.Collections.LinkedList<T>"] = cmajor::debug::ContainerClassTemplateKind::linkedList;
    containerClassTemplateMap[U"System.Collections.List<T>"] = cmajor::debug::ContainerClassTemplateKind::list;
    containerClassTemplateMap[U"System.Collections.Map<Key, Value, KeyCompare>"] = cmajor::debug::ContainerClassTemplateKind::map;
    containerClassTemplateMap[U"System.Collections.Queue<T>"] = cmajor::debug::ContainerClassTemplateKind::queue;
    containerClassTemplateMap[U"System.Collections.Set<T, C>"] = cmajor::debug::ContainerClassTemplateKind::set;
    containerClassTemplateMap[U"System.Collections.Stack<T>"] = cmajor::debug::ContainerClassTemplateKind::stack;
    */
}

/* TODO
cmajor::debug::ContainerClassTemplateKind ContainerClassTemplateMap::GetContainerClassTemplateKind(const std::u32string& fullClassName) const
{
    auto it = containerClassTemplateMap.find(fullClassName);
    if (it != containerClassTemplateMap.cend())
    {
        return it->second;
    }
    else
    {
        return cmajor::debug::ContainerClassTemplateKind::notContainerClassTemplate;
    }
}

cmajor::debug::ContainerClassTemplateKind GetContainerClassTemplateKind(const std::u32string& fullClassName)
{
    return ContainerClassTemplateMap::Instance().GetContainerClassTemplateKind(fullClassName);
}
*/

const char* cmajorModuleTag = "CMM";

ModuleTag::ModuleTag()
{
    bytes[0] = cmajorModuleTag[0];
    bytes[1] = cmajorModuleTag[1];
    bytes[2] = cmajorModuleTag[2];
    bytes[3] = currentModuleFormat;
}

void ModuleTag::Write(SymbolWriter& writer)
{
    writer.GetBinaryStreamWriter().Write(bytes[0]);
    writer.GetBinaryStreamWriter().Write(bytes[1]);
    writer.GetBinaryStreamWriter().Write(bytes[2]);
    writer.GetBinaryStreamWriter().Write(bytes[3]);
}

void ModuleTag::Read(SymbolReader& reader)
{
    bytes[0] = reader.GetBinaryStreamReader().ReadByte();
    bytes[1] = reader.GetBinaryStreamReader().ReadByte();
    bytes[2] = reader.GetBinaryStreamReader().ReadByte();
    bytes[3] = reader.GetBinaryStreamReader().ReadByte();
}

std::string ModuleFlagStr(ModuleFlags flags)
{
    std::string s;
    if ((flags & ModuleFlags::system) != ModuleFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("system");
    }
    if ((flags & ModuleFlags::core) != ModuleFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("core");
    }
    return s;
}

ModuleDependency::ModuleDependency(Module* module_) : module(module_)
{
}

void ModuleDependency::AddReferencedModule(Module* referencedModule)
{
    if (std::find(referencedModules.cbegin(), referencedModules.cend(), referencedModule) == referencedModules.cend())
    {
        referencedModules.push_back(referencedModule);
    }
}

void ModuleDependency::Dump(util::CodeFormatter& formatter)
{
    formatter.IncIndent();
    int n = referencedModules.size();
    for (int i = 0; i < n; ++i)
    {
        Module* referencedModule = referencedModules[i];
        formatter.WriteLine(util::ToUtf8(referencedModule->Name()));
        referencedModule->GetModuleDependency().Dump(formatter);
    }
    formatter.DecIndent();
}

int32_t FileTable::RegisterFilePath(const std::string& filePath)
{
    int32_t fileIndex = filePaths.size();
    filePaths.push_back(filePath);
    return fileIndex;
}

std::string FileTable::GetFilePath(int32_t fileIndex) const
{
    if (fileIndex >= 0 && fileIndex < filePaths.size())
    {
        return filePaths[fileIndex];
    }
    return std::string();
}

void FileTable::Write(util::BinaryStreamWriter& writer, bool systemModule)
{
    uint32_t n = filePaths.size();
    writer.WriteULEB128UInt(n);
    std::string cmajorRoot;
    if (systemModule)
    {
        cmajorRoot = util::GetFullPath(cmajor::ast::CmajorRootDir());
        if (!cmajorRoot.ends_with("/"))
        {
            cmajorRoot.append("/");
        }
    }
    for (uint32_t i = 0; i < n; ++i)
    {
        std::string filePath = util::GetFullPath(filePaths[i]);
        if (systemModule)
        {
            if (filePath.find(cmajorRoot, 0) == 0)
            {
                filePath = filePath.substr(cmajorRoot.size());
            }
        }
        writer.Write(filePath);
    }
}

void FileTable::Read(util::BinaryStreamReader& reader, bool systemModule)
{
    filePaths.clear();
    std::string cmajorRoot;
    if (systemModule)
    {
        cmajorRoot = util::GetFullPath(cmajor::ast::CmajorRootDir());
        if (!cmajorRoot.ends_with("/"))
        {
            cmajorRoot.append("/");
        }
    }
    uint32_t n = reader.ReadULEB128UInt();
    for (uint32_t i = 0; i < n; ++i)
    {
        if (systemModule)
        {
            std::string filePath = reader.ReadUtf8String();
            filePath = util::Path::Combine(cmajorRoot, filePath);
            filePaths.push_back(std::move(filePath));
        }
        else
        {
            std::string filePath = reader.ReadUtf8String();
            filePaths.push_back(std::move(filePath));
        }
    }
}

void FileTable::Dump(util::CodeFormatter& formatter)
{
    if (!IsEmpty())
    {
        formatter.WriteLine("source file paths:");
        formatter.IncIndent();
        int n = filePaths.size();
        for (int i = 0; i < n; ++i)
        {
            formatter.WriteLine(std::to_string(i) + " : " + filePaths[i]);
        }
        formatter.DecIndent();
    }
}

SourceFileCache::SourceFileCache()
{
}

const std::u32string& SourceFileCache::GetFileContent(const std::string& filePath)
{
    auto it = fileContentMap.find(filePath);
    if (it != fileContentMap.cend())
    {
        return *it->second;
    }
    else
    {
        std::unique_ptr<std::u32string> fileContent(new std::u32string(util::ToUtf32(util::ReadFile(filePath))));
        std::u32string* str = fileContent.get();
        fileContentMap[filePath] = std::move(fileContent);
        return *str;
    }
}

/* TODO
cmajor::debug::SourceSpan MakeSourceSpan(const soul::ast::SourcePos& sourcePos, const util::uuid& sourceModuleId)
{
    if (!sourcePos.Valid()) return cmajor::debug::SourceSpan();
    if (sourceModuleId.is_nil()) return cmajor::debug::SourceSpan();
    Module* module = GetModuleById(sourceModuleId);
    if (!module) return cmajor::debug::SourceSpan();
    return module->SpanToSourceSpan(sourcePos);
}
*/

void Visit(std::vector<Module*>& finishReadOrder, Module* module, std::unordered_set<Module*>& visited, std::unordered_set<Module*>& tempVisit,
    std::unordered_map<Module*, ModuleDependency*>& dependencyMap, const Module* rootModule)
{
    if (tempVisit.find(module) == tempVisit.cend())
    {
        if (visited.find(module) == visited.cend())
        {
            tempVisit.insert(module);
            auto i = dependencyMap.find(module);
            if (i != dependencyMap.cend())
            {
                ModuleDependency* dependency = i->second;
                for (Module* dependentAssembly : dependency->ReferencedModules())
                {
                    Visit(finishReadOrder, dependentAssembly, visited, tempVisit, dependencyMap, rootModule);
                }
                tempVisit.erase(module);
                visited.insert(module);
                finishReadOrder.push_back(module);
            }
            else
            {
                throw std::runtime_error("module '" + util::ToUtf8(module->Name()) + "' not found in dependencies of module '" + util::ToUtf8(rootModule->Name()) + "'");
            }
        }
    }
    else
    {
        throw std::runtime_error("circular module dependency '" + util::ToUtf8(module->Name()) + "' detected in dependencies of module '" + util::ToUtf8(rootModule->Name()) + "'");
    }
}

std::vector<Module*> CreateFinishReadOrder(std::vector<Module*>& modules, std::unordered_map<Module*, ModuleDependency*>& dependencyMap, const Module* rootModule)
{
    std::vector<Module*> finishReadOrder;
    std::unordered_set<Module*> visited;
    std::unordered_set<Module*> tempVisit;
    for (Module* module : modules)
    {
        if (visited.find(module) == visited.cend())
        {
            Visit(finishReadOrder, module, visited, tempVisit, dependencyMap, rootModule);
        }
    }
    return finishReadOrder;
}

void FinishReads(Module* rootModule, std::vector<Module*>& finishReadOrder, bool all, bool readRoot)
{
#ifdef MODULE_READING_DEBUG
    LogMessage(rootModule->LogStreamId(), "FinishReads: begin " + util::ToUtf8(rootModule->Name()), rootModule->DebugLogIndent());
    rootModule->IncDebugLogIndent();
#endif 
    int n = finishReadOrder.size() - 1;
    if (all)
    {
        n = finishReadOrder.size();
    }
    for (int i = 0; i < n; ++i)
    {
        Module* module = finishReadOrder[i];
        if (!module->HasSymbolTable() || (module == rootModule && all && readRoot))
        {
#ifdef MODULE_READING_DEBUG
            LogMessage(rootModule->LogStreamId(), "FinishReads: reading " + util::ToUtf8(module->Name()), rootModule->DebugLogIndent());
#endif 
            module->CreateSymbolTable();
            std::vector<TypeOrConceptRequest> typeAndConceptRequests;
            std::vector<FunctionRequest> functionRequests;
            std::vector<ClassTemplateSpecializationSymbol*> classTemplateSpecializations;
            std::vector<FunctionSymbol*> conversions;
            std::vector<ArrayTypeSymbol*> arrayTypes;
            std::vector<DerivedTypeSymbol*> derivedTypes;
            SymbolReader reader(module->FilePathReadFrom());
            // reader.GetAstReader().SetModuleMaps(rootModule->Id(), module->GetModuleNameTable(), rootModule->GetModuleIdMap()); TODO
            reader.SetModule(module);
            reader.SetArrayTypesTarget(&arrayTypes);
            reader.SetDerivedTypesTarget(&derivedTypes);
            reader.SetClassTemplateSpecializationTarget(&classTemplateSpecializations);
            reader.SetTypeAndConceptRequestTarget(&typeAndConceptRequests);
            reader.SetFunctionRequestTarget(&functionRequests);
            reader.SetConversionsTarget(&conversions);
            reader.GetBinaryStreamReader().GetStream().Seek(module->SymbolTablePos(), util::Origin::seekSet);
            reader.SetRootModule(rootModule);
            module->GetSymbolTable().Read(reader);
            for (Module* referencedModule : module->ReferencedModules())
            {
                module->GetSymbolTable().Import(referencedModule->GetSymbolTable());
            }
            module->GetSymbolTable().FinishRead(arrayTypes, derivedTypes, classTemplateSpecializations, typeAndConceptRequests, functionRequests, conversions);
            module->SetImmutable();
#ifdef MODULE_CHECKING
            module->Check();
#endif
            if (rootModule == module) continue;
            rootModule->GetSymbolTable().Import(module->GetSymbolTable());
        }
        else
        {
#ifdef MODULE_READING_DEBUG
            LogMessage(rootModule->LogStreamId(), "FinishReads: " + util::ToUtf8(module->Name()) + " in cache", rootModule->DebugLogIndent());
#endif 
            rootModule->GetSymbolTable().Import(module->GetSymbolTable());
        }
    }
#ifdef MODULE_READING_DEBUG
    rootModule->DecDebugLogIndent();
    LogMessage(rootModule->LogStreamId(), "FinishReads: end " + util::ToUtf8(rootModule->Name()), rootModule->DebugLogIndent());
#endif 
}

void Import(cmajor::ast::Target target, Module* rootModule, Module* module, const std::vector<std::string>& references, std::unordered_set<std::string>& importSet, std::vector<Module*>& modules,
    std::unordered_map<std::string, ModuleDependency*>& moduleDependencyMap, std::unordered_map<std::string, Module*>& readMap, bool& first)
{
#ifdef MODULE_READING_DEBUG
    LogMessage(rootModule->LogStreamId(), "Import: begin " + util::ToUtf8(module->Name()), rootModule->DebugLogIndent());
    rootModule->IncDebugLogIndent();
#endif 
    for (const std::string& reference : references)
    {
        if (importSet.find(reference) == importSet.cend())
        {
            importSet.insert(reference);
#ifdef MODULE_READING_DEBUG
            LogMessage(rootModule->LogStreamId(), "Import: reference: " + reference, rootModule->DebugLogIndent());
#endif 
            std::string config = GetConfig();
            std::filesystem::path mfn = std::filesystem::path(reference).filename();
            std::filesystem::path mfp;
            std::string searchedDirectories;
            if (!rootModule->IsSystemModule())
            {
                cmajor::ast::BackEnd backend = cmajor::ast::BackEnd::llvm;
                if (GetBackEnd() == cmajor::symbols::BackEnd::cmsx)
                {
                    backend = cmajor::ast::BackEnd::cmsx;
                }
                else if (GetBackEnd() == cmajor::symbols::BackEnd::cmcpp)
                {
                    backend = cmajor::ast::BackEnd::cppcm;
                }
                cmajor::ast::SystemDirKind systemDirKind = cmajor::ast::SystemDirKind::regular;
                if (GetGlobalFlag(GlobalFlags::repository))
                {
                    systemDirKind = cmajor::ast::SystemDirKind::repository;
                }
                mfp = cmajor::ast::CmajorSystemLibDir(config, backend, cmajor::ast::GetToolChain(), systemDirKind);
                searchedDirectories.append("\n").append(mfp.generic_string());
                mfp /= mfn;
                if (!std::filesystem::exists(mfp))
                {
                    mfp = reference;
                    if (!std::filesystem::exists(mfp))
                    {
                        std::filesystem::path mrd = mfp;
                        mrd.remove_filename();
                        searchedDirectories.append("\n").append(mrd.generic_string());
                        throw std::runtime_error("Could not find module reference '" + mfn.generic_string() + "'.\nDirectories searched:\n" + searchedDirectories);
                    }
                }
            }
            else
            {
                mfp = reference;
                if (!std::filesystem::exists(mfp))
                {
                    std::filesystem::path mrd = mfp;
                    mrd.remove_filename();
                    searchedDirectories.append("\n").append(mrd.generic_string());
                    throw std::runtime_error("Could not find module reference '" + mfn.generic_string() + "'.\nDirectories searched:\n" + searchedDirectories);
                }
            }
            std::string moduleFilePath = util::GetFullPath(mfp.generic_string());
            if (readMap.find(moduleFilePath) == readMap.cend())
            {
                Module* referencedModule = GetModuleFromModuleCache(moduleFilePath);
                if (referencedModule->GetFlag(ModuleFlags::readFromModuleFile))
                {
                    referencedModule->ResetFlag(ModuleFlags::readFromModuleFile);
                    referencedModule = ResetCachedModule(moduleFilePath);
                }
                rootModule->AllRefModules().push_back(referencedModule);
                readMap[moduleFilePath] = referencedModule;
                importSet.insert(moduleFilePath);
                SymbolReader reader(moduleFilePath);
                // reader.GetAstReader().SetModuleMaps(rootModule->Id(), referencedModule->GetModuleNameTable(), rootModule->GetModuleIdMap()); TODO
                referencedModule->ReadHeader(target, reader, rootModule, importSet, modules, moduleDependencyMap, readMap, first);
                module->AddReferencedModule(referencedModule);
                if (module != rootModule)
                {
                    module->RegisterFileTable(&referencedModule->GetFileTable(), referencedModule);
                }
                Import(target, rootModule, module, referencedModule->ReferenceFilePaths(), importSet, modules, moduleDependencyMap, readMap, first);
            }
        }
        else
        {
            std::string config = GetConfig();
            std::filesystem::path mfn = std::filesystem::path(reference).filename();
            std::filesystem::path mfp;
            std::string searchedDirectories;
            if (!rootModule->IsSystemModule())
            {
                cmajor::ast::BackEnd backend = cmajor::ast::BackEnd::llvm;
                if (GetBackEnd() == cmajor::symbols::BackEnd::cmsx)
                {
                    backend = cmajor::ast::BackEnd::cmsx;
                }
                else if (GetBackEnd() == cmajor::symbols::BackEnd::cmcpp)
                {
                    backend = cmajor::ast::BackEnd::cppcm;
                }
                cmajor::ast::SystemDirKind systemDirKind = cmajor::ast::SystemDirKind::regular;
                if (GetGlobalFlag(GlobalFlags::repository))
                {
                    systemDirKind = cmajor::ast::SystemDirKind::repository;
                }
                mfp = CmajorSystemLibDir(config, backend, cmajor::ast::GetToolChain(), systemDirKind);
                mfp /= mfn;
                if (!std::filesystem::exists(mfp))
                {
                    mfp = reference;
                    if (!std::filesystem::exists(mfp))
                    {
                        std::filesystem::path mrd = mfp;
                        mrd.remove_filename();
                        searchedDirectories.append("\n").append(mrd.generic_string());
                        throw std::runtime_error("Could not find module reference '" + mfn.generic_string() + "'.\nDirectories searched:\n" + searchedDirectories);
                    }
                }
            }
            else
            {
                mfp = reference;
                if (!std::filesystem::exists(mfp))
                {
                    std::filesystem::path mrd = mfp;
                    mrd.remove_filename();
                    searchedDirectories.append("\n").append(mrd.generic_string());
                    throw std::runtime_error("Could not find module reference '" + mfn.generic_string() + "'.\nDirectories searched:\n" + searchedDirectories);
                }
            }
            std::string moduleFilePath = util::GetFullPath(mfp.generic_string());
            auto it = readMap.find(moduleFilePath);
            if (it != readMap.cend())
            {
                Module* referencedModule = it->second;
                if (rootModule->IsSystemModule() ||
                    ((target == cmajor::ast::Target::program || target == cmajor::ast::Target::library || target == cmajor::ast::Target::unitTest) &&
                        referencedModule->Name() != U"System" ||
                        (target == cmajor::ast::Target::winguiapp || target == cmajor::ast::Target::winapp || target == cmajor::ast::Target::winlib) &&
                        referencedModule->Name() != U"System.Windows"))
                {
                    module->AddReferencedModule(referencedModule);
                    if (module != rootModule)
                    {
                        module->RegisterFileTable(&referencedModule->GetFileTable(), referencedModule);
                    }
                }
#ifdef MODULE_READING_DEBUG
                LogMessage(rootModule->LogStreamId(), "Import: " + util::ToUtf8(module->Name()) + " references " + util::ToUtf8(referencedModule->Name()), rootModule->DebugLogIndent());
#endif
            }
            else
            {
                throw std::runtime_error("module file path '" + moduleFilePath + "' not found from module read map for module '" + util::ToUtf8(rootModule->Name()) + "'");
            }
        }
    }
#ifdef MODULE_READING_DEBUG
    rootModule->DecDebugLogIndent();
    LogMessage(rootModule->LogStreamId(), "Import: end " + util::ToUtf8(module->Name()), rootModule->DebugLogIndent());
#endif 
}

void ImportModulesWithReferences(cmajor::ast::Target target,
    Module* rootModule, Module* module, const std::vector<std::string>& references, std::unordered_set<std::string>& importSet, std::vector<Module*>& modules,
    std::unordered_map<std::string, ModuleDependency*>& moduleDependencyMap, std::unordered_map<std::string, Module*>& readMap, bool& first)
{
#ifdef MODULE_READING_DEBUG
    LogMessage(rootModule->LogStreamId(), "ImportModulesWithReferences: begin " + util::ToUtf8(module->Name()), rootModule->DebugLogIndent());
    rootModule->IncDebugLogIndent();
#endif 
    std::vector<std::string> allReferences = references;
    if (!rootModule->IsSystemModule() && !GetGlobalFlag(GlobalFlags::profile))
    {
        cmajor::ast::BackEnd backend = cmajor::ast::BackEnd::llvm;
        if (GetBackEnd() == cmajor::symbols::BackEnd::cmsx)
        {
            backend = cmajor::ast::BackEnd::cmsx;
        }
        else if (GetBackEnd() == cmajor::symbols::BackEnd::cmcpp)
        {
            backend = cmajor::ast::BackEnd::cppcm;
        }
        if (first)
        {
            first = false;
            if (target == cmajor::ast::Target::winguiapp || target == cmajor::ast::Target::winapp || target == cmajor::ast::Target::winlib)
            {
                cmajor::ast::SystemDirKind systemDirKind = cmajor::ast::SystemDirKind::regular;
                if (GetGlobalFlag(GlobalFlags::repository))
                {
                    systemDirKind = cmajor::ast::SystemDirKind::repository;
                }
                allReferences.push_back(CmajorSystemWindowsModuleFilePath(GetConfig(), cmajor::ast::GetToolChain(), systemDirKind));
            }
            else
            {
                cmajor::ast::SystemDirKind systemDirKind = cmajor::ast::SystemDirKind::regular;
                if (GetGlobalFlag(GlobalFlags::repository))
                {
                    systemDirKind = cmajor::ast::SystemDirKind::repository;
                }
                allReferences.push_back(CmajorSystemModuleFilePath(GetConfig(), backend, cmajor::ast::GetToolChain(), systemDirKind));
            }
        }
    }
    Import(target, rootModule, module, allReferences, importSet, modules, moduleDependencyMap, readMap, first);
#ifdef MODULE_READING_DEBUG
    rootModule->DecDebugLogIndent();
    LogMessage(rootModule->LogStreamId(), "ImportModulesWithReferences: end " + util::ToUtf8(module->Name()), rootModule->DebugLogIndent());
#endif 
}

void ImportModules(cmajor::ast::Target target, Module* rootModule, Module* module, std::unordered_set<std::string>& importSet, std::vector<Module*>& modules,
    std::unordered_map<std::string, ModuleDependency*>& dependencyMap, std::unordered_map<std::string, Module*>& readMap, bool& first)
{
#ifdef MODULE_READING_DEBUG
    LogMessage(rootModule->LogStreamId(), "ImportModules: begin " + util::ToUtf8(module->Name()), rootModule->DebugLogIndent());
    rootModule->IncDebugLogIndent();
#endif 
    ImportModulesWithReferences(target, rootModule, module, module->ReferenceFilePaths(), importSet, modules, dependencyMap, readMap, first);
#ifdef MODULE_READING_DEBUG
    rootModule->DecDebugLogIndent();
    LogMessage(rootModule->LogStreamId(), "ImportModules: end " + util::ToUtf8(module->Name()), rootModule->DebugLogIndent());
#endif 
}

Module::Module() :
    format(currentModuleFormat), flags(ModuleFlags::none), name(), id(util::uuid::random()),
    originalFilePath(), filePathReadFrom(), referenceFilePaths(), moduleDependency(this), symbolTablePos(0),
    symbolTable(nullptr), directoryPath(), objectFileDirectoryPath(), libraryFilePaths(), moduleIdMap(), logStreamId(0), headerRead(false), systemCoreModule(nullptr), debugLogIndent(0), index(-1),
    buildStartMs(0), buildStopMs(0), preparing(false), backend(cmajor::ast::BackEnd::llvm), config(cmajor::ast::Config::debug), functionIndex(this)
{
}

Module::Module(const std::string& filePath) : Module(filePath, false)
{
}

Module::Module(const std::string& filePath, bool readRoot) :
    format(currentModuleFormat), flags(ModuleFlags::none), name(), id(util::uuid::random()),
    originalFilePath(), filePathReadFrom(), referenceFilePaths(), moduleDependency(this), symbolTablePos(0),
    symbolTable(new SymbolTable(this)), directoryPath(), objectFileDirectoryPath(), libraryFilePaths(), moduleIdMap(), logStreamId(0), headerRead(false), 
    systemCoreModule(nullptr), debugLogIndent(0),
    index(-1), buildStartMs(0), buildStopMs(0), preparing(false), backend(cmajor::ast::BackEnd::llvm), config(cmajor::ast::Config::debug), functionIndex(this)
{
    SymbolReader reader(filePath);
    ModuleTag expectedTag;
    ModuleTag tag;
    tag.Read(reader);
    for (int i = 0; i < 3; ++i)
    {
        if (tag.bytes[i] != expectedTag.bytes[i])
        {
            throw std::runtime_error("Invalid Cmajor module tag read from file '" + filePath + "', please rebuild module from sources");
        }
    }
    if (tag.bytes[3] != expectedTag.bytes[3])
    {
        throw std::runtime_error("Cmajor module format version mismatch reading from file '" + filePath +
            "': format " + std::string(1, expectedTag.bytes[3]) + " expected, format " + std::string(1, tag.bytes[3]) + " read, please rebuild module from sources");
    }
    flags = ModuleFlags(reader.GetBinaryStreamReader().ReadByte());
    name = reader.GetBinaryStreamReader().ReadUtf32String();
    reader.GetBinaryStreamReader().ReadUuid(id);
    MapModule(this);
    backend = static_cast<cmajor::ast::BackEnd>(reader.GetBinaryStreamReader().ReadSByte());
    config = static_cast<cmajor::ast::Config>(reader.GetBinaryStreamReader().ReadSByte());
    std::unordered_set<std::string> importSet;
    Module* rootModule = this;
    if (!HasRootModuleForCurrentThread())
    {
        rootModule->SetRootModule();
        SetRootModuleForCurrentThread(rootModule);
    }
    std::vector<Module*> modules;
    std::unordered_map<std::string, ModuleDependency*> moduleDependencyMap;
    std::unordered_map<std::string, Module*> readMap;
    if (SystemModuleSet::Instance().IsSystemModule(name)) SetSystemModule();
    SymbolReader reader2(filePath);
    // reader2.GetAstReader().SetModuleMaps(rootModule->Id(), this->GetModuleNameTable(), rootModule->GetModuleIdMap()); TODO
    bool first = true;
    ReadHeader(cmajor::ast::Target::library, reader2, rootModule, importSet, modules, moduleDependencyMap, readMap, first);
    moduleDependencyMap[originalFilePath] = &moduleDependency;
    std::unordered_map<Module*, ModuleDependency*> dependencyMap;
    for (const auto& p : moduleDependencyMap)
    {
        dependencyMap[p.second->GetModule()] = p.second;
    }
    std::vector<Module*> finishReadOrder = CreateFinishReadOrder(modules, dependencyMap, rootModule);
    if (!fileTable.IsEmpty())
    {
#ifdef _WIN32
        if (GetBackEnd() == BackEnd::cmsx)
        {
            libraryFilePath = util::GetFullPath(std::filesystem::path(originalFilePath).replace_extension(".a").generic_string());
        }
        else if (GetBackEnd() == BackEnd::llvm)
        {
            libraryFilePath = util::GetFullPath(std::filesystem::path(originalFilePath).replace_extension(".lib").generic_string());
        }
        else if (GetBackEnd() == BackEnd::cmcpp)
        {
/* TODO
            const Tool& libraryManagerTool = GetLibraryManagerTool(GetPlatform(), cmajor::ast::GetToolChain());
            const Configuration& configuration = GetToolConfiguration(libraryManagerTool, GetConfig());
            libraryFilePath = util::GetFullPath(
                std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(originalFilePath), configuration.outputDirectory), util::Path::GetFileName(originalFilePath))).replace_extension(
                    libraryManagerTool.outputFileExtension).generic_string());
            if (IsSystemModule() && cmajor::ast::GetToolChain() == "vs")
            {
                libraryFilePath = util::GetFullPath(std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(cmajor::ast::CmajorRootDir(),
                    "system"), "platform"), "windows"), "cpp"), cmajor::ast::GetToolChain()),
                    GetConfig()), configuration.outputDirectory), util::Path::GetFileName(originalFilePath))).replace_extension(libraryManagerTool.outputFileExtension).generic_string());
            }
*/
        }
#else
        if (GetBackEnd() == BackEnd::cmcpp)
        {
            const Tool& libraryManagerTool = GetLibraryManagerTool(GetPlatform(), cmajor::ast::GetToolChain());
            const Configuration& configuration = GetToolConfiguration(libraryManagerTool, GetConfig());
            libraryFilePath = util::GetFullPath(
                std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(originalFilePath), configuration.outputDirectory), util::Path::GetFileName(originalFilePath))).replace_extension(
                    libraryManagerTool.outputFileExtension).generic_string());
            if (IsSystemModule() && cmajor::ast::GetToolChain() == "vs")
            {
                libraryFilePath = util::GetFullPath(std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(cmajor::ast::CmajorRootDir(),
                    "system"), "platform"), "linux"), "cpp"), cmajor::ast::GetToolChain()),
                    GetConfig()), configuration.outputDirectory), util::Path::GetFileName(originalFilePath))).replace_extension(libraryManagerTool.outputFileExtension).generic_string());
            }
        }
        else
        {
            libraryFilePath = util::GetFullPath(std::filesystem::path(originalFilePath).replace_extension(".a").generic_string());
        }
#endif
    }
    for (Module* module : finishReadOrder)
    {
        if (!module->LibraryFilePath().empty() && !module->fileTable.IsEmpty())
        {
            libraryFilePaths.push_back(module->LibraryFilePath());
        }
    }
    FinishReads(rootModule, finishReadOrder, true, readRoot);
    MakeFilePathFileIndexMap();
}

Module::Module(const std::u32string& name_, const std::string& filePath_, cmajor::ast::Target target) :
    format(currentModuleFormat), flags(ModuleFlags::none), name(name_), id(util::uuid::random()),
    originalFilePath(filePath_), filePathReadFrom(), referenceFilePaths(), moduleDependency(this), symbolTablePos(0),
    symbolTable(new SymbolTable(this)), directoryPath(), objectFileDirectoryPath(), libraryFilePaths(), moduleIdMap(), logStreamId(0), headerRead(false), 
    systemCoreModule(nullptr), debugLogIndent(0),
    index(-1), buildStartMs(0), buildStopMs(0), preparing(false), backend(cmajor::ast::BackEnd::llvm), config(cmajor::ast::Config::debug), functionIndex(this)
{
    if (SystemModuleSet::Instance().IsSystemModule(name))
    {
        SetSystemModule();
    }
    if (name == U"System.Core")
    {
        SetCore();
    }
}

Module::~Module()
{
}

void Module::SetResourceFilePath(const std::string& resourceFilePath_)
{
    resourceFilePath = resourceFilePath_;
}

void Module::PrepareForCompilation(const std::vector<std::string>& references, cmajor::ast::Target target)
{
    MapModule(this);
    switch (GetBackEnd())
    {
    case BackEnd::llvm:
    {
        backend = cmajor::ast::BackEnd::llvm;
        break;
    }
    case BackEnd::cmsx:
    {
        backend = cmajor::ast::BackEnd::cmsx;
        break;
    }
    case BackEnd::cmcpp:
    {
        backend = cmajor::ast::BackEnd::cppcm;
        break;
    }
    }
    std::string configStr = GetConfig();
    if (configStr == "debug")
    {
        config = cmajor::ast::Config::debug;
    }
    else if (configStr == "release")
    {
        config = cmajor::ast::Config::release;
    }
    else if (configStr == "profile")
    {
        config = cmajor::ast::Config::profile;
    }
    else if (configStr == "trace")
    {
        config = cmajor::ast::Config::trace;
    }
    std::filesystem::path mfd = originalFilePath;
    mfd.remove_filename();
    std::filesystem::create_directories(mfd);
    SetDirectoryPath(util::GetFullPath(mfd.generic_string()));
    SetObjectFileDirectoryPath(util::GetFullPath(mfd.generic_string()));
    if (GetBackEnd() == BackEnd::cmcpp)
    {
/* TODO
        const Tool& compilerTool = GetCompilerTool(GetPlatform(), cmajor::ast::GetToolChain());
        const Configuration& configuration = GetToolConfiguration(compilerTool, GetConfig());
        SetObjectFileDirectoryPath(util::GetFullPath((mfd / configuration.outputDirectory).generic_string()));
*/
    }
    if (name == U"System.Core")
    {
        InitCoreSymbolTable(*symbolTable);
    }
    std::unordered_set<std::string> importSet;
    Module* rootModule = this;
    rootModule->RegisterFileTable(&fileTable, rootModule);
    std::unordered_map<std::string, ModuleDependency*> moduleDependencyMap;
    std::unordered_map<std::string, Module*> readMap;
    std::vector<Module*> modules;
    bool first = true;
    ImportModulesWithReferences(target, rootModule, rootModule, references, importSet, modules, moduleDependencyMap, readMap, first);
    modules.push_back(this);
    moduleDependencyMap[originalFilePath] = &moduleDependency;
    std::unordered_map<Module*, ModuleDependency*> dependencyMap;
    for (const auto& p : moduleDependencyMap)
    {
        dependencyMap[p.second->GetModule()] = p.second;
    }
    std::vector<Module*> finishReadOrder = CreateFinishReadOrder(modules, dependencyMap, rootModule);
    if (!this->fileTable.IsEmpty())
    {
#ifdef _WIN32
        if (GetBackEnd() == BackEnd::cmsx)
        {
            libraryFilePath = util::GetFullPath(std::filesystem::path(originalFilePath).replace_extension(".a").generic_string());
        }
        else if (GetBackEnd() == BackEnd::llvm)
        {
            libraryFilePath = util::GetFullPath(std::filesystem::path(originalFilePath).replace_extension(".lib").generic_string());
        }
        else if (GetBackEnd() == BackEnd::cmcpp)
        {
/* TODO
            const Tool& libraryManagerTool = GetLibraryManagerTool(GetPlatform(), cmajor::ast::GetToolChain());
            const Configuration& configuration = GetToolConfiguration(libraryManagerTool, GetConfig());
            libraryFilePath = util::GetFullPath(
                std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(originalFilePath), configuration.outputDirectory), util::Path::GetFileName(originalFilePath))).replace_extension(
                    libraryManagerTool.outputFileExtension).generic_string());
            if (IsSystemModule() && cmajor::ast::GetToolChain() == "vs")
            {
                libraryFilePath = util::GetFullPath(std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(cmajor::ast::CmajorRootDir(),
                    "system"), "platform"), "windows"), "cpp"), cmajor::ast::GetToolChain()),
                    GetConfig()), configuration.outputDirectory), util::Path::GetFileName(originalFilePath))).replace_extension(libraryManagerTool.outputFileExtension).generic_string());
            }
*/
        }
#else
        if (GetBackEnd() == BackEnd::cmcpp)
        {
            const Tool& libraryManagerTool = GetLibraryManagerTool(GetPlatform(), cmajor::ast::GetToolChain());
            const Configuration& configuration = GetToolConfiguration(libraryManagerTool, GetConfig());
            libraryFilePath = util::GetFullPath(
                std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(originalFilePath), configuration.outputDirectory), util::Path::GetFileName(originalFilePath))).replace_extension(
                    libraryManagerTool.outputFileExtension).generic_string());
            if (IsSystemModule() && cmajor::ast::GetToolChain() == "vs")
            {
                libraryFilePath = util::GetFullPath(std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(cmajor::ast::CmajorRootDir(),
                    "system"), "platform"), "linux"), "cpp"), cmajor::ast::GetToolChain()),
                    GetConfig()), configuration.outputDirectory), util::Path::GetFileName(originalFilePath))).replace_extension(libraryManagerTool.outputFileExtension).generic_string());
            }
        }
        else
        {
            libraryFilePath = util::GetFullPath(std::filesystem::path(originalFilePath).replace_extension(".a").generic_string());
        }
#endif
    }
    for (Module* module : finishReadOrder)
    {
        if (!module->LibraryFilePath().empty())
        {
            if (GetGlobalFlag(GlobalFlags::disableSystem))
            {
                if (module->IsSystemModule())
                {
                    continue;
                }
            }
            libraryFilePaths.push_back(module->LibraryFilePath());
        }
    }
    FinishReads(rootModule, finishReadOrder, false, false);
    MakeFilePathFileIndexMap();
}

void Module::CreateSymbolTable()
{
    symbolTable.reset(new SymbolTable(this));
}

void Module::RegisterFileTable(FileTable* fileTable, Module* module)
{
    if (std::find(fileTables.begin(), fileTables.end(), fileTable) != fileTables.end()) return;
    int16_t moduleId = fileTables.size();
    fileTables.push_back(fileTable);
    std::string moduleName = util::ToUtf8(module->Name());
    moduleIdMap[moduleName] = moduleId;
    moduleNameTable[moduleId] = moduleName;
}

void Module::MakeFilePathFileIndexMap()
{
    if (GetFlag(ModuleFlags::fileIndexFilePathMapBuilt)) return;
    SetFlag(ModuleFlags::fileIndexFilePathMapBuilt);
    int16_t n = fileTable.NumFilePaths();
    for (int16_t i = 0; i < n; ++i)
    {
        int32_t fileIndex = i;
        filePathFileIndexMap[fileTable.GetFilePath(i)] = fileIndex;
    }
}

/*
void Module::SetLexers(std::vector<std::unique_ptr<CmajorLexer>>&& lexers_)
{
    lexers = std::move(lexers_);
    for (const auto& lexer : lexers)
    {
        lexerVec.push_back(lexer.get());
    }
}

std::vector<soulng::lexer::Lexer*>* Module::GetLexers()
{
    return &lexerVec;
}
*/

std::string Module::GetFilePath(int32_t fileIndex) const
{
    if (fileIndex == -1)
    {
        return std::string();
    }
    return fileTable.GetFilePath(fileIndex);
}

/* TODO

std::u32string Module::GetErrorLines(const soul::ast::SourcePos& sourcePos) const
{
    if (sourcePos.file >= 0)
    {
        if (GetFlag(ModuleFlags::compiling))
        {
            if (sourcePos.file < lexers.size())
            {
                return lexers[sourcePos.fileIndex]->ErrorLines(sourcePos);
            }
        }
        else
        {
            std::string filePath = GetFilePath(sourcePos.fileIndex);
            if (filePath.empty())
            {
                return std::u32string();
            }
            std::u32string content = ToUtf32(soulng::util::ReadFile(filePath));
            return soulng::lexer::GetErrorLines(content.c_str(), content.c_str() + content.length(), sourcePos);
        }
    }
    return std::u32string();
}

void Module::GetColumns(const soul::ast::SourcePos& sourcePos, int32_t& startCol, int32_t& endCol) const
{
    if (sourcePos.fileIndex >= 0)
    {
        if (GetFlag(ModuleFlags::compiling))
        {
            if (sourcePos.fileIndex < lexers.size())
            {
                return lexers[sourcePos.fileIndex]->GetColumns(sourcePos, startCol, endCol);
            }
        }
        else
        {
            std::string filePath = GetFilePath(sourcePos.fileIndex);
            if (filePath.empty())
            {
                return;
            }
            std::u32string content = ToUtf32(soulng::util::ReadFile(filePath));
            return soulng::lexer::GetColumns(content.c_str(), content.c_str() + content.length(), sourcePos, startCol, endCol);
        }
    }
}
*/

void Module::Write(SymbolWriter& writer)
{
    ModuleTag tag;
    tag.Write(writer);
    writer.GetBinaryStreamWriter().Write(static_cast<uint8_t>(flags & ~(ModuleFlags::root | ModuleFlags::immutable | ModuleFlags::compiling | ModuleFlags::fileIndexFilePathMapBuilt |
        ModuleFlags::readFromModuleFile)));
    writer.GetBinaryStreamWriter().Write(name);
    writer.GetBinaryStreamWriter().Write(id);
    writer.GetBinaryStreamWriter().Write(static_cast<int8_t>(backend));
    writer.GetBinaryStreamWriter().Write(static_cast<int8_t>(config));
    std::string cmajorRootRelativeFilePath = cmajor::ast::MakeCmajorRootRelativeFilePath(originalFilePath);
    writer.GetBinaryStreamWriter().Write(cmajorRootRelativeFilePath);
    uint32_t nr = referencedModules.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(nr);
    for (uint32_t i = 0; i < nr; ++i)
    {
        Module* referencedModule = referencedModules[i];
        std::string cmajorRootRelativeReferenceFilePath = cmajor::ast::MakeCmajorRootRelativeFilePath(referencedModule->OriginalFilePath());
        writer.GetBinaryStreamWriter().Write(cmajorRootRelativeReferenceFilePath);
    }
    fileTable.Write(writer.GetBinaryStreamWriter(), IsSystemModule());
    int16_t nmnt = moduleNameTable.size();
    writer.GetBinaryStreamWriter().Write(nmnt);
    for (const auto& p : moduleNameTable)
    {
        writer.GetBinaryStreamWriter().Write(p.first);
        writer.GetBinaryStreamWriter().Write(p.second);
    }
    uint32_t n = compileUnitIds.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(n);
    for (const std::string& compileUnitId : compileUnitIds)
    {
        writer.GetBinaryStreamWriter().Write(compileUnitId);
    }
#ifdef _WIN32
    resourceTable.Write(writer.GetBinaryStreamWriter());
#ifdef RESOURCE_DEBUG
    int nres = resourceTable.Resources().size();
    for (int i = 0; i < nres; ++i)
    {
        const Resource& resource = resourceTable.Resources()[i];
        LogMessage(LogStreamId(), "Module.Write: " + util::ToUtf8(name) + ": resource name=" + util::ToUtf8(resource.name) + ", file=" + resource.filePath);
    }
    LogMessage(LogStreamId(), "Module.Write: " + util::ToUtf8(name) + ": " + std::to_string(nres) + " resources written", DebugLogIndent());
#endif
#endif
    uint32_t efn = exportedFunctions.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(efn);
    for (uint32_t i = 0; i < efn; ++i)
    {
        writer.GetBinaryStreamWriter().Write(exportedFunctions[i]);
    }
    uint32_t edn = exportedData.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(edn);
    for (uint32_t i = 0; i < edn; ++i)
    {
        writer.GetBinaryStreamWriter().Write(exportedData[i]);
    }
    symbolTable->Write(writer);
}

void Module::AddReferencedModule(Module* referencedModule)
{
    moduleDependency.AddReferencedModule(referencedModule);
    if (std::find(referencedModules.cbegin(), referencedModules.cend(), referencedModule) == referencedModules.cend())
    {
        referencedModules.push_back(referencedModule);
    }
}

void Module::ReadHeader(cmajor::ast::Target target, SymbolReader& reader, Module* rootModule, std::unordered_set<std::string>& importSet, std::vector<Module*>& modules,
    std::unordered_map<std::string, ModuleDependency*>& dependencyMap, std::unordered_map<std::string, Module*>& readMap, bool& first)
{
    if (headerRead)
    {
#ifdef MODULE_READING_DEBUG
        LogMessage(rootModule->LogStreamId(), "ReadHeader: cached begin " + util::ToUtf8(name), rootModule->DebugLogIndent());
        rootModule->IncDebugLogIndent();
#endif 
        rootModule->RegisterFileTable(&fileTable, this);
        if (this != rootModule)
        {
            RegisterFileTable(&fileTable, this);
        }
        for (const std::string& compileUnitId : compileUnitIds)
        {
            rootModule->allCompileUnitIds.insert(compileUnitId);
        }
#ifdef _WIN32
        int nres = resourceTable.Resources().size();
#ifdef RESOURCE_DEBUG
        LogMessage(rootModule->LogStreamId(), "ReadHeader: cached " + util::ToUtf8(name) + ": " + std::to_string(nres) + " resources", rootModule->DebugLogIndent());
#endif
        for (int i = 0; i < nres; ++i)
        {
            Resource resource = resourceTable.Resources()[i];
#ifdef RESOURCE_DEBUG
            LogMessage(rootModule->LogStreamId(), "ReadHeader: " + util::ToUtf8(name) + ": resource name=" + util::ToUtf8(resource.name) + ", file=" + resource.filePath);
#endif 
            if (!rootModule->globalResourceTable.Contains(resource.name))
            {
                rootModule->globalResourceTable.AddResource(resource);
            }
        }
#endif
        if (dependencyMap.find(originalFilePath) == dependencyMap.cend())
        {
            modules.push_back(this);
            dependencyMap[originalFilePath] = &moduleDependency;
        }
        for (const std::string& exportedFunction : exportedFunctions)
        {
            rootModule->allExportedFunctions.push_back(exportedFunction);
            for (const std::string& data : exportedData)
            {
                rootModule->allExportedData.push_back(data);
            }
        }
        ImportModules(target, rootModule, this, importSet, modules, dependencyMap, readMap, first);
#ifdef MODULE_READING_DEBUG
        rootModule->DecDebugLogIndent();
        LogMessage(rootModule->LogStreamId(), "ReadHeader: cached end " + util::ToUtf8(name), rootModule->DebugLogIndent());
#endif 
        return;
    }
    headerRead = true;
    ModuleTag expectedTag;
    ModuleTag tag;
    tag.Read(reader);
    for (int i = 0; i < 3; ++i)
    {
        if (tag.bytes[i] != expectedTag.bytes[i])
        {
            throw std::runtime_error("Invalid Cmajor module tag read from file '" + reader.GetAstReader().FileName() + "', please rebuild module from sources");
        }
    }
    if (tag.bytes[3] != expectedTag.bytes[3])
    {
        throw std::runtime_error("Cmajor module format version mismatch reading from file '" + reader.GetAstReader().FileName() +
            "': format " + std::string(1, expectedTag.bytes[3]) + " expected, format " + std::string(1, tag.bytes[3]) + " read, please rebuild module from sources");
    }
    flags = ModuleFlags(reader.GetBinaryStreamReader().ReadByte());
    name = reader.GetBinaryStreamReader().ReadUtf32String();
    bool unitTesting = GetGlobalFlag(GlobalFlags::unitTest);
    if (unitTesting && (flags & ModuleFlags::programModule) != ModuleFlags::none)
    {
        throw std::runtime_error("cmunit can test only library modules, module '" + util::ToUtf8(name) + "' is a program module");
    }
    reader.GetBinaryStreamReader().ReadUuid(id);
    MapModule(this);
    backend = static_cast<cmajor::ast::BackEnd>(reader.GetBinaryStreamReader().ReadSByte());
    config = static_cast<cmajor::ast::Config>(reader.GetBinaryStreamReader().ReadSByte());
    rootModule->RegisterFileTable(&fileTable, this);
    if (this != rootModule)
    {
        RegisterFileTable(&fileTable, this);
    }
#ifdef MODULE_READING_DEBUG
    LogMessage(rootModule->LogStreamId(), "ReadHeader: read begin " + util::ToUtf8(name), rootModule->DebugLogIndent());
    rootModule->IncDebugLogIndent();
#endif
    std::string cmajorRootRelativeFilePath = reader.GetBinaryStreamReader().ReadUtf8String();
    originalFilePath = cmajor::ast::ExpandCmajorRootRelativeFilePath(cmajorRootRelativeFilePath);
    if (dependencyMap.find(originalFilePath) == dependencyMap.cend())
    {
        modules.push_back(this);
        dependencyMap[originalFilePath] = &moduleDependency;
    }
    filePathReadFrom = util::GetFullPath(reader.GetAstReader().FileName());
    referenceFilePaths.clear();
    uint32_t nr = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0; i < nr; ++i)
    {
        std::string cmajorRootRelativeReferenceFilePath = reader.GetBinaryStreamReader().ReadUtf8String();
        std::string referenceFilePath = cmajor::ast::ExpandCmajorRootRelativeFilePath(cmajorRootRelativeReferenceFilePath);
        referenceFilePaths.push_back(referenceFilePath);
    }
    fileTable.Read(reader.GetBinaryStreamReader(), IsSystemModule());
    if (GetGlobalFlag(GlobalFlags::updateSourceFileModuleMap))
    {
        UpdateSourceFileModuleMap();
    }
    if (!fileTable.IsEmpty())
    {
#ifdef _WIN32
        if (GetBackEnd() == BackEnd::cmsx)
        {
            libraryFilePath = util::GetFullPath(std::filesystem::path(filePathReadFrom).replace_extension(".a").generic_string());
        }
        else if (GetBackEnd() == BackEnd::llvm)
        {
            libraryFilePath = util::GetFullPath(std::filesystem::path(filePathReadFrom).replace_extension(".lib").generic_string());
        }
        else if (GetBackEnd() == BackEnd::cmcpp)
        {
/*          TODO
            const Tool& libraryManagerTool = GetLibraryManagerTool(GetPlatform(), cmajor::ast::GetToolChain());
            const Configuration& configuration = GetToolConfiguration(libraryManagerTool, GetConfig());
            libraryFilePath = util::GetFullPath(
                std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(filePathReadFrom), configuration.outputDirectory), util::Path::GetFileName(filePathReadFrom))).replace_extension(
                    libraryManagerTool.outputFileExtension).generic_string());
            if (IsSystemModule() && cmajor::ast::GetToolChain() == "vs")
            {
                libraryFilePath = util::GetFullPath(std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(cmajor::ast::CmajorRootDir(),
                    "system"), "platform"), "windows"), "cpp"), cmajor::ast::GetToolChain()),
                    GetConfig()), configuration.outputDirectory), util::Path::GetFileName(filePathReadFrom))).replace_extension(libraryManagerTool.outputFileExtension).generic_string());
            }
*/
        }
#else
        if (GetBackEnd() == BackEnd::cmcpp)
        {
            const Tool& libraryManagerTool = GetLibraryManagerTool(GetPlatform(), cmajor::ast::GetToolChain());
            const Configuration& configuration = GetToolConfiguration(libraryManagerTool, GetConfig());
            libraryFilePath = util::GetFullPath(
                std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::GetDirectoryName(filePathReadFrom), configuration.outputDirectory), util::Path::GetFileName(filePathReadFrom))).replace_extension(
                    libraryManagerTool.outputFileExtension).generic_string());
            if (IsSystemModule() && cmajor::ast::GetToolChain() == "vs")
            {
                libraryFilePath = util::GetFullPath(std::filesystem::path(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(cmajor::ast::CmajorRootDir(),
                    "system"), "platform"), "linux"), "cpp"), cmajor::ast::GetToolChain()),
                    GetConfig()), configuration.outputDirectory), util::Path::GetFileName(filePathReadFrom))).replace_extension(libraryManagerTool.outputFileExtension).generic_string());
            }
        }
        else
        {
            libraryFilePath = util::GetFullPath(std::filesystem::path(filePathReadFrom).replace_extension(".a").generic_string());
        }
#endif
    }
    int16_t nmnt = reader.GetBinaryStreamReader().ReadShort();
    for (int16_t i = 0; i < nmnt; ++i)
    {
        int16_t moduleId = reader.GetBinaryStreamReader().ReadShort();
        std::string moduleName = reader.GetBinaryStreamReader().ReadUtf8String();
        moduleNameTable[moduleId] = moduleName;
    }
    uint32_t numCompileUnitIds = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0; i < numCompileUnitIds; ++i)
    {
        std::string compileUnitId = reader.GetBinaryStreamReader().ReadUtf8String();
        compileUnitIds.insert(compileUnitId);
    }
    for (const std::string& compileUnitId : compileUnitIds)
    {
        rootModule->allCompileUnitIds.insert(compileUnitId);
    }
#ifdef _WIN32
    resourceTable.Read(reader.GetBinaryStreamReader());
    int nres = resourceTable.Resources().size();
#ifdef RESOURCE_DEBUG
    LogMessage(rootModule->LogStreamId(), "ReadHeader: " + util::ToUtf8(name) + ": " + std::to_string(nres) + " resources read", rootModule->DebugLogIndent());
#endif
    for (int i = 0; i < nres; ++i)
    {
        Resource resource = resourceTable.Resources()[i];
#ifdef RESOURCE_DEBUG
        LogMessage(rootModule->LogStreamId(), "ReadHeader: " + util::ToUtf8(name) + ": resource name=" + util::ToUtf8(resource.name) + ", file=" + resource.filePath);
#endif 
        if (!rootModule->globalResourceTable.Contains(resource.name))
        {
            rootModule->globalResourceTable.AddResource(resource);
        }
    }
#endif
    exportedFunctions.clear();
    uint32_t efn = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0; i < efn; ++i)
    {
        exportedFunctions.push_back(reader.GetBinaryStreamReader().ReadUtf8String());
    }
    for (const std::string& exportedFunction : exportedFunctions)
    {
        rootModule->allExportedFunctions.push_back(exportedFunction);
    }
    exportedData.clear();
    uint32_t edn = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0; i < edn; ++i)
    {
        exportedData.push_back(reader.GetBinaryStreamReader().ReadUtf8String());
    }
    for (const std::string& data : exportedData)
    {
        rootModule->allExportedData.push_back(data);
    }
    CheckUpToDate();
    symbolTablePos = reader.GetBinaryStreamReader().Position();
    ImportModules(target, rootModule, this, importSet, modules, dependencyMap, readMap, first);
#ifdef MODULE_READING_DEBUG
    rootModule->DecDebugLogIndent();
    LogMessage(rootModule->LogStreamId(), "ReadHeader: read end " + util::ToUtf8(name), rootModule->DebugLogIndent());
#endif 
    MakeFilePathFileIndexMap();
}

void Module::SetDirectoryPath(const std::string& directoryPath_)
{
    directoryPath = directoryPath_;
}

void Module::SetObjectFileDirectoryPath(const std::string& objectFileDirectoryPath_)
{
    objectFileDirectoryPath = objectFileDirectoryPath_;
}

void Module::AddExportedFunction(const std::string& exportedFunction)
{
    exportedFunctions.push_back(exportedFunction);
}

void Module::AddExportedData(const std::string& data)
{
    std::lock_guard<std::recursive_mutex> lck(lock);
    exportedData.push_back(data);
}

void Module::Dump()
{
    util::CodeFormatter formatter(std::cout);
    formatter.WriteLine("========================");
    formatter.WriteLine("MODULE " + util::ToUtf8(name));
    formatter.WriteLine("========================");
    formatter.WriteLine();
    formatter.WriteLine("format: " + std::string(1, format));
    formatter.WriteLine("flags: " + ModuleFlagStr(flags));
    formatter.WriteLine("original file path: " + originalFilePath);
    formatter.WriteLine("file path read from: " + filePathReadFrom);
    if (!libraryFilePath.empty())
    {
        formatter.WriteLine("library file path: " + libraryFilePath);
    }
    int n = referenceFilePaths.size();
    if (n > 0)
    {
        formatter.WriteLine("reference file paths:");
        formatter.IncIndent();
        for (int i = 0; i < n; ++i)
        {
            formatter.WriteLine(referenceFilePaths[i]);
        }
        formatter.DecIndent();
    }
    fileTable.Dump(formatter);
#ifdef _WIN32
    resourceTable.Dump(formatter);
#endif
    formatter.WriteLine("module dependencies:");
    formatter.IncIndent();
    formatter.WriteLine(util::ToUtf8(Name()));
    moduleDependency.Dump(formatter);
    formatter.DecIndent();
    SymbolCollector collector;
    symbolTable->GlobalNs().Accept(&collector);
    collector.SortByFullName();
    if (!collector.BasicTypes().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("BASIC TYPES");
        for (BasicTypeSymbol* basicType : collector.BasicTypes())
        {
            formatter.WriteLine();
            basicType->Dump(formatter);
        }
    }
    if (!collector.Classes().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("CLASSES");
        for (ClassTypeSymbol* class_ : collector.Classes())
        {
            formatter.WriteLine();
            class_->Dump(formatter);
        }
    }
    if (!collector.Interfaces().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("INTERFACES");
        for (InterfaceTypeSymbol* interface : collector.Interfaces())
        {
            formatter.WriteLine();
            interface->Dump(formatter);
        }
    }
    if (!collector.Functions().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("FUNCTIONS");
        for (FunctionSymbol* function : collector.Functions())
        {
            formatter.WriteLine();
            function->Dump(formatter);
        }
    }
    if (!collector.Typedefs().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("TYPEDEFS");
        for (TypedefSymbol* typedef_ : collector.Typedefs())
        {
            formatter.WriteLine();
            typedef_->Dump(formatter);
        }
    }
    if (!collector.Concepts().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("CONCEPTS");
        for (ConceptSymbol* concept_ : collector.Concepts())
        {
            formatter.WriteLine();
            concept_->Dump(formatter);
        }
    }
    if (!collector.Delegates().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("DELEGATES");
        for (DelegateTypeSymbol* delegate_ : collector.Delegates())
        {
            formatter.WriteLine();
            delegate_->Dump(formatter);
        }
    }
    if (!collector.ClassDelegates().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("CLASS DELEGATES");
        for (ClassDelegateTypeSymbol* classDelegate : collector.ClassDelegates())
        {
            formatter.WriteLine();
            classDelegate->Dump(formatter);
        }
    }
    if (!collector.Constants().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("CONSTANTS");
        for (ConstantSymbol* constant : collector.Constants())
        {
            formatter.WriteLine();
            constant->Dump(formatter);
        }
    }
    if (!collector.EnumeratedTypes().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("ENUMERATED TYPES");
        for (EnumTypeSymbol* enumeratedType : collector.EnumeratedTypes())
        {
            formatter.WriteLine();
            enumeratedType->Dump(formatter);
        }
    }
}

void Module::CheckUpToDate()
{
    if (fileTable.IsEmpty()) return;
    std::filesystem::path libDirPath = std::filesystem::path(originalFilePath).parent_path();
    int n = fileTable.NumFilePaths();
    for (int i = 0; i < n; ++i)
    {
        std::filesystem::path sfp(fileTable.GetFilePath(i));
        if (std::filesystem::exists(sfp))
        {
            std::filesystem::path objectFilePath;
#ifdef _WIN32
            if (GetBackEnd() == BackEnd::llvm)
            {
                objectFilePath = libDirPath / sfp.filename().replace_extension(".obj");
            }
            else if (GetBackEnd() == BackEnd::cmcpp)
            {
/* TODO
                const Tool& compilerTool = GetCompilerTool(GetPlatform(), cmajor::ast::GetToolChain());
                const Configuration& configuration = GetToolConfiguration(compilerTool, GetConfig());
                std::string outputDirPath = configuration.outputDirectory;
                objectFilePath = libDirPath / outputDirPath / sfp.filename().replace_extension(compilerTool.outputFileExtension);
*/
            }
            else if (GetBackEnd() == BackEnd::cmsx)
            {
                objectFilePath = libDirPath / sfp.filename().replace_extension(".o");
            }
#else
            objectFilePath = libDirPath / sfp.filename().replace_extension(".o");
#endif
            if (std::filesystem::exists(objectFilePath))
            {
                if (std::filesystem::last_write_time(sfp) > std::filesystem::last_write_time(objectFilePath))
                {
                    Warning warning(name, "source file '" + util::GetFullPath(sfp.generic_string()) + "' is more recent than object file '" +
                        util::GetFullPath(objectFilePath.generic_string()) + "'");
                    bool found = false;
                    for (const Warning& prev : warnings.Warnings())
                    {
                        if (prev.Message() == warning.Message())
                        {
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        warnings.AddWarning(warning);
                    }
                }
            }
        }
    }
}

void Module::SetCurrentProjectName(const std::u32string& currentProjectName_)
{
    if (IsRootModule())
    {
        currentProjectName = currentProjectName_;
    }
    else
    {
        throw std::runtime_error("cannot set current project name for a nonroot module");
    }
}

std::u32string Module::GetCurrentProjectName()
{
    if (IsRootModule())
    {
        return currentProjectName;
    }
    else
    {
        throw std::runtime_error("cannot get current project name for a nonroot module");
    }
}

void Module::SetCurrentToolName(const std::u32string& currentToolName_)
{
    if (IsRootModule())
    {
        currentToolName = currentToolName_;
    }
    else
    {
        throw std::runtime_error("cannot set current tool name for a nonroot module");
    }
}

std::u32string Module::GetCurrentToolName()
{
    if (IsRootModule())
    {
        return currentToolName;
    }
    else
    {
        throw std::runtime_error("cannot get current tool name for a nonroot module");
    }
}

CompileWarningCollection& Module::WarningCollection()
{
    if (IsRootModule())
    {
        return warnings;
    }
    else
    {
        throw std::runtime_error("cannot get warnings for a nonroot module");
    }
}

void Module::ClearDefines()
{
    if (IsRootModule())
    {
        defines = GetCommandLineDefines();
    }
    else
    {
        throw std::runtime_error("cannot clear defines for a nonroot module");
    }
}

void Module::DefineSymbol(const std::u32string& symbol)
{
    if (IsRootModule())
    {
        defines.insert(symbol);
    }
    else
    {
        throw std::runtime_error("cannot define a symbol for a nonroot module");
    }
}

bool Module::IsSymbolDefined(const std::u32string& symbol)
{
    if (IsRootModule())
    {
        auto it = defines.find(symbol);
        if (it != defines.cend())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        throw std::runtime_error("cannot get defined symbol for a nonroot module");
    }
}

Module* Module::GetSystemCoreModule()
{
    if (IsCore())
    {
        return this;
    }
    if (!systemCoreModule)
    {
        for (Module* referencedModule : referencedModules)
        {
            Module* systemCore = referencedModule->GetSystemCoreModule();
            if (systemCore)
            {
                systemCoreModule = systemCore;
                break;
            }
        }
    }
    return systemCoreModule;
}

void Module::Check()
{
    symbolTable->Check();
}

void Module::StartBuild()
{
    buildStartMs = util::CurrentMs();
}

void Module::StopBuild()
{
    buildStopMs = util::CurrentMs();
}

int Module::GetBuildTimeMs()
{
    return static_cast<int>(buildStopMs - buildStartMs);
}

void Module::AddCompileUnitId(const std::string& compileUnitId)
{
    std::lock_guard<std::recursive_mutex> lck(lock);
    if (compileUnitId.empty())
    {
        throw std::runtime_error("module '" + util::ToUtf8(Name()) + "': compile unit id is empty");
    }
    compileUnitIds.insert(compileUnitId);
    allCompileUnitIds.insert(compileUnitId);
}

void Module::WriteProjectDebugInfoFile(const std::string& projectDebufInfoFilePath)
{
/*  TODO
    util::BinaryStreamWriter writer(projectDebufInfoFilePath);
    int32_t numCompileUnits = fileTable.NumFilePaths();
    cmajor::debug::WriteProjectTableHeader(writer, util::ToUtf8(name), util::Path::GetDirectoryName(originalFilePath), Id(), numCompileUnits, functionIndex.GetMainFunctionId());
    for (int32_t i = 0; i < numCompileUnits; ++i)
    {
        std::string compileUnitBaseName = util::Path::GetFileNameWithoutExtension(fileTable.GetFilePath(i));
        cmajor::debug::WriteProjectTableRecord(writer, compileUnitBaseName);
    }
    int32_t n = fileTable.NumFilePaths();
    cmajor::debug::WriteNumberOfFileIndexRecords(writer, n);
    for (int32_t i = 0; i < n; ++i)
    {
        int32_t fileIndex = i;
        std::string filePath = fileTable.GetFilePath(fileIndex);
        Assert(!filePath.empty(), "source file path is empty");
        cmajor::debug::WriteFileIndexRecord(writer, fileIndex, filePath);
    }
    functionIndex.Write(writer);
    typeIndex.Write(writer);
*/
}

void Module::WriteCmdbFile(const std::string& cmdbFilePath)
{
/*  TODO
    util::BinaryStreamWriter cmdbWriter(cmdbFilePath);
    cmajor::debug::WriteCmdbFileTag(cmdbWriter);
    std::string mainProjectName = util::ToUtf8(name);
    cmajor::debug::WriteMainProjectName(cmdbWriter, mainProjectName);
    int32_t numProjects = 0;
    uint32_t numProjectsPos = cmdbWriter.Pos();
    cmajor::debug::WriteNumberOfProjects(cmdbWriter, numProjects);
    for (Module* referencedModule : referencedModules)
    {
        referencedModule->WriteDebugInfo(cmdbWriter, numProjects, this);
    }
    WriteDebugInfo(cmdbWriter, numProjects, this);
    uint32_t currentPos = cmdbWriter.Pos();
    cmdbWriter.Seek(numProjectsPos);
    cmajor::debug::WriteNumberOfProjects(cmdbWriter, numProjects);
    cmdbWriter.Seek(currentPos);
*/
}

void Module::WriteDebugInfo(util::BinaryStreamWriter& cmdbWriter, int32_t& numProjects, Module* rootModule)
{
/*  TODO
    std::string pdiFilePath = util::Path::ChangeExtension(originalFilePath, ".pdi");
    util::BinaryStreamReader pdiReader(pdiFilePath);
    std::string projectName;
    std::string projectDirectoryPath;
    util::uuid moduleId;
    int32_t numCompileUnits;
    util::uuid mainFunctionId;
    cmajor::debug::ReadProjectTableHeader(pdiReader, projectName, projectDirectoryPath, moduleId, numCompileUnits, mainFunctionId);
    cmajor::debug::WriteProjectTableHeader(cmdbWriter, projectName, projectDirectoryPath, moduleId, numCompileUnits, mainFunctionId);
    for (int32_t i = 0; i < numCompileUnits; ++i)
    {
        std::string compileUnitBaseName;
        cmajor::debug::ReadProjectTableRecord(pdiReader, compileUnitBaseName);
        cmajor::debug::WriteProjectTableRecord(cmdbWriter, compileUnitBaseName);
        std::string cudiFilePath = util::Path::Combine(projectDirectoryPath, compileUnitBaseName + ".cudi");
        util::BinaryStreamReader cudiReader(cudiFilePath);
        int32_t numFunctionRecords;
        cmajor::debug::ReadNumberOfCompileUnitFunctionRecords(cudiReader, numFunctionRecords);
        cmajor::debug::WriteNumberOfCompileUnitFunctionRecords(cmdbWriter, numFunctionRecords);
        for (int32_t i = 0; i < numFunctionRecords; ++i)
        {
            int32_t fileIndex;
            util::uuid sourceModuleId;
            util::uuid functionId;
            cmajor::debug::ReadCompileUnitFunctionRecord(cudiReader, fileIndex, sourceModuleId, functionId);
            cmajor::debug::WriteCompileUnitFunctionRecord(cmdbWriter, fileIndex, sourceModuleId, functionId);
            int32_t numInstructionRecords;
            cmajor::debug::ReadNumberOfInstructionRecords(cudiReader, numInstructionRecords);
            cmajor::debug::WriteNumberOfInstructionRecords(cmdbWriter, numInstructionRecords);
            for (int32_t i = 0; i < numInstructionRecords; ++i)
            {
                int32_t cppLineNumber;
                cmajor::debug::SourceSpan sourcePos;
                int32_t cppLineIndex;
                int16_t scopeId;
                int16_t flags;
                cmajor::debug::ReadInstructionRecord(cudiReader, cppLineNumber, sourcePos, cppLineIndex, scopeId, flags);
                cmajor::debug::WriteInstructionRecord(cmdbWriter, cppLineNumber, sourcePos, cppLineIndex, scopeId, flags);
            }
            int32_t numScopes;
            cmajor::debug::ReadNumberOfScopes(cudiReader, numScopes);
            cmajor::debug::WriteNumberOfScopes(cmdbWriter, numScopes);
            for (int32_t i = 0; i < numScopes; ++i)
            {
                int16_t scopeId;
                int16_t parentScopeId;
                int32_t numLocalVariables;
                cmajor::debug::ReadScopeRecord(cudiReader, scopeId, parentScopeId, numLocalVariables);
                cmajor::debug::WriteScopeRecord(cmdbWriter, scopeId, parentScopeId, numLocalVariables);
                for (int32_t i = 0; i < numLocalVariables; ++i)
                {
                    cmajor::debug::DIVariable variable(cmajor::debug::DIVariable::Kind::localVariable);
                    variable.Read(cudiReader);
                    variable.Write(cmdbWriter);
                }
            }
            int32_t controlFlowGraphNodeCount;
            cmajor::debug::ReadControlFlowGraphNodeCount(cudiReader, controlFlowGraphNodeCount);
            cmajor::debug::WriteControlFlowGraphNodeCount(cmdbWriter, controlFlowGraphNodeCount);
            for (int32_t i = 0; i < controlFlowGraphNodeCount; ++i)
            {
                int32_t nodeId;
                cmajor::debug::SourceSpan sourcePos;
                int32_t cppLineIndex;
                int32_t cppLineNumber;
                cmajor::debug::ReadControlFlowGraphNode(cudiReader, nodeId, sourcePos, cppLineIndex, cppLineNumber);
                cmajor::debug::WriteControlFlowGraphNode(cmdbWriter, nodeId, sourcePos, cppLineIndex, cppLineNumber);
                int32_t edgeCount;
                cmajor::debug::ReadControlFlowGraphNodeEdgeCount(cudiReader, edgeCount);
                cmajor::debug::WriteControlFlowGraphNodeEdgeCount(cmdbWriter, edgeCount);
                for (int32_t i = 0; i < edgeCount; ++i)
                {
                    int32_t endNodeId;
                    cmajor::debug::ReadControlFlowGraphNodeEdge(cudiReader, endNodeId);
                    cmajor::debug::WriteControlFlowGraphNodeEdge(cmdbWriter, endNodeId);
                }
            }
        }
    }
    int32_t numFileIndexRecords;
    cmajor::debug::ReadNumberOfFileIndexRecords(pdiReader, numFileIndexRecords);
    cmajor::debug::WriteNumberOfFileIndexRecords(cmdbWriter, numFileIndexRecords);
    for (int32_t i = 0; i < numFileIndexRecords; ++i)
    {
        int32_t fileIndex;
        std::string sourceFilePath;
        cmajor::debug::ReadFileIndexRecord(pdiReader, fileIndex, sourceFilePath);
        cmajor::debug::WriteFileIndexRecord(cmdbWriter, fileIndex, sourceFilePath);
    }
    int32_t numFunctionIndexRecords;
    cmajor::debug::ReadNumberOfFunctionIndexFunctionRecords(pdiReader, numFunctionIndexRecords);
    cmajor::debug::WriteNumberOfFunctionIndexFunctionRecords(cmdbWriter, numFunctionIndexRecords);
    for (int32_t i = 0; i < numFunctionIndexRecords; ++i)
    {
        util::uuid functionId;
        std::string fullFunctionName;
        std::string mangledFunctionName;
        cmajor::debug::ReadFunctionIndexFunctionRecord(pdiReader, functionId, fullFunctionName, mangledFunctionName);
        cmajor::debug::WriteFunctionIndexFunctionRecord(cmdbWriter, functionId, fullFunctionName, mangledFunctionName);
    }
    int32_t numTypeIndexRecords = pdiReader.ReadInt();
    cmdbWriter.Write(numTypeIndexRecords);
    for (int32_t i = 0; i < numTypeIndexRecords; ++i)
    {
        std::unique_ptr<cmajor::debug::DIType> diType = cmajor::debug::ReadType(pdiReader, nullptr);
        cmajor::debug::WriteType(cmdbWriter, diType.get());
    }
    ++numProjects;
*/
}

/* TODO
cmajor::debug::SourceSpan Module::SpanToSourceSpan(const soul::ast::SourcePos& sourcePos)
{
    if (!sourcePos.Valid()) return cmajor::debug::SourceSpan();
    cmajor::debug::SourceSpan sourceSpan;
    sourceSpan.line = sourcePos.line;
    if (GetFlag(ModuleFlags::compiling))
    {
        CmajorLexer* lexer = lexers[sourcePos.fileIndex].get();
        int32_t startCol = 0;
        int32_t endCol = 0;
        lexer->GetColumns(sourcePos, startCol, endCol);
        sourceSpan.scol = static_cast<int16_t>(startCol);
        sourceSpan.ecol = static_cast<int16_t>(endCol);
    }
    else
    {
        std::lock_guard<std::recursive_mutex> lck(lock);
        std::string filePath = GetFilePath(sourcePos.fileIndex);
        const std::u32string& content = sourceFileCache.GetFileContent(filePath);
        int32_t startCol = 0;
        int32_t endCol = 0;
        soulng::lexer::GetColumns(content.c_str(), content.c_str() + content.length(), sourcePos, startCol, endCol);
        sourceSpan.scol = static_cast<int16_t>(startCol);
        sourceSpan.ecol = static_cast<int16_t>(endCol);
    }
    return sourceSpan;
}
*/

int32_t Module::GetFileIndexForFilePath(const std::string& filePath) const
{
    auto it = filePathFileIndexMap.find(filePath);
    if (it != filePathFileIndexMap.cend())
    {
        return it->second;
    }
    else
    {
        return -1;
    }
}

void Module::UpdateSourceFileModuleMap()
{
    int16_t n = fileTable.NumFilePaths();
    for (int16_t i = 0; i < n; ++i)
    {
        MapSourceFileToModuleId(backend, config, fileTable.GetFilePath(i), Id());
    }
}

ParseResult Module::ParseSources()
{
    if (sources)
    {
        ParseResult parseResult = sources->Parse(this);
        sources->AddSymbols(this);
        sources->GetScopes(this);
        sources->BindTypes(this);
        parseResult.numberOfErrors = sources->GetNumberOfErrors();
        parseResult.synchronized = sources->Synchronized();
        return parseResult;
    }
    else
    {
        ParseResult result;
        result.ok = false;
        result.error = "sources not set";
        return result;
    }
}

ParseResult Module::ParseSource(const std::string& sourceFilePath, const std::u32string& sourceCode)
{
    if (sources)
    {
        return sources->ParseSource(this, sourceFilePath, sourceCode);
    }
    else
    {
        ParseResult result;
        result.ok = false;
        result.error = "sources not set";
        return result;
    }
}

std::string Module::GetCCList(const std::string& sourceFilePath, const std::u32string& ccText, const std::u32string& cursorLine, const std::vector<int>& ruleContext)
{
    if (sources)
    {
        return sources->GetCCList(this, sourceFilePath, ccText, cursorLine, ruleContext);
    }
    else
    {
        throw std::runtime_error("sources not set");
    }
}

std::string Module::GetParamHelpList(const std::string& sourceFilePath, int symbolIndex)
{
    if (sources)
    {
        return sources->GetParamHelpList(this, sourceFilePath, symbolIndex);
    }
    else
    {
        throw std::runtime_error("sources not set");
    }
}

#ifdef _WIN32
__declspec(thread) Module* rootModule = nullptr;
#else
__thread Module* rootModule = nullptr;
#endif

std::string GetSourceFilePath(int32_t fileIndex, const util::uuid& moduleId)
{
    if (fileIndex == -1)
    {
        return std::string();
    }
    Module* module = rootModule;
    if (!moduleId.is_nil())
    {
        Module* m = GetModuleById(moduleId);
        if (m)
        {
            module = m;
        }
    }
    if (module)
    {
        std::string sourceFilePath = module->GetFilePath(fileIndex);
        return sourceFilePath;
    }
    else
    {
        return std::string();
    }
}

bool HasRootModuleForCurrentThread()
{
    return rootModule != nullptr;
}

Module* GetRootModuleForCurrentThread()
{
    if (!rootModule)
    {
        throw std::runtime_error("root module for the current thread not set");
    }
    return rootModule;
}

void SetRootModuleForCurrentThread(Module* rootModule_)
{
    rootModule = rootModule_;
}

class SystemModuleVersionTagVerifier : public cmajor::ast::ModuleVersionTagVerifier
{
public:
    void VerifyModuleVersionTag(const std::string& moduleFilePath) override;
};

void SystemModuleVersionTagVerifier::VerifyModuleVersionTag(const std::string& moduleFilePath)
{
    SymbolReader reader(moduleFilePath);
    ModuleTag expectedTag;
    ModuleTag tag;
    tag.Read(reader);
    for (int i = 0; i < 3; ++i)
    {
        if (tag.bytes[i] != expectedTag.bytes[i])
        {
            throw std::runtime_error("Invalid Cmajor module tag read from file '" + reader.GetAstReader().FileName() + "', please rebuild module from sources");
        }
    }
    if (tag.bytes[3] != expectedTag.bytes[3])
    {
        throw std::runtime_error("Cmajor module format version mismatch reading from file '" + reader.GetAstReader().FileName() +
            "': format " + std::string(1, expectedTag.bytes[3]) + " expected, format " + std::string(1, tag.bytes[3]) + " read, please rebuild module from sources");
    }
}

SystemModuleVersionTagVerifier verifier;

struct InitModule
{
    InitModule();
};

InitModule::InitModule()
{
    cmajor::ast::SetModuleVersionTagVerifier(&verifier);
}

InitModule initModule;

} // namespace cmajor::symbols