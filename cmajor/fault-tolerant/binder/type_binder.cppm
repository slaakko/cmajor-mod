// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.binder.type.binder;

import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.symbols;
import std.core;

export namespace cmajor::fault::tolerant::binder {

class BoundCompileUnit;

class TypeBinder : public cmajor::fault::tolerant::ast::DefaultVisitor
{
public:
    TypeBinder(BoundCompileUnit* boundCompileUnit_, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable_);
    void SetScope(cmajor::fault::tolerant::symbols::ContainerScope* scope_) { scope = scope_; }
    void Visit(cmajor::fault::tolerant::ast::CompileUnitNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::NamespaceDefinitionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::NamespaceImportNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::UsingAliasNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::TypedefAliasNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::StaticConstructorNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstructorNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DestructorNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::MemberFunctionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConversionFunctionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::MemberVariableNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstantNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DelegateNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ClassDelegateNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::EnumConstantNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::EnumTypeNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::FunctionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::GlobalVariableNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::CompoundStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstructionStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ClassNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::InterfaceNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConceptNode& node) override;
private:
    BoundCompileUnit* boundCompileUnit;
    cmajor::fault::tolerant::symbols::SymbolTable* symbolTable;
    cmajor::fault::tolerant::symbols::ContainerScope* scope;
};

void BindTypes(cmajor::fault::tolerant::ast::CompileUnitNode* compileUnitNode, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable);

} // namespace cmajor::fault::tolerant::binder
