// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.evaluator;

import std.core;
import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::binder {

class BoundCompileUnit;
class BoundFunction;

std::unique_ptr<cmajor::symbols::Value> Evaluate(cmajor::ast::Node* node, cmajor::symbols::TypeSymbol* targetType, cmajor::symbols::ContainerScope* containerScope, 
    BoundCompileUnit& boundCompileUnit, bool dontThrow, BoundFunction* currentFunction, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);

} // namespace cmajor::binder
