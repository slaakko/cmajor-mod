// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.building;

import std.core;
import cmajor.ast;
import cmajor.symbols;
import cmajor.ir;

export namespace cmajor::build {

void BuildProject(const std::string& projectFilePath, std::unique_ptr<cmajor::symbols::Module>& rootModule, std::set<std::string>& builtProjects);

void BuildProject(cmajor::ast::Project* project, std::unique_ptr<cmajor::symbols::Module>& rootModule, bool& stop, bool resetRootModule, std::set<std::string>& builtProjects);

void BuildSolution(const std::string& solutionFilePath, std::vector<std::unique_ptr<cmajor::symbols::Module>>& rootModules);

void BuildSolution(const std::string& solutionFilePath, std::vector<std::unique_ptr<cmajor::symbols::Module>>& rootModules, 
    std::string& solutionName,  std::vector<std::string>& moduleNames);

void StopBuild();

void ResetStopBuild();

} // namespace cmajor::build
