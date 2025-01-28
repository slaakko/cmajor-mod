// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.class_template_repository;

import std.core;
import cmajor.ast;
import cmajor.symbols;
import soul.ast.source.pos;
import util;

export namespace cmajor::binder {

class BoundCompileUnit;
class BoundFunction;

struct ClassIdMemberFunctionIndexHash
{
    size_t operator()(const std::pair<util::uuid, int>& p) const;
};

class ClassTemplateRepository
{
public:
    ClassTemplateRepository(BoundCompileUnit& boundCompileUnit_);
    void ResolveDefaultTemplateArguments(std::vector<cmajor::symbols::TypeSymbol*>& templateArgumentTypes, cmajor::symbols::ClassTypeSymbol* classTemplate, 
        cmajor::symbols::ContainerScope* containerScope, cmajor::ast::Node* node);
    void BindClassTemplateSpecialization(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization, cmajor::symbols::ContainerScope* containerScope, 
        cmajor::ast::Node* node);
    cmajor::symbols::FunctionSymbol* Instantiate(cmajor::symbols::FunctionSymbol* memberFunction, cmajor::symbols::ContainerScope* containerScope,
        BoundFunction* currentFunction, cmajor::ast::Node* node);
    void InstantiateAll(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization, cmajor::symbols::ContainerScope* containerScope, 
        BoundFunction* currentFunction, cmajor::ast::Node* node);
private:
    BoundCompileUnit& boundCompileUnit;
    std::unordered_set<cmajor::symbols::FunctionSymbol*> instantiatedMemberFunctions;
    std::unordered_set<std::pair<util::uuid, int>, ClassIdMemberFunctionIndexHash> classIdMemberFunctionIndexSet;
    std::map<cmajor::symbols::FunctionSymbol*, cmajor::symbols::FunctionSymbol*> copyMap;
    bool InstantiateDestructorAndVirtualFunctions(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization, cmajor::symbols::ContainerScope* containerScope, 
        BoundFunction* currentFunction, cmajor::ast::Node* node);
    cmajor::symbols::FunctionSymbol* GetCopy(cmajor::symbols::FunctionSymbol* master) const;
};

} // namespace cmajor::binder
