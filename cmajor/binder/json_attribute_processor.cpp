// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.binder.json.attribute.processor;

import cmajor.binder.statement.binder;
import cmajor.binder.type.binder;
import cmajor.binder.bound.statement;
import cmajor.binder.bound.function;
import cmajor.binder.bound_class;
import cmajor.binder.type.resolver;
import cmajor.binder.bound.compile.unit;
import cmajor.binder.bound.expression;
import cmajor.ast;
import util;

namespace cmajor::binder {

JsonAttributeProcessor::JsonAttributeProcessor(cmajor::symbols::Module* module_) : AttributeProcessor(U"json"), module(module_)
{
}

void JsonAttributeProcessor::TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol)
{
    switch (symbol->GetSymbolType())
    {
    case cmajor::symbols::SymbolType::classTypeSymbol: case cmajor::symbols::SymbolType::classTemplateSpecializationSymbol: case cmajor::symbols::SymbolType::memberVariableSymbol:
    {
        if (attribute->Value() == U"true" || attribute->Value() == U"false")
        {
            if (symbol->IsClassTypeSymbol())
            {
                cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol);
                cmajor::symbols::ClassTypeSymbol* baseClass = classTypeSymbol->BaseClass();
                if (baseClass)
                {
                    bool baseClassHasJsonAttribute = false;
                    cmajor::ast::AttributesNode* baseClassAttributes = baseClass->GetAttributes();
                    if (baseClassAttributes)
                    {
                        cmajor::ast::AttributeNode* jsonAttribute = baseClassAttributes->GetAttribute(U"json");
                        if (jsonAttribute)
                        {
                            baseClassHasJsonAttribute = true;
                        }
                    }
                    if (!baseClassHasJsonAttribute)
                    {
                        cmajor::symbols::Warning warning(module->GetCurrentProjectName(), "base class '" + util::ToUtf8(baseClass->FullName()) + "' of json-attributed class '" +
                            util::ToUtf8(classTypeSymbol->FullName()) + "' does not explicitly declare 'json' attribute value to \"true\" or \"false\"");
                        warning.SetDefined(classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
                        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
                        references.push_back(std::make_pair(baseClass->GetSourcePos(), baseClass->SourceModuleId()));
                        warning.SetReferences(references);
                        module->WarningCollection().AddWarning(warning);
                    }
                }
            }
            return;
        }
        else
        {
            throw cmajor::symbols::Exception("unknown attribute value '" + util::ToUtf8(attribute->Value()) + "' for attribute '" + util::ToUtf8(attribute->Name()) + "'", attribute->GetSourcePos(), attribute->ModuleId());
        }
    }
    }
    AttributeProcessor::TypeCheck(attribute, symbol);
}

void JsonAttributeProcessor::GenerateSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    if (symbol->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol);
        if (attribute->Value() == U"true")
        {
            GenerateMemberVariableJsonFieldNames(classTypeSymbol);
            std::map<std::u32string, cmajor::ast::AttributeNode*> memberVariableFieldNames;
            CheckMemberVariableJsonFieldNames(classTypeSymbol, memberVariableFieldNames);
            GenerateJsonCreatorFunctionSymbol(attribute, classTypeSymbol);
            GenerateJsonConstructorSymbol(attribute, classTypeSymbol);
            GenerateToJsonJsonObjectSymbol(attribute, classTypeSymbol);
            GenerateToJsonSymbol(attribute, classTypeSymbol, boundCompileUnit, containerScope);
        }
    }
}

void JsonAttributeProcessor::CheckMemberVariableJsonFieldNames(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, std::map<std::u32string, cmajor::ast::AttributeNode*>& memberVariableFieldNames)
{
    if (classTypeSymbol->BaseClass())
    {
        cmajor::ast::AttributesNode* attributes = classTypeSymbol->BaseClass()->GetAttributes();
        cmajor::ast::AttributeNode* jsonAttribute = attributes->GetAttribute(U"json");
        if (jsonAttribute && jsonAttribute->Value() != U"false")
        {
            CheckMemberVariableJsonFieldNames(classTypeSymbol->BaseClass(), memberVariableFieldNames);
        }
    }
    for (cmajor::symbols::MemberVariableSymbol* memberVariableSymbol : classTypeSymbol->MemberVariables())
    {
        cmajor::ast::AttributesNode* attributes = memberVariableSymbol->GetAttributes();
        if (attributes)
        {
            cmajor::ast::AttributeNode* jsonAttribute = attributes->GetAttribute(U"json");
            if (jsonAttribute)
            {
                if (jsonAttribute->Value() == U"false")
                {
                    continue;
                }
            }
            cmajor::ast::AttributeNode* jsonFieldNameAttribute = attributes->GetAttribute(U"jsonFieldName");
            if (jsonFieldNameAttribute)
            {
                auto it = memberVariableFieldNames.find(jsonFieldNameAttribute->Value());
                if (it != memberVariableFieldNames.cend())
                {
                    cmajor::ast::AttributeNode* prev = it->second;
                    throw cmajor::symbols::Exception("error in JSON field name generation: 'jsonFieldName' attribute not unique among member variable names of the current class and its base classes",
                        jsonFieldNameAttribute->GetSourcePos(), jsonFieldNameAttribute->ModuleId(), prev->GetSourcePos(), prev->ModuleId());
                }
                memberVariableFieldNames[jsonFieldNameAttribute->Value()] = jsonFieldNameAttribute;
            }
            else
            {
                throw cmajor::symbols::Exception("internal error in JSON field name generation: 'jsonFieldName' attribute not found", memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId());
            }
        }
        else
        {
            throw cmajor::symbols::Exception("internal error in JSON field name generation: attributes not found", memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId());
        }
    }
}

void JsonAttributeProcessor::GenerateMemberVariableJsonFieldNames(cmajor::symbols::ClassTypeSymbol* classTypeSymbol)
{
    for (cmajor::symbols::MemberVariableSymbol* memberVariableSymbol : classTypeSymbol->MemberVariables())
    {
        bool hasJsonFieldAttribute = false;
        cmajor::ast::AttributesNode* attributes = memberVariableSymbol->GetAttributes();
        if (attributes)
        {
            cmajor::ast::AttributeNode* jsonAttribute = attributes->GetAttribute(U"json");
            if (jsonAttribute)
            {
                if (jsonAttribute->Value() == U"false")
                {
                    continue;
                }
            }
            cmajor::ast::AttributeNode* jsonFieldNameAttribute = attributes->GetAttribute(U"jsonFieldName");
            if (jsonFieldNameAttribute)
            {
                hasJsonFieldAttribute = true;
            }
        }
        else
        {
            memberVariableSymbol->SetAttributes(std::unique_ptr<cmajor::ast::AttributesNode>(new cmajor::ast::AttributesNode(classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId())));
            attributes = memberVariableSymbol->GetAttributes();
        }
        if (!hasJsonFieldAttribute)
        {
            attributes->AddAttribute(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), U"jsonFieldName", memberVariableSymbol->Name());
        }
    }
}

void JsonAttributeProcessor::GenerateJsonCreatorFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol)
{
    cmajor::symbols::MemberFunctionSymbol* jsonCreatorFunctionSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"Create");
    jsonCreatorFunctionSymbol->SetGroupName(U"Create");
    jsonCreatorFunctionSymbol->SetModule(module);
    module->GetSymbolTable().SetFunctionIdFor(jsonCreatorFunctionSymbol);
    jsonCreatorFunctionSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    jsonCreatorFunctionSymbol->SetStatic();
    cmajor::symbols::Symbol* jsonValue = classTypeSymbol->GetModule()->GetSymbolTable().GlobalNs().GetContainerScope()->Lookup(U"System.Json.JsonValue");
    if (!jsonValue || jsonValue->GetSymbolType() != cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        throw cmajor::symbols::Exception("System.Json.JsonValue class not found from the symbol table", attribute->GetSourcePos(), attribute->ModuleId(), classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
    }
    cmajor::symbols::ClassGroupTypeSymbol* jsonValueGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(jsonValue);
    cmajor::symbols::ClassTypeSymbol* jsonValueClass = jsonValueGroup->GetClass(0);
    if (!jsonValueClass)
    {
        throw cmajor::symbols::Exception("System.Json.JsonValue class not found from the symbol table", attribute->GetSourcePos(), attribute->ModuleId(), classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
    }
    cmajor::symbols::ParameterSymbol* jsonValueParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"@value");
    jsonValueParam->SetType(jsonValueClass->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    jsonCreatorFunctionSymbol->AddMember(jsonValueParam);
    jsonCreatorFunctionSymbol->SetReturnType(module->GetSymbolTable().GetTypeByName(U"void")->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    classTypeSymbol->AddMember(jsonCreatorFunctionSymbol);
    jsonCreatorFunctionSymbol->ComputeName();
    jsonCreatorMap[classTypeSymbol] = jsonCreatorFunctionSymbol;
}

void JsonAttributeProcessor::GenerateJsonConstructorSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol)
{
    cmajor::symbols::ConstructorSymbol* jsonConstructorSymbol = new cmajor::symbols::ConstructorSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"@constructor");
    jsonConstructorSymbol->SetModule(module);
    module->GetSymbolTable().SetFunctionIdFor(jsonConstructorSymbol);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    cmajor::symbols::ParameterSymbol* jsonValueParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"@value");
    cmajor::symbols::Symbol* jsonValue = classTypeSymbol->GetModule()->GetSymbolTable().GlobalNs().GetContainerScope()->Lookup(U"System.Json.JsonValue");
    if (!jsonValue || jsonValue->GetSymbolType() != cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        throw cmajor::symbols::Exception("System.Json.JsonValue class not found from the symbol table", attribute->GetSourcePos(), attribute->ModuleId(), classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
    }
    cmajor::symbols::ClassGroupTypeSymbol* jsonValueGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(jsonValue);
    cmajor::symbols::ClassTypeSymbol* jsonValueClass = jsonValueGroup->GetClass(0);
    if (!jsonValueClass)
    {
        throw cmajor::symbols::Exception("System.Json.JsonValue class not found from the symbol table", attribute->GetSourcePos(), attribute->ModuleId(), classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
    }
    jsonValueParam->SetType(jsonValueClass->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    jsonConstructorSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    jsonConstructorSymbol->SetExplicit();
    jsonConstructorSymbol->AddMember(thisParam);
    jsonConstructorSymbol->AddMember(jsonValueParam);
    classTypeSymbol->AddMember(jsonConstructorSymbol);
    jsonConstructorSymbol->ComputeName();
    jsonConstructorMap[classTypeSymbol] = jsonConstructorSymbol;
}

void JsonAttributeProcessor::GenerateToJsonJsonObjectSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol)
{
    cmajor::symbols::MemberFunctionSymbol* toJsonJsonObjectMemberFunctionSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"ToJson");
    toJsonJsonObjectMemberFunctionSymbol->SetModule(module);
    toJsonJsonObjectMemberFunctionSymbol->SetGroupName(U"ToJson");
    cmajor::symbols::ClassTypeSymbol* baseClass = classTypeSymbol->BaseClass();
    bool jsonBase = false;
    if (baseClass)
    {
        cmajor::ast::AttributesNode* attributes = baseClass->GetAttributes();
        if (attributes)
        {
            cmajor::ast::AttributeNode* jsonAttribute = attributes->GetAttribute(U"json");
            if (jsonAttribute)
            {
                if (jsonAttribute->Value() == U"true")
                {
                    jsonBase = true;
                }
            }
        }
    }
    if (!jsonBase)
    {
        toJsonJsonObjectMemberFunctionSymbol->SetVirtual();
    }
    else
    {
        toJsonJsonObjectMemberFunctionSymbol->SetOverride();
    }
    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(toJsonJsonObjectMemberFunctionSymbol);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    cmajor::symbols::ParameterSymbol* jsonObjectParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"@object");
    cmajor::symbols::Symbol* jsonObject = cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().GlobalNs().GetContainerScope()->Lookup(U"System.Json.JsonObject");
    if (!jsonObject || jsonObject->GetSymbolType() != cmajor::symbols::SymbolType::classGroupTypeSymbol)
    {
        throw cmajor::symbols::Exception("System.Json.JsonObject class not found from the symbol table", attribute->GetSourcePos(), attribute->ModuleId(), classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
    }
    cmajor::symbols::ClassGroupTypeSymbol* jsonObjectGroup = static_cast<cmajor::symbols::ClassGroupTypeSymbol*>(jsonObject);
    cmajor::symbols::ClassTypeSymbol* jsonObjectClass = jsonObjectGroup->GetClass(0);
    if (!jsonObjectClass)
    {
        throw cmajor::symbols::Exception("System.Json.JsonObject class not found from the symbol table", attribute->GetSourcePos(), attribute->ModuleId(), classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId());
    }
    jsonObjectParam->SetType(jsonObjectClass->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    toJsonJsonObjectMemberFunctionSymbol->SetReturnType(module->GetSymbolTable().GetTypeByName(U"void"));
    toJsonJsonObjectMemberFunctionSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    toJsonJsonObjectMemberFunctionSymbol->AddMember(thisParam);
    toJsonJsonObjectMemberFunctionSymbol->AddMember(jsonObjectParam);
    classTypeSymbol->AddMember(toJsonJsonObjectMemberFunctionSymbol);
    toJsonJsonObjectMemberFunctionSymbol->ComputeName();
    toJsonJsonObjectMemberFunctionSymbolMap[classTypeSymbol] = toJsonJsonObjectMemberFunctionSymbol;
}

void JsonAttributeProcessor::GenerateToJsonSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope)
{
    cmajor::symbols::MemberFunctionSymbol* toJsonMemberFunctionSymbol = new cmajor::symbols::MemberFunctionSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"ToJson");
    toJsonMemberFunctionSymbol->SetModule(module);
    toJsonMemberFunctionSymbol->SetGroupName(U"ToJson");
    cmajor::symbols::ClassTypeSymbol* baseClass = classTypeSymbol->BaseClass();
    bool jsonBase = false;
    if (baseClass)
    {
        cmajor::ast::AttributesNode* attributes = baseClass->GetAttributes();
        if (attributes)
        {
            cmajor::ast::AttributeNode* jsonAttribute = attributes->GetAttribute(U"json");
            if (jsonAttribute)
            {
                if (jsonAttribute->Value() == U"true")
                {
                    jsonBase = true;
                }
            }
        }
    }
    if (!jsonBase)
    {
        toJsonMemberFunctionSymbol->SetVirtual();
    }
    else
    {
        toJsonMemberFunctionSymbol->SetOverride();
    }
    cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().SetFunctionIdFor(toJsonMemberFunctionSymbol);
    cmajor::symbols::ParameterSymbol* thisParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"this");
    thisParam->SetType(classTypeSymbol->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    cmajor::ast::TemplateIdNode templateId(attribute->GetSourcePos(), attribute->ModuleId(), new cmajor::ast::IdentifierNode(attribute->GetSourcePos(), attribute->ModuleId(), U"System.UniquePtr"));
    templateId.AddTemplateArgument(new cmajor::ast::IdentifierNode(attribute->GetSourcePos(), attribute->ModuleId(), U"System.Json.JsonValue"));
    cmajor::symbols::TypeSymbol* uniquePtrJsonValueType = ResolveType(&templateId, boundCompileUnit, containerScope);
    toJsonMemberFunctionSymbol->SetReturnType(uniquePtrJsonValueType);
    cmajor::symbols::ParameterSymbol* returnParam = new cmajor::symbols::ParameterSymbol(attribute->GetSourcePos(), attribute->ModuleId(), U"@return");
    returnParam->SetParent(toJsonMemberFunctionSymbol);
    returnParam->SetType(uniquePtrJsonValueType->AddPointer(attribute->GetSourcePos(), attribute->ModuleId()));
    toJsonMemberFunctionSymbol->SetReturnParam(returnParam);
    toJsonMemberFunctionSymbol->SetAccess(cmajor::symbols::SymbolAccess::public_);
    toJsonMemberFunctionSymbol->AddMember(thisParam);
    classTypeSymbol->AddMember(toJsonMemberFunctionSymbol);
    toJsonMemberFunctionSymbol->ComputeName();
    toJsonObjectMemberFunctionSymbolMap[classTypeSymbol] = toJsonMemberFunctionSymbol;
}

void JsonAttributeProcessor::GenerateImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, StatementBinder* statementBinder)
{
    if (symbol->IsClassTypeSymbol())
    {
        cmajor::symbols::ClassTypeSymbol* classTypeSymbol = static_cast<cmajor::symbols::ClassTypeSymbol*>(symbol);
        auto it = jsonConstructorMap.find(symbol);
        if (it != jsonConstructorMap.cend())
        {
            cmajor::symbols::ConstructorSymbol* jsonConstructorSymbol = it->second;
            GenerateJsonConstructorImplementation(attribute, classTypeSymbol, jsonConstructorSymbol, statementBinder);
        }
        else
        {
            throw cmajor::symbols::Exception("internal error in JSON attribute implementation: constructor symbol for symbol '" + util::ToUtf8(symbol->FullName()) + "' not found", attribute->GetSourcePos(), attribute->ModuleId());
        }
        auto it1 = jsonCreatorMap.find(symbol);
        if (it1 != jsonCreatorMap.cend())
        {
            cmajor::symbols::MemberFunctionSymbol* jsonCreatorSymbol = it1->second;
            GenerateJsonCreatorImplementation(attribute, classTypeSymbol, jsonCreatorSymbol, statementBinder);
        }
        else
        {
            throw cmajor::symbols::Exception("internal error in JSON attribute implementation: Creator function symbol for symbol '" + util::ToUtf8(symbol->FullName()) + "' not found", attribute->GetSourcePos(), attribute->ModuleId());
        }
        auto it2 = toJsonJsonObjectMemberFunctionSymbolMap.find(symbol);
        if (it2 != toJsonJsonObjectMemberFunctionSymbolMap.cend())
        {
            cmajor::symbols::MemberFunctionSymbol* toJsonJsonObjectMemberFunctionSymbol = it2->second;
            GenerateToJsonJsonObjectImplementation(attribute, classTypeSymbol, toJsonJsonObjectMemberFunctionSymbol, statementBinder);
        }
        else
        {
            throw cmajor::symbols::Exception("internal error in JSON attribute implementation: member function 'ToJson' symbol for symbol '" + util::ToUtf8(symbol->FullName()) + "' not found", attribute->GetSourcePos(), attribute->ModuleId());
        }
        auto it3 = toJsonObjectMemberFunctionSymbolMap.find(symbol);
        if (it3 != toJsonObjectMemberFunctionSymbolMap.cend())
        {
            cmajor::symbols::MemberFunctionSymbol* toJsonMemberFunctionSymbol = it3->second;
            GenerateToJsonImplementation(attribute, classTypeSymbol, toJsonMemberFunctionSymbol, statementBinder);
        }
        else
        {
            throw cmajor::symbols::Exception("internal error in JSON attribute implementation: member function 'ToJson' symbol for symbol '" + util::ToUtf8(symbol->FullName()) + "' not found", attribute->GetSourcePos(), attribute->ModuleId());
        }
        cmajor::symbols::SymbolTable& symbolTable = module->GetSymbolTable();
        symbolTable.AddJsonClass(classTypeSymbol->FullName());
    }
}

void JsonAttributeProcessor::GenerateJsonCreatorImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* jsonCreatorFunctionSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        cmajor::symbols::Symbol* jsonValue = classTypeSymbol->GetModule()->GetSymbolTable().GlobalNs().GetContainerScope()->Lookup(U"System.Json.JsonValue");
        if (jsonValue)
        {
            fileScope->AddContainerScope(jsonValue->Ns()->GetContainerScope());
        }
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), jsonCreatorFunctionSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        cmajor::ast::NewNode* newNode = new cmajor::ast::NewNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, classTypeSymbol->FullName()));
        newNode->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@value"));
        cmajor::ast::ReturnStatementNode* returnStatementNode = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, newNode);
        compoundStatementNode.AddStatement(returnStatementNode);
        cmajor::symbols::SymbolTable& symbolTable = statementBinder->GetBoundCompileUnit().GetSymbolTable();
        symbolTable.BeginContainer(jsonCreatorFunctionSymbol);
        cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
        compoundStatementNode.Accept(symbolCreatorVisitor);
        symbolTable.EndContainer();
        TypeBinder typeBinder(statementBinder->GetBoundCompileUnit());
        cmajor::symbols::ContainerScope* containerScope = statementBinder->GetContainerScope();
        typeBinder.SetContainerScope(containerScope);
        statementBinder->SetContainerScope(jsonCreatorFunctionSymbol->GetContainerScope());
        typeBinder.SetCurrentFunctionSymbol(jsonCreatorFunctionSymbol);
        compoundStatementNode.Accept(typeBinder);
        BoundFunction* prevFunction = statementBinder->CurrentFunction();
        statementBinder->SetCurrentFunction(boundFunction.get());
        compoundStatementNode.Accept(*statementBinder);
        statementBinder->SetContainerScope(containerScope);
        BoundStatement* boundStatement = statementBinder->ReleaseStatement();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        statementBinder->CurrentClass()->AddMember(std::move(boundFunction));
        statementBinder->SetCurrentFunction(prevFunction);
        statementBinder->GetBoundCompileUnit().RemoveLastFileScope();
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in JSON attribute generation: could not create JSON Create() function for class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void JsonAttributeProcessor::GenerateJsonConstructorImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::ConstructorSymbol* jsonConstructorSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        cmajor::symbols::Symbol* jsonValue = classTypeSymbol->GetModule()->GetSymbolTable().GlobalNs().GetContainerScope()->Lookup(U"System.Json.JsonValue");
        if (jsonValue)
        {
            fileScope->AddContainerScope(jsonValue->Ns()->GetContainerScope());
        }
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), jsonConstructorSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::ConstructorNode constructorNode(sourcePos, moduleId);
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        cmajor::symbols::ClassTypeSymbol* baseClass = classTypeSymbol->BaseClass();
        if (baseClass)
        {
            cmajor::ast::AttributesNode* attributes = baseClass->GetAttributes();
            if (attributes)
            {
                cmajor::ast::AttributeNode* jsonAttribute = attributes->GetAttribute(U"json");
                if (jsonAttribute)
                {
                    if (jsonAttribute->Value() == U"true")
                    {
                        cmajor::ast::BaseInitializerNode* baseInitializer = new cmajor::ast::BaseInitializerNode(sourcePos, moduleId);
                        baseInitializer->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@value"));
                        constructorNode.AddInitializer(baseInitializer);
                    }
                }
            }
        }
        for (cmajor::symbols::MemberVariableSymbol* memberVariableSymbol : classTypeSymbol->MemberVariables())
        {
            std::u32string jsonFieldName = memberVariableSymbol->Name();
            cmajor::ast::AttributesNode* attributes = memberVariableSymbol->GetAttributes();
            if (attributes)
            {
                cmajor::ast::AttributeNode* jsonAttribute = attributes->GetAttribute(U"json");
                if (jsonAttribute)
                {
                    if (jsonAttribute->Value() == U"false")
                    {
                        continue;
                    }
                }
                cmajor::ast::AttributeNode* jsonFieldNameAttribute = attributes->GetAttribute(U"jsonFieldName");
                if (jsonFieldNameAttribute)
                {
                    jsonFieldName = jsonFieldNameAttribute->Value();
                }
            }
            cmajor::ast::InvokeNode* invokeNode = new cmajor::ast::InvokeNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"FromJson"));
            invokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), U"@value"));
            invokeNode->AddArgument(new cmajor::ast::UStringLiteralNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), jsonFieldName));
            invokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), memberVariableSymbol->Name()));
            cmajor::ast::ExpressionStatementNode* fromJsonStatement = new cmajor::ast::ExpressionStatementNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), invokeNode);
            compoundStatementNode.AddStatement(fromJsonStatement);
        }
        cmajor::symbols::SymbolTable& symbolTable = statementBinder->GetBoundCompileUnit().GetSymbolTable();
        symbolTable.BeginContainer(jsonConstructorSymbol);
        cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
        compoundStatementNode.Accept(symbolCreatorVisitor);
        symbolTable.EndContainer();
        TypeBinder typeBinder(statementBinder->GetBoundCompileUnit());
        cmajor::symbols::ContainerScope* containerScope = statementBinder->GetContainerScope();
        typeBinder.SetContainerScope(containerScope);
        statementBinder->SetContainerScope(jsonConstructorSymbol->GetContainerScope());
        typeBinder.SetCurrentFunctionSymbol(jsonConstructorSymbol);
        compoundStatementNode.Accept(typeBinder);
        BoundFunction* prevFunction = statementBinder->CurrentFunction();
        statementBinder->SetCurrentFunction(boundFunction.get());
        cmajor::symbols::ConstructorSymbol* prevConstructorSymbol = statementBinder->CurrentConstructorSymbol();
        cmajor::ast::ConstructorNode* prevConstructorNode = statementBinder->CurrentConstructorNode();
        statementBinder->SetCurrentConstructor(jsonConstructorSymbol, &constructorNode);
        compoundStatementNode.Accept(*statementBinder);
        statementBinder->SetContainerScope(containerScope);
        BoundStatement* boundStatement = statementBinder->ReleaseStatement();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        statementBinder->CurrentClass()->AddMember(std::move(boundFunction));
        statementBinder->SetCurrentConstructor(prevConstructorSymbol, prevConstructorNode);
        statementBinder->SetCurrentFunction(prevFunction);
        statementBinder->GetBoundCompileUnit().RemoveLastFileScope();
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in JSON attribute generation: could not create JSON constructor for class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void JsonAttributeProcessor::GenerateToJsonJsonObjectImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* toJsonJsonObjectMemberFunctionSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        cmajor::symbols::Symbol* jsonObject = cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().GlobalNs().GetContainerScope()->Lookup(U"System.Json.JsonObject");
        if (jsonObject)
        {
            fileScope->AddContainerScope(jsonObject->Ns()->GetContainerScope());
        }
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), toJsonJsonObjectMemberFunctionSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        cmajor::symbols::ClassTypeSymbol* baseClass = classTypeSymbol->BaseClass();
        if (baseClass)
        {
            cmajor::ast::AttributesNode* attributes = baseClass->GetAttributes();
            if (attributes)
            {
                cmajor::ast::AttributeNode* jsonAttribute = attributes->GetAttribute(U"json");
                if (jsonAttribute)
                {
                    if (jsonAttribute->Value() == U"true")
                    {
                        cmajor::ast::BaseNode* baseNode = new cmajor::ast::BaseNode(sourcePos, moduleId);
                        cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, baseNode, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToJson"));
                        cmajor::ast::InvokeNode* toJsonInvokeNode = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
                        toJsonInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@object"));
                        cmajor::ast::ExpressionStatementNode* toJsonStatement = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, toJsonInvokeNode);
                        compoundStatementNode.AddStatement(toJsonStatement);
                    }
                }
            }
        }
        for (cmajor::symbols::MemberVariableSymbol* memberVariableSymbol : classTypeSymbol->MemberVariables())
        {
            std::u32string jsonFieldName = memberVariableSymbol->Name();
            cmajor::ast::AttributesNode* attributes = memberVariableSymbol->GetAttributes();
            if (attributes)
            {
                cmajor::ast::AttributeNode* jsonAttribute = attributes->GetAttribute(U"json");
                if (jsonAttribute)
                {
                    if (jsonAttribute->Value() == U"false")
                    {
                        continue;
                    }
                }
                cmajor::ast::AttributeNode* jsonFieldNameAttribute = attributes->GetAttribute(U"jsonFieldName");
                if (jsonFieldNameAttribute)
                {
                    jsonFieldName = jsonFieldNameAttribute->Value();
                }
            }
            cmajor::ast::InvokeNode* toJsonInvokeNode = new cmajor::ast::InvokeNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToJson"));
            toJsonInvokeNode->AddArgument(new cmajor::ast::IdentifierNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), memberVariableSymbol->Name()));
            cmajor::ast::ArrowNode* arrowNode = new cmajor::ast::ArrowNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@object"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"AddField"));
            cmajor::ast::InvokeNode* addFieldInvokeNode = new cmajor::ast::InvokeNode(sourcePos, moduleId, arrowNode);
            addFieldInvokeNode->AddArgument(new cmajor::ast::UStringLiteralNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), jsonFieldName));
            addFieldInvokeNode->AddArgument(toJsonInvokeNode);
            cmajor::ast::ExpressionStatementNode* addFieldStatement = new cmajor::ast::ExpressionStatementNode(memberVariableSymbol->GetSourcePos(), memberVariableSymbol->SourceModuleId(), addFieldInvokeNode);
            compoundStatementNode.AddStatement(addFieldStatement);
        }
        cmajor::symbols::SymbolTable& symbolTable = statementBinder->GetBoundCompileUnit().GetSymbolTable();
        symbolTable.BeginContainer(toJsonJsonObjectMemberFunctionSymbol);
        cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
        compoundStatementNode.Accept(symbolCreatorVisitor);
        symbolTable.EndContainer();
        TypeBinder typeBinder(statementBinder->GetBoundCompileUnit());
        cmajor::symbols::ContainerScope* containerScope = statementBinder->GetContainerScope();
        typeBinder.SetContainerScope(containerScope);
        statementBinder->SetContainerScope(toJsonJsonObjectMemberFunctionSymbol->GetContainerScope());
        typeBinder.SetCurrentFunctionSymbol(toJsonJsonObjectMemberFunctionSymbol);
        compoundStatementNode.Accept(typeBinder);
        BoundFunction* prevFunction = statementBinder->CurrentFunction();
        statementBinder->SetCurrentFunction(boundFunction.get());
        compoundStatementNode.Accept(*statementBinder);
        statementBinder->SetContainerScope(containerScope);
        BoundStatement* boundStatement = statementBinder->ReleaseStatement();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        statementBinder->CurrentClass()->AddMember(std::move(boundFunction));
        statementBinder->SetCurrentFunction(prevFunction);
        statementBinder->GetBoundCompileUnit().RemoveLastFileScope();
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in JSON attribute generation: could not create 'void ToJson(JsobObject*)' member function for class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

void JsonAttributeProcessor::GenerateToJsonImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* toJsonMemberFunctionSymbol, StatementBinder* statementBinder)
{
    try
    {
        cmajor::symbols::FileScope* fileScope = new cmajor::symbols::FileScope();
        cmajor::symbols::Symbol* jsonObject = cmajor::symbols::GetRootModuleForCurrentThread()->GetSymbolTable().GlobalNs().GetContainerScope()->Lookup(U"System.Json.JsonObject");
        if (jsonObject)
        {
            fileScope->AddContainerScope(jsonObject->Ns()->GetContainerScope());
        }
        statementBinder->GetBoundCompileUnit().AddFileScope(fileScope);
        std::unique_ptr<BoundFunction> boundFunction(new BoundFunction(&statementBinder->GetBoundCompileUnit(), toJsonMemberFunctionSymbol));
        soul::ast::SourcePos sourcePos = attribute->GetSourcePos();
        util::uuid moduleId = attribute->ModuleId();
        cmajor::ast::CompoundStatementNode compoundStatementNode(sourcePos, moduleId);
        cmajor::ast::TemplateIdNode* uniquePtrJsonObject = new cmajor::ast::TemplateIdNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"UniquePtr"));
        uniquePtrJsonObject->AddTemplateArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"JsonObject"));
        cmajor::ast::ConstructionStatementNode* constructJsonObjectStatement = new cmajor::ast::ConstructionStatementNode(sourcePos, moduleId, uniquePtrJsonObject, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@object"));
        constructJsonObjectStatement->AddArgument(new cmajor::ast::NewNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"JsonObject")));
        compoundStatementNode.AddStatement(constructJsonObjectStatement);
        cmajor::ast::InvokeNode* invokeToJson = new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"ToJson"));
        invokeToJson->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@object"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Get"))));
        cmajor::ast::ExpressionStatementNode* callToJsonStatement = new cmajor::ast::ExpressionStatementNode(sourcePos, moduleId, invokeToJson);
        compoundStatementNode.AddStatement(callToJsonStatement);
        cmajor::ast::TemplateIdNode* uniquePtrJsonValue = new cmajor::ast::TemplateIdNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"UniquePtr"));
        uniquePtrJsonValue->AddTemplateArgument(new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"JsonValue"));
        cmajor::ast::InvokeNode* invokeJsonValue = new cmajor::ast::InvokeNode(sourcePos, moduleId, uniquePtrJsonValue);
        invokeJsonValue->AddArgument(new cmajor::ast::InvokeNode(sourcePos, moduleId, new cmajor::ast::DotNode(sourcePos, moduleId, new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"@object"), new cmajor::ast::IdentifierNode(sourcePos, moduleId, U"Release"))));
        cmajor::ast::ReturnStatementNode* returnStatement = new cmajor::ast::ReturnStatementNode(sourcePos, moduleId, invokeJsonValue);
        compoundStatementNode.AddStatement(returnStatement);
        cmajor::symbols::SymbolTable& symbolTable = statementBinder->GetBoundCompileUnit().GetSymbolTable();
        symbolTable.BeginContainer(toJsonMemberFunctionSymbol);
        cmajor::symbols::SymbolCreatorVisitor symbolCreatorVisitor(symbolTable);
        compoundStatementNode.Accept(symbolCreatorVisitor);
        symbolTable.EndContainer();
        TypeBinder typeBinder(statementBinder->GetBoundCompileUnit());
        cmajor::symbols::ContainerScope* containerScope = statementBinder->GetContainerScope();
        typeBinder.SetContainerScope(containerScope);
        statementBinder->SetContainerScope(toJsonMemberFunctionSymbol->GetContainerScope());
        typeBinder.SetCurrentFunctionSymbol(toJsonMemberFunctionSymbol);
        compoundStatementNode.Accept(typeBinder);
        BoundFunction* prevFunction = statementBinder->CurrentFunction();
        statementBinder->SetCurrentFunction(boundFunction.get());
        compoundStatementNode.Accept(*statementBinder);
        statementBinder->SetContainerScope(containerScope);
        BoundStatement* boundStatement = statementBinder->ReleaseStatement();
        Assert(boundStatement->GetBoundNodeType() == BoundNodeType::boundCompoundStatement, "bound compound statement expected");
        BoundCompoundStatement* compoundStatement = static_cast<BoundCompoundStatement*>(boundStatement);
        boundFunction->SetBody(std::unique_ptr<BoundCompoundStatement>(compoundStatement));
        statementBinder->CurrentClass()->AddMember(std::move(boundFunction));
        statementBinder->SetCurrentFunction(prevFunction);
        statementBinder->GetBoundCompileUnit().RemoveLastFileScope();
    }
    catch (const cmajor::symbols::Exception& ex)
    {
        std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
        references.push_back(std::make_pair(ex.Defined(), ex.DefinedModuleId()));
        references.insert(references.end(), ex.References().begin(), ex.References().end());
        throw cmajor::symbols::Exception("error in JSON attribute generation: could not create 'UniquePtr<JsonValue> ToJson()' member function for class '" + util::ToUtf8(classTypeSymbol->FullName()) + "': " + ex.Message(),
            classTypeSymbol->GetSourcePos(), classTypeSymbol->SourceModuleId(), references);
    }
}

JsonFieldNameAttributeProcessor::JsonFieldNameAttributeProcessor() : AttributeProcessor(U"jsonFieldName")
{
}

void JsonFieldNameAttributeProcessor::TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol)
{
    if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::memberVariableSymbol)
    {
        if (attribute->Value().empty())
        {
            throw cmajor::symbols::Exception("attribute value '" + util::ToUtf8(attribute->Value()) + "' for attribute '" + util::ToUtf8(attribute->Name()) + "' cannot be empty string", attribute->GetSourcePos(), attribute->ModuleId());
        }
        return;
    }
    AttributeProcessor::TypeCheck(attribute, symbol);
}

} // namespace cmajor::binder
