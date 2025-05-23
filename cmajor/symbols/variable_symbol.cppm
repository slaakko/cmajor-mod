// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.variable.symbol;

import cmajor.ast.specifier;
import cmajor.ast.parameter;
import cmajor.ir.emitter;
import soul.ast.span;
import soul.xml.element;
import cmajor.symbols.symbol;
import cmajor.symbols.value;
import util.code.formatter;
import std.core;

export namespace cmajor::symbols {

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_);
    bool IsVariableSymbol() const override { return true; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    const TypeSymbol* GetType() const { return type; }
    TypeSymbol* GetType() { return type; }
    void SetType(TypeSymbol* typeSymbol) { type = typeSymbol; }
    const ContainerScope* GetTypeScope() const override;
    ContainerScope* GetTypeScope() override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "VariableSymbol"; }
    void Check() override;
    std::string GetSymbolHelp() const override;
private:
    TypeSymbol* type;
};

class ParameterSymbol : public VariableSymbol
{
public:
    ParameterSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsExportSymbol() const override;
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    std::string TypeString(Context* context) const override { return "parameter"; }
    std::unique_ptr<soul::xml::Element> CreateDomElement(TypeMap& typeMap) override;
    const char* ClassName() const override { return "ParameterSymbol"; }
    bool ArtificialName() const { return artificialName; }
    void SetArtificialName() { artificialName = true; }
    std::u32string CodeName() const override;
    ParameterSymbol* Clone() const;
    std::string GetSymbolCategoryStr() const override { return "PA"; }
    std::string GetSymbolCategoryDescription() const override { return "parameter"; }
    std::string GetSymbolHelp() const override;
private:
    bool artificialName;
};

class LocalVariableSymbol : public VariableSymbol
{
public:
    LocalVariableSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    bool IsExportSymbol() const override { return false; }
    std::unique_ptr<soul::xml::Element> CreateDomElement(TypeMap& typeMap) override;
    const char* ClassName() const override { return "LocalVariableSymbol"; }
    std::string GetSymbolCategoryStr() const override { return "LV"; }
    std::string GetSymbolCategoryDescription() const override { return "local variable"; }
    std::string GetSymbolHelp() const override;
    bool IsInitialized() const { return initialized; }
    void SetInitialized() { initialized = true; }
    bool IsTemporary() const { return temporary; }
    void SetTemporary() { temporary = true; }
private:
    bool initialized;
    bool temporary;
};

class MemberVariableSymbol : public VariableSymbol
{
public:
    MemberVariableSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsExportSymbol() const override;
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter, Context* context) override;
    std::string TypeString(Context* context) const override { return "member_variable"; }
    std::string Syntax(Context* context) override;
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    int32_t LayoutIndex() const { return layoutIndex; }
    void SetLayoutIndex(int32_t layoutIndex_) { layoutIndex = layoutIndex_; }
    void* GetDIMemberType(cmajor::ir::Emitter& emitter, uint64_t offsetInBits, Context* context);
    std::unique_ptr<soul::xml::Element> CreateDomElement(TypeMap& typeMap) override;
    const char* ClassName() const override { return "MemberVariableSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "MV"; }
    std::string GetSymbolCategoryDescription() const override { return "member variable"; }
private:
    int32_t layoutIndex;
};

class GlobalVariableSymbol;

class GlobalVariableGroupSymbol : public Symbol
{
public:
    GlobalVariableGroupSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    bool IsExportSymbol() const override { return false; }
    std::string TypeString(Context* context) const override { return "global_variable_group"; }
    void ComputeMangledName(Context* context) override;
    void AddGlobalVariable(GlobalVariableSymbol* globalVariableSymbol);
    void RemoveGlobalVariable(GlobalVariableSymbol* globalVariableSymbol);
    bool IsEmpty() const;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "GlobalVariableGroupSymbol"; }
    void CollectGlobalVariables(const std::string& compileUnitFilePath, std::vector<GlobalVariableSymbol*>& globalVariables) const;
    std::string GetSymbolCategoryStr() const override { return "GV"; }
    std::string GetSymbolCategoryDescription() const override { return "global variable"; }
    const ContainerScope* GetTypeScope() const override;
    ContainerScope* GetTypeScope() override;
    std::string GetSymbolHelp() const override;
private:
    std::vector<std::pair<GlobalVariableSymbol*, std::string>> globalVariableSymbols;
};

class GlobalVariableSymbol : public VariableSymbol
{
public:
    GlobalVariableSymbol(const soul::ast::Span& span_, const std::u32string& groupName_, 
       const std::string& compileUnitId, const std::string& compileUnitFilePath_);
    GlobalVariableSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool IsExportSymbol() const override;
    bool IsGlobalVariableSymbol() const override { return true; }
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter, Context* context) override;
    std::string TypeString(Context* context) const override { return "global_variable"; }
    void ComputeMangledName(Context* context) override;
    std::string Syntax(Context* context) override;
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    const char* ClassName() const override { return "GlobalVariableSymbol"; }
    void SetInitializer(std::unique_ptr<Value>&& initializer_);
    Value* Initializer() const { return initializer.get(); }
    void* IrObject(cmajor::ir::Emitter& emitter, Context* context) override;
    void CreateIrObject(cmajor::ir::Emitter& emitter, Context* context);
    const std::u32string& GroupName() const { return groupName; }
    const std::string& CompileUnitFilePath() const { return compileUnitFilePath; }
    void SetGlobalVariableGroup(GlobalVariableGroupSymbol* globalVariableGroup_) { globalVariableGroup = globalVariableGroup_; }
    std::unique_ptr<Symbol> RemoveFromParent() override;
    std::string GetSymbolCategoryStr() const override { return "GV"; }
    std::string GetSymbolCategoryDescription() const override { return "global variable"; }
private:
    std::u32string groupName;
    std::string compileUnitFilePath;
    std::unique_ptr<Value> initializer;
    GlobalVariableGroupSymbol* globalVariableGroup;
};
} // namespace cmajor::symbols
