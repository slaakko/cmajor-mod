// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.symbol.table;

import cmajor.symbols.type.symbol;
import cmajor.symbols.exception;
import cmajor.symbols.context;
import cmajor.symbols.classes;
import cmajor.symbols.modules;
import cmajor.symbols.module_cache;
import cmajor.symbols.array.type.symbol;
import cmajor.symbols.class_template_specializations;
import cmajor.symbols.global.flags;
import cmajor.symbols.concepts;
import cmajor.symbols.function.symbol;
import cmajor.symbols.exception;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.templates;
import cmajor.symbols.interfaces;
import cmajor.symbols.delegate.symbol;
import cmajor.symbols.alias.type;
import cmajor.symbols.constant.symbol;
import cmajor.symbols.enumerations;
import cmajor.symbols.basic.type.symbol;
import cmajor.symbols.basic.type.operation;
import cmajor.symbols.string.functions;
import util;

namespace cmajor::symbols {

size_t ClassTemplateSpecializationKeyHash::operator()(const ClassTemplateSpecializationKey& key) const
{
    size_t x = util::HashValue(key.classTemplate->TypeId());
    int n = key.templateArgumentTypes.size();
    for (int i = 0; i < n; ++i)
    {
        x = x ^ util::HashValue(key.templateArgumentTypes[i]->TypeId());
    }
    return x;
}

bool operator==(const ClassTemplateSpecializationKey& left, const ClassTemplateSpecializationKey& right)
{
    if (!TypesEqual(left.classTemplate, right.classTemplate)) return false;
    int n = left.templateArgumentTypes.size();
    if (n != right.templateArgumentTypes.size()) return false;
    for (int i = 0; i < n; ++i)
    {
        if (!TypesEqual(left.templateArgumentTypes[i], right.templateArgumentTypes[i])) return false;
    }
    return true;
}

bool operator!=(const ClassTemplateSpecializationKey& left, const ClassTemplateSpecializationKey& right)
{
    return !(left == right);
}

bool operator==(const ArrayKey& left, const ArrayKey& right)
{
    if (!TypesEqual(left.elementType, right.elementType)) return false;
    if (left.size != right.size) return false;
    return true;
}

bool operator!=(const ArrayKey& left, const ArrayKey& right)
{
    return !(left == right);
}

size_t ArrayKeyHash::operator()(const ArrayKey& key) const
{
    size_t x = util::HashValue(key.elementType->TypeId());
    x = x ^ std::hash<int64_t>()(key.size);
    return x;
}

void MapIdentifierToSymbolDefinition(cmajor::ast::IdentifierNode* identifierNode, Symbol* symbol)
{
    if (identifierNode->IsInternal()) return;
    if (!identifierNode->GetSpan().IsValid()) return;
    if (identifierNode->ModuleId().is_nil()) return;
    if (!symbol) return;
    if (symbol->ModuleId().is_nil()) return;
    if (!symbol->GetSpan().IsValid()) return;
    Module* module = GetModuleById(identifierNode->ModuleId());
    if (!module) return;
    if (module->IsImmutable()) return;
    module->GetSymbolTable().MapIdentifierToSymbolDefinition(identifierNode, module, symbol);
}

SymbolTable::SymbolTable(Module* module_, Context* context) :
    module(module_), globalNs(soul::ast::Span(), std::u32string()), currentCompileUnit(nullptr), container(&globalNs), currentClass(nullptr),
    currentInterface(nullptr), mainFunctionSymbol(nullptr), currentFunctionSymbol(nullptr), parameterIndex(0), declarationBlockIndex(0),
    conversionTable(ConversionTable::Owner::symbolTable, context, module), numSpecializationsNew(0), numSpecializationsCopied(0), createdFunctionSymbol(nullptr),
    cursorContainer(nullptr), axiomNumber(0), instantiatingTemplate(false), currentFileIndex(-1), currentModuleId(util::nil_uuid())
{
    globalNs.SetModule(module);
    globalNs.SetModuleId(module->Id());
}

SymbolTable::~SymbolTable()
{
}

void SymbolTable::PushInstantiatingTemplate()
{
    instantiatingTemplateStack.push(instantiatingTemplate);
    instantiatingTemplate = true;
}

void SymbolTable::PushCurrentFileIndex(int32_t currentFileIndex_)
{
    fileIndexStack.push(currentFileIndex);
    currentFileIndex = currentFileIndex_;
}

void SymbolTable::PushCurrentModuleId(const util::uuid& currentModuleId_)
{
    moduleIdStack.push(currentModuleId);
    currentModuleId = currentModuleId_;
}

void SymbolTable::PopInstantiatingTemplate()
{
    instantiatingTemplate = instantiatingTemplateStack.top();
    instantiatingTemplateStack.pop();
}

void SymbolTable::PopCurrentFileIndex()
{
    currentFileIndex = fileIndexStack.top();
    fileIndexStack.pop();
}

void SymbolTable::PopCurrentModuleId()
{
    currentModuleId = moduleIdStack.top();
    moduleIdStack.pop();
}

void SymbolTable::Write(SymbolWriter& writer)
{
    if (module->Name() == U"System.Core") // System.Core is special
    {
        uint32_t numDerivationIds = derivationIds.size();
        writer.GetBinaryStreamWriter().WriteULEB128UInt(numDerivationIds);
        for (uint32_t i = 0; i < numDerivationIds; ++i)
        {
            writer.GetBinaryStreamWriter().Write(derivationIds[i]);
        }
        uint32_t numPositionIds = positionIds.size();
        writer.GetBinaryStreamWriter().WriteULEB128UInt(numPositionIds);
        for (uint32_t i = 0; i < numPositionIds; ++i)
        {
            writer.GetBinaryStreamWriter().Write(positionIds[i]);
        }
    }
    globalNs.Write(writer);
    WriteSymbolDefinitionMap(writer);
    std::vector<ArrayTypeSymbol*> exportedArrayTypes;
    for (const std::unique_ptr<ArrayTypeSymbol>& arrayType : arrayTypes)
    {
        if (arrayType->IsProject())
        {
            exportedArrayTypes.push_back(arrayType.get());
        }
    }
    uint32_t na = exportedArrayTypes.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(na);
    for (ArrayTypeSymbol* exportedArrayType : exportedArrayTypes)
    {
        writer.Write(exportedArrayType);
    }
    std::vector<TypeSymbol*> exportedDerivedTypes;
    std::map<util::uuid, TypeSymbol*> derivedTypeMap;
    for (const auto& derivedType : derivedTypes)
    {
        if (derivedType->IsProject())
        {
            if (derivedType->BaseType()->IsClassTemplateSpecializationSymbol() && !derivedType->BaseType()->IsBound()) continue;
            derivedTypeMap[derivedType->TypeId()] = derivedType.get(); // last wins
        }
    }
    for (const auto& p : derivedTypeMap)
    {
        exportedDerivedTypes.push_back(p.second);
    }
    uint32_t ned = exportedDerivedTypes.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(ned);
    for (TypeSymbol* exportedDerivedType : exportedDerivedTypes)
    {
        writer.Write(exportedDerivedType);
    }
    std::vector<TypeSymbol*> exportedClassTemplateSpecializations;
    std::map<util::uuid, TypeSymbol*> specializationMap;
    for (const auto& classTemplateSpecialization : classTemplateSpecializations)
    {
        if (classTemplateSpecialization->IsProject() && classTemplateSpecialization->IsBound())
        {
            specializationMap[classTemplateSpecialization->TypeId()] = classTemplateSpecialization.get(); // last wins
        }
    }
    for (const auto& p : specializationMap)
    {
        exportedClassTemplateSpecializations.push_back(p.second);
    }
    uint32_t nec = exportedClassTemplateSpecializations.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(nec);
    for (TypeSymbol* classTemplateSpecialization : exportedClassTemplateSpecializations)
    {
        writer.Write(classTemplateSpecialization);
    }
    uint32_t nj = jsonClasses.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(nj);
    for (const std::u32string& jsonClass : jsonClasses)
    {
        writer.GetBinaryStreamWriter().Write(jsonClass);
    }
    if (GetGlobalFlag(GlobalFlags::profile))
    {
        uint32_t n = profiledFunctionNameMap.size();
        writer.GetBinaryStreamWriter().WriteULEB128UInt(n);
        for (const auto& p : profiledFunctionNameMap)
        {
            writer.GetBinaryStreamWriter().Write(p.first);
            writer.GetBinaryStreamWriter().Write(p.second);
        }
    }
}

void SymbolTable::Read(SymbolReader& reader)
{
    reader.SetSymbolTable(this);
    if (module->Name() == U"System.Core") // System.Core is special
    {
        uint32_t numDerivationIds = reader.GetBinaryStreamReader().ReadULEB128UInt();
        for (uint32_t i = 0; i < numDerivationIds; ++i)
        {
            util::uuid derivationId;
            reader.GetBinaryStreamReader().ReadUuid(derivationId);
            derivationIds.push_back(derivationId);
        }
        uint32_t numPositionIds = reader.GetBinaryStreamReader().ReadULEB128UInt();
        for (uint32_t i = 0; i < numPositionIds; ++i)
        {
            util::uuid positionId;
            reader.GetBinaryStreamReader().ReadUuid(positionId);
            positionIds.push_back(positionId);
        }
    }
    globalNs.Read(reader);
    ReadSymbolDefinitionMap(reader);
    uint32_t na = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0; i < na; ++i)
    {
        ArrayTypeSymbol* arrayTypeSymbol = reader.ReadArrayTypeSymbol(&globalNs);
        arrayTypes.push_back(std::unique_ptr<ArrayTypeSymbol>(arrayTypeSymbol));
        reader.AddArrayType(arrayTypeSymbol);
    }
    uint32_t nd = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0; i < nd; ++i)
    {
        DerivedTypeSymbol* derivedTypeSymbol = reader.ReadDerivedTypeSymbol(&globalNs);
        derivedTypes.push_back(std::unique_ptr<DerivedTypeSymbol>(derivedTypeSymbol));
        reader.AddDerivedType(derivedTypeSymbol);
    }
    uint32_t nc = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0; i < nc; ++i)
    {
        ClassTemplateSpecializationSymbol* classTemplateSpecialization = reader.ReadClassTemplateSpecializationSymbol(&globalNs);
        classTemplateSpecializations.push_back(std::unique_ptr<ClassTemplateSpecializationSymbol>(classTemplateSpecialization));
        reader.AddClassTemplateSpecialization(classTemplateSpecialization);
    }
    uint32_t nj = reader.GetBinaryStreamReader().ReadULEB128UInt();
    for (uint32_t i = 0; i < nj; ++i)
    {
        std::u32string jsonClass = reader.GetBinaryStreamReader().ReadUtf32String();
        jsonClasses.insert(jsonClass);
    }
    if (GetGlobalFlag(GlobalFlags::profile))
    {
        uint32_t n = reader.GetBinaryStreamReader().ReadULEB128UInt();
        for (uint32_t i = 0; i < n; ++i)
        {
            util::uuid functionId;
            reader.GetBinaryStreamReader().ReadUuid(functionId);
            std::u32string profiledFunctionName = reader.GetBinaryStreamReader().ReadUtf32String();
            MapProfiledFunction(functionId, profiledFunctionName);
        }
    }
}

void SymbolTable::Import(const SymbolTable& symbolTable, Context* context)
{
    globalNs.Import(const_cast<NamespaceSymbol*>(&symbolTable.globalNs), *this, context);
    for (const auto& pair : symbolTable.typeIdMap)
    {
        Symbol* typeOrConcept = pair.second;
        if (typeOrConcept->IsTypeSymbol())
        {
            TypeSymbol* type = static_cast<TypeSymbol*>(typeOrConcept);
            typeIdMap[type->TypeId()] = type;
            typeNameMap[type->FullName()] = type;
        }
        else if (typeOrConcept->GetSymbolType() == SymbolType::conceptSymbol)
        {
            ConceptSymbol* conceptSymbol = static_cast<ConceptSymbol*>(typeOrConcept);
            typeIdMap[conceptSymbol->TypeId()] = conceptSymbol;
        }
        else
        {
            Assert(false, "type or concept symbol expected"); 
        }
    }
    for (const auto& pair : symbolTable.functionIdMap)
    {
        FunctionSymbol* function = pair.second;
        functionIdMap[function->FunctionId()] = function;
    }
    for (const auto& p : symbolTable.derivedTypeMap)
    {
        derivedTypeMap[p.first] = p.second;
    }
    for (const auto& p : symbolTable.classTemplateSpecializationMap)
    {
        classTemplateSpecializationMap[p.first] = p.second;
    }
    for (const auto& p : symbolTable.arrayTypeMap)
    {
        arrayTypeMap[p.first] = p.second;
    }
    conversionTable.Add(symbolTable.conversionTable);
    for (ClassTypeSymbol* polymorphicClass : symbolTable.PolymorphicClasses())
    {
        AddPolymorphicClass(polymorphicClass);
    }
    for (ClassTypeSymbol* classHavingStaticConstructor : symbolTable.ClassesHavingStaticConstructor())
    {
        AddClassHavingStaticConstructor(classHavingStaticConstructor);
    }
    for (const std::u32string& jsonClass : symbolTable.JsonClasses())
    {
        AddJsonClass(jsonClass);
    }
    if (GetGlobalFlag(GlobalFlags::profile))
    {
        for (const auto& p : symbolTable.profiledFunctionNameMap)
        {
            const util::uuid& functionId = p.first;
            const std::u32string& profiledFunctionName = p.second;
            MapProfiledFunction(functionId, profiledFunctionName);
        }
    }
}

void SymbolTable::FinishRead(const std::vector<ArrayTypeSymbol*>& arrayTypes, const std::vector<DerivedTypeSymbol*>& derivedTypes,
    const std::vector<ClassTemplateSpecializationSymbol*>& classTemplateSpecializations, const std::vector<TypeOrConceptRequest>& typeAndConceptRequests,
    const std::vector<FunctionRequest>& functionRequests, std::vector<FunctionSymbol*>& conversions)
{
    ProcessTypeConceptAndFunctionRequests(typeAndConceptRequests, functionRequests);
    for (FunctionSymbol* conversion : conversions)
    {
        AddConversion(conversion, module);
    }
    for (ArrayTypeSymbol* arrayTypeSymbol : arrayTypes)
    {
        ArrayKey key(arrayTypeSymbol->ElementType(), arrayTypeSymbol->Size());
        arrayTypeMap[key] = arrayTypeSymbol;
    }
    for (DerivedTypeSymbol* derivedTypeSymbol : derivedTypes)
    {
        std::vector<DerivedTypeSymbol*>& derivedTypeVec = derivedTypeMap[derivedTypeSymbol->BaseType()->TypeId()];
        int n = derivedTypeVec.size();
        bool found = false;
        for (int i = 0; i < n; ++i)
        {
            DerivedTypeSymbol* prevDerivedTypeSymbol = derivedTypeVec[i];
            if (prevDerivedTypeSymbol->DerivationRec() == derivedTypeSymbol->DerivationRec())
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            derivedTypeVec.push_back(derivedTypeSymbol);
        }
    }
    AddClassTemplateSpecializationsToClassTemplateSpecializationMap(classTemplateSpecializations);
}

void SymbolTable::Clear()
{
    globalNs.Clear();
    typeIdMap.clear();
    functionIdMap.clear();
    typeNameMap.clear();
    profiledFunctionNameMap.clear();
}

void SymbolTable::BeginContainer(ContainerSymbol* container_)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module->IsImmutable())
    {
        throw ModuleImmutableException(module->GetContext()->RootModule(), module, container_->GetFullSpan(), soul::ast::FullSpan());
    }
#endif
    containerStack.push(container);
    container = container_;
}

void SymbolTable::EndContainer()
{
    container = containerStack.top();
    containerStack.pop();
}

void SymbolTable::MapNs(NamespaceSymbol* fromNs, NamespaceSymbol* toNs)
{
    nsMap[fromNs] = toNs;
}

NamespaceSymbol* SymbolTable::GetMappedNs(NamespaceSymbol* fromNs) const
{
    auto it = nsMap.find(fromNs);
    if (it != nsMap.cend())
    {
        return it->second;
    }
    return nullptr;
}

NamespaceSymbol* SymbolTable::BeginNamespace(cmajor::ast::NamespaceNode& namespaceNode, Context* context)
{
    std::u32string nsName = namespaceNode.Id()->Str();
    NamespaceSymbol* ns = BeginNamespace(nsName, namespaceNode.GetSpan(), namespaceNode.ModuleId(), namespaceNode.FileIndex(), context);
    MapNode(&namespaceNode, container);
    return ns;
}

NamespaceSymbol* SymbolTable::BeginNamespace(const std::u32string& namespaceName, const soul::ast::Span& span, const util::uuid& moduleId, int32_t fileIndex, Context* context)
{
    if (namespaceName.empty())
    {
        globalNs.SetSpan(span);
        globalNs.SetModuleId(moduleId);
        globalNs.SetFileIndex(fileIndex);
        BeginContainer(&globalNs);
        return &globalNs;
    }
    else
    {
        Symbol* symbol = container->GetContainerScope()->Lookup(namespaceName, context);
        if (symbol)
        {
            if (symbol->GetSymbolType() == SymbolType::namespaceSymbol)
            {
                NamespaceSymbol* ns = static_cast<NamespaceSymbol*>(symbol);
                BeginContainer(ns);
                return ns;
            }
            else
            {
                throw Exception("symbol '" + util::ToUtf8(symbol->Name()) + "' does not denote a namespace", symbol->GetFullSpan());
            }
        }
        else
        {
            NamespaceSymbol* ns = container->GetContainerScope()->CreateNamespace(namespaceName, span, moduleId, fileIndex, context);
            BeginContainer(ns);
            return ns;
        }
    }
}

void SymbolTable::EndNamespace()
{
    EndContainer();
}

void SymbolTable::BeginFunction(cmajor::ast::FunctionNode& functionNode, int32_t functionIndex)
{
    FunctionSymbol* functionSymbol = new FunctionSymbol(functionNode.GetSpan(), functionNode.GroupId());
    SetFunctionIdFor(functionSymbol);
    functionSymbol->SetIndex(functionIndex);
    if ((functionNode.GetSpecifiers() & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        functionSymbol->SetConstExpr();
    }
    if ((functionNode.GetSpecifiers() & cmajor::ast::Specifiers::winapi) != cmajor::ast::Specifiers::none)
    {
        functionSymbol->SetFlag(FunctionSymbolFlags::winapi);
    }
    functionSymbol->SetHasSource();
    if (instantiatingTemplate)
    {
        functionSymbol->SetFileIndex(currentFileIndex);
        functionSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        functionSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        functionSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    functionSymbol->SetCompileUnit(currentCompileUnit);
    functionSymbol->SetModule(module);
    functionSymbol->SetGroupName(functionNode.GroupId());
    if (functionNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        functionSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(functionNode.WhereConstraint()->Clone(cloneContext)));
    }
    if (functionSymbol->GroupName() == U"main" || functionSymbol->GroupName() == U"wmain" || functionSymbol->GroupName() == U"WinMain" || functionSymbol->GroupName() == U"wWinMain")
    {
        if (functionNode.IsProgramMain())
        {
            functionSymbol->SetCDecl();
            functionSymbol->SetProgramMain();
        }
        else
        {
            if (mainFunctionSymbol)
            {
                throw Exception("already has main function", functionNode.GetFullSpan(), mainFunctionSymbol->GetFullSpan());
            }
            else
            {
                mainFunctionSymbol = functionSymbol;
            }
        }
    }
    MapNode(&functionNode, functionSymbol);
    BeginContainer(functionSymbol);
    parameterIndex = 0;
    ResetDeclarationBlockIndex();
}

void SymbolTable::EndFunction(bool addMember, Context* context)
{
    FunctionSymbol* functionSymbol = static_cast<FunctionSymbol*>(container);
    EndContainer();
    if (addMember)
    {
        container->AddMember(functionSymbol, context);
    }
    else
    {
        createdFunctionSymbol = functionSymbol;
    }
}

void SymbolTable::AddParameter(cmajor::ast::ParameterNode& parameterNode, Context* context)
{
    bool artificialId = false;
    std::u32string parameterName = util::ToUtf32("@p" + std::to_string(parameterIndex));
    if (parameterNode.Id())
    {
        parameterName = parameterNode.Id()->Str();
    }
    else
    {
        parameterNode.SetId(new cmajor::ast::IdentifierNode(parameterNode.GetSpan(), parameterName));
        artificialId = true;
    }
    ParameterSymbol* parameterSymbol = new ParameterSymbol(parameterNode.GetSpan(), parameterName);
    if (instantiatingTemplate)
    {
        parameterSymbol->SetFileIndex(currentFileIndex);
        parameterSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        parameterSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        parameterSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    parameterSymbol->SetCompileUnit(currentCompileUnit);
    MapNode(&parameterNode, parameterSymbol);
    container->AddMember(parameterSymbol, context);
    ++parameterIndex;
    if (artificialId)
    {
        parameterSymbol->SetArtificialName();
    }
}

void SymbolTable::BeginClass(cmajor::ast::ClassNode& classNode)
{
    ClassTypeSymbol* classTypeSymbol = new ClassTypeSymbol(classNode.GetSpan(), classNode.Id()->Str());
    classTypeSymbol->SetGroupName(classNode.Id()->Str());
    currentClassStack.push(currentClass);
    currentClass = classTypeSymbol;
    if (instantiatingTemplate)
    {
        classTypeSymbol->SetFileIndex(currentFileIndex);
        classTypeSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        classTypeSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        classTypeSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    classTypeSymbol->SetCompileUnit(currentCompileUnit);
    classTypeSymbol->SetModule(module);
    MapNode(&classNode, classTypeSymbol);
    SetTypeIdFor(classTypeSymbol);
    BeginContainer(classTypeSymbol);
}

void SymbolTable::EndClass(Context* context)
{
    ClassTypeSymbol* classTypeSymbol = currentClass;
    currentClass = currentClassStack.top();
    currentClassStack.pop();
    EndContainer();
    classTypeSymbol->ComputeMinArity();
    container->AddMember(classTypeSymbol, context);
}

void SymbolTable::BeginClassTemplateSpecialization(cmajor::ast::ClassNode& classInstanceNode, ClassTemplateSpecializationSymbol* classTemplateSpecialization)
{
    currentClassStack.push(currentClass);
    currentClass = classTemplateSpecialization;
    MapNode(&classInstanceNode, classTemplateSpecialization);
    if (classTemplateSpecialization->TypeIdNotSet())
    {
        SetTypeIdFor(classTemplateSpecialization);
    }
    BeginContainer(classTemplateSpecialization);
}

void SymbolTable::EndClassTemplateSpecialization()
{
    EndContainer();
    currentClass = currentClassStack.top();
    currentClassStack.pop();
}

void SymbolTable::AddTemplateParameter(cmajor::ast::TemplateParameterNode& templateParameterNode, Context* context)
{
    TemplateParameterSymbol* templateParameterSymbol = new TemplateParameterSymbol(templateParameterNode.GetSpan(), templateParameterNode.Id()->Str());
    if (templateParameterNode.DefaultTemplateArgument())
    {
        templateParameterSymbol->SetHasDefault();
    }
    templateParameterSymbol->SetCompileUnit(currentCompileUnit);
    templateParameterSymbol->SetModule(module);
    SetTypeIdFor(templateParameterSymbol);
    MapNode(&templateParameterNode, templateParameterSymbol);
    container->AddMember(templateParameterSymbol, context);
}

void SymbolTable::AddTemplateParameter(cmajor::ast::IdentifierNode& identifierNode, Context* context)
{
    TemplateParameterSymbol* templateParameterSymbol = new TemplateParameterSymbol(identifierNode.GetSpan(), identifierNode.Str());
    templateParameterSymbol->SetCompileUnit(currentCompileUnit);
    templateParameterSymbol->SetModule(module);
    SetTypeIdFor(templateParameterSymbol);
    MapNode(&identifierNode, templateParameterSymbol);
    container->AddMember(templateParameterSymbol, context);
}

void SymbolTable::BeginInterface(cmajor::ast::InterfaceNode& interfaceNode, Context* context)
{
    InterfaceTypeSymbol* interfaceTypeSymbol = new InterfaceTypeSymbol(interfaceNode.GetSpan(), interfaceNode.Id()->Str());
    currentInterfaceStack.push(currentInterface);
    currentInterface = interfaceTypeSymbol;
    if (instantiatingTemplate)
    {
        interfaceTypeSymbol->SetFileIndex(currentFileIndex);
        interfaceTypeSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        interfaceTypeSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        interfaceTypeSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    interfaceTypeSymbol->SetCompileUnit(currentCompileUnit);
    interfaceTypeSymbol->SetModule(module);
    MapNode(&interfaceNode, interfaceTypeSymbol);
    SetTypeIdFor(interfaceTypeSymbol);
    container->AddMember(interfaceTypeSymbol, context);
    BeginContainer(interfaceTypeSymbol);
}

void SymbolTable::EndInterface()
{
    currentInterface = currentInterfaceStack.top();
    currentInterfaceStack.pop();
    EndContainer();
}

void SymbolTable::BeginStaticConstructor(cmajor::ast::StaticConstructorNode& staticConstructorNode, int32_t functionIndex)
{
    StaticConstructorSymbol* staticConstructorSymbol = new StaticConstructorSymbol(staticConstructorNode.GetSpan(), U"@static_constructor");
    staticConstructorSymbol->SetIndex(functionIndex);
    SetFunctionIdFor(staticConstructorSymbol);
    staticConstructorSymbol->SetHasSource();
    if (instantiatingTemplate)
    {
        staticConstructorSymbol->SetFileIndex(currentFileIndex);
        staticConstructorSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        staticConstructorSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        staticConstructorSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    staticConstructorSymbol->SetCompileUnit(currentCompileUnit);
    staticConstructorSymbol->SetModule(module);
    if (staticConstructorNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        staticConstructorSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(staticConstructorNode.WhereConstraint()->Clone(cloneContext)));
    }
    MapNode(&staticConstructorNode, staticConstructorSymbol);
    BeginContainer(staticConstructorSymbol);
    ResetDeclarationBlockIndex();
}

void SymbolTable::EndStaticConstructor(bool addMember, Context* context)
{
    StaticConstructorSymbol* staticConstructorSymbol = static_cast<StaticConstructorSymbol*>(container);
    EndContainer();
    if (addMember)
    {
        container->AddMember(staticConstructorSymbol, context);
    }
    else
    {
        createdFunctionSymbol = staticConstructorSymbol;
    }
}

void SymbolTable::BeginConstructor(cmajor::ast::ConstructorNode& constructorNode, int32_t functionIndex, Context* context)
{
    ConstructorSymbol* constructorSymbol = new ConstructorSymbol(constructorNode.GetSpan(), U"@constructor");
    constructorSymbol->SetIndex(functionIndex);
    SetFunctionIdFor(constructorSymbol);
    if ((constructorNode.GetSpecifiers() & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        constructorSymbol->SetConstExpr();
    }
    constructorSymbol->SetHasSource();
    if (instantiatingTemplate)
    {
        constructorSymbol->SetFileIndex(currentFileIndex);
        constructorSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        constructorSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        constructorSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    constructorSymbol->SetCompileUnit(currentCompileUnit);
    constructorSymbol->SetModule(module);
    if (constructorNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        constructorSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(constructorNode.WhereConstraint()->Clone(cloneContext)));
    }
    MapNode(&constructorNode, constructorSymbol);
    BeginContainer(constructorSymbol);
    parameterIndex = 0;
    ResetDeclarationBlockIndex();
    ParameterSymbol* thisParam = new ParameterSymbol(constructorNode.GetSpan(), U"this");
    TypeSymbol* thisParamType = nullptr;
    if (currentClass)
    {
        thisParamType = currentClass->AddPointer(context);
        thisParam->SetType(thisParamType);
        thisParam->SetBound();
        constructorSymbol->AddMember(thisParam, context);
    }
    else if (currentInterface)
    {
        throw Exception("interface type cannot have a constructor", constructorNode.GetFullSpan());
    }
}

void SymbolTable::EndConstructor(bool addMember, Context* context)
{
    ConstructorSymbol* constructorSymbol = static_cast<ConstructorSymbol*>(container);
    EndContainer();
    if (addMember)
    {
        container->AddMember(constructorSymbol, context);
    }
    else
    {
        createdFunctionSymbol = constructorSymbol;
    }
}

void SymbolTable::BeginDestructor(cmajor::ast::DestructorNode& destructorNode, int32_t functionIndex, Context* context)
{
    DestructorSymbol* destructorSymbol = new DestructorSymbol(destructorNode.GetSpan(), U"@destructor");
    destructorSymbol->SetIndex(functionIndex);
    SetFunctionIdFor(destructorSymbol);
    destructorSymbol->SetHasSource();
    if (instantiatingTemplate)
    {
        destructorSymbol->SetFileIndex(currentFileIndex);
        destructorSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        destructorSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        destructorSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    destructorSymbol->SetCompileUnit(currentCompileUnit);
    destructorSymbol->SetModule(module);
    if (destructorNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        destructorSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(destructorNode.WhereConstraint()->Clone(cloneContext)));
    }
    MapNode(&destructorNode, destructorSymbol);
    BeginContainer(destructorSymbol);
    ResetDeclarationBlockIndex();
    ParameterSymbol* thisParam = new ParameterSymbol(destructorNode.GetSpan(), U"this");
    TypeSymbol* thisParamType = nullptr;
    if (currentClass)
    {
        thisParamType = currentClass->AddPointer(context);
        thisParam->SetType(thisParamType);
        thisParam->SetBound();
        destructorSymbol->AddMember(thisParam, context);
    }
    else if (currentInterface)
    {
        throw Exception("interface type cannot have a destructor", destructorNode.GetFullSpan());
    }
}

void SymbolTable::EndDestructor(bool addMember, Context* context)
{
    DestructorSymbol* destructorSymbol = static_cast<DestructorSymbol*>(container);
    EndContainer();
    if (addMember)
    {
        container->AddMember(destructorSymbol, context);
    }
    else
    {
        createdFunctionSymbol = destructorSymbol;
    }
}

void SymbolTable::BeginMemberFunction(cmajor::ast::MemberFunctionNode& memberFunctionNode, int32_t functionIndex, Context* context)
{
    MemberFunctionSymbol* memberFunctionSymbol = new MemberFunctionSymbol(memberFunctionNode.GetSpan(), memberFunctionNode.GroupId());
    memberFunctionSymbol->SetIndex(functionIndex);
    SetFunctionIdFor(memberFunctionSymbol);
    if ((memberFunctionNode.GetSpecifiers() & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        memberFunctionSymbol->SetConstExpr();
    }
    memberFunctionSymbol->SetHasSource();
    if (instantiatingTemplate)
    {
        memberFunctionSymbol->SetFileIndex(currentFileIndex);
        memberFunctionSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        memberFunctionSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        memberFunctionSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    memberFunctionSymbol->SetCompileUnit(currentCompileUnit);
    memberFunctionSymbol->SetModule(module);
    memberFunctionSymbol->SetGroupName(memberFunctionNode.GroupId());
    if (memberFunctionNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        memberFunctionSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(memberFunctionNode.WhereConstraint()->Clone(cloneContext)));
    }
    MapNode(&memberFunctionNode, memberFunctionSymbol);
    BeginContainer(memberFunctionSymbol);
    parameterIndex = 0;
    ResetDeclarationBlockIndex();
    if ((memberFunctionNode.GetSpecifiers() & cmajor::ast::Specifiers::static_) == cmajor::ast::Specifiers::none)
    {
        ParameterSymbol* thisParam = new ParameterSymbol(memberFunctionNode.GetSpan(), U"this");
        TypeSymbol* thisParamType = nullptr;
        if (currentClass)
        {
            if (memberFunctionNode.IsConst())
            {
                thisParamType = currentClass->AddConst(context)->AddPointer(context);
            }
            else
            {
                thisParamType = currentClass->AddPointer(context);
            }
        }
        else if (currentInterface)
        {
            thisParamType = currentInterface->AddPointer(context);
        }
        else
        {
            Assert(false, "class or interface expected"); 
        }
        thisParam->SetType(thisParamType);
        thisParam->SetBound();
        memberFunctionSymbol->AddMember(thisParam, context);
    }
}

void SymbolTable::EndMemberFunction(bool addMember, Context* context)
{
    MemberFunctionSymbol* memberFunctionSymbol = static_cast<MemberFunctionSymbol*>(container);
    EndContainer();
    if (addMember)
    {
        container->AddMember(memberFunctionSymbol, context);
    }
    else
    {
        createdFunctionSymbol = memberFunctionSymbol;
    }
}

void SymbolTable::BeginConversionFunction(cmajor::ast::ConversionFunctionNode& conversionFunctionNode, int32_t functionIndex, Context* context)
{
    ConversionFunctionSymbol* conversionFunctionSymbol = new ConversionFunctionSymbol(conversionFunctionNode.GetSpan(), U"@conversion");
    conversionFunctionSymbol->SetIndex(functionIndex);
    SetFunctionIdFor(conversionFunctionSymbol);
    if ((conversionFunctionNode.GetSpecifiers() & cmajor::ast::Specifiers::constexpr_) != cmajor::ast::Specifiers::none)
    {
        conversionFunctionSymbol->SetConstExpr();
    }
    conversionFunctionSymbol->SetHasSource();
    if (instantiatingTemplate)
    {
        conversionFunctionSymbol->SetFileIndex(currentFileIndex);
        conversionFunctionSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        conversionFunctionSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        conversionFunctionSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    conversionFunctionSymbol->SetCompileUnit(currentCompileUnit);
    conversionFunctionSymbol->SetModule(module);
    conversionFunctionSymbol->SetGroupName(U"@operator_conv");
    if (conversionFunctionNode.WhereConstraint())
    {
        cmajor::ast::CloneContext cloneContext;
        conversionFunctionSymbol->SetConstraint(static_cast<cmajor::ast::WhereConstraintNode*>(conversionFunctionNode.WhereConstraint()->Clone(cloneContext)));
    }
    MapNode(&conversionFunctionNode, conversionFunctionSymbol);
    BeginContainer(conversionFunctionSymbol);
    ResetDeclarationBlockIndex();
    ParameterSymbol* thisParam = new ParameterSymbol(conversionFunctionNode.GetSpan(), U"this");
    TypeSymbol* thisParamType = nullptr;
    if (conversionFunctionNode.IsConst())
    {
        thisParamType = currentClass->AddConst(context)->AddPointer(context);
    }
    else
    {
        thisParamType = currentClass->AddPointer(context);
    }
    thisParam->SetType(thisParamType);
    thisParam->SetBound();
    conversionFunctionSymbol->AddMember(thisParam, context);
}

void SymbolTable::EndConversionFunction(bool addMember, Context* context)
{
    ConversionFunctionSymbol* conversionFunctionSymbol = static_cast<ConversionFunctionSymbol*>(container);
    EndContainer();
    if (addMember)
    {
        container->AddMember(conversionFunctionSymbol, context);
    }
    else
    {
        createdFunctionSymbol = conversionFunctionSymbol;
    }
}

void SymbolTable::AddMemberVariable(cmajor::ast::MemberVariableNode& memberVariableNode, Context* context)
{
    MemberVariableSymbol* memberVariableSymbol = new MemberVariableSymbol(memberVariableNode.GetSpan(), memberVariableNode.Id()->Str());
    if ((memberVariableNode.GetSpecifiers() & cmajor::ast::Specifiers::static_) != cmajor::ast::Specifiers::none)
    {
        memberVariableSymbol->SetStatic();
    }
    if (instantiatingTemplate)
    {
        memberVariableSymbol->SetFileIndex(currentFileIndex);
        memberVariableSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        memberVariableSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        memberVariableSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    memberVariableSymbol->SetCompileUnit(currentCompileUnit);
    MapNode(&memberVariableNode, memberVariableSymbol);
    container->AddMember(memberVariableSymbol, context);
}

void SymbolTable::BeginDelegate(cmajor::ast::DelegateNode& delegateNode, Context* context)
{
    DelegateTypeSymbol* delegateTypeSymbol = new DelegateTypeSymbol(delegateNode.GetSpan(), delegateNode.Id()->Str());
    if (instantiatingTemplate)
    {
        delegateTypeSymbol->SetFileIndex(currentFileIndex);
        delegateTypeSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        delegateTypeSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        delegateTypeSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    delegateTypeSymbol->SetCompileUnit(currentCompileUnit);
    delegateTypeSymbol->SetModule(module);
    MapNode(&delegateNode, delegateTypeSymbol);
    SetTypeIdFor(delegateTypeSymbol);
    container->AddMember(delegateTypeSymbol, context);
    BeginContainer(delegateTypeSymbol);
    parameterIndex = 0;
}

void SymbolTable::EndDelegate()
{
    EndContainer();
}

void SymbolTable::BeginClassDelegate(cmajor::ast::ClassDelegateNode& classDelegateNode, Context* context)
{
    ClassDelegateTypeSymbol* classDelegateTypeSymbol = new ClassDelegateTypeSymbol(classDelegateNode.GetSpan(), classDelegateNode.Id()->Str());
    if (instantiatingTemplate)
    {
        classDelegateTypeSymbol->SetFileIndex(currentFileIndex);
        classDelegateTypeSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        classDelegateTypeSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        classDelegateTypeSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    classDelegateTypeSymbol->SetCompileUnit(currentCompileUnit);
    classDelegateTypeSymbol->SetModule(module);
    MapNode(&classDelegateNode, classDelegateTypeSymbol);
    SetTypeIdFor(classDelegateTypeSymbol);
    container->AddMember(classDelegateTypeSymbol, context);
    BeginContainer(classDelegateTypeSymbol);
    parameterIndex = 0;
}

void SymbolTable::EndClassDelegate()
{
    EndContainer();
}

void SymbolTable::BeginConcept(cmajor::ast::ConceptNode& conceptNode, bool hasSource)
{
    ConceptSymbol* conceptSymbol = new ConceptSymbol(conceptNode.GetSpan(), conceptNode.Id()->Str());
    if (hasSource)
    {
        conceptSymbol->SetHasSource();
    }
    conceptSymbol->SetGroupName(conceptNode.Id()->Str());
    if (instantiatingTemplate)
    {
        conceptSymbol->SetFileIndex(currentFileIndex);
        conceptSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        conceptSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        conceptSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    conceptSymbol->SetCompileUnit(currentCompileUnit);
    conceptSymbol->SetModule(module);
    MapNode(&conceptNode, conceptSymbol);
    SetTypeIdFor(conceptSymbol);
    BeginContainer(conceptSymbol);
}

void SymbolTable::EndConcept(Context* context)
{
    ConceptSymbol* conceptSymbol = static_cast<ConceptSymbol*>(container);
    EndContainer();
    container->AddMember(conceptSymbol, context);
}

void SymbolTable::BeginAxiom(cmajor::ast::AxiomNode& axiomNode)
{
    std::u32string axiomId;
    if (axiomNode.Id())
    {
        axiomId = axiomNode.Id()->Str();
    }
    else
    {
        axiomId = util::ToUtf32("@axiom" + std::to_string(NextAxiomNumber()));
    }
    AxiomSymbol* axiomSymbol = new AxiomSymbol(axiomNode.GetSpan(), axiomId);
    if (instantiatingTemplate)
    {
        axiomSymbol->SetFileIndex(currentFileIndex);
        axiomSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        axiomSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        axiomSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    axiomSymbol->SetCompileUnit(currentCompileUnit);
    MapNode(&axiomNode, axiomSymbol);
    BeginContainer(axiomSymbol);
}

void SymbolTable::EndAxiom(Context* context)
{
    AxiomSymbol* axiomSymbol = static_cast<AxiomSymbol*>(container);
    EndContainer();
    container->AddMember(axiomSymbol, context);
}

void SymbolTable::BeginDeclarationBlock(cmajor::ast::Node& node, Context* context)
{
    DeclarationBlock* declarationBlock = new DeclarationBlock(node.GetSpan(), U"@locals" + util::ToUtf32(std::to_string(GetNextDeclarationBlockIndex())));
    if (instantiatingTemplate)
    {
        declarationBlock->SetFileIndex(currentFileIndex);
        declarationBlock->SetModuleId(currentModuleId);
    }
    else
    {
        declarationBlock->SetFileIndex(currentCompileUnit->FileIndex());
        declarationBlock->SetModuleId(currentCompileUnit->ModuleId());
    }
    declarationBlock->SetCompileUnit(currentCompileUnit);
    declarationBlock->SetModule(module);
    MapNode(&node, declarationBlock);
    container->AddMember(declarationBlock, context);
    BeginContainer(declarationBlock);
}

void SymbolTable::EndDeclarationBlock()
{
    EndContainer();
}

void SymbolTable::AddLocalVariable(cmajor::ast::ConstructionStatementNode& constructionStatementNode, Context* context)
{
    if (!constructionStatementNode.Id()) return;
    if (GetGlobalFlag(GlobalFlags::profile) && constructionStatementNode.Id()->Str() == U"@functionProfiler")
    {
        for (LocalVariableSymbol* localVariableSymbol : currentFunctionSymbol->LocalVariables())
        {
            if (localVariableSymbol->Name() == U"@functionProfiler")
            {
                MapNode(&constructionStatementNode, localVariableSymbol);
                return;
            }
        }
    }
    LocalVariableSymbol* localVariableSymbol = new LocalVariableSymbol(constructionStatementNode.GetSpan(), constructionStatementNode.Id()->Str());
    if (instantiatingTemplate)
    {
        localVariableSymbol->SetFileIndex(currentFileIndex);
        localVariableSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        localVariableSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        localVariableSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    localVariableSymbol->SetCompileUnit(currentCompileUnit);
    localVariableSymbol->SetModule(module);
    MapNode(&constructionStatementNode, localVariableSymbol);
    container->AddMember(localVariableSymbol, context);
}

void SymbolTable::AddLocalVariable(cmajor::ast::IdentifierNode& identifierNode, Context* context)
{
    LocalVariableSymbol* localVariableSymbol = new LocalVariableSymbol(identifierNode.GetSpan(), identifierNode.Str());
    localVariableSymbol->SetCompileUnit(currentCompileUnit);
    localVariableSymbol->SetModule(module);
    MapNode(&identifierNode, localVariableSymbol);
    container->AddMember(localVariableSymbol, context);
}

AliasTypeSymbol* SymbolTable::AddAliasType(cmajor::ast::AliasNode& aliasNode, Context* context)
{
    AliasTypeSymbol* aliasTypeSymbol = new AliasTypeSymbol(aliasNode.GetSpan(), aliasNode.Id()->Str());
    if (instantiatingTemplate)
    {
        aliasTypeSymbol->SetFileIndex(currentFileIndex);
        aliasTypeSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        aliasTypeSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        aliasTypeSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    aliasTypeSymbol->SetCompileUnit(currentCompileUnit);
    aliasTypeSymbol->SetModule(module);
    MapNode(&aliasNode, aliasTypeSymbol);
    container->AddMember(aliasTypeSymbol, context);
    return aliasTypeSymbol;
}

AliasTypeSymbol* SymbolTable::AddAliasType(cmajor::ast::TypedefNode& typedefNode, Context* context)
{
    AliasTypeSymbol* aliasTypeSymbol = new AliasTypeSymbol(typedefNode.GetSpan(), typedefNode.Id()->Str());
    if (instantiatingTemplate)
    {
        aliasTypeSymbol->SetFileIndex(currentFileIndex);
        aliasTypeSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        aliasTypeSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        aliasTypeSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    aliasTypeSymbol->SetCompileUnit(currentCompileUnit);
    aliasTypeSymbol->SetModule(module);
    MapNode(&typedefNode, aliasTypeSymbol);
    container->AddMember(aliasTypeSymbol, context);
    return aliasTypeSymbol;
}

ConstantSymbol* SymbolTable::AddConstant(cmajor::ast::ConstantNode& constantNode, Context* context)
{
    ConstantSymbol* constantSymbol = new ConstantSymbol(constantNode.GetSpan(), constantNode.Id()->Str());
    if (instantiatingTemplate)
    {
        constantSymbol->SetFileIndex(currentFileIndex);
        constantSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        constantSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        constantSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    constantSymbol->SetCompileUnit(currentCompileUnit);
    constantSymbol->SetModule(module);
    constantSymbol->SetStrValue(constantNode.StrValue());
    MapNode(&constantNode, constantSymbol);
    container->AddMember(constantSymbol, context);
    return constantSymbol;
}

GlobalVariableSymbol* SymbolTable::AddGlobalVariable(cmajor::ast::GlobalVariableNode& globalVariableNode, Context* context)
{
    GlobalVariableSymbol* globalVariableSymbol = new GlobalVariableSymbol(globalVariableNode.GetSpan(), globalVariableNode.Id()->Str(), 
        globalVariableNode.CompileUnit()->Id(), globalVariableNode.CompileUnit()->FilePath());
    globalVariableSymbol->SetSpecifiers(globalVariableNode.GetSpecifiers());
    if (instantiatingTemplate)
    {
        globalVariableSymbol->SetFileIndex(currentFileIndex);
        globalVariableSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        globalVariableSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        globalVariableSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    globalVariableSymbol->SetCompileUnit(currentCompileUnit);
    globalVariableSymbol->SetModule(module);
    MapNode(&globalVariableNode, globalVariableSymbol);
    container->AddMember(globalVariableSymbol, context);
    return globalVariableSymbol;
}

void SymbolTable::BeginEnumType(cmajor::ast::EnumTypeNode& enumTypeNode, Context* context)
{
    EnumTypeSymbol* enumTypeSymbol = new EnumTypeSymbol(enumTypeNode.GetSpan(), enumTypeNode.Id()->Str());
    if (instantiatingTemplate)
    {
        enumTypeSymbol->SetFileIndex(currentFileIndex);
        enumTypeSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        enumTypeSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        enumTypeSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    enumTypeSymbol->SetCompileUnit(currentCompileUnit);
    enumTypeSymbol->SetModule(module);
    MapNode(&enumTypeNode, enumTypeSymbol);
    SetTypeIdFor(enumTypeSymbol);
    container->AddMember(enumTypeSymbol, context);
    BeginContainer(enumTypeSymbol);
}

void SymbolTable::EndEnumType()
{
    EndContainer();
}

void SymbolTable::AddEnumConstant(cmajor::ast::EnumConstantNode& enumConstantNode, Context* context)
{
    EnumConstantSymbol* enumConstantSymbol = new EnumConstantSymbol(enumConstantNode.GetSpan(), enumConstantNode.Id()->Str());
    if (instantiatingTemplate)
    {
        enumConstantSymbol->SetFileIndex(currentFileIndex);
        enumConstantSymbol->SetModuleId(currentModuleId);
    }
    else
    {
        enumConstantSymbol->SetFileIndex(currentCompileUnit->FileIndex());
        enumConstantSymbol->SetModuleId(currentCompileUnit->ModuleId());
    }
    enumConstantSymbol->SetCompileUnit(currentCompileUnit);
    enumConstantSymbol->SetModule(module);
    MapNode(&enumConstantNode, enumConstantSymbol);
    container->AddMember(enumConstantSymbol, context);
}

void SymbolTable::AddTypeSymbolToGlobalScope(TypeSymbol* typeSymbol, Context* context)
{
    typeSymbol->SetFileIndex(currentCompileUnit->FileIndex());
    typeSymbol->SetModuleId(currentCompileUnit->ModuleId());
    typeSymbol->SetCompileUnit(currentCompileUnit);
    typeSymbol->SetModule(module);
    SetTypeIdFor(typeSymbol);
    globalNs.AddMember(typeSymbol, context);
    typeNameMap[typeSymbol->FullName()] = typeSymbol;
}

void SymbolTable::AddTypeSymbol(TypeSymbol* typeSymbol, Context* context)
{
    typeSymbol->SetFileIndex(currentCompileUnit->FileIndex());
    typeSymbol->SetModuleId(currentCompileUnit->ModuleId());
    typeSymbol->SetCompileUnit(currentCompileUnit);
    typeSymbol->SetModule(module);
    SetTypeIdFor(typeSymbol);
    container->AddMember(typeSymbol, context);
    typeNameMap[typeSymbol->FullName()] = typeSymbol;
}

void SymbolTable::AddFunctionSymbolToGlobalScope(FunctionSymbol* functionSymbol, Context* context)
{
    functionSymbol->SetFileIndex(currentCompileUnit->FileIndex());
    functionSymbol->SetModuleId(currentCompileUnit->ModuleId());
    functionSymbol->SetCompileUnit(currentCompileUnit);
    SetFunctionIdFor(functionSymbol);
    functionSymbol->SetModule(module);
    globalNs.AddMember(functionSymbol, context);
    if (functionSymbol->IsConversion())
    {
        conversionTable.AddConversion(functionSymbol);
    }
}

void SymbolTable::MapNode(cmajor::ast::Node* node, Symbol* symbol)
{
    std::lock_guard<std::recursive_mutex> lock(module->Lock());
    nodeSymbolMap[node] = symbol;
    symbolNodeMap[symbol] = node;
}

Symbol* SymbolTable::GetSymbolNoThrow(cmajor::ast::Node* node) const
{
    std::lock_guard<std::recursive_mutex> lock(module->Lock());
    auto it = nodeSymbolMap.find(node);
    if (it != nodeSymbolMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

Symbol* SymbolTable::GetSymbol(cmajor::ast::Node* node) const
{
    Symbol* symbol = GetSymbolNoThrow(node);
    if (symbol)
    {
        return symbol;
    }
    else
    {
        throw std::runtime_error("symbol for node not found");
    }
}

cmajor::ast::Node* SymbolTable::GetNodeNoThrow(Symbol* symbol) const
{
    std::lock_guard<std::recursive_mutex> lock(module->Lock());
    auto it = symbolNodeMap.find(symbol);
    if (it != symbolNodeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

cmajor::ast::Node* SymbolTable::GetNode(Symbol* symbol) const
{
    cmajor::ast::Node* node = GetNodeNoThrow(symbol);
    if (node)
    {
        return node;
    }
    else
    {
        throw std::runtime_error("node for symbol not found");
    }
}

void SymbolTable::AddTypeOrConceptSymbolToTypeIdMap(Symbol* typeOrConceptSymbol)
{
    if (typeOrConceptSymbol->IsTypeSymbol())
    {
        TypeSymbol* typeSymbol = static_cast<TypeSymbol*>(typeOrConceptSymbol);
        typeIdMap[typeSymbol->TypeId()] = typeSymbol;
    }
    else if (typeOrConceptSymbol->GetSymbolType() == SymbolType::conceptSymbol)
    {
        ConceptSymbol* conceptSymbol = static_cast<ConceptSymbol*>(typeOrConceptSymbol);
        typeIdMap[conceptSymbol->TypeId()] = conceptSymbol;
    }
    else
    {
        Assert(false, "type or concept symbol expected"); 
    }
}

void SymbolTable::AddFunctionSymbolToFunctionIdMap(FunctionSymbol* functionSymbol)
{
    functionIdMap[functionSymbol->FunctionId()] = functionSymbol;
}

void SymbolTable::SetTypeIdFor(TypeSymbol* typeSymbol)
{
    typeSymbol->SetTypeId(util::random_uuid());
}

void SymbolTable::SetTypeIdFor(ConceptSymbol* conceptSymbol)
{
    conceptSymbol->SetTypeId(util::random_uuid());
}

void SymbolTable::SetFunctionIdFor(FunctionSymbol* functionSymbol)
{
    functionSymbol->SetFunctionId(util::random_uuid());
}

FunctionSymbol* SymbolTable::GetFunctionById(const util::uuid& functionId) const
{
    auto it = functionIdMap.find(functionId);
    if (it != functionIdMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::EmplaceTypeRequest(SymbolReader& reader, Symbol* forSymbol, const util::uuid& typeId, int index)
{
    EmplaceTypeOrConceptRequest(reader, forSymbol, typeId, index);
}

const int conceptRequestIndex = std::numeric_limits<int>::max();

void SymbolTable::EmplaceConceptRequest(SymbolReader& reader, Symbol* forSymbol, const util::uuid& typeId)
{
    EmplaceTypeOrConceptRequest(reader, forSymbol, typeId, conceptRequestIndex);
}

void SymbolTable::EmplaceTypeOrConceptRequest(SymbolReader& reader, Symbol* forSymbol, const util::uuid& typeId, int index)
{
    auto it = typeIdMap.find(typeId);
    if (it != typeIdMap.cend())
    {
        Symbol* typeOrConceptSymbol = it->second;
        if (typeOrConceptSymbol->IsTypeSymbol())
        {
            if (index == conceptRequestIndex)
            {
                throw Exception("internal error: invalid concept request (id denotes a type)", forSymbol->GetFullSpan());
            }
            TypeSymbol* typeSymbol = static_cast<TypeSymbol*>(typeOrConceptSymbol);
            forSymbol->EmplaceType(typeSymbol, index);
        }
        else if (typeOrConceptSymbol->GetSymbolType() == SymbolType::conceptSymbol)
        {
            if (index != conceptRequestIndex)
            {
                throw Exception("internal error: invalid type request (id denotes a concept)", forSymbol->GetFullSpan());
            }
            ConceptSymbol* conceptSymbol = static_cast<ConceptSymbol*>(typeOrConceptSymbol);
            forSymbol->EmplaceConcept(conceptSymbol);
        }
        else
        {
            Assert(false, "internal error: type or concept symbol expected"); 
        }
    }
    else
    {
        reader.AddTypeOrConceptRequest(TypeOrConceptRequest(forSymbol, typeId, index));
    }
}

void SymbolTable::EmplaceFunctionRequest(SymbolReader& reader, Symbol* forSymbol, const::util::uuid& functionId, int index)
{
    auto it = functionIdMap.find(functionId);
    if (it != functionIdMap.cend())
    {
        FunctionSymbol* functionSymbol = it->second;
        forSymbol->EmplaceFunction(functionSymbol, index);
    }
    else
    {
        reader.AddFunctionRequest(FunctionRequest(forSymbol, functionId, index));
    }
}

void SymbolTable::ProcessTypeConceptAndFunctionRequests(const std::vector<TypeOrConceptRequest>& typeAndConceptRequests, const std::vector<FunctionRequest>& functionRequests)
{
    for (const TypeOrConceptRequest& typeOrConceptRequest : typeAndConceptRequests)
    {
        Symbol* symbol = typeOrConceptRequest.symbol;
        auto it = typeIdMap.find(typeOrConceptRequest.typeId);
        if (it != typeIdMap.cend())
        {
            Symbol* typeOrConceptSymbol = it->second;
            int index = typeOrConceptRequest.index;
            if (typeOrConceptSymbol->IsTypeSymbol())
            {
                if (index == conceptRequestIndex)
                {
                    throw Exception("internal error: invalid concept request (id denotes a type)", symbol->GetFullSpan());
                }
                TypeSymbol* typeSymbol = static_cast<TypeSymbol*>(typeOrConceptSymbol);
                symbol->EmplaceType(typeSymbol, index);
            }
            else if (typeOrConceptSymbol->GetSymbolType() == SymbolType::conceptSymbol)
            {
                if (index != conceptRequestIndex)
                {
                    throw Exception("internal error: invalid type request (id denotes a concept)", symbol->GetFullSpan());
                }
                ConceptSymbol* conceptSymbol = static_cast<ConceptSymbol*>(typeOrConceptSymbol);
                symbol->EmplaceConcept(conceptSymbol);
            }
            else
            {
                Assert(false, "internal error: type or concept symbol expected"); 
            }
        }
        else
        {
            throw std::runtime_error("internal error: cannot satisfy type or concept request for symbol '" + util::ToUtf8(symbol->Name()) + "': type or concept not found from symbol table");
        }
    }
    for (const FunctionRequest& functionRequest : functionRequests)
    {
        Symbol* symbol = functionRequest.symbol;
        auto it = functionIdMap.find(functionRequest.functionId);
        if (it != functionIdMap.cend())
        {
            FunctionSymbol* functionSymbol = it->second;
            int index = functionRequest.index;
            symbol->EmplaceFunction(functionSymbol, index);
        }
        else
        {
            throw std::runtime_error("internal error: cannot satisfy function request for symbol '" + util::ToUtf8(symbol->Name()) + "': function not found from symbol table");
        }
    }
}

TypeSymbol* SymbolTable::GetTypeByNameNoThrow(const std::u32string& typeName) const
{
    auto it = typeNameMap.find(typeName);
    if (it != typeNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

TypeSymbol* SymbolTable::GetTypeByName(const std::u32string& typeName) const
{
    TypeSymbol* typeSymbol = GetTypeByNameNoThrow(typeName);
    if (typeSymbol)
    {
        return typeSymbol;
    }
    else
    {
        throw std::runtime_error("type '" + util::ToUtf8(typeName) + "' not found");
    }
}

TypeSymbol* SymbolTable::MakeDerivedType(TypeSymbol* baseType, const TypeDerivationRec& derivationRec, Context* context)
{
    if (!baseType)
    {
        throw Exception("base type is null!", soul::ast::FullSpan());
    }
#ifdef IMMUTABLE_MODULE_CHECK
    if (module->IsImmutable())
    {
        throw ModuleImmutableException(module, baseType->GetModule(), baseType->GetFullSpan(), soul::ast::FullSpan());
    }
#endif
    if (derivationRec.IsEmpty())
    {
        return baseType;
    }
    if (baseType->IsVoidType() && HasReferenceDerivation(derivationRec.derivations) && !HasPointerDerivation(derivationRec.derivations))
    {
        throw Exception("cannot have reference to void type", soul::ast::FullSpan());
    }
    std::lock_guard<std::recursive_mutex> lock(module->Lock());
    std::vector<DerivedTypeSymbol*>& mappedDerivedTypes = derivedTypeMap[baseType->TypeId()];
    int n = mappedDerivedTypes.size();
    for (int i = 0; i < n; ++i)
    {
        DerivedTypeSymbol* derivedType = mappedDerivedTypes[i];
        if (derivedType->DerivationRec() == derivationRec)
        {
            return derivedType;
        }
    }
    DerivedTypeSymbol* derivedType = new DerivedTypeSymbol(baseType->GetSpan(), MakeDerivedTypeName(baseType, derivationRec), baseType, derivationRec);
    derivedType->SetParent(&globalNs);
    derivedType->SetModule(module);
    derivedType->ComputeTypeId();
    mappedDerivedTypes.push_back(derivedType);
    derivedTypes.push_back(std::unique_ptr<DerivedTypeSymbol>(derivedType));
    if (derivedType->IsPointerType() && !derivedType->BaseType()->IsVoidType() && !derivedType->IsReferenceType())
    {
        AliasTypeSymbol* valueType = new AliasTypeSymbol(baseType->GetSpan(), U"ValueType");
        valueType->SetModule(module);
        valueType->SetAccess(SymbolAccess::public_);
        valueType->SetType(derivedType->RemovePointer(context));
        TypeSymbol* withoutConst = valueType->GetType()->RemoveConst(context);
        if (withoutConst->IsBasicTypeSymbol())
        {
            valueType->SetType(withoutConst);
        }
        valueType->SetBound();
        derivedType->AddMember(valueType, context);
        AliasTypeSymbol* referenceType = new AliasTypeSymbol(baseType->GetSpan(), U"ReferenceType");
        referenceType->SetModule(module);
        referenceType->SetAccess(SymbolAccess::public_);
        referenceType->SetType(valueType->GetType()->AddLvalueReference(context));
        referenceType->SetBound();
        derivedType->AddMember(referenceType, context);
        AliasTypeSymbol* pointerType = new AliasTypeSymbol(baseType->GetSpan(), U"PointerType");
        pointerType->SetModule(module);
        pointerType->SetAccess(SymbolAccess::public_);
        pointerType->SetType(derivedType);
        pointerType->SetBound();
        derivedType->AddMember(pointerType, context);
    }
    return derivedType;
}

ClassTemplateSpecializationSymbol* SymbolTable::MakeClassTemplateSpecialization(ClassTypeSymbol* classTemplate, const std::vector<TypeSymbol*>& templateArgumentTypes)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module->IsImmutable())
    {
        throw ModuleImmutableException(module->GetContext()->RootModule(), module, classTemplate->GetFullSpan(), soul::ast::FullSpan());
    }
#endif
    std::lock_guard<std::recursive_mutex> lock(GetModule()->Lock());
    ClassTemplateSpecializationKey key(classTemplate, templateArgumentTypes);
    auto it = classTemplateSpecializationMap.find(key);
    if (it != classTemplateSpecializationMap.cend())
    {
        ClassTemplateSpecializationSymbol* classTemplateSpecialization = it->second;
        return classTemplateSpecialization;
    }
    std::u32string classTemplateSpecializationName = MakeClassTemplateSpecializationName(classTemplate, templateArgumentTypes);
    ClassTemplateSpecializationSymbol* classTemplateSpecialization = new ClassTemplateSpecializationSymbol(classTemplate->GetSpan(), classTemplateSpecializationName, classTemplate,
        templateArgumentTypes);
    SetTypeIdFor(classTemplateSpecialization);
    classTemplateSpecialization->SetGroupName(classTemplate->GroupName());
    classTemplateSpecializationMap[key] = classTemplateSpecialization;
    classTemplateSpecialization->SetParent(&globalNs);
    classTemplateSpecialization->SetFileIndex(classTemplate->FileIndex());
    classTemplateSpecialization->SetModuleId(classTemplate->ModuleId());
    classTemplateSpecialization->SetModule(module);
    classTemplateSpecializations.push_back(std::unique_ptr<ClassTemplateSpecializationSymbol>(classTemplateSpecialization));
    ++numSpecializationsNew;
    return classTemplateSpecialization;
}

ClassTemplateSpecializationSymbol* SymbolTable::CopyClassTemplateSpecialization(ClassTemplateSpecializationSymbol* source)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module->IsImmutable())
    {
        throw ModuleImmutableException(module->GetContext()->RootModule(), module, source->GetFullSpan(), soul::ast::FullSpan());
    }
#endif
    std::lock_guard<std::recursive_mutex> lock(GetModule()->Lock());
    ClassTypeSymbol* classTemplate = source->GetClassTemplate();
    if (classTemplate == nullptr)
    {
        throw std::runtime_error("class template is null!");
    }
    std::vector<TypeSymbol*>& templateArgumentTypes = source->TemplateArgumentTypes();
    ClassTemplateSpecializationKey key(classTemplate, templateArgumentTypes);
    std::u32string classTemplateSpecializationName = MakeClassTemplateSpecializationName(classTemplate, templateArgumentTypes);
    ClassTemplateSpecializationSymbol* copy = new ClassTemplateSpecializationSymbol(source->GetSpan(), classTemplateSpecializationName, classTemplate, templateArgumentTypes);
    copy->SetTypeId(source->TypeId());
    copy->SetGroupName(classTemplate->GroupName());
    classTemplateSpecializationMap[key] = copy;
    copy->SetParent(&globalNs);
    copy->SetFileIndex(source->FileIndex());
    copy->SetModuleId(source->ModuleId());
    copy->SetModule(module);
    if (source->VmtEmitted())
    {
        copy->SetVmtEmitted();
    }
    if (source->HasFullInstantiation())
    {
        copy->SetHasFullInstantiation();
    }
    classTemplateSpecializations.push_back(std::unique_ptr<ClassTemplateSpecializationSymbol>(copy));
    derivedTypeMap[copy->TypeId()].clear();
    specializationCopyMap[source] = copy;
    ++numSpecializationsCopied;
    return copy;
}

ClassTemplateSpecializationSymbol* SymbolTable::GetCurrentClassTemplateSpecialization(ClassTemplateSpecializationSymbol* source)
{
    auto it = specializationCopyMap.find(source);
    if (it != specializationCopyMap.cend())
    {
        return it->second;
    }
    return source;
}

ArrayTypeSymbol* SymbolTable::MakeArrayType(TypeSymbol* elementType, int64_t size, Context* context)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module->IsImmutable())
    {
        throw ModuleImmutableException(module->GetContext()->RootModule(), module, elementType->GetFullSpan(), soul::ast::FullSpan());
    }
#endif
    ArrayKey key(elementType, size);
    auto it = arrayTypeMap.find(key);
    if (elementType->GetSymbolType() != SymbolType::classTemplateSpecializationSymbol && it != arrayTypeMap.cend())
    {
        ArrayTypeSymbol* arrayType = it->second;
        return arrayType;
    }
    ArrayTypeSymbol* arrayType = new ArrayTypeSymbol(elementType->GetSpan(), elementType->FullName() + U"[" + util::ToUtf32(std::to_string(size)) + U"]", elementType, size);
    SetTypeIdFor(arrayType);
    arrayTypeMap[key] = arrayType;
    arrayType->SetParent(&globalNs);
    arrayType->SetModule(module);
    ArrayLengthFunction* arrayLengthFunction = new ArrayLengthFunction(arrayType, context);
    SetFunctionIdFor(arrayLengthFunction);
    arrayType->AddMember(arrayLengthFunction, context);
    ArrayBeginFunction* arrayBeginFunction = new ArrayBeginFunction(arrayType, context);
    SetFunctionIdFor(arrayBeginFunction);
    arrayType->AddMember(arrayBeginFunction, context);
    ArrayEndFunction* arrayEndFunction = new ArrayEndFunction(arrayType, context);
    SetFunctionIdFor(arrayEndFunction);
    arrayType->AddMember(arrayEndFunction, context);
    ArrayCBeginFunction* arrayCBeginFunction = new ArrayCBeginFunction(arrayType, context);
    SetFunctionIdFor(arrayCBeginFunction);
    arrayType->AddMember(arrayCBeginFunction, context);
    ArrayCEndFunction* arrayCEndFunction = new ArrayCEndFunction(arrayType, context);
    SetFunctionIdFor(arrayCEndFunction);
    arrayType->AddMember(arrayCEndFunction, context);
    AliasTypeSymbol* iterator = new AliasTypeSymbol(elementType->GetSpan(), U"Iterator");
    iterator->SetModule(module);
    iterator->SetAccess(SymbolAccess::public_);
    iterator->SetType(arrayType->ElementType()->AddPointer(context));
    iterator->SetBound();
    arrayType->AddMember(iterator, context);
    AliasTypeSymbol* constIterator = new AliasTypeSymbol(elementType->GetSpan(), U"ConstIterator");
    constIterator->SetModule(module);
    constIterator->SetAccess(SymbolAccess::public_);
    constIterator->SetType(arrayType->ElementType()->AddConst(context)->AddPointer(context));
    constIterator->SetBound();
    arrayType->AddMember(constIterator, context);
    arrayTypes.push_back(std::unique_ptr<ArrayTypeSymbol>(arrayType));
    derivedTypeMap[arrayType->TypeId()].clear();
    return arrayType;
}

void SymbolTable::AddClassTemplateSpecializationsToClassTemplateSpecializationMap(const std::vector<ClassTemplateSpecializationSymbol*>& classTemplateSpecializations)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module->IsImmutable())
    {
        throw ModuleImmutableException(module->GetContext()->RootModule(), module, soul::ast::FullSpan(), soul::ast::FullSpan());
    }
#endif
    for (ClassTemplateSpecializationSymbol* classTemplateSpecialization : classTemplateSpecializations)
    {
        ClassTemplateSpecializationKey key(classTemplateSpecialization->GetClassTemplate(), classTemplateSpecialization->TemplateArgumentTypes());
        auto it = classTemplateSpecializationMap.find(key);
        if (it == classTemplateSpecializationMap.cend())
        {
            classTemplateSpecializationMap[key] = classTemplateSpecialization;
        }
    }
}

void SymbolTable::AddConversion(FunctionSymbol* conversion, Module* module)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module && module->IsImmutable())
    {
        throw ModuleImmutableException(module->GetContext()->RootModule(), module, conversion->GetFullSpan(), soul::ast::FullSpan());
    }
#endif
    std::lock_guard<std::recursive_mutex> lock(GetModule()->Lock());
    conversionTable.AddConversion(conversion);
}

void SymbolTable::AddConversion(FunctionSymbol* conversion)
{
    AddConversion(conversion, nullptr);
}

FunctionSymbol* SymbolTable::GetConversion(TypeSymbol* sourceType, TypeSymbol* targetType, Context* context) const
{
    std::lock_guard<std::recursive_mutex> lock(module->Lock());
    return conversionTable.GetConversion(sourceType, targetType, context);
}

void SymbolTable::AddPolymorphicClass(ClassTypeSymbol* polymorphicClass)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module->IsImmutable())
    {
        throw ModuleImmutableException(module->GetContext()->RootModule(), module, polymorphicClass->GetFullSpan(), soul::ast::FullSpan());
    }
#endif
    if (!polymorphicClass->IsPolymorphic())
    {
        throw Exception("not a polymorphic class", polymorphicClass->GetFullSpan());
    }
    polymorphicClasses.insert(polymorphicClass);
}

void SymbolTable::AddClassHavingStaticConstructor(ClassTypeSymbol* classHavingStaticConstructor)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module->IsImmutable())
    {
        throw ModuleImmutableException(module->GetContext()->RootModule(), module, classHavingStaticConstructor->GetFullSpan(), soul::ast::FullSpan());
    }
#endif
    if (!classHavingStaticConstructor->StaticConstructor())
    {
        throw Exception("not having static constructor", classHavingStaticConstructor->GetFullSpan());
    }
    classesHavingStaticConstructor.insert(classHavingStaticConstructor);
}

void SymbolTable::AddJsonClass(const std::u32string& jsonClass)
{
#ifdef IMMUTABLE_MODULE_CHECK
    if (module->IsImmutable())
    {
        throw ModuleImmutableException(module->GetContext()->RootModule(), module, soul::ast::FullSpan(), soul::ast::FullSpan());
    }
#endif
    jsonClasses.insert(jsonClass);
}

std::vector<TypeSymbol*> SymbolTable::Types() const
{
    std::vector<TypeSymbol*> types;
    for (const auto& p : typeNameMap)
    {
        TypeSymbol* type = p.second;
        types.push_back(type);
    }
    for (const std::unique_ptr<DerivedTypeSymbol>& dt : derivedTypes)
    {
        types.push_back(dt.get());
    }
    for (const std::unique_ptr<ClassTemplateSpecializationSymbol>& ts : classTemplateSpecializations)
    {
        types.push_back(ts.get());
    }
    return types;
}

void SymbolTable::Copy(const SymbolTable& that)
{
    for (const auto& p : that.typeIdMap)
    {
        typeIdMap[p.first] = p.second;
    }
    for (const auto& p : that.functionIdMap)
    {
        functionIdMap[p.first] = p.second;
    }
}

void SymbolTable::MapProfiledFunction(const util::uuid& functionId, const std::u32string& profiledFunctionName)
{
    profiledFunctionNameMap[functionId] = profiledFunctionName;
}

std::u32string SymbolTable::GetProfiledFunctionName(const util::uuid& functionId) const
{
    auto it = profiledFunctionNameMap.find(functionId);
    if (it != profiledFunctionNameMap.cend())
    {
        return it->second;
    }
    return std::u32string();
}

std::unique_ptr<soul::xml::Document> SymbolTable::ToDomDocument()
{
    TypeMap typeMap;
    std::unique_ptr<soul::xml::Document> doc(soul::xml::MakeDocument());
    std::unique_ptr<soul::xml::Element> st(soul::xml::MakeElement("symbolTable"));
    std::unique_ptr<soul::xml::Element> globalNsElement = globalNs.ToDomElement(typeMap);
    std::unique_ptr<soul::xml::Element> symbols(soul::xml::MakeElement("symbols"));
    symbols->AppendChild(globalNsElement.release());
    st->AppendChild(symbols.release());
    std::unique_ptr<soul::xml::Element> types(soul::xml::MakeElement("types"));
    std::vector<std::unique_ptr<soul::xml::Element>> typeElements = typeMap.TypeElements();
    int n = typeElements.size();
    for (int i = 0; i < n; ++i)
    {
        std::unique_ptr<soul::xml::Element> typeElement(std::move(typeElements[i]));
        types->AppendChild(typeElement.release());
    }
    st->AppendChild(types.release());
    doc->AppendChild(st.release());
    return doc;
}

void SymbolTable::MapInvoke(cmajor::ast::IdentifierNode* invokeId, FunctionSymbol* functionSymbol)
{
    invokeMap[invokeId] = functionSymbol;
}

FunctionSymbol* SymbolTable::GetInvoke(cmajor::ast::IdentifierNode* invokeId) const
{
    auto it = invokeMap.find(invokeId);
    if (it != invokeMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::MapSymbol(cmajor::ast::Node* node, Symbol* symbol)
{
    std::lock_guard<std::recursive_mutex> lock(module->Lock());
    mappedNodeSymbolMap[node] = symbol;
}

Symbol* SymbolTable::GetMappedSymbol(cmajor::ast::Node* node) const
{
    std::lock_guard<std::recursive_mutex> lock(module->Lock());
    auto it = mappedNodeSymbolMap.find(node);
    if (it != mappedNodeSymbolMap.cend())
    {
        return it->second;
    }
    else
    {
        return nullptr;
    }
}

void SymbolTable::MapIdentifierToSymbolDefinition(cmajor::ast::IdentifierNode* identifierNode, Module* module, Symbol* symbol)
{
    if (GetGlobalFlag(GlobalFlags::cmdoc)) return;
    std::lock_guard<std::recursive_mutex> lock(this->module->Lock());
    if (identifierSymbolDefinitionMap.find(identifierNode) != identifierSymbolDefinitionMap.cend()) return;
    identifierSymbolDefinitionMap[identifierNode] = symbol;
    SymbolLocation identifierLocation = MakeSymbolLocation(identifierNode->GetSpan(), module, identifierNode->FileIndex());
    SymbolLocation definitionLocation;
    if (symbol->GetLocation(definitionLocation))
    {
        symbolDefinitionMap[identifierLocation] = definitionLocation;
    }
}

SymbolLocation SymbolTable::GetDefinitionLocation(const SymbolLocation& identifierLocation) const
{
    auto it = symbolDefinitionMap.find(identifierLocation);
    if (it != symbolDefinitionMap.cend())
    {
        return it->second;
    }
    else
    {
        return SymbolLocation();
    }
}

void SymbolTable::InitUuids()
{
    derivationIds.clear();
    for (int i = 0; i < static_cast<int>(Derivation::max); ++i)
    {
        derivationIds.push_back(util::random_uuid());
    }
    positionIds.clear();
    for (int i = 0; i < util::uuid::static_size(); ++i)
    {
        positionIds.push_back(util::random_uuid());
    }
}

const util::uuid& SymbolTable::GetDerivationId(Derivation derivation) const
{
    if (!module->IsCore())
    {
        throw std::runtime_error("internal error: derivation id provided only from System.Core module");
    }
    int index = static_cast<int>(derivation);
    if (index < 0 || index >= derivationIds.size())
    {
        throw std::runtime_error("internal error: invalid derivation id index");
    }
    return derivationIds[index];
}

const util::uuid& SymbolTable::GetPositionId(int index) const
{
    if (module->Name() != U"System.Core")
    {
        throw std::runtime_error("position id provided only from System.Core module");
    }
    if (index < 0 || index >= positionIds.size())
    {
        throw std::runtime_error("invalid position id index");
    }
    return positionIds[index];
}

void SymbolTable::AddFunctionSymbol(std::unique_ptr<FunctionSymbol>&& functionSymbol)
{
    std::lock_guard<std::recursive_mutex> lock(GetModule()->Lock());
    functionSymbols.push_back(std::move(functionSymbol));
}

void SymbolTable::ResetAliasNodesAndNamespaceImports()
{
    aliasNodes.clear();
    namespaceImports.clear();
}

void SymbolTable::Check()
{
    globalNs.Check();
    if (!module)
    {
        throw SymbolCheckException("symbol table has no module", globalNs.GetFullSpan());
    }
    for (const auto& p : nsMap)
    {
        if (!p.first || !p.second)
        {
            throw SymbolCheckException("symbol table namespace map contains null namespace pointer", globalNs.GetFullSpan());
        }
    }
    for (const auto& p : nodeSymbolMap)
    {
        if (!p.first || !p.second)
        {
            throw SymbolCheckException("symbol table node symbol map contains null symbol or node pointer", globalNs.GetFullSpan());
        }
    }
    for (const auto& p : symbolNodeMap)
    {
        if (!p.first || !p.second)
        {
            throw SymbolCheckException("symbol table symbol node map contains null symbol or node pointer", globalNs.GetFullSpan());
        }
    }
    for (const auto& p : typeIdMap)
    {
        if (!p.second)
        {
            throw SymbolCheckException("symbol table type id map contains null symbol pointer", globalNs.GetFullSpan());
        }
    }
    for (const auto& p : functionIdMap)
    {
        if (!p.second)
        {
            throw SymbolCheckException("symbol table function id map contains null function pointer", globalNs.GetFullSpan());
        }
    }
    for (const auto& p : typeNameMap)
    {
        if (!p.second)
        {
            throw SymbolCheckException("symbol table type name map contains null type pointer", globalNs.GetFullSpan());
        }
    }
    for (const auto& p : derivedTypeMap)
    {
        for (DerivedTypeSymbol* type : p.second)
        {
            if (!type)
            {
                throw SymbolCheckException("symbol table derived type name map contains null derived type pointer", globalNs.GetFullSpan());
            }
        }
    }
    for (const auto& p : classTemplateSpecializationMap)
    {
        if (!p.second)
        {
            throw SymbolCheckException("symbol table class template specialization map contains null specialization pointer", globalNs.GetFullSpan());
        }
    }
    for (const auto& p : specializationCopyMap)
    {
        if (!p.first || !p.second)
        {
            throw SymbolCheckException("symbol table specialization copy map contains null specialization pointer", globalNs.GetFullSpan());
        }
    }
    for (const auto& p : arrayTypeMap)
    {
        if (!p.second)
        {
            throw SymbolCheckException("symbol table specialization array type map contains null array pointer", globalNs.GetFullSpan());
        }
    }
    conversionTable.Check();
    for (ClassTypeSymbol* p : polymorphicClasses)
    {
        if (!p)
        {
            throw SymbolCheckException("symbol table contains null polymorphic class pointer", globalNs.GetFullSpan());
        }
    }
    for (ClassTypeSymbol* p : classesHavingStaticConstructor)
    {
        if (!p)
        {
            throw SymbolCheckException("symbol table contains null static class pointer", globalNs.GetFullSpan());
        }
    }
}

void SymbolTable::WriteSymbolDefinitionMap(SymbolWriter& writer)
{
    int64_t n = symbolDefinitionMap.size();
    writer.GetBinaryStreamWriter().Write(n);
    for (const std::pair<SymbolLocation, SymbolLocation>& p : symbolDefinitionMap)
    {
        const SymbolLocation& left = p.first;
        writer.GetBinaryStreamWriter().Write(left.moduleId);
        writer.GetBinaryStreamWriter().Write(left.fileIndex);
        writer.GetAstWriter().Write(left.span);
        const SymbolLocation& right = p.second;
        writer.GetBinaryStreamWriter().Write(right.moduleId);
        writer.GetBinaryStreamWriter().Write(right.fileIndex);
        writer.GetAstWriter().Write(right.span);
    }
}

void SymbolTable::ReadSymbolDefinitionMap(SymbolReader& reader)
{
    int64_t n = reader.GetBinaryStreamReader().ReadLong();
    for (int64_t i = 0; i < n; ++i)
    {
        SymbolLocation left;
        reader.GetBinaryStreamReader().ReadUuid(left.moduleId);
        left.fileIndex = reader.GetBinaryStreamReader().ReadInt();
        left.span = reader.GetAstReader().ReadSpan();
        SymbolLocation right;
        reader.GetBinaryStreamReader().ReadUuid(right.moduleId);
        right.fileIndex = reader.GetBinaryStreamReader().ReadInt();
        right.span = reader.GetAstReader().ReadSpan();
        symbolDefinitionMap[left] = right;
    }
}

class IntrinsicConcepts
{
public:
    static IntrinsicConcepts& Instance();
    void AddIntrinsicConcept(cmajor::ast::ConceptNode* intrinsicConcept);
    const std::vector<std::unique_ptr<cmajor::ast::ConceptNode>>& GetIntrinsicConcepts() const { return intrinsicConcepts; }
    bool Initialized() const { return initialized; }
    void SetInitialized() { initialized = true; }
private:
    std::vector<std::unique_ptr<cmajor::ast::ConceptNode>> intrinsicConcepts;
    IntrinsicConcepts();
    bool initialized;
};

IntrinsicConcepts& IntrinsicConcepts::Instance()
{
    static IntrinsicConcepts instance;
    return instance;
}

IntrinsicConcepts::IntrinsicConcepts() : initialized(false)
{
}

void IntrinsicConcepts::AddIntrinsicConcept(cmajor::ast::ConceptNode* intrinsicConcept)
{
    intrinsicConcepts.push_back(std::unique_ptr<cmajor::ast::ConceptNode>(intrinsicConcept));
}

void InitCoreSymbolTable(SymbolTable& symbolTable, const soul::ast::Span& rootSpan, cmajor::ast::CompileUnitNode* rootCompileUnit, Context* context)
{
    symbolTable.SetCurrentCompileUnit(rootCompileUnit);
    symbolTable.InitUuids();
    AutoTypeSymbol* autoType = new AutoTypeSymbol(rootSpan, U"auto");
    BoolTypeSymbol* boolType = new BoolTypeSymbol(rootSpan, U"bool");
    SByteTypeSymbol* sbyteType = new SByteTypeSymbol(rootSpan, U"sbyte");
    ByteTypeSymbol* byteType = new ByteTypeSymbol(rootSpan, U"byte");
    ShortTypeSymbol* shortType = new ShortTypeSymbol(rootSpan, U"short");
    UShortTypeSymbol* ushortType = new UShortTypeSymbol(rootSpan, U"ushort");
    IntTypeSymbol* intType = new IntTypeSymbol(rootSpan, U"int");
    UIntTypeSymbol* uintType = new UIntTypeSymbol(rootSpan, U"uint");
    LongTypeSymbol* longType = new LongTypeSymbol(rootSpan, U"long");
    ULongTypeSymbol* ulongType = new ULongTypeSymbol(rootSpan, U"ulong");
    FloatTypeSymbol* floatType = new FloatTypeSymbol(rootSpan, U"float");
    DoubleTypeSymbol* doubleType = new DoubleTypeSymbol(rootSpan, U"double");
    CharTypeSymbol* charType = new CharTypeSymbol(rootSpan, U"char");
    WCharTypeSymbol* wcharType = new WCharTypeSymbol(rootSpan, U"wchar");
    UCharTypeSymbol* ucharType = new UCharTypeSymbol(rootSpan, U"uchar");
    VoidTypeSymbol* voidType = new VoidTypeSymbol(rootSpan, U"void");
    symbolTable.AddTypeSymbolToGlobalScope(autoType, context);
    symbolTable.AddTypeSymbolToGlobalScope(boolType, context);
    symbolTable.AddTypeSymbolToGlobalScope(sbyteType, context);
    symbolTable.AddTypeSymbolToGlobalScope(byteType, context);
    symbolTable.AddTypeSymbolToGlobalScope(shortType, context);
    symbolTable.AddTypeSymbolToGlobalScope(ushortType, context);
    symbolTable.AddTypeSymbolToGlobalScope(intType, context);
    symbolTable.AddTypeSymbolToGlobalScope(uintType, context);
    symbolTable.AddTypeSymbolToGlobalScope(longType, context);
    symbolTable.AddTypeSymbolToGlobalScope(ulongType, context);
    symbolTable.AddTypeSymbolToGlobalScope(floatType, context);
    symbolTable.AddTypeSymbolToGlobalScope(doubleType, context);
    symbolTable.AddTypeSymbolToGlobalScope(charType, context);
    symbolTable.AddTypeSymbolToGlobalScope(wcharType, context);
    symbolTable.AddTypeSymbolToGlobalScope(ucharType, context);
    symbolTable.AddTypeSymbolToGlobalScope(voidType, context);
    symbolTable.BeginNamespace(U"System", rootSpan, symbolTable.GlobalNs().ModuleId(), symbolTable.GlobalNs().FileIndex(), context);
    symbolTable.AddTypeSymbol(new NullPtrType(rootSpan, U"NullPtrType"), context);
    symbolTable.EndNamespace();
    MakeBasicTypeOperations(context, symbolTable, rootSpan, boolType, sbyteType, byteType, shortType, ushortType, intType, uintType, longType, ulongType, 
        floatType, doubleType, charType, wcharType, ucharType, voidType);
    if (!IntrinsicConcepts::Instance().Initialized())
    {
        IntrinsicConcepts::Instance().SetInitialized();
        IntrinsicConcepts::Instance().AddIntrinsicConcept(new cmajor::ast::SameConceptNode(rootSpan, true));
        IntrinsicConcepts::Instance().AddIntrinsicConcept(new cmajor::ast::DerivedConceptNode(rootSpan, true));
        IntrinsicConcepts::Instance().AddIntrinsicConcept(new cmajor::ast::ConvertibleConceptNode(rootSpan, true));
        IntrinsicConcepts::Instance().AddIntrinsicConcept(new cmajor::ast::ExplicitlyConvertibleConceptNode(rootSpan, true));
        IntrinsicConcepts::Instance().AddIntrinsicConcept(new cmajor::ast::CommonConceptNode(rootSpan, true));
        IntrinsicConcepts::Instance().AddIntrinsicConcept(new cmajor::ast::NonreferenceTypeConceptNode(rootSpan, true));
    }
    for (const std::unique_ptr<cmajor::ast::ConceptNode>& conceptNode : IntrinsicConcepts::Instance().GetIntrinsicConcepts())
    {
        symbolTable.BeginConcept(*conceptNode, false);
        ConceptSymbol* conceptSymbol = static_cast<ConceptSymbol*>(symbolTable.Container());
        conceptSymbol->SetAccess(SymbolAccess::public_);
        int n = conceptNode->TypeParameters().Count();
        for (int i = 0; i < n; ++i)
        {
            cmajor::ast::IdentifierNode* typeParamId = conceptNode->TypeParameters()[i];
            symbolTable.AddTemplateParameter(*typeParamId, context);
        }
        symbolTable.EndConcept(context);
        conceptSymbol->ComputeName(context);
    }
    symbolTable.AddTypeSymbolToGlobalScope(new StringFunctionContainerSymbol(rootSpan, context), context); 
}

/*
void CreateClassFile(const std::string& executableFilePath, SymbolTable& symbolTable)
{
    if (GetGlobalFlag(GlobalFlags::verbose) && !GetGlobalFlag(GlobalFlags::unitTest))
    {
        LogMessage(symbolTable.GetModule()->LogStreamId(), "Generating class file...");
    }
    std::filesystem::path cfp = std::filesystem::path(executableFilePath).replace_extension(".cls");
    std::string classFilePath = cfp.generic_string();
    const std::unordered_set<ClassTypeSymbol*>& polymorphicClasses = symbolTable.PolymorphicClasses();
    std::unordered_map<util::uuid, ClassTypeSymbol*, boost::hash<util::uuid>> classIdClassMap;
    for (ClassTypeSymbol* polymorphicClass : polymorphicClasses)
    {
        classIdClassMap[polymorphicClass->TypeId()] = polymorphicClass;
    }
    uint32_t n = classIdClassMap.size();
    BinaryWriter writer(classFilePath);
    writer.WriteULEB128UInt(n);
    for (const auto& p : classIdClassMap)
    {
        const util::uuid& typeId = p.first;
        ClassTypeSymbol* cls = p.second;
        std::string vmtObjectName = cls->VmtObjectNameStr();
        util::uuid baseClassTypeId = util::nil_uuid();
        if (cls->BaseClass())
        {
            baseClassTypeId = cls->BaseClass()->TypeId();
        }
        writer.Write(typeId);
        writer.Write(vmtObjectName);
        writer.Write(baseClassTypeId);
    }
    const std::unordered_set<ClassTypeSymbol*>& classesHavingStaticConstructor = symbolTable.ClassesHavingStaticConstructor();
    uint32_t ns = classesHavingStaticConstructor.size();
    writer.WriteULEB128UInt(ns);
    for (ClassTypeSymbol* classHavingStaticConstructor : classesHavingStaticConstructor)
    {
        const util::uuid& typeId = classHavingStaticConstructor->TypeId();
        writer.Write(typeId);
    }
    if (GetGlobalFlag(GlobalFlags::verbose) && !GetGlobalFlag(GlobalFlags::unitTest))
    {
        LogMessage(symbolTable.GetModule()->LogStreamId(), "==> " + classFilePath);
    }
}
*/

} // namespace cmajor::symbols

