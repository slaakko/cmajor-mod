// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.enumerations;

import cmajor.symbols.type.symbol;
import cmajor.symbols.function.symbol;
import soul.ast.span;
import cmajor.ast.specifier;
import cmajor.ir.emitter;
import cmajor.ir.gen.object;
import util.code.formatter;
import std.core;

export namespace cmajor::symbols {

class EnumConstantSymbol;

class EnumTypeSymbol : public TypeSymbol
{
public:
    EnumTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    std::string TypeString(Context* context) const override { return "enumerated_type"; }
    std::string Syntax(Context* context) override;
    bool IsEnumeratedType() const override { return true; }
    bool IsParentSymbol() const override { return true; }
    void Accept(SymbolCollector* collector) override;
    void CollectMembers(SymbolCollector* collector);
    void Dump(util::CodeFormatter& formatter, Context* context) override;
    void SetSpecifiers(cmajor::ast::Specifiers specifiers);
    const TypeSymbol* UnderlyingType() const { return underlyingType; }
    TypeSymbol* UnderlyingType() { return underlyingType; }
    void SetUnderlyingType(TypeSymbol* underlyingType_) { underlyingType = underlyingType_; }
    std::vector<EnumConstantSymbol*> GetEnumConstants(Context* context);
    void* IrType(cmajor::ir::Emitter& emitter, Context* context) override { return underlyingType->IrType(emitter, context); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter, Context* context) override { return underlyingType->CreateDefaultIrValue(emitter, context); }
    void* CreateDIType(cmajor::ir::Emitter& emitter, Context* context) override;
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    std::u32string Id() const override;
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "EnumTypeSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "ET"; }
    std::string GetSymbolCategoryDescription() const override { return "enumerated type"; }
private:
    TypeSymbol* underlyingType;
};

class EnumConstantSymbol : public Symbol
{
public:
    EnumConstantSymbol(const soul::ast::Span& span_, const std::u32string& name_);
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    std::string TypeString(Context* context) const override { return "enumeration_constant"; }
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter, Context* context) override;
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    bool Evaluating() const { return evaluating; }
    void SetEvaluating() { evaluating = true; }
    void ResetEvaluating() { evaluating = false; }
    const TypeSymbol* GetType() const { return static_cast<const EnumTypeSymbol*>(Parent()); }
    TypeSymbol* GetType() { return static_cast<EnumTypeSymbol*>(Parent()); }
    void SetValue(Value* value_);
    const Value* GetValue() const { return value.get(); }
    Value* GetValue() { return value.get(); }
    std::u32string Info() const override { return Name(); }
    const char* ClassName() const override { return "EnumConstantSymbol"; }
    void SetStrValue(const std::string& strValue_) { strValue = strValue_; }
    const std::string& StrValue() const { return strValue; }
    std::string GetSymbolCategoryStr() const override { return "EC"; }
    std::string GetSymbolCategoryDescription() const override { return "enumeration constant"; }
private:
    std::unique_ptr<Value> value;
    bool evaluating;
    std::string strValue;
};

class EnumTypeDefaultConstructor : public FunctionSymbol
{
public:
    EnumTypeDefaultConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    EnumTypeDefaultConstructor(EnumTypeSymbol* enumType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "EnumTypeDefaultConstructor"; }
    void Check() override;
private:
    FunctionSymbol* underlyingTypeDefaultConstructor;
};

class EnumTypeCopyConstructor : public FunctionSymbol
{
public:
    EnumTypeCopyConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    EnumTypeCopyConstructor(EnumTypeSymbol* enumType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "EnumTypeCopyConstructor"; }
    void Check() override;
private:
    FunctionSymbol* underlyingTypeCopyConstructor;
};

class EnumTypeMoveConstructor : public FunctionSymbol
{
public:
    EnumTypeMoveConstructor(const soul::ast::Span& span_, const std::u32string& name_);
    EnumTypeMoveConstructor(EnumTypeSymbol* enumType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "EnumTypeMoveConstructor"; }
    void Check() override;
private:
    FunctionSymbol* underlyingTypeMoveConstructor;
};

class EnumTypeCopyAssignment : public FunctionSymbol
{
public:
    EnumTypeCopyAssignment(const soul::ast::Span& span_, const std::u32string& name_);
    EnumTypeCopyAssignment(EnumTypeSymbol* enumType_, TypeSymbol* voidType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "EnumTypeCopyAssignment"; }
    void Check() override;
private:
    FunctionSymbol* underlyingTypeCopyAssignment;
};

class EnumTypeMoveAssignment : public FunctionSymbol
{
public:
    EnumTypeMoveAssignment(const soul::ast::Span& span_, const std::u32string& name_);
    EnumTypeMoveAssignment(EnumTypeSymbol* enumType_, TypeSymbol* voidType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "EnumTypeMoveAssignment"; }
    void Check() override;
private:
    FunctionSymbol* underlyingTypeMoveAssignment;
};

class EnumTypeReturn : public FunctionSymbol
{
public:
    EnumTypeReturn(const soul::ast::Span& span_, const std::u32string& name_);
    EnumTypeReturn(EnumTypeSymbol* enumType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "EnumTypeReturn"; }
    void Check() override;
private:
    FunctionSymbol* underlyingTypeReturn;
};

class EnumTypeEqualityOp : public FunctionSymbol
{
public:
    EnumTypeEqualityOp(const soul::ast::Span& span_, const std::u32string& name_);
    EnumTypeEqualityOp(EnumTypeSymbol* enumType_, TypeSymbol* boolType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    const char* ClassName() const override { return "EnumTypeEqualityOp"; }
    void Check() override;
private:
    FunctionSymbol* underlyingTypeEquality;
};

class EnumTypeToUnderlyingTypeConversion : public FunctionSymbol
{
public:
    EnumTypeToUnderlyingTypeConversion(const soul::ast::Span& span_, const std::u32string& name_);
    EnumTypeToUnderlyingTypeConversion(const soul::ast::Span& span_, const std::u32string& name_, TypeSymbol* sourceType_, TypeSymbol* targetType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    ConversionType GetConversionType() const override { return ConversionType::implicit_; }
    uint8_t ConversionDistance() const override { return 1; }
    const char* ClassName() const override { return "EnumTypeToUnderlyingTypeConversion"; }
    void Check() override;
private:
    TypeSymbol* sourceType;
    TypeSymbol* targetType;
};

class UnderlyingTypeToEnumTypeConversion : public FunctionSymbol
{
public:
    UnderlyingTypeToEnumTypeConversion(const soul::ast::Span& span_, const std::u32string& name_);
    UnderlyingTypeToEnumTypeConversion(const soul::ast::Span& span_, const std::u32string& name_, TypeSymbol* sourceType_, TypeSymbol* targetType_, Context* context);
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceType(TypeSymbol* typeSymbol, int index) override;
    SymbolAccess DeclaredAccess() const override { return SymbolAccess::public_; }
    void GenerateCall(cmajor::ir::Emitter& emitter, std::vector<cmajor::ir::GenObject*>& genObjects, cmajor::ir::OperationFlags flags, Context* context) override;
    bool IsBasicTypeOperation() const override { return true; }
    ConversionType GetConversionType() const override { return ConversionType::explicit_; }
    uint8_t ConversionDistance() const override { return 255; }
    const char* ClassName() const override { return "UnderlyingTypeToEnumTypeConversion"; }
    void Check() override;
private:
    TypeSymbol* sourceType;
    TypeSymbol* targetType;
};

} // namespace cmajor::symbols
