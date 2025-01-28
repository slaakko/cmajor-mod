// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.object.resource;

import cmajor.systemx.object.binary.file;
import soul.xml.xpath;
import soul.xml.dom;
import soul.xml.dom.parser;
import util;
import std.filesystem;

namespace cmajor::systemx::object {

std::string ResourceTypeStr(ResourceType type)
{
    switch (type)
    {
    case ResourceType::binary:
    {
        return "binary";
    }
    default:
    {
        return std::string();
    }
    }
}

ResourceType ParseResourceType(const std::string& typeStr, const std::string& resourceXmlFilePath)
{
    if (typeStr == "binary")
    {
        return ResourceType::binary;
    }
    else
    {
        throw std::runtime_error("unknown resource type '" + typeStr + "' in resource file '" + resourceXmlFilePath + "'");
    }
}

Resource::Resource() : name(), type(), filePath()
{
}

Resource::Resource(const std::string& name_, ResourceType type_, const std::string& filePath_) : name(name_), type(type_), filePath(filePath_)
{
}

std::string Resource::ToString() const
{
    std::string str = "Resource";
    str.append("(").append("name=").append(name).append(", type=").append(ResourceTypeStr(type)).append(", filePath=").append(filePath).append(")");
    return str;
}

void Resource::Emit(ResourceSection& resourceSection)
{
    resourceSection.EmitTetra(static_cast<int32_t>(type));
    resourceSection.EmitString(name);
    resourceSection.EmitString(filePath);
}

void Resource::Read(ResourceSection& resourceSection)
{
    type = static_cast<ResourceType>(resourceSection.ReadTetra());
    name = resourceSection.ReadString();
    filePath = resourceSection.ReadString();
}

ResourceInfo::ResourceInfo(int32_t symbolIndex_, int64_t offset_) : symbolIndex(symbolIndex_), offset(offset_)
{
}

std::string GenerateResourceUnit(const std::string& resourceXmlFilePath, const std::string& libraryDir)
{
    std::string objectFilePath = util::Path::Combine(libraryDir, util::Path::GetFileName(resourceXmlFilePath) + ".o");
    std::unique_ptr<soul::xml::Document> resourceDoc = soul::xml::ParseXmlFile(resourceXmlFilePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/resources/resource", resourceDoc.get());
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string nameAttr = element->GetAttribute("name");
            if (nameAttr.empty())
            {
                throw std::runtime_error("'name' attribute of resource not set in resource file '" + resourceXmlFilePath + "'");
            }
            std::string resourceName = nameAttr;
            std::string typeAttr = element->GetAttribute("type");
            if (typeAttr.empty())
            {
                throw std::runtime_error("'type' attribute of resource not set in resource file '" + resourceXmlFilePath + "'");
            }
            ResourceType resourceType = ParseResourceType(typeAttr, resourceXmlFilePath);
            std::string fileAttr = element->GetAttribute("file");
            if (fileAttr.empty())
            {
                throw std::runtime_error("'file' attribute of resource not set in resource file '" + resourceXmlFilePath + "'");
            }
            std::string filePath = util::Path::Combine(util::Path::GetDirectoryName(resourceXmlFilePath), fileAttr);
            std::unique_ptr<Resource> resource(new Resource(resourceName, resourceType, filePath));
            std::unique_ptr<ObjectFile> objectFile(new ObjectFile(objectFilePath));
            objectFile->CreateSections();
            objectFile->GetResourceSection()->AddResource(resource.release());
            objectFile->Finalize();
            objectFile->WriteFile();
        }
    }
    return objectFilePath;
}

void ProcessBinaryResource(Resource* resource, ObjectFile* objectFile, ExecutableFile& executable)
{
    const std::string& resourceFilePath = resource->FilePath();
    int64_t length = std::filesystem::file_size(resourceFilePath);
    std::unique_ptr<Symbol> resourceSymbol(new Symbol(SymbolKind::global, "", "@resource." + resource->Name()));
    resourceSymbol->SetLength(length);
    util::FileStream fileStream(resourceFilePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream bufferedStream(fileStream);
    util::BinaryStreamReader reader(bufferedStream);
    int64_t offset = executable.GetResourceSection()->Address();
    for (int64_t i = 0; i < length; ++i)
    {
        uint8_t x = reader.ReadByte();
        executable.GetResourceSection()->EmitByte(x);
    }
    Symbol* rs = resourceSymbol.get();
    executable.GetSymbolTable().AddSymbol(resourceSymbol.release());
    ResourceInfo resourceInfo(rs->Index(), offset);
    executable.GetResourceSection()->AddResourceInfo(resourceInfo);
}

void ProcessResource(Resource* resource, ObjectFile* objectFile, ExecutableFile& executable)
{
    switch (resource->Type())
    {
    case ResourceType::binary:
    {
        ProcessBinaryResource(resource, objectFile, executable);
        break;
    }
    default:
    {
        throw std::runtime_error("unknown resource type in resource section of object file '" + objectFile->FilePath() + "'");
    }
    }
}

void ProcessResourceInObjectFile(ObjectFile* objectFile, ExecutableFile& executable)
{
    ResourceSection* resourceSection = objectFile->GetResourceSection();
    if (resourceSection)
    {
        for (const auto& resource : resourceSection->Resources())
        {
            ProcessResource(resource.get(), objectFile, executable);
        }
    }
}

void ProcessResources(const std::vector<std::unique_ptr<BinaryFile>>& binaryFiles, ExecutableFile& executable)
{
    for (const auto& binaryFile : binaryFiles)
    {
        switch (binaryFile->Kind())
        {
        case BinaryFileKind::objectFile:
        {
            ObjectFile* objectFile = static_cast<ObjectFile*>(binaryFile.get());
            ProcessResourceInObjectFile(objectFile, executable);
            break;
        }
        case BinaryFileKind::archiveFile:
        {
            ArchiveFile* archiveFile = static_cast<ArchiveFile*>(binaryFile.get());
            for (const auto& objectFile : archiveFile->ObjectFiles())
            {
                ProcessResourceInObjectFile(objectFile.get(), executable);
            }
            break;
        }
        }
    }
}

} // namespace cmajor::systemx::object
