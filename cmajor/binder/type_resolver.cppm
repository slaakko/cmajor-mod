// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.binder.type.resolver;

import std.core;
import cmajor.symbols;
import cmajor.ast;
import cmajor.ir;

export namespace cmajor::binder {

class BoundCompileUnit;

class NamespaceTypeSymbol : public cmajor::symbols::TypeSymbol
{
public:
    NamespaceTypeSymbol(cmajor::symbols::NamespaceSymbol* ns_);
    bool IsInComplete() const override { return true; }
    const cmajor::symbols::NamespaceSymbol* Ns() const { return ns; }
    cmajor::symbols::NamespaceSymbol* Ns() { return ns; }
    void* IrType(cmajor::ir::Emitter& emitter) override 
    { 
        Assert(false, "tried to get ir type of namespace type");  
        return nullptr; 
    }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override 
    { 
        Assert(false, "tried to create default ir value of namespace type");
        return nullptr; 
    }
    const char* ClassName() const override { return "NamespaceTypeSymbol"; }
private:
    cmajor::symbols::NamespaceSymbol* ns;
};

enum class TypeResolverFlags : uint8_t
{
    none = 0,
    resolveClassGroup = 1 << 0,
    createMemberSymbols = 1 << 1
};

inline TypeResolverFlags operator&(TypeResolverFlags left, TypeResolverFlags right)
{
    return TypeResolverFlags(uint8_t(left) & uint8_t(right));
}

inline TypeResolverFlags operator|(TypeResolverFlags left, TypeResolverFlags right)
{
    return TypeResolverFlags(uint8_t(left) | uint8_t(right));
}

cmajor::symbols::TypeSymbol* ResolveType(cmajor::ast::Node* typeExprNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope);
cmajor::symbols::TypeSymbol* ResolveType(cmajor::ast::Node* typeExprNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope,
    cmajor::symbols::ClassTypeSymbol* currentClass);
cmajor::symbols::TypeSymbol* ResolveType(cmajor::ast::Node* typeExprNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope,
    TypeResolverFlags flags);
cmajor::symbols::TypeSymbol* ResolveType(cmajor::ast::Node* typeExprNode, BoundCompileUnit& boundCompileUnit, cmajor::symbols::ContainerScope* containerScope,
    TypeResolverFlags flags, cmajor::symbols::ClassTypeSymbol* currentClass);

} // namespace cmajor::binder
