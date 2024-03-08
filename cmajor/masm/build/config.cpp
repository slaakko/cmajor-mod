// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.build.config;

import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath.evaluate;
import soul.xml.xpath.object;
import std.filesystem;
import util.unicode;
import util.path;

namespace cmajor::masm::build {

std::string GetVCVars64Path()
{
    std::string configFilePath = util::GetFullPath(util::Path::Combine(util::Path::Combine(util::CmajorRoot(), "config"), "masm.config.xml"));
    std::filesystem::create_directories(util::Path::GetDirectoryName(configFilePath));
    if (!std::filesystem::exists(configFilePath))
    {
        soul::xml::Document configDoc;
        soul::xml::Element* configurationElement = soul::xml::MakeElement("configuration");
        configDoc.AppendChild(configurationElement);
        soul::xml::Element* vcvars64Element = soul::xml::MakeElement("vcvars64");
        configurationElement->AppendChild(vcvars64Element);
        vcvars64Element->SetAttribute("path", R"(C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat)");
        std::ofstream configFile(configFilePath);
        util::CodeFormatter formatter(configFile);
        formatter.SetIndentSize(1);
        configDoc.Write(formatter);
    }
    std::unique_ptr<soul::xml::Document> configDoc = soul::xml::ParseXmlFile(configFilePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/configuration/vcvars64", configDoc.get());
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string path = element->GetAttribute("path");
            if (path.empty())
            {
                throw std::runtime_error("/configuration/vcvars64 element has no 'path' attribute");
            }
            return path;
        }
    }
    return std::string();
}

} // namespace cmajor::masm::build
