// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.debug.di.type;

import cmajor.debug.debug.info;
import util;

namespace cmajor::debug {

DIType::DIType(Kind kind_) : kind(kind_), id(), name(), project(nullptr)
{
}

DIType::~DIType()
{
}

void DIType::Write(util::BinaryStreamWriter& writer)
{
    writer.Write(id);
    writer.Write(name);
    writer.Write(irName);
}

void DIType::Read(util::BinaryStreamReader& reader)
{
    reader.ReadUuid(id);
    name = reader.ReadUtf8String();
    irName = reader.ReadUtf8String();
}

void DIType::SetId(const util::uuid& id_)
{
    id = id_;
}

void DIType::SetName(const std::string& name_)
{
    name = name_;
}

void DIType::SetIrName(const std::string& irName_)
{
    irName = irName_;
}

std::string DIType::KindStr(Kind kind)
{
    switch (kind)
    {
    case Kind::primitiveType:
    {
        return "primitive";
    }
    case Kind::enumType:
    {
        return "enum";
    }
    case Kind::classType:
    {
        return "class";
    }
    case Kind::specializationType:
    {
        return "specialization";
    }
    case Kind::delegateType:
    {
        return "delegate";
    }
    case Kind::classDelegateType:
    {
        return "classDelegate";
    }
    case Kind::interfaceType:
    {
        return "interface";
    }
    case Kind::templateParameter:
    {
        return "templateParameter";
    }
    case Kind::constType:
    {
        return "const";
    }
    case Kind::referenceType:
    {
        return "reference";
    }
    case Kind::pointerType:
    {
        return "pointer";
    }
    case Kind::arrayType:
    {
        return "array";
    }
    }
    return "";
}

Scope* DIType::GetScope()
{
    return nullptr;
}

std::unique_ptr<util::JsonValue> DIType::ToJson() const
{
    util::JsonObject* jsonObject = new util::JsonObject();
    jsonObject->AddField(U"kind", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(KindStr(kind)))));
    jsonObject->AddField(U"id", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(id)))));
    jsonObject->AddField(U"name", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(name))));
    jsonObject->AddField(U"irName", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(irName))));
    return std::unique_ptr<util::JsonValue>(jsonObject);
}

DITypeRef::DITypeRef(DIType* type_) : type(type_)
{
}

std::unique_ptr<util::JsonValue> DITypeRef::ToJson()
{
    util::JsonObject* jsonObject = new util::JsonObject();
    jsonObject->AddField(U"name", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(type->Name()))));
    jsonObject->AddField(U"irName", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(type->IrName()))));
    jsonObject->AddField(U"id", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(type->Id())))));
    return std::unique_ptr<util::JsonValue>(jsonObject);
}

DIPrimitiveType::DIPrimitiveType() : DIType(DIType::Kind::primitiveType), kind(Kind::none)
{
}

void DIPrimitiveType::Write(util::BinaryStreamWriter& writer)
{
    DIType::Write(writer);
    writer.Write(static_cast<int8_t>(kind));
}

void DIPrimitiveType::Read(util::BinaryStreamReader& reader)
{
    DIType::Read(reader);
    kind = static_cast<Kind>(reader.ReadSByte());
}

std::string DIPrimitiveType::PrimitiveTypeKindStr(Kind kind)
{
    switch (kind)
    {
    case Kind::boolType: return "bool";
    case Kind::sbyteType: return "sbyte";
    case Kind::byteType: return "byte";
    case Kind::shortType: return "short";
    case Kind::ushortType: return "ushort";
    case Kind::intType: return "int";
    case Kind::uintType: return "uint";
    case Kind::longType: return "long";
    case Kind::ulongType: return "ulong";
    case Kind::floatType: return "float";
    case Kind::doubleType: return "double";
    case Kind::charType: return "char";
    case Kind::wcharType: return "wchar";
    case Kind::ucharType: return "uchar";
    case Kind::voidType: return "void";
    }
    return "none";
}

std::unique_ptr<util::JsonValue> DIPrimitiveType::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = DIType::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"primitiveType", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(PrimitiveTypeKindStr(kind)))));
    }
    return value;
}

bool DIPrimitiveType::IsIntegerType() const
{
    switch (kind)
    {
    case Kind::byteType:
    case Kind::sbyteType:
    case Kind::shortType:
    case Kind::ushortType:
    case Kind::intType:
    case Kind::uintType:
    case Kind::longType:
    case Kind::ulongType:
    {
        return true;
    }
    default:
    {
        return false;
    }
    }
}

DIEnumType::DIEnumType() : DIType(Kind::enumType), underlyingTypeId(util::nil_uuid())
{
}

void DIEnumType::SetUnderlyingTypeId(const util::uuid& underlyingTypeId_)
{
    underlyingTypeId = underlyingTypeId_;
}

void DIEnumType::AddEnumConstant(DIEnumConstant&& enumConstant)
{
    enumConstants.push_back(std::move(enumConstant));
}

DIEnumConstant* DIEnumType::GetEnumConstant(uint64_t& value)
{
    for (DIEnumConstant& enumConstant : enumConstants)
    {
        try
        {
            uint64_t enumConstantValue = std::stoull(enumConstant.Value());
            if (value == 0)
            {
                if (enumConstantValue == 0)
                {
                    return &enumConstant;
                }
            }
            else if ((value & enumConstantValue) != 0)
            {
                value = value & ~enumConstantValue;
                return &enumConstant;
            }
        }
        catch (...)
        {
        }
    }
    return nullptr;
}

void DIEnumType::Write(util::BinaryStreamWriter& writer)
{
    DIType::Write(writer);
    writer.Write(underlyingTypeId);
    int32_t n = enumConstants.size();
    writer.Write(n);
    for (int32_t i = 0; i < n; ++i)
    {
        DIEnumConstant& enumConstant = enumConstants[i];
        enumConstant.Write(writer);
    }
}

void DIEnumType::Read(util::BinaryStreamReader& reader)
{
    DIType::Read(reader);
    reader.ReadUuid(underlyingTypeId);
    int32_t n = reader.ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        DIEnumConstant enumConstant;
        enumConstant.Read(reader);
        enumConstants.push_back(std::move(enumConstant));
    }
}

std::unique_ptr<util::JsonValue> DIEnumType::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = DIType::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"underlyingTypeId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(underlyingTypeId)))));
    }
    return value;
}

DITemplateParameter::DITemplateParameter() : DIType(Kind::templateParameter)
{
}

ClassScope::ClassScope(DIClassType* classType_) : classType(classType_)
{
}

std::string ClassScope::Name() const
{
    return classType->Name();
}

void ClassScope::AddMemberVariable(DIVariable* memberVariable)
{
    memberVariableMap[memberVariable->Name()] = memberVariable;
}

DIVariable* ClassScope::GetVariable(const std::string& name) const
{
    auto it = memberVariableMap.find(name);
    if (it != memberVariableMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

DIClassType::DIClassType() : DIType(Kind::classType), scope(this), baseClassId(util::nil_uuid()), polymorphic(false), vmtPtrIndex(-1)
{
}

DIClassType::DIClassType(Kind kind) : DIType(kind), scope(this), baseClassId(util::nil_uuid()), polymorphic(false), vmtPtrIndex(-1)
{
}

DIType* DIClassType::BaseClassType() const
{
    return GetProject()->GetType(baseClassId);
}

void DIClassType::SetBaseClassId(const util::uuid& baseClassId_)
{
    baseClassId = baseClassId_;
}

void DIClassType::AddTemplateParameter(DITemplateParameter* templateParameter)
{
    templateParameters.push_back(std::unique_ptr<DITemplateParameter>(templateParameter));
}

void DIClassType::AddMemberVariable(DIVariable* memberVariable)
{
    scope.AddMemberVariable(memberVariable);
    memberVariables.push_back(std::unique_ptr<DIVariable>(memberVariable));
}

void DIClassType::SetVmtVariableName(const std::string& vmtVariableName_)
{
    vmtVariableName = vmtVariableName_;
}

void DIClassType::Write(util::BinaryStreamWriter& writer)
{
    DIType::Write(writer);
    writer.Write(baseClassId);
    int32_t ntp = templateParameters.size();
    writer.Write(ntp);
    for (int32_t i = 0; i < ntp; ++i)
    {
        DITemplateParameter* templateParameter = templateParameters[i].get();
        templateParameter->Write(writer);
    }
    int32_t nmv = memberVariables.size();
    writer.Write(nmv);
    for (int32_t i = 0; i < nmv; ++i)
    {
        DIVariable* memberVariable = memberVariables[i].get();
        memberVariable->Write(writer);
    }
    writer.Write(polymorphic);
    if (polymorphic)
    {
        writer.Write(vmtPtrIndex);
        writer.Write(vmtVariableName);
    }
}

void DIClassType::Read(util::BinaryStreamReader& reader)
{
    DIType::Read(reader);
    reader.ReadUuid(baseClassId);
    int32_t ntp = reader.ReadInt();
    for (int32_t i = 0; i < ntp; ++i)
    {
        DITemplateParameter* templateParameter = new DITemplateParameter();
        templateParameter->SetProject(GetProject());
        templateParameter->Read(reader);
        templateParameters.push_back(std::unique_ptr<DITemplateParameter>(templateParameter));
    }
    int32_t nmv = reader.ReadInt();
    for (int32_t i = 0; i < nmv; ++i)
    {
        DIVariable* memberVariable = new DIVariable(DIVariable::Kind::memberVariable);
        memberVariable->SetProject(GetProject());
        memberVariable->Read(reader);
        AddMemberVariable(memberVariable);
    }
    polymorphic = reader.ReadBool();
    if (polymorphic)
    {
        vmtPtrIndex = reader.ReadInt();
        vmtVariableName = reader.ReadUtf8String();
    }
}

std::unique_ptr<util::JsonValue> DIClassType::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = DIType::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"baseClassId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(baseClassId)))));
        util::JsonArray* templateParameterArray = new util::JsonArray();
        for (const auto& templateParameter : templateParameters)
        {
            templateParameterArray->AddItem(templateParameter->ToJson());
        }
        jsonObject->AddField(U"templateParameters", std::unique_ptr<util::JsonValue>(templateParameterArray));
        util::JsonArray* memberVariableArray = new util::JsonArray();
        for (const auto& memberVariable : memberVariables)
        {
            memberVariableArray->AddItem(memberVariable->ToJson());
        }
        jsonObject->AddField(U"memberVariables", std::unique_ptr<util::JsonValue>(memberVariableArray));
        jsonObject->AddField(U"polymorphic", std::unique_ptr<util::JsonValue>(new util::JsonBool(polymorphic)));
        if (polymorphic)
        {
            jsonObject->AddField(U"vmtPtrIndex", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(vmtPtrIndex)))));
            jsonObject->AddField(U"vmtVariableName", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(vmtVariableName))));
        }
    }
    return value;
}

int NumBaseClasses(DIClassType* classType)
{
    if (classType->BaseClassId().is_nil())
    {
        return 0;
    }
    else
    {
        return NumBaseClasses(static_cast<DIClassType*>(classType->BaseClassType())) + 1;
    }
}

DIClassTemplateSpecializationType::DIClassTemplateSpecializationType() :
    DIClassType(Kind::specializationType), containerKind(ContainerClassTemplateKind::notContainerClassTemplate),
    primaryTypeId(util::nil_uuid()), valueTypeId(util::nil_uuid())
{
}

void DIClassTemplateSpecializationType::SetPrimaryTypeId(const util::uuid& primaryTypeId_)
{
    primaryTypeId = primaryTypeId_;
}

void DIClassTemplateSpecializationType::SetValueTypeId(const util::uuid& valueTypeId_)
{
    valueTypeId = valueTypeId_;
}

DIType* DIClassTemplateSpecializationType::ValueType()
{
    return GetProject()->GetType(valueTypeId);
}

void DIClassTemplateSpecializationType::AddTemplateArgumentTypeId(const util::uuid& templateArgumentTypeId)
{
    templateArgumentTypeIds.push_back(templateArgumentTypeId);
}

void DIClassTemplateSpecializationType::Write(util::BinaryStreamWriter& writer)
{
    DIClassType::Write(writer);
    writer.Write(primaryTypeId);
    int32_t n = templateArgumentTypeIds.size();
    writer.Write(n);
    for (int32_t i = 0; i < n; ++i)
    {
        writer.Write(templateArgumentTypeIds[i]);
    }
    writer.Write(static_cast<int8_t>(containerKind));
    if (containerKind != ContainerClassTemplateKind::notContainerClassTemplate)
    {
        writer.Write(valueTypeId);
    }
}

void DIClassTemplateSpecializationType::Read(util::BinaryStreamReader& reader)
{
    DIClassType::Read(reader);
    reader.ReadUuid(primaryTypeId);
    int32_t n = reader.ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        util::uuid templateArgumentTypeId;
        reader.ReadUuid(templateArgumentTypeId);
        AddTemplateArgumentTypeId(templateArgumentTypeId);
    }
    containerKind = static_cast<ContainerClassTemplateKind>(reader.ReadSByte());
    if (containerKind != ContainerClassTemplateKind::notContainerClassTemplate)
    {
        reader.ReadUuid(valueTypeId);
    }
}

std::unique_ptr<util::JsonValue> DIClassTemplateSpecializationType::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = DIClassType::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"primaryTypeId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(primaryTypeId)))));
        if (containerKind != ContainerClassTemplateKind::notContainerClassTemplate)
        {
            jsonObject->AddField(U"container", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(ContainerName(containerKind)))));
            jsonObject->AddField(U"valueTypeId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(valueTypeId)))));
        }
        util::JsonArray* templateArgumentTypeIdArray = new util::JsonArray();
        for (const util::uuid& templateArgumentTypeId : templateArgumentTypeIds)
        {
            templateArgumentTypeIdArray->AddItem(std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(templateArgumentTypeId)))));
        }
        jsonObject->AddField(U"templateArgumentTypeIds", std::unique_ptr<util::JsonValue>(templateArgumentTypeIdArray));
    }
    return value;
}

DIDelegateType::DIDelegateType() : DIType(Kind::delegateType)
{
}

DIClassDelegateType::DIClassDelegateType() : DIType(Kind::classDelegateType), classTypeId()
{
}

Scope* DIClassDelegateType::GetScope()
{
    return GetClassType()->GetScope();
}

void DIClassDelegateType::SetClassTypeId(const util::uuid& classTypeId_)
{
    classTypeId = classTypeId_;
}

DIType* DIClassDelegateType::GetClassType() const
{
    return GetProject()->GetType(classTypeId);
}

void DIClassDelegateType::Write(util::BinaryStreamWriter& writer)
{
    DIType::Write(writer);
    writer.Write(classTypeId);
}

void DIClassDelegateType::Read(util::BinaryStreamReader& reader)
{
    DIType::Read(reader);
    reader.ReadUuid(classTypeId);
}

std::unique_ptr<util::JsonValue> DIClassDelegateType::ToJson() const
{
    DIType* clsType = GetClassType();
    return clsType->ToJson();
}

DIInterfaceType::DIInterfaceType() : DIType(Kind::interfaceType)
{
}

DIConstType::DIConstType() : DIType(Kind::constType), baseTypeId(util::nil_uuid())
{
}

DIType* DIConstType::BaseType() const
{
    return GetProject()->GetType(baseTypeId);
}

void DIConstType::SetBaseTypeId(const util::uuid& baseTypeId_)
{
    baseTypeId = baseTypeId_;
}

void DIConstType::Write(util::BinaryStreamWriter& writer)
{
    DIType::Write(writer);
    writer.Write(baseTypeId);
}

void DIConstType::Read(util::BinaryStreamReader& reader)
{
    DIType::Read(reader);
    reader.ReadUuid(baseTypeId);
}

std::unique_ptr<util::JsonValue> DIConstType::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = DIType::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"baseTypeId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(baseTypeId)))));
    }
    return value;
}

DIReferenceType::DIReferenceType() : DIType(Kind::referenceType), baseTypeId(util::nil_uuid())
{
}

void DIReferenceType::SetBaseTypeId(const util::uuid& baseTypeId_)
{
    baseTypeId = baseTypeId_;
}

DIType* DIReferenceType::BaseType() const
{
    return GetProject()->GetType(baseTypeId);
}

void DIReferenceType::Write(util::BinaryStreamWriter& writer)
{
    DIType::Write(writer);
    writer.Write(baseTypeId);
}

void DIReferenceType::Read(util::BinaryStreamReader& reader)
{
    DIType::Read(reader);
    reader.ReadUuid(baseTypeId);
}

std::unique_ptr<util::JsonValue> DIReferenceType::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = DIType::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"baseTypeId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(baseTypeId)))));
    }
    return value;
}

DIPointerType::DIPointerType() : DIType(Kind::pointerType), pointedTypeId(util::nil_uuid())
{
}

void DIPointerType::SetPointedTypeId(const util::uuid& pointedTypeId_)
{
    pointedTypeId = pointedTypeId_;
}

DIType* DIPointerType::PointedToType() const
{
    return GetProject()->GetType(pointedTypeId);
}

void DIPointerType::Write(util::BinaryStreamWriter& writer)
{
    DIType::Write(writer);
    writer.Write(pointedTypeId);
}

void DIPointerType::Read(util::BinaryStreamReader& reader)
{
    DIType::Read(reader);
    reader.ReadUuid(pointedTypeId);
}

std::unique_ptr<util::JsonValue> DIPointerType::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = DIType::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"pointedTypeId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(pointedTypeId)))));
    }
    return value;
}

DIArrayType::DIArrayType() : DIType(Kind::arrayType), elementTypeId(util::nil_uuid()), size(-1)
{
}

void DIArrayType::SetElementTypeId(const util::uuid& elementTypeId_)
{
    elementTypeId = elementTypeId_;
}

DIType* DIArrayType::ElementType() const
{
    return GetProject()->GetType(elementTypeId);
}

void DIArrayType::Write(util::BinaryStreamWriter& writer)
{
    DIType::Write(writer);
    writer.Write(elementTypeId);
    writer.Write(size);
}

void DIArrayType::Read(util::BinaryStreamReader& reader)
{
    DIType::Read(reader);
    reader.ReadUuid(elementTypeId);
    size = reader.ReadLong();
}

std::unique_ptr<util::JsonValue> DIArrayType::ToJson() const
{
    std::unique_ptr<util::JsonValue> value = DIType::ToJson();
    if (value->Type() == util::JsonValueType::object)
    {
        util::JsonObject* jsonObject = static_cast<util::JsonObject*>(value.get());
        jsonObject->AddField(U"elementTypeId", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(util::ToString(elementTypeId)))));
        jsonObject->AddField(U"size", std::unique_ptr<util::JsonValue>(new util::JsonString(util::ToUtf32(std::to_string(size)))));
    }
    return value;
}

DIType* MakePointerType(DIType* pointedToType)
{
    Project* project = pointedToType->GetProject();
    return project->GetPointerType(pointedToType);
}

DIType* MakeReferenceType(DIType* referredToType)
{
    Project* project = referredToType->GetProject();
    return project->GetReferenceType(referredToType);
}

DIType* MakeConstType(DIType* baseType)
{
    Project* project = baseType->GetProject();
    return project->GetConstType(baseType);
}

void WriteType(util::BinaryStreamWriter& writer, DIType* type)
{
    writer.Write(static_cast<int8_t>(type->GetKind()));
    type->Write(writer);
}

std::unique_ptr<DIType> ReadType(util::BinaryStreamReader& reader, Project* project)
{
    std::unique_ptr<DIType> type;
    int8_t k = reader.ReadSByte();
    DIType::Kind kind = static_cast<DIType::Kind>(k);
    switch (kind)
    {
    case DIType::Kind::primitiveType: type.reset(new DIPrimitiveType()); break;
    case DIType::Kind::enumType: type.reset(new DIEnumType()); break;
    case DIType::Kind::classType: type.reset(new DIClassType()); break;
    case DIType::Kind::specializationType: type.reset(new DIClassTemplateSpecializationType()); break;
    case DIType::Kind::delegateType: type.reset(new DIDelegateType()); break;
    case DIType::Kind::classDelegateType: type.reset(new DIClassDelegateType()); break;
    case DIType::Kind::interfaceType: type.reset(new DIInterfaceType()); break;
    case DIType::Kind::templateParameter: type.reset(new DITemplateParameter()); break;
    case DIType::Kind::constType: type.reset(new DIConstType()); break;
    case DIType::Kind::referenceType: type.reset(new DIReferenceType()); break;
    case DIType::Kind::pointerType: type.reset(new DIPointerType()); break;
    case DIType::Kind::arrayType: type.reset(new DIArrayType()); break;
    default: throw std::runtime_error("unknown DIType kind '" + std::to_string(k));
    }
    type->SetProject(project);
    type->Read(reader);
    return type;
}

} // namespace cmajor::debug
