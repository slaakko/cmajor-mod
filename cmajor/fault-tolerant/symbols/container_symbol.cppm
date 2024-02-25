// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.container.symbol;

import cmajor.fault.tolerant.symbols.symbol;
import cmajor.fault.tolerant.symbols.scope;
import cmajor.fault.tolerant.symbols.error.container;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

    class SymbolTable;

class ContainerSymbol : public Symbol
{
public:
    ContainerSymbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    virtual void AddSymbol(Symbol* symbol, SymbolTable* symbolTable);
    std::unique_ptr<Symbol> RemoveSymbol(int index, ErrorContainer* errors);
    const std::vector<std::unique_ptr<Symbol>>& Symbols() const { return symbols; }
    int Count() const { return count; }
    ContainerScope* Scope() override { return &scope; }
    void Compact() override;
private:
    std::vector<std::unique_ptr<Symbol>> symbols;
    int count;
    ContainerScope scope;
};

} // namespace cmajor::fault::tolerant::symbols
