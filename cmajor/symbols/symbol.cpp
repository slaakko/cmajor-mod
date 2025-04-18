// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.symbol;

import std.core;
import cmajor.ast.writer;
import cmajor.ast.reader;
import cmajor.ast.specifier;
import cmajor.ast.attribute;
import cmajor.symbols.context;
import cmajor.symbols.symbol.writer;
import cmajor.symbols.symbol.reader;
import cmajor.symbols.namespaces;
import cmajor.symbols.exception;
import cmajor.symbols.modules;
import cmajor.symbols.scope;
import cmajor.symbols.basic.type.symbol;
import cmajor.symbols.symbol.table;
import cmajor.symbols.classes;
import cmajor.symbols.interfaces;
import cmajor.symbols.enumerations;
import cmajor.symbols.delegate.symbol;
import cmajor.symbols.module_cache;
import cmajor.symbols.basic.type.operation;
import cmajor.symbols.array.type.symbol;
import cmajor.symbols.concepts;
import cmajor.symbols.trap;
import cmajor.symbols.variable.symbol;
import cmajor.symbols.templates;
import cmajor.symbols.constant.symbol;
import cmajor.symbols.alias.type;
import cmajor.symbols.class_template_specializations;
import cmajor.symbols.string.functions;
import cmajor.symbols.global.flags;
import cmajor.ast.clone;
import util.sha1;

namespace cmajor::symbols {

const char* symbolTypeStr[uint8_t(SymbolType::maxSymbol)] =
{
    "boolTypeSymbol", "sbyteTypeSymbol", "byteTypeSymbol", "shortTypeSymbol", "ushortTypeSymbol", "intTypeSymbol", "uintTypeSymbol", "longTypeSymbol", "ulongTypeSymbol", "floatTypeSymbol", "doubleTypeSymbol",
    "charTypeSymbol", "wcharTypeSymbol", "ucharTypeSymbol", "voidTypeSymbol", "nullPtrTypeSymbol",
    "arrayTypeSymbol", "derivedTypeSymbol",
    "namespaceSymbol", "functionSymbol", "staticConstructorSymbol", "constructorSymbol", "destructorSymbol", "memberFunctionSymbol", "conversionFunctionSymbol", "functionGroupSymbol",
    "classGroupTypeSymbol", "classTypeSymbol", "interfaceTypeSymbol", "conceptGroupSymbol", "conceptSymbol",
    "delegateTypeSymbol", "classDelegateTypeSymbol", "declarationBlock", "aliasTypeSymbol", "constantSymbol", "enumTypeSymbol", "enumConstantSymbol",
    "templateParameterSymbol", "boundTemplateParameterSymbol", "parameterSymbol", "localVariableSymbol", "memberVariableSymbol",
    "basicTypeUnaryPlus", "basicTypeIntUnaryMinus", "basicTypeFloatUnaryMinus", "basicTypeComplement", "basicTypeAdd", "basicTypeFAdd", "basicTypeSub", "basicTypeFSub", "basicTypeMul", "basicTypeFMul",
    "basicTypeSDiv", "basicTypeUDiv", "basicTypeFDiv", "basicTypeSRem", "basicTypeURem", "basicTypeAnd", "basicTypeOr", "basicTypeXor", "basicTypeShl", "basicTypeAShr", "basicTypeLShr",
    "basicTypeNot", "basicTypeIntegerEquality", "basicTypeUnsignedIntegerLessThan", "basicTypeSignedIntegerLessThan", "basicTypeFloatingEquality", "basicTypeFloatingLessThan",
    "defaultInt1", "defaultInt8", "defaultInt16", "defaultInt32", "defaultInt64", "defaultFloat", "defaultDouble", "basicTypeCopyCtor", "basicTypeMoveCtor",
    "basicTypeCopyAssignment", "basicTypeMoveAssignment", "basicTypeReturn",
    "basicTypeImplicitSignExtension", "basicTypeImplicitZeroExtension", "basicTypeExplicitSignExtension", "basicTypeExplicitZeroExtension", "basicTypeTruncation", "basicTypeBitCast",
    "basicTypeImplicitUnsignedIntToFloating", "basicTypeImplicitSignedIntToFloating", "basicTypeExplicitUnsignedIntToFloating", "basicTypeExplicitSignedIntToFloating",
    "basicTypeFloatingToUnsignedInt", "basicTypeFloatingToSignedInt", "basicTypeFloatingExtension", "basicTypeFloatingTruncation", "trap",
    "enumTypeDefaultConstructor", "enumTypeCopyConstructor", "enumTypeMoveConstructor", "enumTypeCopyAssignment", "enumTypeMoveAssignment", "enumTypeReturn", "enumTypeEquality",
    "enumTypeToUnderlyingType", "underlyingToEnumType",
    "delegateTypeDefaultConstructor", "delegateTypeCopyConstructor", "delegateTypeMoveConstructor", "delegateTypeCopyAssignment", "delegateTypeMoveAssignment", "delegateTypeReturn",
    "delegateTypeEquality", "functionToDelegateSymbol",
    "classDelegateTypeDefaultConstructor", "classDelegateTypeCopyConstructor", "classDelegateTypeMoveConstructor", "classDelegateTypeCopyAssignment", "classDelegateTypeMoveAssignment",
    "classDelegateTypeEquality", "memberFunctionToClassDelegateSymbol",
    "arrayLengthFunctionSymbol", "arrayBeginFunctionSymbol", "arrayEndFunctionSymbol", "arrayCBeginFunctionSymbol", "arrayCEndFunctionSymbol",
    "interfaceTypeDefaultCtor", "interfaceTypeCopyCtor", "interfaceTypeMoveCtor", "interfaceTypeCopyAssignment", "interfaceTypeMoveAssignment", 
    "interfaceTypeEqual", "interfaceTypeLess", "classToInterfaceConversion", "getObjectPtrFromInterfaceSymbol",
    "namespaceTypeSymbol", "functionGroupTypeSymbol", "memberExpressionTypeSymbol", "variableValueSymbol", "globalVariableSymbol", "globalVariableGroupSymbol",
    "stringFunctionContainerSymbol", "stringLengthFunctionSymbol", "axiomSymbol", "keywordSymbol", "autoTypeSymbol"
};

std::string SymbolTypeStr(SymbolType symbolType)
{
    return symbolTypeStr[static_cast<uint8_t>(symbolType)];
}

std::string SymbolFlagStr(SymbolFlags symbolFlags)
{
    return SymbolFlagStr(symbolFlags, false);
}

std::string SymbolFlagStr(SymbolFlags symbolFlags, bool noAccess)
{
    std::string s;
    if (!noAccess)
    {
        SymbolAccess access = SymbolAccess(symbolFlags & SymbolFlags::access);
        switch (access)
        {
        case SymbolAccess::private_: s.append("private"); break;
        case SymbolAccess::protected_: s.append("protected"); break;
        case SymbolAccess::internal_: s.append("internal"); break;
        case SymbolAccess::public_: s.append("public"); break;
        }
    }
    if ((symbolFlags & SymbolFlags::static_) != SymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("static");
    }
    if ((symbolFlags & SymbolFlags::external) != SymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("extern");
    }
    if ((symbolFlags & SymbolFlags::nothrow_) != SymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("nothrow");
    }
    if ((symbolFlags & SymbolFlags::project) != SymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("project");
    }
    if ((symbolFlags & SymbolFlags::bound) != SymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("bound");
    }
    if ((symbolFlags & SymbolFlags::installed) != SymbolFlags::none)
    {
        if (!s.empty())
        {
            s.append(1, ' ');
        }
        s.append("installed");
    }
    return s;
}

uint32_t AccessFlag(cmajor::ir::Emitter& emitter, SymbolAccess access)
{
    switch (access)
    {
    case SymbolAccess::private_:
    {
        return emitter.GetPrivateFlag();
    }
    case SymbolAccess::protected_:
    {
        return emitter.GetProtectedFlag();
    }
    case SymbolAccess::public_:
    {
        return emitter.GetPublicFlag();
    }
    }
    return emitter.GetNoFlags();
}

bool operator<(const SymbolLocation& left, const SymbolLocation& right)
{
    if (left.moduleId < right.moduleId) return true;
    if (left.moduleId > right.moduleId) return false;
    if (left.fileIndex < right.fileIndex) return true;
    if (left.fileIndex > right.fileIndex) return false;
    if (left.span.pos < right.span.pos) return true;
    if (left.span.pos > right.span.pos) return false;
    return left.span.len < right.span.len;
}

SymbolLocation MakeSymbolLocation(const soul::ast::Span& span, Module* module, int fileIndex)
{
     return SymbolLocation(module->Id(), fileIndex, span);
}

Symbol::Symbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_) :
    symbolType(symbolType_), span(span_), fileIndex(-1), moduleId(util::nil_uuid()), name(name_), 
    flags(SymbolFlags::project), parent(nullptr), module(nullptr), compileUnit(nullptr), symbolIndex(-1)
{
}

Symbol::~Symbol()
{
}

void Symbol::Write(SymbolWriter& writer)
{
    SymbolFlags f = flags & ~(SymbolFlags::project | SymbolFlags::installed);
    writer.GetBinaryStreamWriter().Write(fileIndex);
    writer.GetBinaryStreamWriter().Write(moduleId);
    writer.GetBinaryStreamWriter().Write(static_cast<uint8_t>(f));
    writer.GetBinaryStreamWriter().Write(mangledName);
    bool hasAttributes = attributes != nullptr;
    writer.GetBinaryStreamWriter().Write(hasAttributes);
    if (hasAttributes)
    {
        writer.GetAstWriter().Write(attributes.get());
    }
}

void Symbol::Read(SymbolReader& reader)
{
    fileIndex = reader.GetBinaryStreamReader().ReadInt();
    reader.GetBinaryStreamReader().ReadUuid(moduleId);
    flags = static_cast<SymbolFlags>(reader.GetBinaryStreamReader().ReadByte());
    if (reader.SetProjectBit())
    {
        flags = flags | SymbolFlags::project;
    }
    mangledName = reader.GetBinaryStreamReader().ReadUtf32String();
    bool hasAttributes = reader.GetBinaryStreamReader().ReadBool();
    if (hasAttributes)
    {
        attributes.reset(reader.GetAstReader().ReadAttributesNode());
    }
}

const ContainerScope* Symbol::GetContainerScope() const
{
    return parent ? parent->GetContainerScope() : nullptr;
}

ContainerScope* Symbol::GetContainerScope()
{
    return parent ? parent->GetContainerScope() : nullptr;
}

const ContainerScope* Symbol::GetTypeScope() const
{
    return GetContainerScope();
}

ContainerScope* Symbol::GetTypeScope()
{
    return GetContainerScope();
}

ContainerScope* Symbol::GetArrowScope()
{
    return module->GetSymbolTable().GlobalNs().GetContainerScope();
}

const ContainerScope* Symbol::GetArrowScope() const
{
    return module->GetSymbolTable().GlobalNs().GetContainerScope();
}

std::u32string Symbol::FullName() const
{
    std::u32string fullName;
    if (parent)
    {
        fullName.append(parent->FullName());
    }
    if (!fullName.empty())
    {
        fullName.append(1, '.');
    }
    fullName.append(Name());
    return fullName;
}

std::u32string Symbol::FullNameWithSpecifiers() const
{
    std::u32string fullNameWithSpecifiers = util::ToUtf32(SymbolFlagStr(GetStableSymbolFlags()));
    if (!fullNameWithSpecifiers.empty())
    {
        fullNameWithSpecifiers.append(1, U' ');
    }
    fullNameWithSpecifiers.append(FullName());
    return fullNameWithSpecifiers;
}

void* Symbol::IrObject(cmajor::ir::Emitter& emitter, Context* context)
{
    return emitter.GetIrObject(this);
}

void Symbol::ComputeMangledName(Context* context)
{
    mangledName = util::ToUtf32(TypeString(context));
    mangledName.append(1, U'_').append(SimpleName());
    mangledName.append(1, U'_').append(util::ToUtf32(util::GetSha1MessageDigest(util::ToUtf8(FullNameWithSpecifiers()))));
}

std::string Symbol::GetSpecifierStr() 
{
    return SymbolFlagStr(flags);
}

std::string Symbol::Syntax(Context* context)
{
    std::string syntax;
    syntax.append(GetSpecifierStr());
    if (!syntax.empty())
    {
        syntax.append(1, ' ');
    }
    syntax.append(TypeString(context));
    syntax.append(1, ' ');
    syntax.append(util::ToUtf8(DocName(context)));
    syntax.append(1, ';');
    return syntax;
}

void Symbol::CopyFrom(const Symbol* that)
{
    symbolType = that->symbolType;
    span = that->span;
    fileIndex = that->fileIndex;
    moduleId = that->moduleId;
    name = that->name;
    flags = that->flags;
    mangledName = that->mangledName;
    parent = that->parent;
    module = that->module;
    compileUnit = that->compileUnit;
    if (that->attributes)
    {
        cmajor::ast::CloneContext cloneContext;
        attributes.reset(static_cast<cmajor::ast::AttributesNode*>(that->attributes->Clone(cloneContext)));
    }
}

void Symbol::Check()
{
}

void Symbol::SetMangledName(const std::u32string& mangledName_)
{
    mangledName = mangledName_;
}

void Symbol::SetAccess(cmajor::ast::Specifiers accessSpecifiers)
{
    ContainerSymbol* cls = ContainingClassNoThrow();
    SymbolAccess access = SymbolAccess::private_;
    bool classMember = true;
    if (!cls)
    {
        if (this->IsGlobalVariableSymbol())
        {
            access = SymbolAccess::private_;
        }
        else
        {
            access = SymbolAccess::internal_;
        }
        classMember = false;
        ContainerSymbol* intf = ContainingInterfaceNoThrow();
        if (intf)
        {
            access = SymbolAccess::public_;
        }
    }
    if (accessSpecifiers == cmajor::ast::Specifiers::public_)
    {
        access = SymbolAccess::public_;
    }
    else if (accessSpecifiers == cmajor::ast::Specifiers::protected_)
    {
        if (classMember)
        {
            access = SymbolAccess::protected_;
        }
        else
        {
            throw Exception("only class members can have protected access", GetFullSpan());
        }
    }
    else if (accessSpecifiers == cmajor::ast::Specifiers::internal_)
    {
        access = SymbolAccess::internal_;
    }
    else if (accessSpecifiers == cmajor::ast::Specifiers::private_)
    {
        if (classMember || this->IsGlobalVariableSymbol())
        {
            access = SymbolAccess::private_;
        }
        else
        {
            throw Exception("only class members and global variables can have private access", GetFullSpan());
        }
    }
    else if (accessSpecifiers != cmajor::ast::Specifiers::none)
    {
        throw Exception("invalid combination of access specifiers: " + SpecifierStr(accessSpecifiers), GetFullSpan());
    }
    SetAccess(access);
}

bool Symbol::IsSameParentOrAncestorOf(const Symbol* that) const
{
    if (!that)
    {
        return false;
    }
    else if (this == that)
    {
        return true;
    }
    else if (this->IsTypeSymbol() && that->IsTypeSymbol() && TypesEqual(static_cast<const TypeSymbol*>(this), static_cast<const TypeSymbol*>(that)))
    {
        return true;
    }
    else if (that->parent)
    {
        return IsSameParentOrAncestorOf(that->parent);
    }
    else
    {
        return false;
    }
}

const NamespaceSymbol* Symbol::Ns(Context* context) const
{
    if (symbolType == SymbolType::namespaceSymbol)
    {
        if (!GetModule()->IsRootModule())
        {
            Module* rootModule = context->RootModule();
            NamespaceSymbol* mappedNs = rootModule->GetSymbolTable().GetMappedNs(const_cast<NamespaceSymbol*>(static_cast<const NamespaceSymbol*>(this)));
            if (mappedNs)
            {
                return mappedNs;
            }
        }
        return static_cast<const NamespaceSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->Ns(context);
        }
        else
        {
            throw Exception("namespace symbol not found", GetFullSpan());
        }
    }
}

NamespaceSymbol* Symbol::Ns(Context* context)
{
    if (symbolType == SymbolType::namespaceSymbol)
    {
        if (!GetModule()->IsRootModule())
        {
            Module* rootModule = context->RootModule();
            NamespaceSymbol* mappedNs = rootModule->GetSymbolTable().GetMappedNs(static_cast<NamespaceSymbol*>(this));
            if (mappedNs)
            {
                return mappedNs;
            }
        }
        return static_cast<NamespaceSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->Ns(context);
        }
        else
        {
            throw Exception("namespace symbol not found", GetFullSpan());
        }
    }
}

const ClassTypeSymbol* Symbol::ClassNoThrow() const
{
    if (IsClassTypeSymbol())
    {
        return static_cast<const ClassTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->ClassNoThrow();
        }
        else
        {
            return nullptr;
        }
    }
}

ClassTypeSymbol* Symbol::ClassNoThrow()
{
    if (IsClassTypeSymbol())
    {
        return static_cast<ClassTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->ClassNoThrow();
        }
        else
        {
            return nullptr;
        }
    }
}

const ContainerSymbol* Symbol::ClassOrNsNoThrow(Context* context) const
{
    if (symbolType == SymbolType::namespaceSymbol)
    {
        if (!GetModule()->IsRootModule())
        {
            Module* rootModule = context->RootModule();
            NamespaceSymbol* mappedNs = rootModule->GetSymbolTable().GetMappedNs(const_cast<NamespaceSymbol*>(static_cast<const NamespaceSymbol*>(this)));
            if (mappedNs)
            {
                return mappedNs;
            }
        }
        return static_cast<const NamespaceSymbol*>(this);
    }
    else if (IsClassTypeSymbol())
    {
        return static_cast<const ClassTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->ClassOrNsNoThrow(context);
        }
        else
        {
            return nullptr;
        }
    }
}

ContainerSymbol* Symbol::ClassOrNsNoThrow(Context* context)
{
    if (symbolType == SymbolType::namespaceSymbol)
    {
        if (!GetModule()->IsRootModule())
        {
            Module* rootModule = context->RootModule();
            NamespaceSymbol* mappedNs = rootModule->GetSymbolTable().GetMappedNs(static_cast<NamespaceSymbol*>(this));
            if (mappedNs)
            {
                return mappedNs;
            }
        }
        return static_cast<NamespaceSymbol*>(this);
    }
    else if (IsClassTypeSymbol())
    {
        return static_cast<ClassTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->ClassOrNsNoThrow(context);
        }
        else
        {
            return nullptr;
        }
    }
}

const ContainerSymbol* Symbol::ClassInterfaceOrNsNoThrow(Context* context) const
{
    if (symbolType == SymbolType::namespaceSymbol)
    {
        if (!GetModule()->IsRootModule())
        {
            Module* rootModule = context->RootModule();
            NamespaceSymbol* mappedNs = rootModule->GetSymbolTable().GetMappedNs(const_cast<NamespaceSymbol*>(static_cast<const NamespaceSymbol*>(this)));
            if (mappedNs)
            {
                return mappedNs;
            }
        }
        return static_cast<const NamespaceSymbol*>(this);
    }
    else if (symbolType == SymbolType::interfaceTypeSymbol)
    {
        return static_cast<const InterfaceTypeSymbol*>(this);
    }
    else if (IsClassTypeSymbol())
    {
        return static_cast<const ClassTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->ClassInterfaceOrNsNoThrow(context);
        }
        else
        {
            return nullptr;
        }
    }
}

ContainerSymbol* Symbol::ClassInterfaceOrNsNoThrow(Context* context)
{
    if (symbolType == SymbolType::namespaceSymbol)
    {
        if (!GetModule()->IsRootModule())
        {
            Module* rootModule = context->RootModule();
            NamespaceSymbol* mappedNs = rootModule->GetSymbolTable().GetMappedNs(static_cast<NamespaceSymbol*>(this));
            if (mappedNs)
            {
                return mappedNs;
            }
        }
        return static_cast<NamespaceSymbol*>(this);
    }
    else if (symbolType == SymbolType::interfaceTypeSymbol)
    {
        return static_cast<InterfaceTypeSymbol*>(this);
    }
    else if (IsClassTypeSymbol())
    {
        return static_cast<ClassTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->ClassInterfaceOrNsNoThrow(context);
        }
        else
        {
            return nullptr;
        }
    }
}

const ContainerSymbol* Symbol::ClassInterfaceEnumDelegateOrNsNoThrow(Context* context) const
{
    if (symbolType == SymbolType::namespaceSymbol)
    {
        if (!GetModule()->IsRootModule())
        {
            Module* rootModule = context->RootModule();
            NamespaceSymbol* mappedNs = rootModule->GetSymbolTable().GetMappedNs(const_cast<NamespaceSymbol*>(static_cast<const NamespaceSymbol*>(this)));
            if (mappedNs)
            {
                return mappedNs;
            }
        }
        return static_cast<const NamespaceSymbol*>(this);
    }
    else if (symbolType == SymbolType::interfaceTypeSymbol)
    {
        return static_cast<const InterfaceTypeSymbol*>(this);
    }
    else if (symbolType == SymbolType::enumTypeSymbol)
    {
        return static_cast<const EnumTypeSymbol*>(this);
    }
    else if (symbolType == SymbolType::delegateTypeSymbol)
    {
        return static_cast<const DelegateTypeSymbol*>(this);
    }
    else if (symbolType == SymbolType::classDelegateTypeSymbol)
    {
        return static_cast<const ClassDelegateTypeSymbol*>(this);
    }
    else if (symbolType == SymbolType::memberExpressionTypeSymbol)
    {
        return static_cast<const MemberExpressionTypeSymbol*>(this);
    }
    else if (IsClassTypeSymbol())
    {
        return static_cast<const ClassTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->ClassInterfaceEnumDelegateOrNsNoThrow(context);
        }
        else
        {
            return nullptr;
        }
    }
}

ContainerSymbol* Symbol::ClassInterfaceEnumDelegateOrNsNoThrow(Context* context)
{
    if (symbolType == SymbolType::namespaceSymbol)
    {
        if (!GetModule()->IsRootModule())
        {
            Module* rootModule = context->RootModule();
            NamespaceSymbol* mappedNs = rootModule->GetSymbolTable().GetMappedNs(static_cast<NamespaceSymbol*>(this));
            if (mappedNs)
            {
                return mappedNs;
            }
        }
        return static_cast<NamespaceSymbol*>(this);
    }
    else if (symbolType == SymbolType::interfaceTypeSymbol)
    {
        return static_cast<InterfaceTypeSymbol*>(this);
    }
    else if (symbolType == SymbolType::enumTypeSymbol)
    {
        return static_cast<EnumTypeSymbol*>(this);
    }
    else if (symbolType == SymbolType::delegateTypeSymbol)
    {
        return static_cast<DelegateTypeSymbol*>(this);
    }
    else if (symbolType == SymbolType::classDelegateTypeSymbol)
    {
        return static_cast<ClassDelegateTypeSymbol*>(this);
    }
    else if (symbolType == SymbolType::memberExpressionTypeSymbol)
    {
        return static_cast<MemberExpressionTypeSymbol*>(this);
    }
    else if (IsClassTypeSymbol())
    {
        return static_cast<ClassTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->ClassInterfaceEnumDelegateOrNsNoThrow(context);
        }
        else
        {
            return nullptr;
        }
    }
}

const ClassTypeSymbol* Symbol::Class() const
{
    const ClassTypeSymbol* cls = ClassNoThrow();
    if (cls)
    {
        return cls;
    }
    else
    {
        throw Exception("class type symbol not found", GetFullSpan());
    }
}

ClassTypeSymbol* Symbol::Class()
{
    ClassTypeSymbol* cls = ClassNoThrow();
    if (cls)
    {
        return cls;
    }
    else
    {
        throw Exception("class type symbol not found", GetFullSpan());
    }
}

const ClassTypeSymbol* Symbol::ContainingClassNoThrow() const
{
    if (parent)
    {
        return parent->ClassNoThrow();
    }
    else
    {
        return nullptr;
    }
}

ClassTypeSymbol* Symbol::ContainingClassNoThrow()
{
    if (parent)
    {
        return parent->ClassNoThrow();
    }
    else
    {
        return nullptr;
    }
}

const InterfaceTypeSymbol* Symbol::InterfaceNoThrow() const
{
    if (symbolType == SymbolType::interfaceTypeSymbol)
    {
        return static_cast<const InterfaceTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->InterfaceNoThrow();
        }
        else
        {
            return nullptr;
        }
    }
}

InterfaceTypeSymbol* Symbol::InterfaceNoThrow()
{
    if (symbolType == SymbolType::interfaceTypeSymbol)
    {
        return static_cast<InterfaceTypeSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->InterfaceNoThrow();
        }
        else
        {
            return nullptr;
        }
    }
}

const InterfaceTypeSymbol* Symbol::ContainingInterfaceNoThrow() const
{
    if (parent)
    {
        return parent->InterfaceNoThrow();
    }
    else
    {
        return nullptr;
    }
}

InterfaceTypeSymbol* Symbol::ContainingInterfaceNoThrow()
{
    if (parent)
    {
        return parent->InterfaceNoThrow();
    }
    else
    {
        return nullptr;
    }
}

const FunctionSymbol* Symbol::FunctionNoThrow() const
{
    if (IsFunctionSymbol())
    {
        return static_cast<const FunctionSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->FunctionNoThrow();
        }
        else
        {
            return nullptr;
        }
    }
}

FunctionSymbol* Symbol::FunctionNoThrow()
{
    if (IsFunctionSymbol())
    {
        return static_cast<FunctionSymbol*>(this);
    }
    else
    {
        if (parent)
        {
            return parent->FunctionNoThrow();
        }
        else
        {
            return nullptr;
        }
    }
}

const FunctionSymbol* Symbol::Function() const
{
    const FunctionSymbol* function = FunctionNoThrow();
    if (function)
    {
        return function;
    }
    else
    {
        throw Exception("function symbol not found", GetFullSpan());
    }
}

FunctionSymbol* Symbol::Function()
{
    FunctionSymbol* function = FunctionNoThrow();
    if (function)
    {
        return function;
    }
    else
    {
        throw Exception("function symbol not found", GetFullSpan());
    }
}

const FunctionSymbol* Symbol::ContainingFunctionNoThrow() const
{
    if (parent)
    {
        return parent->FunctionNoThrow();
    }
    else
    {
        return nullptr;
    }
}

FunctionSymbol* Symbol::ContainingFunctionNoThrow()
{
    if (parent)
    {
        return parent->FunctionNoThrow();
    }
    else
    {
        return nullptr;
    }
}

const ContainerScope* Symbol::ClassOrNsScope(Context* context) const
{
    const ContainerSymbol* classOrNs = ClassOrNsNoThrow(context);
    if (classOrNs)
    {
        return classOrNs->GetContainerScope();
    }
    else
    {
        throw Exception("class or namespace scope '" + util::ToUtf8(Name()) + "' not found", GetFullSpan());
    }
}

ContainerScope* Symbol::ClassOrNsScope(Context* context)
{
    ContainerSymbol* classOrNs = ClassOrNsNoThrow(context);
    if (classOrNs)
    {
        return classOrNs->GetContainerScope();
    }
    else
    {
        throw Exception("class or namespace scope '" + util::ToUtf8(Name()) + "' not found", GetFullSpan());
    }
}

const ContainerScope* Symbol::ClassInterfaceOrNsScope(Context* context) const
{
    const ContainerSymbol* classInterfaceOrNs = ClassInterfaceOrNsNoThrow(context);
    if (classInterfaceOrNs)
    {
        return classInterfaceOrNs->GetContainerScope();
    }
    else
    {
        throw Exception("class, interface or namespace scope not found", GetFullSpan());
    }
}

ContainerScope* Symbol::ClassInterfaceOrNsScope(Context* context)
{
    ContainerSymbol* classInterfaceOrNs = ClassInterfaceOrNsNoThrow(context);
    if (classInterfaceOrNs)
    {
        return classInterfaceOrNs->GetContainerScope();
    }
    else
    {
        throw Exception("class, interface or namespace scope not found", GetFullSpan());
    }
}

const ContainerScope* Symbol::ClassInterfaceEnumDelegateOrNsScope(Context* context) const
{
    const ContainerSymbol* classInterfaceEnumDelegateOrNs = ClassInterfaceEnumDelegateOrNsNoThrow(context);
    if (classInterfaceEnumDelegateOrNs)
    {
        return classInterfaceEnumDelegateOrNs->GetContainerScope();
    }
    else
    {
        throw Exception("class, interface, enumeration, delegate, class delegate or namespace scope not found", GetFullSpan());
    }
}

ContainerScope* Symbol::ClassInterfaceEnumDelegateOrNsScope(Context* context)
{
    ContainerSymbol* classInterfaceEnumDelegateOrNs = ClassInterfaceEnumDelegateOrNsNoThrow(context);
    if (classInterfaceEnumDelegateOrNs)
    {
        return classInterfaceEnumDelegateOrNs->GetContainerScope();
    }
    else
    {
        throw Exception("class, interface, enumeration, delegate, class delegate or namespace scope not found", GetFullSpan());
    }
}

ClassTypeFlagMap& Symbol::GetClassTypeFlagMap()
{
    return GetModule()->GetClassTypeFlagMap();
}

void Symbol::SetAttributes(std::unique_ptr<cmajor::ast::AttributesNode>&& attributes_)
{
    attributes = std::move(attributes_);
}

std::unique_ptr<soul::xml::Element> Symbol::ToDomElement(TypeMap& typeMap)
{
    std::unique_ptr<soul::xml::Element> element = CreateDomElement(typeMap);
    if (element)
    {
        std::u32string info = Info();
        if (!info.empty())
        {
            element->SetAttribute("info", util::ToUtf8(info));
        }
        AppendChildElements(element.get(), typeMap);
    }
    return element;
}

std::unique_ptr<soul::xml::Element> Symbol::CreateDomElement(TypeMap& typeMap)
{
    return std::unique_ptr<soul::xml::Element>(soul::xml::MakeElement(ClassName()));
}

soul::xml::Element* Symbol::ToCCElement(int ccPrefixLength, const std::u32string& replacement, int symbolIndex) const
{
    soul::xml::Element* ccElement = soul::xml::MakeElement("symbol");
    ccElement->SetAttribute("prefixLength", std::to_string(ccPrefixLength));
    ccElement->SetAttribute("category", GetSymbolCategoryStr());
    ccElement->SetAttribute("help", GetSymbolHelp());
    ccElement->SetAttribute("completion", util::ToUtf8(Name()));
    ccElement->SetAttribute("replacement", util::ToUtf8(replacement));
    ccElement->SetAttribute("symbolIndex", std::to_string(symbolIndex));
    return ccElement;
}

std::vector<Symbol*> Symbol::GetParamHelpSymbols() const
{
    return std::vector<Symbol*>();
}

std::string Symbol::GetSymbolHelp() const
{
    std::string help = "(";
    help.append(GetSymbolCategoryDescription()).append(") ");
    help.append(util::ToUtf8(FullName()));
    return help;
}

bool Symbol::GetLocation(SymbolLocation& definitionLocation) const
{
    Module* sourceModule = GetModuleById(ModuleId());
    if (!sourceModule) return false;
    definitionLocation = SymbolLocation(sourceModule->Id(), FileIndex(), GetSpan());
    return true;
}

void Symbol::SetModuleId(const util::uuid& moduleId_)
{
    moduleId = moduleId_;
}

const util::uuid& Symbol::ModuleId() const
{
    if (!moduleId.is_nil())
    {
        return moduleId;
    }
    else if (parent)
    {
        return parent->ModuleId();
    }
    else
    {
        static util::uuid emptyId;
        return emptyId;
    }
}

int Symbol::FileIndex() const
{
    if (fileIndex != -1)
    {
        return fileIndex;
    }
    else if (parent)
    {
        return parent->FileIndex();
    }
    else
    {
        return -1;
    }
}

soul::ast::FullSpan Symbol::GetFullSpan() const
{
    return soul::ast::FullSpan(ModuleId(), FileIndex(), GetSpan());
}

std::unique_ptr<Symbol> Symbol::RemoveMember(int symbolIndex)
{
    return std::unique_ptr<Symbol>();
}

std::unique_ptr<Symbol> Symbol::RemoveFromParent()
{
    std::unique_ptr<Symbol> symbol = parent->RemoveMember(symbolIndex);
    return symbol;
}

bool Symbol::IsNothrow() const
{
    BackEnd backend = GetBackEnd();
    if (backend == BackEnd::llvm || backend == BackEnd::sbin || backend == BackEnd::cpp || backend == BackEnd::masm)
    {
        return true;
    }
    else
    {
        return GetFlag(SymbolFlags::nothrow_);
    }
}

bool Symbol::SpecializationHasFullInstantiation() const
{
    return IsClassTemplateSpecializationSymbol() && static_cast<const ClassTemplateSpecializationSymbol*>(this)->HasFullInstantiation();
}

SymbolCreator::~SymbolCreator()
{
}

template<typename SymbolT>
class ConcreteSymbolCreator : public SymbolCreator
{
public:
    Symbol* CreateSymbol(const soul::ast::Span& span, const std::u32string& name) override
    {
        return new SymbolT(span, name);
    }
};

SymbolFactory& SymbolFactory::Instance()
{
    static SymbolFactory instance;
    return instance;
}

SymbolFactory::SymbolFactory()
{
    symbolCreators.resize(static_cast<uint8_t>(SymbolType::maxSymbol));
    Register(SymbolType::autoTypeSymbol, new ConcreteSymbolCreator<AutoTypeSymbol>());
    Register(SymbolType::boolTypeSymbol, new ConcreteSymbolCreator<BoolTypeSymbol>());
    Register(SymbolType::sbyteTypeSymbol, new ConcreteSymbolCreator<SByteTypeSymbol>());
    Register(SymbolType::byteTypeSymbol, new ConcreteSymbolCreator<ByteTypeSymbol>());
    Register(SymbolType::shortTypeSymbol, new ConcreteSymbolCreator<ShortTypeSymbol>());
    Register(SymbolType::ushortTypeSymbol, new ConcreteSymbolCreator<UShortTypeSymbol>());
    Register(SymbolType::intTypeSymbol, new ConcreteSymbolCreator<IntTypeSymbol>());
    Register(SymbolType::uintTypeSymbol, new ConcreteSymbolCreator<UIntTypeSymbol>());
    Register(SymbolType::longTypeSymbol, new ConcreteSymbolCreator<LongTypeSymbol>());
    Register(SymbolType::ulongTypeSymbol, new ConcreteSymbolCreator<ULongTypeSymbol>());
    Register(SymbolType::floatTypeSymbol, new ConcreteSymbolCreator<FloatTypeSymbol>());
    Register(SymbolType::doubleTypeSymbol, new ConcreteSymbolCreator<DoubleTypeSymbol>());
    Register(SymbolType::charTypeSymbol, new ConcreteSymbolCreator<CharTypeSymbol>());
    Register(SymbolType::wcharTypeSymbol, new ConcreteSymbolCreator<WCharTypeSymbol>());
    Register(SymbolType::ucharTypeSymbol, new ConcreteSymbolCreator<UCharTypeSymbol>());
    Register(SymbolType::voidTypeSymbol, new ConcreteSymbolCreator<VoidTypeSymbol>());
    Register(SymbolType::nullPtrTypeSymbol, new ConcreteSymbolCreator<NullPtrType>());
    Register(SymbolType::arrayTypeSymbol, new ConcreteSymbolCreator<ArrayTypeSymbol>());
    Register(SymbolType::derivedTypeSymbol, new ConcreteSymbolCreator<DerivedTypeSymbol>());
    Register(SymbolType::namespaceSymbol, new ConcreteSymbolCreator<NamespaceSymbol>());
    Register(SymbolType::functionSymbol, new ConcreteSymbolCreator<FunctionSymbol>());
    Register(SymbolType::staticConstructorSymbol, new ConcreteSymbolCreator<StaticConstructorSymbol>());
    Register(SymbolType::constructorSymbol, new ConcreteSymbolCreator<ConstructorSymbol>());
    Register(SymbolType::destructorSymbol, new ConcreteSymbolCreator<DestructorSymbol>());
    Register(SymbolType::memberFunctionSymbol, new ConcreteSymbolCreator<MemberFunctionSymbol>());
    Register(SymbolType::conversionFunctionSymbol, new ConcreteSymbolCreator<ConversionFunctionSymbol>());
    Register(SymbolType::functionGroupSymbol, new ConcreteSymbolCreator<FunctionGroupSymbol>());
    Register(SymbolType::classGroupTypeSymbol, new ConcreteSymbolCreator<ClassGroupTypeSymbol>());
    Register(SymbolType::classTypeSymbol, new ConcreteSymbolCreator<ClassTypeSymbol>());
    Register(SymbolType::classTemplateSpecializationSymbol, new ConcreteSymbolCreator<ClassTemplateSpecializationSymbol>());
    Register(SymbolType::interfaceTypeSymbol, new ConcreteSymbolCreator<InterfaceTypeSymbol>());
    Register(SymbolType::conceptGroupSymbol, new ConcreteSymbolCreator<ConceptGroupSymbol>());
    Register(SymbolType::conceptSymbol, new ConcreteSymbolCreator<ConceptSymbol>());
    Register(SymbolType::delegateTypeSymbol, new ConcreteSymbolCreator<DelegateTypeSymbol>());
    Register(SymbolType::classDelegateTypeSymbol, new ConcreteSymbolCreator<ClassDelegateTypeSymbol>());
    Register(SymbolType::declarationBlock, new ConcreteSymbolCreator<DeclarationBlock>());
    Register(SymbolType::aliasTypeSymbol, new ConcreteSymbolCreator<AliasTypeSymbol>());
    Register(SymbolType::constantSymbol, new ConcreteSymbolCreator<ConstantSymbol>());
    Register(SymbolType::enumTypeSymbol, new ConcreteSymbolCreator<EnumTypeSymbol>());
    Register(SymbolType::enumConstantSymbol, new ConcreteSymbolCreator<EnumConstantSymbol>());
    Register(SymbolType::templateParameterSymbol, new ConcreteSymbolCreator<TemplateParameterSymbol>());
    Register(SymbolType::boundTemplateParameterSymbol, new ConcreteSymbolCreator<BoundTemplateParameterSymbol>());
    Register(SymbolType::parameterSymbol, new ConcreteSymbolCreator<ParameterSymbol>());
    Register(SymbolType::localVariableSymbol, new ConcreteSymbolCreator<LocalVariableSymbol>());
    Register(SymbolType::memberVariableSymbol, new ConcreteSymbolCreator<MemberVariableSymbol>());
    Register(SymbolType::basicTypeUnaryPlus, new ConcreteSymbolCreator<BasicTypeUnaryPlusOperation>());
    Register(SymbolType::basicTypeIntUnaryMinus, new ConcreteSymbolCreator<BasicTypeIntUnaryMinusOperation>());
    Register(SymbolType::basicTypeFloatUnaryMinus, new ConcreteSymbolCreator<BasicTypeFloatUnaryMinusOperation>());
    Register(SymbolType::basicTypeComplement, new ConcreteSymbolCreator<BasicTypeComplementOperation>());
    Register(SymbolType::basicTypeAdd, new ConcreteSymbolCreator<BasicTypeAddOperation>());
    Register(SymbolType::basicTypeFAdd, new ConcreteSymbolCreator<BasicTypeFAddOperation>());
    Register(SymbolType::basicTypeSub, new ConcreteSymbolCreator<BasicTypeSubOperation>());
    Register(SymbolType::basicTypeFSub, new ConcreteSymbolCreator<BasicTypeFSubOperation>());
    Register(SymbolType::basicTypeMul, new ConcreteSymbolCreator<BasicTypeMulOperation>());
    Register(SymbolType::basicTypeFMul, new ConcreteSymbolCreator<BasicTypeFMulOperation>());
    Register(SymbolType::basicTypeSDiv, new ConcreteSymbolCreator<BasicTypeSDivOperation>());
    Register(SymbolType::basicTypeUDiv, new ConcreteSymbolCreator<BasicTypeUDivOperation>());
    Register(SymbolType::basicTypeFDiv, new ConcreteSymbolCreator<BasicTypeFDivOperation>());
    Register(SymbolType::basicTypeSRem, new ConcreteSymbolCreator<BasicTypeSRemOperation>());
    Register(SymbolType::basicTypeURem, new ConcreteSymbolCreator<BasicTypeURemOperation>());
    Register(SymbolType::basicTypeAnd, new ConcreteSymbolCreator<BasicTypeAndOperation>());
    Register(SymbolType::basicTypeOr, new ConcreteSymbolCreator<BasicTypeOrOperation>());
    Register(SymbolType::basicTypeXor, new ConcreteSymbolCreator<BasicTypeXorOperation>());
    Register(SymbolType::basicTypeShl, new ConcreteSymbolCreator<BasicTypeShlOperation>());
    Register(SymbolType::basicTypeAShr, new ConcreteSymbolCreator<BasicTypeAShrOperation>());
    Register(SymbolType::basicTypeLShr, new ConcreteSymbolCreator<BasicTypeLShrOperation>());
    Register(SymbolType::basicTypeNot, new ConcreteSymbolCreator<BasicTypeNotOperation>());
    Register(SymbolType::basicTypeIntegerEquality, new ConcreteSymbolCreator<BasicTypeIntegerEqualityOperation>());
    Register(SymbolType::basicTypeUnsignedIntegerLessThan, new ConcreteSymbolCreator<BasicTypeUnsignedIntegerLessThanOperation>());
    Register(SymbolType::basicTypeSignedIntegerLessThan, new ConcreteSymbolCreator<BasicTypeSignedIntegerLessThanOperation>());
    Register(SymbolType::basicTypeFloatingEquality, new ConcreteSymbolCreator<BasicTypeFloatingEqualityOperation>());
    Register(SymbolType::basicTypeFloatingLessThan, new ConcreteSymbolCreator<BasicTypeFloatingLessThanOperation>());
    Register(SymbolType::defaultInt1, new ConcreteSymbolCreator<BasicTypeDefaultInt1Operation>());
    Register(SymbolType::defaultSInt8, new ConcreteSymbolCreator<BasicTypeDefaultSInt8Operation>());
    Register(SymbolType::defaultUInt8, new ConcreteSymbolCreator<BasicTypeDefaultUInt8Operation>());
    Register(SymbolType::defaultSInt16, new ConcreteSymbolCreator<BasicTypeDefaultSInt16Operation>());
    Register(SymbolType::defaultUInt16, new ConcreteSymbolCreator<BasicTypeDefaultUInt16Operation>());
    Register(SymbolType::defaultSInt32, new ConcreteSymbolCreator<BasicTypeDefaultSInt32Operation>());
    Register(SymbolType::defaultUInt32, new ConcreteSymbolCreator<BasicTypeDefaultUInt32Operation>());
    Register(SymbolType::defaultSInt64, new ConcreteSymbolCreator<BasicTypeDefaultSInt64Operation>());
    Register(SymbolType::defaultUInt64, new ConcreteSymbolCreator<BasicTypeDefaultUInt64Operation>());
    Register(SymbolType::defaultFloat, new ConcreteSymbolCreator<BasicTypeDefaultFloatOperation>());
    Register(SymbolType::defaultDouble, new ConcreteSymbolCreator<BasicTypeDefaultDoubleOperation>());
    Register(SymbolType::defaultChar, new ConcreteSymbolCreator<BasicTypeDefaultCharOperation>());
    Register(SymbolType::defaultWChar, new ConcreteSymbolCreator<BasicTypeDefaultWCharOperation>());
    Register(SymbolType::defaultUChar, new ConcreteSymbolCreator<BasicTypeDefaultUCharOperation>());
    Register(SymbolType::basicTypeCopyCtor, new ConcreteSymbolCreator<BasicTypeCopyCtor>());
    Register(SymbolType::basicTypeMoveCtor, new ConcreteSymbolCreator<BasicTypeMoveCtor>());
    Register(SymbolType::basicTypeCopyAssignment, new ConcreteSymbolCreator<BasicTypeCopyAssignment>());
    Register(SymbolType::basicTypeMoveAssignment, new ConcreteSymbolCreator<BasicTypeMoveAssignment>());
    Register(SymbolType::basicTypeReturn, new ConcreteSymbolCreator<BasicTypeReturn>());
    Register(SymbolType::basicTypeImplicitSignExtension, new ConcreteSymbolCreator<BasicTypeImplicitSignExtensionOperation>());
    Register(SymbolType::basicTypeImplicitZeroExtension, new ConcreteSymbolCreator<BasicTypeImplicitZeroExtensionOperation>());
    Register(SymbolType::basicTypeExplicitSignExtension, new ConcreteSymbolCreator<BasicTypeExplicitSignExtensionOperation>());
    Register(SymbolType::basicTypeExplicitZeroExtension, new ConcreteSymbolCreator<BasicTypeExplicitZeroExtensionOperation>());
    Register(SymbolType::basicTypeTruncation, new ConcreteSymbolCreator<BasicTypeTruncationOperation>());
    Register(SymbolType::basicTypeBitCast, new ConcreteSymbolCreator<BasicTypeBitCastOperation>());
    Register(SymbolType::basicTypeImplicitUnsignedIntToFloating, new ConcreteSymbolCreator<BasicTypeImplicitUnsignedIntToFloatingOperation>());
    Register(SymbolType::basicTypeImplicitSignedIntToFloating, new ConcreteSymbolCreator<BasicTypeImplicitSignedIntToFloatingOperation>());
    Register(SymbolType::basicTypeExplicitUnsignedIntToFloating, new ConcreteSymbolCreator<BasicTypeExplicitUnsignedIntToFloatingOperation>());
    Register(SymbolType::basicTypeExplicitSignedIntToFloating, new ConcreteSymbolCreator<BasicTypeExplicitSignedIntToFloatingOperation>());
    Register(SymbolType::basicTypeFloatingToUnsignedInt, new ConcreteSymbolCreator<BasicTypeFloatingToUnsignedIntOperation>());
    Register(SymbolType::basicTypeFloatingToSignedInt, new ConcreteSymbolCreator<BasicTypeFloatingToSignedIntOperation>());
    Register(SymbolType::basicTypeFloatingExtension, new ConcreteSymbolCreator<BasicTypeFloatingExtensionOperation>());
    Register(SymbolType::basicTypeFloatingTruncation, new ConcreteSymbolCreator<BasicTypeFloatingTruncationOperation>());
    Register(SymbolType::enumTypeDefaultConstructor, new ConcreteSymbolCreator<EnumTypeDefaultConstructor>());
    Register(SymbolType::enumTypeCopyConstructor, new ConcreteSymbolCreator<EnumTypeCopyConstructor>());
    Register(SymbolType::enumTypeMoveConstructor, new ConcreteSymbolCreator<EnumTypeMoveConstructor>());
    Register(SymbolType::enumTypeCopyAssignment, new ConcreteSymbolCreator<EnumTypeCopyAssignment>());
    Register(SymbolType::enumTypeMoveAssignment, new ConcreteSymbolCreator<EnumTypeMoveAssignment>());
    Register(SymbolType::enumTypeReturn, new ConcreteSymbolCreator<EnumTypeReturn>());
    Register(SymbolType::enumTypeEquality, new ConcreteSymbolCreator<EnumTypeEqualityOp>());
    Register(SymbolType::enumTypeToUnderlyingType, new ConcreteSymbolCreator<EnumTypeToUnderlyingTypeConversion>());
    Register(SymbolType::underlyingToEnumType, new ConcreteSymbolCreator<UnderlyingTypeToEnumTypeConversion>());
    Register(SymbolType::delegateTypeDefaultConstructor, new ConcreteSymbolCreator<DelegateTypeDefaultConstructor>());
    Register(SymbolType::delegateTypeCopyConstructor, new ConcreteSymbolCreator<DelegateTypeCopyConstructor>());
    Register(SymbolType::delegateTypeMoveConstructor, new ConcreteSymbolCreator<DelegateTypeMoveConstructor>());
    Register(SymbolType::delegateTypeCopyAssignment, new ConcreteSymbolCreator<DelegateTypeCopyAssignment>());
    Register(SymbolType::delegateTypeMoveAssignment, new ConcreteSymbolCreator<DelegateTypeMoveAssignment>());
    Register(SymbolType::delegateTypeReturn, new ConcreteSymbolCreator<DelegateTypeReturn>());
    Register(SymbolType::delegateTypeEquality, new ConcreteSymbolCreator<DelegateTypeEquality>());
    Register(SymbolType::delegateTypeLess, new ConcreteSymbolCreator<DelegateTypeLess>());
    Register(SymbolType::functionToDelegateSymbol, new ConcreteSymbolCreator<FunctionToDelegateConversion>());
    Register(SymbolType::classDelegateTypeDefaultConstructor, new ConcreteSymbolCreator<ClassDelegateTypeDefaultConstructor>());
    Register(SymbolType::classDelegateTypeCopyConstructor, new ConcreteSymbolCreator<ClassDelegateTypeCopyConstructor>());
    Register(SymbolType::classDelegateTypeMoveConstructor, new ConcreteSymbolCreator<ClassDelegateTypeMoveConstructor>());
    Register(SymbolType::classDelegateTypeCopyAssignment, new ConcreteSymbolCreator<ClassDelegateTypeCopyAssignment>());
    Register(SymbolType::classDelegateTypeMoveAssignment, new ConcreteSymbolCreator<ClassDelegateTypeMoveAssignment>());
    Register(SymbolType::classDelegateTypeEquality, new ConcreteSymbolCreator<ClassDelegateTypeEquality>());
    Register(SymbolType::classDelegateTypeLess, new ConcreteSymbolCreator<ClassDelegateTypeLess>());
    Register(SymbolType::memberFunctionToClassDelegateSymbol, new ConcreteSymbolCreator<MemberFunctionToClassDelegateConversion>());
    Register(SymbolType::arrayLengthFunctionSymbol, new ConcreteSymbolCreator<ArrayLengthFunction>());
    Register(SymbolType::arrayBeginFunctionSymbol, new ConcreteSymbolCreator<ArrayBeginFunction>()),
    Register(SymbolType::arrayEndFunctionSymbol, new ConcreteSymbolCreator<ArrayEndFunction>());
    Register(SymbolType::arrayCBeginFunctionSymbol, new ConcreteSymbolCreator<ArrayCBeginFunction>());
    Register(SymbolType::arrayCEndFunctionSymbol, new ConcreteSymbolCreator<ArrayCEndFunction>());
    Register(SymbolType::interfaceTypeDefaultCtor, new ConcreteSymbolCreator<InterfaceTypeDefaultConstructor>());
    Register(SymbolType::interfaceTypeCopyCtor, new ConcreteSymbolCreator<InterfaceTypeCopyConstructor>());
    Register(SymbolType::interfaceTypeMoveCtor, new ConcreteSymbolCreator<InterfaceTypeMoveConstructor>());
    Register(SymbolType::interfaceTypeCopyAssignment, new ConcreteSymbolCreator<InterfaceTypeCopyAssignment>());
    Register(SymbolType::interfaceTypeMoveAssignment, new ConcreteSymbolCreator<InterfaceTypeMoveAssignment>());
    Register(SymbolType::interfaceTypeEqual, new ConcreteSymbolCreator<InterfaceTypeEqual>());
    Register(SymbolType::interfaceTypeLess, new ConcreteSymbolCreator<InterfaceTypeLess>());
    Register(SymbolType::classToInterfaceConversion, new ConcreteSymbolCreator<ClassToInterfaceConversion>());
    Register(SymbolType::getObjectPtrFromInterfaceSymbol, new ConcreteSymbolCreator<GetObjectPtrFromInterface>());
    Register(SymbolType::globalVariableSymbol, new ConcreteSymbolCreator<GlobalVariableSymbol>());
    Register(SymbolType::stringFunctionContainerSymbol, new ConcreteSymbolCreator<StringFunctionContainerSymbol>());
    Register(SymbolType::stringLengthFunctionSymbol, new ConcreteSymbolCreator<StringLengthFunction>()); 
    Register(SymbolType::axiomSymbol, new ConcreteSymbolCreator<AxiomSymbol>());
#ifdef _WIN32
    Register(SymbolType::trap, new ConcreteSymbolCreator<TrapFunction>());
#endif
}

Symbol* SymbolFactory::CreateSymbol(SymbolType symbolType, const soul::ast::Span& span, const std::u32string& name)
{
    const std::unique_ptr<SymbolCreator>& symbolCreator = symbolCreators[static_cast<uint8_t>(symbolType)];
    if (symbolCreator)
    {
        Symbol* symbol = symbolCreator->CreateSymbol(span, name);
        if (symbol)
        {
            return symbol;
        }
        else
        {
            throw std::runtime_error("could not create symbol");
        }
    }
    else
    {
        throw std::runtime_error("no creator for symbol type '" + SymbolTypeStr(symbolType) + "'");
    }
}

void SymbolFactory::Register(SymbolType symbolType, SymbolCreator* creator)
{
    symbolCreators[static_cast<uint8_t>(symbolType)] = std::unique_ptr<SymbolCreator>(creator);
}

} // namespace cmajor::symbols

