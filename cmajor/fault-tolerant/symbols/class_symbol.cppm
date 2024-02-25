// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.class_symbol;

import cmajor.fault.tolerant.symbols.type.symbol;
import util.uuid;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class InterfaceTypeSymbol;
class TemplateParameterSymbol;
class MemberVariableSymbol;
class SymbolTable;

class ClassTypeSymbol : public TypeSymbol
{
public:
    ClassTypeSymbol(const std::u16string& name_, const soul::ast::Span& span_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void Resolve(SymbolTable* symbolTable) override;
    void AddSymbol(Symbol* symbol, SymbolTable* symbolTable) override;
    const std::vector<TemplateParameterSymbol*>& TemplateParameters() const { return templateParameters; }
    const std::vector<MemberVariableSymbol*>& MemberVariables() const { return memberVariables; }
    bool IsTemplate() const { return !templateParameters.empty(); }
    void SetBaseClass(ClassTypeSymbol* baseClass_) { baseClass = baseClass_; }
    ClassTypeSymbol* BaseClass() const { return baseClass; }
    void AddInterface(InterfaceTypeSymbol* interface);
private:
    ClassTypeSymbol* baseClass;
    util::uuid baseClassId;
    std::vector<InterfaceTypeSymbol*> interfaces;
    std::vector<util::uuid> interfaceIds;
    std::vector<TemplateParameterSymbol*> templateParameters;
    std::vector<MemberVariableSymbol*> memberVariables;
};

} // namespace cmajor::fault::tolerant::symbols
