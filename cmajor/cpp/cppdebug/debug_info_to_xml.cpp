// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.debug.info.to.xml;

import cmajor.debug.io;
import cmajor.debug.debug.info;
import cmajor.debug.di.variable;
import cmajor.debug.di.enum_constant;
import cmajor.debug.di.type;
import soul.xml.dom;
import util;

namespace cmajor::debug {

soul::xml::Element* VariableToXmlElement(DIVariable* variable)
{
    soul::xml::Element* variableElement = soul::xml::MakeElement("variable");
    variableElement->SetAttribute("name", variable->Name());
    variableElement->SetAttribute("irName", variable->IrName());
    variableElement->SetAttribute("typeId", util::ToString(variable->TypeId()));
    return variableElement;
}

soul::xml::Element* TypeToXmlElement(DIType* type)
{
    soul::xml::Element* typeElement = typeElement = soul::xml::MakeElement("type");
    switch (type->GetKind())
    {
    case DIType::Kind::primitiveType:
    {
        DIPrimitiveType* primitiveType = static_cast<DIPrimitiveType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(primitiveType->GetKind()));
        typeElement->SetAttribute("name", primitiveType->Name());
        typeElement->SetAttribute("irName", primitiveType->IrName());
        typeElement->SetAttribute("id", util::ToString(primitiveType->Id()));
        break;
    }
    case DIType::Kind::enumType:
    {
        DIEnumType* enumType = static_cast<DIEnumType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(enumType->GetKind()));
        typeElement->SetAttribute("name", enumType->Name());
        typeElement->SetAttribute("irName", enumType->IrName());
        typeElement->SetAttribute("id", util::ToString(enumType->Id()));
        typeElement->SetAttribute("underlyingTypeId", util::ToString(enumType->UnderlyingTypeId()));
        soul::xml::Element* enumConstantsElement = soul::xml::MakeElement("enumConstants");
        int n = enumType->EnumConstants().size();
        for (int i = 0; i < n; ++i)
        {
            const DIEnumConstant& enumConstant = enumType->EnumConstants()[i];
            soul::xml::Element* enumConstantElement = soul::xml::MakeElement("enumConstant");
            enumConstantElement->SetAttribute("name", enumConstant.Name());
            enumConstantElement->SetAttribute("value", enumConstant.Value());
            enumConstantElement->SetAttribute("strValue", enumConstant.StrValue());
            enumConstantsElement->AppendChild(enumConstantElement);
        }
        typeElement->AppendChild(enumConstantsElement);
        break;
    }
    case DIType::Kind::templateParameter:
    {
        DITemplateParameter* templateParameter = static_cast<DITemplateParameter*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(templateParameter->GetKind()));
        typeElement->SetAttribute("name", templateParameter->Name());
        typeElement->SetAttribute("id", util::ToString(templateParameter->Id()));
        break;
    }
    case DIType::Kind::classType:
    {
        DIClassType* classType = static_cast<DIClassType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(classType->GetKind()));
        typeElement->SetAttribute("name", classType->Name());
        typeElement->SetAttribute("irName", classType->IrName());
        typeElement->SetAttribute("id", util::ToString(classType->Id()));
        if (!classType->BaseClassId().is_nil())
        {
            typeElement->SetAttribute("baseClassId", util::ToString(classType->BaseClassId()));
        }
        if (!classType->TemplateParameters().empty())
        {
            soul::xml::Element* templateParametersElement = soul::xml::MakeElement("templateParameters");
            int n = classType->TemplateParameters().size();
            for (int i = 0; i < n; ++i)
            {
                DITemplateParameter* templateParameter = classType->TemplateParameters()[i].get();
                soul::xml::Element* templateParameterElement = TypeToXmlElement(templateParameter);
                templateParametersElement->AppendChild(templateParameterElement);
            }
            typeElement->AppendChild(templateParametersElement);
        }
        if (!classType->MemberVariables().empty())
        {
            soul::xml::Element* memberVariablesElement = soul::xml::MakeElement("memberVariables");
            int n = classType->MemberVariables().size();
            for (int i = 0; i < n; ++i)
            {
                DIVariable* memberVariable = classType->MemberVariables()[i].get();
                soul::xml::Element* memberVariableElement = VariableToXmlElement(memberVariable);
                memberVariablesElement->AppendChild(memberVariableElement);
            }
            typeElement->AppendChild(memberVariablesElement);
        }
        if (classType->IsPolymorphic())
        {
            typeElement->SetAttribute("polymorphic", "true");
            int32_t vmtPtrIndex = classType->VmtPtrIndex();
            typeElement->SetAttribute("vmtPtrIndex", std::to_string(vmtPtrIndex));
            typeElement->SetAttribute("vmtVariableName", classType->VmtVariableName());
        }
        break;
    }
    case DIType::Kind::specializationType:
    {
        DIClassTemplateSpecializationType* specializationType = static_cast<DIClassTemplateSpecializationType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(specializationType->GetKind()));
        typeElement->SetAttribute("name", specializationType->Name());
        typeElement->SetAttribute("irName", specializationType->IrName());
        typeElement->SetAttribute("id", util::ToString(specializationType->Id()));
        util::uuid primaryTypeId = specializationType->PrimaryTypeId();
        typeElement->SetAttribute("primaryTypeId", util::ToString(primaryTypeId));
        if (!specializationType->BaseClassId().is_nil())
        {
            typeElement->SetAttribute("baseClassId", util::ToString(specializationType->BaseClassId()));
        }
        if (!specializationType->MemberVariables().empty())
        {
            soul::xml::Element* memberVariablesElement = soul::xml::MakeElement("memberVariables");
            int n = specializationType->MemberVariables().size();
            for (int i = 0; i < n; ++i)
            {
                DIVariable* memberVariable = specializationType->MemberVariables()[i].get();
                soul::xml::Element* memberVariableElement = VariableToXmlElement(memberVariable);
                memberVariablesElement->AppendChild(memberVariableElement);
            }
            typeElement->AppendChild(memberVariablesElement);
        }
        if (specializationType->GetContainerClassTemplateKind() != ContainerClassTemplateKind::notContainerClassTemplate)
        {
            typeElement->SetAttribute("container", ContainerName(specializationType->GetContainerClassTemplateKind()));
            typeElement->SetAttribute("valueTypeId", util::ToString(specializationType->ValueTypeId()));
        }
        soul::xml::Element* templateArgumentTypesElement = soul::xml::MakeElement("templateArgumentTypes");
        int32_t n = specializationType->TemplateArgumentTypeIds().size();
        for (int32_t i = 0; i < n; ++i)
        {
            soul::xml::Element* templateArgumentTypeElement = soul::xml::MakeElement("templateArgumentType");
            templateArgumentTypeElement->SetAttribute("templateArgumentTypeId", util::ToString(specializationType->TemplateArgumentTypeIds()[i]));
            templateArgumentTypesElement->AppendChild(templateArgumentTypeElement);
        }
        typeElement->AppendChild(templateArgumentTypesElement);
        if (specializationType->IsPolymorphic())
        {
            typeElement->SetAttribute("polymorphic", "true");
            int32_t vmtPtrIndex = specializationType->VmtPtrIndex();
            typeElement->SetAttribute("vmtPtrIndex", std::to_string(vmtPtrIndex));
            typeElement->SetAttribute("vmtVariableName", specializationType->VmtVariableName());
        }
        break;
    }
    case DIType::Kind::delegateType:
    {
        DIDelegateType* delegateType = static_cast<DIDelegateType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(delegateType->GetKind()));
        typeElement->SetAttribute("name", delegateType->Name());
        typeElement->SetAttribute("irName", delegateType->IrName());
        typeElement->SetAttribute("id", util::ToString(delegateType->Id()));
        break;
    }
    case DIType::Kind::classDelegateType:
    {
        DIClassDelegateType* classDelegateType = static_cast<DIClassDelegateType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(classDelegateType->GetKind()));
        typeElement->SetAttribute("name", classDelegateType->Name());
        typeElement->SetAttribute("irName", classDelegateType->IrName());
        typeElement->SetAttribute("id", util::ToString(classDelegateType->Id()));
        break;
    }
    case DIType::Kind::interfaceType:
    {
        DIInterfaceType* interfaceType = static_cast<DIInterfaceType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(interfaceType->GetKind()));
        typeElement->SetAttribute("name", interfaceType->Name());
        typeElement->SetAttribute("irName", interfaceType->IrName());
        typeElement->SetAttribute("id", util::ToString(interfaceType->Id()));
        break;
    }
    case DIType::Kind::constType:
    {
        DIConstType* constType = static_cast<DIConstType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(constType->GetKind()));
        typeElement->SetAttribute("name", constType->Name());
        typeElement->SetAttribute("irName", constType->IrName());
        typeElement->SetAttribute("id", util::ToString(constType->Id()));
        typeElement->SetAttribute("baseTypeId", util::ToString(constType->BaseTypeId()));
        break;
    }
    case DIType::Kind::referenceType:
    {
        DIReferenceType* referenceType = static_cast<DIReferenceType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(referenceType->GetKind()));
        typeElement->SetAttribute("name", referenceType->Name());
        typeElement->SetAttribute("irName", referenceType->IrName());
        typeElement->SetAttribute("id", util::ToString(referenceType->Id()));
        typeElement->SetAttribute("baseTypeId", util::ToString(referenceType->BaseTypeId()));
        break;
    }
    case DIType::Kind::pointerType:
    {
        DIPointerType* pointerType = static_cast<DIPointerType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(pointerType->GetKind()));
        typeElement->SetAttribute("name", pointerType->Name());
        typeElement->SetAttribute("irName", pointerType->IrName());
        typeElement->SetAttribute("id", util::ToString(pointerType->Id()));
        typeElement->SetAttribute("pointedTypeId", util::ToString(pointerType->PointedTypeId()));
        break;
    }
    case DIType::Kind::arrayType:
    {
        DIArrayType* arrayType = static_cast<DIArrayType*>(type);
        typeElement->SetAttribute("kind", DIType::KindStr(arrayType->GetKind()));
        typeElement->SetAttribute("name", arrayType->Name());
        typeElement->SetAttribute("irName", arrayType->IrName());
        typeElement->SetAttribute("id", util::ToString(arrayType->Id()));
        typeElement->SetAttribute("elementTypeId", util::ToString(arrayType->ElementTypeId()));
        typeElement->SetAttribute("size", std::to_string(arrayType->Size()));
        break;
    }
    }
    return typeElement;
}

std::unique_ptr<soul::xml::Document> GetDebugInfoAsXml(const std::string& cmdbFilePath)
{
    std::unique_ptr<soul::xml::Document> doc(new soul::xml::Document());
    soul::xml::Element* cmdbElement = soul::xml::MakeElement("cmdb");
    doc->AppendChild(cmdbElement);
    util::FileStream fileStream(cmdbFilePath, util::OpenMode::read | util::OpenMode::binary);
    util::BufferedStream bufferedStream(fileStream);
    util::BinaryStreamReader reader(bufferedStream);
    ReadCmdbFileTag(reader, cmdbFilePath);
    std::string mainProjectName;
    ReadMainProjectName(reader, mainProjectName);
    cmdbElement->SetAttribute("mainProjectName", mainProjectName);
    int32_t numProjects;
    ReadNumberOfProjects(reader, numProjects);
    for (int32_t i = 0; i < numProjects; ++i)
    {
        soul::xml::Element* projectElement = soul::xml::MakeElement("project");
        std::string projectName;
        std::string projectDirectoryPath;
        util::uuid moduleId;
        int32_t numCompileUnits;
        util::uuid mainFunctionId;
        ReadProjectTableHeader(reader, projectName, projectDirectoryPath, moduleId, numCompileUnits, mainFunctionId);
        projectElement->SetAttribute("name", projectName);
        projectElement->SetAttribute("directoryPath", projectDirectoryPath);
        projectElement->SetAttribute("moduleId", util::ToString(moduleId));
        if (!mainFunctionId.is_nil())
        {
            projectElement->SetAttribute("mainFunctionId", util::ToString(mainFunctionId));
        }
        for (int32_t i = 0; i < numCompileUnits; ++i)
        {
            soul::xml::Element* compileUnitElement = soul::xml::MakeElement("compileUnit");
            std::string compileUnitBaseName;
            ReadProjectTableRecord(reader, compileUnitBaseName);
            compileUnitElement->SetAttribute("baseName", compileUnitBaseName);
            int32_t numFunctions;
            ReadNumberOfCompileUnitFunctionRecords(reader, numFunctions);
            for (int32_t i = 0; i < numFunctions; ++i)
            {
                int32_t fileIndex;
                util::uuid sourceModuleId;
                util::uuid functionId;
                ReadCompileUnitFunctionRecord(reader, fileIndex, sourceModuleId, functionId);
                soul::xml::Element* functionElement = soul::xml::MakeElement("function");
                functionElement->SetAttribute("fileIndex", std::to_string(fileIndex));
                functionElement->SetAttribute("sourceModuleId", util::ToString(sourceModuleId));
                functionElement->SetAttribute("functionId", util::ToString(functionId));
                int32_t numInstructionRecords;
                ReadNumberOfInstructionRecords(reader, numInstructionRecords);
                for (int32_t i = 0; i < numInstructionRecords; ++i)
                {
                    int32_t cppLineNumber;
                    SourceSpan span;
                    int32_t cppLineIndex;
                    int16_t scopeId;
                    int16_t flags;
                    ReadInstructionRecord(reader, cppLineNumber, span, cppLineIndex, scopeId, flags);
                    soul::xml::Element* instructionElement = soul::xml::MakeElement("instruction");
                    instructionElement->SetAttribute("cppLineNumber", std::to_string(cppLineNumber));
                    instructionElement->SetAttribute("span", span.ToString());
                    instructionElement->SetAttribute("cppLineIndex", std::to_string(cppLineIndex));
                    instructionElement->SetAttribute("scopeId", std::to_string(scopeId));
                    if (flags != 0)
                    {
                        instructionElement->SetAttribute("flags", InstructionFlagsStr(static_cast<InstructionFlags>(flags)));
                    }
                    functionElement->AppendChild(instructionElement);
                }
                soul::xml::Element* scopesElement = nullptr;
                int32_t numScopes;
                ReadNumberOfScopes(reader, numScopes);
                if (numScopes > 0)
                {
                    scopesElement = soul::xml::MakeElement("scopes");
                }
                for (int32_t i = 0; i < numScopes; ++i)
                {
                    int16_t scopeId;
                    int16_t parentScopeId;
                    int32_t numLocalVariables;
                    ReadScopeRecord(reader, scopeId, parentScopeId, numLocalVariables);
                    soul::xml::Element* scopeElement = soul::xml::MakeElement("scope");
                    scopeElement->SetAttribute("id", std::to_string(scopeId));
                    scopeElement->SetAttribute("parentScopeId", std::to_string(parentScopeId));
                    for (int32_t i = 0; i < numLocalVariables; ++i)
                    {
                        DIVariable localVariable(DIVariable::Kind::localVariable);
                        localVariable.Read(reader);
                        soul::xml::Element* localVariableElement = soul::xml::MakeElement("localVar");
                        localVariableElement->SetAttribute("name", localVariable.Name());
                        localVariableElement->SetAttribute("irName", localVariable.IrName());
                        localVariableElement->SetAttribute("typeId", util::ToString(localVariable.TypeId()));
                        scopeElement->AppendChild(localVariableElement);
                    }
                    scopesElement->AppendChild(scopeElement);
                }
                if (scopesElement)
                {
                    functionElement->AppendChild(scopesElement);
                }
                soul::xml::Element* controlFlowGraphElement = soul::xml::MakeElement("controlFlowGraph");
                int32_t controlFlowGraphNodeCount;
                ReadControlFlowGraphNodeCount(reader, controlFlowGraphNodeCount);
                for (int32_t i = 0; i < controlFlowGraphNodeCount; ++i)
                {
                    int32_t nodeId;
                    SourceSpan span;
                    int32_t cppLineIndex;
                    int32_t cppLineNumber;
                    ReadControlFlowGraphNode(reader, nodeId, span, cppLineIndex, cppLineNumber);
                    soul::xml::Element* controlFlowGraphNodeElement = soul::xml::MakeElement("controlFlowGraphNode");
                    controlFlowGraphNodeElement->SetAttribute("nodeId", std::to_string(nodeId));
                    controlFlowGraphNodeElement->SetAttribute("span", span.ToString());
                    controlFlowGraphNodeElement->SetAttribute("cppLineIndex", std::to_string(cppLineIndex));
                    controlFlowGraphNodeElement->SetAttribute("cppLineNumber", std::to_string(cppLineNumber));
                    int32_t edgeCount;
                    ReadControlFlowGraphNodeEdgeCount(reader, edgeCount);
                    for (int32_t i = 0; i < edgeCount; ++i)
                    {
                        int32_t endNodeId;
                        ReadControlFlowGraphNodeEdge(reader, endNodeId);
                        soul::xml::Element* nextElement = soul::xml::MakeElement("next");
                        nextElement->SetAttribute("id", std::to_string(endNodeId));
                        controlFlowGraphNodeElement->AppendChild(nextElement);
                    }
                    controlFlowGraphElement->AppendChild(controlFlowGraphNodeElement);
                }
                functionElement->AppendChild(controlFlowGraphElement);
                compileUnitElement->AppendChild(functionElement);
            }
            projectElement->AppendChild(compileUnitElement);
        }
        soul::xml::Element* filesElement = soul::xml::MakeElement("files");
        int32_t numFileIndexRecords;
        ReadNumberOfFileIndexRecords(reader, numFileIndexRecords);
        for (int32_t i = 0; i < numFileIndexRecords; ++i)
        {
            int32_t fileIndex;
            std::string sourceFilePath;
            ReadFileIndexRecord(reader, fileIndex, sourceFilePath);
            soul::xml::Element* fileElement = soul::xml::MakeElement("file");
            fileElement->SetAttribute("index", std::to_string(fileIndex));
            fileElement->SetAttribute("sourceFilePath", sourceFilePath);
            filesElement->AppendChild(fileElement);
        }
        projectElement->AppendChild(filesElement);
        soul::xml::Element* functionsElement = soul::xml::MakeElement("functions");
        int32_t numFunctionIndexFunctionRecords;
        ReadNumberOfFunctionIndexFunctionRecords(reader, numFunctionIndexFunctionRecords);
        for (int32_t i = 0; i < numFunctionIndexFunctionRecords; ++i)
        {
            util::uuid functionId;
            std::string fullFunctionName;
            std::string mangledFunctionName;
            ReadFunctionIndexFunctionRecord(reader, functionId, fullFunctionName, mangledFunctionName);
            soul::xml::Element* functionElement = soul::xml::MakeElement("function");
            functionElement->SetAttribute("functionId", util::ToString(functionId));
            functionElement->SetAttribute("fullName", fullFunctionName);
            functionElement->SetAttribute("mangledName", mangledFunctionName);
            functionsElement->AppendChild(functionElement);
        }
        projectElement->AppendChild(functionsElement);
        soul::xml::Element* typesElement = soul::xml::MakeElement("types");
        int32_t numTypeIndexRecords = reader.ReadInt();
        for (int32_t i = 0; i < numTypeIndexRecords; ++i)
        {
            std::unique_ptr<DIType> type = ReadType(reader, nullptr);
            soul::xml::Element* typeElement = TypeToXmlElement(type.get());
            typesElement->AppendChild(typeElement);
        }
        projectElement->AppendChild(typesElement);
        cmdbElement->AppendChild(projectElement);
    }
    return doc;
}

void WriteXmlDebugInfo(soul::xml::Document* debugInfoXmlDoc, const std::string& xmlDebugInfoFilePath)
{
    std::ofstream strm(xmlDebugInfoFilePath);
    util::CodeFormatter formatter(strm);
    formatter.SetIndentSize(1);
    debugInfoXmlDoc->Write(formatter);
}

} // namespace cmajor::debug
