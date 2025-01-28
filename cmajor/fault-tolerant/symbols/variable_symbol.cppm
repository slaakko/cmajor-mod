// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.variable.symbol;

import cmajor.fault.tolerant.symbols.symbol;
import util.uuid;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class TypeSymbol;

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_);
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

class ParameterSymbol : public VariableSymbol
{
public:
    ParameterSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

class LocalVariableSymbol : public VariableSymbol
{
public:
    LocalVariableSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

class MemberVariableSymbol : public VariableSymbol
{
public:
    MemberVariableSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

class GlobalVariableSymbol : public VariableSymbol
{
public:
    GlobalVariableSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

} // namespace cmajor::fault::tolerant::symbols
