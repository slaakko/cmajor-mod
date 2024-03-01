// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.symbols.templates;

import cmajor.symbols.type.symbol;
import soul.ast.span;
import soul.xml.element; 
import cmajor.ir.emitter;
import std.core;

export namespace cmajor::symbols {

class TemplateParameterSymbol : public TypeSymbol
{
public:
    TemplateParameterSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    std::u32string FullName() const override { return Name(); }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void* IrType(cmajor::ir::Emitter& emitter) override 
    { 
        Assert(false, "tried to get ir type of template parameter"); 
        return nullptr; 
    }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override 
    {
        Assert(false, "tried to create defualt ir value of template parameter"); 
        return nullptr; 
    }
    TypeSymbol* Unify(TypeSymbol* type) override;
    bool ContainsTemplateParameter() const override { return true; }
    bool HasDefault() const { return hasDefault; }
    void SetHasDefault() { hasDefault = true; }
    void SetDefaultType(TypeSymbol* defaultType_) { defaultType = defaultType_; }
    TypeSymbol* DefaultType() { return defaultType; }
    TypeSymbol* UnifyTemplateArgumentType(SymbolTable& symbolTable, const std::map<TemplateParameterSymbol*, TypeSymbol*>& templateParameterMap) override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "TemplateParameterSymbol"; }
    std::string GetSymbolCategoryStr() const override { return "TP"; }
    std::string GetSymbolCategoryDescription() const override { return "template parameter"; }
private:
    bool hasDefault;
    TypeSymbol* defaultType;
};

class BoundTemplateParameterSymbol : public Symbol
{
public:
    BoundTemplateParameterSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    std::u32string FullName() const override { return Name(); }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    TypeSymbol* GetType() const { return type; }
    void SetType(TypeSymbol* type_) { type = type_; }
    std::unique_ptr<soul::xml::Element> CreateDomElement(TypeMap& typeMap) override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "BoundTemplateParameterSymbol"; }
    void Check() override;
private:
    TypeSymbol* type;
};

} // namespace cmajor::symbols
