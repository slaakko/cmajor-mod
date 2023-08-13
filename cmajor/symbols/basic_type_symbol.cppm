// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.symbols.basic.type.symbol;

import cmajor.symbols.type.symbol;
import soul.ast.source.pos;
import util.code.formatter;
import cmajor.ir.emitter;

export namespace cmajor::symbols {

class BasicTypeSymbol : public TypeSymbol
{
public:
    BasicTypeSymbol(SymbolType symbolType_, const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "basic_type"; }
    bool IsBasicTypeSymbol() const override { return true; }
    void Write(SymbolWriter& writer) override;
    void Read(SymbolReader& reader) override;
    void EmplaceFunction(FunctionSymbol* functionSymbol, int index) override;
    void Accept(SymbolCollector* collector) override;
    void Dump(util::CodeFormatter& formatter) override;
    FunctionSymbol* DefaultConstructor() { return defaultConstructor; }
    void SetDefaultConstructor(FunctionSymbol* defaultConstructor_) { defaultConstructor = defaultConstructor_; }
    FunctionSymbol* CopyConstructor() { return copyConstructor; }
    void SetCopyConstructor(FunctionSymbol* copyConstructor_) { copyConstructor = copyConstructor_; }
    FunctionSymbol* MoveConstructor() { return moveConstructor; }
    void SetMoveConstructor(FunctionSymbol* moveConstructor_) { moveConstructor = moveConstructor_; }
    FunctionSymbol* CopyAssignment() { return copyAssignment; }
    void SetCopyAssignment(FunctionSymbol* copyAssignment_) { copyAssignment = copyAssignment_; }
    FunctionSymbol* MoveAssignment() { return moveAssignment; }
    void SetMoveAssignment(FunctionSymbol* moveAssignment_) { moveAssignment = moveAssignment_; }
    FunctionSymbol* ReturnFun() { return returnFun; }
    void SetReturnFun(FunctionSymbol* returnFun_) { returnFun = returnFun_; }
    FunctionSymbol* EqualityOp() { return equalityOp; }
    void SetEqualityOp(FunctionSymbol* equalityOp_) { equalityOp = equalityOp_; }
    const char* ClassName() const override { return "BasicTypeSymbol"; }
    void Check() override;
    std::string GetSymbolCategoryStr() const override { return "BT"; }
    std::string GetSymbolCategoryDescription() const override { return "basic type"; }
private:
    FunctionSymbol* defaultConstructor;
    FunctionSymbol* copyConstructor;
    FunctionSymbol* moveConstructor;
    FunctionSymbol* copyAssignment;
    FunctionSymbol* moveAssignment;
    FunctionSymbol* returnFun;
    FunctionSymbol* equalityOp;
};

class AutoTypeSymbol : public BasicTypeSymbol
{
public:
    AutoTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "auto"; }
    const char* ClassName() const override { return "AutoTypeSymbol"; }
    void* IrType(cmajor::ir::Emitter& emitter) override;
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override;
};

class BoolTypeSymbol : public BasicTypeSymbol
{
public:
    BoolTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "bool"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForBool(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForBool(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "BoolTypeSymbol"; }
};

class SByteTypeSymbol : public BasicTypeSymbol
{
public:
    SByteTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "sbyte"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForSByte(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForSByte(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsIntegralType() const override { return true; }
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "SByteTypeSymbol"; }
};

class ByteTypeSymbol : public BasicTypeSymbol
{
public:
    ByteTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "byte"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForByte(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForByte(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsIntegralType() const override { return true; }
    bool IsUnsignedType() const override { return true; }
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "ByteTypeSymbol"; }
};

class ShortTypeSymbol : public BasicTypeSymbol
{
public:
    ShortTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "short"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForShort(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForShort(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsIntegralType() const override { return true; }
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "ShortTypeSymbol"; }
};

class UShortTypeSymbol : public BasicTypeSymbol
{
public:
    UShortTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "ushort"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForUShort(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForUShort(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsIntegralType() const override { return true; }
    bool IsUnsignedType() const override { return true; }
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "UShortTypeSymbol"; }
};

class IntTypeSymbol : public BasicTypeSymbol
{
public:
    IntTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "int"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForInt(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForInt(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsIntegralType() const override { return true; }
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "IntTypeSymbol"; }
};

class UIntTypeSymbol : public BasicTypeSymbol
{
public:
    UIntTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "uint"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForUInt(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForUInt(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsIntegralType() const override { return true; }
    bool IsUnsignedType() const override { return true; }
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "UIntTypeSymbol"; }
};

class LongTypeSymbol : public BasicTypeSymbol
{
public:
    LongTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "long"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForLong(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForLong(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsIntegralType() const override { return true; }
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "LongTypeSymbol"; }
};

class ULongTypeSymbol : public BasicTypeSymbol
{
public:
    ULongTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "ulong"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForULong(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForULong(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsIntegralType() const override { return true; }
    bool IsUnsignedType() const override { return true; }
    bool IsSwitchConditionType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "ULongTypeSymbol"; }
};

class FloatTypeSymbol : public BasicTypeSymbol
{
public:
    FloatTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "float"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForFloat(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForFloat(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsFloatingPointType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "FloatTypeSymbol"; }
};

class DoubleTypeSymbol : public BasicTypeSymbol
{
public:
    DoubleTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "double"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForDouble(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForDouble(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsFloatingPointType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "DoubleTypeSymbol"; }
};

class CharTypeSymbol : public BasicTypeSymbol
{
public:
    CharTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "char"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForChar(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForChar(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsSwitchConditionType() const override { return true; }
    bool IsUnsignedType() const override { return true; }
    bool IsCharacterType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "CharTypeSymbol"; }
};

class WCharTypeSymbol : public BasicTypeSymbol
{
public:
    WCharTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "wchar"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForWChar(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForWChar(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsSwitchConditionType() const override { return true; }
    bool IsUnsignedType() const override { return true; }
    bool IsCharacterType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "WCharTypeSymbol"; }
};

class UCharTypeSymbol : public BasicTypeSymbol
{
public:
    UCharTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "uchar"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForUChar(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { return emitter.CreateDefaultIrValueForUChar(); }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsSwitchConditionType() const override { return true; }
    bool IsUnsignedType() const override { return true; }
    bool IsCharacterType() const override { return true; }
    ValueType GetValueType() const override;
    Value* MakeValue() const override;
    const char* ClassName() const override { return "UCharTypeSymbol"; }
};

class VoidTypeSymbol : public BasicTypeSymbol
{
public:
    VoidTypeSymbol(const soul::ast::SourcePos& sourcePos_, const util::uuid& sourceModuleId_, const std::u32string& name_);
    std::string TypeString() const override { return "void"; }
    void* IrType(cmajor::ir::Emitter& emitter) override { return emitter.GetIrTypeForVoid(); }
    void* CreateDefaultIrValue(cmajor::ir::Emitter& emitter) override { Assert(false, "tried to create default value for void type"); return nullptr; }
    void* CreateDIType(cmajor::ir::Emitter& emitter) override;
    bool IsVoidType() const override { return true; }
    const char* ClassName() const override { return "VoidTypeSymbol"; }
};
} // namespace cmajor::symbols
