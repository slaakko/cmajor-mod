// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.inline_function_repository;

import std.core;
import cmajor.symbols;
import cmajor.ast;
import soul.ast.source.pos;
import util;

export namespace cmajor::binder {

class BoundCompileUnit;

class InlineFunctionRepository
{
public:
    InlineFunctionRepository(BoundCompileUnit& boundCompileUnit_);
    cmajor::symbols::FunctionSymbol* Instantiate(cmajor::symbols::FunctionSymbol* inlineFunction, cmajor::symbols::ContainerScope* containerScope, 
        cmajor::ast::Node* node);
private:
    BoundCompileUnit& boundCompileUnit;
    std::unordered_map<cmajor::symbols::FunctionSymbol*, cmajor::symbols::FunctionSymbol*> inlineFunctionMap;
    std::map<cmajor::symbols::FunctionSymbol*, cmajor::symbols::FunctionSymbol*> copyMap;
    cmajor::symbols::FunctionSymbol* GetCopy(cmajor::symbols::FunctionSymbol* master) const;
};

} // namespace cmajor::binder
