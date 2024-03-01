// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.delegate.symbol;

import cmajor.fault.tolerant.symbols.type.symbol;
import util.uuid;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class ParameterSymbol;

class DelegateSymbol : public TypeSymbol
{
public:
    DelegateSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve(SymbolTable* symbolTable) override;
    void AddSymbol(Symbol* symbol, SymbolTable* symbolTable) override;
    const std::vector<ParameterSymbol*>& Parameters() const { return parameters; }
    void SetReturnType(TypeSymbol* returnType_) { returnType = returnType_; }
    TypeSymbol* ReturnType() const { return returnType; }
private:
    TypeSymbol* returnType;
    std::vector<ParameterSymbol*> parameters;
    util::uuid returnTypeId;
};

class ClassDelegateSymbol : public TypeSymbol
{
public:
    ClassDelegateSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve(SymbolTable* symbolTable) override;
    void AddSymbol(Symbol* symbol, SymbolTable* symbolTable) override;
    const std::vector<ParameterSymbol*>& Parameters() const { return parameters; }
    void SetReturnType(TypeSymbol* returnType_) { returnType = returnType_; }
    TypeSymbol* ReturnType() const { return returnType; }
private:
    TypeSymbol* returnType;
    std::vector<ParameterSymbol*> parameters;
    util::uuid returnTypeId;
};

} // namespace cmajor::fault::tolerant::symbols
