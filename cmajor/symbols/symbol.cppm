// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <util/assert.hpp>

export module cmajor.symbols.symbol;

import std.core;
import soul.ast.span;
import soul.xml.dom;
import cmajor.symbols.type.map;
import cmajor.ir.emitter;
import cmajor.ast.attribute;
import cmajor.ast.specifier;
import cmajor.ast.compile.unit;
import util;

export namespace cmajor::symbols {

class SymbolWriter;
class SymbolReader;
class ContainerScope;
class ContainerSymbol;
class TypeSymbol;
class ConceptSymbol;
class ClassTypeSymbol;
class InterfaceTypeSymbol;
class NamespaceSymbol;
class FunctionSymbol;
class SymbolTable;
class Module;
class SymbolCollector;
class ClassTypeFlagMap;

enum class SymbolType : uint8_t
{
    boolTypeSymbol, sbyteTypeSymbol, byteTypeSymbol, shortTypeSymbol, ushortTypeSymbol, intTypeSymbol, uintTypeSymbol, longTypeSymbol, ulongTypeSymbol, floatTypeSymbol, 
    doubleTypeSymbol, charTypeSymbol, wcharTypeSymbol, ucharTypeSymbol, voidTypeSymbol, nullPtrTypeSymbol,
    arrayTypeSymbol, derivedTypeSymbol,
    namespaceSymbol, functionSymbol, staticConstructorSymbol, constructorSymbol, destructorSymbol, memberFunctionSymbol, conversionFunctionSymbol, functionGroupSymbol,
    classGroupTypeSymbol, classTypeSymbol, classTemplateSpecializationSymbol, interfaceTypeSymbol, conceptGroupSymbol, conceptSymbol,
    delegateTypeSymbol, classDelegateTypeSymbol, declarationBlock, aliasTypeSymbol, constantSymbol, enumTypeSymbol, enumConstantSymbol,
    templateParameterSymbol, boundTemplateParameterSymbol, parameterSymbol, localVariableSymbol, memberVariableSymbol,
    basicTypeUnaryPlus, basicTypeIntUnaryMinus, basicTypeFloatUnaryMinus, basicTypeComplement, basicTypeAdd, basicTypeFAdd, basicTypeSub, basicTypeFSub, basicTypeMul, basicTypeFMul,
    basicTypeSDiv, basicTypeUDiv, basicTypeFDiv, basicTypeSRem, basicTypeURem, basicTypeAnd, basicTypeOr, basicTypeXor, basicTypeShl, basicTypeAShr, basicTypeLShr,
    basicTypeNot, basicTypeIntegerEquality, basicTypeUnsignedIntegerLessThan, basicTypeSignedIntegerLessThan, basicTypeFloatingEquality, basicTypeFloatingLessThan,
    defaultInt1, defaultSInt8, defaultUInt8, defaultSInt16, defaultUInt16, defaultSInt32, defaultUInt32, defaultSInt64, defaultUInt64,
    defaultChar, defaultWChar, defaultUChar, defaultFloat, defaultDouble, basicTypeCopyCtor, basicTypeMoveCtor, basicTypeCopyAssignment, basicTypeMoveAssignment,
    basicTypeReturn,
    basicTypeImplicitSignExtension, basicTypeImplicitZeroExtension, basicTypeExplicitSignExtension, basicTypeExplicitZeroExtension, basicTypeTruncation, basicTypeBitCast,
    basicTypeImplicitUnsignedIntToFloating, basicTypeImplicitSignedIntToFloating, basicTypeExplicitUnsignedIntToFloating, basicTypeExplicitSignedIntToFloating,
    basicTypeFloatingToUnsignedInt, basicTypeFloatingToSignedInt, basicTypeFloatingExtension, basicTypeFloatingTruncation, trap,
    enumTypeDefaultConstructor, enumTypeCopyConstructor, enumTypeMoveConstructor, enumTypeCopyAssignment, enumTypeMoveAssignment, enumTypeReturn, enumTypeEquality,
    enumTypeToUnderlyingType, underlyingToEnumType,
    delegateTypeDefaultConstructor, delegateTypeCopyConstructor, delegateTypeMoveConstructor, delegateTypeCopyAssignment, delegateTypeMoveAssignment, delegateTypeReturn, delegateTypeEquality,
    functionToDelegateSymbol,
    classDelegateTypeDefaultConstructor, classDelegateTypeCopyConstructor, classDelegateTypeMoveConstructor, classDelegateTypeCopyAssignment, classDelegateTypeMoveAssignment,
    classDelegateTypeEquality, memberFunctionToClassDelegateSymbol,
    arrayLengthFunctionSymbol, arrayBeginFunctionSymbol, arrayEndFunctionSymbol, arrayCBeginFunctionSymbol, arrayCEndFunctionSymbol,
    interfaceTypeDefaultCtor, interfaceTypeCopyCtor, interfaceTypeMoveCtor, interfaceTypeCopyAssignment, interfaceTypeMoveAssignment, classToInterfaceConversion, getObjectPtrFromInterfaceSymbol,
    namespaceTypeSymbol, functionGroupTypeSymbol, memberExpressionTypeSymbol, variableValueSymbol, globalVariableSymbol, globalVariableGroupSymbol,
    stringFunctionContainerSymbol, stringLengthFunctionSymbol, axiomSymbol, keywordSymbol, autoTypeSymbol,
    maxSymbol
};

std::string SymbolTypeStr(SymbolType symbolType);

enum class SymbolAccess : uint8_t
{
    private_ = 0, protected_ = 1, internal_ = 2, public_ = 3
};

uint32_t AccessFlag(cmajor::ir::Emitter& emitter, SymbolAccess access);

enum class SymbolFlags : uint8_t
{
    none = 0,
    access = 1 << 0 | 1 << 1,
    static_ = 1 << 2,
    external = 1 << 3,
    nothrow_ = 1 << 4,
    project = 1 << 5,
    bound = 1 << 6,
    installed = 1 << 7
};

inline SymbolFlags operator&(SymbolFlags left, SymbolFlags right)
{
    return SymbolFlags(uint8_t(left) & uint8_t(right));
}

inline SymbolFlags operator|(SymbolFlags left, SymbolFlags right)
{
    return SymbolFlags(uint8_t(left) | uint8_t(right));
}

inline SymbolFlags operator~(SymbolFlags flags)
{
    return SymbolFlags(~uint8_t(flags));
}

std::string SymbolFlagStr(SymbolFlags symbolFlags);
std::string SymbolFlagStr(SymbolFlags symbolFlags, bool noAccess);

class SymbolLocation
{
public:
    SymbolLocation() : moduleId(util::nil_uuid()), fileIndex(-1), span()
    {
    }
    SymbolLocation(const util::uuid& moduleId_, int32_t fileIndex_, const soul::ast::Span& span_) :
        moduleId(moduleId_), fileIndex(fileIndex_), span(span_)
    {
    }
    bool IsValid() const { return !moduleId.is_nil() && fileIndex != -1 && span.IsValid(); }
    util::uuid moduleId;
    int32_t fileIndex;
    soul::ast::Span span;
};

bool operator<(const SymbolLocation& left, const SymbolLocation& right);

SymbolLocation MakeSymbolLocation(const soul::ast::Span& span, Module* module, int fileIndex);

class Symbol
{
public:
    Symbol(SymbolType symbolType_, const soul::ast::Span& span_, const std::u32string& name_);
    virtual ~Symbol();
    virtual void Write(SymbolWriter& writer);
    virtual void Read(SymbolReader& reader);
    virtual void EmplaceType(TypeSymbol* typeSymbol, int index) {}
    virtual void EmplaceConcept(ConceptSymbol* conceptSymbol) {}
    virtual void EmplaceFunction(FunctionSymbol* functionSymbol, int index) {}
    virtual bool IsExportSymbol() const { return IsProject(); }
    virtual bool IsContainerSymbol() const { return false; }
    virtual bool IsFunctionSymbol() const { return false; }
    virtual bool IsTypeSymbol() const { return false; }
    virtual bool IsClassTypeSymbol() const { return false; }
    virtual bool IsParentSymbol() const { return false; }
    virtual bool IsVariableSymbol() const { return false; }
    virtual bool IsGlobalVariableSymbol() const { return false; }
    virtual void Accept(SymbolCollector* collector) {}
    virtual const ContainerScope* GetContainerScope() const;
    virtual ContainerScope* GetContainerScope();
    virtual const ContainerScope* GetTypeScope() const;
    virtual ContainerScope* GetTypeScope();
    virtual ContainerScope* GetArrowScope();
    virtual const ContainerScope* GetArrowScope() const;
    virtual std::u32string FullName() const;
    virtual std::u32string FullNameWithSpecifiers() const;
    virtual std::u32string SimpleName() const { return Name(); }
    virtual std::u32string DocName() const { return Name(); }
    virtual std::u32string CodeName() const { return Name(); }
    virtual std::u32string Id() const { return mangledName; }
    virtual SymbolAccess DeclaredAccess() const { return Access(); }
    virtual std::string TypeString() const { return "symbol"; }
    virtual void* IrObject(cmajor::ir::Emitter& emitter);
    virtual void ComputeMangledName();
    virtual void Dump(util::CodeFormatter& formatter) {}
    virtual std::string GetSpecifierStr();
    virtual std::string Syntax();
    virtual void CopyFrom(const Symbol* that);
    virtual void Check();
    bool IsAliasTypeSymbol() const { return symbolType == SymbolType::aliasTypeSymbol; }
    bool IsDeclarationBlock() const { return symbolType == SymbolType::declarationBlock; }
    bool IsClassTemplateSpecializationSymbol() const { return symbolType == SymbolType::classTemplateSpecializationSymbol; }
    void SetMangledName(const std::u32string& mangledName_);
    SymbolAccess Access() const { return SymbolAccess(flags & SymbolFlags::access); }
    void SetAccess(SymbolAccess access_) { flags = flags | SymbolFlags(access_); }
    void SetAccess(cmajor::ast::Specifiers accessSpecifiers);
    bool IsSameParentOrAncestorOf(const Symbol* that) const;
    SymbolType GetSymbolType() const { return symbolType; }
    const soul::ast::Span& GetSpan() const { return span; }
    void SetSpan(const soul::ast::Span& span_) { span = span_; }
    soul::ast::FullSpan GetFullSpan() const;
    const std::u32string& Name() const { return name; }
    void SetName(const std::u32string& name_) { name = name_; }
    SymbolFlags GetSymbolFlags() const { return flags; }
    SymbolFlags GetStableSymbolFlags() const { return flags & ~(SymbolFlags::project | SymbolFlags::bound | SymbolFlags::installed); }
    bool IsStatic() const { return GetFlag(SymbolFlags::static_); }
    void SetStatic() { SetFlag(SymbolFlags::static_); }
    bool IsExternal() const { return GetFlag(SymbolFlags::external); }
    void SetExternal() { SetFlag(SymbolFlags::external); }
    bool IsNothrow() const { return GetFlag(SymbolFlags::nothrow_); }
    void SetNothrow() { SetFlag(SymbolFlags::nothrow_); }
    bool IsProject() const { return GetFlag(SymbolFlags::project); }
    void SetProject() { SetFlag(SymbolFlags::project); }
    bool IsBound() const { return GetFlag(SymbolFlags::bound); }
    void SetBound() { SetFlag(SymbolFlags::bound); }
    bool GetFlag(SymbolFlags flag) const { return (flags & flag) != SymbolFlags::none; }
    void SetFlag(SymbolFlags flag) { flags = flags | flag; }
    void ResetFlag(SymbolFlags flag) { flags = flags & ~flag; }
    const Symbol* Parent() const { return parent; }
    Symbol* Parent() { return parent; }
    void SetParent(Symbol* parent_) { parent = parent_; }
    const NamespaceSymbol* Ns() const;
    NamespaceSymbol* Ns();
    const ClassTypeSymbol* ClassNoThrow() const;
    ClassTypeSymbol* ClassNoThrow();
    const ContainerSymbol* ClassOrNsNoThrow() const;
    ContainerSymbol* ClassOrNsNoThrow();
    const ContainerSymbol* ClassInterfaceOrNsNoThrow() const;
    ContainerSymbol* ClassInterfaceOrNsNoThrow();
    const ContainerSymbol* ClassInterfaceEnumDelegateOrNsNoThrow() const;
    ContainerSymbol* ClassInterfaceEnumDelegateOrNsNoThrow();
    const ClassTypeSymbol* Class() const;
    ClassTypeSymbol* Class();
    const ClassTypeSymbol* ContainingClassNoThrow() const;
    ClassTypeSymbol* ContainingClassNoThrow();
    const InterfaceTypeSymbol* InterfaceNoThrow() const;
    InterfaceTypeSymbol* InterfaceNoThrow();
    const InterfaceTypeSymbol* ContainingInterfaceNoThrow() const;
    InterfaceTypeSymbol* ContainingInterfaceNoThrow();
    const FunctionSymbol* FunctionNoThrow() const;
    FunctionSymbol* FunctionNoThrow();
    const FunctionSymbol* Function() const;
    FunctionSymbol* Function();
    const FunctionSymbol* ContainingFunctionNoThrow() const;
    FunctionSymbol* ContainingFunctionNoThrow();
    const ContainerScope* ClassOrNsScope() const;
    ContainerScope* ClassOrNsScope();
    const ContainerScope* ClassInterfaceOrNsScope() const;
    ContainerScope* ClassInterfaceOrNsScope();
    const ContainerScope* ClassInterfaceEnumDelegateOrNsScope() const;
    ContainerScope* ClassInterfaceEnumDelegateOrNsScope();
    ClassTypeFlagMap& GetClassTypeFlagMap();
    Module* GetModule() const { return module; }
    Module* GetModule() { return module; }
    void SetModule(Module* module_) { module = module_; }
    const cmajor::ast::CompileUnitNode* GetCompileUnit() const { return compileUnit; }
    void SetCompileUnit(cmajor::ast::CompileUnitNode* compileUnit_) { compileUnit = compileUnit_; }
    const std::u32string& MangledName() const { return mangledName; }
    void SetAttributes(std::unique_ptr<cmajor::ast::AttributesNode>&& attributes_);
    cmajor::ast::AttributesNode* GetAttributes() const { return attributes.get(); }
    std::unique_ptr<soul::xml::Element> ToDomElement(TypeMap& typeMap);
    virtual std::unique_ptr<soul::xml::Element> CreateDomElement(TypeMap& typeMap);
    virtual soul::xml::Element* ToCCElement(int ccPrefixLength, const std::u32string& replacement, int symbolIndex) const;
    virtual std::vector<Symbol*> GetParamHelpSymbols() const;
    virtual std::string GetSymbolCategoryStr() const { return "SY"; }
    virtual std::string GetSymbolCategoryDescription() const { return "symbol"; }
    virtual std::string GetSymbolHelp() const;
    virtual std::u32string Info() const { return std::u32string(); }
    virtual void AppendChildElements(soul::xml::Element* element, TypeMap& typeMap) const {}
    virtual bool HasProjectMembers() const { return false; }
    virtual const char* ClassName() const { return "Symbol"; }
    void SetModuleId(const util::uuid& moduleId_);
    virtual const util::uuid& ModuleId() const;
    void SetFileIndex(int32_t fileIndex_) { fileIndex = fileIndex_; }
    virtual int FileIndex() const;
    bool GetLocation(SymbolLocation& definitionLocation) const;
    int SymbolIndex() const { return symbolIndex; }
    void SetSymbolIndex(int symbolIndex_) { symbolIndex = symbolIndex_; }
    virtual std::unique_ptr<Symbol> RemoveMember(int symbolIndex);
    virtual std::unique_ptr<Symbol> RemoveFromParent();
    void SetInstalled() { SetFlag(SymbolFlags::installed); }
    void ResetInstalled() { ResetFlag(SymbolFlags::installed); }
    bool IsInstalled() const { return GetFlag(SymbolFlags::installed); }
    bool IsDestructorSymbol() const { return symbolType == SymbolType::destructorSymbol; }
private:
    SymbolType symbolType;
    soul::ast::Span span;
    int32_t fileIndex;
    util::uuid moduleId;
    std::u32string name;
    SymbolFlags flags;
    std::u32string mangledName;
    Symbol* parent;
    Module* module;
    cmajor::ast::CompileUnitNode* compileUnit;
    std::unique_ptr<cmajor::ast::AttributesNode> attributes;
    int symbolIndex;
};

class SymbolCreator
{
public:
    virtual ~SymbolCreator();
    virtual Symbol* CreateSymbol(const soul::ast::Span& span, const std::u32string& name) = 0;
};

class SymbolFactory
{
public:
    static SymbolFactory& Instance();
    Symbol* CreateSymbol(SymbolType symbolType, const soul::ast::Span& span, const std::u32string& name);
    void Register(SymbolType symbolType, SymbolCreator* creator);
private:
    static std::unique_ptr<SymbolFactory> instance;
    std::vector<std::unique_ptr<SymbolCreator>> symbolCreators;
    SymbolFactory();
};

} // namespace cmajor::symbols

