// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.binder.const_expr_function_repository;

import cmajor.binder.type.binder;
import cmajor.binder.bound.compile.unit;

namespace cmajor::binder {

ConstExprFunctionRepository::ConstExprFunctionRepository(BoundCompileUnit& boundCompileUnit_) : boundCompileUnit(boundCompileUnit_)
{
}

cmajor::ast::FunctionNode* ConstExprFunctionRepository::GetFunctionNodeFor(cmajor::symbols::FunctionSymbol* constExprFunctionSymbol)
{
    cmajor::ast::Node* node = boundCompileUnit.GetSymbolTable().GetNodeNoThrow(constExprFunctionSymbol);
    if (!node)
    {
        node = constExprFunctionSymbol->GetFunctionNode();
    }
    cmajor::ast::FunctionNode* functionNode = static_cast<cmajor::ast::FunctionNode*>(node);
    boundCompileUnit.GetSymbolTable().MapNode(node, constExprFunctionSymbol);
    if (constExprFunctionSymbol->IsProject() && !constExprFunctionSymbol->IsBound())
    {
        TypeBinder typeBinder(boundCompileUnit);
        typeBinder.SetContainerScope(constExprFunctionSymbol->GetContainerScope());
        typeBinder.SetCurrentFunctionSymbol(constExprFunctionSymbol);
        functionNode->Accept(typeBinder);
    }
    return functionNode;
}

} // namespace cmajor::binder
