// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.xml.attribute.processor;

import std.core;
import cmajor.ast;
import cmajor.symbols;
import cmajor.binder.bound.compile.unit;
import cmajor.binder.type.binder;
import cmajor.binder.type.resolver;
import cmajor.binder.statement.binder;
import cmajor.binder.bound.function;
import cmajor.binder.bound_class;
import cmajor.binder.bound.expression;
import util;

namespace cmajor::binder {

uint64_t GetHashCode(const std::string& s)
{
    uint64_t hashCode = 14695981039346656037ull;
    for (char c : s)
    {
        hashCode = hashCode ^ static_cast<uint64_t>(static_cast<uint8_t>(c));
        hashCode = hashCode * 1099511628211ull;
    }
    return hashCode;
}

XmlAttributeProcessor::XmlAttributeProcessor() : AttributeProcessor(U"xml")
{
    reservedMemberFunctionNames.insert(U"StaticClassName");
    reservedMemberFunctionNames.insert(U"Register");
    reservedMemberFunctionNames.insert(U"Create");
    //reservedMemberFunctionNames.insert(U"DestroyObject");
    //reservedMemberFunctionNames.insert(U"ObjectId");
    //reservedMemberFunctionNames.insert(U"SetObjectId");
    //reservedMemberFunctionNames.insert(U"Container");
    //reservedMemberFunctionNames.insert(U"SetContainer");
    //reservedMemberFunctionNames.insert(U"ClassId");
    //reservedMemberFunctionNames.insert(U"ClassName");
    reservedMemberFunctionNames.insert(U"SetObjectXmlAttributes");
    //reservedMemberFunctionNames.insert(U"ToXml");
    //reservedMemberFunctionNames.insert(U"FromXml");
    //reservedMemberFunctionNames.insert(U"IsOwned");
    //reservedMemberFunctionNames.insert(U"SetOwned");
    //reservedMemberFunctionNames.insert(U"ResetOwned");
    reservedMemberVariableNames.insert(U"classId");
    //reservedMemberVariableNames.insert(U"objectId");
    //reservedMemberVariableNames.insert(U"container");
    //reservedMemberVariableNames.insert(U"isOwned");
}

void XmlAttributeProcessor::TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, cmajor::symbols::Context* context)
{
    switch (symbol->GetSymbolType())
    {
        case cmajor::symbols::SymbolType::classTypeSymbol: 
        case cmajor::symbols::SymbolType::classTemplateSpecializationSymbol: 
        case cmajor::symbols::SymbolType::memberVariableSymbol:
        {
            if (attribute->Value() == U"true" || attribute->Value() == U"false")
            {
                if (attribute->Value() == U"true" && (symbol->GetSymbolType() == cmajor::symbols::SymbolType::classTypeSymbol || 
                    symbol->GetSymbolType() == cmajor::symbols::SymbolType::classTemplateSpecializationSymbol))
                {
                    TypeCheckClass(static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol));
                }
                return;
            }
            else
            {
                throw cmajor::symbols::Exception("unknown attribute value '" + util::ToUtf8(attribute->Value()) + "' for attribute '" + util::ToUtf8(attribute->Name()) + "'",
                    attribute->GetFullSpan());
            }
            break;
        }
    }
    AttributeProcessor::TypeCheck(attribute, symbol, context);
}

void XmlAttributeProcessor::TypeCheckClass(cmajor::symbols::ClassTypeSymbol* classType)
{
    for (cmajor::symbols::MemberFunctionSymbol* memberFunction : classType->MemberFunctions())
    {
        if (reservedMemberFunctionNames.find(memberFunction->GroupName()) != reservedMemberFunctionNames.cend())
        {
            throw cmajor::symbols::Exception("member function name '" + util::ToUtf8(memberFunction->GroupName()) + 
                "' is reserved for the implementation when using the '[xml]' attribute for a class",
                memberFunction->GetFullSpan());
        }
    }
    for (cmajor::symbols::MemberVariableSymbol* memberVariable : classType->StaticMemberVariables())
    {
        if (reservedMemberVariableNames.find(memberVariable->Name()) != reservedMemberVariableNames.cend())
        {
            throw cmajor::symbols::Exception("member variable name '" + util::ToUtf8(memberVariable->Name()) + 
                "' is reserved for the implementation when using the '[xml]' attribute for a class",
                memberVariable->GetFullSpan());
        }
    }
    for (cmajor::symbols::MemberVariableSymbol* memberVariable : classType->MemberVariables())
    {
        if (reservedMemberVariableNames.find(memberVariable->Name()) != reservedMemberVariableNames.cend())
        {
            throw cmajor::symbols::Exception("member variable name '" + util::ToUtf8(memberVariable->Name()) + 
                "' is reserved for the implementation when using the '[xml]' attribute for a class",
                memberVariable->GetFullSpan());
        }
    }
    classType->SetHasXmlAttribute();
}

void XmlAttributeProcessor::GenerateMemberSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classType, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
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
const int classIdId = 4;
const int setObjectXmlAttributesId = 5;
const int toXmlId = 6;
const int fromXmlId = 7;

void XmlAttributeProcessor::GenerateSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, BoundCompileUnit& boundCompileUnit, 
    cmajor::symbols::ContainerScope* containerScope)
{
    if (symbol->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol);
        if (attribute->Value() == U"true")
        {
            SetBaseClass(classTypeSymbol, boundCompileUnit, containerScope);
            GenerateMemberSymbols(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateMemberVariableSymbols(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateStaticClassNameSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateCreateFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateRegisterFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateClassIdFunctionSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateSetObjectXmlAttributesSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateToXmlSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
            GenerateFromXmlSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
        }
    }
}

void XmlAttributeProcessor::SetBaseClass(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit,
    cmajor::symbols::ContainerScope* containerScope)
{
    if (!classTypeSymbol->BaseClass())
    {
        cmajor::ast::IdentifierNode xmlSerializableNode(classTypeSymbol->GetSpan(), U"System.Xml.Serialization.XmlSerializable");
        cmajor::symbols::TypeSymbol* baseClassType = ResolveType(&xmlSerializableNode, boundCompileUnit, containerScope);
        if (baseClassType->IsClassTypeSymbol())
        {
            classTypeSymbol->SetBaseClass(static_cast<cmajor::symbols::ClassTypeSymbol*>(baseClassType));
        }
        else
        {
            throw cmajor::symbols::Exception("class type expected", classTypeSymbol->GetFullSpan());
        }
    }
    else if (!classTypeSymbol->BaseClass()->HasXmlAttribute())
    {
        throw cmajor::symbols::Exception("base class of class '" + util::ToUtf8(classTypeSymbol->Name()) + "' should have [xml] attribute", classTypeSymbol->GetFullSpan());
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
                throw cmajor::symbols::Exception("internal error in XML attribute implementation: member function symbol map for class type symbol '" + 
                    util::ToUtf8(classTypeSymbol->FullName()) + "' is empty",
                    attribute->GetFullSpan(), classTypeSymbol->GetFullSpan());
            }
            for (const std::pair<cmajor::symbols::FunctionSymbol*, int>& p : m)
            {
                cmajor::symbols::FunctionSymbol* functionSymbol = p.first;
                switch (p.second)
                {
                    case destructorId:
                    {
                        GenerateDestructorImplementation(attribute, static_cast<cmajor::symbols::DestructorSymbol*>(functionSymbol), statementBinder);
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
                    case classIdId:
                    {
                        GenerateClassIdImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
                        break;
                    }
                    case setObjectXmlAttributesId:
                    {
                        GenerateSetObjectXmlAttributesImplementation(attribute, classTypeSymbol, static_cast<cmajor::symbols::MemberFunctionSymbol*>(functionSymbol), statementBinder);
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
                    default:
                    {
                        throw cmajor::symbols::Exception("internal error in XML attribute implementation: member function symbol map for class type symbol '" + 
                            util::ToUtf8(classTypeSymbol->FullName()) +
                            "' contains invalid member function id " + std::to_string(p.second), attribute->GetFullSpan(), classTypeSymbol->GetFullSpan());
                    }
                }
            }
        }
        else
        {
            throw cmajor::symbols::Exception("internal error in XML attribute implementation: member function symbol map for class type symbol '" +
                util::ToUtf8(classTypeSymbol->FullName()) + "' not found",
                attribute->GetFullSpan(), classTypeSymbol->GetFullSpan());
        }
    }
}

void XmlAttributeProcessor::GenerateDestructorImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::DestructorSymbol* destructorSymbol, 
    StatementBinder* statementBinder)
{
    BoundCompileUnit& boundCompileUnit = statementBinder->GetBoundCompileUnit();
    if (!boundCompileUnit.IsGeneratedDestructorInstantiated(destructorSymbol))
    {
        boundCompileUnit.SetGeneratedDestructorInstantiated(destructorSymbol);
        cmajor::binder::GenerateDestructorImplementation(statementBinder->CurrentClass(), destructorSymbol, boundCompileUnit, statementBinder->GetContainerScope(),
            statementBinder->CurrentFunction(), attribute);
    }
}

void XmlAttributeProcessor::GenerateMemberVariableSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classType, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();

    cmajor::symbols::MemberVariableSymbol* classIdSymbol = new cmajor::symbols::MemberVariableSymbol(attribute->GetSpan(), U"classId");
    classIdSymbol->SetModule(&boundCompileUnit.GetModule());
    classIdSymbol->SetStatic();
    classIdSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    classIdSymbol->SetType(symbolTable.GetTypeByName(U"int"));
    classType->AddMember(classIdSymbol, boundCompileUnit.GetContext());
}

void XmlAttributeProcessor::GenerateStaticClassNameSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    try
    {
        cmajor::symbols::Context* context = boundCompileUnit.GetContext();
        cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
        cmajor::symbols::MemberFunctionSymbol* staticClassNameSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSpan(), U"StaticClassName");
        staticClassNameSymbol->SetModule(&boundCompileUnit.GetModule());
        staticClassNameSymbol->SetGroupName(U"StaticClassName");
        staticClassNameSymbol->SetStatic();
        staticClassNameSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);

        context->RootModule()->GetSymbolTable().SetFunctionIdFor(staticClassNameSymbol);
        cmajor::symbols::TypeSymbol* stringTypeSymbol = symbolTable.GetTypeByName(U"String<char>");
        staticClassNameSymbol->SetReturnType(stringTypeSymbol);

        cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"@return");
        returnParam->SetParent(staticClassNameSymbol);
        returnParam->SetType(stringTypeSymbol->AddPointer(context));
        staticClassNameSymbol->SetReturnParam(returnParam);
        classTypeSymbol->AddMember(staticClassNameSymbol, context);
        staticClassNameSymbol->ComputeName(context);

        auto& m = functionSymbolMap[classTypeSymbol];
        m.push_back(std::make_pair(staticClassNameSymbol, staticClassNameId));
    }
    catch (const std::exception& ex)
    {
        throw cmajor::symbols::Exception("generation of 'StaticClassName' member function symbol failed: " + std::string(ex.what()),
            attribute->GetFullSpan(), classTypeSymbol->GetFullSpan());
    }
}

void XmlAttributeProcessor::GenerateStaticClassNameImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    cmajor::symbols::MemberFunctionSymbol* staticClassNameSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), staticClassNameSymbol));
        soul::ast::Span span = attribute->GetSpan();
        cmajor::ast::MemberFunctionNode memberFunctionNode(span);
        cmajor::ast::CompoundStatementNode compoundStatementNode(span);

        cmajor::ast::StringLiteralNode* classNameLiteralNode = new cmajor::ast::StringLiteralNode(span, util::ToUtf8(classTypeSymbol->FullName()));
        cmajor::ast::ReturnStatementNode* returnStatementNode(new cmajor::ast::ReturnStatementNode(span, classNameLiteralNode));
        compoundStatementNode.AddStatement(returnStatementNode);

        CompileMemberFunction(staticClassNameSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'StaticClassName' function for the class '" + 
            util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetFullSpan(), references);
    }
}

void XmlAttributeProcessor::GenerateCreateFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* createSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSpan(), U"Create");
    createSymbol->SetModule(&boundCompileUnit.GetModule());
    createSymbol->SetGroupName(U"Create");
    createSymbol->SetStatic();
    createSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);

    context->RootModule()->GetSymbolTable().SetFunctionIdFor(createSymbol);

    cmajor::symbols::TypeSymbol* serializablePtrTypeSymbol = symbolTable.GetTypeByName(U"System.Xml.Serialization.XmlSerializable")->AddPointer(context);
    createSymbol->SetReturnType(serializablePtrTypeSymbol);

    classTypeSymbol->AddMember(createSymbol, context);
    createSymbol->ComputeName(context);

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(createSymbol, createId));
}

void XmlAttributeProcessor::GenerateCreateImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    cmajor::symbols::MemberFunctionSymbol* createFunctionSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), createFunctionSymbol));
        soul::ast::Span span = attribute->GetSpan();
        cmajor::ast::MemberFunctionNode memberFunctionNode(span);
        cmajor::ast::CompoundStatementNode compoundStatementNode(span);

        cmajor::ast::IdentifierNode* classNode = new cmajor::ast::IdentifierNode(span, classTypeSymbol->FullName());
        cmajor::ast::NewNode* newNode = new cmajor::ast::NewNode(span, classNode);
        cmajor::ast::ReturnStatementNode* returnStatementNode(new cmajor::ast::ReturnStatementNode(span, newNode));
        compoundStatementNode.AddStatement(returnStatementNode);

        CompileMemberFunction(createFunctionSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'Create' function for the class '" + 
            util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetFullSpan(), references);
    }
}

void XmlAttributeProcessor::GenerateRegisterFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* registerSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSpan(), U"Register");
    registerSymbol->SetModule(&boundCompileUnit.GetModule());
    registerSymbol->SetGroupName(U"Register");
    registerSymbol->SetStatic();
    registerSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);

    context->RootModule()->GetSymbolTable().SetFunctionIdFor(registerSymbol);

    cmajor::ast::DotNode* systemResultNode = new cmajor::ast::DotNode(attribute->GetSpan(),
        new cmajor::ast::IdentifierNode(attribute->GetSpan(), U"System"), new cmajor::ast::IdentifierNode(attribute->GetSpan(), U"Result"));
    cmajor::ast::TemplateIdNode resultBoolNode(attribute->GetSpan(), systemResultNode);
    resultBoolNode.AddTemplateArgument(new cmajor::ast::BoolNode(attribute->GetSpan()));
    cmajor::symbols::TypeSymbol* returnType = ResolveType(&resultBoolNode, boundCompileUnit, containerScope);

    registerSymbol->SetReturnType(returnType);

    cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"@return");
    returnParam->SetParent(registerSymbol);
    returnParam->SetType(returnType->AddPointer(context));
    registerSymbol->SetReturnParam(returnParam);

    classTypeSymbol->AddMember(registerSymbol, context);
    registerSymbol->ComputeName(context);

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(registerSymbol, registerId));
}

void XmlAttributeProcessor::GenerateRegisterImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    cmajor::symbols::MemberFunctionSymbol* registerSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), registerSymbol));
        soul::ast::Span span = attribute->GetSpan();
        cmajor::ast::MemberFunctionNode memberFunctionNode(span);
        cmajor::ast::CompoundStatementNode compoundStatementNode(span);

        std::string fullName = util::ToUtf8(classTypeSymbol->FullName());
        int32_t classId = GetHashCode(fullName) & 0x7FFFFFFF;

        cmajor::ast::AssignmentStatementNode* assignClassIdStatement = new cmajor::ast::AssignmentStatementNode(span, 
            new cmajor::ast::IdentifierNode(span, U"classId"), new cmajor::ast::IntLiteralNode(span, classId));
        compoundStatementNode.AddStatement(assignClassIdStatement);

        // return System.Xml.Serialization.XmlRegister(classId);
        cmajor::ast::TemplateIdNode* xmlRegister = new cmajor::ast::TemplateIdNode(span, new cmajor::ast::IdentifierNode(span, U"System.Xml.Serialization.XmlRegister"));
        xmlRegister->AddTemplateArgument(new cmajor::ast::IdentifierNode(span, classTypeSymbol->FullName()));
        cmajor::ast::InvokeNode* invokeXmlRegisterFunction = new cmajor::ast::InvokeNode(span, xmlRegister);
        invokeXmlRegisterFunction->AddArgument(new cmajor::ast::IdentifierNode(span, U"classId"));
        cmajor::ast::ReturnStatementNode* invokeXmlRegisterStatement = new cmajor::ast::ReturnStatementNode(span, invokeXmlRegisterFunction);
        compoundStatementNode.AddStatement(invokeXmlRegisterStatement);

        CompileMemberFunction(registerSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'Register' function for the class '" + 
            util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetFullSpan(), references);
    }
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

void XmlAttributeProcessor::GenerateClassIdFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* classIdSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSpan(), U"ClassId");
    classIdSymbol->SetModule(&boundCompileUnit.GetModule());
    classIdSymbol->SetGroupName(U"ClassId");
    classIdSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    classIdSymbol->SetConst();

    if (HasXmlBaseClass(classTypeSymbol))
    {
        classIdSymbol->SetOverride();
    }
    else
    {
        classIdSymbol->SetOverride();
    }

    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(context));
    classIdSymbol->AddMember(thisParam, context);

    context->RootModule()->GetSymbolTable().SetFunctionIdFor(classIdSymbol);

    classIdSymbol->SetReturnType(symbolTable.GetTypeByName(U"int"));

    classTypeSymbol->AddMember(classIdSymbol, context);
    classIdSymbol->ComputeName(context);

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(classIdSymbol, classIdId));
}

void XmlAttributeProcessor::GenerateClassIdImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    cmajor::symbols::MemberFunctionSymbol* classIdSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), classIdSymbol));
        soul::ast::Span span = attribute->GetSpan();
        cmajor::ast::MemberFunctionNode memberFunctionNode(span);
        cmajor::ast::CompoundStatementNode compoundStatementNode(span);

        cmajor::ast::StatementNode* statementNode = nullptr;
        cmajor::ast::ReturnStatementNode* returnStatementNode = new cmajor::ast::ReturnStatementNode(span, new cmajor::ast::IdentifierNode(span, U"classId"));
        statementNode = returnStatementNode;
        compoundStatementNode.AddStatement(statementNode);

        CompileMemberFunction(classIdSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'ClassId' function for the class '" + 
            util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(), classTypeSymbol->GetFullSpan(), references);
    }
}

void XmlAttributeProcessor::GenerateSetObjectXmlAttributesSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* setObjectXmlAttributesSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSpan(), U"SetObjectXmlAttributes");
    setObjectXmlAttributesSymbol->SetModule(&boundCompileUnit.GetModule());
    setObjectXmlAttributesSymbol->SetGroupName(U"SetObjectXmlAttributes");
    setObjectXmlAttributesSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        setObjectXmlAttributesSymbol->SetOverride();
    }
    else
    {
        setObjectXmlAttributesSymbol->SetOverride();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(context));
    setObjectXmlAttributesSymbol->AddMember(thisParam, context);

    context->RootModule()->GetSymbolTable().SetFunctionIdFor(setObjectXmlAttributesSymbol);

    cmajor::symbols::TypeSymbol* xmlElementTypeSymbol = symbolTable.GetTypeByName(U"System.Xml.Element");
    cmajor::symbols::ParameterSymbol* elementParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"element");
    elementParam->SetType(xmlElementTypeSymbol->AddPointer(context));
    setObjectXmlAttributesSymbol->AddMember(elementParam, context);

    cmajor::ast::DotNode* systemResultNode = new cmajor::ast::DotNode(attribute->GetSpan(),
        new cmajor::ast::IdentifierNode(attribute->GetSpan(), U"System"), new cmajor::ast::IdentifierNode(attribute->GetSpan(), U"Result"));
    cmajor::ast::TemplateIdNode resultBoolNode(attribute->GetSpan(), systemResultNode);
    resultBoolNode.AddTemplateArgument(new cmajor::ast::BoolNode(attribute->GetSpan()));
    cmajor::symbols::TypeSymbol* returnType = ResolveType(&resultBoolNode, boundCompileUnit, containerScope);

    setObjectXmlAttributesSymbol->SetReturnType(returnType);

    cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"@return");
    returnParam->SetParent(setObjectXmlAttributesSymbol);
    returnParam->SetType(returnType->AddPointer(context));
    setObjectXmlAttributesSymbol->SetReturnParam(returnParam);

    classTypeSymbol->AddMember(setObjectXmlAttributesSymbol, context);
    setObjectXmlAttributesSymbol->ComputeName(context);

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(setObjectXmlAttributesSymbol, setObjectXmlAttributesId));
}

void XmlAttributeProcessor::GenerateSetObjectXmlAttributesImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    cmajor::symbols::MemberFunctionSymbol* setObjectXmlAttributesSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), setObjectXmlAttributesSymbol));
        soul::ast::Span span = attribute->GetSpan();
        cmajor::ast::MemberFunctionNode memberFunctionNode(span);
        cmajor::ast::CompoundStatementNode compoundStatementNode(span);

        // element->SetAttribute("classId", ToString(classId));
        cmajor::ast::ArrowNode* setClassIdArrowNode = new cmajor::ast::ArrowNode(span, new cmajor::ast::IdentifierNode(span, U"element"), 
            new cmajor::ast::IdentifierNode(span, U"SetAttribute"));
        cmajor::ast::InvokeNode* invokeSetClassIdAttribute = new cmajor::ast::InvokeNode(span, setClassIdArrowNode);
        cmajor::ast::StringLiteralNode* classIdLiteralArg = new cmajor::ast::StringLiteralNode(span, "classId");
        invokeSetClassIdAttribute->AddArgument(classIdLiteralArg);
        cmajor::ast::InvokeNode* invokeClassIdToStringFun = new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"ToString"));
        invokeClassIdToStringFun->AddArgument(new cmajor::ast::IdentifierNode(span, U"classId"));
        invokeSetClassIdAttribute->AddArgument(invokeClassIdToStringFun);
        cmajor::ast::ExpressionStatementNode* setClassIdAttributeStatementNode = new cmajor::ast::ExpressionStatementNode(span, invokeSetClassIdAttribute);
        compoundStatementNode.AddStatement(setClassIdAttributeStatementNode);

        // auto objectIdResult = ObjectId()->ToString();
        cmajor::ast::ConstructionStatementNode* constructObjectIdResult = new cmajor::ast::ConstructionStatementNode(span,
            new cmajor::ast::AutoNode(span),
            new cmajor::ast::IdentifierNode(span, U"objectIdResult"));
        cmajor::ast::InvokeNode* invokeObjectIdToStringFun = new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"ToString"));
        invokeObjectIdToStringFun->AddArgument(new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"ObjectId")));
        constructObjectIdResult->AddArgument(invokeObjectIdToStringFun);
        compoundStatementNode.AddStatement(constructObjectIdResult);

        // if (objectIdResult.Error()) return System.Result<bool>(System.ErrorId(objectIdResult.GetErrorId()));
        cmajor::ast::DotNode* objectIdResultErrorCond = new cmajor::ast::DotNode(span,
            new cmajor::ast::IdentifierNode(span, U"objectIdResult"),
            new cmajor::ast::IdentifierNode(span, U"Error"));
        cmajor::ast::InvokeNode* invokeObjectIdResultErrorCond = new cmajor::ast::InvokeNode(span, objectIdResultErrorCond);
        cmajor::ast::TemplateIdNode* resultBoolNode = new cmajor::ast::TemplateIdNode(span, new cmajor::ast::IdentifierNode(span, U"System.Result"));
        resultBoolNode->AddTemplateArgument(new cmajor::ast::BoolNode(span));
        cmajor::ast::InvokeNode* invokeResultBoolNode = new cmajor::ast::InvokeNode(span, resultBoolNode);
        cmajor::ast::IdentifierNode * systemErrorIdNode = new cmajor::ast::IdentifierNode(span, U"System.ErrorId");
        cmajor::ast::InvokeNode* invokeSystemErrorIdNode = new cmajor::ast::InvokeNode(span, systemErrorIdNode);
        cmajor::ast::DotNode* getErrorIdNode = new cmajor::ast::DotNode(span,
            new cmajor::ast::IdentifierNode(span, U"objectIdResult"),
            new cmajor::ast::IdentifierNode(span, U"GetErrorId"));
        cmajor::ast::InvokeNode* invokeGetErrorIdNode = new cmajor::ast::InvokeNode(span, getErrorIdNode);
        invokeSystemErrorIdNode->AddArgument(invokeGetErrorIdNode);
        invokeResultBoolNode->AddArgument(invokeSystemErrorIdNode);
        cmajor::ast::ReturnStatementNode* returnErrorIdStatement = new cmajor::ast::ReturnStatementNode(span, invokeResultBoolNode);
        cmajor::ast::IfStatementNode* ifObjectIdResultStatement = new cmajor::ast::IfStatementNode(span,
            invokeObjectIdResultErrorCond, returnErrorIdStatement, nullptr);
        compoundStatementNode.AddStatement(ifObjectIdResultStatement);

        // element->SetAttribute("objectId", objectIdResult.Value());
        cmajor::ast::ArrowNode* setObjectIdArrowNode = new cmajor::ast::ArrowNode(span, new cmajor::ast::IdentifierNode(span, U"element"), 
            new cmajor::ast::IdentifierNode(span, U"SetAttribute"));
        cmajor::ast::InvokeNode* invokeSetObjectIdAttribute = new cmajor::ast::InvokeNode(span, setObjectIdArrowNode);
        cmajor::ast::StringLiteralNode* objectIdLiteralArg = new cmajor::ast::StringLiteralNode(span, "objectId");
        invokeSetObjectIdAttribute->AddArgument(objectIdLiteralArg);
        cmajor::ast::DotNode* objectIdResultValueNode = new cmajor::ast::DotNode(span,
            new cmajor::ast::IdentifierNode(span, U"objectIdResult"),
            new cmajor::ast::IdentifierNode(span, U"Value"));
        cmajor::ast::InvokeNode* invokeObjectIdResultValue = new cmajor::ast::InvokeNode(span, objectIdResultValueNode);
        invokeSetObjectIdAttribute->AddArgument(invokeObjectIdResultValue);
        cmajor::ast::ExpressionStatementNode* setObjectIdAttributeStatementNode = new cmajor::ast::ExpressionStatementNode(span, invokeSetObjectIdAttribute);
        compoundStatementNode.AddStatement(setObjectIdAttributeStatementNode);

        // element->SetAttribute("className", ClassName());
        cmajor::ast::ArrowNode* setClassNameArrowNode = new cmajor::ast::ArrowNode(span, new cmajor::ast::IdentifierNode(span, U"element"), 
            new cmajor::ast::IdentifierNode(span, U"SetAttribute"));
        cmajor::ast::InvokeNode* invokeSetClassNameAttribute = new cmajor::ast::InvokeNode(span, setClassNameArrowNode);
        cmajor::ast::StringLiteralNode* classNameLiteralArg = new cmajor::ast::StringLiteralNode(span, "className");
        invokeSetClassNameAttribute->AddArgument(classNameLiteralArg);
        invokeSetClassNameAttribute->AddArgument(new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"ClassName")));
        cmajor::ast::ExpressionStatementNode* setClassNameAttributeStatementNode = new cmajor::ast::ExpressionStatementNode(span, invokeSetClassNameAttribute);
        compoundStatementNode.AddStatement(setClassNameAttributeStatementNode);

        // return System.Result<bool>(true);
        cmajor::ast::TemplateIdNode* resultBool = new cmajor::ast::TemplateIdNode(span, new cmajor::ast::IdentifierNode(span, U"System.Result"));
        resultBool->AddTemplateArgument(new cmajor::ast::BoolNode(span));
        cmajor::ast::InvokeNode* invokeResultBoolTrue = new cmajor::ast::InvokeNode(span, resultBool);
        invokeResultBoolTrue->AddArgument(new cmajor::ast::BooleanLiteralNode(span, true));
        cmajor::ast::ReturnStatementNode* returnStatmentNode = new cmajor::ast::ReturnStatementNode(span, invokeResultBoolTrue);
        compoundStatementNode.AddStatement(returnStatmentNode);

        CompileMemberFunction(setObjectXmlAttributesSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'SetObjectXmlAttributes' function for the class '" + 
            util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(), classTypeSymbol->GetFullSpan(), references);
    }
}

void XmlAttributeProcessor::GenerateToXmlSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* toXmlSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSpan(), U"ToXml");
    toXmlSymbol->SetModule(&boundCompileUnit.GetModule());
    toXmlSymbol->SetGroupName(U"ToXml");
    toXmlSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        toXmlSymbol->SetOverride();
    }
    else
    {
        toXmlSymbol->SetOverride();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(context));
    toXmlSymbol->AddMember(thisParam, context);

    cmajor::symbols::ParameterSymbol* elementNameParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"elementName");
    elementNameParam->SetType(symbolTable.GetTypeByName(U"String<char>")->AddConst(context)->AddLvalueReference(context));
    toXmlSymbol->AddMember(elementNameParam, context);

    cmajor::symbols::ParameterSymbol* ctxParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"ctx");
    ctxParam->SetType(symbolTable.GetTypeByName(U"System.Xml.Serialization.XmlSerializationContext")->AddLvalueReference(context));
    toXmlSymbol->AddMember(ctxParam, context);

    context->RootModule()->GetSymbolTable().SetFunctionIdFor(toXmlSymbol);

    cmajor::ast::DotNode* systemResultNode = new cmajor::ast::DotNode(attribute->GetSpan(),
        new cmajor::ast::IdentifierNode(attribute->GetSpan(), U"System"), new cmajor::ast::IdentifierNode(attribute->GetSpan(), U"Result"));
    cmajor::ast::TemplateIdNode resultXmlElementNode(attribute->GetSpan(), systemResultNode);
    resultXmlElementNode.AddTemplateArgument(new cmajor::ast::PointerNode(attribute->GetSpan(), new cmajor::ast::IdentifierNode(attribute->GetSpan(), U"System.Xml.Element")));
    cmajor::symbols::TypeSymbol* returnType = ResolveType(&resultXmlElementNode, boundCompileUnit, containerScope);

    toXmlSymbol->SetReturnType(returnType);

    cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"@return");
    returnParam->SetParent(toXmlSymbol);
    returnParam->SetType(returnType->AddPointer(context));
    toXmlSymbol->SetReturnParam(returnParam);

    classTypeSymbol->AddMember(toXmlSymbol, context);
    toXmlSymbol->ComputeName(context);

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(toXmlSymbol, toXmlId));
}

void XmlAttributeProcessor::GenerateToXmlImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    cmajor::symbols::MemberFunctionSymbol* toXmlSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), toXmlSymbol));
        soul::ast::Span span = attribute->GetSpan();
        cmajor::ast::MemberFunctionNode memberFunctionNode(span);
        cmajor::ast::CompoundStatementNode compoundStatementNode(span);

        if (HasXmlBaseClass(classTypeSymbol))
        {
            // auto result = base->ToXml(elementName, ctx);
            cmajor::ast::ConstructionStatementNode* constructionStatementNode = new cmajor::ast::ConstructionStatementNode(span,
                new cmajor::ast::AutoNode(span),
                new cmajor::ast::IdentifierNode(span, U"result"));
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(span, new cmajor::ast::BaseNode(span), new cmajor::ast::IdentifierNode(span, U"ToXml"));
            cmajor::ast::InvokeNode* invokeNode = new cmajor::ast::InvokeNode(span, arrowNode);
            invokeNode->AddArgument(new cmajor::ast::IdentifierNode(span, U"elementName"));
            invokeNode->AddArgument(new cmajor::ast::IdentifierNode(span, U"ctx"));
            constructionStatementNode->AddArgument(invokeNode);
            constructionStatementNode->SetAssignment();
            compoundStatementNode.AddStatement(constructionStatementNode);
            // if (result.Error()) return result;
            cmajor::ast::DotNode* resultDotError = new cmajor::ast::DotNode(
                span,
                new cmajor::ast::IdentifierNode(span, U"result"),
                new cmajor::ast::IdentifierNode(span, U"Error"));
            cmajor::ast::InvokeNode* invokeResultError = new cmajor::ast::InvokeNode(span, resultDotError);
            cmajor::ast::ReturnStatementNode* returnResultStmt = new cmajor::ast::ReturnStatementNode(span, new cmajor::ast::IdentifierNode(span, U"result"));
            cmajor::ast::IfStatementNode* ifStmt = new cmajor::ast::IfStatementNode(span, invokeResultError, returnResultStmt, nullptr);
            compoundStatementNode.AddStatement(ifStmt);
            // System.Xml.Element* element = result.Value();
            cmajor::ast::DotNode* resultValueNode = new cmajor::ast::DotNode(span,
                new cmajor::ast::IdentifierNode(span, U"result"),
                new cmajor::ast::IdentifierNode(span, U"Value"));
            cmajor::ast::InvokeNode* invokeResultValueNode = new cmajor::ast::InvokeNode(span, resultValueNode);
            cmajor::ast::ConstructionStatementNode* constructElementNode = new cmajor::ast::ConstructionStatementNode(span,
                new cmajor::ast::PointerNode(span, new cmajor::ast::IdentifierNode(span, U"System.Xml.Element")),
                new cmajor::ast::IdentifierNode(span, U"element"));
            constructElementNode->SetAssignment();
            constructElementNode->AddArgument(invokeResultValueNode);
            compoundStatementNode.AddStatement(constructElementNode);
        }
        else
        {
            cmajor::ast::ConstructionStatementNode* constructionStatementNode = new cmajor::ast::ConstructionStatementNode(span,
                new cmajor::ast::PointerNode(span, new cmajor::ast::IdentifierNode(span, U"System.Xml.Element")),
                new cmajor::ast::IdentifierNode(span, U"element"));
            cmajor::ast::InvokeNode* makeElementNode = new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"System.Xml.MakeElement"));
            makeElementNode->AddArgument(new cmajor::ast::IdentifierNode(span, U"elementName"));
            constructionStatementNode->AddArgument(makeElementNode);
            constructionStatementNode->SetAssignment();
            compoundStatementNode.AddStatement(constructionStatementNode);

            cmajor::ast::DotNode* dotNode = new cmajor::ast::DotNode(span, new cmajor::ast::IdentifierNode(span, U"ctx"), new cmajor::ast::IdentifierNode(span, U"GetFlag"));
            cmajor::ast::InvokeNode* invokeNode = new cmajor::ast::InvokeNode(span, dotNode);
            invokeNode->AddArgument(new cmajor::ast::IdentifierNode(span, U"System.XmlSerialization.XmlSerializationFlags.suppressMetadata"));
            cmajor::ast::NotNode* cond = new cmajor::ast::NotNode(span, invokeNode);

            cmajor::ast::InvokeNode* invokeSetObjectXmlAttributesNode = new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"SetObjectXmlAttributes"));
            invokeSetObjectXmlAttributesNode->AddArgument(new cmajor::ast::IdentifierNode(span, U"element"));
            // auto setResult = SetObjectXmlAttributes(element);
            cmajor::ast::ConstructionStatementNode* constructSetResultStatementNode = new cmajor::ast::ConstructionStatementNode(span,
                new cmajor::ast::AutoNode(span),
                new cmajor::ast::IdentifierNode(span, U"setResult"));
            constructSetResultStatementNode->SetAssignment();
            constructSetResultStatementNode->AddArgument(invokeSetObjectXmlAttributesNode);

            // if (setResult.Error()) return System.Result<System.Xml.Element*>(System.ErrorId(setResult.GetErrorId()));
            cmajor::ast::DotNode* setResultErrorCond = new cmajor::ast::DotNode(span,
                new cmajor::ast::IdentifierNode(span, U"setResult"),
                new cmajor::ast::IdentifierNode(span, U"Error"));
            cmajor::ast::InvokeNode* invokeSetResultErrorCond = new cmajor::ast::InvokeNode(span, setResultErrorCond);
            cmajor::ast::TemplateIdNode* systemResultNode = new cmajor::ast::TemplateIdNode(span, new cmajor::ast::IdentifierNode(span, U"System.Result"));
            cmajor::ast::PointerNode* systemXmlElement = new cmajor::ast::PointerNode(span, new cmajor::ast::IdentifierNode(span, U"System.Xml.Element"));
            systemResultNode->AddTemplateArgument(systemXmlElement);
            cmajor::ast::InvokeNode* invokeSystemResultNode = new cmajor::ast::InvokeNode(span, systemResultNode);
            cmajor::ast::IdentifierNode* systemErrorId = new cmajor::ast::IdentifierNode(span, U"System.ErrorId");
            cmajor::ast::InvokeNode* invokeSystemErrorId = new cmajor::ast::InvokeNode(span, systemErrorId);
            cmajor::ast::DotNode* getErrorIdNode = new cmajor::ast::DotNode(span,
                new cmajor::ast::IdentifierNode(span, U"setResult"),
                new cmajor::ast::IdentifierNode(span, U"GetErrorId"));
            cmajor::ast::InvokeNode* invokeGetErrorId = new cmajor::ast::InvokeNode(span, getErrorIdNode);
            invokeSystemErrorId->AddArgument(invokeGetErrorId);
            invokeSystemResultNode->AddArgument(invokeSystemErrorId);
            cmajor::ast::ReturnStatementNode* returnErrorStatement = new cmajor::ast::ReturnStatementNode(span, invokeSystemResultNode);
            cmajor::ast::IfStatementNode* ifSetResultErrorStatement = new cmajor::ast::IfStatementNode(span, invokeSetResultErrorCond, returnErrorStatement, nullptr);

            cmajor::ast::CompoundStatementNode* compoundSetObjectXmlAttributesStatement = new cmajor::ast::CompoundStatementNode(span);
            compoundSetObjectXmlAttributesStatement->AddStatement(constructSetResultStatementNode);
            compoundSetObjectXmlAttributesStatement->AddStatement(ifSetResultErrorStatement);
            cmajor::ast::IfStatementNode* ifStatementNode = new cmajor::ast::IfStatementNode(span, cond, compoundSetObjectXmlAttributesStatement, nullptr);

            compoundStatementNode.AddStatement(ifStatementNode);
        }

        bool first = true;
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
            cmajor::ast::DotNode* toXmlDotNode = new cmajor::ast::DotNode(span,
                new cmajor::ast::DotNode(span,
                    new cmajor::ast::DotNode(span,
                        new cmajor::ast::IdentifierNode(span, U"System"),
                        new cmajor::ast::IdentifierNode(span, U"Xml")),
                    new cmajor::ast::IdentifierNode(span, U"Serialization")),
                new cmajor::ast::IdentifierNode(span, U"ToXml"));
            cmajor::ast::InvokeNode* toXmlInvokeNode = new cmajor::ast::InvokeNode(memberVariableSymbol->GetSpan(), toXmlDotNode);
            toXmlInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), memberVariableSymbol->Name()));
            toXmlInvokeNode->AddArgument(new cmajor::ast::StringLiteralNode(memberVariableSymbol->GetSpan(), util::ToUtf8(memberVariableSymbol->Name())));
            toXmlInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"ctx"));

            if (first)
            {
                first = false;
                // auto memberResult = System.Xml.Serialization.ToXml(variableName, "variableName", ctx);
                cmajor::ast::ConstructionStatementNode* constructionStatement = new cmajor::ast::ConstructionStatementNode(memberVariableSymbol->GetSpan(),
                    new cmajor::ast::AutoNode(memberVariableSymbol->GetSpan()), new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"memberResult"));
                constructionStatement->SetAssignment();
                constructionStatement->AddArgument(toXmlInvokeNode);
                compoundStatementNode.AddStatement(constructionStatement);
            }
            else
            {
                cmajor::ast::AssignmentStatementNode* assignmentStatement = new cmajor::ast::AssignmentStatementNode(memberVariableSymbol->GetSpan(),
                    new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"memberResult"), toXmlInvokeNode);
                compoundStatementNode.AddStatement(assignmentStatement);
            }

            // if (memberResult.Error()) return memberResult;
            cmajor::ast::DotNode* memberResultDotError = new cmajor::ast::DotNode(
                span,
                new cmajor::ast::IdentifierNode(span, U"memberResult"),
                new cmajor::ast::IdentifierNode(span, U"Error"));
            cmajor::ast::InvokeNode* invokeMemberResultError = new cmajor::ast::InvokeNode(span, memberResultDotError);
            cmajor::ast::ReturnStatementNode* returnMemberResultStmt = new cmajor::ast::ReturnStatementNode(span, new cmajor::ast::IdentifierNode(span, U"memberResult"));
            cmajor::ast::IfStatementNode* ifStmt = new cmajor::ast::IfStatementNode(span, invokeMemberResultError, returnMemberResultStmt, nullptr);
            compoundStatementNode.AddStatement(ifStmt);

            cmajor::ast::DotNode* memberResultValueNode = new cmajor::ast::DotNode(span,
                new cmajor::ast::IdentifierNode(span, U"memberResult"),
                new cmajor::ast::IdentifierNode(span, U"Value"));
            cmajor::ast::InvokeNode* invokeMemberResultValueNode = new cmajor::ast::InvokeNode(span, memberResultValueNode);

            // element->AppendChild(memberResult.Value());
            cmajor::ast::ArrowNode* appendChildArrowNode = new cmajor::ast::ArrowNode(span, new cmajor::ast::IdentifierNode(span, U"element"), 
                new cmajor::ast::IdentifierNode(span, U"AppendChild"));
            cmajor::ast::InvokeNode* appendChildInvokeNode = new cmajor::ast::InvokeNode(span, appendChildArrowNode);
            appendChildInvokeNode->AddArgument(invokeMemberResultValueNode);
            cmajor::ast::ExpressionStatementNode* appendChildStatement = new cmajor::ast::ExpressionStatementNode(memberVariableSymbol->GetSpan(), appendChildInvokeNode);
            compoundStatementNode.AddStatement(appendChildStatement);
        }

        cmajor::ast::TemplateIdNode* resultNode = new cmajor::ast::TemplateIdNode(span, new cmajor::ast::IdentifierNode(span, U"System.Result"));
        resultNode->AddTemplateArgument(new cmajor::ast::PointerNode(span, new cmajor::ast::IdentifierNode(span, U"System.Xml.Element")));
        cmajor::ast::InvokeNode* invokeResultNode = new cmajor::ast::InvokeNode(span, resultNode);
        invokeResultNode->AddArgument(new cmajor::ast::IdentifierNode(span, U"element"));
        cmajor::ast::ReturnStatementNode* returnStatement = new cmajor::ast::ReturnStatementNode(span, invokeResultNode);
        compoundStatementNode.AddStatement(returnStatement);

        CompileMemberFunction(toXmlSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'ToXml' function for the class '" + 
            util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetFullSpan(), references);
    }
}

void XmlAttributeProcessor::GenerateFromXmlSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::Context* context = boundCompileUnit.GetContext();
    cmajor::symbols::SymbolTable& symbolTable = boundCompileUnit.GetSymbolTable();
    cmajor::symbols::MemberFunctionSymbol* fromXmlSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSpan(), U"FromXml");
    fromXmlSymbol->SetModule(&boundCompileUnit.GetModule());
    fromXmlSymbol->SetGroupName(U"FromXml");
    fromXmlSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    if (HasXmlBaseClass(classTypeSymbol))
    {
        fromXmlSymbol->SetOverride();
    }
    else
    {
        fromXmlSymbol->SetOverride();
    }
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(context));
    fromXmlSymbol->AddMember(thisParam, context);

    cmajor::symbols::ParameterSymbol* elementParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"element");
    elementParam->SetType(symbolTable.GetTypeByName(U"System.Xml.Element")->AddPointer(context));
    fromXmlSymbol->AddMember(elementParam, context);

    cmajor::symbols::ParameterSymbol* ctxParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"ctx");
    ctxParam->SetType(symbolTable.GetTypeByName(U"System.Xml.Serialization.XmlSerializationContext")->AddLvalueReference(context));
    fromXmlSymbol->AddMember(ctxParam, context);

    context->RootModule()->GetSymbolTable().SetFunctionIdFor(fromXmlSymbol);

    cmajor::ast::DotNode* systemResultNode = new cmajor::ast::DotNode(attribute->GetSpan(),
        new cmajor::ast::IdentifierNode(attribute->GetSpan(), U"System"), new cmajor::ast::IdentifierNode(attribute->GetSpan(), U"Result"));
    cmajor::ast::TemplateIdNode resultBoolNode(attribute->GetSpan(), systemResultNode);
    resultBoolNode.AddTemplateArgument(new cmajor::ast::BoolNode(attribute->GetSpan()));
    cmajor::symbols::TypeSymbol* returnType = ResolveType(&resultBoolNode, boundCompileUnit, containerScope);

    fromXmlSymbol->SetReturnType(returnType); 

    cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSpan(), U"@return");
    returnParam->SetParent(fromXmlSymbol);
    returnParam->SetType(returnType->AddPointer(context));
    fromXmlSymbol->SetReturnParam(returnParam);

    classTypeSymbol->AddMember(fromXmlSymbol, context);
    fromXmlSymbol->ComputeName(context);

    auto& m = functionSymbolMap[classTypeSymbol];
    m.push_back(std::make_pair(fromXmlSymbol, fromXmlId));
}

void XmlAttributeProcessor::GenerateFromXmlImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
    cmajor::symbols::MemberFunctionSymbol* fromXmlSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), fromXmlSymbol));
        soul::ast::Span span = attribute->GetSpan();
        cmajor::ast::MemberFunctionNode memberFunctionNode(span);
        cmajor::ast::CompoundStatementNode compoundStatementNode(span);

        if (HasXmlBaseClass(classTypeSymbol))
        {
            // auto result = base->FromXml(element);
            cmajor::ast::InvokeNode* invokeBaseFromXml = new cmajor::ast::InvokeNode(span, new cmajor::ast::ArrowNode(span, new cmajor::ast::BaseNode(span), 
                new cmajor::ast::IdentifierNode(span, U"FromXml")));
            invokeBaseFromXml->AddArgument(new cmajor::ast::IdentifierNode(span, U"element"));
            invokeBaseFromXml->AddArgument(new cmajor::ast::IdentifierNode(span, U"ctx"));
            cmajor::ast::ConstructionStatementNode* baseFromXmlStatement = new cmajor::ast::ConstructionStatementNode(span,
                new cmajor::ast::AutoNode(span),
                new cmajor::ast::IdentifierNode(span, U"result"));
            baseFromXmlStatement->SetAssignment();
            baseFromXmlStatement->AddArgument(invokeBaseFromXml);
            compoundStatementNode.AddStatement(baseFromXmlStatement);

            // if (result.Error()) return result;
            cmajor::ast::DotNode* resultDotError = new cmajor::ast::DotNode(
                span,
                new cmajor::ast::IdentifierNode(span, U"result"),
                new cmajor::ast::IdentifierNode(span, U"Error"));
            cmajor::ast::InvokeNode* invokeResultError = new cmajor::ast::InvokeNode(span, resultDotError);
            cmajor::ast::ReturnStatementNode* returnResultStmt = new cmajor::ast::ReturnStatementNode(span, new cmajor::ast::IdentifierNode(span, U"result"));
            cmajor::ast::IfStatementNode* ifStmt = new cmajor::ast::IfStatementNode(span, invokeResultError, returnResultStmt, nullptr);
            compoundStatementNode.AddStatement(ifStmt);
        }
        else
        {
            // System.string objectIdAttr = element->GetAttribute("objectId");
            cmajor::ast::ConstructionStatementNode* constructionStatementNode = new cmajor::ast::ConstructionStatementNode(span,
                new cmajor::ast::IdentifierNode(span, U"System.string"),
                new cmajor::ast::IdentifierNode(span, U"objectIdAttr"));
            constructionStatementNode->SetAssignment();
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(span, new cmajor::ast::IdentifierNode(span, U"element"), 
                new cmajor::ast::IdentifierNode(span, U"GetAttribute"));
            cmajor::ast::StringLiteralNode* objectIdLiteral = new cmajor::ast::StringLiteralNode(span, "objectId");
            cmajor::ast::InvokeNode* elementGetAttributeNode = new cmajor::ast::InvokeNode(span, arrowNode);
            elementGetAttributeNode->AddArgument(objectIdLiteral);
            constructionStatementNode->AddArgument(elementGetAttributeNode);
            constructionStatementNode->SetAssignment();
            compoundStatementNode.AddStatement(constructionStatementNode);

            // if (!objectIdAttr.IsEmpty())
            // {
            cmajor::ast::DotNode* dotNode = new cmajor::ast::DotNode(span, new cmajor::ast::IdentifierNode(span, U"objectIdAttr"),
                new cmajor::ast::IdentifierNode(span, U"IsEmpty"));
            cmajor::ast::NotNode* notObjectIdAttrIsEmpty = new cmajor::ast::NotNode(span, new cmajor::ast::InvokeNode(span, dotNode));
            cmajor::ast::CompoundStatementNode* assignObjectIdCompoundStatement = new cmajor::ast::CompoundStatementNode(span);
            cmajor::ast::IfStatementNode* ifStatementNode = new cmajor::ast::IfStatementNode(span, notObjectIdAttrIsEmpty, assignObjectIdCompoundStatement, nullptr);
            compoundStatementNode.AddStatement(ifStatementNode);

            //      auto uuidResult = ParseUuid(objectIdAttr);
            cmajor::ast::InvokeNode* invokeParseUuid = new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"ParseUuid"));
            invokeParseUuid->AddArgument(new cmajor::ast::IdentifierNode(span, U"objectIdAttr"));
            cmajor::ast::ConstructionStatementNode* constructParseResultNode = new cmajor::ast::ConstructionStatementNode(span,
                new cmajor::ast::AutoNode(span),
                new cmajor::ast::IdentifierNode(span, U"uuidResult"));
            constructParseResultNode->SetAssignment();
            constructParseResultNode->AddArgument(invokeParseUuid);
            assignObjectIdCompoundStatement->AddStatement(constructParseResultNode);

            cmajor::ast::DotNode* uuidResultErrorNode = new cmajor::ast::DotNode(span,
                new cmajor::ast::IdentifierNode(span, U"uuidResult"),
                new cmajor::ast::IdentifierNode(span, U"Error"));
            cmajor::ast::InvokeNode* invokeUuidResultErrorNode = new cmajor::ast::InvokeNode(span, uuidResultErrorNode);
            // if (uuidResult.Error()) return Result<bool>(ErrorId(uuidResult.GetErrorId()));
            cmajor::ast::TemplateIdNode* resultBoolNode = new cmajor::ast::TemplateIdNode(span, new cmajor::ast::IdentifierNode(span, U"System.Result"));
            resultBoolNode->AddTemplateArgument(new cmajor::ast::BoolNode(span));
            cmajor::ast::InvokeNode* invokeResultBoolNode = new cmajor::ast::InvokeNode(span, resultBoolNode);
            cmajor::ast::IdentifierNode* errorIdNode = new cmajor::ast::IdentifierNode(span, U"System.ErrorId");
            cmajor::ast::InvokeNode* invokeErrorIdNode = new cmajor::ast::InvokeNode(span, errorIdNode);
            cmajor::ast::DotNode* uuidResultGetErrorIdNode = new cmajor::ast::DotNode(span,
                new cmajor::ast::IdentifierNode(span, U"uuidResult"),
                new cmajor::ast::IdentifierNode(span, U"GetErrorId"));
            cmajor::ast::InvokeNode* invokeUuidResultGetErrorIdNode = new cmajor::ast::InvokeNode(span, uuidResultGetErrorIdNode);
            invokeErrorIdNode->AddArgument(invokeUuidResultGetErrorIdNode);
            invokeResultBoolNode->AddArgument(invokeErrorIdNode);
            cmajor::ast::ReturnStatementNode* returnUuidResultErrorNode = new cmajor::ast::ReturnStatementNode(span, invokeResultBoolNode);
            cmajor::ast::IfStatementNode* ifUuidResultError = new cmajor::ast::IfStatementNode(span, invokeUuidResultErrorNode, returnUuidResultErrorNode, nullptr);
            assignObjectIdCompoundStatement->AddStatement(ifUuidResultError);

            // SetObjectId(uuidResult.Value());
            cmajor::ast::DotNode* uuidResultValue = new cmajor::ast::DotNode(span,
                new cmajor::ast::IdentifierNode(span, U"uuidResult"),
                new cmajor::ast::IdentifierNode(span, U"Value"));
            cmajor::ast::InvokeNode* invokeUuidResultValue = new cmajor::ast::InvokeNode(span, uuidResultValue);
            cmajor::ast::InvokeNode* invokeSetObjectId = new cmajor::ast::InvokeNode(span, new cmajor::ast::IdentifierNode(span, U"SetObjectId"));
            invokeSetObjectId->AddArgument(invokeUuidResultValue);
            cmajor::ast::ExpressionStatementNode* setObjectIdStatement = new cmajor::ast::ExpressionStatementNode(span, invokeSetObjectId);
            assignObjectIdCompoundStatement->AddStatement(setObjectIdStatement);
            // }
        }

        bool first = true;
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
            cmajor::ast::DotNode* fromXmlDotNode = new cmajor::ast::DotNode(span,
                new cmajor::ast::DotNode(span,
                    new cmajor::ast::DotNode(span,
                        new cmajor::ast::IdentifierNode(span, U"System"),
                        new cmajor::ast::IdentifierNode(span, U"Xml")),
                    new cmajor::ast::IdentifierNode(span, U"Serialization")),
                new cmajor::ast::IdentifierNode(span, U"FromXml"));
            cmajor::ast::InvokeNode* fromXmlInvokeNode = new cmajor::ast::InvokeNode(memberVariableSymbol->GetSpan(), fromXmlDotNode);
            fromXmlInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"element"));
            fromXmlInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"ctx"));
            fromXmlInvokeNode->AddArgument(new cmajor::ast::StringLiteralNode(memberVariableSymbol->GetSpan(), util::ToUtf8(memberVariableSymbol->Name())));
            fromXmlInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), memberVariableSymbol->Name()));

            if (first)
            {
                first = false;
                // auto memberResult = System.Xml.Serialization.FromXml(element, "memberVariable", memberVariable);
                
                cmajor::ast::ConstructionStatementNode* constructMemberResult = new cmajor::ast::ConstructionStatementNode(memberVariableSymbol->GetSpan(),
                    new cmajor::ast::AutoNode(memberVariableSymbol->GetSpan()), new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"memberResult"));
                constructMemberResult->AddArgument(fromXmlInvokeNode);
                compoundStatementNode.AddStatement(constructMemberResult);
            }
            else
            {
                // memberResult = System.Xml.Serialization.FromXml(element, "memberVariable", memberVariable);
                cmajor::ast::AssignmentStatementNode* assignmentStatement = new cmajor::ast::AssignmentStatementNode(memberVariableSymbol->GetSpan(),
                    new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"memberResult"),
                    fromXmlInvokeNode);
                compoundStatementNode.AddStatement(assignmentStatement);
            }

            // if (memberResult.Error()) return memberResult;
            cmajor::ast::DotNode* memberResultErrorCond = new cmajor::ast::DotNode(memberVariableSymbol->GetSpan(),
                new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"memberResult"),
                new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"Error"));
            cmajor::ast::InvokeNode* invokeMemberResultErrorCond = new cmajor::ast::InvokeNode(memberVariableSymbol->GetSpan(), memberResultErrorCond);
            cmajor::ast::ReturnStatementNode* returnMemberResultStatement = new cmajor::ast::ReturnStatementNode(memberVariableSymbol->GetSpan(),
                new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSpan(), U"memberResult"));
            cmajor::ast::IfStatementNode* ifStatementNode = new cmajor::ast::IfStatementNode(memberVariableSymbol->GetSpan(),
                invokeMemberResultErrorCond, returnMemberResultStatement, nullptr);
            compoundStatementNode.AddStatement(ifStatementNode);
        }

        // return System.Result<bool>(true);
        cmajor::ast::TemplateIdNode* resultBool = new cmajor::ast::TemplateIdNode(span, new cmajor::ast::IdentifierNode(span, U"System.Result"));
        resultBool->AddTemplateArgument(new cmajor::ast::BoolNode(span));
        cmajor::ast::InvokeNode* invokeResultBoolTrue = new cmajor::ast::InvokeNode(span, resultBool);
        invokeResultBoolTrue->AddArgument(new cmajor::ast::BooleanLiteralNode(span, true));
        cmajor::ast::ReturnStatementNode* returnStatmentNode = new cmajor::ast::ReturnStatementNode(span, invokeResultBoolTrue);
        compoundStatementNode.AddStatement(returnStatmentNode);

        CompileMemberFunction(fromXmlSymbol, compoundStatementNode, memberFunctionNode, std::move(boundFunction), statementBinder);
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<soul::ast::FullSpan> references;
        references.push_back(ex.Defined());
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in XML attribute generation: could not create 'FromXml' function for the class '" + 
            util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(), classTypeSymbol->GetFullSpan(), references);
    }
}

void XmlAttributeProcessor::CompileMemberFunction(cmajor::symbols::MemberFunctionSymbol* memberFunctionSymbol, cmajor::ast::CompoundStatementNode& compoundStatementNode, 
    cmajor::ast::MemberFunctionNode& memberFunctionNode, std::unique_ptr<BoundFunction>&& boundFunction, StatementBinder* statementBinder)
{
    cmajor::symbols::SymbolTable& symbolTable = statementBinder->GetBoundCompileUnit().GetSymbolTable();
    symbolTable.BeginContainer(memberFunctionSymbol);
    cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable, statementBinder->GetBoundCompileUnit().GetContext());
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
    BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
    boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
    statementBinder->CurrentClass()->AddMember(std::move(boundFunction));
    statementBinder->SetCurrentMemberFunction(prevMemberFunctionSymbol, prevMemberFunctionNode);
    statementBinder->SetCurrentFunction(prevFunction);
    statementBinder->GetBoundCompileUnit().RemoveLastFileScope();
}

} // namespace cmajor::binder
