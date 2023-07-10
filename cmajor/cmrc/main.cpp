// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import util;
import std.core;

void InitApplication()
{
    util::Init();
}

struct Resource
{
    Resource(const std::string& name_, const std::string& type_, const std::string& file_);
    std::string name;
    std::string type;
    std::string file;
};

Resource::Resource(const std::string& name_, const std::string& type_, const std::string& file_) : name(name_), type(type_), file(file_)
{
}

void ProcessFile(const std::string& filePath, bool verbose, std::vector<Resource>& resources, std::unordered_set<std::string>& names)
{
    if (verbose)
    {
        std::cout << "> " << filePath << std::endl;
    }
    std::string dir = util::Path::GetDirectoryName(filePath);
    std::unique_ptr<soul::xml::Document> resourceDoc = soul::xml::ParseXmlFile(filePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/resources/include", resourceDoc.get());
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string fileAttribute = element->GetAttribute("file");
            if (fileAttribute.empty())
            {
                throw std::runtime_error("error: include element " + std::to_string(i) + " has no file attribute in resource XML file '" + filePath + "'");
            }
            std::string includeFilePath = util::GetFullPath(util::Path::Combine(dir, fileAttribute));
            if (std::filesystem::exists(includeFilePath))
            {
                ProcessFile(includeFilePath, verbose, resources, names);
            }
            else
            {
                throw std::runtime_error("error: include file '" + includeFilePath + "' in resource XML file '" + filePath + "' does not exist");
            }
        }
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> resourceNodeSet = soul::xml::xpath::EvaluateToNodeSet("/resources/resource", resourceDoc.get());
    int nr = resourceNodeSet->Count();
    for (int i = 0; i < nr; ++i)
    {
        soul::xml::Node* node = resourceNodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string nameAttribute = element->GetAttribute("name");
            if (nameAttribute.empty())
            {
                throw std::runtime_error("error: resource element " + std::to_string(i) + " has no name attribute in resource XML file '" + filePath + "'");
            }
            std::string typeAttribute = element->GetAttribute("type");
            if (typeAttribute.empty())
            {
                throw std::runtime_error("error: resource element " + std::to_string(i) + " has no type attribute in resource XML file '" + filePath + "'");
            }
            std::string fileAttribute = element->GetAttribute("file");
            if (fileAttribute.empty())
            {
                throw std::runtime_error("error: resource element " + std::to_string(i) + " has no file attribute in resource XML file '" + filePath + "'");
            }
            std::string name = nameAttribute;
            if (names.find(name) != names.cend())
            {
                throw std::runtime_error("resource name '" + name + "' not unique in resource XML file '" + filePath + "'");
            }
            names.insert(name);
            Resource resource(name, typeAttribute, util::GetFullPath(util::Path::Combine(dir, fileAttribute)));
            resources.push_back(resource);
        }
    }
}

std::string GetResourceTypeName(const std::string& type)
{
    if (type == "cursor")
    {
        return "CURSOR";
    }
    else if (type == "icon")
    {
        return "ICON";
    }
    else if (type == "bitmap")
    {
        return "BITMAP";
    }
    else
    {
        throw std::runtime_error("unknown resource type '" + type + "'");
    }
}

void CreateResourceScriptFile(const std::vector<Resource>& resources, const std::string& resourceScriptFileName, bool verbose)
{
    std::ofstream resourceScriptFile(resourceScriptFileName);
    util::CodeFormatter formatter(resourceScriptFile);
    for (const Resource& resource : resources)
    {
        std::string line;
        line.append(resource.name).append(1, ' ');
        line.append(GetResourceTypeName(resource.type)).append(1, ' ');
        line.append(1, '"').append(resource.file).append(1, '"');
        formatter.WriteLine(line);
    }
    if (verbose)
    {
        std::cout << "==> " + resourceScriptFileName << std::endl;
    }
}

const char* version = "5.0.0";

void PrintHelp()
{
    std::cout << "Cmajor resource compiler version " << version << std::endl;
    std::cout << "Usage cmrc [options] { RESOURCE.FILE.XML }" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "--help | -h" << std::endl;
    std::cout << "  Print help and exit." << std::endl;
    std::cout << "--verbose | -v" << std::endl;
    std::cout << "  Be verbose." << std::endl;
}

int main(int argc, const char** argv)
{
    try
    {
        InitApplication();
        bool verbose = false;
        std::vector<std::string> files;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    verbose = true;
                }
                else if (arg == "--help")
                {
                    PrintHelp();
                    return 1;
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                for (char o : options)
                {
                    switch (o)
                    {
                    case 'v':
                    {
                        verbose = true;
                        break;
                    }
                    case 'h':
                    {
                        PrintHelp();
                        return 1;
                    }
                    default:
                    {
                        throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                    }
                    }
                }
            }
            else
            {
                files.push_back(util::GetFullPath(arg));
            }
        }
        for (const std::string& file : files)
        {
            std::vector<Resource> resources;
            std::unordered_set<std::string> names;
            ProcessFile(file, verbose, resources, names);
            std::string resourceScriptFile = util::Path::ChangeExtension(file, ".rc");
            CreateResourceScriptFile(resources, resourceScriptFile, verbose);
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
    return 0;
}