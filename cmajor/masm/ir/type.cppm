// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.ir.type;

import cmajor.masm.ir.value;
import util;
import std.core;

export namespace cmajor::masm::ir {

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
const int ptrTypeId = -13;

class Type
{
public:
    Type(int id_);
    int Id() const { return id; }
    virtual ~Type();
    virtual std::string Name() const;
    virtual Value* DefaultValue();
    void Write(util::CodeFormatter& formatter);
    virtual void WriteDeclaration(util::CodeFormatter& formatter);
    bool IsPtrType() const { return id == ptrTypeId; }
    bool IsVoidType() const { return id == voidTypeId; }
    virtual bool IsStructureType() const { return false; }
    virtual bool IsArrayType() const { return false; }
    virtual bool IsFunctionType() const { return false; }
private:
    int id;
};

class VoidType : public Type
{
public:
    VoidType();
};

class BoolType : public Type
{
public:
    BoolType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    BoolValue defaultValue;
};

class SByteType : public Type
{
public:
    SByteType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    SByteValue defaultValue;
};

class ByteType : public Type
{
public:
    ByteType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    ByteValue defaultValue;
};

class ShortType : public Type
{
public:
    ShortType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    ShortValue defaultValue;
};

class UShortType : public Type
{
public:
    UShortType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    UShortValue defaultValue;
};

class IntType : public Type
{
public:
    IntType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    IntValue defaultValue;
};

class UIntType : public Type
{
public:
    UIntType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    UIntValue defaultValue;
};

class LongType : public Type
{
public:
    LongType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    LongValue defaultValue;
};

class ULongType : public Type
{
public:
    ULongType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    ULongValue defaultValue;
};

class FloatType : public Type
{
public:
    FloatType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    FloatValue defaultValue;
};

class DoubleType : public Type
{
public:
    DoubleType();
    Value* DefaultValue() override { return &defaultValue; }
private:
    DoubleValue defaultValue;
};

class PtrType : public Type
{
public:
    PtrType(Type* baseType_);
    std::string Name() const override;
    Value* DefaultValue() override { return &defaultValue; }
    Type* BaseType() const { return baseType; }
private:
    Type* baseType;
    NullValue defaultValue;
};

class StructureType : public Type
{
public:
    StructureType(int id_);
    const std::vector<Type*>& MemberTypes() const { return memberTypes; }
    void SetMemberTypes(const std::vector<Type*>& memberTypes_);
    void WriteDeclaration(util::CodeFormatter& formatter) override;
    bool IsStructureType() const { return true; }
    Type* GetMemberType(uint64_t index) const;
private:
    std::vector<Type*> memberTypes;
};

class ArrayType : public Type
{
public:
    ArrayType(int id_, Type* elementType_, uint64_t size_);
    void WriteDeclaration(util::CodeFormatter& formatter) override;
    bool IsArrayType() const { return true; }
    Type* ElementType() const { return elementType; }
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

bool operator<(const ArrayTypeKey& left, const ArrayTypeKey& right);

class FunctionType : public Type
{
public:
    FunctionType(int id_, Type* returnType_, const std::vector<Type*>& paramTypes_);
    void WriteDeclaration(util::CodeFormatter& formatter) override;
    bool IsFunctionType() const override { return true; }
    Type* ReturnType() const { return returnType; }
    const std::vector<Type*>& ParamTypes() const { return paramTypes; }
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

bool operator<(const FunctionTypeKey& left, const FunctionTypeKey& right);

class TypeRepository
{
public:
    TypeRepository();
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
    Type* GetPtrType(Type* baseType);
    Type* GetStructureType(const std::vector<Type*>& memberTypes);
    Type* CreateStructureType();
    Type* GetArrayType(Type* elementType, uint64_t size);
    Type* GetFunctionType(Type* returnType, const std::vector<Type*>& paramTypes);
    void Write(util::CodeFormatter& formatter);
private:
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
    std::vector<std::unique_ptr<PtrType>> ptrTypes;
    std::map<Type*, PtrType*> ptrTypeMap;
    std::map<std::vector<Type*>, StructureType*> structureTypeMap;
    std::map<ArrayTypeKey, ArrayType*> arrayTypeMap;
    std::map<FunctionTypeKey, FunctionType*> functionTypeMap;
    std::vector<std::unique_ptr<Type>> types;
};

} // namespace cmajor::masm::ir
