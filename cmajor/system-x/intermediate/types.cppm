// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.types;

import cmajor.systemx.intermediate.value;
import cmajor.systemx.assembler;
import soul.ast.source.pos;
import util;
import std.core;

export namespace cmajor::systemx::intermediate {

export namespace types {}

class Context;
class Types;
class ConstantValue;
class Visitor;
class ArrayType;
class StructureType;
class MetadataRef;

const int32_t voidTypeId = 0;
const int32_t boolTypeId = 1;
const int32_t sbyteTypeId = 2;
const int32_t byteTypeId = 3;
const int32_t shortTypeId = 4;
const int32_t ushortTypeId = 5;
const int32_t intTypeId = 6;
const int32_t uintTypeId = 7;
const int32_t longTypeId = 8;
const int32_t ulongTypeId = 9;
const int32_t floatTypeId = 10;
const int32_t doubleTypeId = 11;
const int32_t userTypeId = 12;
const int32_t pointerTypeId = int32_t(1) << 30;

constexpr bool IsFundamentalTypeId(int32_t typeId) { return typeId >= 0 && typeId < userTypeId; }
constexpr bool IsPointerTypeId(int32_t typeId) { return (typeId & pointerTypeId) != 0; }

const int8_t notSystemType = 0;
const int8_t stringType = 1;
const int8_t listType = 2;
const int8_t setType = 3;
const int8_t mapType = 4;
const int8_t linkedListType = 5;
const int8_t hashSetType = 6;
const int8_t hashMapType = 7;
const int8_t forwardListType = 8;

inline void SetSystemType(int32_t& typeId, int8_t systemType)
{
    typeId |= static_cast<int32_t>(systemType) << 20;
}

inline int8_t GetSystemType(int32_t typeId)
{
    return (typeId >> 20) & 0x0F;
}

inline int32_t MakeUserTypeId(const std::string& typeIdStr)
{
    return userTypeId + std::stoi(typeIdStr.substr(2));
}

constexpr int32_t MakeUserTypeId(int32_t typeId)
{
    return typeId - userTypeId;
}

constexpr int32_t MakePointerTypeId(int32_t baseTypeId, int8_t pointerCount)
{
    return pointerTypeId | (int32_t(pointerCount) << (32 - 8)) | baseTypeId;
}

constexpr int32_t MakeTypeId(int32_t baseTypeId, int32_t pointerCount)
{
    if (pointerCount == 0)
    {
        return baseTypeId;
    }
    else
    {
        return MakePointerTypeId(baseTypeId, pointerCount);
    }
}

constexpr int32_t GetBaseTypeId(int32_t typeId)
{
    return typeId & (~(pointerTypeId | (int32_t(0x3F) << (32 - 8))) | 0x00FFFFFF);
}

constexpr int8_t GetPointerCount(int32_t typeId)
{
    return (typeId & (int32_t(0x3F) << (32 - 8))) >> (32 - 8);
}

enum class TypeKind : int
{
    fundamentalType, structureType, arrayType, functionType, pointerType
};

class Type
{
public:
    Type(const soul::ast::SourcePos& sourcePos_, TypeKind kind_, int32_t id_);
    virtual ~Type();
    virtual std::string Name() const;
    virtual void Accept(Visitor& visitor) {}
    virtual int64_t Size() const = 0;
    virtual int64_t Alignment() const = 0;
    TypeKind Kind() const { return kind; }
    bool IsFundamentalType() const { return kind == TypeKind::fundamentalType; }
    bool IsVoidType() const { return id == voidTypeId; }
    bool IsBooleanType() const { return id == boolTypeId; }
    bool IsCompoundType() const { return !IsFundamentalType(); }
    bool IsArithmeticType() const { return IsIntegerType() || IsFloatingPointType(); }
    bool IsIntegerType() const;
    bool IsUnsignedType() const;
    bool IsFloatingPointType() const;
    bool IsFloatType() const { return id == floatTypeId; }
    bool IsDoubleType() const { return id == doubleTypeId; }
    bool IsPointerType() const { return kind == TypeKind::pointerType; }
    int8_t GetSystemType() const { return cmajor::systemx::intermediate::GetSystemType(id); }
    void SetSystemType(int8_t systemType) { cmajor::systemx::intermediate::SetSystemType(id, systemType); }
    Type* AddPointer(Context* context);
    Type* RemovePointer(const soul::ast::SourcePos& sourcePos, Context* context) const;
    bool IsStructureType() const { return kind == TypeKind::structureType; }
    StructureType* GetStructurePointeeType(const soul::ast::SourcePos& sourcePos, Context* context) const;
    ArrayType* GetArrayPointeeType(const soul::ast::SourcePos& sourcePos, Context* context) const;
    bool IsArrayType() const { return kind == TypeKind::arrayType; }
    bool IsFunctionType() const { return kind == TypeKind::functionType; }
    virtual bool IsWeakType() const { return true; }
    virtual void Add(Types* types, Context* context);
    virtual void Resolve(Types* types, Context* context);
    virtual ConstantValue* DefaultValue();
    virtual cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const;
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    int32_t Id() const { return id; }
    void Write(util::CodeFormatter& formatter);
    virtual void WriteDeclaration(util::CodeFormatter& formatter);
private:
    soul::ast::SourcePos sourcePos;
    TypeKind kind;
    int32_t id;
};

class VoidType : public Type
{
public:
    VoidType();
    int64_t Size() const override { return -1; }
    int64_t Alignment() const override { return -1; }
};

class BoolType : public Type
{
public:
    BoolType();
    int64_t Size() const override { return 1; }
    int64_t Alignment() const override { return 1; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    BoolValue defaultValue;
};

class SByteType : public Type
{
public:
    SByteType();
    int64_t Size() const override { return 1; }
    int64_t Alignment() const override { return 1; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    SByteValue defaultValue;
};

class ByteType : public Type
{
public:
    ByteType();
    int64_t Size() const override { return 1; }
    int64_t Alignment() const override { return 1; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    ByteValue defaultValue;
};

class ShortType : public Type
{
public:
    ShortType();
    int64_t Size() const override { return 2; }
    int64_t Alignment() const override { return 2; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    ShortValue defaultValue;
};

class UShortType : public Type
{
public:
    UShortType();
    int64_t Size() const override { return 2; }
    int64_t Alignment() const override { return 2; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    UShortValue defaultValue;
};

class IntType : public Type
{
public:
    IntType();
    int64_t Size() const override { return 4; }
    int64_t Alignment() const override { return 4; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    IntValue defaultValue;
};

class UIntType : public Type
{
public:
    UIntType();
    int64_t Size() const override { return 4; }
    int64_t Alignment() const override { return 4; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    UIntValue defaultValue;
};

class LongType : public Type
{
public:
    LongType();
    int64_t Size() const override { return 8; }
    int64_t Alignment() const override { return 8; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    LongValue defaultValue;
};

class ULongType : public Type
{
public:
    ULongType();
    int64_t Size() const override { return 8; }
    int64_t Alignment() const override { return 8; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    ULongValue defaultValue;
};

class FloatType : public Type
{
public:
    FloatType();
    int64_t Size() const override { return 4; }
    int64_t Alignment() const override { return 4; }
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    FloatValue defaultValue;
};

class DoubleType : public Type
{
public:
    DoubleType();
    int64_t Size() const override { return 8; }
    int64_t Alignment() const override { return 8; }
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    DoubleValue defaultValue;
};

class TypeRef
{
public:
    TypeRef();
    TypeRef(const soul::ast::SourcePos& sourcePos_, int32_t id_);
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    int32_t Id() const { return id; }
    void SetType(Type* type_) { type = type_; }
    Type* GetType() const { return type; }
private:
    soul::ast::SourcePos sourcePos;
    int32_t id;
    Type* type;
};

inline TypeRef MakeTypeRef(const soul::ast::SourcePos& sourcePos, int32_t baseTypeId, int32_t pointerCount)
{
    return TypeRef(sourcePos, MakeTypeId(baseTypeId, pointerCount));
}

class StructureType : public Type
{
public:
    StructureType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, const std::vector<TypeRef>& fieldTypeRefs_);
    StructureType(int32_t typeId_, const std::vector<Type*>& memberTypes);
    StructureType(int32_t typeId_);
    void Accept(Visitor& visitor) override;
    void Add(Types* types, Context* context) override;
    void Resolve(Types* types, Context* context) override;
    int64_t Size() const override;
    int64_t Alignment() const override { return 8; }
    bool IsWeakType() const override;
    int FieldCount() const { return fieldTypeRefs.size(); }
    const std::vector<TypeRef>& FieldTypeRefs() const { return fieldTypeRefs; }
    Type* FieldType(int i) const { return fieldTypeRefs[i].GetType(); }
    int64_t GetFieldOffset(int64_t index) const;
    void SetMemberTypes(const std::vector<Type*>& memberTypes);
    void SetMetadataRef(MetadataRef* metadataRef_) { metadataRef = metadataRef_; }
    MetadataRef* GetMetadataRef() const { return metadataRef; }
    void WriteDeclaration(util::CodeFormatter& formatter) override;
private:
    void ComputeSizeAndOffsets() const;
    std::vector<TypeRef> fieldTypeRefs;
    mutable bool sizeAndOffsetsComputed;
    mutable int64_t size;
    mutable std::vector<int64_t> fieldOffsets;
    MetadataRef* metadataRef;
};

struct ArrayTypeKey
{
    ArrayTypeKey(Type* elementType_, uint64_t size_) : elementType(elementType_), size(size_) {}
    Type* elementType;
    uint64_t size;
};

bool operator<(const ArrayTypeKey& left, const ArrayTypeKey& right);

class ArrayType : public Type
{
public:
    ArrayType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, int64_t elementCount_, const TypeRef& elementTypeRef_);
    ArrayType(int32_t typeId_, int64_t elementCount_, Type* elementType_);
    void Accept(Visitor& visitor) override;
    void Add(Types* types, Context* context) override;
    void Resolve(Types* types, Context* context) override;
    int64_t Size() const override;
    int64_t Alignment() const override { return 8; }
    bool IsWeakType() const override;
    int64_t ElementCount() const { return elementCount; }
    const TypeRef& ElementTypeRef() const { return elementTypeRef; }
    Type* ElementType() const { return elementTypeRef.GetType(); }
    void SetMetadataRef(MetadataRef* metadataRef_) { metadataRef = metadataRef_; }
    MetadataRef* GetMetadataRef() const { return metadataRef; }
    void WriteDeclaration(util::CodeFormatter& formatter) override;
private:
    int64_t elementCount;
    TypeRef elementTypeRef;
    MetadataRef* metadataRef;
};

struct FunctionTypeKey
{
    FunctionTypeKey(Type* returnType_, const std::vector<Type*>& paramTypes_);
    Type* returnType;
    std::vector<Type*> paramTypes;
};

bool operator<(const FunctionTypeKey& left, const FunctionTypeKey& right);

class FunctionType : public Type
{
public:
    FunctionType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, const TypeRef& returnTypeRef_, const std::vector<TypeRef>& paramTypeRefs_);
    FunctionType(int32_t typeId_, Type* returnType_, const std::vector<Type*>& paramTypes_);
    void Accept(Visitor& visitor) override;
    void Add(Types* types, Context* context) override;
    void Resolve(Types* types, Context* context) override;
    bool IsWeakType() const override;
    int Arity() const { return paramTypeRefs.size(); }
    int64_t Size() const override { return 8; }
    int64_t Alignment() const override { return 8; }
    const TypeRef& ReturnTypeRef() const { return returnTypeRef; }
    Type* ReturnType() const { return returnTypeRef.GetType(); }
    const std::vector<TypeRef>& ParamTypeRefs() const { return paramTypeRefs; }
    Type* ParamType(int index) const { return paramTypeRefs[index].GetType(); }
    void SetMetadataRef(MetadataRef* metadataRef_) { metadataRef = metadataRef_; }
    MetadataRef* GetMetadataRef() const { return metadataRef; }
    void WriteDeclaration(util::CodeFormatter& formatter) override;
private:
    TypeRef returnTypeRef;
    std::vector<TypeRef> paramTypeRefs;
    MetadataRef* metadataRef;
};

class PointerType : public Type
{
public:
    PointerType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, int8_t pointerCount_, int32_t baseTypeId_);
    std::string Name() const override;
    int64_t Size() const override { return 8; }
    int64_t Alignment() const override { return 8; }
    int8_t PointerCount() const { return pointerCount; }
    const TypeRef& BaseTypeRef() const { return baseTypeRef; }
    TypeRef& BaseTypeRef() { return baseTypeRef; }
    Type* BaseType() const { return baseTypeRef.GetType(); }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
    ConstantValue* DefaultValue() override { return &defaultValue; }
private:
    int8_t pointerCount;
    TypeRef baseTypeRef;
    NullValue defaultValue;
};

class Types
{
public:
    Types();
    Types(const Types&) = delete;
    Types& operator=(const Types&) = delete;
    Context* GetContext() const { return context; }
    void SetContext(Context* context_) { context = context_; }
    void AddStructureType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs, MetadataRef* metadataRef);
    void AddArrayType(const soul::ast::SourcePos& sourcePos, int32_t typeId, int64_t size, const TypeRef& elementTypeRef, MetadataRef* metadataRef);
    void AddFunctionType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs, 
        MetadataRef* metadataRef);
    void Resolve(Context* context);
    void ResolveType(TypeRef& typeRef, Context* context);
    void Add(Type* type, Context* context);
    Type* Get(int32_t id) const;
    void Map(Type* type);
    void VisitTypeDeclarations(Visitor& visitor);
    VoidType* GetVoidType() const { return const_cast<VoidType*>(&voidType); }
    BoolType* GetBoolType() const { return const_cast<BoolType*>(&boolType); }
    SByteType* GetSByteType() const { return const_cast<SByteType*>(&sbyteType); }
    ByteType* GetByteType() const { return const_cast<ByteType*>(&byteType); }
    ShortType* GetShortType() const { return const_cast<ShortType*>(&shortType); }
    UShortType* GetUShortType() const { return const_cast<UShortType*>(&ushortType); }
    IntType* GetIntType() const { return const_cast<IntType*>(&intType); }
    UIntType* GetUIntType() const { return const_cast<UIntType*>(&uintType); }
    LongType* GetLongType() const { return const_cast<LongType*>(&longType); }
    ULongType* GetULongType() const { return const_cast<ULongType*>(&ulongType); }
    FloatType* GetFloatType() const { return const_cast<FloatType*>(&floatType); }
    DoubleType* GetDoubleType() const { return const_cast<DoubleType*>(&doubleType); }
    Type* GetPointerType(Type* baseType);
    PointerType* MakePointerType(const soul::ast::SourcePos& sourcePos, int32_t baseTypeId, int8_t pointerCount, Context* context);
    StructureType* CreateStructureType();
    Type* GetStructureType(const std::vector<Type*>& memberTypes);
    Type* GetFunctionType(Type* returnType, const std::vector<Type*>& paramTypes);
    Type* GetArrayType(int64_t size, Type* elementType);
    const std::vector<Type*>& DeclaredTypes() const { return declaredTypes; }
    void Write(util::CodeFormatter& formatter);
private:
    Type* GetFundamentalType(int32_t id) const;
    Context* context;
    std::vector<std::unique_ptr<Type>> types;
    std::vector<Type*> declaredTypes;
    std::map<int32_t, Type*> typeMap;
    std::map<std::pair<int32_t, int8_t>, PointerType*> pointerTypeMap;
    std::map<std::vector<Type*>, StructureType*> structureTypeMap;
    std::map<FunctionTypeKey, FunctionType*> functionTypeMap;
    std::map<ArrayTypeKey, ArrayType*> arrayTypeMap;
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
};

} // cmajor::systemx::intermediate
