// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.attribute.binder;

import std.core;
import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::binder {

class BoundCompileUnit;

class StatementBinder;

class AttributeProcessor
{
public:
    AttributeProcessor(const std::u32string& attributeName_);
    virtual ~AttributeProcessor();
    const std::u32string& AttributeName() const { return attributeName; }
    virtual void TypeCheck(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol);
    virtual void GenerateSymbols(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, BoundCompileUnit& boundCompileUnit, 
        cmajor::symbols::ContainerScope* containerScope);
    virtual void GenerateImplementation(cmajor::ast::AttributeNode* attribute, cmajor::symbols::Symbol* symbol, StatementBinder* statementBinder);
private:
    std::u32string attributeName;
};

class AttributeBinder
{
public:
    AttributeBinder(cmajor::symbols::Module* module);
    AttributeBinder(const AttributeBinder&) = delete;
    AttributeBinder& operator=(const AttributeBinder&) = delete;
    void BindAttributes(cmajor::ast::AttributesNode* attrs, cmajor::symbols::Symbol* symbol, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope);
    void GenerateImplementation(cmajor::ast::AttributesNode* attrs, cmajor::symbols::Symbol* symbol, StatementBinder* statementBinder);
private:
    std::vector<std::unique_ptr<AttributeProcessor>> attributeProcessors;
    std::unordered_map<std::u32string, AttributeProcessor*> attributeProcessorMap;
};

} // namespace cmajor::binder
