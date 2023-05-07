// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.namespaces;

import cmajor.symbols.container.symbol;
import std.core;

export namespace cmajor::symbols {

class NamespaceSymbol : public ContainerSymbol
{
public:
    NamespaceSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "namespace"; }
    std::u32string Id() const override { return U"ns_" + FullName(); }
    bool IsParentSymbol() const override { return true; }
    void Import(NamespaceSymbol* that, SymbolTable& symbolTable);
    bool IsGlobalNamespace() const { return Name().empty(); }
    std::unique_ptr<soul::xml::Element> CreateDomElement(TypeMap& typeMap) override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "NamespaceSymbol"; }
    std::string GetSymbolCategoryStr() const override { return "NS"; }
    std::string GetSymbolCategoryDescription() const override { return "namespace"; }
};
} // namespace cmajor::symbols
