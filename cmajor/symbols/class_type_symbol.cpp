// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>
#include <boost/multiprecision/integer.hpp>

module cmajor.symbols.classes;

import soul.ast.span;
import cmajor.symbols.global.flags;
import cmajor.ir.emitter;
import cmajor.symbols.exception;
import cmajor.symbols.function.symbol;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.modules;
import cmajor.symbols.class_template_specializations;
import cmajor.symbols.interfaces;
import cmajor.symbols.templates;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.symbol.collector;
import cmajor.symbols.concepts;
import cmajor.symbols.value;
import cmajor.ast.basic.type;
import cmajor.ast.node;
import cmajor.ast.classes;
import cmajor.ast.compile.unit;
import cmajor.ast.concepts;
import cmajor.ast.specifier;
import cmajor.ast.constant;
import cmajor.ast.literal;
import cmajor.ast.type.expr;
import util;
import std.core;

namespace cmajor::symbols {

// CreateDIType

ClassTypeFlagMap::ClassTypeFlagMap()
{
}

void ClassTypeFlagMap::Import(ClassTypeFlagMap& that)
{
    for (const auto& clsFlags : that.flagMap)
    {
        SetFlag(clsFlags.first, clsFlags.second);
    }
}

void ClassTypeFlagMap::Write(SymbolWriter& writer)
{
    int32_t n = flagMap.size();
    writer.GetBinaryStreamWriter().Write(n);
    for (const auto& clsFlags : flagMap)
    {
        writer.GetBinaryStreamWriter().Write(clsFlags.first);
        writer.GetBinaryStreamWriter().Write(static_cast<uint16_t>(clsFlags.second));
    }
}

void ClassTypeFlagMap::Read(SymbolReader& reader)
{
    int32_t n = reader.GetBinaryStreamReader().ReadInt();
    for (int32_t i = 0; i < n; ++i)
    {
        std::u32string className = reader.GetBinaryStreamReader().ReadUtf32String();
        uint16_t flags = reader.GetBinaryStreamReader().ReadUShort();
        flagMap[className] = static_cast<ClassTypeSymbolFlags>(flags);
    }
}

bool ClassTypeFlagMap::GetFlag(const std::u32string& classTypeMangledName, ClassTypeSymbolFlags flag) 
{
    ClassTypeSymbolFlags& flags = flagMap[classTypeMangledName];
    return (flags & flag) != ClassTypeSymbolFlags::none;
}

void ClassTypeFlagMap::SetFlag(const std::u32string& classTypeMangledName, ClassTypeSymbolFlags flag)
{
    ClassTypeSymbolFlags& flags = flagMap[classTypeMangledName];
    flags = flags | flag;
}

void ClassTypeFlagMap::ResetFlag(const std::u32string& classTypeMangledName, ClassTypeSymbolFlags flag)
{
    ClassTypeSymbolFlags& flags = flagMap[classTypeMangledName];
    flags = flags & ~flag;
}

int32_t GetClassIdVmtIndexOffset()
{
    if (GetBackEnd() == BackEnd::systemx)
    {
        return 0;
    }
    else if (GetBackEnd() == BackEnd::masm || GetBackEnd() == BackEnd::cpp || GetBackEnd() == BackEnd::llvm)
    {
        Assert(false, "MASM, LLVM and C++ backends do not support class ids");
        return -1;
    }
    else
    {
        Assert(false, "unknown backend");
        return -1;
    }
}

int32_t GetTypeIdVmtIndexOffset()
{
    if (GetBackEnd() == BackEnd::systemx)
    {
        return 2;
    }
    else if (GetBackEnd() == BackEnd::masm || GetBackEnd() == BackEnd::cpp || GetBackEnd() == BackEnd::llvm)
    {
        return 0;
    }
    else
    {
        Assert(false, "unknown backend");
        return -1;
    }
}

int32_t GetClassNameVmtIndexOffset()
{
    if (false) // cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm
    {
        return 4;
    }
    else if (GetBackEnd() == BackEnd::systemx)
    {
        return 2;
    }
    else if (GetBackEnd() == BackEnd::masm || GetBackEnd() == BackEnd::cpp || GetBackEnd() == BackEnd::llvm)
    {
        Assert(false, "MASM and C++ backends do not support in-class typename");
        return -1;
    }
    else
    {
        Assert(false, "unknown backend");
        return -1;
    }
}

int32_t GetImtsVmtIndexOffset()
{
    if (false) // cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm
    {
        return 5;
    }
    else if (GetBackEnd() == BackEnd::systemx)
    {
        return 3;
    }
    else if (GetBackEnd() == BackEnd::masm || GetBackEnd() == BackEnd::cpp || GetBackEnd() == BackEnd::llvm)
    {
        return 2;
    }
    else
    {
        Assert(false, "unknown backend");
        return -1;
    }
}

int32_t GetFunctionVmtIndexOffset()
{
    if (false) // cmajor::symbols::GetBackEnd() == cmajor::symbols::BackEnd::llvm
    {
        return 6;
    }
    else if (GetBackEnd() == BackEnd::systemx)
    {
        return 4;
    }
    else if (GetBackEnd() == BackEnd::masm || GetBackEnd() == BackEnd::cpp || GetBackEnd() == BackEnd::llvm)
    {
        return 3;
    }
    else
    {
        Assert(false, "unknown backend");
        return -1;
    }
}

ClassGroupTypeSymbol::ClassGroupTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) : 
    TypeSymbol(SymbolType::classGroupTypeSymbol, span_, name_)
{
}

void* ClassGroupTypeSymbol::IrType(cmajor::ir::Emitter& emitter)
{
    Assert(false, "tried to get ir type of class group");
    return nullptr;
}

void* ClassGroupTypeSymbol::CreateDefaultIrValue(cmajor::ir::Emitter& emitter)
{
    Assert(false, "tried to create default ir value of class group");
    return nullptr;
}

void ClassGroupTypeSymbol::AddClass(ClassTypeSymbol* classTypeSymbol)
{
    for (int arity = classTypeSymbol->MinArity(); arity <= classTypeSymbol->MaxArity(); ++arity)
    {
        if (arityClassMap.find(arity) != arityClassMap.cend())
        {
            throw Exception("already has class with arity " + std::to_string(arity) + " in class group '" + util::ToUtf8(Name()) + "'", GetFullSpan(), classTypeSymbol->GetFullSpan());
        }
        arityClassMap[arity] = classTypeSymbol;
        classTypeSymbol->SetClassGroup(this);
    }
}

void ClassGroupTypeSymbol::RemoveClass(ClassTypeSymbol* classTypeSymbol)
{
    for (int arity = classTypeSymbol->MinArity(); arity <= classTypeSymbol->MaxArity(); ++arity)
    {
        arityClassMap.erase(arity);
    }
}

const ContainerScope* ClassGroupTypeSymbol::GetTypeScope() const
{
    if (arityClassMap.size() == 1)
    {
        ClassTypeSymbol* cls = arityClassMap.begin()->second;
        return cls->GetTypeScope();
    }
    return nullptr;
}

ContainerScope* ClassGroupTypeSymbol::GetTypeScope()
{
    if (arityClassMap.size() == 1)
    {
        ClassTypeSymbol* cls = arityClassMap.begin()->second;
        return cls->GetTypeScope();
    }
    return nullptr;
}

bool ClassGroupTypeSymbol::IsEmpty() const
{
    return arityClassMap.empty();
}

ClassTypeSymbol* ClassGroupTypeSymbol::GetClass(int arity) const
{
    auto it = arityClassMap.find(arity);
    if (it != arityClassMap.cend())
    {
        ClassTypeSymbol* classTypeSymbol = it->second;
        return classTypeSymbol;
    }
    return nullptr;
}

bool ClassGroupTypeSymbol::HasProjectMembers() const
{
    for (const auto& p : arityClassMap)
    {
        ClassTypeSymbol* cls = p.second;
        if (cls->IsProject())
        {
            return true;
        }
    }
    return false;
}

void ClassGroupTypeSymbol::AppendChildElements(soul::xml::Element* element, TypeMap& typeMap) const
{
    for (const auto& p : arityClassMap)
    {
        ClassTypeSymbol* cls = p.second;
        std::unique_ptr<soul::xml::Element> classElement = cls->ToDomElement(typeMap);
        if (cls->IsProject())
        {
            element->AppendChild(classElement.release());
        }
    }
}

void ClassGroupTypeSymbol::Check()
{
    TypeSymbol::Check();
    for (const auto& p : arityClassMap)
    {
        if (!p.second)
        {
            throw SymbolCheckException("class group type symbol has no class type symbol", GetFullSpan());
        }
    }
}

std::string ClassGroupTypeSymbol::GetSymbolHelp() const
{
    if (arityClassMap.size() == 1)
    {
        ClassTypeSymbol* cls = arityClassMap.begin()->second;
        return cls->GetSymbolHelp();
    }
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(FullName())).append(" (").append(std::to_string(arityClassMap.size())).append(" classes)");
    return help;
}

bool ClassGroupTypeSymbol::IsValidCCClassGroup(Module* module, FunctionSymbol* fromFunction) const
{
    if (util::StartsWith(Name(), U"@")) return false;
    AccessCheckFunction canAccess = GetAccessCheckFunction();
    for (const auto& p : arityClassMap)
    {
        ClassTypeSymbol* cls = p.second;
        if (cls->Access() == SymbolAccess::public_) return true;
        if (cls->Access() == SymbolAccess::internal_ && module == cls->GetModule()) return true;
        if (fromFunction && canAccess(fromFunction, cls)) return true;
    }
    return false;
}

std::vector<Symbol*> ClassGroupTypeSymbol::GetParamHelpSymbols() const
{
    std::vector<Symbol*> paramHelpSymbols;
    for (const auto& p : arityClassMap)
    {
        ClassTypeSymbol* cls = p.second;
        paramHelpSymbols.push_back(cls);
    }
    return paramHelpSymbols;
}

ClassTypeSymbol::ClassTypeSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    TypeSymbol(SymbolType::classTypeSymbol, span_, name_),
    minArity(0), baseClass(), flags(ClassTypeSymbolFlags::none), implementedInterfaces(), templateParameters(), memberVariables(), staticMemberVariables(),
    staticConstructor(nullptr), defaultConstructor(nullptr), copyConstructor(nullptr), moveConstructor(nullptr), copyAssignment(nullptr), moveAssignment(nullptr),
    constructors(), destructor(nullptr), memberFunctions(), vmtPtrIndex(-1), prototype(nullptr), classGroup(nullptr)
{
}

ClassTypeSymbol::ClassTypeSymbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_) :
    TypeSymbol(symbolType_, span_, name_),
    minArity(0), baseClass(), flags(ClassTypeSymbolFlags::none), implementedInterfaces(), templateParameters(), memberVariables(), staticMemberVariables(),
    staticConstructor(nullptr), defaultConstructor(nullptr), copyConstructor(nullptr), moveConstructor(nullptr), copyAssignment(nullptr), moveAssignment(nullptr),
    constructors(), destructor(nullptr), memberFunctions(), vmtPtrIndex(-1), prototype(nullptr), classGroup(nullptr)
{
}

bool ClassTypeSymbol::GetFlag(ClassTypeSymbolFlags flag) 
{
    bool specialization = GetSymbolType() == SymbolType::classTemplateSpecializationSymbol;
    if (specialization && flag == ClassTypeSymbolFlags::vmtEmitted)
    {
        return GetClassTypeFlagMap().GetFlag(MangledName(), flag);
    }
    else
    {
        return (flags & flag) != ClassTypeSymbolFlags::none;
    }
}

void ClassTypeSymbol::SetFlag(ClassTypeSymbolFlags flag)
{
    bool specialization = GetSymbolType() == SymbolType::classTemplateSpecializationSymbol;
    if (specialization && flag == ClassTypeSymbolFlags::vmtEmitted)
    {
        GetClassTypeFlagMap().SetFlag(MangledName(), flag);
    }
    else
    {
        flags = flags | flag;
    }
}

void ClassTypeSymbol::ResetFlag(ClassTypeSymbolFlags flag)
{
    bool specialization = GetSymbolType() == SymbolType::classTemplateSpecializationSymbol;
    if (specialization && flag == ClassTypeSymbolFlags::vmtEmitted)
    {
        GetClassTypeFlagMap().ResetFlag(MangledName(), flag);
    }
    else
    {
        flags = flags & ~flag;
    }
}

void ClassTypeSymbol::Write(SymbolWriter& writer)
{
    TypeSymbol::Write(writer);
    writer.GetBinaryStreamWriter().Write(groupName);
    writer.GetBinaryStreamWriter().Write(static_cast<uint16_t>(flags));
    writer.GetBinaryStreamWriter().Write(static_cast<int32_t>(minArity));
    if (IsClassTemplate())
    {
        usingNodes.Write(writer.GetAstWriter());
        cmajor::ast::Node* node = GetRootModuleForCurrentThread()->GetSymbolTable().GetNode(this);
        writer.GetAstWriter().Write(node);
        bool hasPrototype = prototype != nullptr;
        writer.GetBinaryStreamWriter().Write(hasPrototype);
        if (hasPrototype)
        {
            writer.GetBinaryStreamWriter().Write(prototype->TypeId());
        }
    }
    util::uuid baseClassId = util::nil_uuid();
    if (baseClass)
    {
        baseClassId = baseClass->TypeId();
    }
    writer.GetBinaryStreamWriter().Write(baseClassId);
    uint32_t n = uint32_t(implementedInterfaces.size());
    writer.GetBinaryStreamWriter().WriteULEB128UInt(n);
    for (uint32_t i = 0; i < n; ++i)
    {
        InterfaceTypeSymbol* intf = implementedInterfaces[i];
        const util::uuid& intfTypeId = intf->TypeId();
        writer.GetBinaryStreamWriter().Write(intfTypeId);
    }
    bool hasDefaultConstructor = defaultConstructor != nullptr && !defaultConstructor->IsGeneratedFunction();
    writer.GetBinaryStreamWriter().Write(hasDefaultConstructor);
    if (hasDefaultConstructor)
    {
        writer.GetBinaryStreamWriter().Write(defaultConstructor->FunctionId());
    }
    bool hasCopyConstructor = copyConstructor != nullptr && !copyConstructor->IsGeneratedFunction();
    writer.GetBinaryStreamWriter().Write(hasCopyConstructor);
    if (hasCopyConstructor)
    {
        writer.GetBinaryStreamWriter().Write(copyConstructor->FunctionId());
    }
    bool hasMoveConstructor = moveConstructor != nullptr && !moveConstructor->IsGeneratedFunction();
    writer.GetBinaryStreamWriter().Write(hasMoveConstructor);
    if (hasMoveConstructor)
    {
        writer.GetBinaryStreamWriter().Write(moveConstructor->FunctionId());
    }
    bool hasCopyAssignment = copyAssignment != nullptr && !copyAssignment->IsGeneratedFunction();
    writer.GetBinaryStreamWriter().Write(hasCopyAssignment);
    if (hasCopyAssignment)
    {
        writer.GetBinaryStreamWriter().Write(copyAssignment->FunctionId());
    }
    bool hasMoveAssignment = moveAssignment != nullptr && !moveAssignment->IsGeneratedFunction();
    writer.GetBinaryStreamWriter().Write(hasMoveAssignment);
    if (hasMoveAssignment)
    {
        writer.GetBinaryStreamWriter().Write(moveAssignment->FunctionId());
    }
    uint32_t vmtSize = vmt.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(vmtSize);
    writer.GetBinaryStreamWriter().Write(vmtPtrIndex);
    bool hasConstraint = constraint != nullptr;
    writer.GetBinaryStreamWriter().Write(hasConstraint);
    if (hasConstraint)
    {
        writer.GetAstWriter().Write(constraint.get());
    }
    uint32_t oln = objectLayout.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(oln);
    for (TypeSymbol* type : objectLayout)
    {
        writer.GetBinaryStreamWriter().Write(type->TypeId());
    }
    uint32_t sln = staticLayout.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(sln);
    for (TypeSymbol* type : staticLayout)
    {
        writer.GetBinaryStreamWriter().Write(type->TypeId());
    }
}

const int prototypeIndex = 1000000;
const int defaultConstructorIndex = 1000001;
const int copyConstructorIndex = 1000002;
const int moveConstructorIndex = 1000003;
const int copyAssignmentIndex = 1000004;
const int moveAssignmentIndex = 1000005;
const int objectLayoutIndex = 0x10000;
const int staticLayoutIndex = 0x20000;
const int maxObjectLayoutSize = staticLayoutIndex - objectLayoutIndex;
const int maxStaticLayoutSize = maxObjectLayoutSize;

void ClassTypeSymbol::Read(SymbolReader& reader)
{
    TypeSymbol::Read(reader);
    groupName = reader.GetBinaryStreamReader().ReadUtf32String();
    flags = static_cast<ClassTypeSymbolFlags>(reader.GetBinaryStreamReader().ReadUShort());
    minArity = reader.GetBinaryStreamReader().ReadInt();
    if (IsClassTemplate())
    {
        usingNodes.Read(reader.GetAstReader());
        cmajor::ast::Node* node = reader.GetAstReader().ReadNode();
        Assert(node->GetNodeType() == cmajor::ast::NodeType::classNode, "class node expected");
        cmajor::ast::ClassNode* clsNode = static_cast<cmajor::ast::ClassNode*>(node);
        classNode.reset(clsNode);
        bool hasPrototype = reader.GetBinaryStreamReader().ReadBool();
        if (hasPrototype)
        {
            util::uuid prototypeId;
            reader.GetBinaryStreamReader().ReadUuid(prototypeId);
            reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, prototypeId, prototypeIndex);
        }
    }
    util::uuid baseClassId;
    reader.GetBinaryStreamReader().ReadUuid(baseClassId);
    if (!baseClassId.is_nil())
    {
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, baseClassId, 0);
    }
    uint32_t n = reader.GetBinaryStreamReader().ReadULEB128UInt();
    implementedInterfaces.resize(n);
    for (uint32_t i = 0; i < n; ++i)
    {
        util::uuid intfTypeId;
        reader.GetBinaryStreamReader().ReadUuid(intfTypeId);
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, intfTypeId, 1 + i);
    }
    bool hasDefaultConstructor = reader.GetBinaryStreamReader().ReadBool();
    if (hasDefaultConstructor)
    {
        util::uuid defaultConstructorId;
        reader.GetBinaryStreamReader().ReadUuid(defaultConstructorId);
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, defaultConstructorId, defaultConstructorIndex);
    }
    bool hasCopyConstructor = reader.GetBinaryStreamReader().ReadBool();
    if (hasCopyConstructor)
    {
        util::uuid copyConstructorId;
        reader.GetBinaryStreamReader().ReadUuid(copyConstructorId);
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, copyConstructorId, copyConstructorIndex);
    }
    bool hasMoveConstructor = reader.GetBinaryStreamReader().ReadBool();
    if (hasMoveConstructor)
    {
        util::uuid moveConstructorId;
        reader.GetBinaryStreamReader().ReadUuid(moveConstructorId);
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, moveConstructorId, moveConstructorIndex);
    }
    bool hasCopyAssignment = reader.GetBinaryStreamReader().ReadBool();
    if (hasCopyAssignment)
    {
        util::uuid copyAssignmentId;
        reader.GetBinaryStreamReader().ReadUuid(copyAssignmentId);
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, copyAssignmentId, copyAssignmentIndex);
    }
    bool hasMoveAssignment = reader.GetBinaryStreamReader().ReadBool();
    if (hasMoveAssignment)
    {
        util::uuid moveAssignmentId;
        reader.GetBinaryStreamReader().ReadUuid(moveAssignmentId);
        reader.GetSymbolTable()->EmplaceFunctionRequest(reader, this, moveAssignmentId, moveAssignmentIndex);
    }
    uint32_t vmtSize = reader.GetBinaryStreamReader().ReadULEB128UInt();
    vmt.resize(vmtSize);
    if (destructor)
    {
        if (destructor->VmtIndex() != -1)
        {
            Assert(destructor->VmtIndex() < vmt.size(), "invalid destructor vmt index");
            vmt[destructor->VmtIndex()] = destructor;
        }
    }
    for (FunctionSymbol* memberFunction : memberFunctions)
    {
        if (memberFunction->VmtIndex() != -1)
        {
            Assert(memberFunction->VmtIndex() < vmt.size(), "invalid member function vmt index");
            vmt[memberFunction->VmtIndex()] = memberFunction;
        }
    }
    vmtPtrIndex = reader.GetBinaryStreamReader().ReadInt();
    bool hasConstraint = reader.GetBinaryStreamReader().ReadBool();
    if (hasConstraint)
    {
        constraint.reset(reader.GetAstReader().ReadConstraintNode());
    }
    uint32_t oln = reader.GetBinaryStreamReader().ReadULEB128UInt();
    objectLayout.resize(oln);
    for (uint32_t i = 0; i < oln; ++i)
    {
        util::uuid typeId;
        reader.GetBinaryStreamReader().ReadUuid(typeId);
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, objectLayoutIndex + i);
    }
    uint32_t sln = reader.GetBinaryStreamReader().ReadULEB128UInt();
    staticLayout.resize(sln);
    for (uint32_t i = 0; i < sln; ++i)
    {
        util::uuid typeId;
        reader.GetBinaryStreamReader().ReadUuid(typeId);
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeId, staticLayoutIndex + i);
    }
    if (IsPolymorphic() && !IsPrototypeTemplateSpecialization())
    {
        reader.GetSymbolTable()->AddPolymorphicClass(this);
    }
    if (StaticConstructor())
    {
        reader.GetSymbolTable()->AddClassHavingStaticConstructor(this);
    }
}

void ClassTypeSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index == 0)
    {
        Assert(typeSymbol->GetSymbolType() == SymbolType::classTypeSymbol || typeSymbol->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol, "class type symbol expected");
        baseClass = static_cast<ClassTypeSymbol*>(typeSymbol);
    }
    else if (index >= 1)
    {
        if (index == prototypeIndex)
        {
            Assert(typeSymbol->GetSymbolType() == SymbolType::classTemplateSpecializationSymbol, "class template specialization expected");
            SetPrototype(static_cast<ClassTemplateSpecializationSymbol*>(typeSymbol));
        }
        else if (index < objectLayoutIndex)
        {
            Assert(typeSymbol->GetSymbolType() == SymbolType::interfaceTypeSymbol, "interface type symbol expected");
            InterfaceTypeSymbol* interfaceTypeSymbol = static_cast<InterfaceTypeSymbol*>(typeSymbol);
            implementedInterfaces[index - 1] = interfaceTypeSymbol;
        }
        else if (index >= objectLayoutIndex && index < objectLayoutIndex + maxObjectLayoutSize)
        {
            objectLayout[index - objectLayoutIndex] = typeSymbol;
        }
        else if (index >= staticLayoutIndex && index < staticLayoutIndex + maxStaticLayoutSize)
        {
            staticLayout[index - staticLayoutIndex] = typeSymbol;
        }
        else
        {
            throw std::runtime_error("internal error: invalid class type emplace type index");
        }
    }
}

void ClassTypeSymbol::EmplaceFunction(FunctionSymbol* functionSymbol, int index)
{
    switch (index)
    {
    case defaultConstructorIndex: defaultConstructor = static_cast<ConstructorSymbol*>(functionSymbol); break;
    case copyConstructorIndex: copyConstructor = static_cast<ConstructorSymbol*>(functionSymbol); break;
    case moveConstructorIndex: moveConstructor = static_cast<ConstructorSymbol*>(functionSymbol); break;
    case copyAssignmentIndex: copyAssignment = static_cast<MemberFunctionSymbol*>(functionSymbol); break;
    case moveAssignmentIndex: moveAssignment = static_cast<MemberFunctionSymbol*>(functionSymbol); break;
    default:
    {
        throw std::runtime_error("internal error: invalid class type emplace function index");
    }
    }
}

void ClassTypeSymbol::AddMember(Symbol* member)
{
    TypeSymbol::AddMember(member);
    switch (member->GetSymbolType())
    {
    case SymbolType::templateParameterSymbol:
    {
        templateParameters.push_back(static_cast<TemplateParameterSymbol*>(member));
        break;
    }
    case SymbolType::memberVariableSymbol:
    {
        if (member->IsStatic())
        {
            staticMemberVariables.push_back(static_cast<MemberVariableSymbol*>(member));
        }
        else
        {
            memberVariables.push_back(static_cast<MemberVariableSymbol*>(member));
        }
        break;
    }
    case SymbolType::staticConstructorSymbol:
    {
        if (staticConstructor)
        {
            throw Exception("already has a static constructor", member->GetFullSpan(), staticConstructor->GetFullSpan());
        }
        else
        {
            staticConstructor = static_cast<StaticConstructorSymbol*>(member);
        }
        break;
    }
    case SymbolType::constructorSymbol:
    {
        ConstructorSymbol* constructor = static_cast<ConstructorSymbol*>(member);
        constructors.push_back(constructor);
        break;
    }
    case SymbolType::destructorSymbol:
    {
        if (destructor)
        {
            throw Exception("already has a destructor", member->GetFullSpan(), destructor->GetFullSpan());
        }
        else
        {
            destructor = static_cast<DestructorSymbol*>(member);
        }
        break;
    }
    case SymbolType::memberFunctionSymbol:
    {
        memberFunctions.push_back(static_cast<MemberFunctionSymbol*>(member));
        break;
    }
    }
}

std::string ClassTypeSymbol::GetSpecifierStr() 
{
    std::string specifierStr;
    if (IsAbstract())
    {
        specifierStr.append("abstract");
    }
    std::string baseSpecifierStr = TypeSymbol::GetSpecifierStr();
    if (!baseSpecifierStr.empty())
    {
        specifierStr.append(1, ' ');
    }
    specifierStr.append(baseSpecifierStr);
    return specifierStr;
}

bool ClassTypeSymbol::HasNontrivialDestructor() 
{
    if (destructor || IsPolymorphic()) return true;
    if (baseClass && baseClass->Destructor()) return true;
    int n = memberVariables.size();
    for (int i = 0; i < n; ++i)
    {
        MemberVariableSymbol* memberVariable = memberVariables[i];
        if (memberVariable->GetType())
        {
            if (memberVariable->GetType()->HasNontrivialDestructor())
            {
                return true;
            }
        }
    }
    return false;
}

void ClassTypeSymbol::Accept(SymbolCollector* collector)
{
    if (IsProject())
    {
        if (prototype)
        {
            prototype->SetParent(Ns());
            if (Constraint())
            {
                cmajor::ast::CloneContext cloneContext;
                prototype->SetConstraint(static_cast<cmajor::ast::ConstraintNode*>(Constraint()->Clone(cloneContext)));
            }
            collector->AddClass(prototype);
        }
        else
        {
            if (Access() == SymbolAccess::public_)
            {
                collector->AddClass(this);
            }
        }
    }
}

const ContainerScope* ClassTypeSymbol::GetArrowScope() const
{
    return GetContainerScope();
}

ContainerScope* ClassTypeSymbol::GetArrowScope()
{
    return GetContainerScope();
}

void ClassTypeSymbol::CollectMembers(SymbolCollector* collector)
{
    TypeSymbol::Accept(collector);
}

void ClassTypeSymbol::Dump(util::CodeFormatter& formatter)
{
    formatter.WriteLine(util::ToUtf8(Name()));
    formatter.WriteLine("group name: " + util::ToUtf8(groupName));
    formatter.WriteLine("full name: " + util::ToUtf8(FullNameWithSpecifiers()));
    if (baseClass)
    {
        formatter.WriteLine("base class: " + util::ToUtf8(baseClass->FullName()));
    }
    if (!implementedInterfaces.empty())
    {
        formatter.WriteLine("implemented interfaces:");
        formatter.IncIndent();
        for (InterfaceTypeSymbol* interface : implementedInterfaces)
        {
            formatter.WriteLine(util::ToUtf8(interface->FullName()));
        }
        formatter.DecIndent();
    }
    if (constraint)
    {
        formatter.WriteLine("constraint: " + constraint->ToString());
    }
    formatter.WriteLine("typeid: " + util::ToString(TypeId()));
    /*
        if (IsPolymorphic())
        {
            formatter.WriteLine("vmt object name: " + VmtObjectName());
        }
    */
    /*
        if (!staticLayout.empty())
        {
            formatter.WriteLine("static object name: " + StaticObjectName());
        }
    */
    formatter.IncIndent();
    SymbolCollector collector;
    TypeSymbol::Accept(&collector);
    if (!collector.Functions().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("MEMBER FUNCTIONS");
        for (FunctionSymbol* function : collector.Functions())
        {
            formatter.WriteLine();
            function->Dump(formatter);
        }
    }
    if (!collector.Classes().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("CLASSES");
        for (ClassTypeSymbol* class_ : collector.Classes())
        {
            formatter.WriteLine();
            class_->Dump(formatter);
        }
    }
    if (!collector.Interfaces().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("INTERFACES");
        for (InterfaceTypeSymbol* interface : collector.Interfaces())
        {
            formatter.WriteLine();
            interface->Dump(formatter);
        }
    }
    if (!collector.AliasTypes().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("ALIAS TYPES");
        for (AliasTypeSymbol* aliasType : collector.AliasTypes())
        {
            formatter.WriteLine();
            aliasType->Dump(formatter);
        }
    }
    if (!collector.Concepts().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("CONCEPTS");
        for (ConceptSymbol* concept_ : collector.Concepts())
        {
            formatter.WriteLine();
            concept_->Dump(formatter);
        }
    }
    if (!collector.Constants().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("CONSTANTS");
        for (ConstantSymbol* constant : collector.Constants())
        {
            formatter.WriteLine();
            constant->Dump(formatter);
        }
    }
    if (!collector.Delegates().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("DELEGATES");
        for (DelegateTypeSymbol* delegate_ : collector.Delegates())
        {
            formatter.WriteLine();
            delegate_->Dump(formatter);
        }
    }
    if (!collector.ClassDelegates().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("CLASS DELEGATES");
        for (ClassDelegateTypeSymbol* classDelegate : collector.ClassDelegates())
        {
            formatter.WriteLine();
            classDelegate->Dump(formatter);
        }
    }
    if (!collector.EnumeratedTypes().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("ENUMERATED TYPES");
        for (EnumTypeSymbol* enumeratedType : collector.EnumeratedTypes())
        {
            formatter.WriteLine();
            enumeratedType->Dump(formatter);
        }
    }
    if (!collector.MemberVariables().empty())
    {
        formatter.WriteLine();
        formatter.WriteLine("MEMBER VARIABLES");
        for (MemberVariableSymbol* memberVariable : collector.MemberVariables())
        {
            formatter.WriteLine();
            memberVariable->Dump(formatter);
        }
    }
    formatter.DecIndent();
}

bool ClassTypeSymbol::IsRecursive(TypeSymbol* type, std::unordered_set<util::uuid, util::UuidHash>& tested)
{
    if (tested.find(TypeId()) != tested.cend())
    {
        bool equal = TypesEqual(type, this);
        return equal;
    }
    tested.insert(TypeId());
    if (TypeSymbol::IsRecursive(type, tested))
    {
        return true;
    }
    if (baseClass && baseClass->IsRecursive(type, tested))
    {
        return true;
    }
    for (MemberVariableSymbol* memberVariable : memberVariables)
    {
        if (memberVariable->GetType()->IsRecursive(type, tested))
        {
            return true;
        }
    }
    return false;
}

void ClassTypeSymbol::CreateDestructorSymbol()
{
    if (!destructor)
    {
        DestructorSymbol* destructorSymbol = new DestructorSymbol(GetSpan(), U"@destructor");
        destructorSymbol->SetModule(GetModule());
        GetModule()->GetSymbolTable().SetFunctionIdFor(destructorSymbol);
        destructorSymbol->SetGenerated();
        ParameterSymbol* thisParam = new ParameterSymbol(GetSpan(), U"this");
        thisParam->SetType(AddPointer());
        destructorSymbol->SetAccess(SymbolAccess::public_);
        destructorSymbol->AddMember(thisParam);
        AddMember(destructorSymbol);
        Assert(destructor, "destructor expected");
        if (GetSymbolType() == SymbolType::classTemplateSpecializationSymbol)
        {
            destructor->SetLinkOnceOdrLinkage();
        }
        destructor->ComputeName();
    }
}

void ClassTypeSymbol::SetGroupName(const std::u32string& groupName_)
{
    groupName = groupName_;
}

void ClassTypeSymbol::ComputeMinArity()
{
    bool defaultHit = false;
    int n = templateParameters.size();
    for (int i = 0; i < n; ++i)
    {
        TemplateParameterSymbol* templateParameter = templateParameters[i];
        if (templateParameter->HasDefault())
        {
            defaultHit = true;
            break;
        }
        minArity = i;
    }
    if (!defaultHit)
    {
        minArity = n;
    }
}

bool ClassTypeSymbol::HasBaseClass(ClassTypeSymbol* cls) const
{
    if (!baseClass) return false;
    if (TypesEqual(baseClass, cls) || baseClass->HasBaseClass(cls)) return true;
    return false;
}

bool ClassTypeSymbol::HasBaseClass(ClassTypeSymbol* cls, uint8_t& distance) const
{
    if (!baseClass) return false;
    ++distance;
    if (TypesEqual(baseClass, cls)) return true;
    return baseClass->HasBaseClass(cls, distance);
}

void ClassTypeSymbol::AddImplementedInterface(InterfaceTypeSymbol* interfaceTypeSymbol)
{
    int n = implementedInterfaces.size();
    for (int i = 0; i < n; ++i)
    {
        if (implementedInterfaces[i] == interfaceTypeSymbol)
        {
            throw Exception("class cannot implement an interface more than once", GetFullSpan(), interfaceTypeSymbol->GetFullSpan());
        }
    }
    implementedInterfaces.push_back(interfaceTypeSymbol);
}

void ClassTypeSymbol::CloneUsingNodes(const std::vector<cmajor::ast::Node*>& usingNodes_)
{
    cmajor::ast::CloneContext cloneContext;
    for (cmajor::ast::Node* usingNode : usingNodes_)
    {
        usingNodes.Add(usingNode->Clone(cloneContext));
    }
}

void ClassTypeSymbol::SetSpecifiers(cmajor::ast::Specifiers specifiers)
{
    cmajor::ast::Specifiers accessSpecifiers = specifiers & cmajor::ast::Specifiers::access_;
    SetAccess(accessSpecifiers);
    if ((specifiers & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        SetStatic();
    }
    if ((specifiers & cmajor::ast::Specifiers::virtual_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be virtual", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::override_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be override", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::abstract_) != cmajor::ast::Specifiers::none)
    {
        SetAbstract();
        SetPolymorphic();
    }
    if ((specifiers & cmajor::ast::Specifiers::inline_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be inline", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::explicit_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be explicit", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::external_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be external", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::suppress_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be suppressed", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::default_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be default", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be constexpr", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::cdecl_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be cdecl", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::nothrow_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be nothrow", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::throw_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be throw", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::new_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be new", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::const_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be const", GetFullSpan());
    }
    if ((specifiers & cmajor::ast::Specifiers::unit_test_) != cmajor::ast::Specifiers::none)
    {
        throw Exception("class type symbol cannot be unit_test", GetFullSpan());
    }
}

void ClassTypeSymbol::ComputeName()
{
    std::u32string name = Name();
    if (IsClassTemplate())
    {
        name.append(1, '<');
        bool first = true;
        for (TemplateParameterSymbol* templateParameter : templateParameters)
        {
            if (first)
            {
                first = false;
            }
            else
            {
                name.append(U", ");
            }
            name.append(templateParameter->Name());
        }
        name.append(1, '>');
    }
    SetName(name);
    ComputeMangledName();
}

void ClassTypeSymbol::ComputeMangledName()
{
    std::u32string mangledName = util::ToUtf32(TypeString());
    mangledName.append(1, U'_').append(SimpleName());
    std::string constraintStr;
    if (constraint)
    {
        constraintStr = " " + constraint->ToString();
    }
    mangledName.append(1, U'_').append(util::ToUtf32(util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()) + constraintStr)));
    SetMangledName(mangledName);
}

void ClassTypeSymbol::SetSpecialMemberFunctions()
{
    int nc = constructors.size();
    for (int i = 0; i < nc; ++i)
    {
        ConstructorSymbol* constructor = constructors[i];
        if (constructor->IsDefaultConstructor())
        {
            defaultConstructor = constructor;
        }
        else if (constructor->IsCopyConstructor())
        {
            copyConstructor = constructor;
        }
        else if (constructor->IsMoveConstructor())
        {
            moveConstructor = constructor;
        }
    }
    int nm = memberFunctions.size();
    for (int i = 0; i < nm; ++i)
    {
        MemberFunctionSymbol* memberFunction = memberFunctions[i];
        if (memberFunction->IsCopyAssignment())
        {
            copyAssignment = memberFunction;
        }
        else if (memberFunction->IsMoveAssignment())
        {
            moveAssignment = memberFunction;
        }
    }
}

bool ClassTypeSymbol::IsLiteralClassType() 
{
    if (IsPolymorphic()) return false;
    if (HasNontrivialDestructor()) return false;
    if (!StaticMemberVariables().empty()) return false;
    if (BaseClass()) return false;
    return true;
}

void ClassTypeSymbol::SetInitializedVar(MemberVariableSymbol* initializedVar_)
{
    initializedVar.reset(initializedVar_);
}

void ClassTypeSymbol::InitVmt()
{
    if (IsVmtInitialized()) return;
    SetVmtInitialized();
    if (baseClass)
    {
        baseClass->InitVmt();
        if (baseClass->IsPolymorphic())
        {
            SetPolymorphic();
        }
    }
    if (destructor && (destructor->IsVirtual() || destructor->IsOverride()))
    {
        SetPolymorphic();
    }
    for (MemberFunctionSymbol* memberFunction : memberFunctions)
    {
        if (memberFunction->IsVirtualAbstractOrOverride())
        {
            SetPolymorphic();
            break;
        }
    }
    if (!implementedInterfaces.empty())
    {
        SetPolymorphic();
    }
    if (IsPolymorphic())
    {
        CreateDestructorSymbol();
        if (baseClass && baseClass->IsPolymorphic())
        {
            destructor->SetOverride();
        }
        else
        {
            destructor->SetVirtual();
        }
    }
    if (IsPolymorphic())
    {
        if (!baseClass || !baseClass->IsPolymorphic())
        {
            if (baseClass)
            {
                vmtPtrIndex = 1;
            }
            else
            {
                vmtPtrIndex = 0;
            }
        }
        InitVmt(vmt);
        for (FunctionSymbol* virtualFunction : vmt)
        {
            if (virtualFunction->IsAbstract())
            {
                if (!IsAbstract())
                {
                    throw Exception("class containing abstract member functions must be declared abstract", GetFullSpan(), virtualFunction->GetFullSpan());
                }
            }
        }
    }
}

bool Overrides(FunctionSymbol* f, FunctionSymbol* g)
{
    if (f->GroupName() == g->GroupName())
    {
        int n = f->Parameters().size();
        if (n == g->Parameters().size())
        {
            for (int i = 1; i < n; ++i)
            {
                ParameterSymbol* p = f->Parameters()[i];
                ParameterSymbol* q = g->Parameters()[i];
                if (!TypesEqual(p->GetType(), q->GetType())) return false;
            }
            return true;
        }
    }
    return false;
}

void ClassTypeSymbol::InitVmt(std::vector<FunctionSymbol*>& vmtToInit)
{
    if (!IsPolymorphic()) return;
    if (baseClass)
    {
        baseClass->InitVmt(vmtToInit);
    }
    std::vector<FunctionSymbol*> virtualFunctions;
    if (destructor)
    {
        if (destructor->IsVirtual() || destructor->IsOverride())
        {
            virtualFunctions.push_back(destructor);
        }
    }
    for (FunctionSymbol* memberFunction : memberFunctions)
    {
        if (memberFunction->IsVirtualAbstractOrOverride())
        {
            virtualFunctions.push_back(memberFunction);
        }
    }
    int n = virtualFunctions.size();
    for (int i = 0; i < n; ++i)
    {
        FunctionSymbol* f = virtualFunctions[i];
        bool found = false;
        int m = vmtToInit.size();
        for (int j = 0; j < m; ++j)
        {
            FunctionSymbol* v = vmtToInit[j];
            if (Overrides(f, v))
            {
                if (!f->IsOverride())
                {
                    throw Exception("overriding function should be declared with override specifier", f->GetFullSpan());
                }
/* TODO exception handling
                if (f->DontThrow() != v->DontThrow())
                {
                    throw Exception("overriding function has conflicting nothrow specification compared to the base class virtual function", f->GetFullSpan(), v->GetFullSpan());
                }
*/
                f->SetVmtIndex(j);
                vmtToInit[j] = f;
                found = true;
                break;
            }
        }
        if (!found)
        {
            if (f->IsOverride())
            {
                throw Exception("no suitable function to override ('" + util::ToUtf8(f->FullName()) + "')", f->GetFullSpan());
            }
            f->SetVmtIndex(m);
            vmtToInit.push_back(f);
        }
    }
}

bool Implements(MemberFunctionSymbol* classMemFun, MemberFunctionSymbol* intfMemFun)
{
    if (classMemFun->GroupName() != intfMemFun->GroupName()) return false;
    if (!classMemFun->ReturnType() || !intfMemFun->ReturnType()) return false;
    if (!TypesEqual(classMemFun->ReturnType(), intfMemFun->ReturnType())) return false;
    int n = classMemFun->Parameters().size();
    if (n != intfMemFun->Parameters().size()) return false;
    for (int i = 1; i < n; ++i)
    {
        TypeSymbol* classMemFunParamType = classMemFun->Parameters()[i]->GetType();
        TypeSymbol* intfMemFunParamType = intfMemFun->Parameters()[i]->GetType();
        if (!TypesEqual(classMemFunParamType, intfMemFunParamType)) return false;
    }
    return true;
}

void ClassTypeSymbol::InitImts()
{
    if (IsImtsInitialized()) return;
    SetImtsInitialized();
    int n = implementedInterfaces.size();
    if (n == 0) return;
    imts.resize(n);
    for (int32_t i = 0; i < n; ++i)
    {
        std::vector<FunctionSymbol*>& imt = imts[i];
        InterfaceTypeSymbol* intf = implementedInterfaces[i];
        int q = intf->MemberFunctions().size();
        imt.resize(q);
    }
    int m = memberFunctions.size();
    for (int j = 0; j < m; ++j)
    {
        MemberFunctionSymbol* classMemFun = memberFunctions[j];
        for (int32_t i = 0; i < n; ++i)
        {
            std::vector<FunctionSymbol*>& imt = imts[i];
            InterfaceTypeSymbol* intf = implementedInterfaces[i];
            int q = intf->MemberFunctions().size();
            for (int k = 0; k < q; ++k)
            {
                MemberFunctionSymbol* intfMemFun = intf->MemberFunctions()[k];
                if (Implements(classMemFun, intfMemFun))
                {
                    imt[intfMemFun->ImtIndex()] = classMemFun;
                    break;
                }
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        InterfaceTypeSymbol* intf = implementedInterfaces[i];
        std::vector<FunctionSymbol*>& imt = imts[i];
        int m = imt.size();
        for (int j = 0; j < m; ++j)
        {
            if (!imt[j])
            {
                MemberFunctionSymbol* intfMemFun = intf->MemberFunctions()[j];
                throw Exception("class '" + util::ToUtf8(FullName()) + "' does not implement interface '" + util::ToUtf8(intf->FullName()) + 
                    "' because implementation of interface function '" +
                    util::ToUtf8(intfMemFun->FullName()) + "' is missing", GetFullSpan(), intfMemFun->GetFullSpan());
            }
        }
    }
}

void ClassTypeSymbol::CreateLayouts()
{
    if (IsLayoutsComputed()) return;
    SetLayoutsComputed();
    if (baseClass)
    {
        objectLayout.push_back(baseClass);
    }
    else
    {
        if (IsPolymorphic())
        {
            vmtPtrIndex = objectLayout.size();
            objectLayout.push_back(GetRootModuleForCurrentThread()->GetSymbolTable().GetTypeByName(U"void")->AddPointer());
        }
        else if (memberVariables.empty())
        {
            objectLayout.push_back(GetRootModuleForCurrentThread()->GetSymbolTable().GetTypeByName(U"byte"));
        }
    }
    int n = memberVariables.size();
    for (int i = 0; i < n; ++i)
    {
        MemberVariableSymbol* memberVariable = memberVariables[i];
        memberVariable->SetLayoutIndex(objectLayout.size());
        objectLayout.push_back(memberVariable->GetType());
    }
    if (!staticMemberVariables.empty() || StaticConstructor())
    {
        MemberVariableSymbol* initVar = new MemberVariableSymbol(GetSpan(), U"@initialized");
        initVar->SetParent(this);
        initVar->SetStatic();
        initVar->SetType(GetRootModuleForCurrentThread()->GetSymbolTable().GetTypeByName(U"bool"));
        initVar->SetLayoutIndex(0);
        SetInitializedVar(initVar);
        staticLayout.push_back(GetRootModuleForCurrentThread()->GetSymbolTable().GetTypeByName(U"bool"));
        int ns = staticMemberVariables.size();
        for (int i = 0; i < ns; ++i)
        {
            MemberVariableSymbol* staticMemberVariable = staticMemberVariables[i];
            staticMemberVariable->SetLayoutIndex(staticLayout.size());
            staticLayout.push_back(staticMemberVariable->GetType());
        }
    }
}

bool ClassTypeSymbol::IsRecursive()
{
    if (RecursiveComputed())
    {
        return Recursive();
    }
    else
    {
        int n = objectLayout.size();
        for (int i = 0; i < n; ++i)
        {
            TypeSymbol* elementType = objectLayout[i];
            std::unordered_set<util::uuid, util::UuidHash> tested;
            if (elementType->IsRecursive(this, tested))
            {
                SetRecursive();
                SetRecursiveComputed();
                return true;
            }
        }
        SetRecursiveComputed();
        return false;
    }
}

void* ClassTypeSymbol::IrType(cmajor::ir::Emitter& emitter)
{
    if (!IsBound())
    {
        throw Exception("class '" + util::ToUtf8(Name()) + "' not bound", GetFullSpan());
    }
    void* localIrType = emitter.GetIrTypeByTypeId(TypeId());
    if (!localIrType)
    {
        std::vector<void*> elementTypes;
        int n = objectLayout.size();
        if (!IsRecursive())
        {
            for (int i = 0; i < n; ++i)
            {
                TypeSymbol* elementType = objectLayout[i];
                elementTypes.push_back(elementType->IrType(emitter));
            }
            localIrType = emitter.GetIrTypeForClassType(elementTypes);
            emitter.SetIrTypeByTypeId(TypeId(), localIrType);
        }
        else
        {

            void* forwardDeclaredType = emitter.CreateFwdIrTypeForClassType();
            localIrType = forwardDeclaredType;
            emitter.SetIrTypeByTypeId(TypeId(), localIrType);
            for (int i = 0; i < n; ++i)
            {
                TypeSymbol* elementType = objectLayout[i];
                elementTypes.push_back(elementType->IrType(emitter));
            }
            emitter.SetFwdIrTypeBody(forwardDeclaredType, elementTypes);
        }
    }
    return localIrType;
}

void* ClassTypeSymbol::CreateDefaultIrValue(cmajor::ir::Emitter& emitter)
{
    void* irType = IrType(emitter);
    std::vector<void*> arrayOfDefaults;
    for (TypeSymbol* type : objectLayout)
    {
        arrayOfDefaults.push_back(type->CreateDefaultIrValue(emitter));
    }
    return emitter.CreateDefaultIrValueForStruct(irType, arrayOfDefaults);
}

void* ClassTypeSymbol::CreateDIType(cmajor::ir::Emitter& emitter)
{
    void* baseClassDIType = nullptr;
    if (baseClass)
    {
        baseClassDIType = baseClass->GetDIType(emitter);
    }
    void* vtableHolderClass = nullptr;
    if (IsPolymorphic() && VmtPtrHolderClass())
    {
        vtableHolderClass = VmtPtrHolderClass()->CreateDIForwardDeclaration(emitter);
        emitter.MapFwdDeclaration(vtableHolderClass, VmtPtrHolderClass()->TypeId());
    }
    std::vector<void*> elements;
    for (MemberVariableSymbol* memberVariable : memberVariables)
    {
        uint64_t offsetInBits = emitter.GetOffsetInBits(IrType(emitter), memberVariable->LayoutIndex());
        elements.push_back(memberVariable->GetDIMemberType(emitter, offsetInBits));
    }
    //return emitter.CreateDITypeForClassType(IrType(emitter), elements, util::ToUtf8(Name()), vtableHolderClass, util::ToUtf8(MangledName()), baseClassDIType);
    return nullptr; // TODO
}

void* ClassTypeSymbol::CreateDIForwardDeclaration(cmajor::ir::Emitter& emitter)
{
    // todo LLVM debug info
    return emitter.CreateIrDIForwardDeclaration(IrType(emitter), util::ToUtf8(Name()), util::ToUtf8(MangledName()), soul::ast::FullSpan(), soul::ast::LineColLen()); 
}

std::string ClassTypeSymbol::VmtObjectNameStr()
{
    return "vmt_" + util::ToUtf8(SimpleName()) + "_" + util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()));;
}

std::string ClassTypeSymbol::VmtObjectName(cmajor::ir::Emitter& emitter)
{
    std::string localVmtObjectName = emitter.GetVmtObjectName(this);
    if (localVmtObjectName.empty())
    {
        localVmtObjectName = VmtObjectNameStr();
        emitter.SetVmtObjectName(this, localVmtObjectName);
    }
    return localVmtObjectName;
}

std::string ClassTypeSymbol::ImtArrayObjectName(cmajor::ir::Emitter& emitter)
{
    std::string localImtArrayObjectName = emitter.GetImtArrayObjectName(this);
    if (localImtArrayObjectName.empty())
    {
        localImtArrayObjectName = "imts_" + util::ToUtf8(SimpleName()) + "_" + util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()));
        emitter.SetImtArrayObjectName(this, localImtArrayObjectName);
    }
    return localImtArrayObjectName;
}

std::string ClassTypeSymbol::ImtObjectName(int index)
{
    return "imt_" + std::to_string(index) + "_" + util::ToUtf8(SimpleName()) + "_" + util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()));
}

ClassTypeSymbol* ClassTypeSymbol::VmtPtrHolderClass()
{
    if (!IsPolymorphic())
    {
        throw Exception("nonpolymorphic class does not contain a vmt ptr", GetFullSpan());
    }
    if (vmtPtrIndex != -1)
    {
        return this;
    }
    else
    {
        if (baseClass)
        {
            return baseClass->VmtPtrHolderClass();
        }
        else
        {
            throw Exception("vmt ptr holder class not found", GetFullSpan());
        }
    }
}

void* ClassTypeSymbol::VmtArrayType(cmajor::ir::Emitter& emitter)
{
    void* vmtArrayType = emitter.GetIrTypeForArrayType(emitter.GetIrTypeForVoidPtrType(), vmt.size() + GetFunctionVmtIndexOffset());
    return vmtArrayType;
}

void* ClassTypeSymbol::VmtPtrType(cmajor::ir::Emitter& emitter)
{
    void* vmtArrayType = VmtArrayType(emitter);
    void* vmtPtrType = emitter.GetIrTypeForPtrType(vmtArrayType);
    return vmtPtrType;
}

void* ClassTypeSymbol::CreateImt(cmajor::ir::Emitter& emitter, int index)
{
    std::vector<FunctionSymbol*>& imt = imts[index];
    std::string imtObjectName = ImtObjectName(index);
    void* imtType = emitter.GetIrTypeForArrayType(emitter.GetIrTypeForVoidPtrType(), imt.size());
    void* imtObject = emitter.GetOrInsertGlobal(imtObjectName, imtType);
    if (GetBackEnd() != BackEnd::cpp)
    {
        void* comdat = emitter.GetOrInsertAnyComdat(imtObjectName, imtObject);
    }
    std::vector<void*> irImt;
    int n = imt.size();
    for (int i = 0; i < n; ++i)
    {
        FunctionSymbol* memFun = imt[i];
        void* interfaceFun = emitter.GetOrInsertFunction(util::ToUtf8(memFun->MangledName()), memFun->IrType(emitter), memFun->DontThrow());
        if (GetBackEnd() == BackEnd::cpp)
        {
            irImt.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), interfaceFun));
        }
        else
        {
            irImt.push_back(emitter.CreateBitCast(interfaceFun, emitter.GetIrTypeForVoidPtrType()));
        }
    }
    emitter.SetInitializer(imtObject, emitter.CreateIrValueForConstantArray(imtType, irImt, std::string()));
    return imtObject;
}

void* ClassTypeSymbol::CreateImts(cmajor::ir::Emitter& emitter)
{
    std::string imtArrayObjectName = ImtArrayObjectName(emitter);
    void* imtsArrayType = emitter.GetIrTypeForArrayType(emitter.GetIrTypeForVoidPtrType(), implementedInterfaces.size());
    void* imtsArrayObject = emitter.GetOrInsertGlobal(imtArrayObjectName, imtsArrayType);
    if (GetBackEnd() != BackEnd::cpp)
    {
        void* comdat = emitter.GetOrInsertAnyComdat(imtArrayObjectName, imtsArrayObject);
    }
    std::vector<void*> imtsArray;
    int n = imts.size();
    for (int i = 0; i < n; ++i)
    {
        void* irImt = CreateImt(emitter, i);
        if (GetBackEnd() == BackEnd::cpp)
        {
            imtsArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), irImt));
        }
        else
        {
            imtsArray.push_back(emitter.CreateBitCast(irImt, emitter.GetIrTypeForVoidPtrType()));
        }
    }
    emitter.SetInitializer(imtsArrayObject, emitter.CreateIrValueForConstantArray(imtsArrayType, imtsArray, std::string()));
    return imtsArrayObject;
}

void* ClassTypeSymbol::VmtObject(cmajor::ir::Emitter& emitter, bool create)
{
    if (!IsPolymorphic()) return nullptr;
    void* localVmtObjectType = emitter.GetVmtObjectType(this);
    if (!localVmtObjectType)
    {
        localVmtObjectType = emitter.GetIrTypeForArrayType(emitter.GetIrTypeForVoidPtrType(), vmt.size() + GetFunctionVmtIndexOffset()); 
        emitter.SetVmtObjectType(this, localVmtObjectType);
    }
    void* className = nullptr;
    if (GetBackEnd() != BackEnd::masm)
    {
        if (!emitter.IsVmtObjectCreated(this) && create)
        {
            className = emitter.CreateGlobalStringPtr(util::ToUtf8(FullName()));
        }
    }
    void* vmtObject = emitter.GetOrInsertGlobal(VmtObjectName(emitter), localVmtObjectType);
    if (!emitter.IsVmtObjectCreated(this) && create && !VmtEmitted())
    {
        SetVmtEmitted();
        emitter.SetVmtObjectCreated(this);
        bool specialization = GetSymbolType() == SymbolType::classTemplateSpecializationSymbol;
        if (specialization)
        {
            std::string vmtObjectName = VmtObjectName(emitter);
            void* comdat = emitter.GetOrInsertAnyComdat(vmtObjectName, vmtObject);
        }
        std::vector<void*> vmtArray;
        if (GetBackEnd() == BackEnd::llvm) 
        {
            uint64_t typeId1 = 0;
            uint64_t typeId2 = 0;
            UuidToInts(TypeId(), typeId1, typeId2);
            //      16-byte type id:
            vmtArray.push_back(emitter.CreateIntToPtr(emitter.CreateIrValueForULong(typeId1), emitter.GetIrTypeForVoidPtrType()));
            vmtArray.push_back(emitter.CreateIntToPtr(emitter.CreateIrValueForULong(typeId2), emitter.GetIrTypeForVoidPtrType()));
            if (!implementedInterfaces.empty())
            {
                void* itabsArrayObject = CreateImts(emitter);
                vmtArray.push_back(emitter.CreateBitCast(itabsArrayObject, emitter.GetIrTypeForVoidPtrType())); // interface method table pointer
            }
            else
            {
                vmtArray.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
            }
            //      virtual method table:
            int n = vmt.size();
            for (int i = 0; i < n; ++i)
            {
                FunctionSymbol* virtualFunction = vmt[i];
                if (!virtualFunction || virtualFunction->IsAbstract())
                {
                    vmtArray.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
                }
                else
                {
                    void* functionObject = emitter.GetOrInsertFunction(util::ToUtf8(virtualFunction->MangledName()), virtualFunction->IrType(emitter), virtualFunction->DontThrow());
                    vmtArray.push_back(emitter.CreateBitCast(functionObject, emitter.GetIrTypeForVoidPtrType()));
                }
            }
        }
        else if (GetBackEnd() == BackEnd::cpp)
        {
            uint64_t typeId1 = 0;
            uint64_t typeId2 = 0;
            UuidToInts(TypeId(), typeId1, typeId2);
            //      16-byte type id:
            vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), emitter.CreateIrValueForULong(typeId1)));
            vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), emitter.CreateIrValueForULong(typeId2)));
            if (!implementedInterfaces.empty())
            {
                void* itabsArrayObject = CreateImts(emitter);
                vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), itabsArrayObject)); // interface method table pointer
            }
            else
            {
                vmtArray.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
            }
            //      virtual method table:
            int n = vmt.size();
            for (int i = 0; i < n; ++i)
            {
                FunctionSymbol* virtualFunction = vmt[i];
                if (!virtualFunction || virtualFunction->IsAbstract())
                {
                    vmtArray.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
                }
                else
                {
                    void* functionObject = emitter.GetOrInsertFunction(util::ToUtf8(virtualFunction->MangledName()), virtualFunction->IrType(emitter), virtualFunction->DontThrow());
                    vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), functionObject));
                }
            }
        }
        else if (GetBackEnd() == BackEnd::systemx)
        {
            std::string typeId = util::ToString(TypeId());
            vmtArray.push_back(emitter.GetClsIdValue(typeId)); // 128-bit type id
            vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), className)); // class name pointer
            if (!implementedInterfaces.empty())
            {
                void* itabsArrayObject = CreateImts(emitter);
                vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), itabsArrayObject)); // interface method table pointer
            }
            else
            {
                vmtArray.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
            }
            //      virtual method table:
            int n = vmt.size();
            for (int i = 0; i < n; ++i)
            {
                FunctionSymbol* virtualFunction = vmt[i];
                if (!virtualFunction || virtualFunction->IsAbstract())
                {
                    vmtArray.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
                }
                else
                {
                    void* functionObject = emitter.GetOrInsertFunction(util::ToUtf8(virtualFunction->MangledName()), virtualFunction->IrType(emitter), virtualFunction->DontThrow());
                    vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), functionObject));
                }
            }
        }
        else if (GetBackEnd() == BackEnd::masm)
        {
            uint64_t typeId1 = 0;
            uint64_t typeId2 = 0;
            UuidToInts(TypeId(), typeId1, typeId2);
            //      16-byte type id:
            vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), emitter.CreateIrValueForULong(typeId1)));
            vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), emitter.CreateIrValueForULong(typeId2)));
            if (!implementedInterfaces.empty())
            {
                void* itabsArrayObject = CreateImts(emitter);
                vmtArray.push_back(emitter.CreateBitCast(itabsArrayObject, emitter.GetIrTypeForVoidPtrType())); // interface method table pointer
            }
            else
            {
                vmtArray.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
            }
            //      virtual method table:
            int n = vmt.size();
            for (int i = 0; i < n; ++i)
            {
                FunctionSymbol* virtualFunction = vmt[i];
                if (!virtualFunction || virtualFunction->IsAbstract())
                {
                    vmtArray.push_back(emitter.CreateDefaultIrValueForVoidPtrType());
                }
                else
                {
                    void* functionObject = emitter.GetOrInsertFunction(util::ToUtf8(virtualFunction->InstantiatedName()), virtualFunction->IrType(emitter), 
                        virtualFunction->DontThrow());
                    vmtArray.push_back(emitter.GetConversionValue(emitter.GetIrTypeForVoidPtrType(), functionObject));
                }
            }
        }
        void* initializer = emitter.CreateIrValueForConstantArray(localVmtObjectType, vmtArray, std::string());
        emitter.SetInitializer(vmtObject, initializer);
    }
    return vmtObject;
}

void* ClassTypeSymbol::StaticObject(cmajor::ir::Emitter& emitter, bool create)
{
    if (staticLayout.empty()) return nullptr;
    void* staticObject = emitter.GetOrInsertGlobal(StaticObjectName(emitter), StaticObjectType(emitter));
    if (!emitter.IsStaticObjectCreated(this) && create)
    {
        emitter.SetStaticObjectCreated(this);
        void* staticObjectGlobal = staticObject;
        if (StaticsEmitted())
        {
            return staticObject;
        }
        SetStaticsEmitted();
        std::vector<void*> arrayOfStatics;
        for (TypeSymbol* type : staticLayout)
        {
            arrayOfStatics.push_back(type->CreateDefaultIrValue(emitter));
        }
        emitter.SetInitializer(staticObjectGlobal, emitter.CreateIrValueForConstantStruct(StaticObjectType(emitter), arrayOfStatics));
    }
    return staticObject;
}

void* ClassTypeSymbol::StaticObjectType(cmajor::ir::Emitter& emitter)
{
    void* localStaticObjectType = emitter.GetStaticObjectType(this);
    if (!localStaticObjectType)
    {
        std::vector<void*> elementTypes;
        int n = staticLayout.size();
        for (int i = 0; i < n; ++i)
        {
            void* elementType = staticLayout[i]->IrType(emitter);
            elementTypes.push_back(elementType);
        }
        localStaticObjectType = emitter.GetIrTypeForStructType(elementTypes);
    }
    emitter.SetStaticObjectType(this, localStaticObjectType);
    return localStaticObjectType;
}

std::string ClassTypeSymbol::StaticObjectName(cmajor::ir::Emitter& emitter)
{
    std::string localStaticObjectName = emitter.GetStaticObjectName(this);
    if (localStaticObjectName.empty())
    {
        localStaticObjectName = "statics_" + util::ToUtf8(SimpleName()) + "_" + util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()));
        emitter.SetStaticObjectName(this, localStaticObjectName);
    }
    return localStaticObjectName;
}

ValueType ClassTypeSymbol::GetValueType() const
{
    return ValueType::structuredValue;
}

Value* ClassTypeSymbol::MakeValue() const
{
    std::vector<std::unique_ptr<Value>> memberValues;
    return new StructuredValue(GetSpan(), const_cast<TypeSymbol*>(static_cast<const TypeSymbol*>(this)), std::move(memberValues));
}

std::u32string ClassTypeSymbol::Id() const
{
    if (prototype)
    {
        return prototype->Id();
    }
    else
    {
        return MangledName();
    }
}

void ClassTypeSymbol::Check()
{
    TypeSymbol::Check();
    if (groupName.empty())
    {
        throw SymbolCheckException("class type symbol has empty group name", GetFullSpan());
    }
}

std::unique_ptr<Symbol> ClassTypeSymbol::RemoveFromParent()
{
    std::unique_ptr<Symbol> symbol = TypeSymbol::RemoveFromParent();
    if (classGroup)
    {
        classGroup->RemoveClass(this);
        if (classGroup->IsEmpty())
        {
            std::unique_ptr<Symbol> classGroupSymbol = classGroup->RemoveFromParent();
        }
    }
    return symbol;
}

struct ClassInfo
{
    ClassInfo() : cls(nullptr), baseClassInfo(nullptr), level(0), key(0), id(0) {}
    ClassInfo(ClassTypeSymbol* cls_) : cls(cls_), baseClassInfo(nullptr), level(0), key(0), id(0) {}
    ClassTypeSymbol* cls;
    ClassInfo* baseClassInfo;
    int level;
    boost::multiprecision::uint128_t key;
    boost::multiprecision::uint128_t id;
};

void ResolveBaseClasses(std::map<util::uuid, ClassInfo>& classIdMap)
{
    for (auto& p : classIdMap)
    {
        ClassInfo& info = p.second;
        ClassTypeSymbol* baseClass = info.cls->BaseClass();
        if (baseClass)
        {
            auto it = classIdMap.find(baseClass->TypeId());
            if (it != classIdMap.cend())
            {
                ClassInfo* baseClassInfo = &it->second;
                info.baseClassInfo = baseClassInfo;
            }
            else
            {
                throw Exception("internal error: could not resolve base class info for class '" + util::ToUtf8(info.cls->FullName()) + "'",
                    info.cls->GetFullSpan());
            }
        }
    }
}

int NumberOfAncestors(ClassInfo& cls)
{
    int numAncestors = 0;
    ClassInfo* baseClass = cls.baseClassInfo;
    while (baseClass)
    {
        ++numAncestors;
        baseClass = baseClass->baseClassInfo;
    }
    return numAncestors;
}

void AssignLevels(std::map<util::uuid, ClassInfo>& classIdMap)
{
    for (auto& p : classIdMap)
    {
        ClassInfo& cls = p.second;
        cls.level = NumberOfAncestors(cls);
    }
}

struct PriorityGreater
{
    bool operator()(ClassInfo* left, ClassInfo* right) const
    {
        if (left->level < right->level) return true;
        return false;
    }
};

std::vector<ClassInfo*> GetClassesByPriority(std::map<util::uuid, ClassInfo>& classIdMap)
{
    std::vector<ClassInfo*> classesByPriority;
    for (auto& p : classIdMap)
    {
        ClassInfo* cls = &p.second;
        classesByPriority.push_back(cls);
    }
    std::sort(classesByPriority.begin(), classesByPriority.end(), PriorityGreater());
    return classesByPriority;
}

void AssignKeys(std::vector<ClassInfo*>& classesByPriority)
{
    boost::multiprecision::uint128_t key;
    util::NextPrime(key);
    for (ClassInfo* cls : classesByPriority)
    {
        cls->key = key;
        util::NextPrime(key);
        if (key < cls->key)
        {
            throw std::runtime_error("error assigning class key for class " + util::ToUtf8(cls->cls->FullName()) + ": overflow, too many polymorphic classes");
        }
    }
}

boost::multiprecision::uint128_t ComputeClassId(ClassInfo* cls)
{
    boost::multiprecision::uint128_t classId = cls->key;
    ClassInfo* baseClass = cls->baseClassInfo;
    while (baseClass)
    {
        boost::multiprecision::uint128_t key = baseClass->key;
        boost::multiprecision::uint128_t product = classId * key;
        if (product / classId != key || product % key != 0)
        {
            throw std::runtime_error("error computing class key for class " + util::ToUtf8(cls->cls->FullName()) + ": overflow, too many polymorphic classes");
        }
        classId = product;
        baseClass = baseClass->baseClassInfo;
    }
    if (classId == 0)
    {
        throw std::runtime_error("internal error assigning class id's: invalid resulting class id 0");
    }
    return classId;
}

void AssignClassIds(std::vector<ClassInfo*>& classesByPriority)
{
    for (ClassInfo* cls : classesByPriority)
    {
        cls->id = ComputeClassId(cls);
    }
}

cmajor::ast::ConstantNode* MakePolymorphicClassArray(const std::set<ClassTypeSymbol*>& polymorphicClasses, const std::u32string& arrayName)
{
    std::map<util::uuid, ClassInfo> classIdMap;
    for (ClassTypeSymbol* cls : polymorphicClasses)
    {
        classIdMap[cls->TypeId()] = ClassInfo(cls);
    }
    ResolveBaseClasses(classIdMap);
    AssignLevels(classIdMap);
    std::vector<ClassInfo*> classesByPriority = GetClassesByPriority(classIdMap);
    AssignKeys(classesByPriority);
    AssignClassIds(classesByPriority);
    cmajor::ast::ArrayLiteralNode* polymorphicClassArrayLiteral = new cmajor::ast::ArrayLiteralNode(soul::ast::Span());
    for (ClassInfo* info : classesByPriority)
    {
        const util::uuid& typeId = info->cls->TypeId();
        uint64_t typeId1 = 0;
        uint64_t typeId2 = 0;
        UuidToInts(typeId, typeId1, typeId2);
        polymorphicClassArrayLiteral->AddValue(new cmajor::ast::ULongLiteralNode(soul::ast::Span(), typeId1));
        polymorphicClassArrayLiteral->AddValue(new cmajor::ast::ULongLiteralNode(soul::ast::Span(), typeId2));
        polymorphicClassArrayLiteral->AddValue(new cmajor::ast::ULongLiteralNode(soul::ast::Span(), static_cast<uint64_t>(info->id >> 64)));
        polymorphicClassArrayLiteral->AddValue(new cmajor::ast::ULongLiteralNode(soul::ast::Span(), static_cast<uint64_t>(info->id)));
    }
    uint64_t arrayLength = polymorphicClassArrayLiteral->Values().Count();
    cmajor::ast::ConstantNode* polymorphicClassArray = new cmajor::ast::ConstantNode(soul::ast::Span(), cmajor::ast::Specifiers::internal_, new cmajor::ast::ArrayNode(soul::ast::Span(), new cmajor::ast::ULongNode(soul::ast::Span()),
        cmajor::ast::CreateIntegerLiteralNode(soul::ast::Span(), arrayLength, false)), new cmajor::ast::IdentifierNode(soul::ast::Span(), arrayName), polymorphicClassArrayLiteral);
    return polymorphicClassArray;
}

cmajor::ast::ConstantNode* MakeStaticClassArray(const std::set<ClassTypeSymbol*>& classesHavingStaticConstructor, const std::u32string& arrayName)
{
    std::set<util::uuid> staticTypeIdSet;
    for (ClassTypeSymbol* cls : classesHavingStaticConstructor)
    {
        staticTypeIdSet.insert(cls->TypeId());
    }
    cmajor::ast::ArrayLiteralNode* staticTypeIdArrayLiteral = new cmajor::ast::ArrayLiteralNode(soul::ast::Span());
    for (const util::uuid& typeId : staticTypeIdSet)
    {
        uint64_t typeId1 = 0;
        uint64_t typeId2 = 0;
        UuidToInts(typeId, typeId1, typeId2);
        staticTypeIdArrayLiteral->AddValue(new cmajor::ast::ULongLiteralNode(soul::ast::Span(), typeId1));
        staticTypeIdArrayLiteral->AddValue(new cmajor::ast::ULongLiteralNode(soul::ast::Span(), typeId2));
    }
    uint64_t arrayLength = staticTypeIdArrayLiteral->Values().Count();
    cmajor::ast::ConstantNode* staticClassIdArray = new cmajor::ast::ConstantNode(soul::ast::Span(), cmajor::ast::Specifiers::internal_, new cmajor::ast::ArrayNode(soul::ast::Span(), new cmajor::ast::ULongNode(soul::ast::Span()),
        cmajor::ast::CreateIntegerLiteralNode(soul::ast::Span(), arrayLength, false)), new cmajor::ast::IdentifierNode(soul::ast::Span(), arrayName), staticTypeIdArrayLiteral);
    return staticClassIdArray;
}

#define CLASS_ID_FILE_DEBUG 0

void MakeClassIdFile(const std::set<ClassTypeSymbol*>& polymorphicClasses, const std::string& classIdFileName)
{
    util::FileStream file(classIdFileName, util::OpenMode::write | util::OpenMode::binary);
    util::BufferedStream bufferedFile(file);
    util::BinaryStreamWriter binaryStreamWriter(bufferedFile);
    std::map<util::uuid, ClassInfo> classIdMap;
    for (ClassTypeSymbol* cls : polymorphicClasses)
    {
        classIdMap[cls->TypeId()] = ClassInfo(cls);
    }
    ResolveBaseClasses(classIdMap);
    AssignLevels(classIdMap);
    std::vector<ClassInfo*> classesByPriority = GetClassesByPriority(classIdMap);
    AssignKeys(classesByPriority);
    AssignClassIds(classesByPriority);
    uint64_t n = classesByPriority.size();
#if (CLASS_ID_FILE_DEBUG)
    std::cerr << n << " classes" << std::endl;
#endif
    binaryStreamWriter.Write(n);
    uint64_t i = 0;
    for (ClassInfo* info : classesByPriority)
    {
        const util::uuid& typeId = info->cls->TypeId();
        binaryStreamWriter.Write(typeId);
        binaryStreamWriter.Write(static_cast<uint64_t>(info->id >> 64));
        binaryStreamWriter.Write(static_cast<uint64_t>(info->id));
#if (CLASS_ID_FILE_DEBUG)
        std::cerr << "class" << i << std::endl;
#endif
        ++i;
    }
}

void MakeClassIndexFile(const std::set<ClassTypeSymbol*>& polymorphicClasses, const std::string& classIndexFilePath)
{
    util::FileStream file(classIndexFilePath, util::OpenMode::write | util::OpenMode::binary);
    util::BufferedStream bufferedFile(file);
    util::BinaryStreamWriter binaryStreamWriter(bufferedFile);
    binaryStreamWriter.Write(static_cast<int32_t>(polymorphicClasses.size()));
    for (ClassTypeSymbol* cls : polymorphicClasses)
    {
        binaryStreamWriter.Write(cls->TypeId());
        ClassTypeSymbol* baseClass = cls->BaseClass();
        if (baseClass)
        {
            binaryStreamWriter.Write(baseClass->TypeId());
        }
        else
        {
            binaryStreamWriter.Write(util::nil_uuid());
        }
        binaryStreamWriter.Write(cls->FullName());
    }
}

} // namespace cmajor::symbols
