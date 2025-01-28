// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmdoclib.docs;

import cmdoclib.input;
import cmdoclib.content_visitor;
import cmdoclib.diagram;
import cmdoclib.global;
import cmajor.symbols;
import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import std.filesystem;
import util;

namespace cmdoclib {

void AppendToLegend(soul::xml::Element* legend, const std::string& title, const std::string& id, bool& first)
{
    if (first)
    {
        first = false;
    }
    else
    {
        legend->AppendChild(soul::xml::MakeText(" | "));
    }
    soul::xml::Element* link = soul::xml::MakeElement("a");
    link->SetAttribute("href", "#" + id);
    link->AppendChild(soul::xml::MakeText(title));
    legend->AppendChild(link);
}

void GetConcepts(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& conceptNodeSet, std::vector<soul::xml::Element*>& conceptElements)
{
    conceptNodeSet = soul::xml::xpath::EvaluateToNodeSet("concepts/concept", parentElement);
    int n = conceptNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* conceptNode = conceptNodeSet->GetNode(i);
        if (conceptNode->IsElementNode())
        {
            soul::xml::Element* conceptElement = static_cast<soul::xml::Element*>(conceptNode);
            conceptElements.push_back(conceptElement);
        }
    }
}

void GetInterfaces(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& interfaceNodeSet, std::vector<soul::xml::Element*>& interfaceElements)
{
    interfaceNodeSet = soul::xml::xpath::EvaluateToNodeSet("interfaces/interface", parentElement);
    int n = interfaceNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* interfaceNode = interfaceNodeSet->GetNode(i);
        if (interfaceNode->IsElementNode())
        {
            soul::xml::Element* interfaceElement = static_cast<soul::xml::Element*>(interfaceNode);
            interfaceElements.push_back(interfaceElement);
        }
    }
}

void GetClasses(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& classNodeSet, std::vector<soul::xml::Element*>& classElements)
{
    classNodeSet = soul::xml::xpath::EvaluateToNodeSet("classes/class", parentElement);
    int n = classNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* classNode = classNodeSet->GetNode(i);
        if (classNode->IsElementNode())
        {
            soul::xml::Element* classElement = static_cast<soul::xml::Element*>(classNode);
            classElements.push_back(classElement);
        }
    }
}

void GetFunctions(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& functionNodeSet, std::vector<soul::xml::Element*>& functionElements)
{
    functionNodeSet = soul::xml::xpath::EvaluateToNodeSet("functions/function", parentElement);
    int n = functionNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* functionNode = functionNodeSet->GetNode(i);
        if (functionNode->IsElementNode())
        {
            soul::xml::Element* functionElement = static_cast<soul::xml::Element*>(functionNode);
            functionElements.push_back(functionElement);
        }
    }
}

void GetConstructorsAndDestructors(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& ctorDtorNodeSet, 
    std::vector<soul::xml::Element*>& ctorDtorElements)
{
    ctorDtorNodeSet = soul::xml::xpath::EvaluateToNodeSet("ctorsDtors/ctorDtor", parentElement);
    int n = ctorDtorNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* ctorDtorNode = ctorDtorNodeSet->GetNode(i);
        if (ctorDtorNode->IsElementNode())
        {
            soul::xml::Element* ctorDtorElement = static_cast<soul::xml::Element*>(ctorDtorNode);
            ctorDtorElements.push_back(ctorDtorElement);
        }
    }
}

soul::xml::Element* GetConstraint(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& constraintNodeSet)
{
    constraintNodeSet = soul::xml::xpath::EvaluateToNodeSet("where", parentElement);
    if (constraintNodeSet->Count() == 1)
    {
        soul::xml::Node* constraintNode = constraintNodeSet->GetNode(0);
        if (constraintNode->IsElementNode())
        {
            soul::xml::Element* constraintElement = static_cast<soul::xml::Element*>(constraintNode);
            return constraintElement;
        }
    }
    return nullptr;
}

void GetDelegates(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& delegateNodeSet, std::vector<soul::xml::Element*>& delegateElements)
{
    delegateNodeSet = soul::xml::xpath::EvaluateToNodeSet("delegates/delegate", parentElement);
    int n = delegateNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* delegateNode = delegateNodeSet->GetNode(i);
        if (delegateNode->IsElementNode())
        {
            soul::xml::Element* delegateElement = static_cast<soul::xml::Element*>(delegateNode);
            delegateElements.push_back(delegateElement);
        }
    }
}

void GetClassDelegates(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& classDelegateNodeSet,
    std::vector<soul::xml::Element*>& classDelegateElements)
{
    classDelegateNodeSet = soul::xml::xpath::EvaluateToNodeSet("classDelegates/classDelegate", parentElement);
    int n = classDelegateNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* classDelegateNode = classDelegateNodeSet->GetNode(i);
        if (classDelegateNode->IsElementNode())
        {
            soul::xml::Element* classDelegateElement = static_cast<soul::xml::Element*>(classDelegateNode);
            classDelegateElements.push_back(classDelegateElement);
        }
    }
}

void GetTypedefs(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& typedefNodeSet, std::vector<soul::xml::Element*>& typedefElements)
{
    typedefNodeSet = soul::xml::xpath::EvaluateToNodeSet("typedefs/typedef", parentElement);
    int n = typedefNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* typedefNode = typedefNodeSet->GetNode(i);
        if (typedefNode->IsElementNode())
        {
            soul::xml::Element* typedefElement = static_cast<soul::xml::Element*>(typedefNode);
            typedefElements.push_back(typedefElement);
        }
    }
}

void GetEnumTypes(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& enumTypesNodeSet, std::vector<soul::xml::Element*>& enumTypeElements)
{
    enumTypesNodeSet = soul::xml::xpath::EvaluateToNodeSet("enumTypes/enumType", parentElement);
    int n = enumTypesNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* enumTypeNode = enumTypesNodeSet->GetNode(i);
        if (enumTypeNode->IsElementNode())
        {
            soul::xml::Element* enumTypeElement = static_cast<soul::xml::Element*>(enumTypeNode);
            enumTypeElements.push_back(enumTypeElement);
        }
    }
}

void GetEnumConstants(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& enumConstantNodeSet, std::vector<soul::xml::Element*>& enumConstantElements)
{
    enumConstantNodeSet = soul::xml::xpath::EvaluateToNodeSet("enumConstants/enumConstant", parentElement);
    int n = enumConstantNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* enumConstantNode = enumConstantNodeSet->GetNode(i);
        if (enumConstantNode->IsElementNode())
        {
            soul::xml::Element* enumConstantElement = static_cast<soul::xml::Element*>(enumConstantNode);
            enumConstantElements.push_back(enumConstantElement);
        }
    }
}

void GetConstants(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& constantsNodeSet, std::vector<soul::xml::Element*>& constantElements)
{
    constantsNodeSet = soul::xml::xpath::EvaluateToNodeSet("constants/constant", parentElement);
    int n = constantsNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* constantNode = constantsNodeSet->GetNode(i);
        if (constantNode->IsElementNode())
        {
            soul::xml::Element* constantElement = static_cast<soul::xml::Element*>(constantNode);
            constantElements.push_back(constantElement);
        }
    }
}

void GetVariables(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& variableNodeSet, std::vector<soul::xml::Element*>& variableElements)
{
    variableNodeSet = soul::xml::xpath::EvaluateToNodeSet("variables/variable", parentElement);
    int n = variableNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* variableNode = variableNodeSet->GetNode(i);
        if (variableNode->IsElementNode())
        {
            soul::xml::Element* variableElement = static_cast<soul::xml::Element*>(variableNode);
            variableElements.push_back(variableElement);
        }
    }
}

void GetTypeParameters(soul::xml::Element* conceptElement, std::unique_ptr<soul::xml::xpath::NodeSet>& typeParametersNodeSet, 
    std::vector<soul::xml::Element*>& typeParameterElements)
{
    typeParametersNodeSet = soul::xml::xpath::EvaluateToNodeSet("typeParameters/typeParameter", conceptElement);
    int n = typeParametersNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* typeParameterNode = typeParametersNodeSet->GetNode(i);
        if (typeParameterNode->IsElementNode())
        {
            soul::xml::Element* typeParameterElement = static_cast<soul::xml::Element*>(typeParameterNode);
            typeParameterElements.push_back(typeParameterElement);
        }
    }
}

void GetConstraints(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& constraintsNodeSet, std::vector<soul::xml::Element*>& constraintElements)
{
    constraintsNodeSet = soul::xml::xpath::EvaluateToNodeSet("constraints/*", parentElement);
    int n = constraintsNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* constraintNode = constraintsNodeSet->GetNode(i);
        if (constraintNode->IsElementNode())
        {
            soul::xml::Element* constraintElement = static_cast<soul::xml::Element*>(constraintNode);
            constraintElements.push_back(constraintElement);
        }
    }
}

soul::xml::Element* GetRefinedConcept(soul::xml::Element* conceptElement, 
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs)
{
    soul::xml::Element* refinedConceptElement = nullptr;
    std::string refinedConceptId = conceptElement->GetAttribute("refines");
    if (!refinedConceptId.empty())
    {
        refinedConceptElement = moduleXmlDoc->GetElementById(refinedConceptId);
        if (!refinedConceptElement)
        {
            int n = static_cast<int>(otherModuleXmlDocs.size());
            for (int i = 0; i < n; ++i)
            {
                soul::xml::Document* otherModuleXmlDoc = otherModuleXmlDocs[i];
                refinedConceptElement = otherModuleXmlDoc->GetElementById(refinedConceptId);
                if (refinedConceptElement) break;
            }
        }
        if (!refinedConceptElement)
        {
            std::cerr << "refined concept id '" << refinedConceptId << "' not found" << "\n";
            Input* input = GetInputPtr();
            ++input->errors;
            return nullptr;
        }
    }
    return refinedConceptElement;
}

std::vector<soul::xml::Element*> GetDerivedConcepts(soul::xml::Element* conceptElement, soul::xml::Document* moduleXmlDoc,
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs)
{
    std::vector<soul::xml::Element*> derivedConceptElements;
    std::string moduleName = moduleXmlDoc->DocumentElement()->GetAttribute("module");
    std::vector<std::string> derivedConceptIds = GetDerivedConceptIds(moduleName, conceptElement->GetAttribute("id"));
    for (const std::string& derivedConceptId : derivedConceptIds)
    {
        soul::xml::Element* derivedConceptElement = nullptr;
        derivedConceptElement = moduleXmlDoc->GetElementById(derivedConceptId);
        if (!derivedConceptElement)
        {
            int n = static_cast<int>(otherModuleXmlDocs.size());
            for (int i = 0; i < n; ++i)
            {
                soul::xml::Document* otherModuleXmlDoc = otherModuleXmlDocs[i];
                derivedConceptElement = otherModuleXmlDoc->GetElementById(derivedConceptId);
                if (derivedConceptElement) break;
            }
        }
        if (!derivedConceptElement)
        {
            std::cerr << "derived concept id '" << derivedConceptId << "' not found" << "\n"; 
            Input* input = GetInputPtr();
            ++input->errors;
            continue;
        }
        derivedConceptElements.push_back(derivedConceptElement);
    }
    return derivedConceptElements;
}

void GetAxioms(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& axiomsNodeSet, std::vector<soul::xml::Element*>& axiomElements)
{
    axiomsNodeSet = soul::xml::xpath::EvaluateToNodeSet("axioms/axiom", parentElement);
    int n = axiomsNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* axiomNode = axiomsNodeSet->GetNode(i);
        if (axiomNode->IsElementNode())
        {
            soul::xml::Element* axiomElement = static_cast<soul::xml::Element*>(axiomNode);
            axiomElements.push_back(axiomElement);
        }
    }
}

void GetAxiomStatements(soul::xml::Element* parentElement, std::unique_ptr<soul::xml::xpath::NodeSet>& axiomStatementsNodeSet, 
    std::vector<soul::xml::Element*>& axiomStatementElements)
{
    axiomStatementsNodeSet = soul::xml::xpath::EvaluateToNodeSet("axiomStatements/axiomStatement", parentElement);
    int n = axiomStatementsNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* axiomStatementNode = axiomStatementsNodeSet->GetNode(i);
        if (axiomStatementNode->IsElementNode())
        {
            soul::xml::Element* axiomStatementElement = static_cast<soul::xml::Element*>(axiomStatementNode);
            axiomStatementElements.push_back(axiomStatementElement);
        }
    }
}

std::string MakeConceptName(soul::xml::Element* conceptElement)
{
    std::string conceptName = conceptElement->GetAttribute("name");
    std::unique_ptr<soul::xml::xpath::NodeSet> typeParametersNodeSet = soul::xml::xpath::EvaluateToNodeSet("typeParameters/typeParameter", conceptElement);
    int n = typeParametersNodeSet->Count();
    if (n > 0)
    {
        conceptName.append(1, '<');
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                conceptName.append(", ");
            }
            soul::xml::Node* typeParameterNode = typeParametersNodeSet->GetNode(i);
            if (typeParameterNode->IsElementNode())
            {
                soul::xml::Element* typeParameterElement = static_cast<soul::xml::Element*>(typeParameterNode);
                conceptName.append(typeParameterElement->GetAttribute("name"));
            }
        }
        conceptName.append(1, '>');
    }
    return conceptName;
}

std::string MakeFullConceptName(soul::xml::Element* conceptElement)
{
    std::string fullConceptName;
    if (conceptElement)
    {
        if (conceptElement->Name() == "namespaces" || conceptElement->Name() == "concepts")
        {
            soul::xml::Node* parent = conceptElement->Parent();
            if (parent && parent->IsElementNode())
            {
                soul::xml::Element* parentElement = static_cast<soul::xml::Element*>(parent);
                return MakeFullConceptName(parentElement);
            }
        }
        else if (conceptElement->Name() == "namespace" || conceptElement->Name() == "concept")
        {
            std::string name = conceptElement->GetAttribute("name");
            if (!name.empty())
            {
                soul::xml::Node* parent = conceptElement->Parent();
                if (parent && parent->IsElementNode())
                {
                    soul::xml::Element* parentElement = static_cast<soul::xml::Element*>(parent);
                    fullConceptName = MakeFullConceptName(parentElement);
                    if (!fullConceptName.empty())
                    {
                        fullConceptName.append(1, '.');
                    }
                }
                fullConceptName.append(name);
                if (conceptElement->Name() == "concept")
                {
                    std::unique_ptr<soul::xml::xpath::NodeSet> typeParametersNodeSet = soul::xml::xpath::EvaluateToNodeSet("typeParameters/typeParameter", conceptElement);
                    int n = typeParametersNodeSet->Count();
                    if (n > 0)
                    {
                        fullConceptName.append(1, '<');
                        for (int i = 0; i < n; ++i)
                        {
                            if (i > 0)
                            {
                                fullConceptName.append(", ");
                            }
                            soul::xml::Node* typeParameterNode = typeParametersNodeSet->GetNode(i);
                            if (typeParameterNode->IsElementNode())
                            {
                                soul::xml::Element* typeParameterElement = static_cast<soul::xml::Element*>(typeParameterNode);
                                fullConceptName.append(typeParameterElement->GetAttribute("name"));
                            }
                        }
                        fullConceptName.append(1, '>');
                    }
                }
            }
        }
    }
    return fullConceptName;
}

std::string MakeFunctionName(soul::xml::Element* functionElement, std::unique_ptr<soul::xml::xpath::NodeSet>& templateParametersNodeSet,
    std::vector<soul::xml::Element*>& templateParameterElements)
{
    std::string functionName = functionElement->GetAttribute("name");
    templateParametersNodeSet = soul::xml::xpath::EvaluateToNodeSet("templateParameters/templateParameter", functionElement);
    int n = templateParametersNodeSet->Count();
    if (n > 0)
    {
        functionName.append(1, '<');
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                functionName.append(", ");
            }
            soul::xml::Node* templateParameterNode = templateParametersNodeSet->GetNode(i);
            if (templateParameterNode->IsElementNode())
            {
                soul::xml::Element* templateParameterElement = static_cast<soul::xml::Element*>(templateParameterNode);
                templateParameterElements.push_back(templateParameterElement);
                functionName.append(templateParameterElement->GetAttribute("name"));
            }
        }
        functionName.append(1, '>');
    }
    return functionName;
}

std::string MakeClassName(soul::xml::Element* classElement, std::unique_ptr<soul::xml::xpath::NodeSet>& templateParametersNodeSet,
    std::vector<soul::xml::Element*>& templateParameterElements)
{
    std::string className = classElement->GetAttribute("name");
    templateParametersNodeSet = soul::xml::xpath::EvaluateToNodeSet("templateParameters/templateParameter", classElement);
    int n = templateParametersNodeSet->Count();
    if (n > 0)
    {
        className.append(1, '<');
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                className.append(", ");
            }
            soul::xml::Node* templateParameterNode = templateParametersNodeSet->GetNode(i);
            if (templateParameterNode->IsElementNode())
            {
                soul::xml::Element* templateParameterElement = static_cast<soul::xml::Element*>(templateParameterNode);
                templateParameterElements.push_back(templateParameterElement);
                className.append(templateParameterElement->GetAttribute("name"));
            }
        }
        className.append(1, '>');
    }
    return className;
}

struct BaseClassInfo
{
    soul::xml::Element* baseClassElement;
    std::string baseClassSpecializationName;
    std::string libraryPrefix;
};

BaseClassInfo GetBaseClass(Input* input, soul::xml::Element* classElement, soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs)
{
    BaseClassInfo baseClassInfo;
    baseClassInfo.baseClassElement = nullptr;
    std::string baseClassId = classElement->GetAttribute("baseClass");
    baseClassInfo.baseClassSpecializationName = classElement->GetAttribute("baseClassSpecializationName");
    if (!baseClassId.empty())
    {
        baseClassInfo.baseClassElement = moduleXmlDoc->GetElementById(baseClassId);
        if (!baseClassInfo.baseClassElement)
        {
            int n = static_cast<int>(otherModuleXmlDocs.size());
            for (int i = 0; i < n; ++i)
            {
                soul::xml::Document* otherModuleXmlDoc = otherModuleXmlDocs[i];
                std::string moduleName = otherModuleXmlDoc->DocumentElement()->GetAttribute("module");
                auto it = input->libraryPrefixMap.find(moduleName);
                if (it != input->libraryPrefixMap.cend())
                {
                    baseClassInfo.libraryPrefix = it->second;
                    if (!baseClassInfo.libraryPrefix.empty())
                    {
                        baseClassInfo.libraryPrefix = util::Path::Combine("../../..", baseClassInfo.libraryPrefix);
                    }
                }
                else
                {
                    continue;
                }
                baseClassInfo.baseClassElement = otherModuleXmlDoc->GetElementById(baseClassId);
                if (baseClassInfo.baseClassElement) break;
            }
            if (!baseClassInfo.baseClassElement)
            {
                std::cerr << "base class id '" << baseClassId << "' not found" << "\n";
                Input* input = GetInputPtr();
                ++input->errors;
            }
        }
    }
    return baseClassInfo;
}

std::vector<soul::xml::Element*> GetDerivedClasses(soul::xml::Element* classElement,
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs)
{
    std::vector<soul::xml::Element*> derivedClassElements;
    std::string moduleName = moduleXmlDoc->DocumentElement()->GetAttribute("module");
    std::vector<std::string> derivedClassIds = GetDerivedClassIds(moduleName, classElement->GetAttribute("id"));
    for (const std::string& derivedClassId : derivedClassIds)
    {
        soul::xml::Element* derivedClassElement = nullptr;
        derivedClassElement = moduleXmlDoc->GetElementById(derivedClassId);
        if (!derivedClassElement)
        {
            int n = static_cast<int>(otherModuleXmlDocs.size());
            for (int i = 0; i < n; ++i)
            {
                soul::xml::Document* otherModuleXmlDoc = otherModuleXmlDocs[i];
                derivedClassElement = otherModuleXmlDoc->GetElementById(derivedClassId);
                if (derivedClassElement) break;
            }
        }
        if (!derivedClassElement)
        {
            std::cerr << "derived class id '" << derivedClassId << "' not found" << std::endl;
            Input* input = GetInputPtr();
            ++input->errors;

        }
        else
        {
            derivedClassElements.push_back(derivedClassElement);
        }
    }
    return derivedClassElements;
}

std::string MakeFullClassName(soul::xml::Element* classElement)
{
    std::string fullClassName;
    if (classElement)
    {
        if (classElement->Name() == "namespaces" || classElement->Name() == "classes")
        {
            soul::xml::Node* parent = classElement->Parent();
            if (parent && parent->IsElementNode())
            {
                soul::xml::Element* parentElement = static_cast<soul::xml::Element*>(parent);
                return MakeFullClassName(parentElement);
            }
        }
        else if (classElement->Name() == "namespace" || classElement->Name() == "class")
        {
            std::string name = classElement->GetAttribute("name");
            if (!name.empty())
            {
                soul::xml::Node* parent = classElement->Parent();
                if (parent && parent->IsElementNode())
                {
                    soul::xml::Element* parentElement = static_cast<soul::xml::Element*>(parent);
                    fullClassName = MakeFullClassName(parentElement);
                    if (!fullClassName.empty())
                    {
                        fullClassName.append(1, '.');
                    }
                }
                fullClassName.append(name);
                if (classElement->Name() == "class")
                {
                    std::unique_ptr<soul::xml::xpath::NodeSet> templateParametersNodeSet = soul::xml::xpath::EvaluateToNodeSet(
                        "templateParameters/templateParameter", classElement);
                    int n = templateParametersNodeSet->Count();
                    if (n > 0)
                    {
                        fullClassName.append(1, '<');
                        for (int i = 0; i < n; ++i)
                        {
                            if (i > 0)
                            {
                                fullClassName.append(", ");
                            }
                            soul::xml::Node* templateParameterNode = templateParametersNodeSet->GetNode(i);
                            if (templateParameterNode->IsElementNode())
                            {
                                soul::xml::Element* templateParameterElement = static_cast<soul::xml::Element*>(templateParameterNode);
                                fullClassName.append(templateParameterElement->GetAttribute("name"));
                            }
                        }
                        fullClassName.append(1, '>');
                    }
                }
            }
        }
    }
    return fullClassName;
}

void GenerateInheritanceDiagramNodes(Input* input, soul::xml::Element* classElement, const std::string& specializationName, const std::string& libraryPrefix,
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs, std::vector<DiagramNode>& diagramNodes, int& level, int& depth)
{
    BaseClassInfo baseClassInfo = GetBaseClass(input, classElement, moduleXmlDoc, otherModuleXmlDocs);
    if (baseClassInfo.baseClassElement)
    {
        --level;
        ++depth;
        GenerateInheritanceDiagramNodes(input, baseClassInfo.baseClassElement, baseClassInfo.baseClassSpecializationName, baseClassInfo.libraryPrefix,
            moduleXmlDoc, otherModuleXmlDocs, diagramNodes, level, depth);
        ++level;
    }
    int index = static_cast<int>(diagramNodes.size());
    std::string name = specializationName;
    if (name.empty())
    {
        name = MakeFullClassName(classElement);
    }
    bool subject = false;
    bool hasDerivedNodes = level < 0;
    std::string link;
    if (libraryPrefix.empty())
    {
        link.append(classElement->GetAttribute("docPath"));
    }
    else
    {
        link.append(util::Path::Combine(libraryPrefix, classElement->GetAttribute("extPath")));
    }
    diagramNodes.push_back(DiagramNode(index, level, name, subject, hasDerivedNodes, link));
}

void GenerateRefinementDiagramNodes(soul::xml::Element* conceptElement, soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs,
    std::vector<DiagramNode>& diagramNodes, int& level, int& depth)
{
    soul::xml::Element* refinedConceptElement = GetRefinedConcept(conceptElement, moduleXmlDoc, otherModuleXmlDocs);
    if (refinedConceptElement)
    {
        --level;
        ++depth;
        GenerateRefinementDiagramNodes(refinedConceptElement, moduleXmlDoc, otherModuleXmlDocs, diagramNodes, level, depth);
        ++level;
    }
    int index = static_cast<int>(diagramNodes.size());
    std::string name = MakeFullConceptName(conceptElement);
    bool subject = false;
    bool hasDerivedNodes = level < 0;
    std::string link = conceptElement->GetAttribute("docPath");
    diagramNodes.push_back(DiagramNode(index, level, name, subject, hasDerivedNodes, link));
}

void AppendSpecifiers(soul::xml::Element* parentElement, const std::string& specifiers, const std::string& classifier, bool space)
{
    std::vector<std::string> components = util::Split(specifiers, ' ');
    int n = static_cast<int>(components.size());
    if (n > 0)
    {
        for (int i = 0; i < n; ++i)
        {
            if (i > 0)
            {
                parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
            }
            const std::string& specifier = components[i];
            std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
            span->SetAttribute("class", "kw");
            span->AppendChild(soul::xml::MakeText(specifier));
            parentElement->AppendChild(span.release());
        }
        if (space || !classifier.empty())
        {
            parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        }
    }
    if (!classifier.empty())
    {
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("class", "kw");
        span->AppendChild(soul::xml::MakeText(classifier));
        parentElement->AppendChild(span.release());
        if (space)
        {
            parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        }
    }
}

bool AppendDescription(soul::xml::Element* parentElement, Input* input, const std::string& docId, soul::xml::Document* docs,
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    if (!docs) return false;
    bool appended = false;
    soul::xml::Element* docElement = docs->GetElementById(docId);
    if (docElement)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> descriptionNodeSet = soul::xml::xpath::EvaluateToNodeSet("desc", docElement);
        if (descriptionNodeSet->Count() == 1)
        {
            soul::xml::Node* descriptionNode = descriptionNodeSet->GetNode(0);
            if (descriptionNode->IsElementNode())
            {
                soul::xml::Element* descriptionElement = static_cast<soul::xml::Element*>(descriptionNode);
                ContentVisitor visitor(input, parentElement, moduleXmlDoc, otherModuleXmlDocs, prefix);
                descriptionElement->Accept(visitor);
                appended = true;
            }
        }
    }
    return appended;
}

std::unique_ptr<soul::xml::Element> GetDescription(Input* input, const std::string& docId, soul::xml::Document* docs,
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    if (!docs) return std::unique_ptr<soul::xml::Element>();
    std::unique_ptr<soul::xml::Element> descriptionParagraph;
    soul::xml::Element* docElement = docs->GetElementById(docId);
    if (docElement)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> descriptionNodeSet = soul::xml::xpath::EvaluateToNodeSet("desc", docElement);
        if (descriptionNodeSet->Count() == 1)
        {
            soul::xml::Node* descriptionNode = descriptionNodeSet->GetNode(0);
            if (descriptionNode->IsElementNode())
            {
                soul::xml::Element* descriptionElement = static_cast<soul::xml::Element*>(descriptionNode);
                descriptionParagraph.reset(soul::xml::MakeElement("p"));
                descriptionParagraph->SetAttribute("xml:space", "preserve");
                descriptionParagraph->SetAttribute("class", "description");
                ContentVisitor visitor(input, descriptionParagraph.get(), moduleXmlDoc, otherModuleXmlDocs, prefix);
                descriptionElement->Accept(visitor);
            }
        }
    }
    return descriptionParagraph;
}

std::unique_ptr<soul::xml::Element> GetDetails(Input* input, const std::string& docId, soul::xml::Document* docs,
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    if (!docs) return std::unique_ptr<soul::xml::Element>();
    try
    {
        std::unique_ptr<soul::xml::Element> detailParagraphs;
        soul::xml::Element* docElement = docs->GetElementById(docId);
        if (docElement)
        {
            std::unique_ptr<soul::xml::xpath::NodeSet> detailsNodeSet = soul::xml::xpath::EvaluateToNodeSet("details", docElement);
            int n = detailsNodeSet->Count();
            if (n > 0)
            {
                detailParagraphs.reset(soul::xml::MakeElement("span"));
                detailParagraphs->SetAttribute("xml:space", "preserve");
                for (int i = 0; i < n; ++i)
                {
                    soul::xml::Node* detailNode = detailsNodeSet->GetNode(i);
                    if (detailNode->IsElementNode())
                    {
                        soul::xml::Element* detailElement = static_cast<soul::xml::Element*>(detailNode);
                        std::string title = detailElement->GetAttribute("title");
                        if (!title.empty())
                        {
                            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
                            h2Element->AppendChild(soul::xml::MakeText(title));
                            detailParagraphs->AppendChild(h2Element.release());
                        }
                        std::unique_ptr<soul::xml::Element> detailParagraph(soul::xml::MakeElement("p"));
                        detailParagraph->SetAttribute("xml:space", "preserve");
                        detailParagraph->SetAttribute("class", "description");
                        ContentVisitor visitor(input, detailParagraph.get(), moduleXmlDoc, otherModuleXmlDocs, prefix);
                        detailElement->Accept(visitor);
                        detailParagraphs->AppendChild(detailParagraph.release());
                    }
                }
            }
        }
        return detailParagraphs;
    }
    catch (...)
    {
    }
    return std::unique_ptr<soul::xml::Element>();
}

std::unique_ptr<soul::xml::Element> GetDescriptionAndDetails(Input* input, const std::string& docId, soul::xml::Document* docs,
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    if (!docs) return std::unique_ptr<soul::xml::Element>();
    std::unique_ptr<soul::xml::Element> descriptionParagraph = GetDescription(input, docId, docs, moduleXmlDoc, otherModuleXmlDocs, prefix);
    if (!descriptionParagraph) return std::unique_ptr<soul::xml::Element>();
    std::unique_ptr<soul::xml::Element> detailParagraphs = GetDetails(input, docId, docs, moduleXmlDoc, otherModuleXmlDocs, prefix);
    if (!detailParagraphs) return descriptionParagraph;
    std::unique_ptr<soul::xml::Element> allParagraphs(soul::xml::MakeElement("span"));
    allParagraphs->SetAttribute("xml:space", "preserve");
    allParagraphs->AppendChild(descriptionParagraph.release());
    allParagraphs->AppendChild(detailParagraphs.release());
    return allParagraphs;
}

void AppendType(Input* input, soul::xml::Element* parentElement, const std::string& typeId, const std::string& typeName,
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    std::string libraryPrefix;
    soul::xml::Element* typeElement = moduleXmlDoc->GetElementById(typeId);
    if (!typeElement)
    {
        int n = static_cast<int>(otherModuleXmlDocs.size());
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Document* otherModuleXmlDoc = otherModuleXmlDocs[i];
            typeElement = otherModuleXmlDoc->GetElementById(typeId);
            if (typeElement)
            {
                std::string moduleName = otherModuleXmlDoc->DocumentElement()->GetAttribute("module");
                auto it = input->libraryPrefixMap.find(moduleName);
                if (it != input->libraryPrefixMap.cend())
                {
                    libraryPrefix = it->second;
                    if (!libraryPrefix.empty())
                    {
                        libraryPrefix = util::Path::Combine("../../..", libraryPrefix);
                    }
                }
                else
                {
                    continue;
                }
                break;
            }
        }
    }
    if (typeElement)
    {
        if (typeElement->Name() == "basicType")
        {
            std::string typeName = typeElement->GetAttribute("name");
            if (typeName.empty())
            {
                parentElement->AppendChild(soul::xml::MakeText("BASIC_TYPE"));
            }
            else
            {
                std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
                span->SetAttribute("class", "kw");
                span->AppendChild(soul::xml::MakeText(typeName));
                parentElement->AppendChild(span.release());
            }
        }
        else if (typeElement->Name() == "arrayType")
        {
            std::string elementType = typeElement->GetAttribute("elementType");
            if (elementType.empty())
            {
                parentElement->AppendChild(soul::xml::MakeText("ELEMENT_TYPE"));
            }
            else
            {
                AppendType(input, parentElement, elementType, "ELEMENT_TYPE", moduleXmlDoc, otherModuleXmlDocs, prefix);
            }
            parentElement->AppendChild(soul::xml::MakeText("["));
            std::string size = typeElement->GetAttribute("size");
            if (!size.empty())
            {
                parentElement->AppendChild(soul::xml::MakeText(size));
            }
            parentElement->AppendChild(soul::xml::MakeText("]"));
        }
        else if (typeElement->Name() == "derivedType")
        {
            std::unique_ptr<soul::xml::xpath::NodeSet> derivationSet = soul::xml::xpath::EvaluateToNodeSet("derivation", typeElement);
            int n = derivationSet->Count();
            if (n > 0)
            {
                for (int i = 0; i < n; ++i)
                {
                    soul::xml::Node* derivationNode = derivationSet->GetNode(i);
                    if (derivationNode->IsElementNode())
                    {
                        soul::xml::Element* derivationElement = static_cast<soul::xml::Element*>(derivationNode);
                        std::string baseType = derivationElement->GetAttribute("baseType");
                        if (!baseType.empty())
                        {
                            AppendType(input, parentElement, baseType, "BASE_TYPE", moduleXmlDoc, otherModuleXmlDocs, prefix);
                        }
                        else
                        {
                            std::string derivationName = derivationElement->GetAttribute("name");
                            if (derivationName.empty())
                            {
                                parentElement->AppendChild(soul::xml::MakeText("DERIVATION"));
                            }
                            else if (derivationName == "const")
                            {
                                std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
                                span->SetAttribute("class", "kw");
                                span->AppendChild(soul::xml::MakeText(derivationName));
                                parentElement->AppendChild(span.release());
                                parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                            }
                            else
                            {
                                parentElement->AppendChild(soul::xml::MakeText(derivationName));
                            }
                        }
                    }
                }
            }
            else
            {
                parentElement->AppendChild(soul::xml::MakeText(typeElement->GetAttribute("name")));
            }
        }
        else if (typeElement->Name() == "specialization")
        {
            std::string primaryType = typeElement->GetAttribute("primaryType");
            AppendType(input, parentElement, primaryType, "PRIMARY_TYPE", moduleXmlDoc, otherModuleXmlDocs, prefix);
            parentElement->AppendChild(soul::xml::MakeText("<"));
            std::unique_ptr<soul::xml::xpath::NodeSet> templateArgumentTypeNodeSet = soul::xml::xpath::EvaluateToNodeSet(
                "templateArgumentTypes/templateArgumentType", typeElement);
            int n = templateArgumentTypeNodeSet->Count();
            for (int i = 0; i < n; ++i)
            {
                if (i > 0)
                {
                    parentElement->AppendChild(soul::xml::MakeText(", "));
                }
                soul::xml::Node* templateArgumentTypeNode = templateArgumentTypeNodeSet->GetNode(i);
                if (templateArgumentTypeNode->IsElementNode())
                {
                    soul::xml::Element* templateArgumentTypeElement = static_cast<soul::xml::Element*>(templateArgumentTypeNode);
                    std::string templateArgumentType = templateArgumentTypeElement->GetAttribute("ref");
                    if (templateArgumentType.empty())
                    {
                        parentElement->AppendChild(soul::xml::MakeText("TEMPLATE_ARGUMENT_TYPE"));
                    }
                    else
                    {
                        AppendType(input, parentElement, templateArgumentType, "TEMPLATE_ARGUMENT_TYPE", moduleXmlDoc, otherModuleXmlDocs, prefix);
                    }
                }
            }
            parentElement->AppendChild(soul::xml::MakeText(">"));
        }
        else if (typeElement->Name() == "class" || typeElement->Name() == "delegate" || typeElement->Name() == "classDelegate" || typeElement->Name() == "enumType" ||
            typeElement->Name() == "interface" || typeElement->Name() == "templateParameter")
        {
            std::string link;
            if (libraryPrefix.empty())
            {
                link.append(prefix).append(typeElement->GetAttribute("docPath"));
            }
            else
            {
                link.append(prefix).append(util::Path::Combine(libraryPrefix, typeElement->GetAttribute("extPath")));
            }
            std::string name = typeElement->GetAttribute("name");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", link);
            linkElement->AppendChild(soul::xml::MakeText(name));
            parentElement->AppendChild(linkElement.release());
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("TYPE"));
        }
    }
    else
    {
        Input* input = GetInputPtr();
        ++input->errors;
        if (typeName.empty())
        {
            parentElement->AppendChild(soul::xml::MakeText("TYPE"));
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText(typeName));
        }
    }
}

void AppendParameters(Input* input, soul::xml::Element* parentElement, soul::xml::Element* functionElement, soul::xml::Document* moduleXmlDoc,
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    parentElement->AppendChild(soul::xml::MakeText("("));
    std::unique_ptr<soul::xml::xpath::NodeSet> parameterNodeSet = soul::xml::xpath::EvaluateToNodeSet("parameters/parameter", functionElement);
    int n = parameterNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            parentElement->AppendChild(soul::xml::MakeText(", "));
        }
        soul::xml::Node* parameterNode = parameterNodeSet->GetNode(i);
        if (parameterNode->IsElementNode())
        {
            soul::xml::Element* parameterElement = static_cast<soul::xml::Element*>(parameterNode);
            std::string typeId = parameterElement->GetAttribute("type");
            AppendType(input, parentElement, typeId, "TYPE", moduleXmlDoc, otherModuleXmlDocs, prefix);
            std::string name = parameterElement->GetAttribute("name");
            if (!name.empty())
            {
                parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                parentElement->AppendChild(soul::xml::MakeText(name));
            }
        }
    }
    parentElement->AppendChild(soul::xml::MakeText(")"));
}

void AppendSymbol(Input* input, soul::xml::Element* parentElement, soul::xml::Element* symbolElement, soul::xml::Document* moduleXmlDoc, 
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    std::string libraryPrefix;
    std::string elementName = symbolElement->Name();
    if (elementName == "symbol")
    {
        std::string ref = symbolElement->GetAttribute("ref");
        std::string link;
        soul::xml::Element* typeOrConceptElement = nullptr;
        if (!ref.empty())
        {
            typeOrConceptElement = moduleXmlDoc->GetElementById(ref);
            if (!typeOrConceptElement)
            {
                int n = static_cast<int>(otherModuleXmlDocs.size());
                for (int i = 0; i < n; ++i)
                {
                    soul::xml::Document* otherModuleXmlDoc = otherModuleXmlDocs[i];
                    typeOrConceptElement = otherModuleXmlDoc->GetElementById(ref);
                    if (typeOrConceptElement)
                    {
                        std::string moduleName = otherModuleXmlDoc->DocumentElement()->GetAttribute("module");
                        auto it = input->libraryPrefixMap.find(moduleName);
                        if (it != input->libraryPrefixMap.cend())
                        {
                            libraryPrefix = it->second;
                            if (!libraryPrefix.empty())
                            {
                                libraryPrefix = util::Path::Combine("../../..", libraryPrefix);
                            }
                        }
                        else
                        {
                            continue;
                        }
                        break;
                    }
                }
            }
        }
        if (typeOrConceptElement)
        {
            if (libraryPrefix.empty())
            {
                link.append(prefix).append(typeOrConceptElement->GetAttribute("docPath"));
            }
            else
            {
                link.append(prefix).append(util::Path::Combine(libraryPrefix, typeOrConceptElement->GetAttribute("extPath")));
            }
            std::string name = typeOrConceptElement->GetAttribute("name");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", link);
            linkElement->AppendChild(soul::xml::MakeText(name));
            parentElement->AppendChild(linkElement.release());
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText(symbolElement->GetAttribute("name")));
        }
    }
    else if (!elementName.empty())
    {
        parentElement->AppendChild(soul::xml::MakeText(elementName));
    }
    else
    {
        parentElement->AppendChild(soul::xml::MakeText("SYMBOL_ELEMENT"));
    }
}

void AppendConstraint(Input* input, soul::xml::Element* parentElement, soul::xml::Element* element, soul::xml::Document* moduleXmlDoc,
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix, bool wherePrefix)
{
    std::string elementName = element->Name();
    if (elementName == "where")
    {
        if (wherePrefix)
        {
            std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
            span->SetAttribute("class", "kw");
            span->AppendChild(soul::xml::MakeText("where"));
            parentElement->AppendChild(span.release());
            parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        }
        soul::xml::Node* node = element->FirstChild();
        if (node && node->IsElementNode())
        {
            soul::xml::Element* subElement = static_cast<soul::xml::Element*>(node);
            AppendConstraint(input, parentElement, subElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("WHERE_CHILD"));
        }
    }
    else if (elementName == "is")
    {
        soul::xml::Node* firstChildNode = element->FirstChild();
        if (firstChildNode && firstChildNode->IsElementNode())
        {
            soul::xml::Element* typeElement = static_cast<soul::xml::Element*>(firstChildNode);
            AppendConstraint(input, parentElement, typeElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("IS_LEFT_CHILD"));
        }
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("class", "kw");
        parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        span->AppendChild(soul::xml::MakeText("is"));
        parentElement->AppendChild(span.release());
        parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        soul::xml::Node* secondChildNode = firstChildNode->Next();
        if (secondChildNode && secondChildNode->IsElementNode())
        {
            soul::xml::Element* typeOrConceptElement = static_cast<soul::xml::Element*>(secondChildNode);
            AppendConstraint(input, parentElement, typeOrConceptElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("IS_RIGHT_CHILD"));
        }
    }
    else if (elementName == "multi")
    {
        soul::xml::Node* firstChildNode = element->FirstChild();
        if (firstChildNode && firstChildNode->IsElementNode())
        {
            soul::xml::Element* conceptElement = static_cast<soul::xml::Element*>(firstChildNode);
            AppendConstraint(input, parentElement, conceptElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("MULTI_CHILD"));
        }
        soul::xml::Node* secondChildNode = firstChildNode->Next();
        if (secondChildNode && secondChildNode->IsElementNode())
        {
            soul::xml::Element* typesElement = static_cast<soul::xml::Element*>(secondChildNode);
            if (typesElement->Name() == "types")
            {
                parentElement->AppendChild(soul::xml::MakeText("<"));
                soul::xml::Node* typeChildNode = typesElement->FirstChild();
                bool first = true;
                while (typeChildNode)
                {
                    if (typeChildNode->IsElementNode())
                    {
                        if (first)
                        {
                            first = false;
                        }
                        else
                        {
                            parentElement->AppendChild(soul::xml::MakeText(", "));
                        }
                        soul::xml::Element* typeElement = static_cast<soul::xml::Element*>(typeChildNode);
                        AppendConstraint(input, parentElement, typeElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
                    }
                    typeChildNode = typeChildNode->Next();
                }
                parentElement->AppendChild(soul::xml::MakeText(">"));
            }
            else
            {
                parentElement->AppendChild(soul::xml::MakeText("MULTI_TYPES"));
            }
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("MULTI_TYPES"));
        }
    }
    else if (elementName == "and")
    {
        soul::xml::Node* firstChildNode = element->FirstChild();
        if (firstChildNode && firstChildNode->IsElementNode())
        {
            soul::xml::Element* costraintElement = static_cast<soul::xml::Element*>(firstChildNode);
            AppendConstraint(input, parentElement, costraintElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("AND_LEFT_CHILD"));
        }
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("class", "kw");
        parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        span->AppendChild(soul::xml::MakeText("and"));
        parentElement->AppendChild(span.release());
        parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        soul::xml::Node* secondChildNode = firstChildNode->Next();
        if (secondChildNode && secondChildNode->IsElementNode())
        {
            soul::xml::Element* costraintElement = static_cast<soul::xml::Element*>(secondChildNode);
            AppendConstraint(input, parentElement, costraintElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("AND_RIGHT_CHILD"));
        }
    }
    else if (elementName == "or")
    {
        soul::xml::Node* firstChildNode = element->FirstChild();
        if (firstChildNode && firstChildNode->IsElementNode())
        {
            soul::xml::Element* costraintElement = static_cast<soul::xml::Element*>(firstChildNode);
            AppendConstraint(input, parentElement, costraintElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("OR_LEFT_CHILD"));
        }
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("class", "kw");
        parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        span->AppendChild(soul::xml::MakeText("or"));
        parentElement->AppendChild(span.release());
        parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        soul::xml::Node* secondChildNode = firstChildNode->Next();
        if (secondChildNode && secondChildNode->IsElementNode())
        {
            soul::xml::Element* costraintElement = static_cast<soul::xml::Element*>(secondChildNode);
            AppendConstraint(input, parentElement, costraintElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("OR_RIGHT_CHILD"));
        }
    }
    else if (elementName == "dot")
    {
        soul::xml::Node* firstChildNode = element->FirstChild();
        if (firstChildNode && firstChildNode->IsElementNode())
        {
            soul::xml::Element* costraintElement = static_cast<soul::xml::Element*>(firstChildNode);
            AppendConstraint(input, parentElement, costraintElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("DOT_LEFT_CHILD"));
        }
        parentElement->AppendChild(soul::xml::MakeText("."));
        soul::xml::Node* secondChildNode = firstChildNode->Next();
        if (secondChildNode && secondChildNode->IsElementNode())
        {
            soul::xml::Element* costraintElement = static_cast<soul::xml::Element*>(secondChildNode);
            AppendConstraint(input, parentElement, costraintElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("DOT_RIGHT_CHILD"));
        }
    }
    else if (elementName == "symbol")
    {
        AppendSymbol(input, parentElement, element, moduleXmlDoc, otherModuleXmlDocs, prefix);
    }
    else if (elementName == "typename")
    {
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("class", "kw");
        span->AppendChild(soul::xml::MakeText("typename"));
        parentElement->AppendChild(span.release());
        parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        soul::xml::Node* firstChildNode = element->FirstChild();
        if (firstChildNode && firstChildNode->IsElementNode())
        {
            soul::xml::Element* costraintElement = static_cast<soul::xml::Element*>(firstChildNode);
            AppendConstraint(input, parentElement, costraintElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("TYPENAME_CHILD"));
        }
    }
    else if (elementName == "keyword")
    {
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("class", "kw");
        span->AppendChild(soul::xml::MakeText(element->GetAttribute("value")));
        parentElement->AppendChild(span.release());
    }
    else if (elementName == "signature")
    {
        parentElement->AppendChild(soul::xml::MakeText(element->GetAttribute("value")));
    }
    else if (elementName == "predicate")
    {
        parentElement->AppendChild(soul::xml::MakeText(element->GetAttribute("value")));
    }
    else if (elementName == "parens")
    {
        parentElement->AppendChild(soul::xml::MakeText("("));
        soul::xml::Node* firstChildNode = element->FirstChild();
        if (firstChildNode && firstChildNode->IsElementNode())
        {
            soul::xml::Element* costraintElement = static_cast<soul::xml::Element*>(firstChildNode);
            AppendConstraint(input, parentElement, costraintElement, moduleXmlDoc, otherModuleXmlDocs, prefix, wherePrefix);
        }
        else
        {
            parentElement->AppendChild(soul::xml::MakeText("PARENS_CHILD"));
        }
        parentElement->AppendChild(soul::xml::MakeText(")"));
    }
    else if (!elementName.empty())
    {
        parentElement->AppendChild(soul::xml::MakeText(elementName));
    }
    else
    {
        parentElement->AppendChild(soul::xml::MakeText("CONSTRAINT_ELEMENT"));
    }
}

void AppendAxiomParameters(Input* input, soul::xml::Element* parentElement, soul::xml::Element* axiomElement, soul::xml::Document* moduleXmlDoc, 
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    parentElement->AppendChild(soul::xml::MakeText("("));
    std::unique_ptr<soul::xml::xpath::NodeSet> parameterNodeSet = soul::xml::xpath::EvaluateToNodeSet("parameters/parameter", axiomElement);
    int n = parameterNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            parentElement->AppendChild(soul::xml::MakeText(", "));
        }
        soul::xml::Node* parameterNode = parameterNodeSet->GetNode(i);
        if (parameterNode->IsElementNode())
        {
            soul::xml::Element* parameterElement = static_cast<soul::xml::Element*>(parameterNode);
            soul::xml::Node* firstChildNode = parameterElement->FirstChild();
            if (firstChildNode && firstChildNode->IsElementNode())
            {
                soul::xml::Element* symbolElement = static_cast<soul::xml::Element*>(firstChildNode);
                AppendSymbol(input, parentElement, symbolElement, moduleXmlDoc, otherModuleXmlDocs, prefix);
                soul::xml::Node* secondChildNode = firstChildNode->Next();
                if (secondChildNode && secondChildNode->IsElementNode())
                {
                    soul::xml::Element* symbolElement = static_cast<soul::xml::Element*>(secondChildNode);
                    parentElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                    AppendSymbol(input, parentElement, symbolElement, moduleXmlDoc, otherModuleXmlDocs, prefix);
                }
            }

        }
    }
    parentElement->AppendChild(soul::xml::MakeText(")"));
}

void GenerateFunctionDoc(Input* input, soul::xml::Element* parentElement, soul::xml::Element* functionElement, soul::xml::Document* moduleXmlDoc, 
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    soul::xml::Document* docs = input->docs.get();
    std::unique_ptr<soul::xml::Element> h3Element(soul::xml::MakeElement("h3"));
    h3Element->SetAttribute("xml:space", "preserve");
    h3Element->AppendChild(soul::xml::MakeText(functionElement->GetAttribute("name")));
    h3Element->SetAttribute("id", functionElement->GetAttribute("id"));
    h3Element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
    h3Element->AppendChild(soul::xml::MakeText(functionElement->GetAttribute("kind")));
    parentElement->AppendChild(h3Element.release());
    std::unique_ptr<soul::xml::Element> h4Element(soul::xml::MakeElement("h4"));
    h4Element->SetAttribute("xml:space", "preserve");
    AppendSpecifiers(h4Element.get(), functionElement->GetAttribute("specifiers"), std::string(), true);
    std::string returnTypeId = functionElement->GetAttribute("returnType");
    if (!returnTypeId.empty())
    {
        AppendType(input, h4Element.get(), returnTypeId, "RETURN_TYPE", moduleXmlDoc, otherModuleXmlDocs, prefix);
        h4Element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> templateParameterNodeSet;
    std::vector<soul::xml::Element*> templateParameterElements;
    h4Element->AppendChild(soul::xml::MakeText(MakeFunctionName(functionElement, templateParameterNodeSet, templateParameterElements)));
    AppendParameters(input, h4Element.get(), functionElement, moduleXmlDoc, otherModuleXmlDocs, prefix);
    parentElement->AppendChild(h4Element.release());
    if (docs)
    {
        std::unique_ptr<soul::xml::Element> descriptionAndDetails =
            GetDescriptionAndDetails(input, functionElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, prefix);
        if (descriptionAndDetails)
        {
            parentElement->AppendChild(descriptionAndDetails.release());
        }
    }
    bool br = false;
    int ntp = static_cast<int>(templateParameterElements.size());
    if (ntp > 0)
    {
        br = true;
        std::unique_ptr<soul::xml::Element> h5Element(soul::xml::MakeElement("h5"));
        h5Element->AppendChild(soul::xml::MakeText("Template Parameters"));
        parentElement->AppendChild(h5Element.release());
        std::unique_ptr<soul::xml::Element> templateParameterTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ntp; ++i)
        {
            soul::xml::Element* templateParameterElement = templateParameterElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            trElement->SetAttribute("id", templateParameterElement->GetAttribute("id"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            tdElement->AppendChild(soul::xml::MakeText(templateParameterElement->GetAttribute("name")));
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            std::string defaultTypeId = templateParameterElement->GetAttribute("default");
            if (!defaultTypeId.empty())
            {
                AppendType(input, td2Element.get(), defaultTypeId, "DEFAULT_TYPE", moduleXmlDoc, otherModuleXmlDocs, prefix);
            }
            trElement->AppendChild(td2Element.release());
            templateParameterTableElement->AppendChild(trElement.release());
        }
        parentElement->AppendChild(templateParameterTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> constraintNodeSet;
    soul::xml::Element* constraintElement = GetConstraint(functionElement, constraintNodeSet);
    if (constraintElement)
    {
        br = true;
        std::unique_ptr<soul::xml::Element> h5Element(soul::xml::MakeElement("h5"));
        h5Element->AppendChild(soul::xml::MakeText("Constraint"));
        parentElement->AppendChild(h5Element.release());
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("class", "indent");
        AppendConstraint(input, span.get(), constraintElement, moduleXmlDoc, otherModuleXmlDocs, prefix, false);
        parentElement->AppendChild(span.release());
    }
    std::string filePath = functionElement->GetAttribute("filePath");
    std::string line = functionElement->GetAttribute("line");
    std::string fileName = functionElement->GetAttribute("fileName");
    if (!filePath.empty() && !line.empty() && !fileName.empty())
    {
        br = true;
        std::unique_ptr<soul::xml::Element> h5Element(soul::xml::MakeElement("h5"));
        h5Element->AppendChild(soul::xml::MakeText("Definition"));
        parentElement->AppendChild(h5Element.release());
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("xml:space", "preserve");
        span->SetAttribute("class", "indent");
        span->AppendChild(soul::xml::MakeText("Line"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> lineLinkElement(soul::xml::MakeElement("a"));
        lineLinkElement->SetAttribute("href", prefix + filePath + "#" + line);
        lineLinkElement->AppendChild(soul::xml::MakeText(line));
        span->AppendChild(lineLinkElement.release());
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        span->AppendChild(soul::xml::MakeText("of"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> fileLinkElement(soul::xml::MakeElement("a"));
        fileLinkElement->SetAttribute("href", prefix + filePath);
        fileLinkElement->AppendChild(soul::xml::MakeText(fileName));
        span->AppendChild(fileLinkElement.release());
        parentElement->AppendChild(span.release());
    }
    if (br)
    {
        parentElement->AppendChild(soul::xml::MakeElement("br"));
        parentElement->AppendChild(soul::xml::MakeElement("br"));
    }
    parentElement->AppendChild(soul::xml::MakeElement("hr"));
}

void GenerateFunctionDetailsSection(Input* input, soul::xml::Element* parentElement, const std::string& title, const std::vector<soul::xml::Element*> functionElements,
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs, const std::string& prefix)
{
    if (functionElements.empty()) return;
    std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
    h2Element->AppendChild(soul::xml::MakeText(title));
    parentElement->AppendChild(h2Element.release());
    int n = static_cast<int>(functionElements.size());
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Element* functionElement(functionElements[i]);
        GenerateFunctionDoc(input, parentElement, functionElement, moduleXmlDoc, otherModuleXmlDocs, prefix);
    }
}

void GenerateEnumDoc(Input* input, const std::string& docDir, soul::xml::Element* enumTypeElement, 
    soul::xml::Document* moduleXmlDoc, const std::vector<soul::xml::Document*>& otherModuleXmlDocs)
{
    soul::xml::Document* docs = input->docs.get();
    std::string enumName = enumTypeElement->GetAttribute("name");
    enumName.append(" Enumerated Type");
    std::string title = enumName;
    std::string styleFilePath = "../../../style/style.css";
    soul::xml::Document doc;
    std::unique_ptr<soul::xml::Element> htmlElement(soul::xml::MakeElement("html"));
    std::unique_ptr<soul::xml::Element> headElement(soul::xml::MakeElement("head"));
    std::unique_ptr<soul::xml::Element> metaElement(soul::xml::MakeElement("meta"));
    metaElement->SetAttribute("charset", "utf-8");
    headElement->AppendChild(metaElement.release());
    std::unique_ptr<soul::xml::Element> titleElement(soul::xml::MakeElement("title"));
    titleElement->AppendChild(soul::xml::MakeText(title));
    headElement->AppendChild(titleElement.release());
    std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("link"));
    linkElement->SetAttribute("rel", "stylesheet");
    linkElement->SetAttribute("type", "text/css");
    linkElement->SetAttribute("href", styleFilePath);
    headElement->AppendChild(linkElement.release());
    htmlElement->AppendChild(headElement.release());
    std::unique_ptr<soul::xml::Element> bodyElement(soul::xml::MakeElement("body"));
    std::unique_ptr<soul::xml::Element> h1Element(soul::xml::MakeElement("h1"));
    h1Element->AppendChild(soul::xml::MakeText(title));
    bodyElement->AppendChild(h1Element.release());
    if (docs)
    {
        std::unique_ptr<soul::xml::Element> descriptionAndDetails = GetDescriptionAndDetails(input, enumTypeElement->GetAttribute("id"), docs, 
            moduleXmlDoc, otherModuleXmlDocs, std::string());
        if (descriptionAndDetails)
        {
            bodyElement->AppendChild(descriptionAndDetails.release());
        }
    }
    std::string filePath = enumTypeElement->GetAttribute("filePath");
    std::string line = enumTypeElement->GetAttribute("line");
    std::string fileName = enumTypeElement->GetAttribute("fileName");
    if (!filePath.empty() && !line.empty() && !fileName.empty())
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Definition"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("xml:space", "preserve");
        span->SetAttribute("class", "indent");
        span->AppendChild(soul::xml::MakeText("Line"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> lineLinkElement(soul::xml::MakeElement("a"));
        lineLinkElement->SetAttribute("href", filePath + "#" + line);
        lineLinkElement->AppendChild(soul::xml::MakeText(line));
        span->AppendChild(lineLinkElement.release());
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        span->AppendChild(soul::xml::MakeText("of"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> fileLinkElement(soul::xml::MakeElement("a"));
        fileLinkElement->SetAttribute("href", filePath);
        fileLinkElement->AppendChild(soul::xml::MakeText(fileName));
        span->AppendChild(fileLinkElement.release());
        bodyElement->AppendChild(span.release());
    }
    std::string underlyingTypeId = enumTypeElement->GetAttribute("underlyingType");
    if (!underlyingTypeId.empty())
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Underlying Type"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("xml:space", "preserve");
        span->SetAttribute("class", "indent");
        AppendType(input, span.get(), underlyingTypeId, "UNDERLYING_TYPE", moduleXmlDoc, otherModuleXmlDocs, std::string());
        bodyElement->AppendChild(span.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> enumConstantNodeSet;
    std::vector<soul::xml::Element*> enumConstantElements;
    GetEnumConstants(enumTypeElement, enumConstantNodeSet, enumConstantElements);
    int n = static_cast<int>(enumConstantElements.size());
    if (n > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Enumeration Constants"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> enumConstantTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Element* enumConstantElement = enumConstantElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            tdElement->AppendChild(soul::xml::MakeText(enumConstantElement->GetAttribute("name")));
            tdElement->SetAttribute("id", enumConstantElement->GetAttribute("id"));
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            std::string value = enumConstantElement->GetAttribute("value");
            if (!value.empty())
            {
                td2Element->AppendChild(soul::xml::MakeText(value));
            }
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, enumConstantElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            enumConstantTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(enumConstantTableElement.release());
    }
    htmlElement->AppendChild(bodyElement.release());
    doc.AppendChild(htmlElement.release());
    std::string docFilePath = util::GetFullPath(util::Path::Combine(docDir, enumTypeElement->GetAttribute("id") + ".html"));
    std::ofstream docFile(docFilePath);
    util::CodeFormatter formatter(docFile);
    formatter.SetIndentSize(1);
    doc.Write(formatter);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        std::cout << "==> " << docFilePath << std::endl;
    }
}

void GenerateClassDoc(Input* input, const std::string& docDir, soul::xml::Element* classElement, soul::xml::Document* moduleXmlDoc,
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs)
{
    soul::xml::Document* docs = input->docs.get();
    std::unique_ptr<soul::xml::xpath::NodeSet> templateParameterNodeSet;
    std::vector<soul::xml::Element*> templateParameterElements;
    std::string className = MakeClassName(classElement, templateParameterNodeSet, templateParameterElements);
    std::string specifiers = classElement->GetAttribute("specifiers");
    if (specifiers.find("abstract") != specifiers.npos)
    {
        className.append(" Abstract");
    }
    if (specifiers.find("static") != specifiers.npos)
    {
        className.append(" Static");
    }
    className.append(" Class");
    std::string title = className;
    std::string styleFilePath = "../../../style/style.css";
    soul::xml::Document doc;
    std::unique_ptr<soul::xml::Element> htmlElement(soul::xml::MakeElement("html"));
    std::unique_ptr<soul::xml::Element> headElement(soul::xml::MakeElement("head"));
    std::unique_ptr<soul::xml::Element> metaElement(soul::xml::MakeElement("meta"));
    metaElement->SetAttribute("charset", "utf-8");
    headElement->AppendChild(metaElement.release());
    std::unique_ptr<soul::xml::Element> titleElement(soul::xml::MakeElement("title"));
    titleElement->AppendChild(soul::xml::MakeText(title));
    headElement->AppendChild(titleElement.release());
    std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("link"));
    linkElement->SetAttribute("rel", "stylesheet");
    linkElement->SetAttribute("type", "text/css");
    linkElement->SetAttribute("href", styleFilePath);
    headElement->AppendChild(linkElement.release());
    bool diagramCodeGenerated = false;
    std::string svgId;
    BaseClassInfo baseClassInfo = GetBaseClass(input, classElement, moduleXmlDoc, otherModuleXmlDocs);
    std::vector<soul::xml::Element*> derivedClasses = GetDerivedClasses(classElement, moduleXmlDoc, otherModuleXmlDocs);
    if (baseClassInfo.baseClassElement || !derivedClasses.empty())
    {
        std::vector<DiagramNode> diagramNodes;
        int level = 0;
        int depth = 0;
        if (baseClassInfo.baseClassElement)
        {
            --level;
            ++depth;
            GenerateInheritanceDiagramNodes(input, baseClassInfo.baseClassElement, baseClassInfo.baseClassSpecializationName, baseClassInfo.libraryPrefix,
                moduleXmlDoc, otherModuleXmlDocs, diagramNodes, level, depth);
            ++level;
        }
        int index = static_cast<int>(diagramNodes.size());
        std::string name = MakeFullClassName(classElement);
        bool subject = true;
        bool hasDerivedNodes = !derivedClasses.empty();
        std::string link = classElement->GetAttribute("docPath");
        diagramNodes.push_back(DiagramNode(index, level, name, subject, hasDerivedNodes, link));
        ++level;
        int ndc = static_cast<int>(derivedClasses.size());
        for (int i = 0; i < ndc; ++i)
        {
            soul::xml::Element* derivedClassElement = derivedClasses[i];
            index = static_cast<int>(diagramNodes.size());
            name = MakeFullClassName(derivedClassElement);
            subject = false;
            hasDerivedNodes = false;
            link = derivedClassElement->GetAttribute("docPath");
            diagramNodes.push_back(DiagramNode(index, level, name, subject, hasDerivedNodes, link));
        }
        int n = static_cast<int>(diagramNodes.size());
        for (int i = 0; i < n; ++i)
        {
            DiagramNode& node = diagramNodes[i];
            node.level += depth;
        }
        std::sort(diagramNodes.begin(), diagramNodes.end(), DiagramNodeByLevelAndName());
        std::string scriptFileName = classElement->GetAttribute("id") + "_inheritance.js";
        std::string scriptFilePath = util::Path::Combine(docDir, scriptFileName);
        svgId = "inheritance_svg_diagram";
        GenerateDiagramCode(diagramNodes, "drawInheritanceDiagram", svgId, scriptFilePath);
        diagramCodeGenerated = true;
        std::unique_ptr<soul::xml::Element> scriptElement(soul::xml::MakeElement("script"));
        scriptElement->SetAttribute("type", "text/javascript");
        scriptElement->SetAttribute("src", scriptFileName);
        scriptElement->AppendChild(soul::xml::MakeText(" "));
        headElement->AppendChild(scriptElement.release());
    }
    htmlElement->AppendChild(headElement.release());
    std::unique_ptr<soul::xml::Element> bodyElement(soul::xml::MakeElement("body"));
    std::unique_ptr<soul::xml::Element> h1Element(soul::xml::MakeElement("h1"));
    h1Element->AppendChild(soul::xml::MakeText(title));
    bodyElement->AppendChild(h1Element.release());
    std::unique_ptr<soul::xml::Element> legendSpan(soul::xml::MakeElement("span"));
    legendSpan->SetAttribute("class", "indent");
    soul::xml::Element* legend = legendSpan.get();
    bool first = true;
    bodyElement->AppendChild(legendSpan.release());
    if (docs)
    {
        std::unique_ptr<soul::xml::Element> descriptionAndDetails = GetDescriptionAndDetails(input, classElement->GetAttribute("id"), docs, 
            moduleXmlDoc, otherModuleXmlDocs, std::string());
        if (descriptionAndDetails)
        {
            bodyElement->AppendChild(descriptionAndDetails.release());
        }
    }
    std::string filePath = classElement->GetAttribute("filePath");
    std::string line = classElement->GetAttribute("line");
    std::string fileName = classElement->GetAttribute("fileName");
    if (!filePath.empty() && !line.empty() && !fileName.empty())
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Definition"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("xml:space", "preserve");
        span->SetAttribute("class", "indent");
        span->AppendChild(soul::xml::MakeText("Line"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> lineLinkElement(soul::xml::MakeElement("a"));
        lineLinkElement->SetAttribute("href", filePath + "#" + line);
        lineLinkElement->AppendChild(soul::xml::MakeText(line));
        span->AppendChild(lineLinkElement.release());
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        span->AppendChild(soul::xml::MakeText("of"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> fileLinkElement(soul::xml::MakeElement("a"));
        fileLinkElement->SetAttribute("href", filePath);
        fileLinkElement->AppendChild(soul::xml::MakeText(fileName));
        span->AppendChild(fileLinkElement.release());
        bodyElement->AppendChild(span.release());
    }
    if (diagramCodeGenerated)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Inheritance"));
        bodyElement->AppendChild(h2Element.release());
        bodyElement->SetAttribute("onload", "drawInheritanceDiagram()");
        std::unique_ptr<soul::xml::Element> divElement(soul::xml::MakeElement("div"));
        divElement->SetAttribute("class", "diagram");
        std::unique_ptr<soul::xml::Element> svgElement(soul::xml::MakeElement("svg"));
        svgElement->SetAttribute("width", "0");
        svgElement->SetAttribute("height", "0");
        svgElement->SetAttribute("id", svgId);
        svgElement->SetAttribute("xmlns", "http://www.w3.org/2000/svg");
        svgElement->SetAttribute("version", "2.0");
        divElement->AppendChild(svgElement.release());
        bodyElement->AppendChild(divElement.release());
    }
    int ntp = static_cast<int>(templateParameterElements.size());
    if (ntp > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "templateParameters");
        AppendToLegend(legend, "Template Parameters", "templateParameters", first);
        h2Element->AppendChild(soul::xml::MakeText("Template Parameters"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> templateParameterTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ntp; ++i)
        {
            soul::xml::Element* templateParameterElement = templateParameterElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            trElement->SetAttribute("id", templateParameterElement->GetAttribute("id"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            tdElement->AppendChild(soul::xml::MakeText(templateParameterElement->GetAttribute("name")));
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            std::string defaultTypeId = templateParameterElement->GetAttribute("default");
            if (!defaultTypeId.empty())
            {
                AppendType(input, td2Element.get(), defaultTypeId, "DEFAULT_TYPE", moduleXmlDoc, otherModuleXmlDocs, std::string());
            }
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, templateParameterElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            templateParameterTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(templateParameterTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> constraintNodeSet;
    soul::xml::Element* constraintElement = GetConstraint(classElement, constraintNodeSet);
    if (constraintElement)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "constraint");
        AppendToLegend(legend, "Constraint", "constraint", first);
        h2Element->AppendChild(soul::xml::MakeText("Constraint"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("class", "indent");
        AppendConstraint(input, span.get(), constraintElement, 
            moduleXmlDoc, otherModuleXmlDocs, std::string(), false);
        bodyElement->AppendChild(span.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> classNodeSet;
    std::vector<soul::xml::Element*> classElements;
    GetClasses(classElement, classNodeSet, classElements);
    int n = static_cast<int>(classElements.size());
    if (n > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "classes");
        AppendToLegend(legend, "Classes", "classes", first);
        h2Element->AppendChild(soul::xml::MakeText("Classes"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> classTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Element* classElement = classElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), classElement->GetAttribute("specifiers"), "class", false);
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", classElement->GetAttribute("docPath"));
            linkElement->AppendChild(soul::xml::MakeText(MakeClassName(classElement, templateParameterNodeSet, templateParameterElements)));
            td2Element->AppendChild(linkElement.release());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, classElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            classTableElement->AppendChild(trElement.release());
            GenerateClassDoc(input, docDir, classElement, moduleXmlDoc, otherModuleXmlDocs);
        }
        bodyElement->AppendChild(classTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> ctorDtorNodeSet;
    std::vector<soul::xml::Element*> ctorDtorElements;
    GetConstructorsAndDestructors(classElement, ctorDtorNodeSet, ctorDtorElements);
    int ncd = static_cast<int>(ctorDtorElements.size());
    if (ncd > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "ctorDtors");
        AppendToLegend(legend, "Constructors & Destructor", "ctorDtors", first);
        h2Element->AppendChild(soul::xml::MakeText("Constructors & Destructor"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> ctorDtorTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ncd; ++i)
        {
            soul::xml::Element* ctorDtorElement = ctorDtorElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), ctorDtorElement->GetAttribute("specifiers"), "", false);
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", ctorDtorElement->GetAttribute("docPath"));
            std::unique_ptr<soul::xml::xpath::NodeSet> templateParameterNodeSet;
            std::vector<soul::xml::Element*> templateParameterElements;
            linkElement->AppendChild(soul::xml::MakeText(MakeFunctionName(ctorDtorElement, 
                templateParameterNodeSet, templateParameterElements)));
            td2Element->AppendChild(linkElement.release());
            AppendParameters(input, td2Element.get(), ctorDtorElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            if (ctorDtorElement->GetAttribute("includeConstraint") == "true")
            {
                std::unique_ptr<soul::xml::xpath::NodeSet> constraintNodeSet;
                soul::xml::Element* constraintElement = GetConstraint(ctorDtorElement, constraintNodeSet);
                if (constraintElement)
                {
                    td2Element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                    AppendConstraint(input, td2Element.get(), constraintElement, moduleXmlDoc, otherModuleXmlDocs, std::string(), true);
                }
            }
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, ctorDtorElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            ctorDtorTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(ctorDtorTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> memberFunctionNodeSet;
    std::vector<soul::xml::Element*> memberFunctionElements;
    GetFunctions(classElement, memberFunctionNodeSet, memberFunctionElements);
    int nmf = static_cast<int>(memberFunctionElements.size());
    if (nmf > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "memberFunctions");
        AppendToLegend(legend, "Member Functions", "memberFunctions", first);
        h2Element->AppendChild(soul::xml::MakeText("Member Functions"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> memberFunctionTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nmf; ++i)
        {
            soul::xml::Element* memberFunctionElement = memberFunctionElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), memberFunctionElement->GetAttribute("specifiers"), "", true);
            AppendType(input, tdElement.get(), memberFunctionElement->GetAttribute("returnType"), "RETURN_TYPE", 
                moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", memberFunctionElement->GetAttribute("docPath"));
            std::unique_ptr<soul::xml::xpath::NodeSet> templateParameterNodeSet;
            std::vector<soul::xml::Element*> templateParameterElements;
            linkElement->AppendChild(soul::xml::MakeText(MakeFunctionName(memberFunctionElement, templateParameterNodeSet, templateParameterElements)));
            td2Element->AppendChild(linkElement.release());
            AppendParameters(input, td2Element.get(), memberFunctionElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            std::string isConst = memberFunctionElement->GetAttribute("const");
            if (isConst == "true")
            {
                td2Element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
                span->SetAttribute("class", "kw");
                span->AppendChild(soul::xml::MakeText("const"));
                td2Element->AppendChild(span.release());
            }
            if (memberFunctionElement->GetAttribute("includeConstraint") == "true")
            {
                std::unique_ptr<soul::xml::xpath::NodeSet> constraintNodeSet;
                soul::xml::Element* constraintElement = GetConstraint(memberFunctionElement, constraintNodeSet);
                if (constraintElement)
                {
                    td2Element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                    AppendConstraint(input, td2Element.get(), constraintElement, moduleXmlDoc, otherModuleXmlDocs, std::string(), true);
                }
            }
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, memberFunctionElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            memberFunctionTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(memberFunctionTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> delegateNodeSet;
    std::vector<soul::xml::Element*> delegateElements;
    GetDelegates(classElement, delegateNodeSet, delegateElements);
    int ndlg = static_cast<int>(delegateElements.size());
    if (ndlg > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "delegates");
        AppendToLegend(legend, "Delegates", "delegates", first);
        h2Element->AppendChild(soul::xml::MakeText("Delegates"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> delegateTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ndlg; ++i)
        {
            soul::xml::Element* delegateElement = delegateElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            trElement->SetAttribute("id", delegateElement->GetAttribute("id"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), delegateElement->GetAttribute("specifiers"), "delegate", true);
            AppendType(input, tdElement.get(), delegateElement->GetAttribute("returnType"), "RETURN_TYPE", 
                moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->AppendChild(soul::xml::MakeText(delegateElement->GetAttribute("name")));
            AppendParameters(input, td2Element.get(), delegateElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, delegateElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            delegateTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(delegateTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> classDelegateNodeSet;
    std::vector<soul::xml::Element*> classDelegateElements;
    GetClassDelegates(classElement, classDelegateNodeSet, classDelegateElements);
    int ncdlg = static_cast<int>(classDelegateElements.size());
    if (ncdlg > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "classDelegates");
        AppendToLegend(legend, "Class Delegates", "classDelegates", first);
        h2Element->AppendChild(soul::xml::MakeText("Class Delegates"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> classDelegateTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ncdlg; ++i)
        {
            soul::xml::Element* classDelegateElement = classDelegateElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            trElement->SetAttribute("id", classDelegateElement->GetAttribute("id"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), classDelegateElement->GetAttribute("specifiers"), "delegate", true);
            AppendType(input, tdElement.get(), classDelegateElement->GetAttribute("returnType"), "RETURN_TYPE", 
                moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->AppendChild(soul::xml::MakeText(classDelegateElement->GetAttribute("name")));
            AppendParameters(input, td2Element.get(), classDelegateElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, classDelegateElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            classDelegateTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(classDelegateTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> typedefNodeSet;
    std::vector<soul::xml::Element*> typedefElements;
    GetTypedefs(classElement, typedefNodeSet, typedefElements);
    int nt = static_cast<int>(typedefElements.size());
    if (nt > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "typeAliases");
        AppendToLegend(legend, "Type Aliases", "typeAliases", first);
        h2Element->AppendChild(soul::xml::MakeText("Type Aliases"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> typedefTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nt; ++i)
        {
            soul::xml::Element* typedefElement = typedefElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            trElement->SetAttribute("id", typedefElement->GetAttribute("id"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), typedefElement->GetAttribute("specifiers"), "typedef", true);
            tdElement->AppendChild(soul::xml::MakeText(typedefElement->GetAttribute("name")));
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            AppendType(input, td2Element.get(), typedefElement->GetAttribute("type"), "TYPE", 
                moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, typedefElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            typedefTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(typedefTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> enumTypeNodeSet;
    std::vector<soul::xml::Element*> enumTypeElements;
    GetEnumTypes(classElement, enumTypeNodeSet, enumTypeElements);
    int ne = static_cast<int>(enumTypeElements.size());
    if (ne > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "enumTypes");
        AppendToLegend(legend, "Enumerated Types", "enumTypes", first);
        h2Element->AppendChild(soul::xml::MakeText("Enumerated Types"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> enumTypeTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ne; ++i)
        {
            soul::xml::Element* enumTypeElement = enumTypeElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), enumTypeElement->GetAttribute("specifiers"), "enum", false);
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", enumTypeElement->GetAttribute("docPath"));
            linkElement->AppendChild(soul::xml::MakeText(enumTypeElement->GetAttribute("name")));
            td2Element->AppendChild(linkElement.release());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, enumTypeElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            enumTypeTableElement->AppendChild(trElement.release());
            GenerateEnumDoc(input, docDir, enumTypeElement, moduleXmlDoc, otherModuleXmlDocs);
        }
        bodyElement->AppendChild(enumTypeTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> constantNodeSet;
    std::vector<soul::xml::Element*> constantElements;
    GetConstants(classElement, constantNodeSet, constantElements);
    int nc = static_cast<int>(constantElements.size());
    if (nc > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "constants");
        AppendToLegend(legend, "Constants", "constants", first);
        h2Element->AppendChild(soul::xml::MakeText("Constants"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> constantTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nc; ++i)
        {
            soul::xml::Element* constantElement = constantElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), constantElement->GetAttribute("specifiers"), "const", true);
            AppendType(input, tdElement.get(), constantElement->GetAttribute("type"), "CONSTANT_TYPE", 
                moduleXmlDoc, otherModuleXmlDocs, std::string());
            tdElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
            tdElement->AppendChild(soul::xml::MakeText(constantElement->GetAttribute("name")));
            tdElement->SetAttribute("id", constantElement->GetAttribute("id"));
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            td2Element->AppendChild(soul::xml::MakeText(constantElement->GetAttribute("value")));
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, constantElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            constantTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(constantTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> variableNodeSet;
    std::vector<soul::xml::Element*> variableElements;
    GetVariables(classElement, variableNodeSet, variableElements);
    int nv = static_cast<int>(variableElements.size());
    if (nv > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "memberVariables");
        AppendToLegend(legend, "Member Variables", "memberVariables", first);
        h2Element->AppendChild(soul::xml::MakeText("Member Variables"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> variableTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nv; ++i)
        {
            soul::xml::Element* variableElement = variableElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            tdElement->SetAttribute("id", variableElement->GetAttribute("id"));
            AppendSpecifiers(tdElement.get(), variableElement->GetAttribute("specifiers"), std::string(), true);
            AppendType(input, tdElement.get(), variableElement->GetAttribute("type"), "VARIABLE_TYPE", 
                moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            td2Element->AppendChild(soul::xml::MakeText(variableElement->GetAttribute("name")));
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, variableElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            variableTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(variableTableElement.release());
    }
    GenerateFunctionDetailsSection(input, bodyElement.get(), "Constructor& Destructor Details", ctorDtorElements, 
        moduleXmlDoc, otherModuleXmlDocs, std::string());
    GenerateFunctionDetailsSection(input, bodyElement.get(), "Member Function Details", memberFunctionElements, 
        moduleXmlDoc, otherModuleXmlDocs, std::string());
    htmlElement->AppendChild(bodyElement.release());
    doc.AppendChild(htmlElement.release());
    std::string docFilePath = util::GetFullPath(util::Path::Combine(docDir, classElement->GetAttribute("id")) + ".html");
    std::ofstream docFile(docFilePath);
    util::CodeFormatter formatter(docFile);
    formatter.SetIndentSize(1);
    doc.Write(formatter);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        std::cout << "==> " << docFilePath << std::endl;
    }
}

void GenerateConceptDoc(Input* input, const std::string& docDir, soul::xml::Element* conceptElement, soul::xml::Document* moduleXmlDoc,
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs)
{
    soul::xml::Document* docs = input->docs.get();
    std::string title = MakeConceptName(conceptElement) + " Concept";
    std::string styleFilePath = "../../../style/style.css";
    soul::xml::Document doc;
    std::unique_ptr<soul::xml::Element> htmlElement(soul::xml::MakeElement("html"));
    std::unique_ptr<soul::xml::Element> headElement(soul::xml::MakeElement("head"));
    std::unique_ptr<soul::xml::Element> metaElement(soul::xml::MakeElement("meta"));
    metaElement->SetAttribute("charset", "utf-8");
    headElement->AppendChild(metaElement.release());
    std::unique_ptr<soul::xml::Element> titleElement(soul::xml::MakeElement("title"));
    titleElement->AppendChild(soul::xml::MakeText(title));
    headElement->AppendChild(titleElement.release());
    std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("link"));
    linkElement->SetAttribute("rel", "stylesheet");
    linkElement->SetAttribute("type", "text/css");
    linkElement->SetAttribute("href", styleFilePath);
    headElement->AppendChild(linkElement.release());
    soul::xml::Element* refinedConceptElement = GetRefinedConcept(conceptElement, moduleXmlDoc, otherModuleXmlDocs);
    std::vector<soul::xml::Element*> derivedConcepts = GetDerivedConcepts(conceptElement, moduleXmlDoc, otherModuleXmlDocs);
    std::string svgId;
    bool diagramCodeGenerated = false;
    if (refinedConceptElement || !derivedConcepts.empty())
    {
        std::vector<DiagramNode> diagramNodes;
        int level = 0;
        int depth = 0;
        if (refinedConceptElement)
        {
            --level;
            ++depth;
            GenerateRefinementDiagramNodes(refinedConceptElement, moduleXmlDoc, otherModuleXmlDocs, diagramNodes, level, depth);
            ++level;
        }
        int index = static_cast<int>(diagramNodes.size());
        std::string name = MakeFullConceptName(conceptElement);
        bool subject = true;
        bool hasDerivedNodes = !derivedConcepts.empty();
        std::string link = conceptElement->GetAttribute("docPath");
        diagramNodes.push_back(DiagramNode(index, level, name, subject, hasDerivedNodes, link));
        ++level;
        int ndc = static_cast<int>(derivedConcepts.size());
        for (int i = 0; i < ndc; ++i)
        {
            soul::xml::Element* derivedConceptElement = derivedConcepts[i];
            index = static_cast<int>(diagramNodes.size());
            name = MakeFullConceptName(derivedConceptElement);
            subject = false;
            hasDerivedNodes = false;
            link = derivedConceptElement->GetAttribute("docPath");
            diagramNodes.push_back(DiagramNode(index, level, name, subject, hasDerivedNodes, link));
        }
        int n = static_cast<int>(diagramNodes.size());
        for (int i = 0; i < n; ++i)
        {
            DiagramNode& node = diagramNodes[i];
            node.level += depth;
        }
        std::sort(diagramNodes.begin(), diagramNodes.end(), DiagramNodeByLevelAndName());
        std::string scriptFileName = conceptElement->GetAttribute("id") + "_refinement.js";
        std::string scriptFilePath = util::Path::Combine(docDir, scriptFileName);
        svgId = "refinement_svg_diagram";
        GenerateDiagramCode(diagramNodes, "drawRefinementDiagram", svgId, scriptFilePath);
        diagramCodeGenerated = true;
        std::unique_ptr<soul::xml::Element> scriptElement(soul::xml::MakeElement("script"));
        scriptElement->SetAttribute("type", "text/javascript");
        scriptElement->SetAttribute("src", scriptFileName);
        scriptElement->AppendChild(soul::xml::MakeText(" "));
        headElement->AppendChild(scriptElement.release());
    }
    htmlElement->AppendChild(headElement.release());
    std::unique_ptr<soul::xml::Element> bodyElement(soul::xml::MakeElement("body"));
    std::unique_ptr<soul::xml::Element> h1Element(soul::xml::MakeElement("h1"));
    h1Element->AppendChild(soul::xml::MakeText(title));
    bodyElement->AppendChild(h1Element.release());
    if (docs)
    {
        std::unique_ptr<soul::xml::Element> descriptionAndDetails = GetDescriptionAndDetails(input, conceptElement->GetAttribute("id"), docs,
            moduleXmlDoc, otherModuleXmlDocs, std::string());
        if (descriptionAndDetails)
        {
            bodyElement->AppendChild(descriptionAndDetails.release());
        }
    }
    std::string filePath = conceptElement->GetAttribute("filePath");
    std::string line = conceptElement->GetAttribute("line");
    std::string fileName = conceptElement->GetAttribute("fileName");
    if (!filePath.empty() && !line.empty() && !fileName.empty())
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Definition"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("xml:space", "preserve");
        span->SetAttribute("class", "indent");
        span->AppendChild(soul::xml::MakeText("Line"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> lineLinkElement(soul::xml::MakeElement("a"));
        lineLinkElement->SetAttribute("href", filePath + "#" + line);
        lineLinkElement->AppendChild(soul::xml::MakeText(line));
        span->AppendChild(lineLinkElement.release());
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        span->AppendChild(soul::xml::MakeText("of"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> fileLinkElement(soul::xml::MakeElement("a"));
        fileLinkElement->SetAttribute("href", filePath);
        fileLinkElement->AppendChild(soul::xml::MakeText(fileName));
        span->AppendChild(fileLinkElement.release());
        bodyElement->AppendChild(span.release());
    }
    if (diagramCodeGenerated)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Refinement"));
        bodyElement->AppendChild(h2Element.release());
        bodyElement->SetAttribute("onload", "drawRefinementDiagram()");
        std::unique_ptr<soul::xml::Element> divElement(soul::xml::MakeElement("div"));
        divElement->SetAttribute("class", "diagram");
        std::unique_ptr<soul::xml::Element> svgElement(soul::xml::MakeElement("svg"));
        svgElement->SetAttribute("width", "0");
        svgElement->SetAttribute("height", "0");
        svgElement->SetAttribute("id", svgId);
        svgElement->SetAttribute("xmlns", "http://www.w3.org/2000/svg");
        svgElement->SetAttribute("version", "2.0");
        divElement->AppendChild(svgElement.release());
        bodyElement->AppendChild(divElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> typeParametersNodeSet;
    std::vector<soul::xml::Element*> typeParameterElements;
    GetTypeParameters(conceptElement, typeParametersNodeSet, typeParameterElements);
    int ntp = static_cast<int>(typeParameterElements.size());
    if (ntp > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Type Parameters"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> typeParameterTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ntp; ++i)
        {
            soul::xml::Element* typeParameterElement = typeParameterElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            trElement->SetAttribute("id", typeParameterElement->GetAttribute("id"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            tdElement->AppendChild(soul::xml::MakeText(typeParameterElement->GetAttribute("name")));
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            trElement->AppendChild(td2Element.release());
            typeParameterTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(typeParameterTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> constraintsNodeSet;
    std::vector<soul::xml::Element*> constraintElements;
    GetConstraints(conceptElement, constraintsNodeSet, constraintElements);
    int nco = static_cast<int>(constraintElements.size());
    if (nco > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Constraints"));
        bodyElement->AppendChild(h2Element.release());
        for (int i = 0; i < nco; ++i)
        {
            soul::xml::Element* constraintElement = constraintElements[i];
            std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
            span->SetAttribute("xml:space", "preserve");
            span->SetAttribute("class", "indent");
            AppendConstraint(input, span.get(), constraintElement, moduleXmlDoc, otherModuleXmlDocs, std::string(), false);
            bodyElement->AppendChild(span.release());
            bodyElement->AppendChild(soul::xml::MakeElement("br"));
        }
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> axiomsNodeSet;
    std::vector<soul::xml::Element*> axiomElements;
    GetAxioms(conceptElement, axiomsNodeSet, axiomElements);
    int na = static_cast<int>(axiomElements.size());
    if (na > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Axioms"));
        bodyElement->AppendChild(h2Element.release());
        for (int i = 0; i < na; ++i)
        {
            soul::xml::Element* axiomElement = axiomElements[i];
            std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
            span->SetAttribute("xml:space", "preserve");
            span->SetAttribute("class", "indent");
            std::unique_ptr<soul::xml::Element> axiomSpan(soul::xml::MakeElement("span"));
            axiomSpan->SetAttribute("xml:space", "preserve");
            axiomSpan->SetAttribute("class", "kw");
            axiomSpan->AppendChild(soul::xml::MakeText("axiom"));
            span->AppendChild(axiomSpan.release());
            soul::xml::Node* symbolChild = axiomElement->FirstChild();
            if (symbolChild && symbolChild->IsElementNode())
            {
                soul::xml::Element* symbolElement = static_cast<soul::xml::Element*>(symbolChild);
                span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                AppendSymbol(input, span.get(), symbolElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            }
            AppendAxiomParameters(input, span.get(), axiomElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            span->AppendChild(soul::xml::MakeElement("br"));
            bodyElement->AppendChild(span.release());
            std::unique_ptr<soul::xml::Element> axiomStatementSpan(soul::xml::MakeElement("span"));
            axiomStatementSpan->SetAttribute("xml:space", "preserve");
            axiomStatementSpan->SetAttribute("class", "indent");
            axiomStatementSpan->AppendChild(soul::xml::MakeText("{"));
            axiomStatementSpan->AppendChild(soul::xml::MakeElement("br"));
            bodyElement->AppendChild(axiomStatementSpan.release());
            std::unique_ptr<soul::xml::xpath::NodeSet> axiomStatementsNodeSet;
            std::vector<soul::xml::Element*> axiomStatementElements;
            GetAxiomStatements(axiomElement, axiomStatementsNodeSet, axiomStatementElements);
            int ns = static_cast<int>(axiomStatementElements.size());
            for (int i = 0; i < ns; ++i)
            {
                std::unique_ptr<soul::xml::Element> axiomStatementSpan(soul::xml::MakeElement("span"));
                axiomStatementSpan->SetAttribute("xml:space", "preserve");
                axiomStatementSpan->SetAttribute("class", "indent");
                axiomStatementSpan->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                axiomStatementSpan->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                axiomStatementSpan->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                axiomStatementSpan->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                soul::xml::Element* axiomStatementElement = axiomStatementElements[i];
                axiomStatementSpan->AppendChild(soul::xml::MakeText(axiomStatementElement->GetAttribute("value")));
                axiomStatementSpan->AppendChild(soul::xml::MakeElement("br"));
                bodyElement->AppendChild(axiomStatementSpan.release());
            }
            std::unique_ptr<soul::xml::Element> endExiomStatementSpan(soul::xml::MakeElement("span"));
            endExiomStatementSpan->SetAttribute("xml:space", "preserve");
            endExiomStatementSpan->SetAttribute("class", "indent");
            endExiomStatementSpan->AppendChild(soul::xml::MakeText("}"));
            endExiomStatementSpan->AppendChild(soul::xml::MakeElement("br"));
            bodyElement->AppendChild(endExiomStatementSpan.release());
            bodyElement->AppendChild(soul::xml::MakeElement("br"));
        }
    }
    htmlElement->AppendChild(bodyElement.release());
    doc.AppendChild(htmlElement.release());
    std::string docFilePath = util::GetFullPath(util::Path::Combine(docDir, conceptElement->GetAttribute("id")) + ".html");
    std::ofstream docFile(docFilePath);
    util::CodeFormatter formatter(docFile);
    formatter.SetIndentSize(1);
    doc.Write(formatter);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        std::cout << "==> " << docFilePath << std::endl;
    }
}

bool GenerateNamespaceNames(int level, soul::xml::Element* namespaceTableElement, soul::xml::Element* namespaceParentElement, const std::string& prefix,
    std::vector<std::unique_ptr<soul::xml::xpath::NodeSet>>& namespaceNodeSets, std::vector<soul::xml::Element*>& nsElements)
{
    bool retval = false;
    std::unique_ptr<soul::xml::xpath::NodeSet> namespaceNodeSet = soul::xml::xpath::EvaluateToNodeSet("namespaces/namespace", namespaceParentElement);
    int n = namespaceNodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* nsNode = namespaceNodeSet->GetNode(i);
        if (nsNode->IsElementNode())
        {
            soul::xml::Element* nsElement = static_cast<soul::xml::Element*>(nsNode);
            if (nsElement->GetAttribute("name").find("unnamed_ns_") != std::string::npos)
            {
                continue;
            }
            nsElements.push_back(nsElement);
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            for (int l = 0; l < level; ++l)
            {
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                trElement->AppendChild(tdElement.release());
            }
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            std::string nsName = nsElement->GetAttribute("name");
            if (nsName.empty())
            {
                nsName = "Global Namespace";
            }
            std::string nsId = nsElement->GetAttribute("id");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", prefix + nsId + ".html");
            linkElement->AppendChild(soul::xml::MakeText(nsName));
            tdElement->AppendChild(linkElement.release());
            trElement->AppendChild(tdElement.release());
            namespaceTableElement->AppendChild(trElement.release());
            retval = true;
            GenerateNamespaceNames(level + 1, namespaceTableElement, nsElement, prefix, namespaceNodeSets, nsElements);
        }
    }
    namespaceNodeSets.push_back(std::move(namespaceNodeSet));
    return retval;
}

std::string MakeFullNamespaceName(soul::xml::Element* nsElement)
{
    std::string fullNamespaceName;
    if (nsElement)
    {
        if (nsElement->Name() == "namespaces")
        {
            soul::xml::Node* parent = nsElement->Parent();
            if (parent && parent->IsElementNode())
            {
                soul::xml::Element* parentElement = static_cast<soul::xml::Element*>(parent);
                return MakeFullNamespaceName(parentElement);
            }
        }
        else if (nsElement->Name() == "namespace")
        {
            std::string namespaceName = nsElement->GetAttribute("name");
            if (!namespaceName.empty())
            {
                soul::xml::Node* parent = nsElement->Parent();
                if (parent && parent->IsElementNode())
                {
                    soul::xml::Element* parentElement = static_cast<soul::xml::Element*>(parent);
                    fullNamespaceName = MakeFullNamespaceName(parentElement);
                    if (!fullNamespaceName.empty())
                    {
                        fullNamespaceName.append(1, '.');
                    }
                }
                fullNamespaceName.append(namespaceName);
            }
        }
    }
    return fullNamespaceName;
}

void GenerateInterfaceDoc(Input* input, const std::string& docDir, soul::xml::Element* interfaceElement, soul::xml::Document* moduleXmlDoc, 
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs)
{
    soul::xml::Document* docs = input->docs.get();
    std::string interfaceName = interfaceElement->GetAttribute("name");
    interfaceName.append(" Interface");
    std::string specifiers = interfaceElement->GetAttribute("specifiers");
    std::string title = interfaceName;
    std::string styleFilePath = "../../../style/style.css";
    soul::xml::Document doc;
    std::unique_ptr<soul::xml::Element> htmlElement(soul::xml::MakeElement("html"));
    std::unique_ptr<soul::xml::Element> headElement(soul::xml::MakeElement("head"));
    std::unique_ptr<soul::xml::Element> metaElement(soul::xml::MakeElement("meta"));
    metaElement->SetAttribute("charset", "utf-8");
    headElement->AppendChild(metaElement.release());
    std::unique_ptr<soul::xml::Element> titleElement(soul::xml::MakeElement("title"));
    titleElement->AppendChild(soul::xml::MakeText(title));
    headElement->AppendChild(titleElement.release());
    std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("link"));
    linkElement->SetAttribute("rel", "stylesheet");
    linkElement->SetAttribute("type", "text/css");
    linkElement->SetAttribute("href", styleFilePath);
    headElement->AppendChild(linkElement.release());
    htmlElement->AppendChild(headElement.release());
    std::unique_ptr<soul::xml::Element> bodyElement(soul::xml::MakeElement("body"));
    std::unique_ptr<soul::xml::Element> h1Element(soul::xml::MakeElement("h1"));
    h1Element->AppendChild(soul::xml::MakeText(title));
    bodyElement->AppendChild(h1Element.release());
    if (docs)
    {
        std::unique_ptr<soul::xml::Element> descriptionAndDetails = GetDescriptionAndDetails(input, interfaceElement->GetAttribute("id"), docs, 
            moduleXmlDoc, otherModuleXmlDocs, std::string());
        if (descriptionAndDetails)
        {
            bodyElement->AppendChild(descriptionAndDetails.release());
        }
    }
    std::string filePath = interfaceElement->GetAttribute("filePath");
    std::string line = interfaceElement->GetAttribute("line");
    std::string fileName = interfaceElement->GetAttribute("fileName");
    if (!filePath.empty() && !line.empty() && !fileName.empty())
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Definition"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
        span->SetAttribute("xml:space", "preserve");
        span->SetAttribute("class", "indent");
        span->AppendChild(soul::xml::MakeText("Line"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> lineLinkElement(soul::xml::MakeElement("a"));
        lineLinkElement->SetAttribute("href", filePath + "#" + line);
        lineLinkElement->AppendChild(soul::xml::MakeText(line));
        span->AppendChild(lineLinkElement.release());
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        span->AppendChild(soul::xml::MakeText("of"));
        span->AppendChild(soul::xml::MakeEntityReference("nbsp"));
        std::unique_ptr<soul::xml::Element> fileLinkElement(soul::xml::MakeElement("a"));
        fileLinkElement->SetAttribute("href", filePath);
        fileLinkElement->AppendChild(soul::xml::MakeText(fileName));
        span->AppendChild(fileLinkElement.release());
        bodyElement->AppendChild(span.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> memberFunctionNodeSet;
    std::vector<soul::xml::Element*> memberFunctionElements;
    GetFunctions(interfaceElement, memberFunctionNodeSet, memberFunctionElements);
    int nmf = static_cast<int>(memberFunctionElements.size());
    if (nmf > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->AppendChild(soul::xml::MakeText("Member Functions"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> memberFunctionTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nmf; ++i)
        {
            soul::xml::Element* memberFunctionElement = memberFunctionElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), memberFunctionElement->GetAttribute("specifiers"), "", true);
            AppendType(input, tdElement.get(), memberFunctionElement->GetAttribute("returnType"), "RETURN_TYPE", 
                moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", memberFunctionElement->GetAttribute("docPath"));
            std::unique_ptr<soul::xml::xpath::NodeSet> templateParameterNodeSet;
            std::vector<soul::xml::Element*> templateParameterElements;
            linkElement->AppendChild(soul::xml::MakeText(MakeFunctionName(memberFunctionElement, templateParameterNodeSet, templateParameterElements)));
            td2Element->AppendChild(linkElement.release());
            AppendParameters(input, td2Element.get(), memberFunctionElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            std::string isConst = memberFunctionElement->GetAttribute("const");
            if (isConst == "true")
            {
                td2Element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                std::unique_ptr<soul::xml::Element> span(soul::xml::MakeElement("span"));
                span->SetAttribute("class", "kw");
                span->AppendChild(soul::xml::MakeText("const"));
                td2Element->AppendChild(span.release());
            }
            if (memberFunctionElement->GetAttribute("includeConstraint") == "true")
            {
                std::unique_ptr<soul::xml::xpath::NodeSet> constraintNodeSet;
                soul::xml::Element* constraintElement = GetConstraint(memberFunctionElement, constraintNodeSet);
                if (constraintElement)
                {
                    td2Element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                    AppendConstraint(input, td2Element.get(), constraintElement, moduleXmlDoc, otherModuleXmlDocs, std::string(), true);
                }
            }
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, memberFunctionElement->GetAttribute("id"), docs, 
                moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            memberFunctionTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(memberFunctionTableElement.release());
    }
    GenerateFunctionDetailsSection(input, bodyElement.get(), "Member Function Details", memberFunctionElements, 
        moduleXmlDoc, otherModuleXmlDocs, std::string());
    htmlElement->AppendChild(bodyElement.release());
    doc.AppendChild(htmlElement.release());
    std::string docFilePath = util::GetFullPath(util::Path::Combine(docDir, interfaceElement->GetAttribute("id")) + ".html");
    std::ofstream docFile(docFilePath);
    util::CodeFormatter formatter(docFile);
    formatter.SetIndentSize(1);
    doc.Write(formatter);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        std::cout << "==> " << docFilePath << std::endl;
    }
}

void GenerateNamespaceDoc(Input* input, const std::string& docDir, soul::xml::Element* nsElement, soul::xml::Document* moduleXmlDoc,
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs)
{
    soul::xml::Document* docs = input->docs.get();
    std::string title = MakeFullNamespaceName(nsElement) + " Namespace";
    std::string styleFilePath = "../../../style/style.css";
    soul::xml::Document doc;
    std::unique_ptr<soul::xml::Element> htmlElement(soul::xml::MakeElement("html"));
    std::unique_ptr<soul::xml::Element> headElement(soul::xml::MakeElement("head"));
    std::unique_ptr<soul::xml::Element> metaElement(soul::xml::MakeElement("meta"));
    metaElement->SetAttribute("charset", "utf-8");
    headElement->AppendChild(metaElement.release());
    std::unique_ptr<soul::xml::Element> titleElement(soul::xml::MakeElement("title"));
    titleElement->AppendChild(soul::xml::MakeText(title));
    headElement->AppendChild(titleElement.release());
    std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("link"));
    linkElement->SetAttribute("rel", "stylesheet");
    linkElement->SetAttribute("type", "text/css");
    linkElement->SetAttribute("href", styleFilePath);
    headElement->AppendChild(linkElement.release());
    htmlElement->AppendChild(headElement.release());
    std::unique_ptr<soul::xml::Element> bodyElement(soul::xml::MakeElement("body"));
    std::unique_ptr<soul::xml::Element> h1Element(soul::xml::MakeElement("h1"));
    h1Element->AppendChild(soul::xml::MakeText(title));
    bodyElement->AppendChild(h1Element.release());
    std::unique_ptr<soul::xml::Element> legendSpan(soul::xml::MakeElement("span"));
    legendSpan->SetAttribute("class", "indent");
    soul::xml::Element* legend = legendSpan.get();
    bool first = true;
    bodyElement->AppendChild(legendSpan.release());
    if (docs)
    {
        std::unique_ptr<soul::xml::Element> descriptionAndDetails = GetDescriptionAndDetails(input, nsElement->GetAttribute("id"), docs, moduleXmlDoc, 
            otherModuleXmlDocs, std::string());
        if (descriptionAndDetails)
        {
            bodyElement->AppendChild(descriptionAndDetails.release());
        }
    }
    std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
    h2Element->AppendChild(soul::xml::MakeText("Namespaces"));
    std::unique_ptr<soul::xml::Element> namespaceTableElement(soul::xml::MakeElement("table"));
    std::vector<std::unique_ptr<soul::xml::xpath::NodeSet>> namespaceNodeSets;
    std::vector<soul::xml::Element*> nsElements;
    if (GenerateNamespaceNames(0, namespaceTableElement.get(), nsElement, "", namespaceNodeSets, nsElements))
    {
        h2Element->SetAttribute("id", "namespaces");
        AppendToLegend(legend, "Namespaces", "namespaces", first);
        bodyElement->AppendChild(h2Element.release());
        bodyElement->AppendChild(namespaceTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> conceptNodeSet;
    std::vector<soul::xml::Element*> conceptElements;
    GetConcepts(nsElement, conceptNodeSet, conceptElements);
    int n = static_cast<int>(conceptElements.size());
    if (n > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "concepts");
        AppendToLegend(legend, "Concepts", "concepts", first);
        h2Element->AppendChild(soul::xml::MakeText("Concepts"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> conceptTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Element* conceptElement = conceptElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), conceptElement->GetAttribute("specifiers"), "concept", false);
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", conceptElement->GetAttribute("docPath"));
            linkElement->AppendChild(soul::xml::MakeText(MakeConceptName(conceptElement)));
            td2Element->AppendChild(linkElement.release());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, conceptElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            conceptTableElement->AppendChild(trElement.release());
            GenerateConceptDoc(input, docDir, conceptElement, moduleXmlDoc, otherModuleXmlDocs);
        }
        bodyElement->AppendChild(conceptTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> templateParametersNodeSet;
    std::vector<soul::xml::Element*> templateParameterElements;
    std::unique_ptr<soul::xml::xpath::NodeSet> classNodeSet;
    std::vector<soul::xml::Element*> classElements;
    GetClasses(nsElement, classNodeSet, classElements);
    int nc = static_cast<int>(classElements.size());
    if (nc > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "classes");
        AppendToLegend(legend, "Classes", "classes", first);
        h2Element->AppendChild(soul::xml::MakeText("Classes"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> classTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nc; ++i)
        {
            soul::xml::Element* classElement = classElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), classElement->GetAttribute("specifiers"), "class", false);
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", classElement->GetAttribute("docPath"));
            linkElement->AppendChild(soul::xml::MakeText(MakeClassName(classElement, templateParametersNodeSet, templateParameterElements)));
            td2Element->AppendChild(linkElement.release());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, classElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            classTableElement->AppendChild(trElement.release());
            GenerateClassDoc(input, docDir, classElement, moduleXmlDoc, otherModuleXmlDocs);
        }
        bodyElement->AppendChild(classTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> interfaceNodeSet;
    std::vector<soul::xml::Element*> interfaceElements;
    GetInterfaces(nsElement, interfaceNodeSet, interfaceElements);
    int ni = static_cast<int>(interfaceElements.size());
    if (ni > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "interfaces");
        AppendToLegend(legend, "Interfaces", "interfaces", first);
        h2Element->AppendChild(soul::xml::MakeText("Interfaces"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> interfaceTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ni; ++i)
        {
            soul::xml::Element* interfaceElement = interfaceElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), interfaceElement->GetAttribute("specifiers"), "interface", false);
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", interfaceElement->GetAttribute("docPath"));
            linkElement->AppendChild(soul::xml::MakeText(interfaceElement->GetAttribute("name")));
            td2Element->AppendChild(linkElement.release());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, interfaceElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            interfaceTableElement->AppendChild(trElement.release());
            GenerateInterfaceDoc(input, docDir, interfaceElement, moduleXmlDoc, otherModuleXmlDocs);
        }
        bodyElement->AppendChild(interfaceTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> functionNodeSet;
    std::vector<soul::xml::Element*> functionElements;
    GetFunctions(nsElement, functionNodeSet, functionElements);
    int nf = static_cast<int>(functionElements.size());
    if (nf > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "functions");
        AppendToLegend(legend, "Functions", "functions", first);
        h2Element->AppendChild(soul::xml::MakeText("Functions"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> functionTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nf; ++i)
        {
            soul::xml::Element* functionElement = functionElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), functionElement->GetAttribute("specifiers"), std::string(), true);
            AppendType(input, tdElement.get(), functionElement->GetAttribute("returnType"), "RETURN_TYPE", moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", functionElement->GetAttribute("docPath"));
            std::unique_ptr<soul::xml::xpath::NodeSet> templateParametersNodeSet;
            std::vector<soul::xml::Element*> templateParameterElements;
            linkElement->AppendChild(soul::xml::MakeText(MakeFunctionName(functionElement, templateParametersNodeSet, templateParameterElements)));
            td2Element->AppendChild(linkElement.release());
            AppendParameters(input, td2Element.get(), functionElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            if (functionElement->GetAttribute("includeConstraint") == "true")
            {
                std::unique_ptr<soul::xml::xpath::NodeSet> constraintNodeSet;
                soul::xml::Element* constraintElement = GetConstraint(functionElement, constraintNodeSet);
                if (constraintElement)
                {
                    td2Element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                    AppendConstraint(input, td2Element.get(), constraintElement, moduleXmlDoc, otherModuleXmlDocs, std::string(), true);
                }
            }
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, functionElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            functionTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(functionTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> delegateNodeSet;
    std::vector<soul::xml::Element*> delegateElements;
    GetDelegates(nsElement, delegateNodeSet, delegateElements);
    int ndlg = static_cast<int>(delegateElements.size());
    if (ndlg > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "delegates");
        AppendToLegend(legend, "Delegates", "delegates", first);
        h2Element->AppendChild(soul::xml::MakeText("Delegates"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> delegateTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ndlg; ++i)
        {
            soul::xml::Element* delegateElement = delegateElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            trElement->SetAttribute("id", delegateElement->GetAttribute("id"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), delegateElement->GetAttribute("specifiers"), "delegate", true);
            AppendType(input, tdElement.get(), delegateElement->GetAttribute("returnType"), "RETURN_TYPE", moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->AppendChild(soul::xml::MakeText(delegateElement->GetAttribute("name")));
            AppendParameters(input, td2Element.get(), delegateElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, delegateElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            delegateTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(delegateTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> classDelegateNodeSet;
    std::vector<soul::xml::Element*> classDelegateElements;
    GetClassDelegates(nsElement, classDelegateNodeSet, classDelegateElements);
    int ncdlg = static_cast<int>(classDelegateElements.size());
    if (ncdlg > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "classDelegates");
        AppendToLegend(legend, "Class Delegates", "classDelegates", first);
        h2Element->AppendChild(soul::xml::MakeText("Class Delegates"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> classDelegateTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ncdlg; ++i)
        {
            soul::xml::Element* classDelegateElement = classDelegateElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            trElement->SetAttribute("id", classDelegateElement->GetAttribute("id"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), classDelegateElement->GetAttribute("specifiers"), "class delegate", true);
            AppendType(input, tdElement.get(), classDelegateElement->GetAttribute("returnType"), "RETURN_TYPE", moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->AppendChild(soul::xml::MakeText(classDelegateElement->GetAttribute("name")));
            AppendParameters(input, td2Element.get(), classDelegateElement, moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, classDelegateElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            classDelegateTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(classDelegateTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> typedefNodeSet;
    std::vector<soul::xml::Element*> typedefElements;
    GetTypedefs(nsElement, typedefNodeSet, typedefElements);
    int nt = static_cast<int>(typedefElements.size());
    if (nt > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "typeAliases");
        AppendToLegend(legend, "Type Aliases", "typeAliases", first);
        h2Element->AppendChild(soul::xml::MakeText("Type Aliases"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> typedefTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nt; ++i)
        {
            soul::xml::Element* typedefElement = typedefElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            trElement->SetAttribute("id", typedefElement->GetAttribute("id"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), typedefElement->GetAttribute("specifiers"), "typedef", true);
            tdElement->AppendChild(soul::xml::MakeText(typedefElement->GetAttribute("name")));
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            AppendType(input, td2Element.get(), typedefElement->GetAttribute("type"), "TYPE", moduleXmlDoc, otherModuleXmlDocs, std::string());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, typedefElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            typedefTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(typedefTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> enumTypesNodeSet;
    std::vector<soul::xml::Element*> enumTypeElements;
    GetEnumTypes(nsElement, enumTypesNodeSet, enumTypeElements);
    int ne = static_cast<int>(enumTypeElements.size());
    if (ne > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "enumTypes");
        AppendToLegend(legend, "Enumerated Types", "enumTypes", first);
        h2Element->AppendChild(soul::xml::MakeText("Enumerated Types"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> enumTypeTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < ne; ++i)
        {
            soul::xml::Element* enumTypeElement = enumTypeElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            AppendSpecifiers(tdElement.get(), enumTypeElement->GetAttribute("specifiers"), "enum", false);
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            linkElement->SetAttribute("href", enumTypeElement->GetAttribute("docPath"));
            linkElement->AppendChild(soul::xml::MakeText(enumTypeElement->GetAttribute("name")));
            td2Element->AppendChild(linkElement.release());
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, enumTypeElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            enumTypeTableElement->AppendChild(trElement.release());
            GenerateEnumDoc(input, docDir, enumTypeElement, moduleXmlDoc, otherModuleXmlDocs);
        }
        bodyElement->AppendChild(enumTypeTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> constantsNodeSet;
    std::vector<soul::xml::Element*> constantElements;
    GetConstants(nsElement, constantsNodeSet, constantElements);
    int nco = static_cast<int>(constantElements.size());
    if (nco > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "constants");
        AppendToLegend(legend, "Constants", "constants", first);
        h2Element->AppendChild(soul::xml::MakeText("Constants"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> constantTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nco; ++i)
        {
            soul::xml::Element* constantElement = constantElements[i];
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            tdElement->SetAttribute("class", "rightAlignedCol");
            tdElement->SetAttribute("id", constantElement->GetAttribute("id"));
            AppendSpecifiers(tdElement.get(), constantElement->GetAttribute("specifiers"), "const", true);
            AppendType(input, tdElement.get(), constantElement->GetAttribute("type"), "CONSTANT_TYPE", moduleXmlDoc, otherModuleXmlDocs, std::string());
            tdElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
            tdElement->AppendChild(soul::xml::MakeText(constantElement->GetAttribute("name")));
            trElement->AppendChild(tdElement.release());
            std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
            td2Element->SetAttribute("xml:space", "preserve");
            td2Element->AppendChild(soul::xml::MakeText(constantElement->GetAttribute("value")));
            trElement->AppendChild(td2Element.release());
            std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
            td3Element->SetAttribute("xml:space", "preserve");
            if (AppendDescription(td3Element.get(), input, constantElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
            {
                trElement->AppendChild(td3Element.release());
            }
            constantTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(constantTableElement.release());
    }
    GenerateFunctionDetailsSection(input, bodyElement.get(), "Function Details", functionElements, moduleXmlDoc, otherModuleXmlDocs, std::string());
    htmlElement->AppendChild(bodyElement.release());
    doc.AppendChild(htmlElement.release());
    std::string docFilePath = util::GetFullPath(util::Path::Combine(docDir, nsElement->GetAttribute("id")) + ".html");
    std::ofstream docFile(docFilePath);
    util::CodeFormatter formatter(docFile);
    formatter.SetIndentSize(1);
    doc.Write(formatter);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        std::cout << "==> " << docFilePath << std::endl;
    }
}

void GenerateModuleIndexHtml(Input* input, const std::string& moduleDir, const std::string& moduleName, soul::xml::Document* moduleXmlDoc,
    const std::vector<soul::xml::Document*>& otherModuleXmlDocs, std::vector<soul::xml::Element*>& grammarElements)
{
    soul::xml::Document* docs = input->docs.get();
    std::string docDir = util::GetFullPath(util::Path::Combine(moduleDir, "doc"));
    std::string title = moduleName + " Module";
    std::string styleFilePath = "../../style/style.css";
    soul::xml::Document indexDoc;
    std::unique_ptr<soul::xml::Element> htmlElement(soul::xml::MakeElement("html"));
    std::unique_ptr<soul::xml::Element> headElement(soul::xml::MakeElement("head"));
    std::unique_ptr<soul::xml::Element> metaElement(soul::xml::MakeElement("meta"));
    metaElement->SetAttribute("charset", "utf-8");
    headElement->AppendChild(metaElement.release());
    std::unique_ptr<soul::xml::Element> titleElement(soul::xml::MakeElement("title"));
    titleElement->AppendChild(soul::xml::MakeText(title));
    headElement->AppendChild(titleElement.release());
    std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("link"));
    linkElement->SetAttribute("rel", "stylesheet");
    linkElement->SetAttribute("type", "text/css");
    linkElement->SetAttribute("href", styleFilePath);
    headElement->AppendChild(linkElement.release());
    htmlElement->AppendChild(headElement.release());
    std::unique_ptr<soul::xml::Element> bodyElement(soul::xml::MakeElement("body"));
    std::unique_ptr<soul::xml::Element> h1Element(soul::xml::MakeElement("h1"));
    h1Element->AppendChild(soul::xml::MakeText(title));
    bodyElement->AppendChild(h1Element.release());
    std::unique_ptr<soul::xml::Element> legendSpan(soul::xml::MakeElement("span"));
    legendSpan->SetAttribute("class", "indent");
    soul::xml::Element* legend = legendSpan.get();
    bool first = true;
    bodyElement->AppendChild(legendSpan.release());
    if (docs)
    {
        std::unique_ptr<soul::xml::Element> descriptionAndDetails = GetDescriptionAndDetails(input, moduleName, docs, moduleXmlDoc, otherModuleXmlDocs, "doc/");
        if (descriptionAndDetails)
        {
            bodyElement->AppendChild(descriptionAndDetails.release());
        }
    }
    if (!grammarElements.empty())
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "grammars");
        AppendToLegend(legend, "Grammars", "grammars", first);
        h2Element->AppendChild(soul::xml::MakeText("Grammars"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> grammarTableElement(soul::xml::MakeElement("table"));
        for (soul::xml::Element* grammarElement : grammarElements)
        {
            std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
            std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
            tdElement->SetAttribute("xml:space", "preserve");
            std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
            std::string fileAttr = grammarElement->GetAttribute("file");
            std::string grammarName = util::Path::GetFileNameWithoutExtension(fileAttr);
            linkElement->SetAttribute("href", "doc/" + grammarName + ".html");
            std::string grammarFilePath = util::GetFullPath(util::Path::Combine(input->baseDir, fileAttr));
            std::unique_ptr<soul::xml::Document> grammarDoc = soul::xml::ParseXmlFile(grammarFilePath);
            std::string grammarTitle = grammarDoc->DocumentElement()->GetAttribute("title");
            linkElement->AppendChild(soul::xml::MakeText(grammarTitle));
            tdElement->AppendChild(linkElement.release());
            trElement->AppendChild(tdElement.release());
            grammarTableElement->AppendChild(trElement.release());
        }
        bodyElement->AppendChild(grammarTableElement.release());
    }
    bool hasNamespaces = false;
    std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
    h2Element->AppendChild(soul::xml::MakeText("Namespaces"));
    std::unique_ptr<soul::xml::Element> namespaceTableElement(soul::xml::MakeElement("table"));
    std::unique_ptr<soul::xml::xpath::NodeSet> rootNamespaceNodeSet = soul::xml::xpath::EvaluateToNodeSet("/symbolTable/namespace", moduleXmlDoc);
    soul::xml::Element* rootNamespaceElement = nullptr;
    int n = rootNamespaceNodeSet->Count();
    if (n == 1)
    {
        soul::xml::Node* rootNamespaceNode = rootNamespaceNodeSet->GetNode(0);
        if (rootNamespaceNode->IsElementNode())
        {
            rootNamespaceElement = static_cast<soul::xml::Element*>(rootNamespaceNode);
            std::vector<std::unique_ptr<soul::xml::xpath::NodeSet>> namespaceXPathNodeSets;
            std::vector<soul::xml::Element*> nsElements;
            if (GenerateNamespaceNames(0, namespaceTableElement.get(), rootNamespaceElement, "doc/", namespaceXPathNodeSets, nsElements))
            {
                hasNamespaces = true;
            }
            int n = static_cast<int>(nsElements.size());
            for (int i = 0; i < n; ++i)
            {
                soul::xml::Element* nsElement = nsElements[i];
                GenerateNamespaceDoc(input, docDir, nsElement, moduleXmlDoc, otherModuleXmlDocs);
            }
        }
    }
    if (hasNamespaces)
    {
        h2Element->SetAttribute("id", "namespaces");
        AppendToLegend(legend, "Namespaces", "namespaces", first);
        bodyElement->AppendChild(h2Element.release());
        bodyElement->AppendChild(namespaceTableElement.release());
    }
    std::unique_ptr<soul::xml::xpath::NodeSet> filesNodeSet = soul::xml::xpath::EvaluateToNodeSet("/symbolTable/files/file", moduleXmlDoc);
    int nf = filesNodeSet->Count();
    if (nf > 0)
    {
        std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
        h2Element->SetAttribute("id", "files");
        AppendToLegend(legend, "Files", "files", first);
        h2Element->AppendChild(soul::xml::MakeText("Files"));
        bodyElement->AppendChild(h2Element.release());
        std::unique_ptr<soul::xml::Element> fileTableElement(soul::xml::MakeElement("table"));
        for (int i = 0; i < nf; ++i)
        {
            soul::xml::Node* fileNode = filesNodeSet->GetNode(i);
            if (fileNode->IsElementNode())
            {
                soul::xml::Element* fileElement = static_cast<soul::xml::Element*>(fileNode);
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                std::unique_ptr<soul::xml::Element> fileLinkElement(soul::xml::MakeElement("a"));
                fileLinkElement->SetAttribute("href", fileElement->GetAttribute("path"));
                fileLinkElement->AppendChild(soul::xml::MakeText(fileElement->GetAttribute("name")));
                tdElement->AppendChild(fileLinkElement.release());
                trElement->AppendChild(tdElement.release());
                fileTableElement->AppendChild(trElement.release());
            }
        }
        bodyElement->AppendChild(fileTableElement.release());
    }
    if (rootNamespaceElement)
    {
        std::unique_ptr<soul::xml::xpath::NodeSet> templateParametersNodeSet;
        std::vector<soul::xml::Element*> templateParameterElements;
        std::unique_ptr<soul::xml::xpath::NodeSet> conceptNodeSet;
        std::vector<soul::xml::Element*> conceptElements;
        GetConcepts(rootNamespaceElement, conceptNodeSet, conceptElements);
        int n = static_cast<int>(conceptElements.size());
        if (n > 0)
        {
            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
            h2Element->SetAttribute("id", "concepts");
            AppendToLegend(legend, "Concepts", "concepts", first);
            h2Element->AppendChild(soul::xml::MakeText("Concepts"));
            bodyElement->AppendChild(h2Element.release());
            std::unique_ptr<soul::xml::Element> conceptTableElement(soul::xml::MakeElement("table"));
            for (int i = 0; i < n; ++i)
            {
                soul::xml::Element* conceptElement = conceptElements[i];
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                tdElement->SetAttribute("class", "rightAlignedCol");
                AppendSpecifiers(tdElement.get(), conceptElement->GetAttribute("specifiers"), "concept", false);
                trElement->AppendChild(tdElement.release());
                std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
                std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
                linkElement->SetAttribute("href", "doc/" + conceptElement->GetAttribute("id") + ".html");
                linkElement->AppendChild(soul::xml::MakeText(MakeConceptName(conceptElement)));
                td2Element->AppendChild(linkElement.release());
                trElement->AppendChild(td2Element.release());
                std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
                td3Element->SetAttribute("xml:space", "preserve");
                if (AppendDescription(td3Element.get(), input, conceptElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, "doc/"))
                {
                    trElement->AppendChild(td3Element.release());
                }
                conceptTableElement->AppendChild(trElement.release());
                GenerateConceptDoc(input, docDir, conceptElement, moduleXmlDoc, otherModuleXmlDocs);
            }
            bodyElement->AppendChild(conceptTableElement.release());
        }
        std::unique_ptr<soul::xml::xpath::NodeSet> classNodeSet;
        std::vector<soul::xml::Element*> classElements;
        GetClasses(rootNamespaceElement, classNodeSet, classElements);
        int nc = static_cast<int>(classElements.size());
        if (nc > 0)
        {
            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
            h2Element->SetAttribute("id", "classes");
            AppendToLegend(legend, "Classes", "classes", first);
            h2Element->AppendChild(soul::xml::MakeText("Classes"));
            bodyElement->AppendChild(h2Element.release());
            std::unique_ptr<soul::xml::Element> classTableElement(soul::xml::MakeElement("table"));
            for (int i = 0; i < nc; ++i)
            {
                soul::xml::Element* classElement = classElements[i];
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                tdElement->SetAttribute("class", "rightAlignedCol");
                AppendSpecifiers(tdElement.get(), classElement->GetAttribute("specifiers"), "class", false);
                trElement->AppendChild(tdElement.release());
                std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
                std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
                linkElement->SetAttribute("href", "doc/" + classElement->GetAttribute("id") + ".html");
                linkElement->AppendChild(soul::xml::MakeText(MakeClassName(classElement, templateParametersNodeSet, templateParameterElements)));
                td2Element->AppendChild(linkElement.release());
                trElement->AppendChild(td2Element.release());
                std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
                td3Element->SetAttribute("xml:space", "preserve");
                if (AppendDescription(td3Element.get(), input, classElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, "doc/"))
                {
                    trElement->AppendChild(td3Element.release());
                }
                classTableElement->AppendChild(trElement.release());
                GenerateClassDoc(input, docDir, classElement, moduleXmlDoc, otherModuleXmlDocs);
            }
            bodyElement->AppendChild(classTableElement.release());
        }
        std::unique_ptr<soul::xml::xpath::NodeSet> interfaceNodeSet;
        std::vector<soul::xml::Element*> interfaceElements;
        GetInterfaces(rootNamespaceElement, interfaceNodeSet, interfaceElements);
        int ni = static_cast<int>(interfaceElements.size());
        if (ni > 0)
        {
            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
            h2Element->SetAttribute("id", "interfaces");
            AppendToLegend(legend, "Interfaces", "interfaces", first);
            h2Element->AppendChild(soul::xml::MakeText("Interfaces"));
            bodyElement->AppendChild(h2Element.release());
            std::unique_ptr<soul::xml::Element> interfaceTableElement(soul::xml::MakeElement("table"));
            for (int i = 0; i < nc; ++i)
            {
                soul::xml::Element* interfaceElement = interfaceElements[i];
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                tdElement->SetAttribute("class", "rightAlignedCol");
                AppendSpecifiers(tdElement.get(), interfaceElement->GetAttribute("specifiers"), "interface", false);
                trElement->AppendChild(tdElement.release());
                std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
                std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
                linkElement->SetAttribute("href", interfaceElement->GetAttribute("docPath"));
                linkElement->AppendChild(soul::xml::MakeText(interfaceElement->GetAttribute("name")));
                td2Element->AppendChild(linkElement.release());
                trElement->AppendChild(td2Element.release());
                std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
                td3Element->SetAttribute("xml:space", "preserve");
                if (AppendDescription(td3Element.get(), input, interfaceElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
                {
                    trElement->AppendChild(td3Element.release());
                }
                interfaceTableElement->AppendChild(trElement.release());
                GenerateClassDoc(input, docDir, interfaceElement, moduleXmlDoc, otherModuleXmlDocs);
            }
            bodyElement->AppendChild(interfaceTableElement.release());
        }
        std::unique_ptr<soul::xml::xpath::NodeSet> functionNodeSet;
        std::vector<soul::xml::Element*> functionElements;
        GetFunctions(rootNamespaceElement, functionNodeSet, functionElements);
        int nf = static_cast<int>(functionElements.size());
        if (nf > 0)
        {
            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
            h2Element->SetAttribute("id", "functions");
            AppendToLegend(legend, "Functions", "functions", first);
            h2Element->AppendChild(soul::xml::MakeText("Functions"));
            bodyElement->AppendChild(h2Element.release());
            std::unique_ptr<soul::xml::Element> functionTableElement(soul::xml::MakeElement("table"));
            for (int i = 0; i < nf; ++i)
            {
                soul::xml::Element* functionElement = functionElements[i];
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                tdElement->SetAttribute("class", "rightAlignedCol");
                AppendSpecifiers(tdElement.get(), functionElement->GetAttribute("specifiers"), "", true);
                AppendType(input, tdElement.get(), functionElement->GetAttribute("returnType"), "RETURN_TYPE", 
                    moduleXmlDoc, otherModuleXmlDocs, "doc/");
                trElement->AppendChild(tdElement.release());
                std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
                td2Element->SetAttribute("xml:space", "preserve");
                std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
                std::string link = "doc/";
                link.append(functionElement->GetAttribute("docPath"));
                linkElement->SetAttribute("href", link);
                std::unique_ptr<soul::xml::xpath::NodeSet> templateParametersNodeSet;
                std::vector<soul::xml::Element*> templateParameterElements;
                linkElement->AppendChild(soul::xml::MakeText(MakeFunctionName(functionElement, templateParametersNodeSet, templateParameterElements)));
                td2Element->AppendChild(linkElement.release());
                AppendParameters(input, td2Element.get(), functionElement, moduleXmlDoc, otherModuleXmlDocs, "doc/");
                if (functionElement->GetAttribute("includeConstraint") == "true")
                {
                    std::unique_ptr<soul::xml::xpath::NodeSet> constraintNodeSet;
                    soul::xml::Element* constraintElement = GetConstraint(functionElement, constraintNodeSet);
                    if (constraintElement)
                    {
                        td2Element->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                        AppendConstraint(input, td2Element.get(), constraintElement, moduleXmlDoc, otherModuleXmlDocs, "doc/", true);
                    }
                }
                trElement->AppendChild(td2Element.release());
                std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
                td3Element->SetAttribute("xml:space", "preserve");
                if (AppendDescription(td3Element.get(), input, functionElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, "doc/"))
                {
                    trElement->AppendChild(td3Element.release());
                }
                functionTableElement->AppendChild(trElement.release());
            }
            bodyElement->AppendChild(functionTableElement.release());
        }
        std::unique_ptr<soul::xml::xpath::NodeSet> delegateNodeSet;
        std::vector<soul::xml::Element*> delegateElements;
        GetDelegates(rootNamespaceElement, delegateNodeSet, delegateElements);
        int ndlg = static_cast<int>(delegateElements.size());
        if (ndlg > 0)
        {
            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
            h2Element->SetAttribute("id", "delegates");
            AppendToLegend(legend, "Delegates", "delegates", first);
            h2Element->AppendChild(soul::xml::MakeText("Delegates"));
            bodyElement->AppendChild(h2Element.release());
            std::unique_ptr<soul::xml::Element> delegateTableElement(soul::xml::MakeElement("table"));
            for (int i = 0; i < ndlg; ++i)
            {
                soul::xml::Element* delegateElement = delegateElements[i];
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                trElement->SetAttribute("id", delegateElement->GetAttribute("id"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                tdElement->SetAttribute("class", "rightAlignedCol");
                AppendSpecifiers(tdElement.get(), delegateElement->GetAttribute("specifiers"), "delegate", true);
                AppendType(input, tdElement.get(), delegateElement->GetAttribute("returnType"), "RETURN_TYPE", 
                    moduleXmlDoc, otherModuleXmlDocs, "doc/");
                trElement->AppendChild(tdElement.release());
                std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
                td2Element->AppendChild(soul::xml::MakeText(delegateElement->GetAttribute("name")));
                AppendParameters(input, td2Element.get(), delegateElement, moduleXmlDoc, otherModuleXmlDocs, "doc/");
                trElement->AppendChild(td2Element.release());
                std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
                td3Element->SetAttribute("xml:space", "preserve");
                if (AppendDescription(td3Element.get(), input, delegateElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, "doc/"))
                {
                    trElement->AppendChild(td3Element.release());
                }
                delegateTableElement->AppendChild(trElement.release());
            }
            bodyElement->AppendChild(delegateTableElement.release());
        }
        std::unique_ptr<soul::xml::xpath::NodeSet> classDelegateNodeSet;
        std::vector<soul::xml::Element*> classDelegateElements;
        GetClassDelegates(rootNamespaceElement, classDelegateNodeSet, classDelegateElements);
        int ncdlg = static_cast<int>(classDelegateElements.size());
        if (ncdlg > 0)
        {
            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
            h2Element->SetAttribute("id", "classDelegates");
            AppendToLegend(legend, "Class Delegates", "classDelegates", first);
            h2Element->AppendChild(soul::xml::MakeText("Class Delegates"));
            bodyElement->AppendChild(h2Element.release());
            std::unique_ptr<soul::xml::Element> classDelegateTableElement(soul::xml::MakeElement("table"));
            for (int i = 0; i < ncdlg; ++i)
            {
                soul::xml::Element* classDelegateElement = classDelegateElements[i];
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                trElement->SetAttribute("id", classDelegateElement->GetAttribute("id"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                tdElement->SetAttribute("class", "rightAlignedCol");
                AppendSpecifiers(tdElement.get(), classDelegateElement->GetAttribute("specifiers"), "class delegate", true);
                AppendType(input, tdElement.get(), classDelegateElement->GetAttribute("returnType"), "RETURN_TYPE", 
                    moduleXmlDoc, otherModuleXmlDocs, "doc/");
                trElement->AppendChild(tdElement.release());
                std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
                td2Element->AppendChild(soul::xml::MakeText(classDelegateElement->GetAttribute("name")));
                AppendParameters(input, td2Element.get(), classDelegateElement, moduleXmlDoc, otherModuleXmlDocs, "doc/");
                trElement->AppendChild(td2Element.release());
                std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
                td3Element->SetAttribute("xml:space", "preserve");
                if (AppendDescription(td3Element.get(), input, classDelegateElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, "doc/"))
                {
                    trElement->AppendChild(td3Element.release());
                }
                classDelegateTableElement->AppendChild(trElement.release());
            }
            bodyElement->AppendChild(classDelegateTableElement.release());
        }
        std::unique_ptr<soul::xml::xpath::NodeSet> typedefNodeSet;
        std::vector<soul::xml::Element*> typedefElements;
        GetTypedefs(rootNamespaceElement, typedefNodeSet, typedefElements);
        int nt = static_cast<int>(typedefElements.size());
        if (nt > 0)
        {
            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
            h2Element->SetAttribute("id", "typeAliases");
            AppendToLegend(legend, "Type Aliases", "typeAliases", first);
            h2Element->AppendChild(soul::xml::MakeText("Type Aliases"));
            bodyElement->AppendChild(h2Element.release());
            std::unique_ptr<soul::xml::Element> typedefTableElement(soul::xml::MakeElement("table"));
            for (int i = 0; i < nt; ++i)
            {
                soul::xml::Element* typedefElement = typedefElements[i];
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                trElement->SetAttribute("id", typedefElement->GetAttribute("id"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                tdElement->SetAttribute("class", "rightAlignedCol");
                AppendSpecifiers(tdElement.get(), typedefElement->GetAttribute("specifiers"), "typedef", true);
                tdElement->AppendChild(soul::xml::MakeText(typedefElement->GetAttribute("name")));
                trElement->AppendChild(tdElement.release());
                std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
                td2Element->SetAttribute("xml:space", "preserve");
                AppendType(input, td2Element.get(), typedefElement->GetAttribute("type"), "TYPE", moduleXmlDoc, otherModuleXmlDocs, "doc/");
                trElement->AppendChild(td2Element.release());
                std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
                td3Element->SetAttribute("xml:space", "preserve");
                if (AppendDescription(td3Element.get(), input, typedefElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, std::string()))
                {
                    trElement->AppendChild(td3Element.release());
                }
                typedefTableElement->AppendChild(trElement.release());
            }
            bodyElement->AppendChild(typedefTableElement.release());
        }
        std::unique_ptr<soul::xml::xpath::NodeSet> enumTypesNodeSet;
        std::vector<soul::xml::Element*> enumTypeElements;
        GetEnumTypes(rootNamespaceElement, enumTypesNodeSet, enumTypeElements);
        int ne = static_cast<int>(enumTypeElements.size());
        if (ne > 0)
        {
            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
            h2Element->SetAttribute("id", "enumTypes");
            AppendToLegend(legend, "Enumerated Types", "enumTypes", first);
            h2Element->AppendChild(soul::xml::MakeText("Enumerated Types"));
            bodyElement->AppendChild(h2Element.release());
            std::unique_ptr<soul::xml::Element> enumTypeTableElement(soul::xml::MakeElement("table"));
            for (int i = 0; i < ne; ++i)
            {
                soul::xml::Element* enumTypeElement = enumTypeElements[i];
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                tdElement->SetAttribute("class", "rightAlignedCol");
                AppendSpecifiers(tdElement.get(), enumTypeElement->GetAttribute("specifiers"), "enum", false);
                trElement->AppendChild(tdElement.release());
                std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
                td2Element->SetAttribute("xml:space", "preserve");
                std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
                linkElement->SetAttribute("href", "doc/" + enumTypeElement->GetAttribute("docPath"));
                linkElement->AppendChild(soul::xml::MakeText(enumTypeElement->GetAttribute("name")));
                td2Element->AppendChild(linkElement.release());
                trElement->AppendChild(td2Element.release());
                std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
                td3Element->SetAttribute("xml:space", "preserve");
                if (AppendDescription(td3Element.get(), input, enumTypeElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, "doc/"))
                {
                    trElement->AppendChild(td3Element.release());
                }
                enumTypeTableElement->AppendChild(trElement.release());
                GenerateEnumDoc(input, docDir, enumTypeElement, moduleXmlDoc, otherModuleXmlDocs);
            }
            bodyElement->AppendChild(enumTypeTableElement.release());
        }
        std::unique_ptr<soul::xml::xpath::NodeSet> constantsNodeSet;
        std::vector<soul::xml::Element*> constantElements;
        GetConstants(rootNamespaceElement, constantsNodeSet, constantElements);
        int nco = static_cast<int>(constantElements.size());
        if (nco > 0)
        {
            std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
            h2Element->SetAttribute("id", "constants");
            AppendToLegend(legend, "Constants", "constants", first);
            h2Element->AppendChild(soul::xml::MakeText("Constants"));
            bodyElement->AppendChild(h2Element.release());
            std::unique_ptr<soul::xml::Element> constantTableElement(soul::xml::MakeElement("table"));
            for (int i = 0; i < nco; ++i)
            {
                soul::xml::Element* constantElement = constantElements[i];
                std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
                std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
                tdElement->SetAttribute("xml:space", "preserve");
                tdElement->SetAttribute("class", "rightAlignedCol");
                tdElement->SetAttribute("id", constantElement->GetAttribute("id"));
                AppendSpecifiers(tdElement.get(), constantElement->GetAttribute("specifiers"), "const", true);
                AppendType(input, tdElement.get(), constantElement->GetAttribute("type"), "CONSTANT_TYPE", 
                    moduleXmlDoc, otherModuleXmlDocs, "doc/");
                tdElement->AppendChild(soul::xml::MakeEntityReference("nbsp"));
                tdElement->AppendChild(soul::xml::MakeText(constantElement->GetAttribute("name")));
                trElement->AppendChild(tdElement.release());
                std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
                td2Element->SetAttribute("xml:space", "preserve");
                td2Element->AppendChild(soul::xml::MakeText(constantElement->GetAttribute("value")));
                trElement->AppendChild(td2Element.release());
                std::unique_ptr<soul::xml::Element> td3Element(soul::xml::MakeElement("td"));
                td3Element->SetAttribute("xml:space", "preserve");
                if (AppendDescription(td3Element.get(), input, constantElement->GetAttribute("id"), docs, moduleXmlDoc, otherModuleXmlDocs, "doc/"))
                {
                    trElement->AppendChild(td3Element.release());
                }
                constantTableElement->AppendChild(trElement.release());
            }
            bodyElement->AppendChild(constantTableElement.release());
        }
        GenerateFunctionDetailsSection(input, bodyElement.get(), "Function Details", functionElements, moduleXmlDoc, otherModuleXmlDocs, "doc/");
    }
    htmlElement->AppendChild(bodyElement.release());
    indexDoc.AppendChild(htmlElement.release());
    std::string indexFilePath = util::GetFullPath(util::Path::Combine(moduleDir, "index.html"));
    std::ofstream indexFile(indexFilePath);
    util::CodeFormatter formatter(indexFile);
    formatter.SetIndentSize(1);
    indexDoc.Write(formatter);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        std::cout << "==> " << indexFilePath << std::endl;
    }
}

void GenerateRootIndexHtml(Input* input, const std::string& targetDir, const std::string& solutionName, const std::vector<std::string>& moduleNames,
    const std::vector<std::string>& moduleLinks, const std::vector<std::unique_ptr<soul::xml::Document>>& moduleXmlDocs)
{
    soul::xml::Document* moduleXmlDoc = nullptr;
    std::vector<soul::xml::Document*> otherModuleXmlDocs;
    for (const auto& doc : moduleXmlDocs)
    {
        otherModuleXmlDocs.push_back(doc.get());
    }
    soul::xml::Document* docs = input->docs.get();
    std::string title = solutionName + " Reference";
    std::string styleFilePath = "style/style.css";
    soul::xml::Document indexDoc;
    std::unique_ptr<soul::xml::Element> htmlElement(soul::xml::MakeElement("html"));
    std::unique_ptr<soul::xml::Element> headElement(soul::xml::MakeElement("head"));
    std::unique_ptr<soul::xml::Element> metaElement(soul::xml::MakeElement("meta"));
    metaElement->SetAttribute("charset", "utf-8");
    headElement->AppendChild(metaElement.release());
    std::unique_ptr<soul::xml::Element> titleElement(soul::xml::MakeElement("title"));
    titleElement->AppendChild(soul::xml::MakeText(title));
    headElement->AppendChild(titleElement.release());
    std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("link"));
    linkElement->SetAttribute("rel", "stylesheet");
    linkElement->SetAttribute("type", "text/css");
    linkElement->SetAttribute("href", styleFilePath);
    headElement->AppendChild(linkElement.release());
    htmlElement->AppendChild(headElement.release());
    std::unique_ptr<soul::xml::Element> bodyElement(soul::xml::MakeElement("body"));
    std::unique_ptr<soul::xml::Element> h1Element(soul::xml::MakeElement("h1"));
    h1Element->AppendChild(soul::xml::MakeText(title));
    bodyElement->AppendChild(h1Element.release());
    if (docs)
    {
        std::unique_ptr<soul::xml::Element> descriptionAndDetails = GetDescriptionAndDetails(input, "solution", docs, 
            moduleXmlDoc, otherModuleXmlDocs, "content/System/doc");
        if (descriptionAndDetails)
        {
            bodyElement->AppendChild(descriptionAndDetails.release());
        }
    }
    std::unique_ptr<soul::xml::Element> h2Element(soul::xml::MakeElement("h2"));
    h2Element->AppendChild(soul::xml::MakeText("Modules"));
    bodyElement->AppendChild(h2Element.release());
    std::unique_ptr<soul::xml::Element> moduleTableElement(soul::xml::MakeElement("table"));
    int n = static_cast<int>(moduleNames.size());
    for (int i = n - 1; i >= 0; --i)
    {
        std::unique_ptr<soul::xml::Element> trElement(soul::xml::MakeElement("tr"));
        const std::string& moduleName = moduleNames[i];
        std::string moduleLink = moduleLinks[i];
        std::unique_ptr<soul::xml::Element> tdElement(soul::xml::MakeElement("td"));
        std::unique_ptr<soul::xml::Element> linkElement(soul::xml::MakeElement("a"));
        linkElement->AppendChild(soul::xml::MakeText(moduleName));
        linkElement->SetAttribute("href", moduleLink);
        tdElement->AppendChild(linkElement.release());
        trElement->AppendChild(tdElement.release());
        std::unique_ptr<soul::xml::Element> td2Element(soul::xml::MakeElement("td"));
        if (AppendDescription(td2Element.get(), input, moduleName, docs, moduleXmlDoc, otherModuleXmlDocs, "content/System/doc/"))
        {
            trElement->AppendChild(td2Element.release());
        }
        moduleTableElement->AppendChild(trElement.release());
    }
    bodyElement->AppendChild(moduleTableElement.release());
    htmlElement->AppendChild(bodyElement.release());
    indexDoc.AppendChild(htmlElement.release());
    std::string indexFilePath = util::GetFullPath(util::Path::Combine(targetDir, "index.html"));
    std::ofstream indexFile(indexFilePath);
    util::CodeFormatter formatter(indexFile);
    formatter.SetIndentSize(1);
    indexDoc.Write(formatter);
    if (cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose))
    {
        std::cout << "==> " << indexFilePath << std::endl;
    }
}

void BuildDocs(const std::string& solutionName, const std::vector<std::string>& moduleNames)
{
    bool verbose = cmajor::symbols::GetGlobalFlag(cmajor::symbols::GlobalFlags::verbose);
    if (verbose)
    {
        std::cout << "Building documentation for solution '" << solutionName + "'..." << "\n";
    }
    Input* input = GetInputPtr();
    std::string targetDir = input->targetDirPath;
    std::filesystem::create_directories(targetDir);
    std::string contentDir = util::GetFullPath(util::Path::Combine(targetDir, "content"));
    std::filesystem::create_directories(contentDir);
    std::vector<std::unique_ptr<soul::xml::Document>> moduleXmlFiles;
    std::vector<std::string> moduleLinks;
    for (const std::string& moduleName : moduleNames)
    {
        std::string moduleDir = util::GetFullPath(util::Path::Combine(contentDir, moduleName));
        std::filesystem::create_directories(moduleDir);
        std::string moduleXmlFilePath = util::Path::Combine(moduleDir, moduleName + ".xml");
        std::string moduleLink = util::Path::Combine(util::Path::Combine("content", moduleName), "index.html");
        moduleLinks.push_back(moduleLink);
        std::string docDir = util::GetFullPath(util::Path::Combine(moduleDir, "doc"));
        std::filesystem::create_directories(docDir);
        if (verbose)
        {
            std::cout << "> " << moduleXmlFilePath << std::endl;
        }
        std::unique_ptr<soul::xml::Document> moduleXmlFile = soul::xml::ParseXmlFile(moduleXmlFilePath);
        moduleXmlFiles.push_back(std::move(moduleXmlFile));
    }
    int n = static_cast<int>(moduleNames.size());
    for (int i = 0; i < n; ++i)
    {
        const std::string& moduleName = moduleNames[i];
        std::string moduleDir = util::GetFullPath(util::Path::Combine(contentDir, moduleName));
        std::string relativeModuleDir = util::Path::Combine("content", moduleName);
        soul::xml::Document* moduleXmlDoc = moduleXmlFiles[i].get();
        std::vector<soul::xml::Document*> otherModuleXmlDocs;
        std::vector<soul::xml::Element*> grammarElements;
        for (const auto& externalModuleDoc : input->externalModuleDocs)
        {
            otherModuleXmlDocs.push_back(externalModuleDoc.get());
        }
        for (int j = 0; j < n; ++j)
        {
            if (i != j)
            {
                otherModuleXmlDocs.push_back(moduleXmlFiles[j].get());
            }
        }
        GenerateModuleIndexHtml(input, moduleDir, moduleName, moduleXmlDoc, otherModuleXmlDocs, grammarElements);
    }
    GenerateRootIndexHtml(input, targetDir, solutionName, moduleNames, moduleLinks, moduleXmlFiles);
    if (verbose)
    {
        std::cout << "Documentation generated." << std::endl;
    }
}

} // namespace cmdoclib
