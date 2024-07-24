// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmdoclib.constraint;

namespace cmdoclib {

class ConstraintSymbolBinder : public cmajor::binder::ConstraintSymbolBinder
{
public:
    void BindConstraintSymbols(cmajor::ast::Node* node, cmajor::symbols::ContainerScope* containerScope, cmajor::binder::BoundCompileUnit& boundCompileUnit) override;
};

void ConstraintSymbolBinder::BindConstraintSymbols(cmajor::ast::Node* node, cmajor::symbols::ContainerScope* containerScope, cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    cmdoclib::BindConstraintSymbols(node, containerScope, boundCompileUnit);
}

ConstraintSymbolBinder constraintSymbolBinder;

void SetConstraintSymbolBinder()
{
    cmajor::binder::SetConstraintSymbolBinder(&constraintSymbolBinder);
}

class ConstraintBinderVisitor : public cmajor::ast::Visitor
{
public:
    ConstraintBinderVisitor(cmajor::symbols::ContainerScope* containerScope_, cmajor::binder::BoundCompileUnit& boundCompileUnit_);
    void Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode) override;
    void Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode) override;
    void Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode) override;
    void Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode) override;
    void Visit(cmajor::ast::ConceptNode& conceptNode) override;
    void Visit(cmajor::ast::ConceptIdNode& conceptIdNode) override;
    void Visit(cmajor::ast::IsConstraintNode& isConstraintnode) override;
    void Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode) override;
    void Visit(cmajor::ast::IdentifierNode& identifierNode) override;
    void Visit(cmajor::ast::DotNode& dotNode) override;
    cmajor::ast::IdentifierNode* PrevId() const { return prevId; }
private:
    cmajor::symbols::ContainerScope* containerScope;
    cmajor::binder::BoundCompileUnit& boundCompileUnit;
    cmajor::symbols::SymbolTable& symbolTable;
    cmajor::symbols::ConceptGroupSymbol* conceptGroup;
    cmajor::symbols::NamespaceSymbol* ns;
    cmajor::ast::IdentifierNode* prevId;
};

ConstraintBinderVisitor::ConstraintBinderVisitor(cmajor::symbols::ContainerScope* containerScope_, cmajor::binder::BoundCompileUnit& boundCompileUnit_) :
    symbolTable(boundCompileUnit_.GetSymbolTable()), containerScope(containerScope_), boundCompileUnit(boundCompileUnit_), conceptGroup(nullptr), ns(nullptr),
    prevId(boundCompileUnit.GetLatestIdentifier())
{
    boundCompileUnit.SetLatestIdentifier(nullptr);
}

void ConstraintBinderVisitor::Visit(cmajor::ast::ParenthesizedConstraintNode& parenthesizedConstraintNode)
{
    parenthesizedConstraintNode.Constraint()->Accept(*this);
}

void ConstraintBinderVisitor::Visit(cmajor::ast::DisjunctiveConstraintNode& disjunctiveConstraintNode)
{
    disjunctiveConstraintNode.Left()->Accept(*this);
    disjunctiveConstraintNode.Right()->Accept(*this);
}

void ConstraintBinderVisitor::Visit(cmajor::ast::ConjunctiveConstraintNode& conjunctiveConstraintNode)
{
    conjunctiveConstraintNode.Left()->Accept(*this);
    conjunctiveConstraintNode.Right()->Accept(*this);
}

void ConstraintBinderVisitor::Visit(cmajor::ast::WhereConstraintNode& whereConstraintNode)
{
    whereConstraintNode.Constraint()->Accept(*this);
}

void ConstraintBinderVisitor::Visit(cmajor::ast::ConceptNode& conceptNode)
{
    if (conceptNode.Refinement())
    {
        conceptNode.Refinement()->Accept(*this);
    }
    int n = conceptNode.Constraints().Count();
    for (int i = 0; i < n; ++i)
    {
        conceptNode.Constraints()[i]->Accept(*this);
    }
}

void ConstraintBinderVisitor::Visit(cmajor::ast::ConceptIdNode& conceptIdNode)
{
    conceptIdNode.Id()->Accept(*this);
    if (conceptGroup)
    {
        int n = conceptIdNode.Arity();
        cmajor::symbols::ConceptSymbol* conceptSymbol = conceptGroup->GetConcept(n);
        if (conceptSymbol)
        {
            if (boundCompileUnit.GetLatestIdentifier())
            {
                symbolTable.MapSymbol(boundCompileUnit.GetLatestIdentifier(), conceptSymbol);
                MapIdentifierToSymbolDefinition(boundCompileUnit.GetLatestIdentifier(), conceptSymbol);
            }
        }
    }
    int n = conceptIdNode.TypeParameters().Count();
    for (int i = 0; i < n; ++i)
    {
        conceptIdNode.TypeParameters()[i]->Accept(*this);
    }
}

void ConstraintBinderVisitor::Visit(cmajor::ast::IsConstraintNode& isConstraintNode)
{
    isConstraintNode.TypeExpr()->Accept(*this);
    isConstraintNode.ConceptOrTypeName()->Accept(*this);
    if (conceptGroup)
    {
        cmajor::symbols::ConceptSymbol* conceptSymbol = conceptGroup->GetConcept(1);
        if (conceptSymbol)
        {
            if (boundCompileUnit.GetLatestIdentifier())
            {
                symbolTable.MapSymbol(boundCompileUnit.GetLatestIdentifier(), conceptSymbol);
                MapIdentifierToSymbolDefinition(boundCompileUnit.GetLatestIdentifier(), conceptSymbol);
            }
        }
    }
}

void ConstraintBinderVisitor::Visit(cmajor::ast::MultiParamConstraintNode& multiParamConstraintNode)
{
    multiParamConstraintNode.ConceptId()->Accept(*this);
    if (conceptGroup)
    {
        int n = multiParamConstraintNode.TypeExprs().Count();
        cmajor::symbols::ConceptSymbol* conceptSymbol = conceptGroup->GetConcept(n);
        if (conceptSymbol)
        {
            if (boundCompileUnit.GetLatestIdentifier())
            {
                symbolTable.MapSymbol(boundCompileUnit.GetLatestIdentifier(), conceptSymbol);
                MapIdentifierToSymbolDefinition(boundCompileUnit.GetLatestIdentifier(), conceptSymbol);
            }
        }
    }
    int n = multiParamConstraintNode.TypeExprs().Count();
    for (int i = 0; i < n; ++i)
    {
        multiParamConstraintNode.TypeExprs()[i]->Accept(*this);
    }
}

void ConstraintBinderVisitor::Visit(cmajor::ast::IdentifierNode& identifierNode)
{
    ns = nullptr;
    conceptGroup = nullptr;
    boundCompileUnit.SetLatestIdentifier(&identifierNode);
    const std::u32string& name = identifierNode.Str();
    cmajor::symbols::Symbol* symbol = containerScope->Lookup(name, cmajor::symbols::ScopeLookup::this_and_base_and_parent);
    if (!symbol)
    {
        for (const std::unique_ptr<cmajor::symbols::FileScope>& fileScope : boundCompileUnit.FileScopes())
        {
            symbol = fileScope->Lookup(name);
            if (symbol)
            {
                break;
            }
        }
    }
    if (symbol)
    {
        if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::conceptGroupSymbol)
        {
            conceptGroup = static_cast<cmajor::symbols::ConceptGroupSymbol*>(symbol);
        }
        else
        {
            if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::boundTemplateParameterSymbol)
            {
                cmajor::symbols::BoundTemplateParameterSymbol* boundTemplateParam = static_cast<cmajor::symbols::BoundTemplateParameterSymbol*>(symbol);
                symbol = boundTemplateParam->GetType();
            }
            symbolTable.MapSymbol(&identifierNode, symbol);
            MapIdentifierToSymbolDefinition(&identifierNode, symbol);
        }
    }
}

void ConstraintBinderVisitor::Visit(cmajor::ast::DotNode& dotNode)
{
    conceptGroup = nullptr;
    ns = nullptr;
    dotNode.Subject()->Accept(*this);
    boundCompileUnit.SetLatestIdentifier(dotNode.MemberId());
    if (ns)
    {
        cmajor::symbols::Symbol* symbol = ns->GetContainerScope()->Lookup(dotNode.MemberId()->Str());
        if (symbol)
        {
            if (symbol->GetSymbolType() == cmajor::symbols::SymbolType::conceptGroupSymbol)
            {
                conceptGroup = static_cast<cmajor::symbols::ConceptGroupSymbol*>(symbol);
            }
            else if (symbol->GetSymbolType() != cmajor::symbols::SymbolType::namespaceSymbol)
            {
                symbolTable.MapSymbol(dotNode.MemberId(), symbol);
            }
        }
    }
}

void BindConstraintSymbols(cmajor::ast::Node* node, cmajor::symbols::ContainerScope* containerScope, cmajor::binder::BoundCompileUnit& boundCompileUnit)
{
    ConstraintBinderVisitor visitor(containerScope, boundCompileUnit);
    node->Accept(visitor);
    boundCompileUnit.SetLatestIdentifier(visitor.PrevId());
}

} // namespace cmdoclib
