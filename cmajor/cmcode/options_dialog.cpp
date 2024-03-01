// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.options.dialog;

namespace cmcode {

bool IsValidRecentSolutionsNumber(const std::string& text)
{
    try
    {
        if (text.empty()) return false;
        for (char c : text)
        {
            if (c < '0' || c > '9') return false;
        }
        int number = std::stoi(text);
        if (number > 0 && number <= 20) return true;
    }
    catch (...)
    {
    }
    return false;
}

bool IsValidParsingFrequency(const std::string& text)
{
    try
    {
        if (text.empty()) return false;
        for (char c : text)
        {
            if (c < '0' || c > '9') return false;
        }
        int number = std::stoi(text);
        if (number > 0 && number <= 60000) return true;
    }
    catch (...)
    {
    }
    return false;
}

OptionsDialog::OptionsDialog() : wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.OptionsDialog").WindowStyle(wing::DialogWindowStyle()).Text("Options").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(120)))),
    okButton(nullptr),
    cancelButton(nullptr),
    showStartupDialogCheckBox(nullptr),
    numberOfRecentSolutionsTextBox(nullptr),
    codeCompletionCheckBox(nullptr),
    parsingFrequencyTextBox(nullptr)
{
    themes = wing::GetThemes();
    selectedThemeIndex = 0;

    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();
    wing::Size defaultCheckBoxSize = wing::ScreenMetrics::Get().DefaultCheckBoxSize();
    wing::Size defaultComboBoxSize = wing::ScreenMetrics::Get().DefaultComboBoxSize();

    wing::Point showStartupDialogCheckBoxLocation(16, 16);
    std::unique_ptr<wing::CheckBox> showStartupDialogCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Show startup dialog").
        Location(showStartupDialogCheckBoxLocation).SetSize(defaultCheckBoxSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    showStartupDialogCheckBox = showStartupDialogCheckBoxPtr.get();
    showStartupDialogCheckBox->SetChecked(true);
    AddChild(showStartupDialogCheckBoxPtr.release());

    wing::Point numberOfRecentSolutionsLabelLocation(16, 16 + 24);
    std::unique_ptr<wing::Label> numberOfRecentSolutionsLabel(new wing::Label(wing::LabelCreateParams().Text("Number of recent solutions in the startup dialog:").
        Location(numberOfRecentSolutionsLabelLocation).SetSize(defaultLabelSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(numberOfRecentSolutionsLabel.release());

    wing::Point numberOfRecentSolutionsTextBoxLocation(16, 16 + 24 + 24);
    std::unique_ptr<wing::TextBox> numberOfRecentSolutionsTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Text("5")));
    numberOfRecentSolutionsTextBox = numberOfRecentSolutionsTextBoxPtr.get();
    numberOfRecentSolutionsTextBox->TextChanged().AddHandler(this, &OptionsDialog::TextBoxTextChanged);
    std::unique_ptr<wing::Control> paddedTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(numberOfRecentSolutionsTextBoxPtr.release()).
        SetSize(wing::PaddedSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(10), defaultTextBoxSize.Height + 4), wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTextBox.release()).
        Location(numberOfRecentSolutionsTextBoxLocation).
        SetSize(wing::BorderedSize(wing::PaddedSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(10), defaultTextBoxSize.Height + 4), wing::DefaultPadding()), 
            wing::BorderStyle::single)).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(borderedTextBox.release());

    std::unique_ptr<wing::CheckBox> codeCompletionCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Code completion").
        Location(wing::Point(16, 24 + 24 + 24 + 24)).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    codeCompletionCheckBox = codeCompletionCheckBoxPtr.get();
    codeCompletionCheckBox->SetChecked(true);
    AddChild(codeCompletionCheckBoxPtr.release());

    wing::Point parsingFrequencyLabelLocation(16, 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> parsingFrequencyLabel(new wing::Label(wing::LabelCreateParams().Text("Source code parsing frequency for code completion (milliseconds):").
        Location(parsingFrequencyLabelLocation).SetSize(defaultLabelSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(parsingFrequencyLabel.release());

    wing::Point parsingFrequencyTextBoxLocation(16, 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::TextBox> parsingFrequencyTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Text("1000")));
    parsingFrequencyTextBox = parsingFrequencyTextBoxPtr.get();
    parsingFrequencyTextBox->TextChanged().AddHandler(this, &OptionsDialog::ParsingFrequencyTextBoxTextChanged);
    std::unique_ptr<wing::Control> paddedPFTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(parsingFrequencyTextBoxPtr.release()).
        SetSize(wing::PaddedSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(10), defaultTextBoxSize.Height + 4), wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedPFTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedPFTextBox.release()).
        Location(parsingFrequencyTextBoxLocation).
        SetSize(wing::BorderedSize(wing::PaddedSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(10), defaultTextBoxSize.Height + 4), wing::DefaultPadding()), 
            wing::BorderStyle::single)).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(borderedPFTextBox.release());

    wing::Point themeLabelLocation(16, 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> themeLabel(new wing::Label(wing::LabelCreateParams().Text("Color Theme:").
        Location(themeLabelLocation).SetSize(defaultLabelSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(themeLabel.release());

    wing::Point themeComboBoxLocation(16, 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size comboBoxSize(defaultComboBoxSize.Width, defaultComboBoxSize.Height * 4);
    std::unique_ptr<wing::ComboBox> themeComboBoxPtr(new wing::ComboBox(wing::ComboBoxCreateParams().SetAnchors(wing::Anchors::top | wing::Anchors::left).
        Location(themeComboBoxLocation).SetSize(comboBoxSize)));
    themeComboBox = themeComboBoxPtr.get();
    AddChild(themeComboBoxPtr.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;
    std::unique_ptr<wing::Button> cancelButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Cancel").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    cancelButton = cancelButtonPtr.get();
    cancelButton->SetDialogResult(wing::DialogResult::cancel);
    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;
    std::unique_ptr<wing::Button> okButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("OK").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    okButton = okButtonPtr.get();
    okButton->SetDialogResult(wing::DialogResult::ok);
    okButton->SetDefault();
    AddChild(okButtonPtr.release());
    AddChild(cancelButtonPtr.release());
    SetDefaultButton(okButton);
    SetCancelButton(cancelButton);
    showStartupDialogCheckBox->SetFocus();
}

void OptionsDialog::CheckValid()
{
    if (IsValidRecentSolutionsNumber(numberOfRecentSolutionsTextBox->Text()) && IsValidParsingFrequency(parsingFrequencyTextBox->Text()))
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

void OptionsDialog::TextBoxTextChanged()
{
    CheckValid();
}

void OptionsDialog::ParsingFrequencyTextBoxTextChanged()
{
    CheckValid();
}

void OptionsDialog::SetOptionsFrom(const Options& options)
{
    showStartupDialogCheckBox->SetChecked(options.showStartupDialog);
    numberOfRecentSolutionsTextBox->SetText(std::to_string(options.numberOfRecentSolutions));
    codeCompletionCheckBox->SetChecked(options.codeCompletion);
    parsingFrequencyTextBox->SetText(std::to_string(options.parsingFrequency));
    int n = themes.size();
    for (int i = 0; i < n; ++i)
    {
        if (themes[i] == options.theme)
        {
            selectedThemeIndex = i;
            break;
        }
    }
}

Options OptionsDialog::GetOptions() const
{
    Options options;
    options.showStartupDialog = showStartupDialogCheckBox->Checked();
    options.numberOfRecentSolutions = std::stoi(numberOfRecentSolutionsTextBox->Text());
    options.codeCompletion = codeCompletionCheckBox->Checked();
    options.parsingFrequency = std::stoi(parsingFrequencyTextBox->Text());
    options.theme = themes[themeComboBox->GetSelectedIndex()];
    return options;
}

void OptionsDialog::OnShown()
{
    wing::Window::OnShown();
    int n = themes.size();
    for (int i = 0; i < n; ++i)
    {
        themeComboBox->AddItem(themes[i]);
    }
    themeComboBox->SetSelectedIndex(selectedThemeIndex);
}

} // namespace cmcode
