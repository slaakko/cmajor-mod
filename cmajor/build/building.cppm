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

std::unique_ptr<cmajor::ast::Project> ReadProject(const std::string& projectFilePath);

class ProjectSet
{
public:
    ProjectSet();
    bool ProjectStartedToBuild(const std::string& projectFilePath);
private:
    std::mutex mtx;
    std::set<std::string> projectsStartedToBuild;
};

void BuildProject(const std::string& projectFilePath, std::unique_ptr<cmajor::symbols::Module>& rootModule, ProjectSet& projectSet);

void BuildProject(cmajor::ast::Project* project, std::unique_ptr<cmajor::symbols::Module>& rootModule, bool& stop, bool resetRootModule, ProjectSet& projectSet);

void BuildSolution(const std::string& solutionFilePath, std::vector<std::unique_ptr<cmajor::symbols::Module>>& rootModules);

void BuildSolution(const std::string& solutionFilePath, std::vector<std::unique_ptr<cmajor::symbols::Module>>& rootModules, 
    std::string& solutionName,  std::vector<std::string>& moduleNames);

void StopBuild();

void ResetStopBuild();

void Install(const std::string& projectFilePath);

} // namespace cmajor::build
