// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.source.file.module_map;

import cmajor.symbols.module_cache;

namespace cmajor::symbols {

struct SourceFileModuleMapKey
{
    SourceFileModuleMapKey(cmajor::ast::BackEnd backend_, cmajor::ast::Config config_, const std::string& sourceFilePath_) : backend(backend_), config(config_), sourceFilePath(sourceFilePath_) {}
    cmajor::ast::BackEnd backend;
    cmajor::ast::Config config;
    std::string sourceFilePath;
};

bool operator==(const SourceFileModuleMapKey& left, const SourceFileModuleMapKey& right)
{
    return left.backend == right.backend && left.config == right.config && left.sourceFilePath == right.sourceFilePath;
}

bool operator<(const SourceFileModuleMapKey& left, const SourceFileModuleMapKey& right)
{
    if (left.backend < right.backend) return true;
    if (left.backend > right.backend) return false;
    if (left.config < right.config) return true;
    if (left.config > right.config) return false;
    return left.sourceFilePath < right.sourceFilePath;
}

class SourceFileModuleMap
{
public:
    static SourceFileModuleMap& Instance();
    void SetModuleId(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath, const util::uuid& moduleId);
    util::uuid GetModuleId(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath) const;
private:
    static std::unique_ptr<SourceFileModuleMap> instance;
    std::map<SourceFileModuleMapKey, util::uuid> sourceFileModuleMap;
    SourceFileModuleMap();
};

SourceFileModuleMap& SourceFileModuleMap::Instance()
{
    static SourceFileModuleMap instance;
    return instance;
}

SourceFileModuleMap::SourceFileModuleMap()
{
}

void SourceFileModuleMap::SetModuleId(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath, const util::uuid& moduleId)
{
    SourceFileModuleMapKey key(backend, config, sourceFilePath);
    sourceFileModuleMap[key] = moduleId;
}

util::uuid SourceFileModuleMap::GetModuleId(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath) const
{
    SourceFileModuleMapKey key(backend, config, sourceFilePath);
    auto it = sourceFileModuleMap.find(key);
    if (it != sourceFileModuleMap.cend())
    {
        return it->second;
    }
    else
    {
        return util::nil_uuid();
    }
}

void MapSourceFileToModuleId(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath, const util::uuid& moduleId)
{
    SourceFileModuleMap::Instance().SetModuleId(backend, config, sourceFilePath, moduleId);
}

Module* GetModuleBySourceFile(cmajor::ast::BackEnd backend, cmajor::ast::Config config, const std::string& sourceFilePath)
{
    util::uuid moduleId = SourceFileModuleMap::Instance().GetModuleId(backend, config, sourceFilePath);
    if (moduleId.is_nil())
    {
        return nullptr;
    }
    else
    {
        return GetModuleById(moduleId);
    }
}

} // namespace cmajor::symbols;
