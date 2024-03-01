// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.resources;

import cmajor.symbols;
import cmajor.ast;
import std.core;

export namespace cmajor::build {

void AddResources(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths);

} // namespace cmajor::build
