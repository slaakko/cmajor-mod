// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.systemx.object.type;

import cmajor.systemx.object.value;
import cmajor.systemx.machine;
import std.core;

export namespace cmajor::systemx::object {

class Section;
class StringTable;
class SymbolTable;

enum class TypeKind : uint8_t
{
    fundamentalType, structureType, arrayType, functionType, pointerType, stringType, listType, setType, mapType, linkedListType, hashSetType, hashMapType, forwardListType
};

const int64_t maxCharPtrStrLen = 64;

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
const int32_t charTypeId = 12;
const int32_t wcharTypeId = 13;
const int32_t ucharTypeId = 14;
const int32_t userTypeId = 15;
const int32_t pointerTypeId = int32_t(1) << 30;

constexpr bool IsFundamentalTypeId(int32_t typeId) { return typeId >= 0 && typeId < userTypeId; }
constexpr bool IsPointerTypeId(int32_t typeId) { return (typeId & pointerTypeId) != 0; }

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

class Type
{
public:
    Type(TypeKind kind_);
    Type(TypeKind kind_, int32_t id_, const std::string& name_);
    virtual ~Type();
    TypeKind Kind() const { return kind; }
    bool IsPointerType() const { return kind == TypeKind::pointerType; }
    bool IsByteType() const { return id == byteTypeId; }
    bool IsCharType() const { return id == charTypeId; }
    bool IsUShortType() const { return id == ushortTypeId; }
    bool IsWCharType() const { return id == wcharTypeId; }
    bool IsUIntType() const { return id == uintTypeId; }
    bool IsUCharType() const { return id == ucharTypeId; }
    int32_t Id() const { return id; }
    const std::string& Name() const { return name; }
    virtual int64_t Size() const = 0;
    virtual std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context);
    virtual void Write(Section* section);
    virtual int64_t Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable, StringTable& stringTable);
    virtual void AddStrings(StringTable* stringTable);
private:
    TypeKind kind;
    int32_t id;
    std::string name;
    int32_t nameId;
};

class VoidType : public Type
{
public:
    VoidType();
    int64_t Size() const override { return -1; }
};

class BoolType : public Type
{
public:
    BoolType();
    int64_t Size() const override { return 1; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class SByteType : public Type
{
public:
    SByteType();
    int64_t Size() const override { return 1; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class ByteType : public Type
{
public:
    ByteType();
    int64_t Size() const override { return 1; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class ShortType : public Type
{
public:
    ShortType();
    int64_t Size() const override { return 2; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class UShortType : public Type
{
public:
    UShortType();
    int64_t Size() const override { return 2; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class IntType : public Type
{
public:
    IntType();
    int64_t Size() const override { return 4; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class UIntType : public Type
{
public:
    UIntType();
    int64_t Size() const override { return 4; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class LongType : public Type
{
public:
    LongType();
    int64_t Size() const override { return 8; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class ULongType : public Type
{
public:
    ULongType();
    int64_t Size() const override { return 8; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class FloatType : public Type
{
public:
    FloatType();
    int64_t Size() const override { return 4; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class DoubleType : public Type
{
public:
    DoubleType();
    int64_t Size() const override { return 8; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class CharType : public Type
{
public:
    CharType();
    int64_t Size() const override { return 1; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class WCharType : public Type
{
public:
    WCharType();
    int64_t Size() const override { return 2; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class UCharType : public Type
{
public:
    UCharType();
    int64_t Size() const override { return 4; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class Field
{
public:
    Field();
    Field(const std::string& name_, int32_t typeId_, int32_t offset_);
    const std::string& Name() const { return name; }
    int32_t TypeId() const { return typeId; }
    int32_t Offset() const { return offset; }
    void AddStrings(StringTable* stringTable);
    void Write(Section* section);
    int64_t Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable, StringTable& stringTable);
private:
    std::string name;
    int32_t nameId;
    int32_t typeId;
    int32_t offset;
};

class StructureType : public Type
{
public:
    StructureType();
    StructureType(int32_t id_, const std::string& name_, int64_t size_);
    int64_t Size() const override { return size; }
    void AddField(Field&& field);
    const std::vector<Field>& Fields() const { return fields; }
    void AddStrings(StringTable* stringTable) override;
    void Write(Section* section) override;
    int64_t Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable, StringTable& stringTable) override;
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
private:
    int64_t size;
    std::vector<Field> fields;
};

class ArrayType : public Type
{
public:
    ArrayType();
    ArrayType(int32_t id_, const std::string& name_, int32_t elementTypeId_, int64_t size_);
    int32_t ElementTypeId() const { return elementTypeId; }
    int64_t Size() const override { return size; }
    void Write(Section* section) override;
    int64_t Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable, StringTable& stringTable) override;
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
private:
    int32_t elementTypeId;
    int64_t size;
};

class FunctionType : public Type
{
public:
    FunctionType();
    FunctionType(int32_t id_, const std::string& name_);
    int64_t Size() const override { return -1; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class PointerType : public Type
{
public:
    PointerType(int32_t id_, const std::string& name_, int8_t pointerCount_, Type* baseType_);
    int8_t PointerCount() const { return pointerCount; }
    Type* BaseType() const { return baseType; }
    int64_t Size() const override { return 8; }
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
private:
    int8_t pointerCount;
    Type* baseType;
};

class SystemType : public Type
{
public:
    SystemType(TypeKind kind_, int32_t id_, const std::string& name_, Type* baseType_);
    int64_t Size() const override { return baseType->Size(); }
    Type* BaseType() const { return baseType; }
private:
    Type* baseType;
};

class StringType : public SystemType
{
public:
    StringType(int32_t id_, Type* baseType_);
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class ListType : public SystemType
{
public:
    ListType(int32_t id_, Type* baseType_);
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
};

class TreeType : public SystemType
{
public:
    TreeType(TypeKind kind_, int32_t id_, const std::string& name_, Type* baseType_);
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
private:
    PointerValue* Min(PointerValue* ptr, EvaluationContext& context);
    PointerValue* Next(PointerValue* ptr, Type* headerPtrType, EvaluationContext& context);
};

class SetType : public TreeType
{
public:
    SetType(int32_t id_, Type* baseType_);
};

class MapType : public TreeType
{
public:
    MapType(int32_t id_, Type* baseType_);
};

class LinkedListType : public SystemType
{
public:
    LinkedListType(int32_t id_, Type* baseType_);
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
private:
    PointerValue* Next(PointerValue* ptr, EvaluationContext& context);
};

class HashTableType : public SystemType
{
public:
    HashTableType(TypeKind kind_, int32_t id_, const std::string& name_, Type* baseType_);
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
private:
    PointerValue* Next(PointerValue* bucket, PointerValue* nullValue, int64_t& bucketIndex, int64_t bucketCount, Type* bucketsListType, EvaluationContext& context);
};

class HashSetType : public HashTableType
{
public:
    HashSetType(int32_t id_, Type* baseType_);
};

class HashMapType : public HashTableType
{
public:
    HashMapType(int32_t id_, Type* baseType_);
};

class ForwardListType : public SystemType
{
public:
    ForwardListType(int32_t id_, Type* baseType_);
    std::unique_ptr<TypedValue> Evaluate(EvaluationContext& context) override;
private:
    PointerValue* Next(PointerValue* ptr, EvaluationContext& context);
    int64_t Count(PointerValue* head, EvaluationContext& context);
};

Type* ReadType(int64_t address, SymbolTable& symbolTable, StringTable& stringTable, uint64_t rv, cmajor::systemx::machine::Memory& memory);

Type* MakeSystemType(int8_t systemTypeId, int32_t typeId, Type* baseType);

} // namespace cmajor::systemx::object
