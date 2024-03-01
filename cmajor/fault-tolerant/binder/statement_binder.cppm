// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.binder.statement.binder;

import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.symbols;
import std.core;

export namespace cmajor::fault::tolerant::binder {

class BoundCompileUnit;

class StatementBinder : public cmajor::fault::tolerant::ast::DefaultVisitor
{
public:
    StatementBinder(BoundCompileUnit* boundCompileUnit_, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_);
    void Visit(cmajor::fault::tolerant::ast::CompileUnitNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::NamespaceDefinitionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::NamespaceImportNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::UsingAliasNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::InitializerListNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ThisInitializerNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::BaseInitializerNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::MemberInitializerNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DelegateNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ClassDelegateNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::FunctionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::StaticConstructorNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstructorNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DestructorNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::MemberFunctionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConversionFunctionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::CompoundStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ReturnStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::IfStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::WhileStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DoStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::RangeForStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ForStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::BreakStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ContinueStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::GotoStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::GotoCaseStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::GotoDefaultStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::CaseStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DefaultStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::SwitchStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::AssignmentStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ExpressionStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::EmptyStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstructionStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DeleteStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DestroyStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ThrowStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::CatchNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::TryStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::LabeledStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::AssertStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::CondCompStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::IfGroupNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ElifGroupNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ElseGroupNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ClassNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::InterfaceNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConceptNode& node) override;
private:
    BoundCompileUnit* boundCompileUnit;
    cmajor::fault::tolerant::symbols::SymbolTable* symbolTable;
    cmajor::fault::tolerant::symbols::ContainerScope* scope;
};

void BindStatements(cmajor::fault::tolerant::ast::CompileUnitNode* node, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable);

} // namespace cmajor::fault::tolerant::binder
