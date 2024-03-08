// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.masm.ir.type;

namespace cmajor::masm::ir {

const char* fundamentalTypeName[] =
{
    "", "void", "bool", "sbyte", "byte", "short", "ushort", "int", "uint", "long", "ulong", "float", "double"
};

std::string TypeName(int typeId)
{
    if (typeId < 0)
    {
        return fundamentalTypeName[-typeId];
    }
    else
    {
        return "$T" + std::to_string(typeId);
    }
}

Type::Type(int id_) : id(id_)
{
}

Type::~Type()
{
}

std::string Type::Name() const
{
    return TypeName(id);
}

Value* Type::DefaultValue()
{
    throw std::runtime_error("no default value for this kind of type");
}

void Type::Write(util::CodeFormatter& formatter)
{
    formatter.Write(Name());
}

void Type::WriteDeclaration(util::CodeFormatter& formatter)
{
    formatter.Write(Name());
    formatter.Write(" = type ");
}

VoidType::VoidType() : Type(voidTypeId)
{
}

BoolType::BoolType() : Type(boolTypeId), defaultValue()
{
}

SByteType::SByteType() : Type(sbyteTypeId), defaultValue()
{
}

ByteType::ByteType() : Type(byteTypeId), defaultValue()
{
}

ShortType::ShortType() : Type(shortTypeId), defaultValue()
{
}

UShortType::UShortType() : Type(ushortTypeId), defaultValue()
{
}

IntType::IntType() : Type(intTypeId), defaultValue()
{
}

UIntType::UIntType() : Type(uintTypeId), defaultValue()
{
}

LongType::LongType() : Type(longTypeId), defaultValue()
{
}

ULongType::ULongType() : Type(ulongTypeId), defaultValue()
{
}

FloatType::FloatType() : Type(floatTypeId), defaultValue()
{
}

DoubleType::DoubleType() : Type(doubleTypeId), defaultValue()
{
}

PtrType::PtrType(Type* baseType_) : Type(ptrTypeId), baseType(baseType_), defaultValue(this)
{
}

std::string PtrType::Name() const
{
    return baseType->Name() + "*";
}

StructureType::StructureType(int id_) : Type(id_)
{
}

void StructureType::SetMemberTypes(const std::vector<Type*>& memberTypes_)
{
    memberTypes = memberTypes_;
}

void StructureType::WriteDeclaration(util::CodeFormatter& formatter)
{
    Type::WriteDeclaration(formatter);
    formatter.Write("{ ");
    bool first = true;
    for (Type* memberType : memberTypes)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        memberType->Write(formatter);
    }
    formatter.Write(" }");
}

Type* StructureType::GetMemberType(uint64_t index) const
{
    Assert(index < memberTypes.size(), "invalid member type index");
    return memberTypes[index];
}

ArrayType::ArrayType(int id_, Type* elementType_, uint64_t size_) : Type(id_), elementType(elementType_), size(size_)
{
}

void ArrayType::WriteDeclaration(util::CodeFormatter& formatter)
{
    Type::WriteDeclaration(formatter);
    formatter.Write("[");
    formatter.Write(std::to_string(size));
    formatter.Write(" x ");
    elementType->Write(formatter);
    formatter.Write("]");
}

bool operator<(const ArrayTypeKey& left, const ArrayTypeKey& right)
{
    if (left.elementType < right.elementType) return true;
    if (left.elementType > right.elementType) return false;
    return left.size < right.size;
}

FunctionType::FunctionType(int id_, Type* returnType_, const std::vector<Type*>& paramTypes_) : Type(id_), returnType(returnType_), paramTypes(paramTypes_)
{
}

void FunctionType::WriteDeclaration(util::CodeFormatter& formatter)
{
    Type::WriteDeclaration(formatter);
    formatter.Write("function ");
    returnType->Write(formatter);
    formatter.Write("(");
    bool first = true;
    for (Type* paramType : paramTypes)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            formatter.Write(", ");
        }
        paramType->Write(formatter);
    }
    formatter.Write(")");
}

FunctionTypeKey::FunctionTypeKey(Type* returnType_, const std::vector<Type*>& paramTypes_) : returnType(returnType_), paramTypes(paramTypes_)
{
}

bool operator<(const FunctionTypeKey& left, const FunctionTypeKey& right)
{
    if (left.returnType < right.returnType) return true;
    if (left.returnType > right.returnType) return false;
    return left.paramTypes < right.paramTypes;
}

TypeRepository::TypeRepository()
{
}

Type* TypeRepository::GetPtrType(Type* baseType)
{
    auto it = ptrTypeMap.find(baseType);
    if (it != ptrTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        PtrType* ptrType = new PtrType(baseType);
        ptrTypeMap[baseType] = ptrType;
        ptrTypes.push_back(std::unique_ptr<PtrType>(ptrType));
        return ptrType;
    }
}

Type* TypeRepository::GetStructureType(const std::vector<Type*>& memberTypes)
{
    auto it = structureTypeMap.find(memberTypes);
    if (it != structureTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        StructureType* structureType = new StructureType(types.size());
        structureType->SetMemberTypes(memberTypes);
        structureTypeMap[memberTypes] = structureType;
        types.push_back(std::unique_ptr<Type>(structureType));
        return structureType;
    }
}

Type* TypeRepository::CreateStructureType()
{
    StructureType* structureType = new StructureType(types.size());
    types.push_back(std::unique_ptr<Type>(structureType));
    return structureType;
}

Type* TypeRepository::GetArrayType(Type* elementType, uint64_t size)
{
    ArrayTypeKey key(elementType, size);
    auto it = arrayTypeMap.find(key);
    if (it != arrayTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        ArrayType* arrayType = new ArrayType(types.size(), elementType, size);
        arrayTypeMap[key] = arrayType;
        types.push_back(std::unique_ptr<Type>(arrayType));
        return arrayType;
    }
}

Type* TypeRepository::GetFunctionType(Type* returnType, const std::vector<Type*>& paramTypes)
{
    FunctionTypeKey key(returnType, paramTypes);
    auto it = functionTypeMap.find(key);
    if (it != functionTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        FunctionType* functionType = new FunctionType(types.size(), returnType, paramTypes);
        functionTypeMap[key] = functionType;
        types.push_back(std::unique_ptr<Type>(functionType));
        return functionType;
    }
}

void TypeRepository::Write(util::CodeFormatter& formatter)
{
    if (types.empty()) return;
    formatter.WriteLine("types");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& t : types)
    {
        t->WriteDeclaration(formatter);
        formatter.WriteLine();
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

} // namespace cmajor::masm::ir
