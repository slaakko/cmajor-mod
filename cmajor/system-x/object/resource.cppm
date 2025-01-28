// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.resource;

import std.core;

export namespace cmajor::systemx::object {

class ResourceSection;
class BinaryFile;
class ExecutableFile;

enum ResourceType : int32_t
{
    binary = 0
};

std::string ResourceTypeStr(ResourceType type);
ResourceType ParseResourceType(const std::string& typeStr, const std::string& resourceXmlFilePath);

class Resource
{
public:
    Resource();
    Resource(const std::string& name_, ResourceType type_, const std::string& filePath_);
    std::string ToString() const;
    const std::string& Name() const { return name; }
    ResourceType Type() const { return type; }
    const std::string& FilePath() const { return filePath; }
    void Emit(ResourceSection& resourceSection);
    void Read(ResourceSection& resourceSection);
private:
    std::string name;
    ResourceType type;
    std::string filePath;
};

class ResourceInfo
{
public:
    ResourceInfo(int32_t symbolIndex_, int64_t offset_);
    int32_t SymbolIndex() const { return symbolIndex; }
    int64_t Offset() const { return offset; }
private:
    int32_t symbolIndex;
    int64_t offset;
};

std::string GenerateResourceUnit(const std::string& resourceXmlFilePath, const std::string& libraryDir);
void ProcessResources(const std::vector<std::unique_ptr<BinaryFile>>& binaryFiles, ExecutableFile& executable);

} // namespace cmsx::object
