// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.compound.type.symbol;

import cmajor.fault.tolerant.symbols.type.symbol;
import cmajor.fault.tolerant.symbols.derivations;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class SymbolWriter;
class SymbolReader;

class CompoundTypeSymbol : public TypeSymbol
{
public:
    CompoundTypeSymbol(const std::u16string& name_);
    CompoundTypeSymbol(TypeSymbol* baseType_, const Derivations& derivations_);
    TypeSymbol* BaseType() override { return baseType; }
    TypeSymbol* PlainType(SymbolTable* symbolTable) override;
    const Derivations& GetDerivations() const override { return derivations; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve(SymbolTable* symbolTable) override;
private:
    TypeSymbol* baseType;
    Derivations derivations;
    util::uuid baseTypeId;
};

} // namespace cmajor::fault::tolerant::symbols
