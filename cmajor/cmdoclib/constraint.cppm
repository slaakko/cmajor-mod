// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmdoclib.constraint;

import cmajor.ast;
import cmajor.symbols;
import cmajor.binder;
import std.core;

export namespace cmdoclib {

void SetConstraintSymbolBinder();
void BindConstraintSymbols(cmajor::ast::Node* node, cmajor::symbols::ContainerScope* containerScope, cmajor::binder::BoundCompileUnit& boundCompileUnit);

} // namespace cmdoclib
