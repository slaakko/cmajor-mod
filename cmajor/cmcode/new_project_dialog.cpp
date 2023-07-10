// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.new_project.dialog;

import cmcode.select.project.type.dialog;
import util;

namespace cmcode {

bool IsValidProjectOrSolutionName(const std::string& text)
{
    if (text.empty()) return false;
    std::u32string str = util::ToUtf32(text);
    if (!util::IsIdStart(str[0])) return false;
    for (int i = 1; i < str.length(); ++i)
    {
        char32_t c = str[i];
        if (!util::IsIdCont(c) && c != '.') return false;
    }
    return true;
}

NewProjectDialog::NewProjectDialog() : wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.NewProjectDialog").WindowStyle(wing::DialogWindowStyle()).Text("New Project").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(100)))),
    projectNameTextBox(nullptr),
    projectTypeTextLabel(nullptr),
    projectLocationTextBox(nullptr),
    solutionNameTextBox(nullptr),
    cancelButton(nullptr),
    createButton(nullptr),
    placeSolutionInSameDirectoryCheckBox(nullptr),
    projectType(cmajor::ast::Target::program)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();
    wing::Size defaultCheckBoxSize = wing::ScreenMetrics::Get().DefaultCheckBoxSize();

    wing::Point projectNameLabelLocation(16, 16);
    std::unique_ptr<wing::Label> projectNameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Project name:").Location(projectNameLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(projectNameLabelPtr.release());

    wing::Size projectNameTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> projectNameTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    projectNameTextBox = projectNameTextBoxPtr.get();
    projectNameTextBox->TextChanged().AddHandler(this, &NewProjectDialog::ProjectNameTextBoxTextChanged);
    wing::Point projectNameTextBoxLocation(16, 16 + 24);
    std::unique_ptr<wing::Control> paddedProjectNameTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(projectNameTextBoxPtr.release()).
        SetSize(wing::PaddedSize(projectNameTextBoxSize, wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedProjectNameTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedProjectNameTextBox.release()).
        Location(projectNameTextBoxLocation).SetSize(wing::BorderedSize(wing::PaddedSize(projectNameTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(borderedProjectNameTextBox.release());

    wing::Point projectTypeLabelLocation(16, 16 + defaultControlSpacing.Height + 24 + 24);
    std::unique_ptr<wing::Label> projectTypeLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Project type:").Location(projectTypeLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(projectTypeLabelPtr.release());

    projectTypeNameMap[cmajor::ast::Target::program] = "Console Application";
    projectTypeNameMap[cmajor::ast::Target::winguiapp] = "Windows GUI Application";
    projectTypeNameMap[cmajor::ast::Target::library] = "Library";
    projectTypeNameMap[cmajor::ast::Target::winlib] = "Windows Library";

    wing::Point projectTypeTextLocation(16, 16 + defaultControlSpacing.Height + 24 + 24 + 24);
    std::unique_ptr<wing::Label> projectTypeTextLabelPtr(new wing::Label(wing::LabelCreateParams().Text(GetProjectTypeName(projectType)).Location(projectTypeTextLocation).
        SetSize(defaultLabelSize).SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    projectTypeTextLabel = projectTypeTextLabelPtr.get();
    AddChild(projectTypeTextLabelPtr.release());

    wing::Point selectProjectTypeButtonLocation(wing::ScreenMetrics::Get().MMToHorizontalPixels(120) + 32, 16 + defaultControlSpacing.Height + 24 + 24 + 24);
    wing::Size selectProjectTypeButtonSize(24, 24);
    std::unique_ptr<wing::Button> selectProjectTypeButton(new wing::Button(wing::ControlCreateParams().Location(selectProjectTypeButtonLocation).
        SetSize(selectProjectTypeButtonSize).Text("...").SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    selectProjectTypeButton->Click().AddHandler(this, &NewProjectDialog::SelectProjectTypeButtonClick);
    AddChild(selectProjectTypeButton.release());

    wing::Point projectLocationLabelLocation(16, 16 + defaultControlSpacing.Height + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> projectLocationLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Project location:").Location(projectLocationLabelLocation).
        SetSize(defaultLabelSize).SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(projectLocationLabelPtr.release());

    wing::Point projectLocationTextBoxLocation(16, 16 + defaultControlSpacing.Height + 24 + 24 + 24 + 24 + 24);
    wing::Size projectLocationTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> projectLocationTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    projectLocationTextBox = projectLocationTextBoxPtr.get();
    projectLocationTextBox->TextChanged().AddHandler(this, &NewProjectDialog::ProjectLocationTextBoxTextChanged);
    std::unique_ptr<wing::Control> paddedProjectLocationTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(projectLocationTextBoxPtr.release()).
        SetSize(wing::PaddedSize(projectLocationTextBoxSize, wing::DefaultPadding()))));
    std::unique_ptr<Control> borderedProjectLocationTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedProjectLocationTextBox.release()).
        Location(projectLocationTextBoxLocation).SetSize(wing::BorderedSize(wing::PaddedSize(projectLocationTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(borderedProjectLocationTextBox.release());

    wing::Point selectProjectLocationButtonLocation(wing::ScreenMetrics::Get().MMToHorizontalPixels(120) + 32, 16 + defaultControlSpacing.Height + 24 + 24 + 24 + 24 + 24);
    wing::Size selectProjectLocationButtonSize(24, 24);
    std::unique_ptr<wing::Button> selectProjectLocationButton(new wing::Button(wing::ControlCreateParams().Location(selectProjectLocationButtonLocation).
        SetSize(selectProjectLocationButtonSize).Text("...").SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    selectProjectLocationButton->Click().AddHandler(this, &NewProjectDialog::SelectProjectLocationButtonClick);
    AddChild(selectProjectLocationButton.release());

    wing::Point solutionNameLabelLocation(16, 16 + 2 * defaultControlSpacing.Height + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> solutionNameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Solution name:").Location(solutionNameLabelLocation).
        SetSize(defaultLabelSize).SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(solutionNameLabelPtr.release());

    wing::Point solutionNameTextBoxLocation(16, 16 + 2 * defaultControlSpacing.Height + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size solutionNameTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> solutionNameTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    solutionNameTextBox = solutionNameTextBoxPtr.get();
    solutionNameTextBox->TextChanged().AddHandler(this, &NewProjectDialog::SolutionNameTextBoxTextChanged);
    std::unique_ptr<wing::Control> paddedSolutionNameTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(solutionNameTextBoxPtr.release()).
        SetSize(wing::PaddedSize(solutionNameTextBoxSize, wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedSolutionNameTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedSolutionNameTextBox.release()).
        Location(solutionNameTextBoxLocation).SetSize(BorderedSize(wing::PaddedSize(solutionNameTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(borderedSolutionNameTextBox.release());

    wing::Point placeSolutionInSameDirectoryCheckBoxLocation(32, solutionNameTextBoxLocation.Y + BorderedSize(wing::PaddedSize(solutionNameTextBoxSize, wing::DefaultPadding()), 
        wing::BorderStyle::single).Height +
        24 + 24);
    std::unique_ptr<wing::CheckBox> placeSolutionInSameDirectoryCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().BackgroundColor(wing::GetColor("check.box")).
        Text("Place solution and project in the same directory").
        Location(placeSolutionInSameDirectoryCheckBoxLocation).SetSize(defaultCheckBoxSize)));
    placeSolutionInSameDirectoryCheckBox = placeSolutionInSameDirectoryCheckBoxPtr.get();
    placeSolutionInSameDirectoryCheckBox->CheckedChanged().AddHandler(this, &NewProjectDialog::PlaceSolutionInSameDirectoryCheckBoxCheckedChanged);
    AddChild(placeSolutionInSameDirectoryCheckBoxPtr.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;
    std::unique_ptr<wing::Button> cancelButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Cancel").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    cancelButton = cancelButtonPtr.get();
    cancelButton->SetDialogResult(wing::DialogResult::cancel);
    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;
    std::unique_ptr<wing::Button> createButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Create").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    createButton = createButtonPtr.get();
    createButton->Disable();
    createButton->SetDialogResult(wing::DialogResult::ok);
    createButton->SetDefault();
    AddChild(createButtonPtr.release());
    AddChild(cancelButtonPtr.release());
    SetDefaultButton(createButton);
    SetCancelButton(cancelButton);
    projectNameTextBox->SetFocus();
}

std::string NewProjectDialog::GetProjectName() const
{
    return projectNameTextBox->Text();
}

std::string NewProjectDialog::GetProjectLocation() const
{
    return projectLocationTextBox->Text();
}

void NewProjectDialog::SetProjectLocation(const std::string& location)
{
    projectLocationTextBox->SetText(location);
}

std::string NewProjectDialog::GetSolutionName() const
{
    return solutionNameTextBox->Text();
}

bool NewProjectDialog::PlaceSolutionInSameDirectory() const
{
    return placeSolutionInSameDirectoryCheckBox->Checked();
}

std::string NewProjectDialog::GetProjectTypeName(cmajor::ast::Target projectType)
{
    auto it = projectTypeNameMap.find(projectType);
    if (it != projectTypeNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void NewProjectDialog::ProjectNameTextBoxTextChanged()
{
    solutionNameTextBox->SetText(projectNameTextBox->Text());
    if (Valid())
    {
        createButton->Enable();
    }
    else
    {
        createButton->Disable();
    }
}

void NewProjectDialog::SelectProjectTypeButtonClick()
{
    SelectProjectTypeDialog dialog;
    if (dialog.ShowDialog(*this) == wing::DialogResult::ok)
    {
        projectType = dialog.GetTarget();
        projectTypeTextLabel->SetText(GetProjectTypeName(projectType));
        projectLocationTextBox->SetFocus();
        Invalidate();
    }
}

void NewProjectDialog::ProjectLocationTextBoxTextChanged()
{
    if (Valid())
    {
        createButton->Enable();
    }
    else
    {
        createButton->Disable();
    }
}

void NewProjectDialog::SelectProjectLocationButtonClick()
{
    std::string location = wing::SelectDirectory(Handle(), projectLocationTextBox->Text());
    if (!location.empty())
    {
        SetProjectLocation(location);
    }
}

void NewProjectDialog::SolutionNameTextBoxTextChanged()
{
    if (Valid())
    {
        createButton->Enable();
    }
    else
    {
        createButton->Disable();
    }
}

void NewProjectDialog::PlaceSolutionInSameDirectoryCheckBoxCheckedChanged()
{
    if (placeSolutionInSameDirectoryCheckBox->Checked())
    {
        solutionNameTextBox->SetText(projectNameTextBox->Text());
        solutionNameTextBox->Disable();
    }
    else
    {
        solutionNameTextBox->Enable();
    }
}

bool NewProjectDialog::Valid() const
{
    return IsValidProjectOrSolutionName(projectNameTextBox->Text()) && IsValidProjectOrSolutionName(solutionNameTextBox->Text()) && !projectLocationTextBox->Text().empty();
}

} // namespace cmcode
