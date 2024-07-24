// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.xml.attribute.processor;

import std.core;
import cmajor.binder.attribute.binder;
import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::binder {

class BoundClass;
class BoundFunction;

class XmlAttributeProcessor : public AttributeProcessor
{
public:
    XmlAttributeProcessor();
    void TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol) override;
    void GenerateSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope) override;
    void GenerateImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, StatementBinder* statementBinder) override;
private:
    void TypeCheckClass(cmajor::symbols::ClassTypeSymbol* classType);
    void SetBaseClass(cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit,
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateMemberSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classType, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    //void CheckXmlSerializableInterface(cmajor::symbols::ClassTypeSymbol* classType, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope);
    //void CheckVirtualDestructor(cmajor::symbols::ClassTypeSymbol* classTypeSymbol);
    void GenerateDestructorImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::DestructorSymbol* destructorSymbol, StatementBinder* statementBinder);
    void GenerateMemberVariableSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classType, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateStaticClassNameSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateStaticClassNameImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* staticClassNameSymbol, StatementBinder* statementBinder);
    void GenerateCreateFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateCreateImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* createFunctionSymbol, StatementBinder* statementBinder);
    void GenerateRegisterFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateRegisterImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* registerSymbol, StatementBinder* statementBinder);
/*
    void GenerateDestroyObjectFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateDestroyObjectImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* destroyObjectSymbol, StatementBinder* statementBinder);
    void GenerateObjectIdFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateObjectIdImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* objectIdSymbol, StatementBinder* statementBinder);
    void GenerateSetObjectIdFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateSetObjectIdImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* setObjectIdSymbol, StatementBinder* statementBinder);
    void GenerateContainerFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateContainerImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* containerSymbol, StatementBinder* statementBinder);
    void GenerateSetContainerFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateSetContainerImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* setContainerSymbol, StatementBinder* statementBinder);
*/
    void GenerateClassIdFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateClassIdImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* classIdSymbol, StatementBinder* statementBinder);
/*
    void GenerateClassNameFunctionSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateClassNameImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* classNameSymbol, StatementBinder* statementBinder);
*/
    void GenerateSetObjectXmlAttributesSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateSetObjectXmlAttributesImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* setObjectXmlAttributesSymbol, StatementBinder* statementBinder);
/*
    void GenerateToXmlPlainSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateToXmlPlainImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* toXmlSymbol, StatementBinder* statementBinder);
*/
    void GenerateToXmlSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateToXmlImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* toXmlSymbol, StatementBinder* statementBinder);
    void GenerateFromXmlSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateFromXmlImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* fromXmlSymbol, StatementBinder* statementBinder);
/*
    void GenerateGetPtrsSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateGetPtrsImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* getPtrsSymbol, StatementBinder* statementBinder);
    void GenerateIsOwnedSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateIsOwnedImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* isOwnedMemFunSymbol, StatementBinder* statementBinder);
    void GenerateSetOwnedSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateSetOwnedImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* setOwnedMemFunSymbol, StatementBinder* statementBinder);
    void GenerateResetOwnedSymbol(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    void GenerateResetOwnedImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::ClassTypeSymbol* classTypeSymbol, 
        cmajor::symbols::MemberFunctionSymbol* setOwnedMemFunSymbol, StatementBinder* statementBinder);
*/
    bool HasXmlBaseClass(cmajor::symbols::ClassTypeSymbol* classType) const;
    void CompileMemberFunction(cmajor::symbols::MemberFunctionSymbol* memberFunctionSymbol, cmajor::ast::CompoundStatementNode& compoundStatementNode, 
        cmajor::ast::MemberFunctionNode& memberFunctionNode, std::unique_ptr<BoundFunction>&& boundFunction, StatementBinder* statementBinder);
    std::set<std::u32string> reservedMemberFunctionNames;
    std::set<std::u32string> reservedMemberVariableNames;
    std::map<cmajor::symbols::Symbol*, std::vector<std::pair<cmajor::symbols::FunctionSymbol*, int>>> functionSymbolMap;
};

} // namespace cmajor::binder
