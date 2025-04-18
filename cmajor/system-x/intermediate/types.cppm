// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.intermediate.types;

import cmajor.systemx.assembler;
import soul.ast.source.pos;
import std.core;

export namespace cmajor::systemx::intermediate {

export namespace types {}

class Context;
class Types;
class Visitor;
class ArrayType;
class StructureType;

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
const int32_t pointerTypeId = int32_t(1) << 31;

constexpr bool IsFundamentalTypeId(int32_t typeId) { return typeId >= 0 && typeId < userTypeId; }
constexpr bool IsPointerTypeId(int32_t typeId) { return (typeId & pointerTypeId) != 0; }

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
    return typeId & (~(pointerTypeId | (int32_t(0x7F) << (32 - 8))) | 0x00FFFFFF);
}

constexpr int8_t GetPointerCount(int32_t typeId)
{
    return (typeId & (int32_t(0x7F) << (32 - 8))) >> (32 - 8);
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
    Type* AddPointer(Context* context) const;
    Type* RemovePointer(const soul::ast::SourcePos& sourcePos, Context* context) const;
    virtual std::string Name() const = 0;
    bool IsStructureType() const { return kind == TypeKind::structureType; }
    StructureType* GetStructurePointeeType(const soul::ast::SourcePos& sourcePos, Context* context) const;
    ArrayType* GetArrayPointeeType(const soul::ast::SourcePos& sourcePos, Context* context) const;
    bool IsArrayType() const { return kind == TypeKind::arrayType; }
    bool IsFunctionType() const { return kind == TypeKind::functionType; }
    virtual bool IsWeakType() const { return true; }
    virtual void Add(Types* types, Context* context);
    virtual void Resolve(Types* types, Context* context);
    virtual cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const;
    const soul::ast::SourcePos& GetSourcePos() const { return sourcePos; }
    int32_t Id() const { return id; }
private:
    soul::ast::SourcePos sourcePos;
    TypeKind kind;
    int32_t id;
};

class VoidType : public Type
{
public:
    VoidType();
    std::string Name() const override { return "void"; }
    int64_t Size() const override { return -1; }
    int64_t Alignment() const override { return -1; }
};

class BoolType : public Type
{
public:
    BoolType();
    std::string Name() const override { return "bool"; }
    int64_t Size() const override { return 1; }
    int64_t Alignment() const override { return 1; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
};

class SByteType : public Type
{
public:
    SByteType();
    std::string Name() const override { return "sbyte"; }
    int64_t Size() const override { return 1; }
    int64_t Alignment() const override { return 1; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
};

class ByteType : public Type
{
public:
    ByteType();
    std::string Name() const override { return "byte"; }
    int64_t Size() const override { return 1; }
    int64_t Alignment() const override { return 1; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
};

class ShortType : public Type
{
public:
    ShortType();
    std::string Name() const override { return "short"; }
    int64_t Size() const override { return 2; }
    int64_t Alignment() const override { return 2; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
};

class UShortType : public Type
{
public:
    UShortType();
    std::string Name() const override { return "ushort"; }
    int64_t Size() const override { return 2; }
    int64_t Alignment() const override { return 2; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
};

class IntType : public Type
{
public:
    IntType();
    std::string Name() const override { return "int"; }
    int64_t Size() const override { return 4; }
    int64_t Alignment() const override { return 4; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
};

class UIntType : public Type
{
public:
    UIntType();
    std::string Name() const override { return "uint"; }
    int64_t Size() const override { return 4; }
    int64_t Alignment() const override { return 4; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
};

class LongType : public Type
{
public:
    LongType();
    std::string Name() const override { return "long"; }
    int64_t Size() const override { return 8; }
    int64_t Alignment() const override { return 8; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
};

class ULongType : public Type
{
public:
    ULongType();
    std::string Name() const override { return "ulong"; }
    int64_t Size() const override { return 8; }
    int64_t Alignment() const override { return 8; }
    cmajor::systemx::assembler::Instruction* MakeAssemblyInst(Context* context) const override;
};

class FloatType : public Type
{
public:
    FloatType();
    std::string Name() const override { return "float"; }
    int64_t Size() const override { return 4; }
    int64_t Alignment() const override { return 4; }
};

class DoubleType : public Type
{
public:
    DoubleType();
    std::string Name() const override { return "double"; }
    int64_t Size() const override { return 8; }
    int64_t Alignment() const override { return 8; }
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
    void Accept(Visitor& visitor) override;
    void Add(Types* types, Context* context) override;
    void Resolve(Types* types, Context* context) override;
    int64_t Size() const override;
    int64_t Alignment() const override { return 8; }
    std::string Name() const override { return "struct " + std::to_string(Id()); }
    bool IsWeakType() const override;
    int FieldCount() const { return fieldTypeRefs.size(); }
    const std::vector<TypeRef>& FieldTypeRefs() const { return fieldTypeRefs; }
    Type* FieldType(int i) const { return fieldTypeRefs[i].GetType(); }
    int64_t GetFieldOffset(int64_t index) const;
private:
    void ComputeSizeAndOffsets() const;
    std::vector<TypeRef> fieldTypeRefs;
    mutable bool sizeAndOffsetsComputed;
    mutable int64_t size;
    mutable std::vector<int64_t> fieldOffsets;
};

class ArrayType : public Type
{
public:
    ArrayType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, int64_t elementCount_, const TypeRef& elementTypeRef_);
    void Accept(Visitor& visitor) override;
    void Add(Types* types, Context* context) override;
    void Resolve(Types* types, Context* context) override;
    int64_t Size() const override;
    int64_t Alignment() const override { return 8; }
    std::string Name() const override { return "array " + std::to_string(Id()); }
    bool IsWeakType() const override;
    int64_t ElementCount() const { return elementCount; }
    const TypeRef& ElementTypeRef() const { return elementTypeRef; }
    Type* ElementType() const { return elementTypeRef.GetType(); }
private:
    int64_t elementCount;
    TypeRef elementTypeRef;
};

class FunctionType : public Type
{
public:
    FunctionType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, const TypeRef& returnTypeRef_, const std::vector<TypeRef>& paramTypeRefs_);
    void Accept(Visitor& visitor) override;
    void Add(Types* types, Context* context) override;
    void Resolve(Types* types, Context* context) override;
    bool IsWeakType() const override;
    int Arity() const { return paramTypeRefs.size(); }
    int64_t Size() const override { return 8; }
    int64_t Alignment() const override { return 8; }
    std::string Name() const override { return "function " + std::to_string(Id()); }
    const TypeRef& ReturnTypeRef() const { return returnTypeRef; }
    Type* ReturnType() const { return returnTypeRef.GetType(); }
    const std::vector<TypeRef>& ParamTypeRefs() const { return paramTypeRefs; }
    Type* ParamType(int index) const { return paramTypeRefs[index].GetType(); }
private:
    TypeRef returnTypeRef;
    std::vector<TypeRef> paramTypeRefs;
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
private:
    int8_t pointerCount;
    TypeRef baseTypeRef;
};

class Types
{
public:
    Types();
    Types(const Types&) = delete;
    Types& operator=(const Types&) = delete;
    Context* GetContext() const { return context; }
    void SetContext(Context* context_) { context = context_; }
    void AddStructureType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs);
    void AddArrayType(const soul::ast::SourcePos& sourcePos, int32_t typeId, int64_t size, const TypeRef& elementTypeRef);
    void AddFunctionType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs);
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
    PointerType* MakePointerType(const soul::ast::SourcePos& sourcePos, int32_t baseTypeId, int8_t pointerCount, Context* context);
private:
    Type* GetFundamentalType(int32_t id) const;
    Context* context;
    std::vector<std::unique_ptr<Type>> types;
    std::vector<Type*> declaratedTypes;
    std::map<int32_t, Type*> typeMap;
    std::map<std::pair<int32_t, int8_t>, PointerType*> pointerTypeMap;
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
