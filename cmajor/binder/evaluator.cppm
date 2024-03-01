// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.evaluator;

import std.core;
import cmajor.ast;
import cmajor.symbols;
import soul.ast.source.pos;
import util;

export namespace cmajor::binder {

class BoundCompileUnit;
class BoundFunction;

std::unique_ptr<cmajor::symbols::Value> Evaluate(cmajor::ast::Node* node, cmajor::symbols::TypeSymbol* targetType, cmajor::symbols::ContainerScope* containerScope, 
    BoundCompileUnit& boundCompileUnit, bool dontThrow, BoundFunction* currentFunction);

} // namespace cmajor::binder
