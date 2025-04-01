// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.scope;

import std.core;
import soul.ast.span;
import cmajor.ast.namespaces;
import cmajor.ast.alias;
import util.uuid;

export namespace cmajor::symbols {

class Context;
class Symbol;
class ContainerSymbol;
class NamespaceSymbol;
class FunctionSymbol;
class TypeSymbol;
class Module;
class ViableFunctionSet;

enum class ScopeLookup : uint8_t
{
    none = 0,
    this_ = 1 << 0,
    base = 1 << 1,
    parent = 1 << 2,
    this_and_base = this_ | base,
    this_and_parent = this_ | parent,
    this_and_base_and_parent = this_ | base | parent,
    fileScopes = 1 << 3
};

inline ScopeLookup operator&(ScopeLookup left, ScopeLookup right)
{
    return ScopeLookup(uint8_t(left) & uint8_t(right));
}

inline ScopeLookup operator~(ScopeLookup subject)
{
    return ScopeLookup(~uint8_t(subject));
}

struct CCSymbolEntry
{
    CCSymbolEntry(Symbol* symbol_, int ccPrefixLen_, const std::u32string& replacement_) : symbol(symbol_), ccPrefixLen(ccPrefixLen_), replacement(replacement_) {}
    Symbol* symbol;
    int ccPrefixLen;
    std::u32string replacement;
};

struct CCSymbolEntryLess
{
    bool operator()(const CCSymbolEntry& left, const CCSymbolEntry& right) const;
};

void AddMatches(std::vector<CCSymbolEntry>& matches, std::vector<CCSymbolEntry>& matchesToAdd);

enum class CCComponentSeparator : int8_t
{
    dot = 0, arrow = 1
};

struct CCComponent
{
    CCComponent(CCComponentSeparator separator_, const std::u32string& str_) : separator(separator_), str(str_) {}
    CCComponentSeparator separator;
    std::u32string str;
};

class Scope
{
public:
    virtual ~Scope();
    virtual Symbol* Lookup(const std::u32string& name, Context* context) const = 0;
    virtual Symbol* Lookup(const std::u32string& name, ScopeLookup lookup, Context* context) const = 0;
    virtual std::vector<CCSymbolEntry> LookupBeginWith(const std::u32string& prefix, Context* context) const = 0;
    virtual std::vector<CCSymbolEntry> LookupBeginWith(const std::u32string& prefix, ScopeLookup lookup, Context* context) const = 0;
};

class ContainerScope : public Scope
{
public:
    ContainerScope();
    ContainerScope* BaseScope() const;
    ContainerScope* ParentScope(Context* context) const;
    void SetParentScope(ContainerScope* parentScope_) { parentScope = parentScope_; }
    ContainerSymbol* Container() { return container; }
    const ContainerSymbol* Container() const { return container; }
    void SetContainer(ContainerSymbol* container_) { container = container_; }
    void Install(Symbol* symbol);
    void Uninstall(Symbol* symbol);
    Symbol* Lookup(const std::u32string& name, Context* context) const override;
    Symbol* Lookup(const std::u32string& name, ScopeLookup lookup, Context* context) const override;
    Symbol* LookupQualified(const std::vector<std::u32string>& components, ScopeLookup lookup, Context* context) const;
    std::vector<CCSymbolEntry> LookupBeginWith(const std::u32string& prefix, Context* context) const override;
    std::vector<CCSymbolEntry> LookupBeginWith(const std::u32string& prefix, ScopeLookup lookup, Context* context) const override;
    std::vector<CCSymbolEntry> LookupQualifiedBeginWith(const std::vector<CCComponent>& components, ScopeLookup lookup, Context* context) const;
    const NamespaceSymbol* Ns(Context* context) const;
    NamespaceSymbol* Ns(Context* context);
    void Clear();
    NamespaceSymbol* CreateNamespace(const std::u32string& qualifiedNsName, const soul::ast::Span& span_, const util::uuid& moduleId, int32_t fileIndex, Context* context);
    void CollectViableFunctions(int arity, const std::u32string& groupName, std::unordered_set<ContainerScope*>& scopesLookedUp, ScopeLookup scopeLookup,
        ViableFunctionSet& viableFunctions, Module* module, Context* context);
    const std::map<std::u32string, Symbol*>& SymbolMap() const { return symbolMap; }
private:
    ContainerSymbol* container;
    ContainerScope* parentScope;
    std::map<std::u32string, Symbol*> symbolMap;
};

class FileScope : public Scope
{
public:
    FileScope();
    void AddContainerScope(ContainerScope* containerScope);
    void InstallAlias(cmajor::ast::AliasNode* aliasNode, TypeSymbol* type);
    void InstallNamespaceImport(ContainerScope* containerScope, cmajor::ast::NamespaceImportNode* namespaceImportNode, Context* context);
    Symbol* Lookup(const std::u32string& name, Context* context) const override;
    Symbol* Lookup(const std::u32string& name, ScopeLookup lookup, Context* context) const override;
    std::vector<CCSymbolEntry> LookupBeginWith(const std::u32string& prefi, Context* contextx) const override;
    std::vector<CCSymbolEntry> LookupBeginWith(const std::u32string& prefix, ScopeLookup lookup, Context* context) const override;
    void CollectViableFunctions(int arity, const std::u32string& groupName, std::unordered_set<ContainerScope*>& scopesLookedUp, ViableFunctionSet& viableFunctions,
        Module* module, Context* context);
private:
    std::vector<ContainerScope*> containerScopes;
    std::map<std::u32string, TypeSymbol*> aliasMap;
};

} // namespace cmajor::symbols
