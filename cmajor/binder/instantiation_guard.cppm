// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.instantiation_guard;

import cmajor.symbols;
import util;
import std.core;

export namespace cmajor::binder {

class InstantiationGuard
{
public:
    InstantiationGuard(cmajor::symbols::SymbolTable& symbolTable_, int32_t fileIndex, const util::uuid& moduleId);
    ~InstantiationGuard();
private:
    cmajor::symbols::SymbolTable& symbolTable;
};

} // namespace cmajor::binder
