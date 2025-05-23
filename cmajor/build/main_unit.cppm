// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.main.unit;

import cmajor.ast;
import cmajor.symbols;
import std.core;

export namespace cmajor::build {

void GenerateMainUnit(cmajor::ast::Project* project, cmajor::symbols::Context* context, std::vector<std::string>& objectFilePaths, std::vector<std::string>& cppFilePaths);

} // namespace cmajor::build
