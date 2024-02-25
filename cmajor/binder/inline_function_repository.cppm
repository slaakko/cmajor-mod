// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.inline_function_repository;

import std.core;
import cmajor.symbols;
import soul.ast.source.pos;
import util;

export namespace cmajor::binder {

class BoundCompileUnit;

class InlineFunctionRepository
{
public:
    InlineFunctionRepository(BoundCompileUnit& boundCompileUnit_);
    cmajor::symbols::FunctionSymbol* Instantiate(cmajor::symbols::FunctionSymbol* inlineFunction, cmajor::symbols::ContainerScope* containerScope, 
        const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
private:
    BoundCompileUnit& boundCompileUnit;
    std::unordered_map<cmajor::symbols::FunctionSymbol*, cmajor::symbols::FunctionSymbol*> inlineFunctionMap;
};

} // namespace cmajor::binder
