// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.ast.solution;

import cmajor.ast.compile.unit;
import cmajor.ast.project;
import std.core;
import std.filesystem;

export namespace cmajor::ast {

class SolutionDeclaration
{
public:
    SolutionDeclaration();
    SolutionDeclaration(const SolutionDeclaration&) = delete;
    SolutionDeclaration& operator=(const SolutionDeclaration&) = delete;
    virtual ~SolutionDeclaration();
};

class SolutionProjectDeclaration : public SolutionDeclaration
{
public:
    SolutionProjectDeclaration(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
private:
    std::string filePath;
};

class SolutionActiveProjectDeclaration : public SolutionDeclaration
{
public:
    SolutionActiveProjectDeclaration(const std::u32string& activeProjectName_);
    const std::u32string& ActiveProjectName() const { return activeProjectName; }
private:
    std::u32string activeProjectName;
};

class ProjectDependencyDeclaration : public SolutionDeclaration
{
public:
    ProjectDependencyDeclaration(const std::u32string& projectName_);
    ProjectDependencyDeclaration(const ProjectDependencyDeclaration&) = delete;
    ProjectDependencyDeclaration& operator=(const ProjectDependencyDeclaration&) = delete;
    void AddDependency(const std::u32string& dependsOn);
    const std::u32string& ProjectName() const { return projectName; }
    const std::vector<std::u32string>& DependsOnProjects() const { return dependsOnProjects; }
private:
    std::u32string projectName;
    std::vector<std::u32string> dependsOnProjects;
};

class Solution
{
public:
    Solution(const std::u32string& name_, const std::string& filePath_);
    Solution(const Solution&) = delete;
    Solution& operator=(const Solution&) = delete;
    const std::u32string& Name() const { return name; }
    const std::string& FilePath() const { return filePath; }
    const std::vector<std::unique_ptr<Project>>& Projects() const { return projects; }
    const std::filesystem::path& BasePath() const { return basePath; }
    const std::vector<std::string>& ProjectFilePaths() const { return projectFilePaths; }
    const std::vector<std::string>& RelativeProjectFilePaths() const { return relativeProjectFilePaths; }
    const std::u32string& ActiveProjectName() const { return activeProjectName; }
    void AddProject(std::unique_ptr<Project>&& project);
    bool HasProject(const std::u32string& projectName) const;
    std::vector<Project*> CreateBuildOrder();
    void AddDeclaration(SolutionDeclaration* declaration);
    void ResolveDeclarations();
    Project* ActiveProject() const { return activeProject; }
    void SetActiveProject(Project* activeProject_) { activeProject = activeProject_; }
    void SortByProjectName();
    void Save();
    void RemoveProject(Project* project);
private:
    std::u32string name;
    std::string filePath;
    std::filesystem::path basePath;
    std::vector<std::unique_ptr<SolutionDeclaration>> declarations;
    std::vector<std::string> projectFilePaths;
    std::vector<std::string> relativeProjectFilePaths;
    std::vector<std::unique_ptr<Project>> projects;
    Project* activeProject;
    std::u32string activeProjectName;
    std::vector<std::unique_ptr<ProjectDependencyDeclaration>> additionalDependencyDeclarations;
    std::unordered_map<std::u32string, ProjectDependencyDeclaration*> dependencyMap;
    void AddDependencies();
};

} // namespace cmajor::ast
