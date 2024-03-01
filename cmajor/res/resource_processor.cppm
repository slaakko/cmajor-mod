// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.resource.processor;

import cmajor.ast;
import cmajor.symbols;
import std.core;

export namespace cmajor::resources {

void ProcessResourcesInProject(cmajor::ast::Project* project, cmajor::symbols::Module* module);

} // namespace cmajor::cmpm
