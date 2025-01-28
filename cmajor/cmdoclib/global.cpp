// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmdoclib.global;

import cmdoclib.input;
import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import std.filesystem;
import util;

namespace cmdoclib {

class DerivedClassMap
{
public:
    void AddDerivedClass(const std::string& baseClassId, const std::string& derivedClassId);
    std::vector<std::string> GetDerivedClassIds(const std::string& baseClassId) const;
    soul::xml::Element* ToXmlElement();
private:
    std::map<std::string, std::set<std::string>> derivedClassMap;
};

void DerivedClassMap::AddDerivedClass(const std::string& baseClassId, const std::string& derivedClassId)
{
    std::set<std::string>& derivedClasses = derivedClassMap[baseClassId];
    derivedClasses.insert(derivedClassId);
}

std::vector<std::string> DerivedClassMap::GetDerivedClassIds(const std::string& baseClassId) const
{
    std::vector<std::string> derivedClassIds;
    auto it = derivedClassMap.find(baseClassId);
    if (it != derivedClassMap.cend())
    {
        const std::set<std::string>& ids = it->second;
        for (const std::string& id : ids)
        {
            derivedClassIds.push_back(id);
        }
    }
    return derivedClassIds;
}

soul::xml::Element* DerivedClassMap::ToXmlElement()
{
    soul::xml::Element* mapElement(soul::xml::MakeElement("derivedClassMap"));
    for (const auto& p : derivedClassMap)
    {
        const std::string& baseClassId = p.first;
        const std::set<std::string>& derivedClassIds = p.second;
        soul::xml::Element* baseClassElement(soul::xml::MakeElement("baseClass"));
        baseClassElement->SetAttribute("classId", baseClassId);
        for (const std::string& derivedClassId : derivedClassIds)
        {
            soul::xml::Element* derivedClassElement(soul::xml::MakeElement("derivedClass"));
            derivedClassElement->SetAttribute("classId", derivedClassId);
            baseClassElement->AppendChild(derivedClassElement);
        }
        mapElement->AppendChild(baseClassElement);
    }
    return mapElement;
}

class Global
{
public:
    static Global& Instance();
    void Read(const std::vector<std::string>& moduleNames);
    void Write(const std::vector<std::string>& moduleNames);
    DerivedClassMap* GetDerivedClassMap(const std::string& moduleName);
private:
    std::map<std::string, DerivedClassMap*> moduleDerivedClassMap;
    std::vector<std::unique_ptr<DerivedClassMap>> derivedClassMaps;
};

Global& Global::Instance()
{
    static Global instance;
    return instance;
}

void Global::Read(const std::vector<std::string>& moduleNames)
{
    Input* input = GetInputPtr();
    std::string targetDir = input->targetDirPath;
    std::string contentDir = util::GetFullPath(util::Path::Combine(targetDir, "content"));
    for (const std::string& moduleName : moduleNames)
    {
        std::string moduleDir = util::GetFullPath(util::Path::Combine(contentDir, moduleName));
        std::string globalXmlPath = util::GetFullPath(util::Path::Combine(moduleDir, "global.xml"));
        if (std::filesystem::exists(globalXmlPath))
        {
            std::unique_ptr<soul::xml::Document> globalDoc = soul::xml::ParseXmlFile(globalXmlPath);
            std::unique_ptr<soul::xml::xpath::NodeSet> baseClasses = soul::xml::xpath::EvaluateToNodeSet("/global/derivedClassMap/baseClass", globalDoc.get());
            int n = baseClasses->Count();
            for (int i = 0; i < n; ++i)
            {
                soul::xml::Node* baseClassNode = baseClasses->GetNode(i);
                if (baseClassNode->IsElementNode())
                {
                    soul::xml::Element* baseClassElement = static_cast<soul::xml::Element*>(baseClassNode);
                    std::string baseClassId = baseClassElement->GetAttribute("classId");
                    std::unique_ptr<soul::xml::xpath::NodeSet> derivedClasses = soul::xml::xpath::EvaluateToNodeSet("/derivedClass", baseClassElement);
                    int n = derivedClasses->Count();
                    for (int i = 0; i < n; ++i)
                    {
                        soul::xml::Node* derivedClassNode = derivedClasses->GetNode(i);
                        if (derivedClassNode->IsElementNode())
                        {
                            soul::xml::Element* derivedClassElement = static_cast<soul::xml::Element*>(derivedClassNode);
                            std::string derivedClassId = derivedClassElement->GetAttribute("classId");
                            GetDerivedClassMap(moduleName)->AddDerivedClass(baseClassId, derivedClassId);
                        }
                    }
                }
            }
        }
    }
}

void Global::Write(const std::vector<std::string>& moduleNames)
{
    Input* input = GetInputPtr();
    std::string targetDir = input->targetDirPath;
    std::string contentDir = util::GetFullPath(util::Path::Combine(targetDir, "content"));
    for (const std::string& moduleName : moduleNames)
    {
        std::string moduleDir = util::GetFullPath(util::Path::Combine(contentDir, moduleName));
        std::string globalXmlPath = util::GetFullPath(util::Path::Combine(moduleDir, "global.xml"));
        soul::xml::Document globalDoc;
        soul::xml::Element* globalElement(soul::xml::MakeElement("global"));
        DerivedClassMap* derivedClassMap = moduleDerivedClassMap[moduleName];
        if (derivedClassMap)
        {
            soul::xml::Element* derivedClassMapElement = derivedClassMap->ToXmlElement();
            globalElement->AppendChild(derivedClassMapElement);
            globalDoc.AppendChild(globalElement);
            std::ofstream globalFile(globalXmlPath);
            util::CodeFormatter formatter(globalFile);
            globalDoc.Write(formatter);
        }
    }
}

DerivedClassMap* Global::GetDerivedClassMap(const std::string& moduleName)
{
    DerivedClassMap* derivedClassMap = moduleDerivedClassMap[moduleName];
    if (!derivedClassMap)
    {
        derivedClassMap = new DerivedClassMap();
        moduleDerivedClassMap[moduleName] = derivedClassMap;
        derivedClassMaps.push_back(std::unique_ptr<DerivedClassMap>(derivedClassMap));
    }
    return derivedClassMap;
}

void ReadGlobals(const std::vector<std::string>& moduleNames)
{
    Global::Instance().Read(moduleNames);
}

void WriteGlobals(const std::vector<std::string>& moduleNames)
{
    Global::Instance().Write(moduleNames);
}

void AddDerivedClass(cmajor::symbols::ClassTypeSymbol* baseClass, cmajor::symbols::ClassTypeSymbol* derivedClass)
{
    cmajor::symbols::Module* originalModule = baseClass->GetModule();
    std::string moduleName = util::ToUtf8(originalModule->Name());
    Global::Instance().GetDerivedClassMap(moduleName)->AddDerivedClass(util::ToUtf8(baseClass->Id()), util::ToUtf8(derivedClass->Id()));
}

std::vector<std::string> GetDerivedClassIds(const std::string& moduleName, const std::string& baseClassId)
{
    return Global::Instance().GetDerivedClassMap(moduleName)->GetDerivedClassIds(baseClassId);
}

void AddConceptRefinement(cmajor::symbols::ConceptSymbol* refinedConcept, cmajor::symbols::ConceptSymbol* conceptSymbol)
{
    cmajor::symbols::Module* originalModule = refinedConcept->GetModule();
    std::string moduleName = util::ToUtf8(originalModule->Name());
    Global::Instance().GetDerivedClassMap(moduleName)->AddDerivedClass(util::ToUtf8(refinedConcept->Id()), util::ToUtf8(conceptSymbol->Id()));
}

std::vector<std::string> GetDerivedConceptIds(const std::string& moduleName, const std::string& refinedConceptId)
{
    return Global::Instance().GetDerivedClassMap(moduleName)->GetDerivedClassIds(refinedConceptId);
}

void WriteModuleXml(const std::string& targetDir, const std::vector<std::string>& moduleNames)
{
    std::unique_ptr<soul::xml::Document> moduleXmlDoc(new soul::xml::Document());
    std::unique_ptr<soul::xml::Element> modulesElement(soul::xml::MakeElement("modules"));
    for (const std::string& moduleName : moduleNames)
    {
        std::unique_ptr<soul::xml::Element> moduleElement(soul::xml::MakeElement("module"));
        moduleElement->SetAttribute("name", moduleName);
        modulesElement->AppendChild(moduleElement.release());
    }
    moduleXmlDoc->AppendChild(modulesElement.release());
    std::string moduleXmlFilePath = util::Path::Combine(targetDir, "modules.xml");
    std::ofstream moduleXmlFile(moduleXmlFilePath);
    util::CodeFormatter formatter(moduleXmlFile);
    formatter.SetIndentSize(1);
    moduleXmlDoc->Write(formatter);
}

} // namespace cmdoclib
