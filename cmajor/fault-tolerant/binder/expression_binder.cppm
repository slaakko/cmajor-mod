// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.binder.expression.binder;

import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.symbols;
import std.core;

export namespace cmajor::fault::tolerant::binder {

class BoundCompileUnit;

void BindExpression(cmajor::fault::tolerant::ast::Node* expression, BoundCompileUnit* boundCompileUnit, cmajor::fault::tolerant::symbols::SymbolTable* symbolTable,
    cmajor::fault::tolerant::symbols::ContainerScope* scope);

} // namespace cmajor::fault::tolerant::binder
