// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.resource.table;

import util.binary.stream.reader;
import util.binary.stream.writer;
import util.code.formatter;
import std.core;

export namespace cmajor::symbols {

struct Resource
{
    enum class Type : int
    {
        cursor = 0, icon = 1, bitmap = 2, rcData = 3
    };
    Resource();
    Resource(const std::string& name_, Type type_, const std::string& filePath_);
    void Write(util::BinaryStreamWriter& writer, const std::string& cmajorRootDir);
    void Read(util::BinaryStreamReader& reader, const std::string& cmajorRootDir);
    void Dump(util::CodeFormatter& formatter, int index);
    std::string name;
    Type type;
    std::string filePath;
};

std::string ResourceTypeStr(Resource::Type resourceType);

class ResourceTable
{
public:
    bool Contains(const std::string& resourceName) const;
    void AddResource(const Resource& resource);
    const std::vector<Resource>& Resources() const { return resources; }
    void Write(util::BinaryStreamWriter& writer);
    void Read(util::BinaryStreamReader& reader);
    void Dump(util::CodeFormatter& formatter);
private:
    std::vector<Resource> resources;
    std::set<std::string> resourceNameSet;
};

} // namespace cmajor::symbols

