// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.add_new_project.dialog;

import cmajor.ast;
import wing;
import std.core;

export namespace cmcode {

class AddNewProjectDialog : public wing::Window
{
public:
    AddNewProjectDialog();
    std::string GetProjectName() const;
    std::string GetProjectLocation() const;
    void SetProjectLocation(const std::string& location);
    cmajor::ast::Target GetProjectType() const { return projectType; }
private:
    void ProjectNameTextBoxTextChanged();
    void ProjectLocationTextBoxTextChanged();
    void SelectProjectTypeButtonClick();
    void SelectProjectLocationButtonClick();
    std::string GetProjectTypeName(cmajor::ast::Target projectType);
    bool Valid() const;
    wing::TextBox* projectNameTextBox;
    wing::Label* projectTypeTextLabel;
    wing::TextBox* projectLocationTextBox;
    wing::Button* createButton;
    wing::Button* cancelButton;
    cmajor::ast::Target projectType;
    std::map<cmajor::ast::Target, std::string> projectTypeNameMap;
};

} // namespace cmcode
