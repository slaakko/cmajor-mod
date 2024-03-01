// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debug.di.type;

import soul.xml.dom;
import cmajor.debug.container;
import cmajor.debug.di.enum_constant;
import cmajor.debug.di.variable;
import util;
import std.core;

export namespace cmajor::debug {

class DebugInfo;

class Scope
{
public:
    virtual ~Scope();
    virtual DIVariable* GetVariable(const std::string& name) const = 0;
    virtual std::string Name() const = 0;
    virtual int LocalVariableCount() const = 0;
    virtual bool IsFunctionScope() const { return false; }
};

class DIType
{
public:
    enum class Kind : int8_t
    {
        primitiveType,
        enumType,
        classType,
        specializationType,
        delegateType,
        classDelegateType,
        interfaceType,
        templateParameter,
        constType,
        referenceType,
        pointerType,
        arrayType
    };
    DIType(Kind kind_);
    virtual ~DIType();
    virtual void Write(util::BinaryStreamWriter& writer);
    virtual void Read(util::BinaryStreamReader& reader);
    Kind GetKind() const { return kind; }
    const util::uuid& Id() const { return id; }
    void SetId(const util::uuid& id_);
    const std::string& Name() const { return name; }
    void SetName(const std::string& name_);
    const std::string& IrName() const { return irName; }
    void SetIrName(const std::string& irName_);
    static std::string KindStr(Kind kind);
    virtual Scope* GetScope();
    Project* GetProject() const { return project; }
    void SetProject(Project* project_) { project = project_; }
    virtual std::unique_ptr<util::JsonValue> ToJson() const;
    virtual DIType* DerefType() { return this; }
    virtual std::unique_ptr<soul::xml::Element> ToXml() const;
private:
    Kind kind;
    util::uuid id;
    std::string name;
    std::string irName;
    Project* project;
};

class DITypeRef
{
public:
    DITypeRef(DIType* type_);
    std::unique_ptr<util::JsonValue> ToJson();
    std::unique_ptr<soul::xml::Element> ToXml(const std::string& elementName) const;
private:
    DIType* type;
};

class DIPrimitiveType : public DIType
{
public:
    enum class Kind : int8_t
    {
        none, boolType, sbyteType, byteType, shortType, ushortType, intType, uintType, longType, ulongType, floatType, doubleType, charType, wcharType, ucharType, voidType
    };
    DIPrimitiveType();
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    void SetPrimitiveTypeKind(Kind kind_) { kind = kind_; }
    Kind GetPrimitiveTypeKind() const { return kind; }
    static std::string PrimitiveTypeKindStr(Kind kind);
    std::unique_ptr<util::JsonValue> ToJson() const override;
    std::unique_ptr<soul::xml::Element> ToXml() const override;
    bool IsIntegerType() const;
private:
    Kind kind;
};

class DIEnumType : public DIType
{
public:
    DIEnumType();
    void SetUnderlyingTypeId(const util::uuid& underlyingTypeId_);
    const util::uuid& UnderlyingTypeId() const { return underlyingTypeId; }
    const std::vector<DIEnumConstant>& EnumConstants() const { return enumConstants; }
    void AddEnumConstant(DIEnumConstant&& enumConstant);
    DIEnumConstant* GetEnumConstant(uint64_t& value);
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    std::unique_ptr<util::JsonValue> ToJson() const override;
    std::unique_ptr<soul::xml::Element> ToXml() const override;
private:
    util::uuid underlyingTypeId;
    std::vector<DIEnumConstant> enumConstants;
};

class DITemplateParameter : public DIType
{
public:
    DITemplateParameter();
};

class DIClassType;

class ClassScope : public Scope
{
public:
    ClassScope(DIClassType* classType_);
    ClassScope(const ClassScope&) = delete;
    ClassScope(ClassScope&&) = delete;
    ClassScope& operator=(const ClassScope&) = delete;
    ClassScope& operator=(ClassScope&&) = delete;
    std::string Name() const override;
    void AddMemberVariable(DIVariable* memberVariable);
    DIVariable* GetVariable(const std::string& name) const override;
    int LocalVariableCount() const override { return 0; }
private:
    DIClassType* classType;
    std::unordered_map<std::string, DIVariable*> memberVariableMap;
};

class DIClassType : public DIType
{
public:
    DIClassType();
    DIClassType(Kind kind);
    DIClassType(const DIClassType&) = delete;
    DIClassType(DIClassType&&) = delete;
    DIClassType& operator=(const DIClassType&) = delete;
    DIClassType& operator=(DIClassType&&) = delete;
    DIType* BaseClassType() const;
    void SetBaseClassId(const util::uuid& baseClassId_);
    const util::uuid& BaseClassId() const { return baseClassId; }
    void AddTemplateParameter(DITemplateParameter* templateParameter);
    const std::vector<std::unique_ptr<DITemplateParameter>>& TemplateParameters() const { return templateParameters; }
    void AddMemberVariable(DIVariable* memberVariable);
    const std::vector<std::unique_ptr<DIVariable>>& MemberVariables() const { return memberVariables; }
    void SetPolymorphic() { polymorphic = true; }
    bool IsPolymorphic() const { return polymorphic; }
    void SetVmtPtrIndex(int32_t vmtPtrIndex_) { vmtPtrIndex = vmtPtrIndex_; }
    int32_t VmtPtrIndex() const { return vmtPtrIndex; }
    void SetVmtVariableName(const std::string& vmtVariableName_);
    const std::string& VmtVariableName() const { return vmtVariableName; }
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    std::unique_ptr<util::JsonValue> ToJson() const override;
    std::unique_ptr<soul::xml::Element> ToXml() const override;
    Scope* GetScope() override { return &scope; }
private:
    ClassScope scope;
    util::uuid baseClassId;
    std::vector<std::unique_ptr<DITemplateParameter>> templateParameters;
    std::vector<std::unique_ptr<DIVariable>> memberVariables;
    bool polymorphic;
    int32_t vmtPtrIndex;
    std::string vmtVariableName;
};

int NumBaseClasses(DIClassType* classType);

class DIClassTemplateSpecializationType : public DIClassType
{
public:
    DIClassTemplateSpecializationType();
    void SetPrimaryTypeId(const util::uuid& primaryTypeId_);
    const util::uuid& PrimaryTypeId() const { return primaryTypeId; }
    void SetContainerClassTemplateKind(ContainerClassTemplateKind containerKind_) { containerKind = containerKind_; }
    ContainerClassTemplateKind GetContainerClassTemplateKind() const { return containerKind; }
    const util::uuid& ValueTypeId() const { return valueTypeId; }
    void SetValueTypeId(const util::uuid& valueTypeId_);
    DIType* ValueType();
    void AddTemplateArgumentTypeId(const util::uuid& templateArgumentTypeId);
    const std::vector< util::uuid>& TemplateArgumentTypeIds() const { return templateArgumentTypeIds; }
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    std::unique_ptr<util::JsonValue> ToJson() const override;
    std::unique_ptr<soul::xml::Element> ToXml() const override;
private:
    ContainerClassTemplateKind containerKind;
    util::uuid primaryTypeId;
    util::uuid valueTypeId;
    std::vector<util::uuid> templateArgumentTypeIds;
};

class DIDelegateType : public DIType
{
public:
    DIDelegateType();
};

class DIClassDelegateType : public DIType
{
public:
    DIClassDelegateType();
    void SetClassTypeId(const util::uuid& classTypeId_);
    DIType* GetClassType() const;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    std::unique_ptr<util::JsonValue> ToJson() const override;
    std::unique_ptr<soul::xml::Element> ToXml() const override;
    Scope* GetScope() override;
private:
    util::uuid classTypeId;
};

class DIInterfaceType : public DIType
{
public:
    DIInterfaceType();
};

class DIConstType : public DIType
{
public:
    DIConstType();
    void SetBaseTypeId(const util::uuid& baseTypeId_);
    const util::uuid& BaseTypeId() const { return baseTypeId; }
    DIType* BaseType() const;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    std::unique_ptr<util::JsonValue> ToJson() const override;
    std::unique_ptr<soul::xml::Element> ToXml() const override;
    DIType* DerefType() override { return BaseType()->DerefType(); }
private:
    util::uuid baseTypeId;
};

class DIReferenceType : public DIType
{
public:
    DIReferenceType();
    void SetBaseTypeId(const util::uuid& baseTypeId_);
    const util::uuid& BaseTypeId() const { return baseTypeId; }
    DIType* BaseType() const;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    std::unique_ptr<util::JsonValue> ToJson() const override;
    std::unique_ptr<soul::xml::Element> ToXml() const override;
    DIType* DerefType() override { return BaseType(); }
private:
    util::uuid baseTypeId;
};

class DIPointerType : public DIType
{
public:
    DIPointerType();
    void SetPointedTypeId(const util::uuid& pointedTypeId_);
    const util::uuid& PointedTypeId() const { return pointedTypeId; }
    DIType* PointedToType() const;
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    std::unique_ptr<util::JsonValue> ToJson() const override;
    std::unique_ptr<soul::xml::Element> ToXml() const override;
    DIType* DerefType() override { return PointedToType(); }
private:
    util::uuid pointedTypeId;
};

class DIArrayType : public DIType
{
public:
    DIArrayType();
    void SetElementTypeId(const util::uuid& elementTypeId_);
    const util::uuid& ElementTypeId() const { return elementTypeId; }
    DIType* ElementType() const;
    int64_t Size() const { return size; }
    void SetSize(int64_t size_) { size = size_; }
    void Write(util::BinaryStreamWriter& writer) override;
    void Read(util::BinaryStreamReader& reader) override;
    std::unique_ptr<util::JsonValue> ToJson() const override;
    std::unique_ptr<soul::xml::Element> ToXml() const override;
private:
    util::uuid elementTypeId;
    int64_t size;
};

DIType* MakePointerType(DIType* pointedToType);
DIType* MakeReferenceType(DIType* referredToType);
DIType* MakeConstType(DIType* baseType);

void WriteType(util::BinaryStreamWriter& writer, DIType* type);
std::unique_ptr<DIType> ReadType(util::BinaryStreamReader& reader, Project* project);

} // namespace cmajor::debug
