module cmajor.binder.xml.attribute.processor;
// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

import std.core;
import cmajor.ast;
import cmajor.symbols;
import cmajor.binder.bound.compile.unit;
import cmajor.binder.type.binder;
import cmajor.binder.type.resolver;
import cmajor.binder.statement.binder;
import cmajor.binder.bound.function;
import cmajor.binder.bound_class;
import util;

namespace cmajor::binder {


XmlAttributeProcessor::XmlAttributeProcessor() : AttributeProcessor(U"xml")
{
    reservedMemberFunctionNames.insert(U"StaticClassName");
    reservedMemberFunctionNames.insert(U"Register");
    reservedMemberFunctionNames.insert(U"Create");
    reservedMemberFunctionNames.insert(U"DestroyObject");
    reservedMemberFunctionNames.insert(U"ObjectId");
    reservedMemberFunctionNames.insert(U"SetObjectId");
    reservedMemberFunctionNames.insert(U"Container");
    reservedMemberFunctionNames.insert(U"SetContainer");
    reservedMemberFunctionNames.insert(U"ClassId");
    reservedMemberFunctionNames.insert(U"ClassName");
    reservedMemberFunctionNames.insert(U"SetObjectXmlAttributes");
    reservedMemberFunctionNames.insert(U"ToXml");
    reservedMemberFunctionNames.insert(U"FromXml");
    reservedMemberFunctionNames.insert(U"IsOwned");
    reservedMemberFunctionNames.insert(U"SetOwned");
    reservedMemberFunctionNames.insert(U"ResetOwned");
    reservedMemberVariableNames.insert(U"classId");
    reservedMemberVariableNames.insert(U"objectId");
    reservedMemberVariableNames.insert(U"container");
    reservedMemberVariableNames.insert(U"isOwned");
}

void XmlAttributeProcessor::TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol)
{
    switch (symbol->GetSymbolType())
    {
    case cmajor::symbols::SymbolType::classTypeSymbol: case cmajor::symbols::SymbolType::classTemplateSpecializationSymbol: case cmajor::symbols::SymbolType::memberVariableSymbol:
    {
        if (attribute->Value() == U"true" || attribute->Value() == U"false")
        {
            if (attribute->Value() == U"true" && (symbol->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || symbol->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol))
            {
                TypeCheckClass(static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol));
            }
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("unknown attribute value '" + util::ToUtf8(attribute->Value()) + "' for attribute '" + util::ToUtf8(attribute->Name()) + "'", attribute->GetSourcePos(), attribute->ModuleId());
        }
        break;
    }
    }
    AttributeProcessor::TypeCheck(attribute, symbol);
}

void XmlAttributeProcessor::TypeCheckClass(cmajor::symbols::ClassTypeSymbol* classType)
{
    for (cmajor::symbols::MemberFunctionSymbol* memberFunction : classType->MemberFunctions())
    {
        if (reservedMemberFunctionNames.find(memberFunction->GroupName()) != reservedMemberFunctionNames.cend())
        {
            throw cmajor::symbols::Exception("member function name '" + util::ToUtf8(memberFunction->GroupName()) + "' is reserved for the implementation when using the '[xml]' attribute for a class",
                memberFunction->GetSourcePos(), memberFunction->SourceModuleId());
        }
    }
    for (cmajor::symbols::MemberVariableSymbol* memberVariable : classType->StaticMemberVariables())
    {
        if (reservedMemberVariableNames.find(memberVariable->Name()) != reservedMemberVariableNames.cend())
        {
            throw cmajor::symbols::Exception("member variable name '" + util::ToUtf8(memberVariable->Name()) + "' is reserved for the implementation when using the '[xml]' attribute for a class",
                memberVariable->GetSourcePos(), memberVariable->SourceModuleId());
        }
    }
    for (cmajor::symbols::MemberVariableSymbol* memberVariable : classType->MemberVariables())
    {
        if (reservedMemberVariableNames.find(memberVariable->Name()) != reservedMemberVariableNames.cend())
        {
            throw cmajor::symbols::Exception("member variable name '" + util::ToUtf8(memberVariable->Name()) + "' is reserved for the implementation when using the '[xml]' attribute for a class",
                memberVariable->GetSourcePos(), memberVariable->SourceModuleId());
        }
    }
    classType->SetHasXmlAttribute();
}

void XmlAttributeProcessor::GenerateMemberSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classType, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    for (cmajor::symbols::MemberVariableSymbol* memberVariableSymbol : classType->MemberVariables())
    {
        cmajor::ast::AttributesNode* attributes = memberVariableSymbol->GetAttributes();
        if (attributes)
        {
            cmajor::ast::AttributeNode* xmlAttribute = attributes->GetAttribute(U"xml");
            if (xmlAttribute)
            {
                if (xmlAttribute->Value() == U"false")
                {
                    continue;
                }
            }
        }
        if (reservedMemberVariableNames.find(memberVariableSymbol->Name()) != reservedMemberVariableNames.cend())
        {
            continue;
        }
        cmajor::symbols::TypeSymbol* memberVariableType = memberVariableSymbol->GetType();
        if (memberVariableType->IsClassTypeSymbol())
        {
            cmajor::symbols::ClassTypeSymbol* memberVarClassType = static_cast<cmajor::symbols::ClassTypeSymbol*>(memberVariableType);
            cmajor::ast::Node* node = boundCompileUnit.GetSymbolTable().GetNodeNoThrow(memberVarClassType);
            if (node)
            {
                BindClass(memberVarClassType, &boundCompileUnit);
            }
        }
    }
}

const int destructorId = 0;
const int staticClassNameId = 1;
const int createId = 2;
const int registerId = 3;
const int destroyObjectId = 4;
const int objectIdId = 5;
const int setObjectIdId = 6;
const int containerId = 7;
const int setContainerId = 8;
const int classIdId = 9;
const int classNameId = 10;
const int setObjectXmlAttributesId = 11;
const int toXmlPlainId = 12;
const int toXmlId = 13;
const int fromXmlId = 14;
const int getPtrsId = 15;
const int isOwnedMemFunId = 16;
const int setOwnedMemFunId = 17;
const int resetOwnedMemFunId = 18;

void XmlAttributeProcessor::GenerateSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    if (symbol->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol);
        if (attribute->Value() == U"true")
        {
            CheckXmlSerializableInterface(classTypeSymbol, boundCompileUnit, containerScope);
            if (!classTypeSymbol->BaseClass())
            {
                CheckVirtualDestructor(classTypeSymbol);
            }
            GenerateMemberSymbols(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateMemberVariableSymbols(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateStaticClassNameSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateCreateFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateRegisterFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateDestroyObjectFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateObjectIdFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateSetObjectIdFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateContainerFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateSetContainerFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateClassIdFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateClassNameFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateSetObjectXmlAttributesSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateToXmlPlainSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateToXmlSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateFromXmlSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateGetPtrsSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateIsOwnedSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateSetOwnedSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateResetOwnedSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
        }
    }
}

void XmlAttributeProcessor::CheckXmlSerializableInterface(cmajor::symbols::ClassTypeSymbol* classType, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    const std::vector<cmajor::symbols::InterfaceTypeSymbol*>& interfaces = classType->ImplementedInterfaces();
    bool xmlSerializableInterfaceFound = false;
    for (cmajor::symbols::InterfaceTypeSymbol* intf : interfaces)
    {
        if (intf->FullName() == U"System.Xml.Serialization.XmlSerializable")
        {
            xmlSerializableInterfaceFound = true;
            break;
        }
    }
    if (!xmlSerializableInterfaceFound)
    {
        cmajor::ast::IdentifierNode xmlSerializableInterfaceNode(classType->GetSourcePos(), classType->SourceModuleId(), U"System.Xml.Serialization.XmlSerializable");
        cmajor::symbols::TypeSymbol* xmlSerializableType = ResolveType(&xmlSerializableInterfaceNode, boundCompileUnit, containerScope);
        if (xmlSerializableType->GetSymbolType() == cmajor::symbols::SymbolType::interfaceTypeSymbol)
        {
            cmajor::symbols::InterfaceTypeSymbol* xmlSerialiableInterfaceType = static_cast<cmajor::symbols::InterfaceTypeSymbol*>(xmlSerializableType);
            classType->AddImplementedInterface(xmlSerialiableInterfaceType);
        }
    }

}

void XmlAttributeProcessor::CheckVirtualDestructor(cmajor::symbols::ClassTypeSymbol* classTypeSymbol)
{
    if (classTypeSymbol->Destructor())
    {
        if (!classTypeSymbol->Destructor()->IsVirtual())
        {
            throw cmajor::symbols::Exception("destructor of an XML class '" + util::ToUtf8(classTypeSymbol->Name()) + "' is not virtual",
                classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
        }
    }
    else
    {
        classTypeSymbol->CreateDestructorSymbol();
        classTypeSymbol->Destructor()->SetVirtual();
        auto& m = functionSymbolMap[classTypeSymbol];
        m.push_back(std::make_pair(classTypeSymbol->Destructor(), destructorId));
    }
}

void XmlAttributeProcessor::GenerateImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, StatementBinder* statementBinder)
{
    if (symbol->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol);
        auto  it = functionSymbolMap.find(symbol);
        if (it != functionSymbolMap.cend())
        {
            const auto& m = it->second;
            if (m.empty())
            {
                throw cmajor::symbols::Exception("internal error in XML attribute implementation: member function symbol map for class type symbol '" + util::ToUtf8(classTypeSymbol->FullName()) + "' is empty",
                    classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
            }
            for (const std::pair<cmajor::symbols::FunctionSymbol*, int>& p : m)
            {
                cmajor::symbols::FunctionSymbol* functionSymbol = p.first;
                switch (p.second)
                {
                case destructorId:
                {
                    GenerateDestructorImplementation(static_cast<cmajor::symbols::DestructorSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case staticClassNameId:
                {
                    GenerateStaticClassNameImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case createId:
                {
                    GenerateCreateImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case registerId:
                {
                    GenerateRegisterImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case destroyObjectId:
                {
                    GenerateDestroyObjectImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case objectIdId:
                {
                    GenerateObjectIdImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case setObjectIdId:
                {
                    GenerateSetObjectIdImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case containerId:
                {
                    GenerateContainerImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case setContainerId:
                {
                    GenerateSetContainerImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case classIdId:
                {
                    GenerateClassIdImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case classNameId:
                {
                    GenerateClassNameImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case setObjectXmlAttributesId:
                {
                    GenerateSetObjectXmlAttributesImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case toXmlPlainId:
                {
                    GenerateToXmlPlainImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case toXmlId:
                {
                    GenerateToXmlImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case fromXmlId:
                {
                    GenerateFromXmlImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case getPtrsId:
                {
                    GenerateGetPtrsImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case isOwnedMemFunId:
                {
                    GenerateIsOwnedImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case setOwnedMemFunId:
                {
                    GenerateSetOwnedImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                case resetOwnedMemFunId:
                {
                    GenerateSetOwnedImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                    break;
                }
                default:
                {
                    throw cmajor::symbols::Exception("internal error in XML attribute implementation: member function symbol map for class type symbol '" + util::ToUtf8(classTypeSymbol->FullName()) +
                        "' contains invalid member function id " + std::to_string(p.second), classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
                }
                }
            }
        }
        else
        {
            throw cmajor::symbols::Exception("internal error in XML attribute implementation: member function symbol map for class type symbol '" + util::ToUtf8(classTypeSymbol->FullName()) + "' not found",
                classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
        }
    }
}

void XmlAttributeProcessor::GenerateDestructorImplementation(cmajor::symbols::DestructorSymbol* destructorSymbol, StatementBinder* statementBinder)
{
    BoundCompileUnit& boundCompileUnit = statementBinder->GetBoundCompileUnit();
    if (!boundCompileUnit.IsGeneratedDestructorInstantiated(destructorSymbol))
    {
        boundCompileUnit.SetGeneratedDestructorInstantiated(destructorSymbol);
        cmajor::binder::GenerateDestructorImplementation(statementBinder->CurrentClass(), destructorSymbol, boundCompileUnit, statementBinder->GetContainerScope(),
            statementBinder->CurrentFunction(), destructorSymbol->GetSourcePos(), destructorSymbol->SourceModuleId());
    }
}

void XmlAttributeProcessor::GenerateMemberVariableSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classType, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberVariableSymbol* classIdSymbol = new cmajor::symbols::MemberVariableSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"classId");
    classIdSymbol->SetModule(&boundCompileUnit.GetModule());
    classIdSymbol->SetStatic();
    classIdSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    classIdSymbol->SetType(symbolTable.GetTypeByName(U"int"));
    classType->AddMember(classIdSymbol);

    if (!HasXmlBaseClass(classType))
    {
        cmajor::symbols::MemberVariableSymbol* objectIdSymbol = new cmajor::symbols::MemberVariableSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"objectId");
        objectIdSymbol->SetModule(&boundCompileUnit.GetModule());
        objectIdSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
        objectIdSymbol->SetType(symbolTable.GetTypeByName(U"System.Uuid"));
        classType->AddMember(objectIdSymbol);

        cmajor::symbols::MemberVariableSymbol* containerSymbol = new cmajor::symbols::MemberVariableSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"container");
        containerSymbol->SetModule(&boundCompileUnit.GetModule());
        containerSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
        containerSymbol->SetType(symbolTable.GetTypeByName(U"System.Xml.Serialization.XmlContainer")->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
        classType->AddMember(containerSymbol);

        cmajor::symbols::MemberVariableSymbol* isOwnedSymbol = new cmajor::symbols::MemberVariableSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"isOwned");
        isOwnedSymbol->SetModule(&boundCompileUnit.GetModule());
        isOwnedSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
        isOwnedSymbol->SetType(symbolTable.GetTypeByName(U"bool"));
        classType->AddMember(isOwnedSymbol);
    }
}

void XmlAttributeProcessor::GenerateStaticClassNameSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    try
    {
        cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
        cmajor::symbols::MemberFunctionSymbol* staticClassNameSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"StaticClassName");
        staticClassNameSymbol->SetModule(&boundCompileUnit.GetModule());
        staticClassNameSymbol->SetGroupName(U"StaticClassName");
        staticClassNameSymbol->SetStatic();
        staticClassNameSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);

        cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(staticClassNameSymbol);
        cmajor::symbols::TypeSymbol* stringTypeSymbol = symbolTable.GetTypeByName(U"String<char>");
        staticClassNameSymbol->SetReturnType(stringTypeSymbol);

        cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"@return");
        returnParam->SetParent(staticClassNameSymbol);
        returnParam->SetType(stringTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
        staticClassNameSymbol->SetReturnParam(returnParam);
        classTypeSymbol->AddMember(staticClassNameSymbol);
        staticClassNameSymbol->ComputeName();

        auto& m = functionSymbolMap[classTypeSymbol];
        m.push_back(std::make_pair(staticClassNameSymbol, staticClassNameId));
    }
    catch (const std::exception& ex)
    {
        throw cmajor::symbols::Exception("generation of 'StaticClassName' member function symbol failed: " + std::string(ex.what()), classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
    }
}

void XmlAttributeProcessor::GenerateStaticClassNameImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* staticClassNameSymbol,
    StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), staticClassNameSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StringLiteralNode* classNameLiteralNode = new cmajor::ast::StringLiteralNode(sourcePos, moduleId, util::ToUtf8(classTypeSymbol->FullName()));
        cmajor::ast::ReturnStatementNode* returnStatementNode(new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, classNameLiteralNode));
        compoundStatementNode.AddStatement(returnStatementNode);

        CompileMemberFunction(staticClassNameSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'StaticClassName' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateCreateFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* createSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"Create");
    createSymbol->SetModule(&boundCompileUnit.GetModule());
    createSymbol->SetGroupName(U"Create");
    createSymbol->SetStatic();
    createSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(createSymbol);

    cmajor::symbols::TypeSymbol* voidPtrTypeSymbol = symbolTable.GetTypeByName(U"void")->AddPointer(attribute->GetSourcePos(), attribute->ModuleId());
    createSymbol->SetReturnType(voidPtrTypeSymbol);

    classTypeSymbol->AddMember(createSymbol);
    createSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(createSymbol, createId));
}

void XmlAttributeProcessor::GenerateCreateImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* createFunctionSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), createFunctionSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::IdentifierNode* classNode = new cmajor::ast::IdentifierNode(sourcePos, moduleId, classTypeSymbol->FullName());
        cmajor::ast::NewNode* newNode = new cmajor::ast::NewNode(sourcePos, moduleId, classNode);
        cmajor::ast::ReturnStatementNode* returnStatementNode(new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, newNode));
        compoundStatementNode.AddStatement(returnStatementNode);

        CompileMemberFunction(createFunctionSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'Create' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateRegisterFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* registerSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"Register");
    registerSymbol->SetModule(&boundCompileUnit.GetModule());
    registerSymbol->SetGroupName(U"Register");
    registerSymbol->SetStatic();
    registerSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(registerSymbol);

    cmajor::symbols::TypeSymbol* voidTypeSymbol = symbolTable.GetTypeByName(U"void");
    registerSymbol->SetReturnType(voidTypeSymbol);

    cmajor::symbols::ParameterSymbol* classIdParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"classId_");
    cmajor::symbols::TypeSymbol* intTypeSymbol = symbolTable.GetTypeByName(U"int");
    classIdParam->SetType(intTypeSymbol);
    registerSymbol->AddMember(classIdParam);
    classTypeSymbol->AddMember(registerSymbol);
    registerSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(registerSymbol, registerId));
}

void XmlAttributeProcessor::GenerateRegisterImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* registerSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), registerSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::AssignmentStatementNode* assignClassIdStatement = new cmajor::ast::AssignmentStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"classId"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"classId_"));
        compoundStatementNode.AddStatement(assignClassIdStatement);

        cmajor::ast::TemplateIdNode* xmlRegister = new cmajor::ast::TemplateIdNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Xml.Serialization.XmlRegister"));
        xmlRegister->AddTemplateArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, classTypeSymbol->FullName()));
        cmajor::ast::InvokeNode* invokeXmlRegisterFunction = new cmajor::ast::InvokeNode(sourcePos, moduleId, xmlRegister);
        invokeXmlRegisterFunction->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"classId"));
        cmajor::ast::ExpressionStatementNode* invokeXmlRegisterStatement = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, invokeXmlRegisterFunction);
        compoundStatementNode.AddStatement(invokeXmlRegisterStatement);

        CompileMemberFunction(registerSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'Register' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateDestroyObjectFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* destroyObjectSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"DestroyObject");
    destroyObjectSymbol->SetModule(&boundCompileUnit.GetModule());
    destroyObjectSymbol->SetGroupName(U"DestroyObject");
    destroyObjectSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        destroyObjectSymbol->SetOverride();
    }
    else
    {
        destroyObjectSymbol->SetVirtual();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    destroyObjectSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(destroyObjectSymbol);

    cmajor::symbols::TypeSymbol* voidTypeSymbol = symbolTable.GetTypeByName(U"void");
    destroyObjectSymbol->SetReturnType(voidTypeSymbol);

    classTypeSymbol->AddMember(destroyObjectSymbol);
    destroyObjectSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(destroyObjectSymbol, destroyObjectId));
}

void XmlAttributeProcessor::GenerateDestroyObjectImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* destroyObjectSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), destroyObjectSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"DestroyObject"));
            cmajor::ast::InvokeNode* baseDestroyObjectCall = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            statementNode = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, baseDestroyObjectCall);
        }
        else
        {
            statementNode = new cmajor::ast::DeleteStatementNode(sourcePos, moduleId, new cmajor::ast::ThisNode(sourcePos, moduleId));
        }
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(destroyObjectSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'DestroyObject' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateObjectIdFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* objectIdSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"ObjectId");
    objectIdSymbol->SetModule(&boundCompileUnit.GetModule());
    objectIdSymbol->SetGroupName(U"ObjectId");
    objectIdSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        objectIdSymbol->SetOverride();
    }
    else
    {
        objectIdSymbol->SetVirtual();
    }
    objectIdSymbol->SetConst();
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    objectIdSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(objectIdSymbol);

    cmajor::symbols::TypeSymbol* uuidTypeSymbol = symbolTable.GetTypeByName(U"System.Uuid");
    objectIdSymbol->SetReturnType(uuidTypeSymbol);

    cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"@return");
    returnParam->SetParent(objectIdSymbol);
    returnParam->SetType(uuidTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    objectIdSymbol->SetReturnParam(returnParam);

    classTypeSymbol->AddMember(objectIdSymbol);
    objectIdSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(objectIdSymbol, objectIdId));
}

void XmlAttributeProcessor::GenerateObjectIdImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* objectIdSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), objectIdSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ObjectId"));
            cmajor::ast::InvokeNode* baseObjectIdCall = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            statementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, baseObjectIdCall);
        }
        else
        {
            cmajor::ast::ReturnStatementNode* returnStatementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"objectId"));
            statementNode = returnStatementNode;
        }
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(objectIdSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'ObjectId' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateSetObjectIdFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* setObjectIdSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"SetObjectId");
    setObjectIdSymbol->SetModule(&boundCompileUnit.GetModule());
    setObjectIdSymbol->SetGroupName(U"SetObjectId");
    setObjectIdSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        setObjectIdSymbol->SetOverride();
    }
    else
    {
        setObjectIdSymbol->SetVirtual();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    setObjectIdSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(setObjectIdSymbol);

    cmajor::symbols::TypeSymbol* uuidTypeSymbol = symbolTable.GetTypeByName(U"System.Uuid");
    cmajor::symbols::ParameterSymbol* objectIdParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"objectId_");
    objectIdParam->SetType(uuidTypeSymbol->AddConst(attribute->GetSourcePos(), attribute->ModuleId())->AddLvalueReference(attribute->GetSourcePos(), attribute->ModuleId()));
    setObjectIdSymbol->AddMember(objectIdParam);

    setObjectIdSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));

    classTypeSymbol->AddMember(setObjectIdSymbol);
    setObjectIdSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(setObjectIdSymbol, setObjectIdId));
}

bool XmlAttributeProcessor::HasXmlBaseClass(cmajor::symbols::ClassTypeSymbol* classType) const
{
    cmajor::symbols::ClassTypeSymbol* baseClass = classType->BaseClass();
    if (baseClass)
    {
        return baseClass->HasXmlAttribute();
    }
    return false;
}

void XmlAttributeProcessor::GenerateSetObjectIdImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* setObjectIdSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), setObjectIdSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"SetObjectId"));
            cmajor::ast::InvokeNode* baseObjectIdCall = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            baseObjectIdCall->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"objectId_"));
            statementNode = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, baseObjectIdCall);
        }
        else
        {
            cmajor::ast::AssignmentStatementNode* assignmentStatementNode = new cmajor::ast::AssignmentStatementNode(sourcePos, moduleId,
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"objectId"),
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"objectId_"));
            statementNode = assignmentStatementNode;
        }
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(setObjectIdSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'SetObjectId' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateContainerFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* containerSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"Container");
    containerSymbol->SetModule(&boundCompileUnit.GetModule());
    containerSymbol->SetGroupName(U"Container");
    containerSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        containerSymbol->SetOverride();
    }
    else
    {
        containerSymbol->SetVirtual();
    }
    containerSymbol->SetConst();
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    containerSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(containerSymbol);

    cmajor::symbols::TypeSymbol* containerTypeSymbol = symbolTable.GetTypeByName(U"System.Xml.Serialization.XmlContainer")->AddPointer(attribute->GetSourcePos(), attribute->ModuleId());
    containerSymbol->SetReturnType(containerTypeSymbol);

    classTypeSymbol->AddMember(containerSymbol);
    containerSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(containerSymbol, containerId));
}

void XmlAttributeProcessor::GenerateContainerImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* containerSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), containerSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Container"));
            cmajor::ast::InvokeNode* baseObjectIdCall = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            statementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, baseObjectIdCall);
        }
        else
        {
            cmajor::ast::ReturnStatementNode* returnStatementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"container"));
            statementNode = returnStatementNode;
        }
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(containerSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'Container' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateSetContainerFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* setContainerSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"SetContainer");
    setContainerSymbol->SetModule(&boundCompileUnit.GetModule());
    setContainerSymbol->SetGroupName(U"SetContainer");
    setContainerSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        setContainerSymbol->SetOverride();
    }
    else
    {
        setContainerSymbol->SetVirtual();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    setContainerSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(setContainerSymbol);

    cmajor::symbols::TypeSymbol* containerTypeSymbol = symbolTable.GetTypeByName(U"System.Xml.Serialization.XmlContainer");
    cmajor::symbols::ParameterSymbol* containerParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"container_");
    containerParam->SetType(containerTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    setContainerSymbol->AddMember(containerParam);

    setContainerSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));

    classTypeSymbol->AddMember(setContainerSymbol);
    setContainerSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(setContainerSymbol, setContainerId));
}

void XmlAttributeProcessor::GenerateSetContainerImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* setContainerSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), setContainerSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"SetContainer"));
            cmajor::ast::InvokeNode* baseContainerCall = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            baseContainerCall->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"container_"));
            statementNode = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, baseContainerCall);
        }
        else
        {
            cmajor::ast::AssignmentStatementNode* assignmentStatementNode = new cmajor::ast::AssignmentStatementNode(sourcePos, moduleId,
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"container"),
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"container_"));
            statementNode = assignmentStatementNode;
        }
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(setContainerSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'SetContainer' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateClassIdFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* classIdSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"ClassId");
    classIdSymbol->SetModule(&boundCompileUnit.GetModule());
    classIdSymbol->SetGroupName(U"ClassId");
    classIdSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    classIdSymbol->SetConst();
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    classIdSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(classIdSymbol);

    classIdSymbol->SetReturnType(symbolTable.GetTypeByName(U"int"));

    classTypeSymbol->AddMember(classIdSymbol);
    classIdSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(classIdSymbol, classIdId));
}

void XmlAttributeProcessor::GenerateClassIdImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* classIdSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), classIdSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        cmajor::ast::ReturnStatementNode* returnStatementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"classId"));
        statementNode = returnStatementNode;
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(classIdSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'ClassId' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateClassNameFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* classNameSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"ClassName");
    classNameSymbol->SetModule(&boundCompileUnit.GetModule());
    classNameSymbol->SetGroupName(U"ClassName");
    classNameSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    classNameSymbol->SetConst();
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    classNameSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(classNameSymbol);

    cmajor::symbols::TypeSymbol* stringTypeSymbol = symbolTable.GetTypeByName(U"String<char>");
    classNameSymbol->SetReturnType(stringTypeSymbol);

    cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"@return");
    returnParam->SetParent(classNameSymbol);
    returnParam->SetType(stringTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    classNameSymbol->SetReturnParam(returnParam);
    classTypeSymbol->AddMember(classNameSymbol);
    classNameSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(classNameSymbol, classNameId));
}

void XmlAttributeProcessor::GenerateClassNameImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* classNameSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), classNameSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        cmajor::ast::TypeNameNode* typeNameNode = new cmajor::ast::TypeNameNode(sourcePos, moduleId, new cmajor::ast::DerefNode(sourcePos, moduleId, new cmajor::ast::ThisNode(sourcePos, moduleId)));
        cmajor::ast::ReturnStatementNode* returnStatementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, typeNameNode);
        statementNode = returnStatementNode;
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(classNameSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'ClassName' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateSetObjectXmlAttributesSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* setObjectXmlAttributesSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"SetObjectXmlAttributes");
    setObjectXmlAttributesSymbol->SetModule(&boundCompileUnit.GetModule());
    setObjectXmlAttributesSymbol->SetGroupName(U"SetObjectXmlAttributes");
    setObjectXmlAttributesSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        setObjectXmlAttributesSymbol->SetOverride();
    }
    else
    {
        setObjectXmlAttributesSymbol->SetVirtual();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    setObjectXmlAttributesSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(setObjectXmlAttributesSymbol);

    cmajor::symbols::TypeSymbol* domElementTypeSymbol = symbolTable.GetTypeByName(U"System.Dom.Element");
    cmajor::symbols::ParameterSymbol* elementParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"element");
    elementParam->SetType(domElementTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    setObjectXmlAttributesSymbol->AddMember(elementParam);

    setObjectXmlAttributesSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));

    classTypeSymbol->AddMember(setObjectXmlAttributesSymbol);
    setObjectXmlAttributesSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(setObjectXmlAttributesSymbol, setObjectXmlAttributesId));
}

void XmlAttributeProcessor::GenerateSetObjectXmlAttributesImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* setObjectXmlAttributesSymbol,
    StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), setObjectXmlAttributesSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::ArrowNode* setClassIdArrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"SetAttribute"));
        cmajor::ast::InvokeNode* invokeSetClassIdAttribute = new cmajor::ast::InvokeNode(sourcePos, moduleId, setClassIdArrowNode);
        cmajor::ast::UStringLiteralNode* classIdLiteralArg = new cmajor::ast::UStringLiteralNode(sourcePos, moduleId, U"classId");
        invokeSetClassIdAttribute->AddArgument(classIdLiteralArg);
        cmajor::ast::InvokeNode* invokeClassIdToStringFun = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToString"));
        invokeClassIdToStringFun->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"classId"));
        cmajor::ast::InvokeNode* invokeClassIdToUtf32Fun = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToUtf32"));
        invokeClassIdToUtf32Fun->AddArgument(invokeClassIdToStringFun);
        invokeSetClassIdAttribute->AddArgument(invokeClassIdToUtf32Fun);
        cmajor::ast::ExpressionStatementNode* setClassIdAttributeStatementNode = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, invokeSetClassIdAttribute);
        compoundStatementNode.AddStatement(setClassIdAttributeStatementNode);

        cmajor::ast::ArrowNode* setObjectIdArrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"SetAttribute"));
        cmajor::ast::InvokeNode* invokeSetObjectIdAttribute = new cmajor::ast::InvokeNode(sourcePos, moduleId, setObjectIdArrowNode);
        cmajor::ast::UStringLiteralNode* objectIdLiteralArg = new cmajor::ast::UStringLiteralNode(sourcePos, moduleId, U"objectId");
        invokeSetObjectIdAttribute->AddArgument(objectIdLiteralArg);
        cmajor::ast::InvokeNode* invokeObjectIdToStringFun = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToString"));
        invokeObjectIdToStringFun->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ObjectId")));
        cmajor::ast::InvokeNode* invokeObjectIdToUtf32Fun = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToUtf32"));
        invokeObjectIdToUtf32Fun->AddArgument(invokeObjectIdToStringFun);
        invokeSetObjectIdAttribute->AddArgument(invokeObjectIdToUtf32Fun);
        cmajor::ast::ExpressionStatementNode* setObjectIdAttributeStatementNode = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, invokeSetObjectIdAttribute);
        compoundStatementNode.AddStatement(setObjectIdAttributeStatementNode);

        cmajor::ast::ArrowNode* setClassNameArrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"SetAttribute"));
        cmajor::ast::InvokeNode* invokeSetClassNameAttribute = new cmajor::ast::InvokeNode(sourcePos, moduleId, setClassNameArrowNode);
        cmajor::ast::UStringLiteralNode* classNameLiteralArg = new cmajor::ast::UStringLiteralNode(sourcePos, moduleId, U"className");
        invokeSetClassNameAttribute->AddArgument(classNameLiteralArg);
        cmajor::ast::InvokeNode* invokeClassNameToUtf32Fun = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToUtf32"));
        invokeClassNameToUtf32Fun->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ClassName")));
        invokeSetClassNameAttribute->AddArgument(invokeClassNameToUtf32Fun);
        cmajor::ast::ExpressionStatementNode* setClassNameAttributeStatementNode = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, invokeSetClassNameAttribute);
        compoundStatementNode.AddStatement(setClassNameAttributeStatementNode);

        CompileMemberFunction(setObjectXmlAttributesSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'SetObjectXmlAttributes' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateToXmlPlainSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* toXmlSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"ToXml");
    toXmlSymbol->SetModule(&boundCompileUnit.GetModule());
    toXmlSymbol->SetGroupName(U"ToXml");
    toXmlSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);

    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    toXmlSymbol->AddMember(thisParam);

    cmajor::symbols::ParameterSymbol* elementNameParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"elementName");
    elementNameParam->SetType(symbolTable.GetTypeByName(U"String<char>")->AddConst(attribute->GetSourcePos(), attribute->ModuleId())->AddLvalueReference(attribute->GetSourcePos(), attribute->ModuleId()));
    toXmlSymbol->AddMember(elementNameParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(toXmlSymbol);

    cmajor::symbols::TypeSymbol* elementTypeSymbol = symbolTable.GetTypeByName(U"System.Dom.Element")->AddPointer(attribute->GetSourcePos(), attribute->ModuleId());
    toXmlSymbol->SetReturnType(elementTypeSymbol);

    classTypeSymbol->AddMember(toXmlSymbol);
    toXmlSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(toXmlSymbol, toXmlPlainId));
}

void XmlAttributeProcessor::GenerateToXmlPlainImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* toXmlSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), toXmlSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::ConstructionStatementNode* constructionStatementNode = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId,
            new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Xml.Serialization.XmlSerializationContext"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ctx"));
        compoundStatementNode.AddStatement(constructionStatementNode);

        cmajor::ast::InvokeNode* invokeToXmlNode = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToXml"));
        invokeToXmlNode->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"elementName"));
        invokeToXmlNode->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ctx"));
        cmajor::ast::ReturnStatementNode* returnStatementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, invokeToXmlNode);
        compoundStatementNode.AddStatement(returnStatementNode);

        CompileMemberFunction(toXmlSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'ToXml' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateToXmlSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* toXmlSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"ToXml");
    toXmlSymbol->SetModule(&boundCompileUnit.GetModule());
    toXmlSymbol->SetGroupName(U"ToXml");
    toXmlSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        toXmlSymbol->SetOverride();
    }
    else
    {
        toXmlSymbol->SetVirtual();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    toXmlSymbol->AddMember(thisParam);

    cmajor::symbols::ParameterSymbol* elementNameParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"elementName");
    elementNameParam->SetType(symbolTable.GetTypeByName(U"String<char>")->AddConst(attribute->GetSourcePos(), attribute->ModuleId())->AddLvalueReference(attribute->GetSourcePos(), attribute->ModuleId()));
    toXmlSymbol->AddMember(elementNameParam);

    cmajor::symbols::ParameterSymbol* ctxParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"ctx");
    ctxParam->SetType(symbolTable.GetTypeByName(U"System.Xml.Serialization.XmlSerializationContext")->AddLvalueReference(attribute->GetSourcePos(), attribute->ModuleId()));
    toXmlSymbol->AddMember(ctxParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(toXmlSymbol);

    cmajor::symbols::TypeSymbol* elementTypeSymbol = symbolTable.GetTypeByName(U"System.Dom.Element")->AddPointer(attribute->GetSourcePos(), attribute->ModuleId());
    toXmlSymbol->SetReturnType(elementTypeSymbol);

    classTypeSymbol->AddMember(toXmlSymbol);
    toXmlSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(toXmlSymbol, toXmlId));
}

void XmlAttributeProcessor::GenerateToXmlImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* toXmlSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), toXmlSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::ConstructionStatementNode* constructionStatementNode = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId,
                new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Dom.Element")),
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"));
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToXml"));
            cmajor::ast::InvokeNode* invokeNode = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            invokeNode->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"elementName"));
            invokeNode->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ctx"));
            constructionStatementNode->AddArgument(invokeNode);
            constructionStatementNode->SetAssignment();
            compoundStatementNode.AddStatement(constructionStatementNode);
        }
        else
        {
            cmajor::ast::ConstructionStatementNode* constructionStatementNode = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId,
                new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Dom.Element")),
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"));
            cmajor::ast::NewNode* newElementNode = new cmajor::ast::NewNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Dom.Element"));
            cmajor::ast::InvokeNode* toUtf32Node = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToUtf32"));
            toUtf32Node->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"elementName"));
            newElementNode->AddArgument(toUtf32Node);
            constructionStatementNode->AddArgument(newElementNode);
            constructionStatementNode->SetAssignment();
            compoundStatementNode.AddStatement(constructionStatementNode);

            //if (!ctx.GetFlag(sngxml.xmlser.XmlSerializationFlags.suppressMetadata)

            cmajor::ast::DotNode* dotNode = new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ctx"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"GetFlag"));
            cmajor::ast::InvokeNode* invokeNode = new cmajor::ast::InvokeNode(sourcePos, moduleId, dotNode);
            invokeNode->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.XmlSerialization.XmlSerializationFlags.suppressMetadata"));
            cmajor::ast::NotNode* cond = new cmajor::ast::NotNode(sourcePos, moduleId, invokeNode);

            cmajor::ast::InvokeNode* invokeSetObjectXmlAttributesNode = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"SetObjectXmlAttributes"));
            invokeSetObjectXmlAttributesNode->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"));
            cmajor::ast::ExpressionStatementNode* expressionStatement = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, invokeSetObjectXmlAttributesNode);
            cmajor::ast::IfStatementNode* ifStatementNode = new cmajor::ast::IfStatementNode(sourcePos, moduleId, cond, expressionStatement, nullptr);

            compoundStatementNode.AddStatement(ifStatementNode);
        }

        for (cmajor::symbols::MemberVariableSymbol* memberVariableSymbol : classTypeSymbol->MemberVariables())
        {
            cmajor::ast::AttributesNode* attributes = memberVariableSymbol->GetAttributes();
            if (attributes)
            {
                cmajor::ast::AttributeNode* xmlAttribute = attributes->GetAttribute(U"xml");
                if (xmlAttribute)
                {
                    if (xmlAttribute->Value() == U"false")
                    {
                        continue;
                    }
                }
            }
            if (reservedMemberVariableNames.find(memberVariableSymbol->Name()) != reservedMemberVariableNames.cend())
            {
                continue;
            }
            cmajor::ast::DotNode* toXmlDotNode = new cmajor::ast::DotNode(sourcePos, moduleId,
                new cmajor::ast::DotNode(sourcePos, moduleId,
                    new cmajor::ast::DotNode(sourcePos, moduleId,
                        new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System"),
                        new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Xml")),
                    new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Serialization")),
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToXml"));
            cmajor::ast::InvokeNode* toXmlInvokeNode = new cmajor::ast::InvokeNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), toXmlDotNode);
            toXmlInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), memberVariableSymbol->Name()));
            toXmlInvokeNode->AddArgument(new cmajor::ast::StringLiteralNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), util::ToUtf8(memberVariableSymbol->Name())));
            toXmlInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), U"ctx"));
            cmajor::ast::DotNode* dotNode = new cmajor::ast::DotNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), toXmlInvokeNode, new cmajor::ast::IdentifierNode(
                memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), U"Release"));
            cmajor::ast::InvokeNode* dotInvokeNode = new cmajor::ast::InvokeNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), dotNode);
            cmajor::ast::TemplateIdNode* uniquePtrNode = new cmajor::ast::TemplateIdNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(),
                new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), U"System.UniquePtr"));
            uniquePtrNode->AddTemplateArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(),
                U"System.Dom.cmajor::ast::Node"));
            cmajor::ast::InvokeNode* uniquePtrNodeInvokeNode = new cmajor::ast::InvokeNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), uniquePtrNode);
            uniquePtrNodeInvokeNode->AddArgument(dotInvokeNode);
            cmajor::ast::ArrowNode* appendChildArrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"AppendChild"));
            cmajor::ast::InvokeNode* appendChildInvokeNode = new cmajor::ast::InvokeNode(sourcePos, moduleId, appendChildArrowNode);
            appendChildInvokeNode->AddArgument(uniquePtrNodeInvokeNode);
            cmajor::ast::ExpressionStatementNode* appendChildStatement = new cmajor::ast::ExpressionStatementNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), appendChildInvokeNode);
            compoundStatementNode.AddStatement(appendChildStatement);
        }

        cmajor::ast::ReturnStatementNode* returnStatement = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"));
        compoundStatementNode.AddStatement(returnStatement);

        CompileMemberFunction(toXmlSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'ToXml' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateFromXmlSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* fromXmlSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"FromXml");
    fromXmlSymbol->SetModule(&boundCompileUnit.GetModule());
    fromXmlSymbol->SetGroupName(U"FromXml");
    fromXmlSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        fromXmlSymbol->SetOverride();
    }
    else
    {
        fromXmlSymbol->SetVirtual();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    fromXmlSymbol->AddMember(thisParam);

    cmajor::symbols::ParameterSymbol* elementParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"element");
    elementParam->SetType(symbolTable.GetTypeByName(U"System.Dom.Element")->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    fromXmlSymbol->AddMember(elementParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(fromXmlSymbol);

    fromXmlSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));

    classTypeSymbol->AddMember(fromXmlSymbol);
    fromXmlSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(fromXmlSymbol, fromXmlId));
}

void XmlAttributeProcessor::GenerateFromXmlImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* fromXmlSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), fromXmlSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::InvokeNode* invokeBaseFromXml = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"FromXml")));
            invokeBaseFromXml->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"));
            cmajor::ast::ExpressionStatementNode* baseFromXmlStatement = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, invokeBaseFromXml);
            compoundStatementNode.AddStatement(baseFromXmlStatement);
        }
        else
        {
            cmajor::ast::ConstructionStatementNode* constructionStatementNode = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId,
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.ustring"),
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"objectIdAttr"));
            constructionStatementNode->SetAssignment();
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"element"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"GetAttribute"));
            cmajor::ast::UStringLiteralNode* objectIdLiteral = new cmajor::ast::UStringLiteralNode(sourcePos, moduleId, U"objectId");
            cmajor::ast::InvokeNode* elementGetAttributeNode = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            elementGetAttributeNode->AddArgument(objectIdLiteral);
            constructionStatementNode->AddArgument(elementGetAttributeNode);
            constructionStatementNode->SetAssignment();
            compoundStatementNode.AddStatement(constructionStatementNode);

            cmajor::ast::InvokeNode* invokeParseUuid = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ParseUuid"));
            cmajor::ast::InvokeNode* toUtf8Node = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToUtf8"));
            toUtf8Node->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"objectIdAttr"));
            invokeParseUuid->AddArgument(toUtf8Node);
            cmajor::ast::AssignmentStatementNode* assignObjectIdStatement = new cmajor::ast::AssignmentStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"objectId"), invokeParseUuid);
            cmajor::ast::DotNode* dotNode = new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"objectIdAttr"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"IsEmpty"));
            cmajor::ast::NotNode* notObjectIdAttrIsEmpty = new cmajor::ast::NotNode(sourcePos, moduleId, new cmajor::ast::InvokeNode(sourcePos, moduleId, dotNode));
            cmajor::ast::IfStatementNode* ifStatementNode = new cmajor::ast::IfStatementNode(sourcePos, moduleId, notObjectIdAttrIsEmpty, assignObjectIdStatement, nullptr);
            compoundStatementNode.AddStatement(ifStatementNode);
        }

        for (cmajor::symbols::MemberVariableSymbol* memberVariableSymbol : classTypeSymbol->MemberVariables())
        {
            cmajor::ast::AttributesNode* attributes = memberVariableSymbol->GetAttributes();
            if (attributes)
            {
                cmajor::ast::AttributeNode* xmlAttribute = attributes->GetAttribute(U"xml");
                if (xmlAttribute)
                {
                    if (xmlAttribute->Value() == U"false")
                    {
                        continue;
                    }
                }
            }
            if (reservedMemberVariableNames.find(memberVariableSymbol->Name()) != reservedMemberVariableNames.cend())
            {
                continue;
            }
            cmajor::ast::DotNode* fromXmlDotNode = new cmajor::ast::DotNode(sourcePos, moduleId,
                new cmajor::ast::DotNode(sourcePos, moduleId,
                    new cmajor::ast::DotNode(sourcePos, moduleId,
                        new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System"),
                        new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Xml")),
                    new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Serialization")),
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"FromXml"));
            cmajor::ast::InvokeNode* fromXmlInvokeNode = new cmajor::ast::InvokeNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), fromXmlDotNode);
            fromXmlInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), U"element"));
            fromXmlInvokeNode->AddArgument(new cmajor::ast::StringLiteralNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), util::ToUtf8(memberVariableSymbol->Name())));
            fromXmlInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), memberVariableSymbol->Name()));
            cmajor::ast::ExpressionStatementNode* fromXmlStatement = new cmajor::ast::ExpressionStatementNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), fromXmlInvokeNode);
            compoundStatementNode.AddStatement(fromXmlStatement);
        }

        CompileMemberFunction(fromXmlSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'FromXml' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateGetPtrsSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* getPtrsSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"GetPtrs");
    getPtrsSymbol->SetModule(&boundCompileUnit.GetModule());
    getPtrsSymbol->SetGroupName(U"GetPtrs");
    getPtrsSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    getPtrsSymbol->SetConst();
    if (HasXmlBaseClass(classTypeSymbol))
    {
        getPtrsSymbol->SetOverride();
    }
    else
    {
        getPtrsSymbol->SetVirtual();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    getPtrsSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(getPtrsSymbol);

    cmajor::symbols::TypeSymbol* listOfXmlPtrBasePtrs = symbolTable.GetTypeByName(U"List<XmlPtrBase*>");
    getPtrsSymbol->SetReturnType(listOfXmlPtrBasePtrs);

    cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"@return");
    returnParam->SetParent(getPtrsSymbol);
    returnParam->SetType(listOfXmlPtrBasePtrs->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    getPtrsSymbol->SetReturnParam(returnParam);

    classTypeSymbol->AddMember(getPtrsSymbol);
    getPtrsSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(getPtrsSymbol, getPtrsId));
}

void XmlAttributeProcessor::GenerateGetPtrsImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* getPtrsSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), getPtrsSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::TemplateIdNode* ptrsList = new cmajor::ast::TemplateIdNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Collections.List"));
            ptrsList->AddTemplateArgument(new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Xml.Serialization.XmlPtrBase")));
            cmajor::ast::ConstructionStatementNode* constructPtrsList = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, ptrsList, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ptrs"));
            constructPtrsList->SetAssignment();
            cmajor::ast::ArrowNode* arrow = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"GetPtrs"));
            constructPtrsList->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, arrow));
            compoundStatementNode.AddStatement(constructPtrsList);
        }
        else
        {
            cmajor::ast::TemplateIdNode* ptrsList = new cmajor::ast::TemplateIdNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Collections.List"));
            ptrsList->AddTemplateArgument(new cmajor::ast::PointerNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"System.Xml.Serialization.XmlPtrBase")));
            cmajor::ast::ConstructionStatementNode* constructPtrsList = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, ptrsList, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ptrs"));
            compoundStatementNode.AddStatement(constructPtrsList);
        }

        for (cmajor::symbols::MemberVariableSymbol* memberVariableSymbol : classTypeSymbol->MemberVariables())
        {
            cmajor::ast::AttributesNode* attributes = memberVariableSymbol->GetAttributes();
            if (attributes)
            {
                cmajor::ast::AttributeNode* xmlAttribute = attributes->GetAttribute(U"xml");
                if (xmlAttribute)
                {
                    if (xmlAttribute->Value() == U"false")
                    {
                        continue;
                    }
                }
            }
            if (reservedMemberVariableNames.find(memberVariableSymbol->Name()) != reservedMemberVariableNames.cend())
            {
                continue;
            }
            cmajor::symbols::TypeSymbol* typeSymbol = memberVariableSymbol->GetType();
            if (typeSymbol->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
            {
                cmajor::symbols::ClassTemplateSpecializationSymbol* specialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(typeSymbol);
                cmajor::symbols::ClassTypeSymbol* primaryType = specialization->GetClassTemplate();
                std::u32string primaryTypeGroupName = primaryType->GroupName();
                if (primaryTypeGroupName == U"XmlPtr" || primaryTypeGroupName == U"UniqueXmlPtr")
                {
                    // ptrs.Add(&x)
                    cmajor::ast::DotNode* addDot = new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ptrs"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Add"));
                    cmajor::ast::InvokeNode* invokeAdd = new cmajor::ast::InvokeNode(sourcePos, moduleId, addDot);
                    invokeAdd->AddArgument(new cmajor::ast::AddrOfNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, memberVariableSymbol->Name())));
                    cmajor::ast::ExpressionStatementNode* addStatement = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, invokeAdd);
                    compoundStatementNode.AddStatement(addStatement);
                }
                else if (primaryTypeGroupName == U"List")
                {
                    if (specialization->TemplateArgumentTypes().size() == 1)
                    {
                        cmajor::symbols::TypeSymbol* argType = specialization->TemplateArgumentTypes()[0];
                        if (argType->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol)
                        {
                            cmajor::symbols::ClassTemplateSpecializationSymbol* argSpecialization = static_cast<cmajor::symbols::ClassTemplateSpecializationSymbol*>(argType);
                            cmajor::symbols::ClassTypeSymbol* argPrimaryType = argSpecialization->GetClassTemplate();
                            std::u32string argGroupName = argPrimaryType->GroupName();
                            if (argGroupName == U"XmlPtr" || argGroupName == U"UniqueXmlPtr")
                            {
                                // long n_x = x.Count();
                                // for (long i = 0; i < n_x; ++i)
                                //{
                                //     ptrs.Add(&x[i]);
                                //}
                                cmajor::ast::ConstructionStatementNode* count = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"long"),
                                    new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"n_" + memberVariableSymbol->Name()));
                                count->SetAssignment();
                                cmajor::ast::DotNode* dotCount = new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, memberVariableSymbol->Name()), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Count"));
                                cmajor::ast::InvokeNode* invokeCount = new cmajor::ast::InvokeNode(sourcePos, moduleId, dotCount);
                                count->AddArgument(invokeCount);
                                compoundStatementNode.AddStatement(count);

                                cmajor::ast::ConstructionStatementNode* constructI = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"long"),
                                    new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"i"));
                                cmajor::ast::LessNode* testI = new cmajor::ast::LessNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"i"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"n_" + memberVariableSymbol->Name()));
                                cmajor::ast::ExpressionStatementNode* incI = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, new cmajor::ast::PrefixIncrementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"i")));
                                cmajor::ast::DotNode* addDot = new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ptrs"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Add"));
                                cmajor::ast::InvokeNode* invokeAdd = new cmajor::ast::InvokeNode(sourcePos, moduleId, addDot);
                                cmajor::ast::IndexingNode* indexing = new cmajor::ast::IndexingNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, memberVariableSymbol->Name()),
                                    new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"i"));
                                invokeAdd->AddArgument(new cmajor::ast::AddrOfNode(sourcePos, moduleId, indexing));
                                cmajor::ast::ExpressionStatementNode* actionS = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, invokeAdd);
                                cmajor::ast::ForStatementNode* forStatement = new cmajor::ast::ForStatementNode(sourcePos, moduleId, constructI, testI, incI, actionS);
                                compoundStatementNode.AddStatement(forStatement);
                            }
                        }
                    }
                }
            }
        }

        cmajor::ast::ReturnStatementNode* returnStatementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ptrs"));
        compoundStatementNode.AddStatement(returnStatementNode);

        CompileMemberFunction(getPtrsSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'GetPtrs' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateIsOwnedSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* isOwnedMemFunSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"IsOwned");
    isOwnedMemFunSymbol->SetModule(&boundCompileUnit.GetModule());
    isOwnedMemFunSymbol->SetGroupName(U"IsOwned");
    isOwnedMemFunSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    isOwnedMemFunSymbol->SetConst();
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    isOwnedMemFunSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(isOwnedMemFunSymbol);

    isOwnedMemFunSymbol->SetReturnType(symbolTable.GetTypeByName(U"bool"));

    classTypeSymbol->AddMember(isOwnedMemFunSymbol);
    isOwnedMemFunSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(isOwnedMemFunSymbol, isOwnedMemFunId));
}

void XmlAttributeProcessor::GenerateIsOwnedImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* isOwnedMemFunSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), isOwnedMemFunSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        cmajor::ast::ReturnStatementNode* returnStatementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"isOwned"));
        statementNode = returnStatementNode;
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(isOwnedMemFunSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'IsOwned' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateSetOwnedSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* setOwnedMemFunSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"SetOwned");
    setOwnedMemFunSymbol->SetModule(&boundCompileUnit.GetModule());
    setOwnedMemFunSymbol->SetGroupName(U"SetOwned");
    setOwnedMemFunSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        setOwnedMemFunSymbol->SetOverride();
    }
    else
    {
        setOwnedMemFunSymbol->SetVirtual();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    setOwnedMemFunSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(setOwnedMemFunSymbol);

    setOwnedMemFunSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));

    classTypeSymbol->AddMember(setOwnedMemFunSymbol);
    setOwnedMemFunSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(setOwnedMemFunSymbol, setOwnedMemFunId));
}

void XmlAttributeProcessor::GenerateSetOwnedImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* setOwnedMemFunSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), setOwnedMemFunSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"SetOwned"));
            cmajor::ast::InvokeNode* baseSetOwnedCall = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            statementNode = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, baseSetOwnedCall);
        }
        else
        {
            cmajor::ast::AssignmentStatementNode* assignmentStatementNode = new cmajor::ast::AssignmentStatementNode(sourcePos, moduleId,
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"isOwned"),
                new cmajor::ast::BooleanLiteralNode(sourcePos, moduleId, true));
            statementNode = assignmentStatementNode;
        }
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(setOwnedMemFunSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'SetOwned' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::GenerateResetOwnedSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* resetOwnedMemFunSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"ResetOwned");
    resetOwnedMemFunSymbol->SetModule(&boundCompileUnit.GetModule());
    resetOwnedMemFunSymbol->SetGroupName(U"ResetOwned");
    resetOwnedMemFunSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        resetOwnedMemFunSymbol->SetOverride();
    }
    else
    {
        resetOwnedMemFunSymbol->SetVirtual();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    resetOwnedMemFunSymbol->AddMember(thisParam);

    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(resetOwnedMemFunSymbol);

    resetOwnedMemFunSymbol->SetReturnType(symbolTable.GetTypeByName(U"void"));

    classTypeSymbol->AddMember(resetOwnedMemFunSymbol);
    resetOwnedMemFunSymbol->ComputeName();

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(resetOwnedMemFunSymbol, resetOwnedMemFunId));
}

void XmlAttributeProcessor::GenerateResetOwnedImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* resetOwnedMemFunSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), resetOwnedMemFunSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::MemberFunctionNode memberFunctionNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        //TODO: compoundStatementNode.SetEndBraceSourcePos(sourcePos);

        cmajor::ast::StatementNode* statementNode = nullptr;
        if (HasXmlBaseClass(classTypeSymbol))
        {
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::BaseNode(sourcePos, moduleId), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ResetOwned"));
            cmajor::ast::InvokeNode* baseResetOwnedCall = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            statementNode = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, baseResetOwnedCall);
        }
        else
        {
            cmajor::ast::AssignmentStatementNode* assignmentStatementNode = new cmajor::ast::AssignmentStatementNode(sourcePos, moduleId,
                new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"isOwned"),
                new cmajor::ast::BooleanLiteralNode(sourcePos, moduleId, false));
            statementNode = assignmentStatementNode;
        }
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(resetOwnedMemFunSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'ResetOwned' function for the class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void XmlAttributeProcessor::CompileMemberFunction(cmajor::symbols::MemberFunctionSymbol* memberFunctionSymbol, cmajor::ast::CompoundStatementNode& compoundStatementNode, cmajor::ast::MemberFunctionNode& memberFunctionNode,
    std::unique_ptr<BoundFunction>&& boundFunction, StatementBinder* statementBinder)
{
    cmajor::symbols::SymbolTable& symbolTable = statementBinder->GetBoundCompileUnit().GetSymbolTable();
    symbolTable.BeginContainer(memberFunctionSymbol);
    cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
    compoundStatementNode.Accept(symbolCreatorVisitor);
    symbolTable.EndContainer();
    TypeBinder typeBinder(statementBinder->GetBoundCompileUnit());
    cmajor::symbols::ContainerScope* containerScope = statementBinder->GetContainerScope();
    typeBinder.SetContainerScope(containerScope);
    statementBinder->SetContainerScope(memberFunctionSymbol->GetContainerScope());
    typeBinder.SetCurrentFunctionSymbol(memberFunctionSymbol);
    compoundStatementNode.Accept(typeBinder);
    BoundFunction* prevFunction = statementBinder->CurrentFunction();
    statementBinder->SetCurrentFunction(boundFunction.get());
    cmajor::symbols::MemberFunctionSymbol* prevMemberFunctionSymbol = statementBinder->CurrentMemberFunctionSymbol();
    cmajor::ast::MemberFunctionNode* prevMemberFunctionNode = statementBinder->CurrentMemberFunctionNode();
    statementBinder->SetCurrentMemberFunction(memberFunctionSymbol, &memberFunctionNode);
    compoundStatementNode.Accept(*statementBinder);
    statementBinder->SetContainerScope(containerScope);
    BoundStatement* boundStatement = statementBinder->ReleaseStatement();
    //Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
    BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
    boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
    statementBinder->CurrentClass()->AddMember(std::move(boundFunction));
    statementBinder->SetCurrentMemberFunction(prevMemberFunctionSymbol, prevMemberFunctionNode);
    statementBinder->SetCurrentFunction(prevFunction);
    statementBinder->GetBoundCompileUnit().RemoveLastFileScope();
}
} // namespace cmajor::binder
