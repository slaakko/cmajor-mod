// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.ast.solution;

import util;

namespace cmajor::fault::tolerant::ast {

SolutionDeclaration::SolutionDeclaration(SolutionDeclarationKind kind_) : kind(kind_)
{
}

SolutionDeclaration::~SolutionDeclaration()
{
}

SolutionProjectDeclaration::SolutionProjectDeclaration(const std::string& filePath_) : SolutionDeclaration(SolutionDeclarationKind::projectDeclaration), filePath(filePath_)
{
}

ActiveProjectDeclaration::ActiveProjectDeclaration(const std::string& activeProjectName_) : 
    SolutionDeclaration(SolutionDeclarationKind::activeProjectDeclaration), activeProjectName(activeProjectName_)
{
}

Solution::Solution(const std::string& name_, const std::string& filePath_) : name(name_), filePath(filePath_), valid(true)
{
}

void Solution::AddDeclaration(SolutionDeclaration* declaration)
{
    declarations.push_back(std::unique_ptr<SolutionDeclaration>(declaration));
}

void Solution::ResolveDeclarations()
{
    std::string basePath = util::Path::GetDirectoryName(filePath);
    for (const auto& declaration : declarations)
    {
        switch (declaration->Kind())
        {
            case SolutionDeclarationKind::projectDeclaration:
            {
                SolutionProjectDeclaration* projectDeclaration = static_cast<SolutionProjectDeclaration*>(declaration.get());
                relativeProjectFilePaths.push_back(projectDeclaration->FilePath());
                projectFilePaths.push_back(util::GetFullPath(util::Path::Combine(basePath, projectDeclaration->FilePath())));
                break;
            }
            case SolutionDeclarationKind::activeProjectDeclaration:
            {
                ActiveProjectDeclaration* activeProjectDeclaration = static_cast<ActiveProjectDeclaration*>(declaration.get());
                activeProjectName = activeProjectDeclaration->ActiveProjectName();
                break;
            }
        }
    }
}

void Solution::AddProject(Project* project)
{
    project->SetIndex(projects.size());
    projects.push_back(std::unique_ptr<Project>(project));
}

std::unique_ptr<Project> Solution::ReleaseProject(int index)
{
    return std::move(projects[index]);
}

void Visit(std::vector<Project*>& buildOrder, Project* project, std::set<Project*>& visited, std::set<Project*>& tempVisit, const Solution* solution)
{
    if (tempVisit.find(project) == tempVisit.end())
    {
        if (visited.find(project) == visited.end())
        {
            tempVisit.insert(project);
            for (auto dependsOnProject : project->DependsOn())
            {
                Visit(buildOrder, dependsOnProject, visited, tempVisit, solution);
            }
            tempVisit.erase(project);
            visited.insert(project);
            buildOrder.push_back(project);
        }
    }
    else
    {
        throw std::runtime_error("circular project dependency '" + project->Name() + "' detected in dependencies of solution '" + solution->Name() + "' (" +
            util::GetFullPath(solution->FilePath()) + ")");
    }
}

std::vector<Project*> Solution::CreateBuildOrder() const
{
    std::vector<Project*> buildOrder;
    std::map<std::string, Project*> projectMap;
    for (const auto& project : projects)
    {
        projectMap[project->FilePath()] = project.get();
    }
    for (const auto& project : projects)
    {
        for (const auto& referenceFilePath : project->ReferenceFilePaths())
        {
            auto it = projectMap.find(referenceFilePath);
            if (it != projectMap.end())
            {
                Project* dependsOnProject = it->second;
                project->AddDependsOnProject(dependsOnProject);
            }
            else
            {
                throw std::runtime_error("referenced project '" + referenceFilePath + "' not found from solution '" + name + "'");
            }
        }
    }
    std::set<Project*> visited;
    std::set<Project*> tempVisit;
    for (const auto& project : projects)
    {
        if (visited.find(project.get()) == visited.end())
        {
            Visit(buildOrder, project.get(), visited, tempVisit, this);
        }
    }
    return buildOrder;
}

} // namespace cmajor::fault::tolerant::ast {
