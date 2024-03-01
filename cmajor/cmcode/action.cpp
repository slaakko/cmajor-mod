// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.action;

import cmcode.main.window;

namespace cmcode {

BuildSolutionAction::BuildSolutionAction(wing::MenuItem* menuItem, MainWindow* mainWindow_) : ClickAction(menuItem), mainWindow(mainWindow_)
{
}

void BuildSolutionAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->BuildSolutionClick();
}

RebuildSolutionAction::RebuildSolutionAction(wing::MenuItem* menuItem, MainWindow* mainWindow_) : ClickAction(menuItem), mainWindow(mainWindow_)
{
}

void RebuildSolutionAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->RebuildSolutionClick();
}

CleanSolutionAction::CleanSolutionAction(wing::MenuItem* menuItem, MainWindow* mainWindow_) : ClickAction(menuItem), mainWindow(mainWindow_)
{
}

void CleanSolutionAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->CleanSolutionClick();
}

BuildProjectAction::BuildProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_) : ClickAction(menuItem), mainWindow(mainWindow_), project(project_)
{
}

void BuildProjectAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->BuildProject(project);
}

RebuildProjectAction::RebuildProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_) : ClickAction(menuItem), mainWindow(mainWindow_), project(project_)
{
}

void RebuildProjectAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->RebuildProject(project);
}

CleanProjectAction::CleanProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_) : ClickAction(menuItem), mainWindow(mainWindow_), project(project_)
{
}

void CleanProjectAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->CleanProject(project);
}

AddNewProjectAction::AddNewProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_) : ClickAction(menuItem), mainWindow(mainWindow_)
{
}

void AddNewProjectAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->AddNewProject();
}

AddExistingProjectAction::AddExistingProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_) : ClickAction(menuItem), mainWindow(mainWindow_)
{
}

void AddExistingProjectAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->AddExistingProject();
}

SetActiveProjectAction::SetActiveProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* newActiveProjectNode_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_), newActiveProjectNode(newActiveProjectNode_)
{
}

void SetActiveProjectAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->SetActiveProject(project, newActiveProjectNode);
}

OpenFileLocationAction::OpenFileLocationAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, const std::string& filePath_) : ClickAction(menuItem), mainWindow(mainWindow_), filePath(filePath_)
{
}

void OpenFileLocationAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->OpenFileLocation(filePath);
}

RemoveProjectAction::RemoveProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_) : ClickAction(menuItem), mainWindow(mainWindow_), project(project_)
{
}

void RemoveProjectAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->RemoveProject(project);
}

AddNewSourceFileAction::AddNewSourceFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_), projectNode(projectNode_)
{
}

void AddNewSourceFileAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->AddNewSourceFile(project, projectNode);
}

AddExistingSourceFileAction::AddExistingSourceFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_), projectNode(projectNode_)
{
}

void AddExistingSourceFileAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->AddExistingSourceFile(project, projectNode);
}

AddNewResourceFileAction::AddNewResourceFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_), projectNode(projectNode_)
{
}

void AddNewResourceFileAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->AddNewResourceFile(project, projectNode);
}

AddExistingResourceFileAction::AddExistingResourceFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_), projectNode(projectNode_)
{
}

void AddExistingResourceFileAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->AddExistingResourceFile(project, projectNode);
}

AddNewTextFileAction::AddNewTextFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_), projectNode(projectNode_)
{
}

void AddNewTextFileAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->AddNewTextFile(project, projectNode);
}

AddExistingTextFileAction::AddExistingTextFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_), projectNode(projectNode_)
{
}

void AddExistingTextFileAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->AddExistingTextFile(project, projectNode);
}

RemoveFileAction::RemoveFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, const std::string& filePath_, const std::string& fileName_, wing::TreeViewNode* fileNode_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_), filePath(filePath_), fileName(fileName_), fileNode(fileNode_)
{
}

void RemoveFileAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->RemoveFile(project, filePath, fileName, fileNode);
}

GotoDefinitionAction::GotoDefinitionAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, const std::string& identifier_, 
    const cmajor::info::bs::DefinitionSourceLocation& sourceLocation_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_), identifier(identifier_), sourceLocation(sourceLocation_)
{
}

void GotoDefinitionAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->GotoDefinition(project, identifier, sourceLocation);
}

GotoCursorAction::GotoCursorAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, const cmajor::info::db::SourceLoc& sourceLocation_) : 
    ClickAction(menuItem), mainWindow(mainWindow_), sourceLocation(sourceLocation_)
{
}

void GotoCursorAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->GotoCursor(sourceLocation);
}

ProjectReferencesAction::ProjectReferencesAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_) :
    ClickAction(menuItem), mainWindow(mainWindow_), project(project_)
{
}

void ProjectReferencesAction::Execute()
{
    mainWindow->HideContextMenu();
    mainWindow->SetProjectReferences(project);
}

} // namespace cmcode
