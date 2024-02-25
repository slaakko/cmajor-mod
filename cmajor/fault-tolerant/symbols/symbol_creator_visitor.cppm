// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.symbols.symbol.creator.visitor;

import cmajor.fault.tolerant.symbols.error.container;
import cmajor.fault.tolerant.ast;
import std.core;

export namespace cmajor::fault::tolerant::symbols {

class SymbolTable;
class Source;

class SymbolCreatorVisitor : public cmajor::fault::tolerant::ast::DefaultVisitor
{
public:
    SymbolCreatorVisitor(SymbolTable* symbolTable_, Source* source_);
    void Visit(cmajor::fault::tolerant::ast::NamespaceDefinitionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::UsingAliasNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::TypedefAliasNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstantNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DelegateNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ClassDelegateNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::EnumTypeNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::EnumConstantNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::TemplateParameterNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::FunctionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::StaticConstructorNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstructorNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::DestructorNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::MemberFunctionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConversionFunctionNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ParameterNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::CompoundStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConstructionStatementNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::MemberVariableNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::GlobalVariableNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ClassNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::InterfaceNode& node) override;
    void Visit(cmajor::fault::tolerant::ast::ConceptNode& node) override;
private:
    SymbolTable* symbolTable;
    int paramIndex;
    int blockCount;
    ErrorContainer errors;
    Source* source;
};

void CreateSymbols(cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit, SymbolTable* symbolTable, Source* source);

} // namespace cmajor::fault::tolerant::symbols
