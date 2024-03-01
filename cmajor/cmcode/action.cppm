// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.action;

import cmajor.info;
import cmajor.ast;
import wing;
import std.core;

export namespace cmcode {

class MainWindow;

class BuildSolutionAction : public wing::ClickAction
{
public:
    BuildSolutionAction(wing::MenuItem* menuItem, MainWindow* mainWindow_);
    void Execute() override;
private:
    MainWindow* mainWindow;
};

class RebuildSolutionAction : public wing::ClickAction
{
public:
    RebuildSolutionAction(wing::MenuItem* menuItem, MainWindow* mainWindow_);
    void Execute() override;
private:
    MainWindow* mainWindow;
};

class CleanSolutionAction : public wing::ClickAction
{
public:
    CleanSolutionAction(wing::MenuItem* menuItem, MainWindow* mainWindow_);
    void Execute() override;
private:
    MainWindow* mainWindow;
};

class BuildProjectAction : public wing::ClickAction
{
public:
    BuildProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
};

class RebuildProjectAction : public wing::ClickAction
{
public:
    RebuildProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
};

class CleanProjectAction : public wing::ClickAction
{
public:
    CleanProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
};

class AddNewProjectAction : public wing::ClickAction
{
public:
    AddNewProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_);
    void Execute() override;
private:
    MainWindow* mainWindow;
};

class AddExistingProjectAction : public wing::ClickAction
{
public:
    AddExistingProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_);
    void Execute() override;
private:
    MainWindow* mainWindow;
};

class SetActiveProjectAction : public wing::ClickAction
{
public:
    SetActiveProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* newActiveProjectNode_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
    wing::TreeViewNode* newActiveProjectNode;
};

class RemoveProjectAction : public wing::ClickAction
{
public:
    RemoveProjectAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
};

class OpenFileLocationAction : public wing::ClickAction
{
public:
    OpenFileLocationAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, const std::string& filePath_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    std::string filePath;
};

class AddNewSourceFileAction : public wing::ClickAction
{
public:
    AddNewSourceFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
    wing::TreeViewNode* projectNode;
};

class AddExistingSourceFileAction : public wing::ClickAction
{
public:
    AddExistingSourceFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
    wing::TreeViewNode* projectNode;
};

class AddNewResourceFileAction : public wing::ClickAction
{
public:
    AddNewResourceFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
    wing::TreeViewNode* projectNode;
};

class AddExistingResourceFileAction : public wing::ClickAction
{
public:
    AddExistingResourceFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
    wing::TreeViewNode* projectNode;
};

class AddNewTextFileAction : public wing::ClickAction
{
public:
    AddNewTextFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
    wing::TreeViewNode* projectNode;
};

class AddExistingTextFileAction : public wing::ClickAction
{
public:
    AddExistingTextFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, wing::TreeViewNode* projectNode_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
    wing::TreeViewNode* projectNode;
};

class RemoveFileAction : public wing::ClickAction
{
public:
    RemoveFileAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, const std::string& filePath_, const std::string& fileName_, wing::TreeViewNode* fileNode_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
    std::string filePath;
    std::string fileName;
    wing::TreeViewNode* fileNode;
};

class GotoDefinitionAction : public wing::ClickAction
{
public:
    GotoDefinitionAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_, const std::string& identifier_, 
        const cmajor::info::bs::DefinitionSourceLocation& sourceLocation_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
    std::string identifier;
    cmajor::info::bs::DefinitionSourceLocation sourceLocation;
};

class GotoCursorAction : public wing::ClickAction
{
public:
    GotoCursorAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, const cmajor::info::db::SourceLoc& sourceLocation_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::info::db::SourceLoc sourceLocation;
};

class ProjectReferencesAction : public wing::ClickAction
{
public:
    ProjectReferencesAction(wing::MenuItem* menuItem, MainWindow* mainWindow_, cmajor::ast::Project* project_);
    void Execute() override;
private:
    MainWindow* mainWindow;
    cmajor::ast::Project* project;
};

} // namespace cmcode
