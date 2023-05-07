// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.typedefs;

import cmajor.symbols.symbol;
import soul.ast.source.pos;
import cmajor.ast.specifier;
import util.code.formatter;
import soul.xml.element;
import std.core;

export namespace cmajor::symbols {

class TypedefSymbol : public Symbol
{
public:
    TypedefSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    bool IsExportSymbol() const override;
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter) override;
    std::string TypeString() const override { return "typedef"; }
    std::string Syntax() const override;
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    const TypeSymbol* GetType() const { return type; }
    TypeSymbol* GetType() { return type; }
    void SetType(TypeSymbol* typeSymbol) { type = typeSymbol; }
    std::unique_ptr<soul::xml::Element> CreateDomElement(TypeMap& typeMap) override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "TypedefSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "TY"; }
    std::string GetSymbolCategoryDescription() const override { return "typedef"; }
    std::string GetSymbolHelp() const override;
private:
    TypeSymbol* type;
};

} // namespace cmajor::symbols
