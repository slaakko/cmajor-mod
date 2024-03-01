// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.namespace_symbol;

import cmajor.fault.tolerant.symbols.container.symbol;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class NamespaceSymbol : public ContainerSymbol
{
public:
    NamespaceSymbol(const std::u16string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Import(const NamespaceSymbol* that, SymbolTable* symbolTable);
};

class GlobalNamespaceSymbol : public NamespaceSymbol
{
public:
    GlobalNamespaceSymbol();
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void SetSymbolTable(SymbolTable* symbolTable_) { symbolTable = symbolTable_; }
    SymbolTable* GetSymbolTable() override { return symbolTable; }
    void AddSymbol(Symbol* symbol, SymbolTable* symbolTable) override;
private:
    SymbolTable* symbolTable;
};

} // namespace cmajor::fault::tolerant::symbols
