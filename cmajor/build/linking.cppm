// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.linking;

import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::build {

void Link(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule);

} // namespace cmajor::build
