// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.resource.table;

import cmajor.ast.project;
import util;

namespace cmajor::symbols {

std::string ResourceTypeStr(Resource::Type resourceType)
{
    switch (resourceType)
    {
        case Resource::Type::bitmap: return "bitmap";
        case Resource::Type::icon: return "icon";
        case Resource::Type::cursor: return "cursor";
        case Resource::Type::rcData: return "rcdata";
    }
    return std::string();
}

std::string MakeCmajorRootDirRelativeFilePath(const std::string& cmajorRootDir, const std::string& filePath)
{
    if (filePath.find(cmajorRootDir, 0) == 0)
    {
        return "$CMAJOR$" + filePath.substr(cmajorRootDir.size());
    }
    return filePath;
}

std::string MakeFullPathFromCmajorRootDirRelativeFilePath(const std::string& cmajorRootDir, const std::string& filePath)
{
    if (filePath.find("$CMAJOR$") == 0)
    {
        return util::Path::Combine(cmajorRootDir, filePath.substr(8));
    }
    return filePath;
}

Resource::Resource() : name(), type(), filePath()
{
}

Resource::Resource(const std::string& name_, Type type_, const std::string& filePath_) : name(name_), type(type_), filePath(util::GetFullPath(filePath_))
{
}

void Resource::Write(util::BinaryStreamWriter& writer, const std::string& cmajorRootDir)
{
    writer.Write(name);
    writer.Write(static_cast<int32_t>(type));
    writer.Write(MakeCmajorRootDirRelativeFilePath(cmajorRootDir, filePath));
}

void Resource::Read(util::BinaryStreamReader& reader, const std::string& cmajorRootDir)
{
    name = reader.ReadUtf8String();
    type = static_cast<Type>(reader.ReadInt());
    filePath = reader.ReadUtf8String();
    filePath = MakeFullPathFromCmajorRootDirRelativeFilePath(cmajorRootDir, filePath);
}

void Resource::Dump(util::CodeFormatter& formatter, int index)
{
    formatter.WriteLine("RESOURCE " + std::to_string(index));
    formatter.WriteLine("resource name: " + util::ToUtf8(name));
    formatter.WriteLine("resource type: " + ResourceTypeStr(type));
    formatter.WriteLine("resource file: " + filePath);
}

bool ResourceTable::Contains(const std::string& resourceName) const
{
    return resourceNameSet.find(resourceName) != resourceNameSet.cend();
}

void ResourceTable::AddResource(const Resource& resource)
{
    resourceNameSet.insert(resource.name);
    resources.push_back(resource);
}

void ResourceTable::Write(util::BinaryStreamWriter& writer)
{
    int32_t n = resources.size();
    writer.Write(n);
    if (n > 0)
    {
        std::string cmajorRootDir = util::GetFullPath(cmajor::ast::CmajorRootDir());
        if (!cmajorRootDir.ends_with("/"))
        {
            cmajorRootDir.append("/");
        }
        for (int32_t i = 0; i < n; ++i)
        {
            resources[i].Write(writer, cmajorRootDir);
        }
    }
}

void ResourceTable::Read(util::BinaryStreamReader& reader)
{
    int32_t n = reader.ReadInt();
    if (n > 0)
    {
        std::string cmajorRootDir = util::GetFullPath(cmajor::ast::CmajorRootDir());
        if (!cmajorRootDir.ends_with("/"))
        {
            cmajorRootDir.append("/");
        }
        for (int32_t i = 0; i < n; ++i)
        {
            Resource resource;
            resource.Read(reader, cmajorRootDir);
            resources.push_back(resource);
        }
    }
}

void ResourceTable::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine("RESOURCE TABLE:");
    int index = 0;
    for (Resource& resource : resources)
    {
        resource.Dump(formatter, index);
        ++index;
    }
}

} // namespace cmajor::symbols
