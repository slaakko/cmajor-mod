// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.constant.symbol;

import cmajor.fault.tolerant.symbols.symbol;
import util.uuid;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class TypeSymbol;

class ConstantSymbol : public Symbol
{
public:
    ConstantSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve(SymbolTable* symbolTable) override;
    void SetType(TypeSymbol* type_) { type = type_; }
    TypeSymbol* Type() const { return type; }
    TypeSymbol* GetType() const override { return type; }
private:
    TypeSymbol* type;
    util::uuid typeId;
};

} // namespace cmajor::fault::tolerant::symbols
