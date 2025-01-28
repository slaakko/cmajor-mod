// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.project.references.dialog;

import util;

namespace cmcode {

ProjectReferencesDialog::ProjectReferencesDialog(cmajor::ast::Project* project_, cmajor::ast::Solution* solution_) :
    wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.ProjectReferencesDialog").WindowStyle(wing::DialogWindowStyle()).
        Text(util::ToUtf8(project_->Name()) + " Project References").
        WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
        Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(60)))),
    project(project_), solution(solution_), cancelButton(nullptr), okButton(nullptr)
{
    SetCaretDisabled();
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();

    std::unique_ptr<wing::GroupBox> groupBoxPtr(new wing::GroupBox(wing::GroupBoxCreateParams().Location(wing::Point(16, 16)).
        SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), wing::ScreenMetrics::Get().MMToVerticalPixels(40))).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    std::vector<cmajor::ast::Project*> referencedProjects = cmajor::ast::GetReferencedProjects(project, solution);
    wing::Point loc(16, 24);
    for (const auto& proj : solution->Projects())
    {
        if (proj->GetTarget() == cmajor::ast::Target::library || proj->GetTarget() == cmajor::ast::Target::winlib)
        {
            bool disable = false;
            if (proj.get() == project)
            {
                disable = true;
            }
            else
            {
                std::set<cmajor::ast::Project*> projReferences = cmajor::ast::GetAllReferencedProjects(proj.get(), solution);
                if (projReferences.find(project) != projReferences.cend())
                {
                    disable = true;
                }
            }
            std::unique_ptr<wing::CheckBox> checkBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text(util::ToUtf8(proj->Name())).Location(loc).
                SetAnchors(wing::Anchors::top | wing::Anchors::left)));
            if (disable)
            {
                checkBoxPtr->Disable();
            }
            libraryProjects.push_back(std::make_pair(checkBoxPtr.get(), proj.get()));
            if (std::find(referencedProjects.begin(), referencedProjects.end(), proj.get()) != referencedProjects.end())
            {
                checkBoxPtr->SetChecked(true);
            }
            groupBoxPtr->AddChild(checkBoxPtr.release());
            loc.Y = loc.Y + 16;
        }
    }
    loc.Y = loc.Y + 16;
    wing::Size gbs = groupBoxPtr->GetSize();
    wing::GroupBox* groupBox = groupBoxPtr.get();
    groupBox->SetSize(wing::Size(gbs.Width, loc.Y));
    AddChild(groupBoxPtr.release());

    wing::Size sz = s;
    sz.Height = groupBox->Location().Y;
    sz.Height += groupBox->GetSize().Height;
    sz.Height += 2 * (defaultButtonSize.Height + defaultControlSpacing.Height + 64);
    SetSize(sz);
    s = GetSize();

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;
    std::unique_ptr<wing::Button> cancelButtonPtr(new wing::Button(wing::ControlCreateParams().Text("Cancel").Location(wing::Point(x, y)).SetSize(defaultButtonSize).
        SetAnchors(wing::Anchors::bottom | wing::Anchors::right)));
    cancelButton = cancelButtonPtr.get();
    cancelButton->SetDialogResult(wing::DialogResult::cancel);
    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;
    std::unique_ptr<wing::Button> okButtonPtr(new wing::Button(wing::ControlCreateParams().Text("OK").Location(wing::Point(x, y)).SetSize(defaultButtonSize).
        SetAnchors(wing::Anchors::bottom | wing::Anchors::right)));
    okButton = okButtonPtr.get();
    okButton->SetDefault();
    okButton->SetDialogResult(wing::DialogResult::ok);
    AddChild(okButtonPtr.release());
    AddChild(cancelButtonPtr.release());
    SetDefaultButton(okButton);
    SetCancelButton(cancelButton);
    okButton->SetFocus();
}

std::vector<cmajor::ast::Project*> ProjectReferencesDialog::ReferencedProjects() const
{
    std::vector<cmajor::ast::Project*> referencedProjects;
    int n = libraryProjects.size();
    for (int i = 0; i < n; ++i)
    {
        const auto& p = libraryProjects[i];
        wing::CheckBox* checkBox = p.first;
        if (checkBox->Checked())
        {
            cmajor::ast::Project* project = p.second;
            referencedProjects.push_back(project);
        }
    }
    return referencedProjects;
}

} // namespace cmcode
