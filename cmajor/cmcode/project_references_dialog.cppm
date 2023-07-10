// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.project.references.dialog;

import wing;
import cmajor.ast;

export namespace cmcode {

class ProjectReferencesDialog : public wing::Window
{
public:
    ProjectReferencesDialog(cmajor::ast::Project* project_, cmajor::ast::Solution* solution_);
    std::vector<cmajor::ast::Project*> ReferencedProjects() const;
private:
    cmajor::ast::Project* project;
    cmajor::ast::Solution* solution;
    wing::Button* cancelButton;
    wing::Button* okButton;
    std::vector<std::pair<wing::CheckBox*, cmajor::ast::Project*>> libraryProjects;
};

} // namespace cmcode
