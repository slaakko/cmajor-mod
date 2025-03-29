// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.kernel.config;

import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import util;
import std.filesystem;

namespace {

std::string CmajorConfigDir()
{
    return util::Path::Combine(util::CmajorRoot(), "config");
}

} // namespace

namespace cmajor::systemx::kernel {

Config& Config::Instance()
{
    static Config instance;
    return instance;
}

Config::Config() : filePath(util::Path::Combine(CmajorConfigDir(), "sx.kernel.config.xml"))
{
    if (!std::filesystem::exists(filePath))
    {
        mountRootDirs.insert(util::GetFullPath("C:\\"));
        mountRootDirs.insert(util::GetFullPath("D:\\"));
        Write();
    }
    Read();
}

bool Config::IncludedInMountRootDirs(const std::string& rootDir) const
{
    std::string fullRootDirPath = util::GetFullPath(rootDir);
    return mountRootDirs.find(rootDir) != mountRootDirs.end();
}

void Config::Write()
{
    std::ofstream file(filePath);
    soul::xml::Document configDoc;
    soul::xml::Element* configurationElement = soul::xml::MakeElement("configuration");
    configDoc.AppendChild(configurationElement);
    soul::xml::Element* mountsElement = soul::xml::MakeElement("mounts");
    configurationElement->AppendChild(mountsElement);
    for (const auto& mountDrive : mountRootDirs)
    {
        soul::xml::Element* mountElement = soul::xml::MakeElement("mount");
        mountElement->SetAttribute("rootDir", mountDrive);
        mountsElement->AppendChild(mountElement);
    }
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    configDoc.Write(formatter);
}

void Config::Read()
{
    mountRootDirs.clear();
    std::unique_ptr<soul::xml::Document> configDoc = soul::xml::ParseXmlFile(filePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("configuration", configDoc.get());
    int n = nodeSet->Count();
    if (n == 1)
    {
        soul::xml::Node* node = nodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("mounts/mount", element);
            int n = nodeSet->Count();
            for (int i = 0; i < n; ++i)
            {
                soul::xml::Node* node = nodeSet->GetNode(i);
                if (node->IsElementNode())
                {
                    soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                    std::string mountRootDir = element->GetAttribute("rootDir");
                    if (!mountRootDir.empty())
                    {
                        mountRootDirs.insert(util::GetFullPath(mountRootDir));
                    }
                }
            }
        }
    }
    else
    {
        throw std::runtime_error("error reading configuration from '" + filePath + "': single 'configuration' element expected");
    }
}

} // namespace cmajor::systemx::kernel
