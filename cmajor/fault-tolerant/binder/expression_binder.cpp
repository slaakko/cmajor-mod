// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.binder.expression.binder;

import cmajor.fault.tolerant.binder.bound.compile.unit;

namespace cmajor::fault::tolerant::binder {

class ExpressionBinder : public cmajor::fault::tolerant::ast::DefaultVisitor
{
public:
    ExpressionBinder(BoundCompileUnit* boundCompileUnit_, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_,
        cmajor::fault::tolerant::symbols::ContainerScope* scope_);
    void Visit(cmajor::fault::tolerant::ast::AutoKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::BoolKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::SByteKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ByteKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ShortKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::UShortKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::IntKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::UIntKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::LongKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ULongKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::FloatKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DoubleKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::CharKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::WCharKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::UCharKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::VoidKeywordNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::BinaryExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::PrefixExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::PostfixExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::SubscriptExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::InvokeExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::IsExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::AsExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DotMemberNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ArrowMemberNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ParenthesizedExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::SizeOfExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::TypeNameExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::TypeIdExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::CastExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstructExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::NewExprNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::QualifiedIdNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::IdentifierNode& node) override;
private:
    BoundCompileUnit* boundCompileUnit;
    cmajor::fault::tolerant::symbols::SymbolTable* symbolTable;
    cmajor::fault::tolerant::symbols::ContainerScope* scope;
    cmajor::fault::tolerant::symbols::Symbol* symbol;
};

ExpressionBinder::ExpressionBinder(BoundCompileUnit* boundCompileUnit_, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_,
    cmajor::fault::tolerant::symbols::ContainerScope* scope_) : boundCompileUnit(boundCompileUnit_), symbolTable(symbolTable_), scope(scope_), symbol(nullptr)
{
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::AutoKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::autoTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::BoolKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::boolTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::SByteKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::sbyteTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::ByteKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::byteTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::ShortKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::shortTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::UShortKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::ushortTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::IntKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::intTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::UIntKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::uintTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::LongKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::longTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::ULongKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::ulongTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::FloatKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::floatTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::DoubleKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::doubleTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::CharKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::charTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::WCharKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::wcharTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::UCharKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::ucharTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::VoidKeywordNode& node)
{
    symbolTable->MapNode(&node, symbolTable->GetBasicType(cmajor::fault::tolerant::symbols::SymbolKind::voidTypeSymbol));
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::BinaryExprNode& node)
{
    node.Left()->Accept(*this);
    node.Right()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::PrefixExprNode& node)
{
    node.Operand()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::PostfixExprNode& node)
{
    node.Operand()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::SubscriptExprNode& node)
{
    node.Operand()->Accept(*this);
    if (node.Index())
    {
        node.Index()->Accept(*this);
    }
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::InvokeExprNode& node)
{
    node.Operand()->Accept(*this);
    node.ArgumentList()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::IsExprNode& node)
{
    node.Left()->Accept(*this);
    node.Right()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::AsExprNode& node)
{
    node.Left()->Accept(*this);
    node.Right()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::DotMemberNode& node)
{
    cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
    node.Left()->Accept(*this);
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
    node.Right()->Accept(*this);
    scope = prevScope;
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::ArrowMemberNode& node)
{
    cmajor::fault::tolerant::symbols::ContainerScope* prevScope = scope;
    node.Left()->Accept(*this);
    if (symbol)
    {
        cmajor::fault::tolerant::symbols::TypeSymbol* type = symbol->GetType();
        if (type)
        {
            scope = type->BaseType()->Scope();
        }
    }
    node.Right()->Accept(*this);
    scope = prevScope;
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::ParenthesizedExprNode& node)
{
    node.Operand()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::SizeOfExprNode& node)
{
    node.Operand()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::TypeNameExprNode& node)
{
    node.Operand()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::TypeIdExprNode& node)
{
    node.Operand()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::CastExprNode& node)
{
    node.Left()->Accept(*this);
    node.Right()->Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::ConstructExprNode& node)
{
    node.Operand()->Accept(*this);
    node.Args().Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::NewExprNode& node)
{
    node.Args().Accept(*this);
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::QualifiedIdNode& node)
{
    symbol = scope->Lookup(&node, 
        cmajor::fault::tolerant::symbols::ScopeLookup::this_and_base_and_parent | cmajor::fault::tolerant::symbols::ScopeLookup::map,
        symbolTable);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::fault::tolerant::symbols::FileScope>& fileScope : boundCompileUnit->FileScopes())
        {
            symbol = fileScope->Lookup(&node, 
                cmajor::fault::tolerant::symbols::ScopeLookup::this_ | cmajor::fault::tolerant::symbols::ScopeLookup::map,
                symbolTable);
            if (symbol)
            {
                break;
            }
        }
    }
}

void ExpressionBinder::Visit(cmajor::fault::tolerant::ast::IdentifierNode& node)
{
    symbol = scope->Lookup(&node,
        cmajor::fault::tolerant::symbols::ScopeLookup::this_and_base_and_parent | cmajor::fault::tolerant::symbols::ScopeLookup::map,
        symbolTable);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::fault::tolerant::symbols::FileScope>& fileScope : boundCompileUnit->FileScopes())
        {
            symbol = fileScope->Lookup(&node,
                cmajor::fault::tolerant::symbols::ScopeLookup::this_ | cmajor::fault::tolerant::symbols::ScopeLookup::map,
                symbolTable);
            if (symbol)
            {
                break;
            }
        }
    }
}

void BindExpression(cmajor::fault::tolerant::ast::Node* expression, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable,
    cmajor::fault::tolerant::symbols::ContainerScope* scope)
{
    ExpressionBinder binder(boundCompileUnit, symbolTable, scope);
    expression->Accept(binder);
}

} // namespace cmajor::fault::tolerant::binder
