// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.symbol;

import cmajor.fault.tolerant.symbols.error.container;
import cmajor.fault.tolerant.ast;
import soul.ast.span;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

enum class SymbolKind
{
    namespaceSymbol, aliasTypeSymbol, constantSymbol, delegateSymbol, classDelegateSymbol, enumTypeSymbol, enumConstantSymbol, 
    parameterSymbol, localVariableSymbol, memberVariableSymbol, globalVariableSymbol, templateParameterSymbol,
    functionSymbol, staticConstructorSymbol, constructorSymbol, destructorSymbol, memberFunctionSymbol, conversionFunctionSymbol, 
    classTypeSymbol, interfaceTypeSymbol, conceptSymbol, declarationBlockSymbol, typeParameterSymbol,
    autoTypeSymbol, boolTypeSymbol, sbyteTypeSymbol, byteTypeSymbol, shortTypeSymbol, ushortTypeSymbol, intTypeSymbol, uintTypeSymbol, 
    longTypeSymbol, ulongTypeSymbol, floatTypeSymbol, doubleTypeSymbol, charTypeSymbol, wcharTypeSymbol, ucharTypeSymbol, voidTypeSymbol,
    compoundTypeSymbol, arrayTypeSymbol
};

class Source;
class SymbolWriter;
class SymbolReader;
class ContainerSymbol;
class ContainerScope;
class TypeSymbol;
class SymbolTable;

enum class SymbolFlags : int
{
    none = 0,
    installed = 1 << 0,
    bound = 1 << 1
};

constexpr SymbolFlags operator|(SymbolFlags left, SymbolFlags right)
{
    return SymbolFlags(int(left) | int(right));
}

constexpr SymbolFlags operator&(SymbolFlags left, SymbolFlags right)
{
    return SymbolFlags(int(left) & int(right));
}

constexpr SymbolFlags operator~(SymbolFlags flags)
{
    return SymbolFlags(~int(flags));
}

class Symbol
{
public:
    Symbol(SymbolKind kind_, const std::u16string& name_, const soul::ast::Span& span_);
    virtual ~Symbol();
    virtual void Write(SymbolWriter& writer);
    virtual void Read(SymbolReader& reader);
    const soul::ast::Span& Span() const { return span; }
    SymbolKind Kind() const { return kind; }
    SymbolFlags Flags() const { return flags; }
    bool IsInstalled() const { return (flags & SymbolFlags::installed) != SymbolFlags::none; }
    void SetInstalled() { flags = flags | SymbolFlags::installed; }
    void ResetInstalled() { flags = flags & ~SymbolFlags::installed; }
    bool IsBound() const { return (flags & SymbolFlags::bound) != SymbolFlags::none; }
    void SetBound() { flags = flags | SymbolFlags::bound; }
    const std::u16string& Name() const { return name; }
    void SetParent(ContainerSymbol* parent_) { parent = parent_; }
    ContainerSymbol* Parent() { return parent; }
    virtual ContainerScope* Scope();
    virtual TypeSymbol* GetType() const { return nullptr; }
    void SetSpecifiers(cmajor::fault::tolerant::ast::Specifiers specifiers_) { specifiers = specifiers_; }
    cmajor::fault::tolerant::ast::Specifiers Specifiers() const { return specifiers; }
    void SetSource(Source* source_) { source = source_; }
    Source* GetSource() const { return source; }
    void SetIndex(int index_) { index = index_; }
    int Index() const { return index; }
    void RemoveFromParent(ErrorContainer* errors, std::vector<std::unique_ptr<Symbol>>& removedSymbols);
    bool IsNamespaceSymbol() const { return kind == SymbolKind::namespaceSymbol; }
    bool IsParameterSymbol() const { return kind == SymbolKind::parameterSymbol; }
    bool IsLocalVariableSymbol() const { return kind == SymbolKind::localVariableSymbol; }
    bool IsMemberVariableSymbol() const { return kind == SymbolKind::memberVariableSymbol; }
    bool IsGlobalVariableSymbol() const { return kind == SymbolKind::globalVariableSymbol; }
    bool IsTemplateParameterSymbol() const { return kind == SymbolKind::templateParameterSymbol; }
    bool IsAliasTypeSymbol() const { return kind == SymbolKind::aliasTypeSymbol; }
    bool IsFunctionSymbol() const { return kind == SymbolKind::functionSymbol; }
    bool IsStaticConstructorSymbol() const { return kind == SymbolKind::staticConstructorSymbol; }
    bool IsConstructorSymbol() const { return kind == SymbolKind::constructorSymbol; }
    bool IsDestructorSymbol() const { return kind == SymbolKind::destructorSymbol; }
    bool IsMemberFunctionSymbol() const { return kind == SymbolKind::memberFunctionSymbol; }
    bool IsConversionFunctionSymbol() const { return kind == SymbolKind::conversionFunctionSymbol; }
    bool IsConstantSymbol() const { return kind == SymbolKind::constantSymbol; }
    bool IsDelegateSymbol() const { return kind == SymbolKind::delegateSymbol; }
    bool IsClassDelegateSymbol() const { return kind == SymbolKind::classDelegateSymbol; }
    bool IsEnumConstantSymbol() const { return kind == SymbolKind::enumConstantSymbol; }
    bool IsEnumTypeSymbol() const { return kind == SymbolKind::enumTypeSymbol; }
    bool IsBasicTypeSymbol() const;
    bool IsCompoundTypeSymbol() const { return kind == SymbolKind::compoundTypeSymbol; }
    bool IsArrayTypeSymbol() const { return kind == SymbolKind::arrayTypeSymbol; }
    bool IsTypeSymbol() const;
    bool IsClassTypeSymbol() const { return kind == SymbolKind::classTypeSymbol; }
    bool IsInterfaceTypeSymbol() const { return kind == SymbolKind::interfaceTypeSymbol; }
    bool IsConceptSymbol() const { return kind == SymbolKind::conceptSymbol; }
    bool IsDeclarationBlockSymbol() const { return kind == SymbolKind::declarationBlockSymbol; }
    bool IsContainerSymbol() const;
    virtual void Resolve(SymbolTable* symbolTable);
    virtual SymbolTable* GetSymbolTable();
    virtual void Compact();
private:
    SymbolKind kind;
    SymbolFlags flags;
    std::u16string name;
    soul::ast::Span span;
    cmajor::fault::tolerant::ast::Specifiers specifiers;
    ContainerSymbol* parent;
    int index;
    Source* source;
};

} // namespace cmajor::fault::tolerant::symbols
