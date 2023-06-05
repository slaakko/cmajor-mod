// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.llvm.link.windows;

import std.core;
import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::llvm {

void LinkWindows(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule);

} // cmajor::llvm
