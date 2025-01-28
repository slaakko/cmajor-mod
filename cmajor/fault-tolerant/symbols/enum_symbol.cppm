// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.enum_symbol;

import cmajor.fault.tolerant.symbols.type.symbol;
import util.uuid;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class TypeSymbol;

class EnumTypeSymbol : public TypeSymbol
{
public:
    EnumTypeSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve(SymbolTable* symbolTable) override;
    void SetUnderlyingType(TypeSymbol* underlyingType_) { underlyingType = underlyingType_; }
    TypeSymbol* UnderlyingType() const { return underlyingType; }
private:
    TypeSymbol* underlyingType;
    util::uuid underlyingTypeId;
};

class EnumConstantSymbol : public Symbol
{
public:
    EnumConstantSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

} // namespace cmajor::fault::tolerant::symbols
