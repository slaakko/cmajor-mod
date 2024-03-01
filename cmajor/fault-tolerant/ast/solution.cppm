// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.fault.tolerant.ast.solution;

import cmajor.fault.tolerant.ast.project;
import std.core;

export namespace cmajor::fault::tolerant::ast {

enum class SolutionDeclarationKind
{
    projectDeclaration, activeProjectDeclaration
};

class SolutionDeclaration
{
public:
    SolutionDeclaration(SolutionDeclarationKind kind_);
    virtual ~SolutionDeclaration();
    SolutionDeclarationKind Kind() const { return kind; }
private:
    SolutionDeclarationKind kind;
};

class SolutionProjectDeclaration : public SolutionDeclaration
{
public:
    SolutionProjectDeclaration(const std::string& filePath_);
    const std::string& FilePath() const { return filePath; }
private:
    std::string filePath;
};

class ActiveProjectDeclaration : public SolutionDeclaration
{
public:
    ActiveProjectDeclaration(const std::string& activeProjectName_);
    const std::string& ActiveProjectName()  const { return activeProjectName; }
private:
    std::string activeProjectName;
};

class Solution
{
public:
    Solution(const std::string& name_, const std::string& filePath_);
    const std::string& Name() const { return name; }
    const std::string& FilePath() const { return filePath; }
    const std::string& ActiveProjectName() const { return activeProjectName; }
    void AddDeclaration(SolutionDeclaration* declaration);
    void ResolveDeclarations();
    const std::vector<std::string>& RelativeProjectFilePaths() const { return relativeProjectFilePaths; }
    const std::vector<std::string>& ProjectFilePaths() const { return projectFilePaths; }
    void SetValid(bool valid_) { valid = valid_; }
    bool IsValid() const { return valid; }
    void AddProject(Project* project);
    std::unique_ptr<Project> ReleaseProject(int index);
    const std::vector<std::unique_ptr<Project>>& Projects() const { return projects; }
    std::vector<Project*> CreateBuildOrder() const;
private:
    std::string name;
    std::string filePath;
    std::vector<std::unique_ptr<SolutionDeclaration>> declarations;
    std::string activeProjectName;
    std::vector<std::string> relativeProjectFilePaths;
    std::vector<std::string> projectFilePaths;
    std::vector<std::unique_ptr<Project>> projects;
    bool valid;
};

} // namespace cmajor::fault::tolerant::ast {
