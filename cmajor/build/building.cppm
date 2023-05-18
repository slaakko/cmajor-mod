// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.building;

import std.core;
import cmajor.ast;
import cmajor.symbols;

export namespace cmajor::build {

void BuildProject(const std::string& projectFilePath, std::unique_ptr<cmajor::symbols::Module>& rootModule, std::set<std::string>& builtProjects);
void BuildProject(cmajor::ast::Project* project, std::unique_ptr<cmajor::symbols::Module>& rootModule, bool& stop, bool resetRootModule, std::set<std::string>& builtProjects);

} // namespace cmajor::build
