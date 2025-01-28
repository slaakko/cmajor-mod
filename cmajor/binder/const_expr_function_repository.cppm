// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.const_expr_function_repository;

import std.core;
import cmajor.symbols;
import cmajor.ast;

export namespace cmajor::binder {

class BoundCompileUnit;

class ConstExprFunctionRepository
{
public:
    ConstExprFunctionRepository(BoundCompileUnit& boundCompileUnit_);
    cmajor::ast::FunctionNode* GetFunctionNodeFor(cmajor::symbols::FunctionSymbol* constExprFunctionSymbol);
private:
    BoundCompileUnit& boundCompileUnit;
};

} // namespace cmajor::binder
