// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.main.unit;

import cmajor.ast;
import cmajor.symbols;
import std.core;

export namespace cmajor::build {

void GenerateMainUnit(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths);

} // namespace cmajor::build
