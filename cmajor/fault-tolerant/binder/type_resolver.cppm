// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.binder.type.resolver;

import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.symbols;
import std.core;

export namespace cmajor::fault::tolerant::binder {

class BoundCompileUnit;

cmajor::fault::tolerant::symbols::TypeSymbol* ResolveType(
    cmajor::fault::tolerant::ast::Node* node, 
    cmajor::fault::tolerant::symbols::ContainerScope* scope, 
    cmajor::fault::tolerant::symbols::SymbolTable* symbolTable,
    BoundCompileUnit* boundCompileUnit);

} // namespace cmajor::fault::tolerant::binder
