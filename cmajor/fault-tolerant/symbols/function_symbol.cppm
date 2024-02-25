// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.function.symbol;

import cmajor.fault.tolerant.symbols.container.symbol;
import util.uuid;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class TemplateParameterSymbol;
class ParameterSymbol;
class LocalVariableSymbol;

class FunctionSymbol : public ContainerSymbol
{
public:
    FunctionSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    FunctionSymbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve(SymbolTable* symbolTable) override;
    void AddSymbol(Symbol* symbol, SymbolTable* symbolTable) override;
    const std::vector<TemplateParameterSymbol*>& TemplateParameters() const { return templateParameters; }
    const std::vector<ParameterSymbol*>& Parameters() const { return parameters; }
    const std::vector<LocalVariableSymbol*>& LocalVariables() const { return localVariables; }
    void SetReturnType(TypeSymbol* returnType_) { returnType = returnType_; }
    TypeSymbol* ReturnType() const { return returnType; }
    bool IsTemplate() const { return !templateParameters.empty(); }
private:
    std::vector<TemplateParameterSymbol*> templateParameters;
    std::vector<ParameterSymbol*> parameters;
    std::vector<LocalVariableSymbol*> localVariables;
    TypeSymbol* returnType;
    util::uuid returnTypeId;
};

class StaticConstructorSymbol : public FunctionSymbol
{
public:
    StaticConstructorSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

class ConstructorSymbol : public FunctionSymbol
{
public:
    ConstructorSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

class DestructorSymbol : public FunctionSymbol
{
public:
    DestructorSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

class MemberFunctionSymbol : public FunctionSymbol
{
public:
    MemberFunctionSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

class ConversionFunctionSymbol : public FunctionSymbol
{
public:
    ConversionFunctionSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
};

} // namespace cmajor::fault::tolerant::symbols
