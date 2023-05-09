// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.json.attribute.processor;

import cmajor.binder.attribute.binder;
import std.core;
import cmajor.symbols;

export namespace cmajor::binder {

class JsonAttributeProcessor : public AttributeProcessor
{
public:
    JsonAttributeProcessor(cmajor::symbols::Module* module_);
    void TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol) override;
    void GenerateSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope) override;
    void GenerateImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, StatementBinder* statementBinder) override;
private:
    cmajor::symbols::Module* module;
    std::unordered_map<cmajor::symbols::Symbol*, cmajor::symbols::MemberFunctionSymbol*> jsonCreatorMap;
    std::unordered_map<cmajor::symbols::Symbol*, cmajor::symbols::ConstructorSymbol*> jsonConstructorMap;
    std::unordered_map<cmajor::symbols::Symbol*, cmajor::symbols::MemberFunctionSymbol*> toJsonJsonObjectMemberFunctionSymbolMap;
    std::unordered_map<cmajor::symbols::Symbol*, cmajor::symbols::MemberFunctionSymbol*> toJsonObjectMemberFunctionSymbolMap;
    void GenerateMemberVariableJsonFieldNames(cmajor::symbols::ClassTypeSymbol* classTypeSymbol);
    void CheckMemberVariableJsonFieldNames(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, std::map<std::u32string, cmajor::ast::AttributeNode*>& memberVariableFieldNames);
    void GenerateJsonCreatorFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol);
    void GenerateJsonConstructorSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol);
    void GenerateToJsonJsonObjectSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol);
    void GenerateToJsonSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateJsonConstructorImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::ConstructorSymbol* jsonConstructorSymbol, 
        StatementBinder* statementBinder);
    void GenerateJsonCreatorImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* jsonCreatorFunctionSymbol, StatementBinder* statementBinder);
    void GenerateToJsonJsonObjectImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* toJsonJsonObjectMemberFunctionSymbol, StatementBinder* statementBinder);
    void GenerateToJsonImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, cmajor::symbols::MemberFunctionSymbol* toJsonMemberFunctionSymbol, 
        StatementBinder* statementBinder);
};

class JsonFieldNameAttributeProcessor : public AttributeProcessor
{
public:
    JsonFieldNameAttributeProcessor();
    void TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol) override;
};

} // namespace cmajor::binder
