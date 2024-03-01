// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.binder.concept_binder;

import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.symbols;
import std.core;

export namespace cmajor::fault::tolerant::binder {

class BoundCompileUnit;

class ConceptBinder : public cmajor::fault::tolerant::ast::DefaultVisitor
{
public:
    ConceptBinder(BoundCompileUnit* boundCompileUnit_, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_, cmajor::fault::tolerant::symbols::ContainerScope* scope_);
    void Visit(cmajor::fault::tolerant::ast::DisjunctiveConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConjunctiveConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ParenthesizedConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::PredicateConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::IsConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::MultiparamConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::TypeNameConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstructorConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DestructorConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::MemberFunctionConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::FunctionConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::WhereConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::EmbeddedConstraintNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::AxiomStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::AxiomNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConceptNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConceptIdNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::IdentifierNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::QualifiedIdNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DotMemberNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::TemplateIdNode& node) override;
private:
    BoundCompileUnit* boundCompileUnit;
    cmajor::fault::tolerant::symbols::SymbolTable* symbolTable;
    cmajor::fault::tolerant::symbols::ContainerScope* scope;
    cmajor::fault::tolerant::symbols::Symbol* symbol;
};

void BindConstraint(cmajor::fault::tolerant::ast::Node* constraint, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable,
    cmajor::fault::tolerant::symbols::ContainerScope* scope);

void BindConcept(cmajor::fault::tolerant::ast::ConceptNode* concept_, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable,
    cmajor::fault::tolerant::symbols::ContainerScope* scope);

} // namespace cmajor::fault::tolerant::binder
