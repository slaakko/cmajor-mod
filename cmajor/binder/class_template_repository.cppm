// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.class_template_repository;

import std.core;
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
        cmajor::symbols::ContainerScope* containerScope, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    void BindClassTemplateSpecialization(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization, cmajor::symbols::ContainerScope* containerScope, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    bool Instantiate(cmajor::symbols::FunctionSymbol* memberFunction, cmajor::symbols::ContainerScope* containerScope, BoundFunction* currentFunction, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
    void InstantiateAll(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization, cmajor::symbols::ContainerScope* containerScope, 
        BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos,
        const util::uuid& moduleId);
private:
    BoundCompileUnit& boundCompileUnit;
    std::unordered_set<cmajor::symbols::FunctionSymbol*> instantiatedMemberFunctions;
    std::unordered_set<std::pair<util::uuid, int>, ClassIdMemberFunctionIndexHash> classIdMemberFunctionIndexSet;
    bool InstantiateDestructorAndVirtualFunctions(cmajor::symbols::ClassTemplateSpecializationSymbol* classTemplateSpecialization, cmajor::symbols::ContainerScope* containerScope, 
        BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
};

} // namespace cmajor::binder
