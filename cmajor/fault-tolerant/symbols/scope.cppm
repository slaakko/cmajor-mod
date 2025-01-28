// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.scope;

import cmajor.fault.tolerant.ast;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class Symbol;
class ContainerSymbol;
class TypeSymbol;
class SymbolTable;

enum class ScopeLookup
{
    none = 0,
    this_ = 1 << 0,
    base = 1 << 1,
    parent = 1 << 2,
    this_and_base = this_ | base,
    this_and_parent = this_ | parent,
    this_and_base_and_parent = this_ | base | parent,
    fileScopes = 1 << 3,
    map = 1 << 4
};

constexpr ScopeLookup operator|(ScopeLookup left, ScopeLookup right)
{
    return ScopeLookup(int(left) | int(right));
}

constexpr ScopeLookup operator&(ScopeLookup left, ScopeLookup right)
{
    return ScopeLookup(int(left) & int(right));
}

constexpr ScopeLookup operator~(ScopeLookup lookup)
{
    return ScopeLookup(~int(lookup));
}

class Scope
{
public:
    virtual ~Scope();
    virtual Symbol* Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, SymbolTable* symbolTable) const = 0;
    virtual Symbol* Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, ScopeLookup scopeLookup, SymbolTable* symbolTable) const = 0;
    virtual Symbol* Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, SymbolTable* symbolTable) const = 0;
    virtual Symbol* Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, ScopeLookup scopeLookup, SymbolTable* symbolTable) const = 0;
};

class ContainerScope : public Scope
{
public:
    ContainerScope();
    void SetContainer(ContainerSymbol* container_) { container = container_; }
    ContainerSymbol* Container() { return container; }
    ContainerScope* BaseScope() const;
    ContainerScope* ParentScope() const;
    void Install(Symbol* symbol);
    void Uninstall(Symbol* symbol);
    Symbol* Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, SymbolTable* symbolTable) const override;
    Symbol* Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, ScopeLookup scopeLookup, SymbolTable* symbolTable) const override;
    Symbol* Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, SymbolTable* symbolTable) const override;
    Symbol* Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, ScopeLookup scopeLookup, SymbolTable* symbolTable) const override;
private:
    ContainerSymbol* container;
    std::map<std::u16string, Symbol*> symbolMap;
};

class FileScope : public Scope
{
public:
    FileScope();
    void InstallNamespaceImport(ContainerScope* scope, cmajor::fault::tolerant::ast::NamespaceImportNode* namespaceImport, SymbolTable* symbolTable);
    void InstallAlias(cmajor::fault::tolerant::ast::UsingAliasNode* alias, TypeSymbol* type);
    Symbol* Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, SymbolTable* symbolTable) const override;
    Symbol* Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, ScopeLookup scopeLookup, SymbolTable* symbolTable) const override;
    Symbol* Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, SymbolTable* symbolTable) const override;
    Symbol* Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, ScopeLookup scopeLookup, SymbolTable* symbolTable) const override;
private:
    std::vector<ContainerScope*> containerScopes;
    std::map<std::u16string, TypeSymbol*> aliasMap;
};

} // namespace cmajor::fault::tolerant::symbols
