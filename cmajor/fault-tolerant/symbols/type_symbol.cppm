// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.type.symbol;

import cmajor.fault.tolerant.symbols.container.symbol;
import cmajor.fault.tolerant.symbols.derivations;
import util.uuid;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class SymbolTable;

class TypeSymbol : public ContainerSymbol
{
public:
    TypeSymbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_);
    virtual TypeSymbol* BaseType() { return this; }
    virtual TypeSymbol* PlainType(SymbolTable* symbolTable) { return this; }
    virtual const Derivations& GetDerivations() const;
    TypeSymbol* GetType() const override { return const_cast<TypeSymbol*>(this); }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    const util::uuid& Id() const { return id; }
private:
    util::uuid id;
};

} // namespace cmajor::fault::tolerant::symbols
