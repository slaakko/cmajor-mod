// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.cpp.ir.type;

import cmajor.cpp.ir.value;
import util;
import std.core;

export namespace cmajor::cpp::ir {

const int voidTypeId = -1;
const int boolTypeId = -2;
const int sbyteTypeId = -3;
const int byteTypeId = -4;
const int shortTypeId = -5;
const int ushortTypeId = -6;
const int intTypeId = -7;
const int uintTypeId = -8;
const int longTypeId = -9;
const int ulongTypeId = -10;
const int floatTypeId = -11;
const int doubleTypeId = -12;
const int charTypeId = -13;
const int wcharTypeId = -14;
const int ucharTypeId = -15;
const int ptrTypeId = -16;

std::string TypeName(int typeId, const std::string& compileUnitId);

class Type
{
public:
    Type(int id_, Context& context_);
    virtual ~Type();
    virtual std::string Name() const;
    virtual ConstantValue* DefaultValue();
    virtual void WriteForwardDeclaration(util::CodeFormatter& formatter);
    virtual void WriteDeclaration(util::CodeFormatter& formatter);
    virtual bool IsPrimitiveType() const { return false; }
    virtual bool IsStructureType() const { return false; }
    virtual bool IsArrayType() const { return false; }
    virtual bool IsFunctionType() const { return false; }
    virtual int SizeInBytes() const = 0;
    bool IsPtrType() const { return id == ptrTypeId; }
    bool IsVoidType() const { return id == voidTypeId; }
    int Id() const { return id; }
    void Write(util::CodeFormatter& formatter);
    Context& GetContext() { return context; }
private:
    Context& context;
    int id;
};

class PrimitiveType : public Type
{
public:
    PrimitiveType(int id, Context& context);
    bool IsPrimitiveType() const override { return true; }
};

class VoidType : public PrimitiveType
{
public:
    VoidType(Context& context);
    int SizeInBytes() const override { return 0; }
};

class BoolType : public PrimitiveType
{
public:
    BoolType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 1; }
private:
    BoolValue defaultValue;
};

class SByteType : public PrimitiveType
{
public:
    SByteType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 1; }
private:
    SByteValue defaultValue;
};

class ByteType : public PrimitiveType
{
public:
    ByteType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 1; }
private:
    ByteValue defaultValue;
};

class ShortType : public PrimitiveType
{
public:
    ShortType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 2; }
private:
    ShortValue defaultValue;
};

class UShortType : public PrimitiveType
{
public:
    UShortType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 2; }
private:
    UShortValue defaultValue;
};

class IntType : public PrimitiveType
{
public:
    IntType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 4; }
private:
    IntValue defaultValue;
};

class UIntType : public PrimitiveType
{
public:
    UIntType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 4; }
private:
    UIntValue defaultValue;
};

class LongType : public PrimitiveType
{
public:
    LongType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 8; }
private:
    LongValue defaultValue;
};

class ULongType : public PrimitiveType
{
public:
    ULongType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 8; }
private:
    ULongValue defaultValue;
};

class FloatType : public PrimitiveType
{
public:
    FloatType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 4; }
private:
    FloatValue defaultValue;
};

class DoubleType : public PrimitiveType
{
public:
    DoubleType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 8; }
private:
    DoubleValue defaultValue;
};

class CharType : public PrimitiveType
{
public:
    CharType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 8; }
private:
    CharValue defaultValue;
};

class WCharType : public PrimitiveType
{
public:
    WCharType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 16; }
private:
    WCharValue defaultValue;
};

class UCharType : public PrimitiveType
{
public:
    UCharType(Context& context);
    ConstantValue* DefaultValue() override { return &defaultValue; }
    int SizeInBytes() const override { return 32; }
private:
    UCharValue defaultValue;
};

class PtrType : public Type
{
public:
    PtrType(Type* baseType_, Context& context);
    std::string Name() const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
    Type* BaseType() const { return baseType; }
    int SizeInBytes() const override { return 8; }
private:
    Type* baseType;
    NullValue defaultValue;
};

class StructureType : public Type
{
public:
    StructureType(int id_, Context& context);
    const std::vector<Type*>& MemberTypes() const { return memberTypes; }
    void SetMemberTypes(const std::vector<Type*>& memberTypes_);
    void WriteForwardDeclaration(util::CodeFormatter& formatter) override;
    void WriteDeclaration(util::CodeFormatter& formatter) override;
    bool IsStructureType() const override { return true; }
    Type* GetMemberType(uint64_t index) const;
    int SizeInBytes() const override;
private:
    std::vector<Type*> memberTypes;
};

struct StructureTypeHash
{
    size_t operator()(const std::vector<Type*>& memberTypes) const;
};

struct StructureTypeEqual
{
    size_t operator()(const std::vector<Type*>& leftMemberTypes, const std::vector<Type*>& rightMemberTypes) const;
};

class ArrayType : public Type
{
public:
    ArrayType(int id_, Type* elementType_, uint64_t size_, Context& context);
    void WriteDeclaration(util::CodeFormatter& formatter) override;
    bool IsArrayType() const override { return true; }
    Type* ElementType() const { return elementType; }
    int SizeInBytes() const override;
    uint64_t Size() const { return size; }
private:
    Type* elementType;
    uint64_t size;
};

struct ArrayTypeKey
{
    ArrayTypeKey(Type* elementType_, uint64_t size_) : elementType(elementType_), size(size_) {}
    Type* elementType;
    uint64_t size;
};

struct ArrayTypeKeyHash
{
    size_t operator()(const ArrayTypeKey& key) const;
};

struct ArrayTypeKeyEqual
{
    size_t operator()(const ArrayTypeKey& left, const ArrayTypeKey& right) const;
};

class FunctionType : public Type
{
public:
    FunctionType(int id_, Type* returnType_, const std::vector<Type*>& paramTypes_, Context& context);
    void WriteDeclaration(util::CodeFormatter& formatter) override;
    bool IsFunctionType() const override { return true; }
    Type* ReturnType() const { return returnType; }
    const std::vector<Type*>& ParamTypes() const { return paramTypes; }
    int SizeInBytes() const override { return 0; }
private:
    Type* returnType;
    std::vector<Type*> paramTypes;
};

struct FunctionTypeKey
{
    FunctionTypeKey(Type* returnType_, const std::vector<Type*>& paramTypes_);
    Type* returnType;
    std::vector<Type*> paramTypes;
};

struct FunctionTypeKeyHash
{
    size_t operator()(const FunctionTypeKey& key) const;
};

struct FunctionTypeKeyEqual
{
    size_t operator()(const FunctionTypeKey& left, const FunctionTypeKey& right) const;
};

class TypeRepository
{
public:
    TypeRepository(Context& context_);
    TypeRepository(const TypeRepository&) = delete;
    TypeRepository& operator=(const TypeRepository&) = delete;
    void Write(util::CodeFormatter& formatter);
    Type* GetVoidType() { return &voidType; }
    Type* GetBoolType() { return &boolType; }
    Type* GetSByteType() { return &sbyteType; }
    Type* GetByteType() { return &byteType; }
    Type* GetShortType() { return &shortType; }
    Type* GetUShortType() { return &ushortType; }
    Type* GetIntType() { return &intType; }
    Type* GetUIntType() { return &uintType; }
    Type* GetLongType() { return &longType; }
    Type* GetULongType() { return &ulongType; }
    Type* GetFloatType() { return &floatType; }
    Type* GetDoubleType() { return &doubleType; }
    Type* GetCharType() { return &charType; }
    Type* GetWCharType() { return &wcharType; }
    Type* GetUCharType() { return &ucharType; }
    Type* GetPtrType(Type* baseType);
    Type* GetStructureType(const std::vector<Type*>& memberTypes);
    Type* CreateStructureType();
    Type* GetArrayType(Type* elementType, uint64_t size);
    Type* GetFunctionType(Type* returnType, const std::vector<Type*>& paramTypes);
private:
    Context& context;
    VoidType voidType;
    BoolType boolType;
    SByteType sbyteType;
    ByteType byteType;
    ShortType shortType;
    UShortType ushortType;
    IntType intType;
    UIntType uintType;
    LongType longType;
    ULongType ulongType;
    FloatType floatType;
    DoubleType doubleType;
    CharType charType;
    WCharType wcharType;
    UCharType ucharType;
    std::vector<std::unique_ptr<PtrType>> ptrTypes;
    std::unordered_map<Type*, PtrType*> ptrTypeMap;
    std::unordered_map<std::vector<Type*>, StructureType*, StructureTypeHash, StructureTypeEqual> structureTypeMap;
    std::unordered_map<ArrayTypeKey, ArrayType*, ArrayTypeKeyHash, ArrayTypeKeyEqual> arrayTypeMap;
    std::unordered_map<FunctionTypeKey, FunctionType*, FunctionTypeKeyHash, FunctionTypeKeyEqual> functionTypeMap;
    std::vector<std::unique_ptr<Type>> types;
};

} // namespace cmajor::cpp::ir
