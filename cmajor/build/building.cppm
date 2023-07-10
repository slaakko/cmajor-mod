// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.building;

import std.core;
import cmajor.ast;
import cmajor.symbols;
import cmajor.ir;

export namespace cmajor::build {

void BuildProject(const std::string& projectFilePath, std::unique_ptr<cmajor::symbols::Module>& rootModule, cmajor::ir::EmittingContext* emittingContext, 
    std::set<std::string>& builtProjects);

void BuildProject(cmajor::ast::Project* project, std::unique_ptr<cmajor::symbols::Module>& rootModule, cmajor::ir::EmittingContext* emittingContext, 
    bool& stop, bool resetRootModule, std::set<std::string>& builtProjects);

void BuildSolution(const std::string& solutionFilePath, std::vector<std::unique_ptr<cmajor::symbols::Module>>& rootModules, cmajor::ir::EmittingContext* emittingContext);

void StopBuild();

void ResetStopBuild();

} // namespace cmajor::build
