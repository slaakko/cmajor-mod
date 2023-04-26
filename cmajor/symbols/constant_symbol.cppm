export module cmajor.symbols.constant.symbol;

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import cmajor.symbols.symbol;
import cmajor.ast.specifier;
import soul.ast.source.pos;
import util.code.formatter;
import std.core;

export namespace cmajor::symbols {

class Value;

class ConstantSymbol : public Symbol
{
public:
    ConstantSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter) override;
    std::string TypeString() const override { return "constant"; }
    std::string Syntax() const override;
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    void ComputeMangledName() override;
    bool Evaluating() const { return evaluating; }
    void SetEvaluating() { evaluating = true; }
    void ResetEvaluating() { evaluating = false; }
    const TypeSymbol* GetType() const { return type; }
    TypeSymbol* GetType() { return type; }
    void SetType(TypeSymbol* typeSymbol) { type = typeSymbol; }
    void SetValue(Value* value_);
    Value* GetValue();
    void* ArrayIrObject(cmajor::ir::Emitter& emitter, bool create);
    void* StructureIrObject(cmajor::ir::Emitter& emitter, bool create);
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "ConstantSymbol"; }
    void SetStrValue(const std::u32string& strValue_) { strValue = strValue_; }
    const std::u32string& StrValue() const { return strValue; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "CN"; }
    std::string GetSymbolCategoryDescription() const override { return "constant"; }
    std::string GetSymbolHelp() const override;
private:
    TypeSymbol* type;
    std::unique_ptr<Value> value;
    bool evaluating;
    uint32_t sizeOfValue;
    uint32_t valuePos;
    std::string filePathReadFrom;
    std::u32string strValue;
};
} // namespace cmajor::symbols
