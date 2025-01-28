// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.config;

import soul.xml.document;
import soul.xml.text;
import soul.xml.dom.parser;
import soul.xml.xpath;
import util;

namespace cmajor::build {

BuildConfig::BuildConfig() : 
    filePath(util::GetFullPath(util::Path::Combine(util::Path::Combine(util::CmajorRoot(), "config"), "build_config.xml"))),
    clangxxPath("clang++"), gxxPath("g++"), gdbPath("gdb"), arPath("ar")
{
    if (!std::filesystem::exists(filePath))
    {
        std::ofstream file(filePath);
        util::CodeFormatter formatter(file);
        formatter.SetIndentSize(1);
        soul::xml::Document configDoc;
        soul::xml::Element* root = soul::xml::MakeElement("build_config");
        configDoc.AppendChild(root);

        soul::xml::Element* clangxxPathElement = soul::xml::MakeElement("clangxx_path");
        clangxxPathElement->AppendChild(soul::xml::MakeText(clangxxPath));
        root->AppendChild(clangxxPathElement);

        soul::xml::Element* gxxPathElement = soul::xml::MakeElement("gxx_path");
        gxxPathElement->AppendChild(soul::xml::MakeText(gxxPath));
        root->AppendChild(gxxPathElement);

        soul::xml::Element* gdbPathElement = soul::xml::MakeElement("gdb_path");
        gdbPathElement->AppendChild(soul::xml::MakeText(gdbPath));
        root->AppendChild(gdbPathElement);

        soul::xml::Element* arPathElement = soul::xml::MakeElement("ar_path");
        arPathElement->AppendChild(soul::xml::MakeText(arPath));
        root->AppendChild(arPathElement);
        configDoc.Write(formatter);
    }
    std::unique_ptr<soul::xml::Document> configDoc = soul::xml::ParseXmlFile(filePath);

    std::unique_ptr<soul::xml::xpath::NodeSet> clangxxPathElements = soul::xml::xpath::EvaluateToNodeSet("/build_config/clangxx_path", configDoc.get());
    for (int i = 0; i < clangxxPathElements->Count(); ++i)
    {
        soul::xml::Node* node = clangxxPathElements->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* clangxxPathElement = static_cast<soul::xml::Element*>(node);
            soul::xml::Node* node = clangxxPathElement->FirstChild();
            if (node && node->IsTextNode())
            {
                soul::xml::Text* text = static_cast<soul::xml::Text*>(node);
                clangxxPath = text->Data();
            }
        }
    }

    std::unique_ptr<soul::xml::xpath::NodeSet> gxxPathElements = soul::xml::xpath::EvaluateToNodeSet("/build_config/gxx_path", configDoc.get());
    for (int i = 0; i < gxxPathElements->Count(); ++i)
    {
        soul::xml::Node* node = gxxPathElements->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* gxxPathElement = static_cast<soul::xml::Element*>(node);
            soul::xml::Node* node = gxxPathElement->FirstChild();
            if (node && node->IsTextNode())
            {
                soul::xml::Text* text = static_cast<soul::xml::Text*>(node);
                gxxPath = text->Data();
            }
        }
    }

    std::unique_ptr<soul::xml::xpath::NodeSet> gdbPathElements = soul::xml::xpath::EvaluateToNodeSet("/build_config/gdb_path", configDoc.get());
    for (int i = 0; i < gdbPathElements->Count(); ++i)
    {
        soul::xml::Node* node = gdbPathElements->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* gdbPathElement = static_cast<soul::xml::Element*>(node);
            soul::xml::Node* node = gdbPathElement->FirstChild();
            if (node && node->IsTextNode())
            {
                soul::xml::Text* text = static_cast<soul::xml::Text*>(node);
                gdbPath = text->Data();
            }
        }
    }

    std::unique_ptr<soul::xml::xpath::NodeSet> arPathElements = soul::xml::xpath::EvaluateToNodeSet("/build_config/ar_path", configDoc.get());
    for (int i = 0; i < arPathElements->Count(); ++i)
    {
        soul::xml::Node* node = arPathElements->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* arPathElement = static_cast<soul::xml::Element*>(node);
            soul::xml::Node* node = arPathElement->FirstChild();
            if (node && node->IsTextNode())
            {
                soul::xml::Text* text = static_cast<soul::xml::Text*>(node);
                arPath = text->Data();
            }
        }
    }
}

BuildConfig& BuildConfig::Instance()
{
    static BuildConfig instance;
    return instance;
}

const std::string& GetClangXXPathFromBuildConfig()
{
    return BuildConfig::Instance().ClangXXPath();
}

const std::string& GetGXXPathFromBuildConfig()
{
    return BuildConfig::Instance().GXXPath();
}

const std::string& GetGDBPathFromBuildConfig()
{
    return BuildConfig::Instance().GDBPath();
}

const std::string& GetARPathFromBuildConfig()
{
    return BuildConfig::Instance().ARPath();
}

} // namespace cmajor::build
