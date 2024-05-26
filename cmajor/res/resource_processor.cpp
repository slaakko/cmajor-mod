// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.resource.processor;

import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
//import cmajor.llvm.rc;
import util;

namespace cmajor::resources {

class ResourceTypeNameRegistry
{
public:
    static ResourceTypeNameRegistry& Instance();
    cmajor::symbols::Resource::Type GetResourceType(const std::string& resourceTypeName) const;
    const std::string& GetResourceTypeName(cmajor::symbols::Resource::Type resourceType) const;
private:
    static std::unique_ptr<ResourceTypeNameRegistry> instance;
    ResourceTypeNameRegistry();
    std::map<std::string, cmajor::symbols::Resource::Type> resourceTypeMap;
    std::vector<std::string> resourceTypeNames;
};

ResourceTypeNameRegistry& ResourceTypeNameRegistry::Instance()
{
    static ResourceTypeNameRegistry instance;
    return instance;
}

ResourceTypeNameRegistry::ResourceTypeNameRegistry() 
{
    resourceTypeMap["cursor"] = cmajor::symbols::Resource::Type::cursor;
    resourceTypeMap["icon"] = cmajor::symbols::Resource::Type::icon;
    resourceTypeMap["bitmap"] = cmajor::symbols::Resource::Type::bitmap;
    resourceTypeMap["rcdata"] = cmajor::symbols::Resource::Type::rcData;
    resourceTypeNames.push_back("CURSOR");
    resourceTypeNames.push_back("ICON");
    resourceTypeNames.push_back("BITMAP");
    resourceTypeNames.push_back("RCDATA");
}

cmajor::symbols::Resource::Type ResourceTypeNameRegistry::GetResourceType(const std::string& resourceTypeName) const
{
    auto it = resourceTypeMap.find(resourceTypeName);
    if (it != resourceTypeMap.cend())
    {
        return it->second;
    }
    throw std::runtime_error("resource type name '" + util::ToUtf8(resourceTypeName) + "' not found from resource type name registry");
}

const std::string& ResourceTypeNameRegistry::GetResourceTypeName(cmajor::symbols::Resource::Type resourceType) const
{
    int resourceTypeIndex = static_cast<int>(resourceType);
    if (resourceTypeIndex >= resourceTypeNames.size())
    {
        throw std::runtime_error("internal error in resource processor");
    }
    return resourceTypeNames[resourceTypeIndex];
}

cmajor::symbols::Resource::Type GetResourceType(const std::string& resourceTypeName)
{
    return ResourceTypeNameRegistry::Instance().GetResourceType(resourceTypeName);
}

const std::string& GetResourceTypeName(cmajor::symbols::Resource::Type resourceType)
{
    return ResourceTypeNameRegistry::Instance().GetResourceTypeName(resourceType);
}

void AddResourcesInProjectToCurrentModule(cmajor::ast::Project* project, cmajor::symbols::Module* module)
{
    std::string cmajorResourceDir = cmajor::ast::CmajorResourceDir();
    int n = project->ResourceFilePaths().size();
    for (int i = 0; i < n; ++i)
    {
        const std::string& relativeResourceFilePath = project->RelativeResourceFilePaths()[i];
        const std::string& resourceXmlFilePath = project->ResourceFilePaths()[i];
        if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
        {
            util::LogMessage(module->LogStreamId(), "> " + relativeResourceFilePath);
        }
        std::unique_ptr<soul::xml::Document> resourceXmlDoc = soul::xml::ParseXmlFile(resourceXmlFilePath);
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/resources/resource", resourceXmlDoc.get());
        int n = nodeSet->Count();
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Node* node = nodeSet->GetNode(i);
            if (node->IsElementNode())
            {
                soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                std::string resourceName = element->GetAttribute("name");
                if (resourceName.empty())
                {
                    throw std::runtime_error(std::to_string(i) + "'th resource element has no name attribute in resource XML file '" + 
                        resourceXmlFilePath + "' in project '" + project->FilePath() + "'.");
                }
                if (module->GetResourceTable().Contains(resourceName))
                {
                    throw std::runtime_error("Resource table of module '" + util::ToUtf8(module->Name()) + " (" + 
                        module->OriginalFilePath() + ") already contains resource name '" + util::ToUtf8(resourceName) +
                        ". Detected when processing resource XML file '" + resourceXmlFilePath + "' in project '" + project->FilePath() + "'.");
                }
                if (module->GetGlobalResourceTable().Contains(resourceName))
                {
                    throw std::runtime_error(std::to_string(i) + "'th resource name '" + util::ToUtf8(resourceName) + 
                        "' not globally unique. Detected when processing resource XML file '" + resourceXmlFilePath + "' in project '" + project->FilePath() + "'.");
                }
                std::string resourceType = element->GetAttribute("type");
                if (resourceType.empty())
                {
                    throw std::runtime_error(std::to_string(i) + "'th resource element has no type attribute in resource XML file '" + resourceXmlFilePath + "' in project '" + 
                        project->FilePath() + "'.");
                }
                std::string resourceFile = element->GetAttribute("file");
                if (resourceFile.empty())
                {
                    throw std::runtime_error(std::to_string(i) + "'th resource element has no file attribute in resource XML file '" + resourceXmlFilePath + 
                        "' in project '" + project->FilePath() + "'.");
                }
                std::string resourceFilePath = util::Path::MakeCanonical(resourceFile);
                std::string fullResourceFilePath = resourceFilePath;
                if (util::Path::IsRelative(resourceFilePath))
                {
                    fullResourceFilePath = util::Path::Combine(cmajorResourceDir, resourceFilePath);
                }
                fullResourceFilePath = util::GetFullPath(fullResourceFilePath);
                if (!std::filesystem::exists(fullResourceFilePath))
                {
                    fullResourceFilePath = util::GetFullPath(util::Path::Combine(project->SourceBasePath().generic_string(), resourceFilePath));
                }
                if (std::filesystem::exists(fullResourceFilePath))
                {
                    cmajor::symbols::Resource resource(resourceName, GetResourceType(resourceType), fullResourceFilePath);
                    module->GetResourceTable().AddResource(resource);
                    module->GetGlobalResourceTable().AddResource(resource);
                }
                else
                {
                    throw std::runtime_error("resource file '" + util::ToUtf8(resourceFile) + "' not found when processing resource XML file '" + 
                        resourceFilePath + "' in project '" + project->FilePath() + "'.");
                }
            }
        }
    }
}

void CreateResourceScriptFile(cmajor::symbols::Module* module, const std::string& resourceScriptFileName)
{
    std::ofstream resourceScriptFile(resourceScriptFileName);
    util::CodeFormatter formatter(resourceScriptFile);
    for (const cmajor::symbols::Resource& resource : module->GetGlobalResourceTable().Resources())
    {
        std::string line;
        line.append(util::ToUtf8(resource.name)).append(1, ' ');
        line.append(GetResourceTypeName(resource.type)).append(1, ' ');
        line.append(1, '"').append(resource.filePath).append(1, '"');
        formatter.WriteLine(line);
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(module->LogStreamId(), "==> " + resourceScriptFileName);
    }
}

void CompileResourceScriptFile(cmajor::symbols::Module* module, const std::string& resourceScriptFileName, cmajor::ast::BackEnd backend)
{
    if (module->GetGlobalResourceTable().Resources().empty()) return;
    std::string resourceFilePath;
    std::string commandLine;
    std::string errors;
    if (backend == cmajor::ast::BackEnd::llvm)
    {
/*      TODO make it work
        resourceFilePath = util::Path::ChangeExtension(module->LibraryFilePath(), ".res");
        commandLine.append("llvm-rc /V /FO ").append(util::QuotedPath(resourceFilePath));
        commandLine.append(1, ' ').append(util::QuotedPath(resourceScriptFileName));
        cmajor::llvm::ResourceCompile(resourceScriptFileName, resourceFilePath); todo removed

*/
    }
    else if (backend == cmajor::ast::BackEnd::cpp)
    {
        resourceFilePath = util::Path::ChangeExtension(module->LibraryFilePath(), ".res.o");
        commandLine.append("windres --verbose ").append(util::QuotedPath(resourceScriptFileName)).append(1, ' ').append(util::QuotedPath(resourceFilePath));
        try
        {
            util::Process::Redirections redirections = util::Process::Redirections::processStdErr;
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                redirections = redirections | util::Process::Redirections::processStdOut;
            }
            util::Process process(commandLine, redirections);
            if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
            {
                while (!process.Eof(util::Process::StdHandle::stdOut))
                {
                    std::string line = process.ReadLine(util::Process::StdHandle::stdOut);
                    if (!line.empty())
                    {
                        util::LogMessage(-1, line);
                    }
                }
            }
            errors = process.ReadToEnd(util::Process::StdHandle::stdErr);
            process.WaitForExit();
            int exitCode = process.ExitCode();
            if (exitCode != 0)
            {
                throw std::runtime_error("executing '" + commandLine + "' failed with exit code: " + std::to_string(exitCode));
            }
        }
        catch (const std::exception& ex)
        {
            throw std::runtime_error("compiling resource script '" + resourceScriptFileName + "' failed: " + ex.what() + ":\nerrors:\n" + errors);
        }
    }
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        util::LogMessage(module->LogStreamId(), "==> " + resourceFilePath);
    }
    module->AddResourceFilePath(resourceFilePath);
}

void ProcessResourcesInProject(cmajor::ast::Project* project, cmajor::symbols::Module* module)
{
    switch (project->GetBackEnd())
    {
        case cmajor::ast::BackEnd::systemx: return;
        case cmajor::ast::BackEnd::masm: return;
    }
    AddResourcesInProjectToCurrentModule(project, module);
    if (project->GetTarget() == cmajor::ast::Target::program || project->GetTarget() == cmajor::ast::Target::winapp || project->GetTarget() == cmajor::ast::Target::winguiapp)
    {
        std::string resourceScriptFileName = util::Path::ChangeExtension(project->ModuleFilePath(), ".rc");
        CreateResourceScriptFile(module, resourceScriptFileName);
        CompileResourceScriptFile(module, resourceScriptFileName, project->GetBackEnd());
    }
}

} // namespace cmajor::cmpm
