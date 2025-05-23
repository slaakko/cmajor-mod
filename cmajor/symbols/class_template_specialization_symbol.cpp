// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

module cmajor.symbols.class_template_specializations;

import cmajor.ast.reader;
import cmajor.ast.writer;
import soul.ast.span;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.symbol.table;
import cmajor.symbols.exception;
import cmajor.symbols.modules;
import cmajor.symbols.variable.symbol;
import cmajor.ir.emitter;
import util;
import std.core;

namespace cmajor::symbols {

SystemTypeMap& SystemTypeMap::Instance()
{
    static SystemTypeMap instance;
    return instance;
}

int8_t SystemTypeMap::GetSystemType(ClassTemplateSpecializationSymbol* specialization) const
{
    auto it = templateTypeMap.find(specialization->GetClassTemplate()->Name());
    if (it != templateTypeMap.end())
    {
        return it->second;
    }
    return notSystemType;
}

SystemTypeMap::SystemTypeMap()
{
    templateTypeMap[U"String<CharT>"] = stringType;
    templateTypeMap[U"List<T>"] = listType;
    templateTypeMap[U"Set<T, C>"] = setType;
    templateTypeMap[U"Map<Key, Value, KeyCompare>"] = mapType;
    templateTypeMap[U"LinkedList<T>"] = linkedListType;
    templateTypeMap[U"HashSet<T, H, C>"] = hashSetType;
    templateTypeMap[U"HashMap<K, T, H, C>"] = hashMapType;
    templateTypeMap[U"ForwardList<T>"] = forwardListType;
}

std::u32string MakeClassTemplateSpecializationName(ClassTypeSymbol* classTemplate, const std::vector<TypeSymbol*>& templateArgumentTypes)
{
    std::u32string name = classTemplate->GroupName();
    name.append(1, '<');
    int n = templateArgumentTypes.size();
    for (int i = 0; i < n; ++i)
    {
        if (i > 0)
        {
            name.append(U", ");
        }
        name.append(templateArgumentTypes[i]->FullName());
    }
    name.append(1, '>');
    return name;
}

ClassTemplateSpecializationSymbol::ClassTemplateSpecializationSymbol(const soul::ast::Span& span_, const std::u32string& name_) :
    ClassTypeSymbol(SymbolType::classTemplateSpecializationSymbol, span_, name_), classTemplate(nullptr), templateArgumentTypes(), flags(ClassTemplateSpecializationFlags::none)
{
}

ClassTemplateSpecializationSymbol::ClassTemplateSpecializationSymbol(const soul::ast::Span& span_, std::u32string& name_, ClassTypeSymbol* classTemplate_, const std::vector<TypeSymbol*>& templateArgumentTypes_) :
    ClassTypeSymbol(SymbolType::classTemplateSpecializationSymbol, span_, name_), classTemplate(classTemplate_), templateArgumentTypes(templateArgumentTypes_), flags(ClassTemplateSpecializationFlags::none)
{
}

ClassTemplateSpecializationSymbol::~ClassTemplateSpecializationSymbol()
{
}

std::u32string ClassTemplateSpecializationSymbol::SimpleName() const
{
    std::u32string simpleName = classTemplate->GroupName();
    return simpleName;
}

void ClassTemplateSpecializationSymbol::Write(SymbolWriter& writer)
{
    ClassTypeSymbol::Write(writer);
    const util::uuid& classTemplateId = classTemplate->TypeId();
    writer.GetBinaryStreamWriter().Write(classTemplateId);
    uint32_t n = templateArgumentTypes.size();
    writer.GetBinaryStreamWriter().WriteULEB128UInt(n);
    for (uint32_t i = 0; i < n; ++i)
    {
        TypeSymbol* templateArgumentType = templateArgumentTypes[i];
        const util::uuid& templateArgumentTypeId = templateArgumentType->TypeId();
        writer.GetBinaryStreamWriter().Write(templateArgumentTypeId);
    }
    writer.GetBinaryStreamWriter().Write(uint8_t(flags));
}

void ClassTemplateSpecializationSymbol::Read(SymbolReader& reader)
{
    ClassTypeSymbol::Read(reader);
    util::uuid classTemplateId;
    reader.GetBinaryStreamReader().ReadUuid(classTemplateId);
    reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, classTemplateId, -1);
    uint32_t n = reader.GetBinaryStreamReader().ReadULEB128UInt();
    templateArgumentTypes.resize(n);
    for (uint32_t i = 0; i < n; ++i)
    {
        util::uuid typeArgumentId;
        reader.GetBinaryStreamReader().ReadUuid(typeArgumentId);
        reader.GetSymbolTable()->EmplaceTypeRequest(reader, this, typeArgumentId, -2 - i);
    }
    flags = ClassTemplateSpecializationFlags(reader.GetBinaryStreamReader().ReadByte());
}

void ClassTemplateSpecializationSymbol::EmplaceType(TypeSymbol* typeSymbol, int index)
{
    if (index < 0)
    {
        if (index == -1)
        {
            Assert(typeSymbol->GetSymbolType() == SymbolType::classTypeSymbol, "class type symbol expected");
            classTemplate = static_cast<ClassTypeSymbol*>(typeSymbol);
        }
        else
        {
            int typeArgumentIndex = -(index + 2);
            if (typeArgumentIndex < 0 || typeArgumentIndex >= templateArgumentTypes.size())
            {
                Assert(false, "invalid emplace type index in class template specialization");
            }
            templateArgumentTypes[typeArgumentIndex] = typeSymbol;
        }
    }
    else
    {
        ClassTypeSymbol::EmplaceType(typeSymbol, index);
    }
}

bool ClassTemplateSpecializationSymbol::IsPrototypeTemplateSpecialization() const
{
    return IsPrototype();
}

void* ClassTemplateSpecializationSymbol::IrType(cmajor::ir::Emitter& emitter, Context* context)
{
    void* localIrType = nullptr;
    if (IsRecursive())
    {
        localIrType = emitter.GetIrTypeByTypeId(TypeId());
        if (!localIrType)
        {
            localIrType = ClassTypeSymbol::IrType(emitter, context);
            emitter.SetIrTypeByTypeId(TypeId(), localIrType);
        }
    }
    else
    {
        localIrType = ClassTypeSymbol::IrType(emitter, context);
    }
    int8_t systemType = SystemTypeMap::Instance().GetSystemType(this);
    if (systemType != notSystemType)
    {
        emitter.SetSystemType(localIrType, systemType);
    }
    return localIrType;
}

const ContainerScope* ClassTemplateSpecializationSymbol::GetArrowScope() const
{
    if (classTemplate->GroupName() == U"UniquePtr" || classTemplate->GroupName() == U"SharedPtr" || classTemplate->GroupName() == U"WeakPtr")
    {
        if (templateArgumentTypes.size() == 1)
        {
            return templateArgumentTypes[0]->GetContainerScope();
        }
    }
    return Symbol::GetArrowScope();
}

ContainerScope* ClassTemplateSpecializationSymbol::GetArrowScope()
{
    if (classTemplate->GroupName() == U"UniquePtr" || classTemplate->GroupName() == U"SharedPtr" || classTemplate->GroupName() == U"WeakPtr")
    {
        if (templateArgumentTypes.size() == 1)
        {
            return templateArgumentTypes[0]->GetContainerScope();
        }
    }
    return Symbol::GetArrowScope();
}

void ClassTemplateSpecializationSymbol::SetGlobalNs(std::unique_ptr<cmajor::ast::Node>&& globalNs_)
{
    globalNs = std::move(globalNs_);
}

void ClassTemplateSpecializationSymbol::SetFileScope(FileScope* fileScope_)
{
    fileScope.reset(fileScope_);
}

FileScope* ClassTemplateSpecializationSymbol::ReleaseFileScope()
{
    return fileScope.release();
}

TypeSymbol* ClassTemplateSpecializationSymbol::UnifyTemplateArgumentType(SymbolTable& symbolTable, const std::map<TemplateParameterSymbol*, TypeSymbol*>& templateParameterMap)
{
    std::vector<TypeSymbol*> targetTemplateArgumentTypes;
    for (int i = 0; i < templateArgumentTypes.size(); ++i)
    {
        TypeSymbol* templateArgumentType = templateArgumentTypes[i]->UnifyTemplateArgumentType(symbolTable, templateParameterMap);
        if (templateArgumentType)
        {
            targetTemplateArgumentTypes.push_back(templateArgumentType);
        }
        else
        {
            return nullptr;
        }
    }
    return symbolTable.MakeClassTemplateSpecialization(classTemplate, targetTemplateArgumentTypes);
}

std::u32string ClassTemplateSpecializationSymbol::Id() const
{
    if (IsPrototype())
    {
        return MangledName();
    }
    else
    {
        return TypeSymbol::Id();
    }
}

void ClassTemplateSpecializationSymbol::Check()
{
    ClassTypeSymbol::Check();
    if (!classTemplate)
    {
        throw SymbolCheckException("class template specialization has no class template", GetFullSpan());
    }
    for (TypeSymbol* templateArguementType : templateArgumentTypes)
    {
        if (!templateArguementType)
        {
            throw SymbolCheckException("class template specialization has no template argument type", GetFullSpan());
        }
    }
}

void ClassTemplateSpecializationSymbol::ComputeMangledName(Context* context)
{
    std::u32string mangledName = util::ToUtf32(TypeString(context));
    std::string constraintStr;
    if (Constraint())
    {
        constraintStr = " " + Constraint()->ToString();
    }
    std::string templateArgumentStr;
    for (const auto& templateArgumentType : TemplateArgumentTypes())
    {
        templateArgumentStr.append(".").append(
            util::ToUtf8(templateArgumentType->BaseType()->FullName()).append(".").append(util::ToUtf8(templateArgumentType->FullName())));
    }
    mangledName.append(1, U'_').append(SimpleName());
    mangledName.append(1, U'_').append(util::ToUtf32(util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()) + constraintStr + templateArgumentStr)));
    SetMangledName(mangledName);

}

} // namespace cmajor::symbols
