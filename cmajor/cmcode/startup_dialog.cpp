// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.startup.dialog;

namespace cmcode {

StartupDialog::StartupDialog() : Window(wing::WindowCreateParams().WindowClassName("cmcode.StartupDialog").Text("Cmajor Code Startup").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(150), wing::ScreenMetrics::Get().MMToVerticalPixels(60)))),
    recentSolutions(GetRecentSolutions()), startButton(nullptr), removeButton(nullptr), startWithoutSolutionButton(nullptr), listBox(nullptr), solutionFilePathLabel(nullptr)
{
    SetSmallIcon(wing::Application::GetResourceManager().GetIcon("cmcode.small.icon"));
    wing::Size s = GetSize();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultListBoxSize = wing::ScreenMetrics::Get().DefaultListBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point listBoxLocation(16, 16);
    wing::Size listBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(90), wing::ScreenMetrics::Get().MMToVerticalPixels(30));
    std::unique_ptr<wing::ListBox> listBoxPtr(new wing::ListBox(wing::ListBoxCreateParams().Location(listBoxLocation).SetSize(listBoxSize).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    listBox = listBoxPtr.get();
    listBox->SelectedIndexChanged().AddHandler(this, &StartupDialog::SelectedIndexChanged);
    std::unique_ptr<wing::Control> borderedListBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(listBoxPtr.release()).
        Location(listBoxLocation).SetSize(listBoxSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    wing::Control* borderedListBox = borderedListBoxPtr.get();
    AddChild(borderedListBoxPtr.release());
    borderedListBox->SetSize(wing::BorderedSize(listBox->GetSize(), wing::BorderStyle::single));

    wing::Point labelLocation(16, 16 + borderedListBox->GetSize().Height + defaultControlSpacing.Height);
    std::unique_ptr<wing::Label> label(new wing::Label(wing::LabelCreateParams().Location(labelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    solutionFilePathLabel = label.get();
    AddChild(label.release());

    wing::Size buttonSize = defaultButtonSize;
    buttonSize.Width = wing::ScreenMetrics::Get().MMToHorizontalPixels(40);
    wing::Point startButtonLocation(borderedListBox->GetSize().Width + 3 * defaultControlSpacing.Width, 16);
    std::unique_ptr<wing::Button> startButtonPtr(new wing::Button(wing::ControlCreateParams().Text("Start").Location(startButtonLocation).SetSize(buttonSize).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    startButton = startButtonPtr.get();
    startButton->SetDialogResult(wing::DialogResult::ok);
    startButton->SetDefault();
    wing::Point removeButtonLocation(borderedListBox->GetSize().Width + 3 * defaultControlSpacing.Width, 24 + buttonSize.Height);
    std::unique_ptr<wing::Button> removeButtonPtr(new wing::Button(wing::ControlCreateParams().Text("Remove").Location(removeButtonLocation).SetSize(buttonSize).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    removeButton = removeButtonPtr.get();
    removeButton->Click().AddHandler(this, &StartupDialog::RemoveClick);
    AddChild(removeButtonPtr.release());
    wing::Point startWithNoSolutionButtonLocation(borderedListBox->GetSize().Width + 3 * defaultControlSpacing.Width, 16 + 2 * buttonSize.Height + 16);
    std::unique_ptr<wing::Button> startWithoutSolutionButtonPtr(new wing::Button(wing::ControlCreateParams().Text("Start without solution").
        Location(startWithNoSolutionButtonLocation).SetSize(buttonSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    startWithoutSolutionButton = startWithoutSolutionButtonPtr.get();
    startWithoutSolutionButton->SetDialogResult(wing::DialogResult::cancel);
    AddChild(startButtonPtr.release());
    AddChild(startWithoutSolutionButtonPtr.release());
    SetDefaultButton(startButton);
    SetCancelButton(startWithoutSolutionButton);
    listBox->SetFocus();
}

const RecentSolution& StartupDialog::GetSelectedSolution() const
{
    return recentSolutions[listBox->GetSelectedIndex()];
}

void StartupDialog::OnShown()
{
    wing::Window::OnShown();
    int n = recentSolutions.size();
    for (int i = 0; i < n; ++i)
    {
        listBox->AddItem(recentSolutions[i].name);
    }
    if (n > 0)
    {
        listBox->SetSelectedIndex(0);
        const RecentSolution& recentSolution = recentSolutions[0];
        solutionFilePathLabel->SetText(recentSolution.filePath);
    }
    else
    {
        startButton->Disable();
        removeButton->Disable();
        solutionFilePathLabel->SetText(std::string());
    }
}

void StartupDialog::RemoveClick()
{
    int index = listBox->GetSelectedIndex();
    if (index >= 0 && index < recentSolutions.size())
    {
        listBox->DeleteItem(index);
        const RecentSolution& recentSolution = recentSolutions[index];
        RemoveRecentSolution(recentSolution.filePath);
        recentSolutions = GetRecentSolutions();
    }
    int n = recentSolutions.size();
    if (n > 0)
    {
        listBox->SetSelectedIndex(0);
        const RecentSolution& recentSolution = recentSolutions[0];
        solutionFilePathLabel->SetText(recentSolution.filePath);
        listBox->SetFocus();
    }
    else
    {
        startButton->Disable();
        removeButton->Disable();
        solutionFilePathLabel->SetText(std::string());
    }
}

void StartupDialog::SelectedIndexChanged()
{
    int index = listBox->GetSelectedIndex();
    if (index >= 0 && index < recentSolutions.size())
    {
        const RecentSolution& recentSolution = recentSolutions[index];
        solutionFilePathLabel->SetText(recentSolution.filePath);
    }
    else
    {
        startButton->Disable();
        removeButton->Disable();
        solutionFilePathLabel->SetText(std::string());
    }
}

} // namespace cmcode
