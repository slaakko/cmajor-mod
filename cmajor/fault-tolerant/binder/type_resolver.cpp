// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.binder.type.resolver;

import cmajor.fault.tolerant.binder.bound.compile.unit;
import cmajor.fault.tolerant.binder.type.binder;

namespace cmajor::fault::tolerant::binder {

class TypeResolver : public cmajor::fault::tolerant::ast::DefaultVisitor
{
public:
    TypeResolver(cmajor::fault::tolerant::symbols::ContainerScope* scope_, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_, BoundCompileUnit* boundCompileUnit_);
    cmajor::fault::tolerant::symbols::TypeSymbol* GetType() const;
    void ResolveSymbol(cmajor::fault::tolerant::symbols::Symbol* symbol);
    void Visit(cmajor::fault::tolerant::ast::AutoKeywordNode& autoKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::BoolKeywordNode& boolKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::SByteKeywordNode& sbyteKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::ByteKeywordNode& byteKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::ShortKeywordNode& shortKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::UShortKeywordNode& ushortKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::IntKeywordNode& intKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::UIntKeywordNode& uintKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::LongKeywordNode& longKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::ULongKeywordNode& ulongKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::FloatKeywordNode& floatKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::DoubleKeywordNode& doubleKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::CharKeywordNode& charKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::WCharKeywordNode& wcharKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::UCharKeywordNode& ucharKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::VoidKeywordNode& voidKeywordNode) override;
    void Visit(cmajor::fault::tolerant::ast::ConstKeywordNode& constNode) override;
    void Visit(cmajor::fault::tolerant::ast::LValueRefNode& lvalueRefNode) override;
    void Visit(cmajor::fault::tolerant::ast::RValueRefNode& rvalueRefNode) override;
    void Visit(cmajor::fault::tolerant::ast::PointerNode& pointerNode) override;
    void Visit(cmajor::fault::tolerant::ast::ArrayNode& arrayNode) override;
    void Visit(cmajor::fault::tolerant::ast::IdentifierNode& identifierNode) override;
    void Visit(cmajor::fault::tolerant::ast::QualifiedIdNode& qualifiedIdNode) override;
    void Visit(cmajor::fault::tolerant::ast::TemplateIdNode& templateIdNode) override;
    void Visit(cmajor::fault::tolerant::ast::DotMemberNode& dotMemberNode) override;
private:
    cmajor::fault::tolerant::symbols::ContainerScope* scope;
    cmajor::fault::tolerant::symbols::SymbolTable* symbolTable;
    BoundCompileUnit* boundCompileUnit;
    cmajor::fault::tolerant::symbols::TypeSymbol* type;
    cmajor::fault::tolerant::symbols::Derivations derivations;
    cmajor::fault::tolerant::symbols::Symbol* symbol;
};

TypeResolver::TypeResolver(cmajor::fault::tolerant::symbols::ContainerScope* scope_, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_, BoundCompileUnit* boundCompileUnit_) :
    scope(scope_), symbolTable(symbolTable_), boundCompileUnit(boundCompileUnit_), type(nullptr), symbol(nullptr)
{
}

cmajor::fault::tolerant::symbols::TypeSymbol* TypeResolver::GetType() const
{
    if (derivations.IsEmpty())
    {
        return type;
    }
    else if (type)
    {
        return symbolTable->MakeCompoundType(type, derivations);
    }
    else
    {
        return nullptr;
    }
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::AutoKeywordNode& autoKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::autoTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::BoolKeywordNode& boolKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::boolTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::SByteKeywordNode& sbyteKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::sbyteTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::ByteKeywordNode& byteKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::byteTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::ShortKeywordNode& shortKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::shortTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::UShortKeywordNode& ushortKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::ushortTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::IntKeywordNode& intKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::intTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::UIntKeywordNode& uintKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::uintTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::LongKeywordNode& longKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::longTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::ULongKeywordNode& ulongKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::ulongTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::FloatKeywordNode& floatKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::floatTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::DoubleKeywordNode& doubleKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::doubleTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::CharKeywordNode& charKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::charTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::WCharKeywordNode& wcharKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::wcharTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::UCharKeywordNode& ucharKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::ucharTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::VoidKeywordNode& voidKeywordNode)
{
    type = symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::voidTypeSymbol);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::ConstKeywordNode& constNode)
{
    derivations.vec.push_back(cmajor::fault::tolerant::symbols::Derivation::constDerivation);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::LValueRefNode& lvalueRefNode)
{
    lvalueRefNode.Subject()->Accept(*this);
    derivations.vec.push_back(cmajor::fault::tolerant::symbols::Derivation::lvalueRefDerivation);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::RValueRefNode& rvalueRefNode)
{
    rvalueRefNode.Subject()->Accept(*this);
    derivations.vec.push_back(cmajor::fault::tolerant::symbols::Derivation::rvalueRefDerivation);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::PointerNode& pointerNode)
{
    pointerNode.Subject()->Accept(*this);
    derivations.vec.push_back(cmajor::fault::tolerant::symbols::Derivation::pointerDerivation);
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::ArrayNode& arrayNode)
{
    arrayNode.Subject()->Accept(*this);
    if (type)
    {
        type = symbolTable->MakeArrayType(type, -1); // todo size
    }
    else
    {
        type = nullptr;
    }
}

void TypeResolver::ResolveSymbol(cmajor::fault::tolerant::symbols::Symbol* symbol)
{
    if (symbol->IsAliasTypeSymbol())
    {
        cmajor::fault::tolerant::symbols::AliasTypeSymbol* aliasTypeSymbol = static_cast<cmajor::fault::tolerant::symbols::AliasTypeSymbol*>(symbol);
        if (!aliasTypeSymbol->IsBound())
        {
            TypeBinder typeBinder(boundCompileUnit, symbolTable);
            typeBinder.SetScope(scope);
            cmajor::fault::tolerant::ast::Node* node = symbolTable->GetMappedNode(symbol);
            if (node)
            {
                node->Accept(typeBinder);
            }
        }
        type = aliasTypeSymbol->GetType();
    }
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::IdentifierNode& identifierNode)
{
    symbol = scope->Lookup(&identifierNode,
        cmajor::fault::tolerant::symbols::ScopeLookup::this_and_base_and_parent | cmajor::fault::tolerant::symbols::ScopeLookup::map, 
        symbolTable);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::fault::tolerant::symbols::FileScope>& fileScope : boundCompileUnit->FileScopes())
        {
            symbol = fileScope->Lookup(&identifierNode, 
                cmajor::fault::tolerant::symbols::ScopeLookup::this_ | cmajor::fault::tolerant::symbols::ScopeLookup::map,
                symbolTable);
            if (symbol)
            {
                break;
            }
        }
    }
    if (symbol)
    {
        if (symbol->IsTypeSymbol())
        {
            type = static_cast<cmajor::fault::tolerant::symbols::TypeSymbol*>(symbol);
        }
        else
        {
            ResolveSymbol(symbol);
        }
    }
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::QualifiedIdNode& qualifiedIdNode)
{
    symbol = scope->Lookup(&qualifiedIdNode,
        cmajor::fault::tolerant::symbols::ScopeLookup::this_and_base_and_parent | cmajor::fault::tolerant::symbols::ScopeLookup::map,
        symbolTable);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::fault::tolerant::symbols::FileScope>& fileScope : boundCompileUnit->FileScopes())
        {
            symbol = fileScope->Lookup(&qualifiedIdNode,
                cmajor::fault::tolerant::symbols::ScopeLookup::this_ | cmajor::fault::tolerant::symbols::ScopeLookup::map,
                symbolTable);
            if (symbol)
            {
                break;
            }
        }
    }
    if (symbol)
    {
        if (symbol->IsTypeSymbol())
        {
            type = static_cast<cmajor::fault::tolerant::symbols::TypeSymbol*>(symbol);
        }
        else
        {
            ResolveSymbol(symbol);
        }
    }
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::TemplateIdNode& templateIdNode)
{
    // todo
}

void TypeResolver::Visit(cmajor::fault::tolerant::ast::DotMemberNode& dotMemberNode)
{
    cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
    dotMemberNode.Left()->Accept(*this);
    if (symbol)
    {
        if (symbol->IsNamespaceSymbol() || symbol->IsEnumTypeSymbol() || symbol->IsClassTypeSymbol() || symbol->IsArrayTypeSymbol())
        {
            scope = symbol->Scope();
        }
        else
        {
            cmajor::fault::tolerant::symbols::TypeSymbol* type = symbol->GetType();
            if (type)
            {
                scope = type->BaseType()->Scope();
            }
        }
    }
    dotMemberNode.Right()->Accept(*this);
    scope = prevScope;
}

cmajor::fault::tolerant::symbols::TypeSymbol* ResolveType(
    cmajor::fault::tolerant::ast::Node* node, 
    cmajor::fault::tolerant::symbols::ContainerScope* scope,
    cmajor::fault::tolerant::symbols::SymbolTable* symbolTable,
    BoundCompileUnit* boundCompileUnit)
{
    TypeResolver resolver(scope, symbolTable, boundCompileUnit);
    node->Accept(resolver);
    return resolver.GetType();
}

} // namespace cmajor::fault::tolerant::binder
