// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.systemx.object.type;

import cmajor.systemx.object.expr;
import cmajor.systemx.object.binary.file;
import cmajor.systemx.object.function.table;
import cmajor.systemx.object.type.table;
import util;

namespace cmajor::systemx::object {

Type::Type(TypeKind kind_, int32_t id_, const std::string& name_) : kind(kind_), id(id_), name(name_), nameId(-1)
{
}

Type::Type(TypeKind kind_) : kind(kind_), id(-1), name(), nameId(-1)
{
}

Type::~Type()
{
}

std::unique_ptr<TypedValue> Type::Evaluate(EvaluationContext& context)
{
    return std::unique_ptr<TypedValue>(new ErrorValue());
}

void Type::AddStrings(StringTable* stringTable)
{
    nameId = stringTable->AddString(name);
}

void Type::Write(Section* section)
{
    section->Align(4);
    section->EmitByte(static_cast<uint8_t>(kind));
    section->Align(4);
    section->EmitTetra(id);
    section->EmitTetra(nameId);
}

int64_t Type::Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable, StringTable& stringTable)
{
    int64_t addr = address;
    id = memory.ReadTetra(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 4;
    nameId = memory.ReadTetra(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 4;
    name = stringTable.GetString(symbolTable, nameId, rv, memory);
    return addr;
}

VoidType::VoidType() : Type(TypeKind::fundamentalType, voidTypeId, "void")
{
}

BoolType::BoolType() : Type(TypeKind::fundamentalType, boolTypeId, "bool")
{
}

std::unique_ptr<TypedValue> BoolType::Evaluate(EvaluationContext& context)
{
    bool value = context.memory.ReadByte(context.rv, context.address, cmajor::systemx::machine::Protection::read) == 1;
    BoolValue* boolValue = new BoolValue(value);
    boolValue->SetType(this);
    return std::unique_ptr<TypedValue>(boolValue);
}

SByteType::SByteType() : Type(TypeKind::fundamentalType, sbyteTypeId, "sbyte")
{
}

std::unique_ptr<TypedValue> SByteType::Evaluate(EvaluationContext& context)
{
    int8_t value = static_cast<int8_t>(context.memory.ReadByte(context.rv, context.address, cmajor::systemx::machine::Protection::read));
    SByteValue* sbyteValue = new SByteValue(value);
    sbyteValue->SetType(this);
    return std::unique_ptr<TypedValue>(sbyteValue);
}

ByteType::ByteType() : Type(TypeKind::fundamentalType, byteTypeId, "byte")
{
}

std::unique_ptr<TypedValue> ByteType::Evaluate(EvaluationContext& context)
{
    uint8_t value = context.memory.ReadByte(context.rv, context.address, cmajor::systemx::machine::Protection::read);
    ByteValue* byteValue = new ByteValue(value);
    byteValue->SetType(this);
    return std::unique_ptr<TypedValue>(byteValue);
}

ShortType::ShortType() : Type(TypeKind::fundamentalType, shortTypeId, "short")
{
}

std::unique_ptr<TypedValue> ShortType::Evaluate(EvaluationContext& context)
{
    int16_t value = static_cast<int16_t>(context.memory.ReadWyde(context.rv, context.address, cmajor::systemx::machine::Protection::read));
    ShortValue* shortValue = new ShortValue(value);
    shortValue->SetType(this);
    return std::unique_ptr<TypedValue>(shortValue);
}

UShortType::UShortType() : Type(TypeKind::fundamentalType, ushortTypeId, "ushort")
{
}

std::unique_ptr<TypedValue> UShortType::Evaluate(EvaluationContext& context)
{
    uint16_t value = context.memory.ReadWyde(context.rv, context.address, cmajor::systemx::machine::Protection::read);
    UShortValue* ushortValue = new UShortValue(value);
    ushortValue->SetType(this);
    return std::unique_ptr<TypedValue>(ushortValue);
}

IntType::IntType() : Type(TypeKind::fundamentalType, intTypeId, "int")
{
}

std::unique_ptr<TypedValue> IntType::Evaluate(EvaluationContext& context)
{
    int32_t value = static_cast<int32_t>(context.memory.ReadTetra(context.rv, context.address, cmajor::systemx::machine::Protection::read));
    IntValue* intValue = new IntValue(value);
    intValue->SetType(this);
    return std::unique_ptr<TypedValue>(intValue);
}

UIntType::UIntType() : Type(TypeKind::fundamentalType, uintTypeId, "uint")
{
}

std::unique_ptr<TypedValue> UIntType::Evaluate(EvaluationContext& context)
{
    uint32_t value = context.memory.ReadTetra(context.rv, context.address, cmajor::systemx::machine::Protection::read);
    UIntValue* uintValue = new UIntValue(value);
    uintValue->SetType(this);
    return std::unique_ptr<TypedValue>(uintValue);
}

LongType::LongType() : Type(TypeKind::fundamentalType, longTypeId, "long")
{
}

std::unique_ptr<TypedValue> LongType::Evaluate(EvaluationContext& context)
{
    int64_t value = static_cast<int64_t>(context.memory.ReadOcta(context.rv, context.address, cmajor::systemx::machine::Protection::read));
    LongValue* longValue = new LongValue(value);
    longValue->SetType(this);
    return std::unique_ptr<TypedValue>(longValue);
}

ULongType::ULongType() : Type(TypeKind::fundamentalType, ulongTypeId, "ulong")
{
}

std::unique_ptr<TypedValue> ULongType::Evaluate(EvaluationContext& context)
{
    uint64_t value = context.memory.ReadOcta(context.rv, context.address, cmajor::systemx::machine::Protection::read);
    ULongValue* ulongValue = new ULongValue(value);
    ulongValue->SetType(this);
    return std::unique_ptr<TypedValue>(ulongValue);
}

FloatType::FloatType() : Type(TypeKind::fundamentalType, floatTypeId, "float")
{
}

std::unique_ptr<TypedValue> FloatType::Evaluate(EvaluationContext& context)
{
    uint32_t tetra = context.memory.ReadTetra(context.rv, context.address, cmajor::systemx::machine::Protection::read);
    float value = *static_cast<float*>(static_cast<void*>(&tetra));
    FloatValue* floatValue = new FloatValue(value);
    floatValue->SetType(this);
    return std::unique_ptr<TypedValue>(floatValue);
}

DoubleType::DoubleType() : Type(TypeKind::fundamentalType, doubleTypeId, "double")
{
}

std::unique_ptr<TypedValue> DoubleType::Evaluate(EvaluationContext& context)
{
    uint64_t octa = context.memory.ReadOcta(context.rv, context.address, cmajor::systemx::machine::Protection::read);
    double value = *static_cast<double*>(static_cast<void*>(&octa));
    DoubleValue* doubleValue = new DoubleValue(value);
    doubleValue->SetType(this);
    return std::unique_ptr<TypedValue>(doubleValue);
}

Field::Field() : name(), typeId(-1), offset(-1), nameId(-1)
{
}

Field::Field(const std::string& name_, int32_t typeId_, int32_t offset_) : name(name_), typeId(typeId_), offset(offset_), nameId(-1)
{
}

void Field::AddStrings(StringTable* stringTable)
{
    nameId = stringTable->AddString(name);
}

void Field::Write(Section* section)
{
    section->EmitTetra(nameId);
    section->EmitTetra(typeId);
    section->EmitTetra(offset);
}

int64_t Field::Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable, StringTable& stringTable)
{
    int64_t addr = address;
    nameId = memory.ReadTetra(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 4;
    name = stringTable.GetString(symbolTable, nameId, rv, memory);
    typeId = memory.ReadTetra(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 4;
    offset = memory.ReadTetra(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 4;
    return addr;
}

StructureType::StructureType() : Type(TypeKind::structureType), size(0)
{
}

StructureType::StructureType(int32_t id_, const std::string& name_, int64_t size_) : Type(TypeKind::structureType, id_, name_), size(size_)
{
}

void StructureType::AddField(Field&& field)
{
    fields.push_back(std::move(field));
}

void StructureType::AddStrings(StringTable* stringTable)
{
    Type::AddStrings(stringTable);
    for (auto& field : fields)
    {
        field.AddStrings(stringTable);
    }
}

void StructureType::Write(Section* section)
{
    Type::Write(section);
    section->Align(8);
    section->EmitOcta(size);
    section->EmitTetra(fields.size());
    for (auto& field : fields)
    {
        field.Write(section);
    }
}

int64_t StructureType::Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable, StringTable& stringTable)
{
    int64_t addr = address;
    addr = Type::Read(addr, rv, memory, symbolTable, stringTable);
    addr = util::Align(addr, 8);
    size = memory.ReadOcta(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 8;
    int32_t fieldCount = memory.ReadTetra(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 4;
    for (int32_t i = 0; i < fieldCount; ++i)
    {
        Field field;
        addr = field.Read(addr, rv, memory, symbolTable, stringTable);
        fields.push_back(std::move(field));
    }
    return addr;
}

std::unique_ptr<TypedValue> StructureType::Evaluate(EvaluationContext& context)
{
    StructureValue* structureValue = new StructureValue();
    structureValue->SetType(this);
    for (const auto& field : fields)
    {
        Type* fieldType = context.typeTable->GetType(field.TypeId(), context.rv, context.memory, context.stringTable, context.symbolTable);
        EvaluationContext fieldEvaluationContext = context;
        fieldEvaluationContext.address = context.address + field.Offset();
        std::unique_ptr<TypedValue> value = fieldType->Evaluate(fieldEvaluationContext);
        FieldValue fieldValue(field.Name(), value.get());
        structureValue->AddFieldValue(std::move(fieldValue));
        context.values->push_back(std::move(value));
    }
    return std::unique_ptr<TypedValue>(structureValue);
}

ArrayType::ArrayType() : Type(TypeKind::arrayType)
{
}

ArrayType::ArrayType(int32_t id_, const std::string& name_, int32_t elementTypeId_, int64_t size_) :
    Type(TypeKind::arrayType, id_, name_), elementTypeId(elementTypeId_), size(size_)
{
}

void ArrayType::Write(Section* section)
{
    Type::Write(section);
    section->EmitTetra(elementTypeId);
    section->Align(8);
    section->EmitOcta(size);
}

int64_t ArrayType::Read(int64_t address, uint64_t rv, cmajor::systemx::machine::Memory& memory, SymbolTable& symbolTable, StringTable& stringTable)
{
    int64_t addr = address;
    addr = Type::Read(addr, rv, memory, symbolTable, stringTable);
    elementTypeId = memory.ReadTetra(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 4;
    addr = util::Align(addr, 8);
    size = memory.ReadOcta(rv, addr, cmajor::systemx::machine::Protection::read);
    addr += 8;
    return addr;
}

std::unique_ptr<TypedValue> ArrayType::Evaluate(EvaluationContext& context)
{
    ArrayValue* arrayValue = new ArrayValue();
    arrayValue->SetType(this);
    Type* elementType = context.typeTable->GetType(elementTypeId, context.rv, context.memory, context.stringTable, context.symbolTable);
    int64_t count = std::min(context.pageSize, static_cast<int64_t>(size / elementType->Size()));
    count -= context.start;
    for (int64_t i = 0; i < count; ++i)
    {
        int64_t index = i + context.start;
        EvaluationContext elementEvaluationContext = context;
        elementEvaluationContext.address = context.address + index * elementType->Size();
        std::unique_ptr<TypedValue> value = elementType->Evaluate(elementEvaluationContext);
        arrayValue->AddElementValue(ElementValue(index, value.get()));
        context.values->push_back(std::move(value));
    }
    return std::unique_ptr<TypedValue>(arrayValue);
}

FunctionType::FunctionType() : Type(TypeKind::functionType)
{
}

FunctionType::FunctionType(int32_t id_, const std::string& name_) : Type(TypeKind::functionType, id_, name_)
{
}

std::unique_ptr<TypedValue> FunctionType::Evaluate(EvaluationContext& context)
{
    FunctionValue* functionValue = new FunctionValue();
    functionValue->SetType(this);
    functionValue->SetName(Name());
    return std::unique_ptr<TypedValue>(functionValue);
}

PointerType::PointerType(int32_t id_, const std::string& name_, int8_t pointerCount_, Type* baseType_) : 
    Type(TypeKind::pointerType, id_, name_), pointerCount(pointerCount_), baseType(baseType_)
{
}

std::unique_ptr<TypedValue> PointerType::Evaluate(EvaluationContext& context)
{
    PointerValue* pointerValue = new PointerValue();
    pointerValue->SetType(this);
    uint64_t address = context.memory.ReadOcta(context.rv, context.address, cmajor::systemx::machine::Protection::read);
    pointerValue->SetAddress(address);
    return std::unique_ptr<TypedValue>(pointerValue);
}

SystemType::SystemType(TypeKind kind_, int32_t id_, const std::string& name_, Type* baseType_) : Type(kind_, id_, name_), baseType(baseType_)
{
}

StringType::StringType(int32_t id_, Type* baseType_) : SystemType(TypeKind::stringType, id_, "string", baseType_)
{
}

std::unique_ptr<TypedValue> StringType::Evaluate(EvaluationContext& context)
{
    std::unique_ptr<TypedValue> value = BaseType()->Evaluate(context);
    TypedValue* val = value->Get();
    if (val->IsStructureValue())
    {
        StructureValue* svalue = static_cast<StructureValue*>(val);
        TypedValue* lenValue = svalue->GetFieldValue("len").Value()->Get();
        if (lenValue->IsLongValue())
        {
            LongValue* lenLongValue = static_cast<LongValue*>(lenValue);
            int64_t length = lenLongValue->Value();
            TypedValue* ptrValue = svalue->GetFieldValue("chars").Value()->Get();
            if (ptrValue->IsPointerValue())
            {
                PointerValue* charsPtrValue = static_cast<PointerValue*>(ptrValue);
                Type* type = charsPtrValue->GetType();
                if (type && type->IsPointerType())
                {
                    PointerType* charsPtrType = static_cast<PointerType*>(type);
                    if (charsPtrType->PointerCount() == 1)
                    {
                        Type* charType = charsPtrType->BaseType();
                        if (charType->IsByteType())
                        {
                            std::string s;
                            for (int64_t index = 0; index < length; ++index)
                            {
                                IndexExpr expr(new ValueExpr(ptrValue), index);
                                expr.Evaluate(context);
                                std::unique_ptr<TypedValue> charValue = context.Pop();
                                TypedValue* chrValue = charValue->Get();
                                if (chrValue->IsByteValue())
                                {
                                    ByteValue* byteValue = static_cast<ByteValue*>(chrValue);
                                    s.append(1, static_cast<char>(byteValue->Value()));
                                }
                                else
                                {
                                    throw std::runtime_error("byte value expected");
                                }
                            }
                            StringValue* stringValue = new StringValue();
                            stringValue->SetStr(s);
                            return std::unique_ptr<TypedValue>(stringValue);
                        }
                        else if (charType->IsUShortType())
                        {
                            std::u16string s;
                            for (int64_t index = 0; index < length; ++index)
                            {
                                IndexExpr expr(new ValueExpr(ptrValue), index);
                                expr.Evaluate(context);
                                std::unique_ptr<TypedValue> charValue = context.Pop();
                                TypedValue* chrValue = charValue->Get();
                                if (chrValue->IsUShortValue())
                                {
                                    UShortValue* ushortValue = static_cast<UShortValue*>(chrValue);
                                    s.append(1, static_cast<char16_t>(ushortValue->Value()));
                                }
                                else
                                {
                                    throw std::runtime_error("ushort value expected");
                                }
                            }
                            WStringValue* stringValue = new WStringValue();
                            stringValue->SetStr(s);
                            return std::unique_ptr<TypedValue>(stringValue);
                        }
                        else if (charType->IsUIntType())
                        {
                            std::u32string s;
                            for (int64_t index = 0; index < length; ++index)
                            {
                                IndexExpr expr(new ValueExpr(ptrValue), index);
                                expr.Evaluate(context);
                                std::unique_ptr<TypedValue> charValue = context.Pop();
                                TypedValue* chrValue = charValue->Get();
                                if (chrValue->IsUIntValue())
                                {
                                    UIntValue* uintValue = static_cast<UIntValue*>(chrValue);
                                    s.append(1, static_cast<char32_t>(uintValue->Value()));
                                }
                                else
                                {
                                    throw std::runtime_error("uint value expected");
                                }
                            }
                            UStringValue* stringValue = new UStringValue();
                            stringValue->SetStr(s);
                            return std::unique_ptr<TypedValue>(stringValue);
                        }
                        else
                        {
                            throw std::runtime_error("byte, ushort or uint type expected");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("pointer count 1 expected");
                    }
                }
                else
                {
                    throw std::runtime_error("pointer type expected");
                }
            }
            else
            {
                throw std::runtime_error("pointer value expected");
            }
        }
        else
        {
            throw std::runtime_error("long value expected");
        }
    }
    else
    {
        throw std::runtime_error("structure value expected");
    }
}

ListType::ListType(int32_t id_, Type* baseType_) : SystemType(TypeKind::listType, id_, "list", baseType_)
{
}

std::unique_ptr<TypedValue> ListType::Evaluate(EvaluationContext& context)
{
    std::unique_ptr<TypedValue> value = BaseType()->Evaluate(context);
    TypedValue* val = value->Get();
    if (val->IsStructureValue())
    {
        StructureValue* svalue = static_cast<StructureValue*>(val);
        TypedValue* typedCountValue = svalue->GetFieldValue("count").Value()->Get();
        if (typedCountValue->IsLongValue())
        {
            LongValue* countValue = static_cast<LongValue*>(typedCountValue);
            int64_t count = countValue->Value();
            TypedValue* itemsValue = svalue->GetFieldValue("items").Value()->Get();
            if (itemsValue->IsPointerValue())
            {
                PointerValue* itemsPtrValue = static_cast<PointerValue*>(itemsValue);
                ContainerValue* containerValue = new ContainerValue();
                containerValue->SetCount(count);
                containerValue->SetType(this);
                if (context.content)
                {
                    containerValue->SetContent();
                    if (context.count == 0)
                    {
                        context.count = context.pageSize;
                    }
                    containerValue->SetStart(context.start);
                    containerValue->SetAddress(context.address);
                    int64_t n = std::min(context.count, count - context.start);
                    for (int64_t i = 0; i < n; ++i)
                    {
                        int64_t index = i + context.start;
                        IndexExpr expr(new ValueExpr(itemsPtrValue), index);
                        EvaluationContext elementContext = context;
                        elementContext.content = false;
                        expr.Evaluate(elementContext);
                        std::unique_ptr<TypedValue> elemValue = elementContext.Pop();
                        ElementValue* elementValue = new ElementValue(index, elemValue.get());
                        context.values->push_back(std::move(elemValue));
                        containerValue->AddElement(elementValue);
                    }
                    if (n < count - context.start && context.count == context.pageSize)
                    {
                        containerValue->SetNextStart(context.start + n);
                    }
                }
                return std::unique_ptr<TypedValue>(containerValue);
            }
            else
            {
                throw std::runtime_error("pointer value expected");
            }
        }
        else
        {
            throw std::runtime_error("long value expected");
        }
    }
    else
    {
        throw std::runtime_error("structure value expected");
    }
}

TreeType::TreeType(TypeKind kind_, int32_t id_, const std::string& name_, Type* baseType_) : SystemType(kind_, id_, name_, baseType_)
{
}

std::unique_ptr<TypedValue> TreeType::Evaluate(EvaluationContext& context)
{
    std::unique_ptr<TypedValue> value = BaseType()->Evaluate(context);
    TypedValue* val = value->Get();
    if (val->IsStructureValue())
    {
        StructureValue* svalue = static_cast<StructureValue*>(val);
        TypedValue* typedTreeValue = svalue->GetFieldValue("tree").Value()->Get();
        if (typedTreeValue->IsStructureValue())
        {
            StructureValue* treeValue = static_cast<StructureValue*>(typedTreeValue);
            TypedValue* typedCountValue = treeValue->GetFieldValue("count").Value()->Get();
            if (typedCountValue->IsLongValue())
            {
                LongValue* countValue = static_cast<LongValue*>(typedCountValue);
                int64_t count = countValue->Value();
                ContainerValue* containerValue = new ContainerValue();
                containerValue->SetCount(count);
                containerValue->SetType(this);
                if (context.content)
                {
                    containerValue->SetContent();
                    if (context.count == 0)
                    {
                        context.count = context.pageSize;
                    }
                    containerValue->SetStart(context.start);
                    containerValue->SetAddress(context.address);
                    TypedValue* typedHeaderValue = treeValue->GetFieldValue("header").Value()->Get();
                    if (typedHeaderValue->IsStructureValue())
                    {
                        StructureValue* headerValue = static_cast<StructureValue*>(typedHeaderValue);
                        TypedValue* headerPtrValue = headerValue->GetFieldValue("ptr").Value()->Get();
                        if (headerPtrValue->IsPointerValue())
                        {
                            PointerValue* headerPtr = static_cast<PointerValue*>(headerPtrValue);
                            ArrowExpr baseExpr(new ValueExpr(headerPtr), "base");
                            EvaluationContext baseContext = context;
                            baseExpr.Evaluate(baseContext);
                            std::unique_ptr<TypedValue> uniqueTypedBaseValue = baseContext.Pop();
                            TypedValue* typedBaseValue = uniqueTypedBaseValue->Get();
                            context.values->push_back(std::move(uniqueTypedBaseValue));
                            if (typedBaseValue->IsStructureValue())
                            {
                                StructureValue* baseValue = static_cast<StructureValue*>(typedBaseValue);
                                TypedValue* typedLefmostPtrValue = baseValue->GetFieldValue("left").Value()->Get();
                                if (typedLefmostPtrValue->IsPointerValue())
                                {
                                    PointerValue* ptr = static_cast<PointerValue*>(typedLefmostPtrValue);
                                    for (int64_t i = 0; i < context.start; ++i)
                                    {
                                        ptr = Next(ptr, headerPtrValue->GetType(), context);
                                    }
                                    int64_t n = std::min(context.count, count - context.start);
                                    for (int64_t i = 0; i < n; ++i)
                                    {
                                        int64_t index = i + context.start;
                                        CastExpr cast(new ValueExpr(ptr), headerPtr->GetType());
                                        EvaluationContext castContext = context;
                                        cast.Evaluate(castContext);
                                        std::unique_ptr<TypedValue> castedTypedValue = castContext.Pop();
                                        TypedValue* castedValue = castedTypedValue->Get();
                                        context.values->push_back(std::move(castedTypedValue));
                                        if (castedValue->IsPointerValue())
                                        {
                                            PointerValue* castedPtrValue = static_cast<PointerValue*>(castedValue);
                                            ArrowExpr valueExpr(new ValueExpr(castedPtrValue), "value");
                                            EvaluationContext valueContext = context;
                                            valueExpr.Evaluate(valueContext);
                                            std::unique_ptr<TypedValue> elemValue = valueContext.Pop();
                                            ElementValue* elementValue = new ElementValue(index, elemValue.get());
                                            context.values->push_back(std::move(elemValue));
                                            containerValue->AddElement(elementValue);
                                        }
                                        else
                                        {
                                            throw std::runtime_error("pointer value expected");
                                        }
                                        ptr = Next(ptr, headerPtr->GetType(), context);
                                    }
                                }
                                else
                                {
                                    throw std::runtime_error("pointer value expected");
                                }
                            }
                            else
                            {
                                throw std::runtime_error("structure value expected");
                            }
                        }
                        else
                        {
                            throw std::runtime_error("pointer value expected");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("structure value expected");
                    }
                }
                return std::unique_ptr<TypedValue>(containerValue);
            }
            else
            {
                throw std::runtime_error("long value expected");
            }
        }
        else
        {
            throw std::runtime_error("structure value expected");
        }
    }
    else
    {
        throw std::runtime_error("structure value expected");
    }
}

PointerValue* TreeType::Min(PointerValue* ptr, EvaluationContext& context)
{
    EvaluationContext arrowContext = context;
    ArrowExpr arrowExpr(new ValueExpr(ptr), "left");
    arrowExpr.Evaluate(arrowContext);
    std::unique_ptr<TypedValue> leftPtrValue = arrowContext.Pop();
    TypedValue* leftValue = leftPtrValue->Get();
    context.values->push_back(std::move(leftPtrValue));
    if (leftValue->IsPointerValue())
    {
        PointerValue* left = static_cast<PointerValue*>(leftValue);
        while (!left->IsNull())
        {
            ptr = left;
            ArrowExpr arrowExpr(new ValueExpr(ptr), "left");
            arrowExpr.Evaluate(arrowContext);
            std::unique_ptr<TypedValue> leftPtrValue = arrowContext.Pop();
            TypedValue* leftValue = leftPtrValue->Get();
            context.values->push_back(std::move(leftPtrValue));
            if (leftValue->IsPointerValue())
            {
                left = static_cast<PointerValue*>(leftValue);
            }
            else
            {
                throw std::runtime_error("pointer value expected");
            }
        }
    }
    else
    {
        throw std::runtime_error("pointer value expected");
    }
    return ptr;
}

PointerValue* TreeType::Next(PointerValue* ptr, Type* headerPtrType, EvaluationContext& context)
{
    CastExpr cast(new ValueExpr(ptr), headerPtrType);
    EvaluationContext castContext = context;
    cast.Evaluate(castContext);
    std::unique_ptr<TypedValue> castedTypedValue = castContext.Pop();
    TypedValue* castedValue = castedTypedValue->Get();
    context.values->push_back(std::move(castedTypedValue));
    if (castedValue->IsPointerValue())
    {
        PointerValue* castedPtrValue = static_cast<PointerValue*>(castedValue);
        ptr = castedPtrValue;
    }
    else
    {
        throw std::runtime_error("pointer value expected");
    }
    ArrowExpr baseExpr(new ValueExpr(ptr), "base");
    EvaluationContext baseContext = context;
    baseExpr.Evaluate(baseContext);
    std::unique_ptr<TypedValue> uniqueTypedBaseValue = baseContext.Pop();
    TypedValue* typedBaseValue = uniqueTypedBaseValue->Get();
    context.values->push_back(std::move(uniqueTypedBaseValue));
    if (typedBaseValue->IsStructureValue())
    {
        StructureValue* baseValue = static_cast<StructureValue*>(typedBaseValue);
        TypedValue* rightValue = baseValue->GetFieldValue("right").Value()->Get();
        if (rightValue->IsPointerValue())
        {
            PointerValue* right  = static_cast<PointerValue*>(rightValue);
            if (!right->IsNull())
            {
                return Min(right, context);
            }
            else
            {
                ArrowExpr baseExpr(new ValueExpr(ptr), "base");
                EvaluationContext baseContext = context;
                baseExpr.Evaluate(baseContext);
                std::unique_ptr<TypedValue> uniqueTypedBaseValue = baseContext.Pop();
                TypedValue* typedBaseValue = uniqueTypedBaseValue->Get();
                context.values->push_back(std::move(uniqueTypedBaseValue));
                if (typedBaseValue->IsStructureValue())
                {
                    StructureValue* baseValue = static_cast<StructureValue*>(typedBaseValue);
                    TypedValue* parentValue = baseValue->GetFieldValue("parent").Value()->Get();
                    if (parentValue->IsPointerValue())
                    {
                        PointerValue* parent = static_cast<PointerValue*>(parentValue);
                        PointerValue* u = parent;
                        ArrowExpr arrowExpr(new ValueExpr(u), "right");
                        EvaluationContext rightContext = context;
                        arrowExpr.Evaluate(rightContext);
                        std::unique_ptr<TypedValue> uniqueRightValue = rightContext.Pop();
                        TypedValue* rightValue = uniqueRightValue->Get();
                        context.values->push_back(std::move(uniqueRightValue));
                        if (rightValue->IsPointerValue())
                        {
                            PointerValue* right = static_cast<PointerValue*>(rightValue);
                            while (ptr->Address() == right->Address())
                            {
                                ptr = u;
                                ArrowExpr arrowExpr(new ValueExpr(u), "parent");
                                EvaluationContext parentContext = context;
                                arrowExpr.Evaluate(parentContext);
                                std::unique_ptr<TypedValue> uniqueParentValue = parentContext.Pop();
                                TypedValue* parentValue = uniqueParentValue->Get();
                                context.values->push_back(std::move(uniqueParentValue));
                                if (parentValue->IsPointerValue())
                                {
                                    PointerValue* parent = static_cast<PointerValue*>(parentValue);
                                    u = parent;
                                }
                                else
                                {
                                    throw std::runtime_error("pointer value expected");
                                }
                            }
                            CastExpr cast(new ValueExpr(ptr), headerPtrType);
                            EvaluationContext castContext = context;
                            cast.Evaluate(castContext);
                            std::unique_ptr<TypedValue> castedTypedValue = castContext.Pop();
                            TypedValue* castedValue = castedTypedValue->Get();
                            context.values->push_back(std::move(castedTypedValue));
                            if (castedValue->IsPointerValue())
                            {
                                PointerValue* castedPtrValue = static_cast<PointerValue*>(castedValue);
                                ptr = castedPtrValue;
                            }
                            else
                            {
                                throw std::runtime_error("pointer value expected");
                            }
                            ArrowExpr baseExpr(new ValueExpr(ptr), "base");
                            EvaluationContext baseContext = context;
                            baseExpr.Evaluate(baseContext);
                            std::unique_ptr<TypedValue> uniqueTypedBaseValue = baseContext.Pop();
                            TypedValue* typedBaseValue = uniqueTypedBaseValue->Get();
                            context.values->push_back(std::move(uniqueTypedBaseValue));
                            if (typedBaseValue->IsStructureValue())
                            {
                                StructureValue* baseValue = static_cast<StructureValue*>(typedBaseValue);
                                TypedValue* rightValue = baseValue->GetFieldValue("right").Value()->Get();
                                if (rightValue->IsPointerValue())
                                {
                                    PointerValue* right = static_cast<PointerValue*>(rightValue);
                                    if (right->Address() != u->Address())
                                    {
                                        return u;
                                    }
                                    else
                                    {
                                        return ptr;
                                    }
                                }
                                else
                                {
                                    throw std::runtime_error("pointer value expected");
                                }
                            }
                            else
                            {
                                throw std::runtime_error("structure value expected");
                            }
                        }
                        else
                        {
                            throw std::runtime_error("pointer value expected");
                        }
                    }
                    else
                    {
                        throw std::runtime_error("pointer value expected");
                    }
                }
                else
                {
                    throw std::runtime_error("structure value expected");
                }
            }
        }
        else
        {
            throw std::runtime_error("pointer value expected");
        }
    }
    else
    {
        throw std::runtime_error("structure value expected");
    }
    return ptr;
}

SetType::SetType(int32_t id_, Type* baseType_) : TreeType(TypeKind::setType, id_, "set", baseType_)
{
}

MapType::MapType(int32_t id_, Type* baseType_) : TreeType(TypeKind::mapType, id_, "map", baseType_)
{
}

LinkedListType::LinkedListType(int32_t id_, Type* baseType_) : SystemType(TypeKind::linkedListType, id_, "linked_list", baseType_)
{
}

std::unique_ptr<TypedValue> LinkedListType::Evaluate(EvaluationContext& context)
{
    std::unique_ptr<TypedValue> value = BaseType()->Evaluate(context);
    TypedValue* val = value->Get();
    if (val->IsStructureValue())
    {
        StructureValue* svalue = static_cast<StructureValue*>(val);
        TypedValue* typedCountValue = svalue->GetFieldValue("count").Value()->Get();
        if (typedCountValue->IsLongValue())
        {
            LongValue* countValue = static_cast<LongValue*>(typedCountValue);
            int64_t count = countValue->Value();
            ContainerValue* containerValue = new ContainerValue();
            containerValue->SetCount(count);
            containerValue->SetType(this);
            if (context.content)
            {
                containerValue->SetContent();
                if (context.count == 0)
                {
                    context.count = context.pageSize;
                }
                containerValue->SetStart(context.start);
                containerValue->SetAddress(context.address);
                TypedValue* headValue = svalue->GetFieldValue("head").Value()->Get();
                if (headValue->IsPointerValue())
                {
                    PointerValue* ptr = static_cast<PointerValue*>(headValue);
                    for (int64_t i = 0; i < context.start; ++i)
                    {
                        ptr = Next(ptr, context);
                    }
                    int64_t n = std::min(context.count, count - context.start);
                    for (int64_t i = 0; i < n; ++i)
                    {
                        int64_t index = i + context.start;
                        ArrowExpr valueExpr(new ValueExpr(ptr), "value");
                        EvaluationContext valueContext = context;
                        valueExpr.Evaluate(valueContext);
                        std::unique_ptr<TypedValue> elemValue = valueContext.Pop();
                        ElementValue* elementValue = new ElementValue(index, elemValue.get());
                        context.values->push_back(std::move(elemValue));
                        containerValue->AddElement(elementValue);
                        ptr = Next(ptr, context);
                    }
                }
                else
                {
                    throw std::runtime_error("pointer value expected");
                }
            }
            return std::unique_ptr<TypedValue>(containerValue);
        }
        else
        {
            throw std::runtime_error("long value expected");
        }
    }
    else
    {
        throw std::runtime_error("structure value expected");
    }
}

PointerValue* LinkedListType::Next(PointerValue* ptr, EvaluationContext& context)
{
    ArrowExpr nextExpr(new ValueExpr(ptr), "next");
    EvaluationContext nextContext = context;
    nextExpr.Evaluate(nextContext);
    std::unique_ptr<TypedValue> nextValue = nextContext.Pop();
    TypedValue* nextVal = nextValue->Get();
    context.values->push_back(std::move(nextValue));
    if (nextVal->IsPointerValue())
    {
        ptr = static_cast<PointerValue*>(nextVal);
    }
    else
    {
        throw std::runtime_error("pointer value expected");
    }
    return ptr;
}

HashTableType::HashTableType(TypeKind kind_, int32_t id_, const std::string& name_, Type* baseType_) : SystemType(kind_, id_, name_, baseType_)
{
}

std::unique_ptr<TypedValue> HashTableType::Evaluate(EvaluationContext& context)
{
    std::unique_ptr<TypedValue> value = BaseType()->Evaluate(context);
    TypedValue* val = value->Get();
    if (val->IsStructureValue())
    {
        StructureValue* svalue = static_cast<StructureValue*>(val);
        TypedValue* typedTableValue = svalue->GetFieldValue("table").Value()->Get();
        if (typedTableValue->IsStructureValue())
        {
            StructureValue* tableValue = static_cast<StructureValue*>(typedTableValue);
            TypedValue* typedCountValue = tableValue->GetFieldValue("count").Value()->Get();
            if (typedCountValue->IsLongValue())
            {
                LongValue* countValue = static_cast<LongValue*>(typedCountValue);
                int64_t count = countValue->Value();
                ContainerValue* containerValue = new ContainerValue();
                containerValue->SetCount(count);
                containerValue->SetType(this);
                if (context.content)
                {
                    containerValue->SetContent();
                    if (context.count == 0)
                    {
                        context.count = context.pageSize;
                    }
                    containerValue->SetStart(context.start);
                    containerValue->SetAddress(context.address);
                    TypedValue* bucketsValue = tableValue->GetFieldValue("buckets").Value()->Get();
                    if (bucketsValue->IsContainerValue())
                    {
                        ContainerValue* bucketsContainer = static_cast<ContainerValue*>(bucketsValue);
                        PointerValue nullValue;
                        nullValue.SetAddress(0);
                        PointerValue* nullPtr = &nullValue;
                        int64_t bucketIndex = -1;
                        PointerValue* ptr = Next(nullPtr, nullPtr, bucketIndex, bucketsContainer->Count(), bucketsValue->GetType(), context);
                        for (int64_t i = 0; i < context.start; ++i)
                        {
                            ptr = Next(ptr, nullPtr, bucketIndex, bucketsContainer->Count(), bucketsValue->GetType(), context);
                        }
                        int64_t n = std::min(context.count, count - context.start);
                        for (int64_t i = 0; i < n; ++i)
                        {
                            int64_t index = i + context.start;
                            ArrowExpr valueExpr(new ValueExpr(ptr), "value");
                            EvaluationContext valueContext = context;
                            valueExpr.Evaluate(valueContext);
                            std::unique_ptr<TypedValue> elemValue = valueContext.Pop();
                            ElementValue* elementValue = new ElementValue(index, elemValue.get());
                            context.values->push_back(std::move(elemValue));
                            containerValue->AddElement(elementValue);
                            ptr = Next(ptr, nullPtr, bucketIndex, bucketsContainer->Count(), bucketsValue->GetType(), context);
                        }
                    }
                    else
                    {
                        throw std::runtime_error("container value expected");
                    }
                }
                return std::unique_ptr<TypedValue>(containerValue);
            }
            else
            {
                throw std::runtime_error("long value expected");
            }
        }
        else
        {
            throw std::runtime_error("structure value expected");
        }
    }
    else
    {
        throw std::runtime_error("structure value expected");
    }
}

PointerValue* HashTableType::Next(PointerValue* bucket, PointerValue* nullValue, int64_t& bucketIndex, int64_t bucketCount, Type* bucketsListType, EvaluationContext& context)
{
    if (!bucket->IsNull())
    {
        ArrowExpr arrowExpr(new ValueExpr(bucket), "next");
        EvaluationContext arrowContext = context;
        arrowExpr.Evaluate(arrowContext);
        std::unique_ptr<TypedValue> uniqueNextValue = arrowContext.Pop();
        TypedValue* nextValue = uniqueNextValue->Get();
        context.values->push_back(std::move(uniqueNextValue));
        if (nextValue->IsPointerValue())
        {
            PointerValue* next = static_cast<PointerValue*>(nextValue);
            if (!next->IsNull())
            {
                return next;
            }
            else
            {
                ++bucketIndex;
            }
        }
        else
        {
            throw std::runtime_error("pointer value expected");
        }
    }
    else
    {
        ++bucketIndex;
    }
    while (bucketIndex < bucketCount)
    {
        EvaluationContext bucketContext = context;
        bucketContext.start = bucketIndex;
        bucketContext.count = 1;
        std::unique_ptr<TypedValue> uniqueTypedContainerValue = bucketsListType->Evaluate(bucketContext);
        TypedValue* typedContainerValue = uniqueTypedContainerValue->Get();
        context.values->push_back(std::move(uniqueTypedContainerValue));
        if (typedContainerValue->IsContainerValue())
        {
            ContainerValue* containerValue = static_cast<ContainerValue*>(typedContainerValue);
            TypedValue* typedElementValue = containerValue->Elements()[0].get();
            if (typedElementValue->IsElementValue())
            {
                ElementValue* elementValue = static_cast<ElementValue*>(typedElementValue);
                TypedValue* value = elementValue->GetValue();
                if (value->IsPointerValue())
                {
                    PointerValue* bucket = static_cast<PointerValue*>(value);
                    if (!bucket->IsNull())
                    {
                        return bucket;
                    }
                }
                else
                {
                    throw std::runtime_error("pointer value expected");
                }
            }
            else
            {
                throw std::runtime_error("element value expected");
            }
        }
        else
        {
            throw std::runtime_error("container value expected");
        }
        ++bucketIndex;
    }
    return nullValue;
}

HashSetType::HashSetType(int32_t id_, Type* baseType_) : HashTableType(TypeKind::hashSetType, id_, "hash_set", baseType_)
{
}

HashMapType::HashMapType(int32_t id_, Type* baseType_) : HashTableType(TypeKind::hashMapType, id_, "hash_map", baseType_)
{
}

ForwardListType::ForwardListType(int32_t id_, Type* baseType_) : SystemType(TypeKind::forwardListType, id_, "forward_list", baseType_)
{
}

std::unique_ptr<TypedValue> ForwardListType::Evaluate(EvaluationContext& context)
{
    return std::unique_ptr<TypedValue>();
}

Type* ReadType(int64_t address, SymbolTable& symbolTable, StringTable& stringTable, uint64_t rv, cmajor::systemx::machine::Memory& memory)
{
    int64_t addr = util::Align(address, 4);
    TypeKind typeKind = static_cast<TypeKind>(memory.ReadByte(rv, addr, cmajor::systemx::machine::Protection::read));
    ++addr;
    addr = util::Align(addr, 4);
    switch (typeKind)
    {
        case TypeKind::structureType:
        {
            StructureType* structureType = new StructureType();
            structureType->Read(addr, rv, memory, symbolTable, stringTable);
            return structureType;
        }
        case TypeKind::arrayType:
        {
            ArrayType* arrayType = new ArrayType();
            arrayType->Read(addr, rv, memory, symbolTable, stringTable);
            return arrayType;
        }
        case TypeKind::functionType:
        {
            FunctionType* functionType = new FunctionType();
            functionType->Read(addr, rv, memory, symbolTable, stringTable);
            return functionType;
        }
        default:
        {
            throw std::runtime_error("structure, array or function type expected");
        }
    }
}

Type* MakeSystemType(int8_t systemTypeId, int32_t typeId, Type* baseType)
{
    switch (systemTypeId)
    {
        case stringType:
        {
            return new StringType(typeId, baseType);
        }
        case listType:
        {
            return new ListType(typeId, baseType);
        }
        case setType:
        {
            return new SetType(typeId, baseType);
        }
        case mapType:
        {
            return new MapType(typeId, baseType);
        }
        case linkedListType:
        {
            return new LinkedListType(typeId, baseType);
        }
        case hashSetType:
        {
            return new HashSetType(typeId, baseType);
        }
        case hashMapType:
        {
            return new HashMapType(typeId, baseType);
        }
        case forwardListType:
        {
            return new ForwardListType(typeId, baseType);
        }
    }
    return nullptr;
}

} // namespace cmajor::systemx::object
