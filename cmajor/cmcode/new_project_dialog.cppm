// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.new_project.dialog;

import wing;
import cmajor.ast;
import std.core;

export namespace cmcode {

bool IsValidProjectOrSolutionName(const std::string& text);

class NewProjectDialog : public wing::Window
{
public:
    NewProjectDialog();
    std::string GetProjectName() const;
    cmajor::ast::Target GetProjectType() const { return projectType; }
    std::string GetProjectLocation() const;
    void SetProjectLocation(const std::string& location);
    std::string GetSolutionName() const;
    bool PlaceSolutionInSameDirectory() const;
private:
    void ProjectNameTextBoxTextChanged();
    void SelectProjectTypeButtonClick();
    void ProjectLocationTextBoxTextChanged();
    void SelectProjectLocationButtonClick();
    void SolutionNameTextBoxTextChanged();
    void PlaceSolutionInSameDirectoryCheckBoxCheckedChanged();
    bool Valid() const;
    std::string GetProjectTypeName(cmajor::ast::Target projectType);
    wing::TextBox* projectNameTextBox;
    wing::Label* projectTypeTextLabel;
    wing::TextBox* projectLocationTextBox;
    wing::TextBox* solutionNameTextBox;
    wing::CheckBox* placeSolutionInSameDirectoryCheckBox;
    wing::Button* cancelButton;
    wing::Button* createButton;
    cmajor::ast::Target projectType;
    std::map<cmajor::ast::Target, std::string> projectTypeNameMap;
};

} // namespace cmcode
