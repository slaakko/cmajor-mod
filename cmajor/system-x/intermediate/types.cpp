// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.intermediate.types;

import cmajor.systemx.intermediate.context;
import cmajor.systemx.intermediate.error;
import cmajor.systemx.intermediate.visitor;
import util;

namespace cmajor::systemx::intermediate {

const char* fundamentalTypeName[] =
{
    "void", "bool", "sbyte", "byte", "short", "ushort", "int", "uint", "long", "ulong", "float", "double"
};

std::string TypeName(int typeId)
{
    if (IsFundamentalTypeId(typeId))
    {
        return fundamentalTypeName[typeId];
    }
    else
    {
        return "$T" + std::to_string(MakeUserTypeId(typeId));
    }
}

Type::Type(const soul::ast::SourcePos& sourcePos_, TypeKind kind_, int32_t id_) : sourcePos(sourcePos_), kind(kind_), id(id_)
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

void Type::SetSystemType(int8_t systemType)
{
    cmajor::systemx::intermediate::SetSystemType(id, systemType);
}

Type* Type::AddPointer(Context* context) 
{
    if (IsPointerType())
    {
        PointerType* pointerType = static_cast<PointerType*>(this);
        return context->GetTypes().MakePointerType(soul::ast::SourcePos(), GetBaseTypeId(pointerType->Id()), GetPointerCount(pointerType->Id()) + 1, context);
    }
    else
    {
        return context->GetTypes().MakePointerType(soul::ast::SourcePos(), Id(), 1, context);
    }
}

Type* Type::RemovePointer(const soul::ast::SourcePos& sourcePos, Context* context) const
{
    if (IsPointerType())
    {
        const PointerType* pointerType = static_cast<const PointerType*>(this);
        return pointerType->BaseType();
    }
    else
    {
        Error("pointer type expected", sourcePos, context);
    }
    return nullptr;
}

StructureType* Type::GetStructurePointeeType(const soul::ast::SourcePos& sourcePos, Context* context) const
{
    Type* pointeeType = RemovePointer(sourcePos, context);
    if (pointeeType)
    {
        if (pointeeType->IsStructureType())
        {
            return static_cast<StructureType*>(pointeeType);
        }
        else
        {
            Error("structure type expected", sourcePos, context);
        }
    }
    else
    {
        Error("pointer type expected", sourcePos, context);
    }
    return nullptr;
}

ArrayType* Type::GetArrayPointeeType(const soul::ast::SourcePos& sourcePos, Context* context) const
{
    Type* pointeeType = RemovePointer(sourcePos, context);
    if (pointeeType)
    {
        if (pointeeType->IsArrayType())
        {
            return static_cast<ArrayType*>(pointeeType);
        }
        else
        {
            Error("array type expected", sourcePos, context);
        }
    }
    else
    {
        Error("pointer type expected", sourcePos, context);
    }
    return nullptr;
}

void Type::Add(Types* types, Context* context)
{
}

void Type::Resolve(Types* types, Context* context)
{
}

cmajor::systemx::assembler::Instruction* Type::MakeAssemblyInst(Context* context) const
{
    Error("no assembly instruction for this type", sourcePos, context);
    return nullptr;
}

ConstantValue* Type::DefaultValue()
{
    return nullptr;
}

void Type::Write(util::CodeFormatter& formatter)
{
    formatter.Write(Name());
}

std::string Type::Name() const
{
    return TypeName(id);
}

void Type::WriteDeclaration(util::CodeFormatter& formatter)
{
    formatter.Write(Name());
    formatter.Write(" = type ");
}

VoidType::VoidType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, voidTypeId)
{
}

BoolType::BoolType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, boolTypeId), defaultValue(false, this)
{
}

cmajor::systemx::assembler::Instruction* BoolType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BYTE);
}

SByteType::SByteType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, sbyteTypeId), defaultValue(0, this)
{
}

cmajor::systemx::assembler::Instruction* SByteType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BYTE);
}

ByteType::ByteType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, byteTypeId), defaultValue(0, this)
{
}

cmajor::systemx::assembler::Instruction* ByteType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::BYTE);
}

ShortType::ShortType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, shortTypeId), defaultValue(0, this)
{
}

cmajor::systemx::assembler::Instruction* ShortType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::WYDE);
}

UShortType::UShortType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, ushortTypeId), defaultValue(0, this)
{
}

cmajor::systemx::assembler::Instruction* UShortType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::WYDE);
}

IntType::IntType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, intTypeId), defaultValue(0, this)
{
}

cmajor::systemx::assembler::Instruction* IntType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::TETRA);
}

UIntType::UIntType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, uintTypeId), defaultValue(0, this)
{
}

cmajor::systemx::assembler::Instruction* UIntType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::TETRA);
}

LongType::LongType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, longTypeId), defaultValue(0, this)
{
}

cmajor::systemx::assembler::Instruction* LongType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
}

ULongType::ULongType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, ulongTypeId), defaultValue(0, this)
{
}

cmajor::systemx::assembler::Instruction* ULongType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
}

FloatType::FloatType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, floatTypeId), defaultValue(0.0f, this)
{
}

DoubleType::DoubleType() : Type(soul::ast::SourcePos(), TypeKind::fundamentalType, doubleTypeId), defaultValue(0.0, this)
{
}

TypeRef::TypeRef() : sourcePos(), id(-1), type(nullptr)
{
}

TypeRef::TypeRef(const soul::ast::SourcePos& sourcePos_, int32_t id_) : sourcePos(sourcePos_), id(id_), type(nullptr)
{
}

StructureType::StructureType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, const std::vector<TypeRef>& fieldTypeRefs_) :
    Type(sourcePos_, TypeKind::structureType, typeId_), fieldTypeRefs(fieldTypeRefs_), sizeAndOffsetsComputed(false), metadataRef(nullptr)
{
}

StructureType::StructureType(int32_t typeId_, const std::vector<Type*>& memberTypes) : 
    Type(soul::ast::SourcePos(), TypeKind::structureType, typeId_), sizeAndOffsetsComputed(false), metadataRef(nullptr)
{
    for (Type* memberType : memberTypes)
    {
        TypeRef fieldTypeRef;
        fieldTypeRef.SetType(memberType);
        fieldTypeRefs.push_back(fieldTypeRef);
    }
}

StructureType::StructureType(int32_t typeId_) : 
    Type(soul::ast::SourcePos(), TypeKind::structureType, typeId_), sizeAndOffsetsComputed(false), metadataRef(nullptr)
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

void StructureType::SetMemberTypes(const std::vector<Type*>& memberTypes)
{
    fieldTypeRefs.clear();
    for (Type* memberType : memberTypes)
    {
        TypeRef fieldTypeRef;
        fieldTypeRef.SetType(memberType);
        fieldTypeRefs.push_back(fieldTypeRef);
    }
}

void StructureType::WriteDeclaration(util::CodeFormatter& formatter)
{
    Type::WriteDeclaration(formatter);
    formatter.Write("{ ");
    int n = FieldCount();
    for (int i = 0; i < n; ++i)
    {
        Type* fieldType = FieldType(i);
        if (i > 0)
        {
            formatter.Write(", ");
        }
        fieldType->Write(formatter);
    }
    formatter.Write(" }");
    if (metadataRef)
    {
        formatter.Write(" ");
        metadataRef->Write(formatter);
    }
}

bool operator<(const ArrayTypeKey& left, const ArrayTypeKey& right)
{
    if (left.elementType < right.elementType) return true;
    if (left.elementType > right.elementType) return false;
    return left.size < right.size;
}

ArrayType::ArrayType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, int64_t elementCount_, const TypeRef& elementTypeRef_) :
    Type(sourcePos_, TypeKind::arrayType, typeId_), elementCount(elementCount_), elementTypeRef(elementTypeRef_), metadataRef(nullptr)
{
}

ArrayType::ArrayType(int32_t typeId_, int64_t elementCount_, Type* elementType_) : 
    Type(soul::ast::SourcePos(), TypeKind::arrayType, typeId_), elementCount(elementCount_), metadataRef(nullptr)
{
    elementTypeRef.SetType(elementType_);
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

void ArrayType::WriteDeclaration(util::CodeFormatter& formatter)
{
    Type::WriteDeclaration(formatter);
    formatter.Write("[");
    formatter.Write(std::to_string(elementCount));
    formatter.Write(" x ");
    ElementType()->Write(formatter);
    formatter.Write("]");
    if (metadataRef)
    {
        formatter.Write(" ");
        metadataRef->Write(formatter);
    }
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

FunctionType::FunctionType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, const TypeRef& returnTypeRef_, const std::vector<TypeRef>& paramTypeRefs_) :
    Type(sourcePos_, TypeKind::functionType, typeId_), returnTypeRef(returnTypeRef_), paramTypeRefs(paramTypeRefs_), metadataRef(nullptr)
{
}

FunctionType::FunctionType(int32_t typeId_, Type* returnType_, const std::vector<Type*>& paramTypes_) : 
    Type(soul::ast::SourcePos(), TypeKind::functionType, typeId_), metadataRef(nullptr)
{
    returnTypeRef.SetType(returnType_);
    int n = paramTypes_.size();
    for (int i = 0; i < n; ++i)
    {
        TypeRef typeRef;
        typeRef.SetType(paramTypes_[i]);
        paramTypeRefs.push_back(typeRef);
    }
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

void FunctionType::WriteDeclaration(util::CodeFormatter& formatter)
{
    Type::WriteDeclaration(formatter);
    formatter.Write("function ");
    ReturnType()->Write(formatter);
    formatter.Write("(");
    int n = Arity();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            formatter.Write(", ");
        }
        ParamType(i)->Write(formatter);
    }
    formatter.Write(")");
    if (metadataRef)
    {
        formatter.Write(" ");
        metadataRef->Write(formatter);
    }
}

PointerType::PointerType(const soul::ast::SourcePos& sourcePos_, int32_t typeId_, int8_t pointerCount_, int32_t baseTypeId_) :
    Type(sourcePos_, TypeKind::pointerType, typeId_), pointerCount(pointerCount_), baseTypeRef(sourcePos_, baseTypeId_), defaultValue(this)
{
}

std::string PointerType::Name() const
{
    return baseTypeRef.GetType()->Name() + "*";
}

cmajor::systemx::assembler::Instruction* PointerType::MakeAssemblyInst(Context* context) const
{
    return new cmajor::systemx::assembler::Instruction(cmajor::systemx::assembler::OCTA);
}

Types::Types() : context(nullptr)
{
}

void Types::AddStructureType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const std::vector<TypeRef>& fieldTypeRefs, MetadataRef* metadataRef)
{
    StructureType* structureType = new StructureType(sourcePos, typeId, fieldTypeRefs);
    structureType->SetMetadataRef(metadataRef);
    types.push_back(std::unique_ptr<Type>(structureType));
}

void Types::AddArrayType(const soul::ast::SourcePos& sourcePos, int32_t typeId, int64_t size, const TypeRef& elementTypeRef, MetadataRef* metadataRef)
{
    ArrayType* arrayType = new ArrayType(sourcePos, typeId, size, elementTypeRef);
    arrayType->SetMetadataRef(metadataRef);
    types.push_back(std::unique_ptr<Type>(arrayType));
}

void Types::AddFunctionType(const soul::ast::SourcePos& sourcePos, int32_t typeId, const TypeRef& returnTypeRef, const std::vector<TypeRef>& paramTypeRefs, 
    MetadataRef* metadataRef)
{
    FunctionType* functionType = new FunctionType(sourcePos, typeId, returnTypeRef, paramTypeRefs);
    functionType->SetMetadataRef(metadataRef);
    types.push_back(std::unique_ptr<Type>(functionType));
}

void Types::Add(Type* type, Context* context)
{
    Type* prev = Get(type->Id());
    if (prev)
    {
        Error("error adding type id " + std::to_string(type->Id()) + ": type id not unique", type->GetSourcePos(), context, prev->GetSourcePos());
    }
    Map(type);
    declaredTypes.push_back(type);
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
    for (Type* declaredType : declaredTypes)
    {
        declaredType->Accept(visitor);
    }
}

Type* Types::GetPointerType(Type* baseType)
{
    Map(baseType);
    return baseType->AddPointer(context);
}

PointerType* Types::MakePointerType(const soul::ast::SourcePos& sourcePos, int32_t baseTypeId, int8_t pointerCount, Context* context)
{
    auto it = pointerTypeMap.find(std::make_pair(baseTypeId, pointerCount));
    if (it != pointerTypeMap.cend())
    {
        return it->second;
    }
    PointerType* type = nullptr;
    if (pointerCount > 1)
    {
        type = new PointerType(sourcePos, MakePointerTypeId(baseTypeId, pointerCount), pointerCount, MakePointerTypeId(baseTypeId, pointerCount - 1));
    }
    else if (pointerCount == 1)
    {
        type = new PointerType(sourcePos, MakePointerTypeId(baseTypeId, pointerCount), pointerCount, baseTypeId);
    }
    else
    {
        Error("pointer count > 0 expected", sourcePos, context);
    }
    ResolveType(type->BaseTypeRef(), context);
    types.push_back(std::unique_ptr<Type>(type));
    Map(type);
    pointerTypeMap[std::make_pair(baseTypeId, pointerCount)] = type;
    return type;
}

StructureType* Types::CreateStructureType()
{
    StructureType* structureType = new StructureType(userTypeId + types.size());
    types.push_back(std::unique_ptr<Type>(structureType));
    declaredTypes.push_back(structureType);
    Map(structureType);
    return structureType;
}

Type* Types::GetStructureType(const std::vector<Type*>& memberTypes)
{
    auto it = structureTypeMap.find(memberTypes);
    if (it != structureTypeMap.end())
    {
        return it->second;
    }
    else
    {
        StructureType* structureType = new StructureType(userTypeId + types.size(), memberTypes);
        structureTypeMap[memberTypes] = structureType;
        types.push_back(std::unique_ptr<Type>(structureType));
        declaredTypes.push_back(structureType);
        Map(structureType);
        return structureType;
    }
}

Type* Types::GetFunctionType(Type* returnType, const std::vector<Type*>& paramTypes)
{
    FunctionTypeKey key(returnType, paramTypes);
    auto it = functionTypeMap.find(key);
    if (it != functionTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        FunctionType* functionType = new FunctionType(userTypeId + types.size(), returnType, paramTypes);
        functionTypeMap[key] = functionType;
        types.push_back(std::unique_ptr<Type>(functionType));
        declaredTypes.push_back(functionType);
        Map(functionType);
        return functionType;
    }
}

Type* Types::GetArrayType(int64_t size, Type* elementType)
{
    ArrayTypeKey key(elementType, size);
    auto it = arrayTypeMap.find(key);
    if (it != arrayTypeMap.cend())
    {
        return it->second;
    }
    else
    {
        ArrayType* arrayType = new ArrayType(userTypeId + types.size(), size, elementType);
        arrayTypeMap[key] = arrayType;
        types.push_back(std::unique_ptr<Type>(arrayType));
        declaredTypes.push_back(arrayType);
        Map(arrayType);
        return arrayType;
    }
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
            type = MakePointerType(typeRef.GetSourcePos(), GetBaseTypeId(typeRef.Id()), GetPointerCount(typeRef.Id()), context);
        }
    }
    if (!type)
    {
        Error("error resolving type: type id " + std::to_string(typeRef.Id()) + " not found", typeRef.GetSourcePos(), context);
    }
    typeRef.SetType(type);
}

void Types::Write(util::CodeFormatter& formatter)
{
    if (types.empty()) return;
    formatter.WriteLine("types");
    formatter.WriteLine("{");
    formatter.IncIndent();
    for (const auto& type : declaredTypes)
    {
        type->WriteDeclaration(formatter);
        formatter.WriteLine();
    }
    formatter.DecIndent();
    formatter.WriteLine("}");
    formatter.WriteLine();
}

} // cmajor::systemx::intermediate