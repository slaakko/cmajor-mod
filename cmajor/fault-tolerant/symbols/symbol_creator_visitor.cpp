// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.symbols.symbol.creator.visitor;

import cmajor.fault.tolerant.symbols.symbol.table;
import cmajor.fault.tolerant.symbols.source;

namespace cmajor::fault::tolerant::symbols {

SymbolCreatorVisitor::SymbolCreatorVisitor(SymbolTable* symbolTable_, Source* source_) : symbolTable(symbolTable_), paramIndex(0), blockCount(0), source(source_)
{
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::NamespaceDefinitionNode& node)
{
    if (node.NamespaceName())
    {
        for (cmajor::fault::tolerant::ast::IdentifierNode* identifier : node.NamespaceName()->IdentifierNodes())
        {
            if (!symbolTable->BeginNamespace(identifier, source, &errors, true))
            {
                return;
            }
            symbolTable->MapNode(identifier, symbolTable->Container());
        }
    }
    symbolTable->MapNode(&node, symbolTable->Container());
    node.Content().Accept(*this);
    if (node.NamespaceName())
    {
        int n = node.NamespaceName()->IdentifierNodes().size();
        for (int i = 0; i < n; ++i)
        {
            symbolTable->EndNamespace();
        }
    }
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::UsingAliasNode& node)
{
    symbolTable->AddAliasType(node, source);
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::TypedefAliasNode& node)
{
    symbolTable->AddAliasType(node, source);
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::ConstantNode& node)
{
    symbolTable->AddConstant(node, source);
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::DelegateNode& node)
{
    symbolTable->BeginDelegate(node, source);
    paramIndex = 0;
    node.ParameterList()->Accept(*this);
    symbolTable->EndDelegate();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::ClassDelegateNode& node)
{
    symbolTable->BeginClassDelegate(node, source);
    paramIndex = 0;
    node.ParameterList()->Accept(*this);
    symbolTable->EndClassDelegate();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::EnumTypeNode& node)
{
    symbolTable->BeginEnumType(node, source);
    node.EnumConstantList()->Accept(*this);
    symbolTable->EndEnumType();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::EnumConstantNode& node)
{
    symbolTable->AddEnumConstant(node, source);
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::TemplateParameterNode& node)
{
    symbolTable->AddTemplateParameter(node, source);
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::FunctionNode& node)
{
    blockCount = 0;
    symbolTable->BeginFunction(node, source);
    node.TemplateParameterList()->Accept(*this);
    paramIndex = 0;
    node.ParameterList()->Accept(*this);
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    symbolTable->EndFunction();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::StaticConstructorNode& node)
{
    blockCount = 0;
    symbolTable->BeginStaticConstructor(node, source);
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    symbolTable->EndStaticConstructor();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::ConstructorNode& node)
{
    blockCount = 0;
    symbolTable->BeginConstructor(node, source);
    paramIndex = 0;
    node.ParameterList()->Accept(*this);
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    symbolTable->EndConstructor();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::DestructorNode& node)
{
    blockCount = 0;
    symbolTable->BeginDestructor(node, source);
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    symbolTable->EndDestructor();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::MemberFunctionNode& node)
{
    blockCount = 0;
    symbolTable->BeginMemberFunction(node, source);
    paramIndex = 0;
    node.ParameterList()->Accept(*this);
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    symbolTable->EndMemberFunction();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::ConversionFunctionNode& node)
{
    blockCount = 0;
    symbolTable->BeginConversionFunction(node, source);
    if (node.Body())
    {
        node.Body()->Accept(*this);
    }
    symbolTable->EndConversionFunction();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::ParameterNode& node)
{
    symbolTable->AddParameter(node, paramIndex, source);
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::CompoundStatementNode& node)
{
    symbolTable->BeginDeclarationBlock(node, source, blockCount);
    node.Statements()->Accept(*this);
    symbolTable->EndDeclarationBlock();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::ConstructionStatementNode& node)
{
    symbolTable->AddLocalVariable(node, source);
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::MemberVariableNode& node)
{
    symbolTable->AddMemberVariable(node, source);
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::GlobalVariableNode& node)
{
    symbolTable->AddGlobalVariable(node, source);
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::ClassNode& node)
{
    symbolTable->BeginClass(node, source);
    node.TemplateParameterList()->Accept(*this);
    for (const auto& member : node.Members().Nodes())
    {
        member->Accept(*this);
    }
    symbolTable->EndClass();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::InterfaceNode& node)
{
    symbolTable->BeginInterface(node, source);
    for (const auto& function : node.MemberFunctions()->Nodes())
    {
        function->Accept(*this);
    }
    symbolTable->EndInterface();
}

void SymbolCreatorVisitor::Visit(cmajor::fault::tolerant::ast::ConceptNode& node)
{
    symbolTable->BeginConcept(node, source);
    for (const auto& typeParam : node.TypeParameterList()->Nodes())
    {
        if (!typeParam->IsComma())
        {
            symbolTable->AddTypeParameter(*static_cast<cmajor::fault::tolerant::ast::IdentifierNode*>(typeParam.get()), source);
        }
    }
    symbolTable->EndConcept();
}

void CreateSymbols(cmajor::fault::tolerant::ast::CompileUnitNode* compileUnit, SymbolTable* symbolTable, Source* source)
{
    SymbolCreatorVisitor visitor(symbolTable, source);
    compileUnit->Accept(visitor);
}

} // namespace cmajor::fault::tolerant::symbols
