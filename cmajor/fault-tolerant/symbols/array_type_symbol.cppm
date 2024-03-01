// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.array.type.symbol;

import cmajor.fault.tolerant.symbols.type.symbol;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class ArrayTypeSymbol : public TypeSymbol
{
public:
    ArrayTypeSymbol(TypeSymbol* elementType_, int64_t size_);
    ArrayTypeSymbol(const std::u16string& name_);
    TypeSymbol* ElementType() const { return elementType; }
    int64_t Size() const { return size; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve(SymbolTable* symbolTable) override;
private:
    TypeSymbol* elementType;
    int64_t size;
    util::uuid elementTypeId;
};

} // namespace cmajor::fault::tolerant::symbols
