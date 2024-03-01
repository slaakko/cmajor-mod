// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.scope;

import cmajor.fault.tolerant.symbols.symbol;
import cmajor.fault.tolerant.symbols.container.symbol;
import cmajor.fault.tolerant.symbols.symbol.table;
import cmajor.fault.tolerant.symbols.type.symbol;
import cmajor.fault.tolerant.symbols.class_symbol;

namespace cmajor::fault::tolerant::symbols {

Scope::~Scope()
{
}

ContainerScope::ContainerScope() : container(nullptr)
{
}

ContainerScope* ContainerScope::BaseScope() const
{
    if (container && container->IsClassTypeSymbol())
    {
        ClassTypeSymbol* classTypeSymbol = static_cast<ClassTypeSymbol*>(container);
        if (classTypeSymbol->BaseClass())
        {
            return classTypeSymbol->BaseClass()->Scope();
        }
    }
    return nullptr;
}

ContainerScope* ContainerScope::ParentScope() const
{
    if (container && container->Parent())
    {
        return container->Parent()->Scope();
    }
    else
    {
        return nullptr;
    }
}

void ContainerScope::Install(Symbol* symbol)
{
    symbolMap[symbol->Name()] = symbol;
    symbol->SetInstalled();
}

void ContainerScope::Uninstall(Symbol* symbol)
{
    symbolMap.erase(symbol->Name());
    symbol->ResetInstalled();
}

Symbol* ContainerScope::Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, SymbolTable* symbolTable) const
{
    return Lookup(name, ScopeLookup::this_, symbolTable);
}

Symbol* ContainerScope::Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, ScopeLookup scopeLookup, SymbolTable* symbolTable) const
{
    if ((scopeLookup & ScopeLookup::this_) != ScopeLookup::none)
    {
        auto it = symbolMap.find(name->Str());
        if (it != symbolMap.end())
        {
            Symbol* symbol = it->second;
            if ((scopeLookup & ScopeLookup::map) != ScopeLookup::none)
            {
                symbolTable->MapNode(name, symbol);
            }
            return symbol;
        }
    }
    if ((scopeLookup & ScopeLookup::base) != ScopeLookup::none)
    {
        ContainerScope* baseScope = BaseScope();
        if (baseScope)
        {
            Symbol* symbol = baseScope->Lookup(name, scopeLookup, symbolTable);
            if (symbol)
            {
                if ((scopeLookup & ScopeLookup::map) != ScopeLookup::none)
                {
                    symbolTable->MapNode(name, symbol);
                }
                return symbol;
            }
        }
    }
    if ((scopeLookup & ScopeLookup::parent) != ScopeLookup::none)
    {
        ContainerScope* parentScope = ParentScope();
        if (parentScope)
        {
            Symbol* symbol = parentScope->Lookup(name, scopeLookup, symbolTable);
            if (symbol)
            {
                if ((scopeLookup & ScopeLookup::map) != ScopeLookup::none)
                {
                    symbolTable->MapNode(name, symbol);
                }
                return symbol;
            }
        }
    }
    return nullptr;
}

Symbol* ContainerScope::Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, SymbolTable* symbolTable) const
{
    return Lookup(qualifiedName, ScopeLookup::this_, symbolTable);
}

Symbol* ContainerScope::Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, ScopeLookup scopeLookup, SymbolTable* symbolTable) const
{
    const Scope* scope = this;
    Symbol* symbol = nullptr;
    int n = qualifiedName->IdentifierNodes().size();
    if (n == 1)
    {
        return Lookup(qualifiedName->IdentifierNodes().front(), scopeLookup, symbolTable);
    }
    bool allComponentsMatched = true;
    for (int i = 0; i < n; ++i)
    {
        if (scope)
        {
            symbol = scope->Lookup(qualifiedName->IdentifierNodes()[i], scopeLookup & (ScopeLookup::this_ | ScopeLookup::map), symbolTable);
            if (symbol)
            {
                scope = symbol->Scope();
            }
            else
            {
                allComponentsMatched = false;
            }
        }
    }
    if (!symbol || !allComponentsMatched)
    { 
        if ((scopeLookup & ScopeLookup::parent) != ScopeLookup::none)
        {
            ContainerScope* parentScope = ParentScope();
            if (parentScope)
            {
                return parentScope->Lookup(qualifiedName, scopeLookup, symbolTable);
            }
            else
            {
                return nullptr;
            }
        }
    }
    return symbol;
}

FileScope::FileScope()
{
}

void FileScope::InstallNamespaceImport(ContainerScope* scope, cmajor::fault::tolerant::ast::NamespaceImportNode* namespaceImport, SymbolTable* symbolTable)
{
    Symbol* symbol = scope->Lookup(namespaceImport->QualifiedId(), ScopeLookup::this_and_parent | ScopeLookup::map, symbolTable);
    if (symbol)
    {
        containerScopes.push_back(symbol->Scope());
    }
}

void FileScope::InstallAlias(cmajor::fault::tolerant::ast::UsingAliasNode* alias, TypeSymbol* type)
{
    aliasMap[alias->Identifier()->Str()] = type;
}

Symbol* FileScope::Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, SymbolTable* symbolTable) const
{
    return Lookup(name, ScopeLookup::this_, symbolTable);
}

Symbol* FileScope::Lookup(cmajor::fault::tolerant::ast::IdentifierNode* name, ScopeLookup scopeLookup, SymbolTable* symbolTable) const
{
    auto it = aliasMap.find(name->Str());
    if (it != aliasMap.end())
    {
        Symbol* symbol = it->second;
        if ((scopeLookup & ScopeLookup::map) != ScopeLookup::none)
        {
            symbolTable->MapNode(name, symbol);
        }
        return symbol;
    }
    for (ContainerScope* containerScope : containerScopes)
    {
        Symbol* symbol = containerScope->Lookup(name, scopeLookup & (ScopeLookup::this_ | ScopeLookup::map), symbolTable);
        if (symbol)
        {
            return symbol;
        }
    }
    return nullptr;
}

Symbol* FileScope::Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, SymbolTable* symbolTable) const
{
    return Lookup(qualifiedName, ScopeLookup::this_, symbolTable);
}

Symbol* FileScope::Lookup(cmajor::fault::tolerant::ast::QualifiedIdNode* qualifiedName, ScopeLookup scopeLookup, SymbolTable* symbolTable) const
{
    for (ContainerScope* containerScope : containerScopes)
    {
        Symbol* symbol = containerScope->Lookup(qualifiedName, scopeLookup & (ScopeLookup::this_ | ScopeLookup::map), symbolTable);
        if (symbol)
        {
            return symbol;
        }
    }
    return nullptr;
}

} // namespace cmajor::fault::tolerant::symbols
