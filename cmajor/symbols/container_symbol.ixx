// =================================
// Copyright (c) 2022 Seppo Laakko
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
    ContainerSymbol(SymbolType symbolType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    virtual void AddMember(Symbol* member);
    void AddOwnedMember(Symbol* ownedMember);
    std::unique_ptr<Symbol> RemoveMember(int symbolIndex) override;
    void Accept(SymbolCollector* collector) override;
    void Clear();
    std::string TypeString() const override { return "container"; }
    bool IsContainerSymbol() const override { return true; }
    const ContainerScope* GetContainerScope() const override { return containerScope.get(); }
    ContainerScope* GetContainerScope() override { return containerScope.get(); }
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
    std::unique_ptr<ContainerScope> containerScope;
    std::unordered_map<int32_t, FunctionSymbol*> functionIndexMap;
    FunctionGroupSymbol* MakeFunctionGroupSymbol(const std::u32string& groupName, const soul::ast::SourcePos& sourcePos, const util::uuid& sourceModuleId);
    ConceptGroupSymbol* MakeConceptGroupSymbol(const std::u32string& groupName, const soul::ast::SourcePos& sourcePos, const util::uuid& sourceModuleId);
    ClassGroupTypeSymbol* MakeClassGroupTypeSymbol(const std::u32string& groupName, const soul::ast::SourcePos& sourcePos, const util::uuid& sourceModuleId);
    GlobalVariableGroupSymbol* MakeGlobalVariableGroupSymbol(const std::u32string& groupName, const soul::ast::SourcePos& sourcePos, const util::uuid& sourceModuleId);
};

class DeclarationBlock : public ContainerSymbol
{
public:
    DeclarationBlock(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    void AddMember(Symbol* member) override;
    const char* ClassName() const override { return "DeclarationBlock"; }
};
} // namespace cmajor::symbols
