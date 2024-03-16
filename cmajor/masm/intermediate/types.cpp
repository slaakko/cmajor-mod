// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.masm.intermediate.types;

import cmajor.masm.intermediate.context;
import cmajor.masm.intermediate.error;
import cmajor.masm.intermediate.visitor;
import util;

namespace cmajor::masm::intermediate {

Type::Type(const soul::ast::Span& span_, TypeKind kind_, int32_t id_) : span(span_), kind(kind_), id(id_)
{
}

Type::~Type()
{
}

bool Type::IsIntegerType() const
{
    switch (id)
    {
        case sbyteTypeId:
        case byteTypeId:
        case shortTypeId:
        case ushortTypeId:
        case intTypeId:
        case uintTypeId:
        case longTypeId:
        case ulongTypeId:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Type::IsUnsignedType() const
{
    switch (id)
    {
        case byteTypeId:
        case ushortTypeId:
        case uintTypeId:
        case ulongTypeId:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Type::IsSignedType() const
{
    switch (id)
    {
        case sbyteTypeId:
        case shortTypeId:
        case intTypeId:
        case longTypeId:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

bool Type::IsFloatingPointType() const
{
    switch (id)
    {
        case floatTypeId:
        case doubleTypeId:
        {
            return true;
        }
        default:
        {
            return false;
        }
    }
}

Type* Type::AddPointer(Context* context) const
{
    if (IsPointerType())
    {
        const PointerType* pointerType = static_cast<const PointerType*>(this);
        return context->GetTypes().MakePointerType(Span(), GetBaseTypeId(pointerType->Id()), GetPointerCount(pointerType->Id()) + 1, context);
    }
    else
    {
        return context->GetTypes().MakePointerType(Span(), Id(), 1, context);
    }
}

Type* Type::RemovePointer(const soul::ast::Span& span, Context* context) const
{
    if (IsPointerType())
    {
        const PointerType* pointerType = static_cast<const PointerType*>(this);
        return pointerType->BaseType();
    }
    else
    {
        Error("pointer type expected", span, context);
    }
    return nullptr;
}

StructureType* Type::GetStructurePointeeType(const soul::ast::Span& span, Context* context) const
{
    Type* pointeeType = RemovePointer(span, context);
    if (pointeeType)
    {
        if (pointeeType->IsStructureType())
        {
            return static_cast<StructureType*>(pointeeType);
        }
        else
        {
            Error("structure type expected", span, context);
        }
    }
    else
    {
        Error("pointer type expected", span, context);
    }
    return nullptr;
}

ArrayType* Type::GetArrayPointeeType(const soul::ast::Span& span, Context* context) const
{
    Type* pointeeType = RemovePointer(span, context);
    if (pointeeType)
    {
        if (pointeeType->IsArrayType())
        {
            return static_cast<ArrayType*>(pointeeType);
        }
        else
        {
            Error("array type expected", span, context);
        }
    }
    else
    {
        Error("pointer type expected", span, context);
    }
    return nullptr;
}

void Type::Add(Types* types, Context* context)
{
}

void Type::Resolve(Types* types, Context* context)
{
}

VoidType::VoidType() : Type(soul::ast::Span(), TypeKind::fundamentalType, voidTypeId)
{
}

BoolType::BoolType() : Type(soul::ast::Span(), TypeKind::fundamentalType, boolTypeId)
{
}

SByteType::SByteType() : Type(soul::ast::Span(), TypeKind::fundamentalType, sbyteTypeId)
{
}

ByteType::ByteType() : Type(soul::ast::Span(), TypeKind::fundamentalType, byteTypeId)
{
}

ShortType::ShortType() : Type(soul::ast::Span(), TypeKind::fundamentalType, shortTypeId)
{
}

UShortType::UShortType() : Type(soul::ast::Span(), TypeKind::fundamentalType, ushortTypeId)
{
}

IntType::IntType() : Type(soul::ast::Span(), TypeKind::fundamentalType, intTypeId)
{
}

UIntType::UIntType() : Type(soul::ast::Span(), TypeKind::fundamentalType, uintTypeId)
{
}

LongType::LongType() : Type(soul::ast::Span(), TypeKind::fundamentalType, longTypeId)
{
}

ULongType::ULongType() : Type(soul::ast::Span(), TypeKind::fundamentalType, ulongTypeId)
{
}

FloatType::FloatType() : Type(soul::ast::Span(), TypeKind::fundamentalType, floatTypeId)
{
}

DoubleType::DoubleType() : Type(soul::ast::Span(), TypeKind::fundamentalType, doubleTypeId)
{
}

TypeRef::TypeRef() : span(), id(-1), type(nullptr)
{
}

TypeRef::TypeRef(const soul::ast::Span& span_, int32_t id_) : span(span_), id(id_), type(nullptr)
{
}

StructureType::StructureType(const soul::ast::Span& span_, int32_t typeId_, const std::vector<TypeRef>& fieldTypeRefs_) :
    Type(span_, TypeKind::structureType, typeId_), fieldTypeRefs(fieldTypeRefs_), sizeAndOffsetsComputed(false)
{
}

void StructureType::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void StructureType::Add(Types* types, Context* context)
{
    types->Add(this, context);
}

void StructureType::Resolve(Types* types, Context* context)
{
    for (auto& typeRef : fieldTypeRefs)
    {
        types->ResolveType(typeRef, context);
    }
}

bool StructureType::IsWeakType() const
{
    int n = FieldCount();
    for (int i = 0; i < n; ++i)
    {
        Type* type = FieldType(i);
        if (type->IsStructureType() || type->IsArrayType() || type->IsFunctionType())
        {
            return false;
        }
    }
    return true;
}

int64_t StructureType::Size() const
{
    if (!sizeAndOffsetsComputed)
    {
        ComputeSizeAndOffsets();
    }
    return size;
}

void StructureType::ComputeSizeAndOffsets() const
{
    int64_t offset = 0;
    int n = FieldCount();
    for (int i = 0; i < n; ++i)
    {
        Type* fieldType = FieldType(i);
        int64_t memberOffset = offset;
        if (i > 0)
        {
            int64_t alignment = fieldType->Alignment();
            memberOffset = alignment * ((offset - 1) / alignment + 1);
        }
        fieldOffsets.push_back(memberOffset);
        offset = memberOffset + fieldType->Size();
    }
    if (n == 0)
    {
        size = 8;
    }
    else
    {
        size = 8 * ((offset - 1) / 8 + 1);
    }
    sizeAndOffsetsComputed = true;
}

int64_t StructureType::GetFieldOffset(int64_t index) const
{
    if (!sizeAndOffsetsComputed)
    {
        ComputeSizeAndOffsets();
    }
    return fieldOffsets[index];
}

ArrayType::ArrayType(const soul::ast::Span& span_, int32_t typeId_, int64_t elementCount_, const TypeRef& elementTypeRef_) :
    Type(span_, TypeKind::arrayType, typeId_), elementCount(elementCount_), elementTypeRef(elementTypeRef_)
{
}

void ArrayType::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void ArrayType::Add(Types* types, Context* context)
{
    types->Add(this, context);
}

void ArrayType::Resolve(Types* types, Context* context)
{
    types->ResolveType(elementTypeRef, context);
}

int64_t ArrayType::Size() const
{
    return util::Align(elementCount * ElementType()->Size(), 8);
}

bool ArrayType::IsWeakType() const
{
    Type* elementType = ElementType();
    if (elementType->IsStructureType() || elementType->IsArrayType() || elementType->IsFunctionType())
    {
        return false;
    }
    return true;
}

FunctionType::FunctionType(const soul::ast::Span& span_, int32_t typeId_, const TypeRef& returnTypeRef_, const std::vector<TypeRef>& paramTypeRefs_) :
    Type(span_, TypeKind::functionType, typeId_), returnTypeRef(returnTypeRef_), paramTypeRefs(paramTypeRefs_)
{
}

void FunctionType::Accept(Visitor& visitor)
{
    visitor.Visit(*this);
}

void FunctionType::Add(Types* types, Context* context)
{
    types->Add(this, context);
}

void FunctionType::Resolve(Types* types, Context* context)
{
    types->ResolveType(returnTypeRef, context);
    for (auto& paramTypeRef : paramTypeRefs)
    {
        types->ResolveType(paramTypeRef, context);
    }
}

bool FunctionType::IsWeakType() const
{
    Type* returnType = ReturnType();
    if (returnType->IsStructureType() || returnType->IsArrayType() || returnType->IsFunctionType())
    {
        return false;
    }
    int n = Arity();
    for (int i = 0; i < n; ++i)
    {
        Type* paramType = ParamType(i);
        if (paramType->IsStructureType() || paramType->IsArrayType() || paramType->IsFunctionType())
        {
            return false;
        }
    }
    return true;
}

PointerType::PointerType(const soul::ast::Span& span_, int32_t typeId_, int8_t pointerCount_, int32_t baseTypeId_) :
    Type(span_, TypeKind::pointerType, typeId_), pointerCount(pointerCount_), baseTypeRef(span_, baseTypeId_)
{
}

std::string PointerType::Name() const
{
    return baseTypeRef.GetType()->Name() + "*";
}

Types::Types() : context(nullptr)
{
}

void Types::AddStructureType(const soul::ast::Span& span, int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs)
{
    types.push_back(std::unique_ptr<Type>(new StructureType(span, typeId, fieldTypeRefs)));
}

void Types::AddArrayType(const soul::ast::Span& span, int32_t typeId, int64_t size, const TypeRef& elementTypeRef)
{
    types.push_back(std::unique_ptr<Type>(new ArrayType(span, typeId, size, elementTypeRef)));
}

void Types::AddFunctionType(const soul::ast::Span& span, int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs)
{
    types.push_back(std::unique_ptr<Type>(new FunctionType(span, typeId, returnTypeRef, paramTypeRefs)));
}

void Types::Add(Type* type, Context* context)
{
    Type* prev = Get(type->Id());
    if (prev)
    {
        Error("error adding type id " + std::to_string(type->Id()) + ": type id not unique", type->Span(), context, prev->Span());
    }
    Map(type);
    declaratedTypes.push_back(type);
}

Type* Types::Get(int32_t id) const
{
    if (IsFundamentalTypeId(id))
    {
        return GetFundamentalType(id);
    }
    auto it = typeMap.find(id);
    if (it != typeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Type* Types::GetFundamentalType(int32_t id) const
{
    switch (id)
    {
    case voidTypeId: return const_cast<VoidType*>(&voidType);
    case boolTypeId: return const_cast<BoolType*>(&boolType);
    case sbyteTypeId: return const_cast<SByteType*>(&sbyteType);
    case byteTypeId: return const_cast<ByteType*>(&byteType);
    case shortTypeId: return const_cast<ShortType*>(&shortType);
    case ushortTypeId: return const_cast<UShortType*>(&ushortType);
    case intTypeId: return const_cast<IntType*>(&intType);
    case uintTypeId: return const_cast<UIntType*>(&uintType);
    case longTypeId: return const_cast<LongType*>(&longType);
    case ulongTypeId: return const_cast<ULongType*>(&ulongType);
    case floatTypeId: return const_cast<FloatType*>(&floatType);
    case doubleTypeId: return const_cast<DoubleType*>(&doubleType);
    }
    return nullptr;
}

void Types::Map(Type* type)
{
    typeMap[type->Id()] = type;
}

void Types::VisitTypeDeclarations(Visitor& visitor)
{
    for (Type* declaredType : declaratedTypes)
    {
        declaredType->Accept(visitor);
    }
}

PointerType* Types::MakePointerType(const soul::ast::Span& span, int32_t baseTypeId, int8_t pointerCount, Context* context)
{
    auto it = pointerTypeMap.find(std::make_pair(baseTypeId, pointerCount));
    if (it != pointerTypeMap.cend())
    {
        return it->second;
    }
    PointerType* type = nullptr;
    if (pointerCount > 1)
    {
        type = new PointerType(span, MakePointerTypeId(baseTypeId, pointerCount), pointerCount, MakePointerTypeId(baseTypeId, pointerCount - 1));
    }
    else if (pointerCount == 1)
    {
        type = new PointerType(span, MakePointerTypeId(baseTypeId, pointerCount), pointerCount, baseTypeId);
    }
    else
    {
        Error("pointer count > 0 expected", span, context);
    }
    ResolveType(type->BaseTypeRef(), context);
    types.push_back(std::unique_ptr<Type>(type));
    Map(type);
    pointerTypeMap[std::make_pair(baseTypeId, pointerCount)] = type;
    return type;
}

void Types::Resolve(Context* context)
{
    for (const auto& type : types)
    {
        type->Add(this, context);
    }
    std::vector<Type*> toResolve;
    for (const auto& type : types)
    {
        toResolve.push_back(type.get());
    }
    for (Type* type : toResolve)
    {
        type->Resolve(this, context);
    }
}

void Types::ResolveType(TypeRef& typeRef, Context* context)
{
    Type* type = Get(typeRef.Id());
    if (!type)
    {
        if (IsPointerTypeId(typeRef.Id()))
        {
            type = MakePointerType(typeRef.Span(), GetBaseTypeId(typeRef.Id()), GetPointerCount(typeRef.Id()), context);
        }
    }
    if (!type)
    {
        Error("error resolving type: type id " + std::to_string(typeRef.Id()) + " not found", typeRef.Span(), context);
    }
    typeRef.SetType(type);
}

} // cmajor::masm::intermediate