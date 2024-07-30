// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmdoclib.input;

import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import util;

namespace cmdoclib {

Input::Input() : errors(0)
{
}

Input* inputPtr = nullptr;

Input ReadInput(const std::string& cmdocXmlFilePath)
{
    Input input;

    input.baseDir = util::GetFullPath(util::Path::GetDirectoryName(util::Path::MakeCanonical(cmdocXmlFilePath)));
    std::unique_ptr<soul::xml::Document> cmdocXmlDocument = soul::xml::ParseXmlFile(cmdocXmlFilePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> solutionNodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdoc/solution", cmdocXmlDocument.get());
    int n = solutionNodeSet->Count();
    if (n == 1)
    {
        soul::xml::Node* node = solutionNodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string relativeSolutionFilePath = element->GetAttribute("filePath");
            if (!relativeSolutionFilePath.empty())
            {
                input.relativeSolutionFilePath = relativeSolutionFilePath;
                input.solutionFilePath = util::GetFullPath(util::Path::Combine(input.baseDir, relativeSolutionFilePath));
            }
            else
            {
                throw std::runtime_error("'solution' element has no 'filePath' attribute in '" + cmdocXmlFilePath + "'");
            }
        }
    }
    else
    {
        throw std::runtime_error("single 'solution' element expected in '" + cmdocXmlFilePath + "'");
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> docNodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdoc/doc", cmdocXmlDocument.get());
    if (docNodeSet->Count() == 1)
    {
        soul::xml::Node* node = docNodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string docFilePath = util::GetFullPath(util::Path::Combine(input.baseDir, element->GetAttribute("filePath")));
            input.docs = soul::xml::ParseXmlFile(docFilePath);
        }
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> targetNodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdoc/target", cmdocXmlDocument.get());
    int tn = targetNodeSet->Count();
    if (tn == 1)
    {
        soul::xml::Node* node = targetNodeSet->GetNode(0);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string relativeTargetDirPath = element->GetAttribute("dir");
            if (relativeTargetDirPath.empty())
            {
                throw std::runtime_error("'target' element has no 'dir' attribute in '" + cmdocXmlFilePath + "'");
            }
            std::string targetDirPath = util::GetFullPath(util::Path::Combine(input.baseDir, relativeTargetDirPath));
            input.targetDirPath = targetDirPath;
        }
        else
        {
            throw std::runtime_error("single 'target' element expected in '" + cmdocXmlFilePath + "'");
        }
    }
    else
    {
        throw std::runtime_error("single 'target' element expected in '" + cmdocXmlFilePath + "'");
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> libraryNodeSet = soul::xml::xpath::EvaluateToNodeSet("/cmdoc/libraries/library", cmdocXmlDocument.get());
    for (int i = 0; i < libraryNodeSet->Count(); ++i)
    {
        soul::xml::Node* node = libraryNodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string libraryDir = element->GetAttribute("dir");
            if (!libraryDir.empty())
            {
                input.libraryDirs.push_back(libraryDir);
                std::string libraryPrefix = util::Path::Combine(libraryDir, "content");
                std::string libDir = util::Path::Combine(input.baseDir, libraryDir);
                std::unique_ptr<soul::xml::Document> modulesDoc = soul::xml::ParseXmlFile(util::Path::Combine(libDir, "modules.xml"));
                std::unique_ptr<soul::xml::xpath::NodeSet> moduleNodeSet = soul::xml::xpath::EvaluateToNodeSet("/modules/module", modulesDoc.get());
                for (int i = 0; i < moduleNodeSet->Count(); ++i)
                {
                    soul::xml::Node* node = moduleNodeSet->GetNode(i);
                    if (node->IsElementNode())
                    {
                        soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                        std::string moduleName = element->GetAttribute("name");
                        input.libraryPrefixMap[moduleName] = libraryPrefix;
                    }
                }
            }
        }
    }
    return input;
}

Input* GetInputPtr()
{
    return inputPtr;
}

void SetInputPtr(Input* input)
{
    inputPtr = input;
}

void SetEmptyLibraryPrefix(const std::string& moduleName)
{
    Input* input = GetInputPtr();
    auto it = input->libraryPrefixMap.find(moduleName);
    if (it != input->libraryPrefixMap.cend())
    {
        throw std::runtime_error("library prefix for module '" + moduleName + "' already set");
    }
    else
    {
        input->libraryPrefixMap[moduleName] = "";
    }
}

} // namespace cmdoclib
