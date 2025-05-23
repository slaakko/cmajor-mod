// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.scope;

import cmajor.symbols.context;
import cmajor.symbols.container.symbol;
import cmajor.symbols.classes;
import cmajor.symbols.class_template_specializations;
import cmajor.symbols.modules;
import cmajor.symbols.namespaces;
import cmajor.symbols.symbol.table;
import cmajor.symbols.exception;
import cmajor.symbols.function.symbol;
import cmajor.symbols.variable.symbol;
import util;

namespace cmajor::symbols {

Scope::~Scope()
{
}

ContainerScope::ContainerScope() : container(nullptr), parentScope(nullptr), symbolMap()
{
}

ContainerScope* ContainerScope::BaseScope() const
{
    if (container)
    {
        if (container->GetSymbolType() == SymbolType::classTypeSymbol || container->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
        {
            ClassTypeSymbol* cls = static_cast<ClassTypeSymbol*>(container);
            ClassTypeSymbol* baseClass = cls->BaseClass();
            if (baseClass)
            {
                return baseClass->GetContainerScope();
            }
        }
    }
    return nullptr;
}

ContainerScope* ContainerScope::ParentScope(Context* context) const
{
    if (parentScope)
    {
        return parentScope;
    }
    if (container)
    {
        Symbol* parent = nullptr;
        if (container->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
        {
            ClassTemplateSpecializationSymbol* specialization = static_cast<ClassTemplateSpecializationSymbol*>(container);
            ClassTypeSymbol* classTemplate = specialization->GetClassTemplate();
            parent = classTemplate->Parent();
        }
        else
        {
            parent = container->Parent();
        }
        if (parent)
        {
            if (!parent->GetModule()->IsRootModule() && parent->GetSymbolType() == SymbolType::namespaceSymbol)
            {
                NamespaceSymbol* ns = static_cast<NamespaceSymbol*>(parent);
                Module* rootModule = context->RootModule();
                NamespaceSymbol* mappedNs = rootModule->GetSymbolTable().GetMappedNs(ns);
                if (mappedNs)
                {
                    return mappedNs->GetContainerScope();
                }
            }
            return parent->GetContainerScope();
        }
    }
    return nullptr;
}

void ContainerScope::Install(Symbol* symbol)
{
    auto it = symbolMap.find(symbol->Name());
    if (symbol->GetSymbolType() != SymbolType::namespaceSymbol &&
        symbol->GetSymbolType() != SymbolType::declarationBlock &&
        symbol->GetSymbolType() != SymbolType::aliasTypeSymbol &&
        it != symbolMap.cend())
    {
        Symbol* prev = it->second;
        if (prev != symbol)
        {
            throw Exception("symbol '" + util::ToUtf8(symbol->Name()) + "' already defined", symbol->GetFullSpan(), prev->GetFullSpan());
        }
    }
    else
    {
        symbolMap[symbol->Name()] = symbol;
    }
    symbol->SetInstalled();
}

void ContainerScope::Uninstall(Symbol* symbol)
{
    symbolMap.erase(symbol->Name());
    symbol->ResetInstalled();
}

Symbol* ContainerScope::Lookup(const std::u32string& name, Context* context) const
{
    return Lookup(name, ScopeLookup::this_, context);
}

int CountQualifiedNameComponents(const std::u32string& qualifiedName)
{
    int numComponents = 0;
    int componentSize = 0;
    int state = 0;
    int angleBracketCount = 0;
    for (char32_t c : qualifiedName)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '.')
            {
                ++numComponents;
                componentSize = 0;
            }
            else if (c == '<')
            {
                ++componentSize;
                angleBracketCount = 1;
                state = 1;
            }
            else
            {
                ++componentSize;
            }
            break;
        }
        case 1:
        {
            ++componentSize;
            if (c == '<')
            {
                ++angleBracketCount;
            }
            else if (c == '>')
            {
                --angleBracketCount;
                if (angleBracketCount == 0)
                {
                    state = 0;
                }
            }
            break;
        }
        }
    }
    if (componentSize > 0)
    {
        ++numComponents;
    }
    return numComponents;
}

std::vector<std::u32string> ParseQualifiedName(const std::u32string& qualifiedName)
{
    std::vector<std::u32string> components;
    int state = 0;
    std::u32string component;
    int angleBracketCount = 0;
    for (char32_t c : qualifiedName)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '.')
            {
                components.push_back(component);
                component.clear();
            }
            else if (c == '<')
            {
                component.append(1, c);
                angleBracketCount = 1;
                state = 1;
            }
            else
            {
                component.append(1, c);
            }
            break;
        }
        case 1:
        {
            component.append(1, c);
            if (c == '<')
            {
                ++angleBracketCount;
            }
            else if (c == '>')
            {
                --angleBracketCount;
                if (angleBracketCount == 0)
                {
                    state = 0;
                }
            }
            break;
        }
        }
    }
    if (!component.empty())
    {
        components.push_back(component);
    }
    return components;
}

std::vector<CCComponent> ParseCCName(const std::u32string& qualifiedName)
{
    std::vector<CCComponent> components;
    int state = 0;
    std::u32string str;
    int angleBracketCount = 0;
    CCComponentSeparator separator = CCComponentSeparator::dot;
    for (char32_t c : qualifiedName)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '.')
            {
                components.push_back(CCComponent(separator, str));
                separator = CCComponentSeparator::dot;
                str.clear();
            }
            else if (c == '-')
            {
                state = 2;
            }
            else if (c == '<')
            {
                str.append(1, c);
                angleBracketCount = 1;
                state = 1;
            }
            else
            {
                str.append(1, c);
            }
            break;
        }
        case 1:
        {
            str.append(1, c);
            if (c == '<')
            {
                ++angleBracketCount;
            }
            else if (c == '>')
            {
                --angleBracketCount;
                if (angleBracketCount == 0)
                {
                    state = 0;
                }
            }
            break;
        }
        case 2:
        {
            if (c == '>')
            {
                components.push_back(CCComponent(separator, str));
                separator = CCComponentSeparator::arrow;
                str.clear();
                state = 0;
            }
            else if (c != '-')
            {
                state = 0;
            }
            break;
        }
        }
    }
    components.push_back(CCComponent(separator, str));
    return components;
}

int CountCCComponents(const std::u32string& qualifiedName)
{
    int numComponents = 0;
    int state = 0;
    int angleBracketCount = 0;
    for (char32_t c : qualifiedName)
    {
        switch (state)
        {
        case 0:
        {
            if (c == '.')
            {
                ++numComponents;
            }
            else if (c == '-')
            {
                state = 2;
            }
            else if (c == '<')
            {
                angleBracketCount = 1;
                state = 1;
            }
            break;
        }
        case 1:
        {
            if (c == '<')
            {
                ++angleBracketCount;
            }
            else if (c == '>')
            {
                --angleBracketCount;
                if (angleBracketCount == 0)
                {
                    state = 0;
                }
            }
            break;
        }
        case 2:
        {
            if (c == '>')
            {
                ++numComponents;
                state = 0;
            }
            else if (c != '-')
            {
                state = 0;
            }
            break;
        }
        }
    }
    ++numComponents;
    return numComponents;
}

std::u32string MakeCCMatch(const std::vector<CCComponent>& components, const std::u32string& last)
{
    std::u32string ccMatch;
    int n = components.size();
    bool first = true;
    for (int i = 0; i < n - 1; ++i)
    {
        const CCComponent& component = components[i];
        if (first)
        {
            first = false;
        }
        else
        {
            if (component.separator == CCComponentSeparator::dot)
            {
                ccMatch.append(1, '.');
            }
            else if (component.separator == CCComponentSeparator::arrow)
            {
                ccMatch.append(U"->");
            }
        }
        ccMatch.append(component.str);
    }
    const CCComponent& component = components[n - 1];
    if (first)
    {
        first = false;
    }
    else
    {
        if (component.separator == CCComponentSeparator::dot)
        {
            ccMatch.append(1, '.');
        }
        else if (component.separator == CCComponentSeparator::arrow)
        {
            ccMatch.append(U"->");
        }
    }
    ccMatch.append(last);
    return ccMatch;
}

std::vector<CCSymbolEntry> MakeCCMatches(const std::vector<CCComponent>& components, const std::vector<CCSymbolEntry>& matches)
{
    std::vector<CCSymbolEntry> ccMatches;
    for (const CCSymbolEntry& match : matches)
    {
        ccMatches.push_back(CCSymbolEntry(match.symbol, match.ccPrefixLen, MakeCCMatch(components, match.replacement)));
    }
    return ccMatches;
}

bool CCSymbolEntryLess::operator()(const CCSymbolEntry& left, const CCSymbolEntry& right) const
{
    if (left.symbol->Name() < right.symbol->Name()) return true;
    if (left.symbol->Name() > right.symbol->Name()) return false;
    return left.symbol->GetSymbolCategoryStr() < right.symbol->GetSymbolCategoryStr();
}

void AddMatches(std::vector<CCSymbolEntry>& matches, std::vector<CCSymbolEntry>& matchesToAdd)
{
    for (CCSymbolEntry& entry : matchesToAdd)
    {
        Symbol* s = entry.symbol;
        bool found = false;
        for (CCSymbolEntry& m : matches)
        {
            if (s == m.symbol)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            matches.push_back(std::move(entry));
        }
    }
}

Symbol* ContainerScope::Lookup(const std::u32string& name, ScopeLookup lookup, Context* context) const
{
    int numQualifiedNameComponents = CountQualifiedNameComponents(name);
    if (numQualifiedNameComponents > 1)
    {
        std::vector<std::u32string> components = ParseQualifiedName(name);
        return LookupQualified(components, lookup, context);
    }
    else
    {
        auto it = symbolMap.find(name);
        if (it != symbolMap.end())
        {
            Symbol* s = it->second;
            return s;
        }
        if ((lookup & ScopeLookup::base) != ScopeLookup::none)
        {
            ContainerScope* baseScope = BaseScope();
            if (baseScope)
            {
                Symbol* s = baseScope->Lookup(name, lookup, context);
                if (s)
                {
                    return s;
                }
            }
        }
        if ((lookup & ScopeLookup::parent) != ScopeLookup::none)
        {
            ContainerScope* parentScope = ParentScope(context);
            if (parentScope)
            {
                Symbol* s = parentScope->Lookup(name, lookup, context);
                if (s)
                {
                    return s;
                }
            }
        }
        return nullptr;
    }
}

Symbol* ContainerScope::LookupQualified(const std::vector<std::u32string>& components, ScopeLookup lookup, Context* context) const
{
    const ContainerScope* scope = this;
    Symbol* s = nullptr;
    int n = int(components.size());
    bool allComponentsMatched = true;
    for (int i = 0; i < n; ++i)
    {
        const std::u32string& component = components[i];
        if (scope)
        {
            s = scope->Lookup(component, ScopeLookup::this_, context);
            if (s)
            {
                scope = s->GetContainerScope();
            }
            else
            {
                allComponentsMatched = false;
            }
        }
    }
    if (!s || !allComponentsMatched)
    {
        if ((lookup & ScopeLookup::parent) != ScopeLookup::none)
        {
            ContainerScope* parentScope = ParentScope(context);
            if (parentScope)
            {
                return parentScope->LookupQualified(components, lookup, context);
            }
            else
            {
                return nullptr;
            }
        }
    }
    return s;
}

std::vector<CCSymbolEntry> ContainerScope::LookupBeginWith(const std::u32string& prefix, Context* context) const
{
    return LookupBeginWith(prefix, ScopeLookup::this_, context);
}

std::vector<CCSymbolEntry> ContainerScope::LookupBeginWith(const std::u32string& prefix, ScopeLookup lookup, Context* context) const
{
    int numQualifiedNameComponents = CountCCComponents(prefix);
    if (numQualifiedNameComponents > 1)
    {
        std::vector<CCComponent> components = ParseCCName(prefix);
        return LookupQualifiedBeginWith(components, lookup, context);
    }
    else
    {
        std::vector<CCSymbolEntry> matches;
        auto it = symbolMap.lower_bound(prefix);
        while (it != symbolMap.cend() && it->first.starts_with(prefix))
        {
            matches.push_back(CCSymbolEntry(it->second, prefix.length(), it->second->Name()));
            ++it;
        }
        if ((lookup & ScopeLookup::base) != ScopeLookup::none)
        {
            ContainerScope* baseScope = BaseScope();
            if (baseScope)
            {
                std::vector<CCSymbolEntry> m = baseScope->LookupBeginWith(prefix, lookup, context);
                AddMatches(matches, m);
            }
        }
        if ((lookup & ScopeLookup::parent) != ScopeLookup::none)
        {
            ContainerScope* parentScope = ParentScope(context);
            if (parentScope)
            {
                std::vector<CCSymbolEntry> m = parentScope->LookupBeginWith(prefix, lookup, context);
                AddMatches(matches, m);
            }
        }
        return matches;
    }
}

std::vector<CCSymbolEntry> ContainerScope::LookupQualifiedBeginWith(const std::vector<CCComponent>& components, ScopeLookup lookup, Context* context) const
{
    std::vector<CCSymbolEntry> matches;
    const ContainerScope* scope = this;
    int n = int(components.size());
    const Symbol* s = nullptr;
    for (int i = 0; i < n - 1; ++i)
    {
        const CCComponent& component = components[i];
        if (scope)
        {
            if (component.separator == CCComponentSeparator::arrow)
            {
                const ContainerSymbol* containerSymbol = scope->Container();
                scope = containerSymbol->GetArrowScope();
            }
            if (component.str == U"this")
            {
                const ContainerSymbol* containerSymbol = Container();
                const FunctionSymbol* f = containerSymbol->FunctionNoThrow();
                if (f && f->GetSymbolType() == SymbolType::memberFunctionSymbol)
                {
                    s = f->Parameters()[0];
                }
                else
                {
                    s = nullptr;
                }
            }
            else
            {
                s = scope->Lookup(component.str, ScopeLookup::this_, context);
            }
            if (s)
            {
                scope = s->GetTypeScope();
            }
        }
        else
        {
            s = nullptr;
        }
    }
    if (s && scope)
    {
        bool validAccess = true;
        if (components[n - 1].separator == CCComponentSeparator::dot)
        {
            switch (s->GetSymbolType())
            {
            case SymbolType::localVariableSymbol:
            case SymbolType::memberVariableSymbol:
            case SymbolType::parameterSymbol:
            {
                const VariableSymbol* variableSymbol = static_cast<const VariableSymbol*>(s);
                const TypeSymbol* type = variableSymbol->GetType();
                if (type && type->IsPointerType())
                {
                    validAccess = false;
                }
            }
            }
        }
        else if (components[n - 1].separator == CCComponentSeparator::arrow)
        {
            const ContainerSymbol* containerSymbol = scope->Container();
            scope = containerSymbol->GetArrowScope();
        }
        if (validAccess)
        {
            std::vector<CCSymbolEntry> m = MakeCCMatches(components, scope->LookupBeginWith(components[n - 1].str, context));
            AddMatches(matches, m);
            lookup = lookup & ~ScopeLookup::parent;
        }
    }
    if ((lookup & ScopeLookup::base) != ScopeLookup::none)
    {
        ContainerScope* baseScope = BaseScope();
        if (baseScope)
        {
            std::vector<CCSymbolEntry> m = baseScope->LookupQualifiedBeginWith(components, ScopeLookup::this_and_base, context);
            AddMatches(matches, m);
        }
    }
    if ((lookup & ScopeLookup::parent) != ScopeLookup::none)
    {
        ContainerScope* parentScope = ParentScope(context);
        if (parentScope)
        {
            std::vector<CCSymbolEntry> m = parentScope->LookupQualifiedBeginWith(components, ScopeLookup::this_and_base_and_parent, context);
            AddMatches(matches, m);
        }
    }
    return matches;
}

const NamespaceSymbol* ContainerScope::Ns(Context* context) const
{
    return container->Ns(context);
}

NamespaceSymbol* ContainerScope::Ns(Context* context)
{
    return container->Ns(context);
}

void ContainerScope::Clear()
{
    symbolMap.clear();
}

NamespaceSymbol* ContainerScope::CreateNamespace(const std::u32string& qualifiedNsName, const soul::ast::Span& span, const util::uuid& moduleId, int32_t fileIndex, 
    Context* context)
{
    ContainerScope* scope = this;
    NamespaceSymbol* parentNs = scope->Ns(context);
    std::vector<std::u32string> components = util::Split(qualifiedNsName, '.');
    for (const std::u32string& component : components)
    {
        Symbol* s = scope->Lookup(component, context);
        if (s)
        {
            if (s->GetSymbolType() == SymbolType::namespaceSymbol)
            {
                scope = s->GetContainerScope();
                parentNs = scope->Ns(context);
            }
            else
            {
                throw Exception("symbol '" + util::ToUtf8(s->Name()) + "' does not denote a namespace", s->GetFullSpan());
            }
        }
        else
        {
            NamespaceSymbol* newNs = new NamespaceSymbol(span, component);
            newNs->SetModuleId(moduleId);
            newNs->SetFileIndex(fileIndex);
            newNs->SetModule(container->GetModule());
            scope = newNs->GetContainerScope();
            parentNs->AddMember(newNs, context);
            parentNs = newNs;
        }
    }
    return parentNs;
}

void ContainerScope::CollectViableFunctions(int arity, const std::u32string& groupName, std::unordered_set<ContainerScope*>& scopesLookedUp, ScopeLookup scopeLookup,
    ViableFunctionSet& viableFunctions, Module* module, Context* context)
{
    if ((scopeLookup & ScopeLookup::this_) != ScopeLookup::none)
    {
        if (scopesLookedUp.find(this) == scopesLookedUp.end())
        {
            scopesLookedUp.insert(this);
            Symbol* symbol = Lookup(groupName, context);
            if (symbol)
            {
                if (symbol->GetSymbolType() == SymbolType::functionGroupSymbol)
                {
                    FunctionGroupSymbol* functionGroupSymbol = static_cast<FunctionGroupSymbol*>(symbol);
                    functionGroupSymbol->CollectViableFunctions(arity, viableFunctions, module);
                }
            }
        }
    }
    if ((scopeLookup & ScopeLookup::base) != ScopeLookup::none)
    {
        ContainerScope* baseScope = BaseScope();
        if (baseScope)
        {
            baseScope->CollectViableFunctions(arity, groupName, scopesLookedUp, scopeLookup, viableFunctions, module, context);
        }
    }
    if ((scopeLookup & ScopeLookup::parent) != ScopeLookup::none)
    {
        ContainerScope* parentScope = ParentScope(context);
        if (parentScope)
        {
            parentScope->CollectViableFunctions(arity, groupName, scopesLookedUp, scopeLookup, viableFunctions, module, context);
        }
    }
}

FileScope::FileScope()
{
}

void FileScope::AddContainerScope(ContainerScope* containerScope)
{
    if (std::find(containerScopes.begin(), containerScopes.end(), containerScope) == containerScopes.end())
    {
        containerScopes.push_back(containerScope);
    }
}

void FileScope::InstallAlias(cmajor::ast::AliasNode* aliasNode, TypeSymbol* type)
{
    aliasMap[aliasNode->Id()->Str()] = type;
}

void FileScope::InstallNamespaceImport(ContainerScope* containerScope, cmajor::ast::NamespaceImportNode* namespaceImportNode, Context* context)
{
    try
    {
        Assert(containerScope, "container scope is null"); 
        std::u32string importedNamespaceName = namespaceImportNode->Ns()->Str();
        Symbol* symbol = containerScope->Lookup(importedNamespaceName, ScopeLookup::this_and_parent, context);
        if (symbol)
        {
            if (symbol->GetSymbolType() == SymbolType::namespaceSymbol)
            {
                ContainerScope* symbolContainerScope = symbol->GetContainerScope();
                if (std::find(containerScopes.begin(), containerScopes.end(), symbolContainerScope) == containerScopes.end())
                {
                    containerScopes.push_back(symbolContainerScope);
                }
            }
            else
            {
                throw Exception("'" + util::ToUtf8(namespaceImportNode->Ns()->Str()) + "' does not denote a namespace", namespaceImportNode->Ns()->GetFullSpan());
            }
        }
        else
        {
            throw Exception("referred namespace symbol '" + util::ToUtf8(namespaceImportNode->Ns()->Str()) + "' not found", namespaceImportNode->Ns()->GetFullSpan());
        }
    }
    catch (const Exception&)
    {
        throw;
    }
}

Symbol* FileScope::Lookup(const std::u32string& name, Context* context) const
{
    return Lookup(name, ScopeLookup::this_, context);
}

Symbol* FileScope::Lookup(const std::u32string& name, ScopeLookup lookup, Context* context) const
{
    if (lookup != ScopeLookup::this_)
    {
        throw std::runtime_error("file scope supports only this scope lookup");
    }
    std::unordered_set<Symbol*> foundSymbols;
    auto it = aliasMap.find(name);
    if (it != aliasMap.cend())
    {
        TypeSymbol* symbol = it->second;
        foundSymbols.insert(symbol);
    }
    else
    {
        for (ContainerScope* containerScope : containerScopes)
        {
            Symbol* symbol = containerScope->Lookup(name, ScopeLookup::this_, context);
            if (symbol)
            {
                foundSymbols.insert(symbol);
            }
        }
    }
    if (foundSymbols.empty())
    {
        return nullptr;
    }
    else if (foundSymbols.size() > 1)
    {
        std::string message("reference to object '" + util::ToUtf8(name) + "' is ambiguous: ");
        bool first = true;
        soul::ast::FullSpan fullSpan;
        for (Symbol* symbol : foundSymbols)
        {
            if (first)
            {
                first = false;
                fullSpan = symbol->GetFullSpan();
            }
            else
            {
                message.append(" or ");
            }
            message.append(util::ToUtf8(symbol->FullName()));
        }
        throw Exception(message, fullSpan);
    }
    else
    {
        return *foundSymbols.begin();
    }
}

std::vector<CCSymbolEntry> FileScope::LookupBeginWith(const std::u32string& prefix, Context* context) const
{
    return LookupBeginWith(prefix, ScopeLookup::this_, context);
}

std::vector<CCSymbolEntry> FileScope::LookupBeginWith(const std::u32string& prefix, ScopeLookup lookup, Context* context) const
{
    std::vector<CCSymbolEntry> matches;
    auto it = aliasMap.lower_bound(prefix);
    if (it != aliasMap.cend())
    {
        while (it != aliasMap.cend() && it->first.starts_with(prefix))
        {
            matches.push_back(CCSymbolEntry(it->second, prefix.length(), it->second->Name()));
            ++it;
        }
    }
    for (ContainerScope* containerScope : containerScopes)
    {
        std::vector<CCSymbolEntry> m = containerScope->LookupBeginWith(prefix, ScopeLookup::this_, context);
        AddMatches(matches, m);
    }
    return matches;
}

void FileScope::CollectViableFunctions(int arity, const std::u32string& groupName, std::unordered_set<ContainerScope*>& scopesLookedUp, ViableFunctionSet& viableFunctions,
    Module* module, Context* context)
{
    for (ContainerScope* containerScope : containerScopes)
    {
        if (scopesLookedUp.find(containerScope) == scopesLookedUp.end())
        {
            containerScope->CollectViableFunctions(arity, groupName, scopesLookedUp, ScopeLookup::this_, viableFunctions, module, context);
            scopesLookedUp.insert(containerScope);
        }
    }
}

} // namespace cmajor::symbols
