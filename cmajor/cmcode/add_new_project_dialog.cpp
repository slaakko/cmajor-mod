// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.add_new_project.dialog;

import cmcode.new_project.dialog;
import cmcode.select.project.type.dialog;

namespace cmcode {

AddNewProjectDialog::AddNewProjectDialog() : wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.AddNewProjectDialog").
    WindowStyle(wing::DialogWindowStyle()).Text("Add New Project").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(100)))),
    projectNameTextBox(nullptr),
    projectTypeTextLabel(nullptr),
    projectLocationTextBox(nullptr),
    projectType(cmajor::ast::Target::program),
    createButton(nullptr),
    cancelButton(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point projectNameLabelLocation(16, 16);
    std::unique_ptr<wing::Label> projectNameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Project name:").Location(projectNameLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(projectNameLabelPtr.release());

    wing::Size projectNameTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> projectNameTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    projectNameTextBox = projectNameTextBoxPtr.get();
    projectNameTextBox->TextChanged().AddHandler(this, &AddNewProjectDialog::ProjectNameTextBoxTextChanged);
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
    selectProjectTypeButton->Click().AddHandler(this, &AddNewProjectDialog::SelectProjectTypeButtonClick);
    AddChild(selectProjectTypeButton.release());

    wing::Point projectLocationLabelLocation(16, 16 + defaultControlSpacing.Height + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> projectLocationLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Project location:").Location(projectLocationLabelLocation).
        SetSize(defaultLabelSize).SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(projectLocationLabelPtr.release());

    wing::Point projectLocationTextBoxLocation(16, 16 + defaultControlSpacing.Height + 24 + 24 + 24 + 24 + 24);
    wing::Size projectLocationTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> projectLocationTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    projectLocationTextBox = projectLocationTextBoxPtr.get();
    projectLocationTextBox->TextChanged().AddHandler(this, &AddNewProjectDialog::ProjectLocationTextBoxTextChanged);
    std::unique_ptr<wing::Control> paddedProjectLocationTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(projectLocationTextBoxPtr.release()).
        SetSize(PaddedSize(projectLocationTextBoxSize, wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedProjectLocationTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedProjectLocationTextBox.release()).
        Location(projectLocationTextBoxLocation).SetSize(wing::BorderedSize(wing::PaddedSize(projectLocationTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(borderedProjectLocationTextBox.release());

    wing::Point selectProjectLocationButtonLocation(wing::ScreenMetrics::Get().MMToHorizontalPixels(120) + 32, 16 + defaultControlSpacing.Height + 24 + 24 + 24 + 24 + 24);
    wing::Size selectProjectLocationButtonSize(24, 24);
    std::unique_ptr<wing::Button> selectProjectLocationButton(new wing::Button(wing::ControlCreateParams().Location(selectProjectLocationButtonLocation).
        SetSize(selectProjectLocationButtonSize).Text("...").SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    selectProjectLocationButton->Click().AddHandler(this, &AddNewProjectDialog::SelectProjectLocationButtonClick);
    AddChild(selectProjectLocationButton.release());

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

std::string AddNewProjectDialog::GetProjectName() const
{
    return projectNameTextBox->Text();
}

std::string AddNewProjectDialog::GetProjectLocation() const
{
    return projectLocationTextBox->Text();
}

void AddNewProjectDialog::SetProjectLocation(const std::string& location)
{
    projectLocationTextBox->SetText(location);
}

void AddNewProjectDialog::ProjectNameTextBoxTextChanged()
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

void AddNewProjectDialog::ProjectLocationTextBoxTextChanged()
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

void AddNewProjectDialog::SelectProjectTypeButtonClick()
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

void AddNewProjectDialog::SelectProjectLocationButtonClick()
{
    std::string location = wing::SelectDirectory(Handle(), projectLocationTextBox->Text());
    if (!location.empty())
    {
        SetProjectLocation(location);
    }
}

std::string AddNewProjectDialog::GetProjectTypeName(cmajor::ast::Target projectType)
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

bool AddNewProjectDialog::Valid() const
{
    return IsValidProjectOrSolutionName(projectNameTextBox->Text()) && !projectLocationTextBox->Text().empty();
}

} // namespace cmcode
