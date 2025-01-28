// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.container.symbol;

import cmajor.symbols.symbol;
import cmajor.symbols.scope;
import std.core;

export namespace cmajor::symbols {

class FunctionGroupSymbol;
class ConceptGroupSymbol;
class ClassGroupTypeSymbol;
class GlobalVariableGroupSymbol;

class ContainerSymbol : public Symbol
{
public:
    ContainerSymbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    virtual void AddMember(Symbol* member);
    void AddOwnedMember(Symbol* ownedMember);
    std::unique_ptr<Symbol> RemoveMember(int symbolIndex) override;
    void Accept(SymbolCollector* collector) override;
    void Clear();
    std::string TypeString() const override { return "container"; }
    bool IsContainerSymbol() const override { return true; }
    const ContainerScope* GetContainerScope() const override { return &containerScope; }
    ContainerScope* GetContainerScope() override { return &containerScope; }
    const std::vector<std::unique_ptr<Symbol>>& Members() const { return members; }
    std::vector<std::unique_ptr<Symbol>>& Members() { return members; }
    void AppendChildElements(soul::xml::Element* element, TypeMap& typeMap) const override;
    bool HasProjectMembers() const override;
    const char* ClassName() const override { return "ContainerSymbol"; }
    void Check() override;
    void CopyFrom(const Symbol* that) override;
    FunctionSymbol* GetFunctionByIndex(int32_t functionIndex) const;
    virtual bool IsImmutable() const { return false; }
private:
    std::vector<std::unique_ptr<Symbol>> members;
    ContainerScope containerScope;
    std::unordered_map<int32_t, FunctionSymbol*> functionIndexMap;
    void MapFunctionByIndex(FunctionSymbol* fn);
    FunctionGroupSymbol* MakeFunctionGroupSymbol(const std::u32string& groupName, const soul::ast::Span& span);
    ConceptGroupSymbol* MakeConceptGroupSymbol(const std::u32string& groupName, const soul::ast::Span& span);
    ClassGroupTypeSymbol* MakeClassGroupTypeSymbol(const std::u32string& groupName, const soul::ast::Span& span);
    GlobalVariableGroupSymbol* MakeGlobalVariableGroupSymbol(const std::u32string& groupName, const soul::ast::Span& span);
};

class DeclarationBlock : public ContainerSymbol
{
public:
    DeclarationBlock(const soul::ast::Span& span_, const std::u32string& name_);
    void AddMember(Symbol* member) override;
    const char* ClassName() const override { return "DeclarationBlock"; }
};
} // namespace cmajor::symbols
