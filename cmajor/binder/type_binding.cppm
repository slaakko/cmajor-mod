// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.binder.type.binding;

import std.core;
import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::binder {

std::vector<std::string> BindTypes(cmajor::symbols::Module* module, cmajor::ast::CompileUnitNode* compileUnit);

} // namespace cmajor::binder
