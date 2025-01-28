// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.build.settings.dialog;

import cmajor.symbols;
import util;

namespace cmcode {

BuildSettingsDialog::BuildSettingsDialog() : wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.BuildSettingsDialog").WindowStyle(wing::DialogWindowStyle()).
    Text("Build Settings").WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), wing::ScreenMetrics::Get().MMToVerticalPixels(80)))),
    okButton(nullptr), cancelButton(nullptr), singleThreadedCompileCheckBox(nullptr), generateIntermediateCodeFilesCheckBox(nullptr), disabledWarningsTextBox(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();
    wing::Size defaultCheckBoxSize = wing::ScreenMetrics::Get().DefaultCheckBoxSize();

    wing::Point singleThreadedCheckBoxLocation(16, 16);
    std::unique_ptr<wing::CheckBox> singleThreadedCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().
        Text("Compile using only one thread").
        Location(singleThreadedCheckBoxLocation).SetSize(defaultCheckBoxSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    singleThreadedCompileCheckBox = singleThreadedCheckBoxPtr.get();
    AddChild(singleThreadedCheckBoxPtr.release());

    wing::Point generateIntermediateCodeFilesCheckBoxLocation(16, 16 + 24);
    std::unique_ptr<wing::CheckBox> generateIntermediateCodeCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Generate intermediate code files").
        Location(generateIntermediateCodeFilesCheckBoxLocation).SetSize(defaultCheckBoxSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    generateIntermediateCodeFilesCheckBox = generateIntermediateCodeCheckBoxPtr.get();
    AddChild(generateIntermediateCodeCheckBoxPtr.release());

    wing::Point warningsLabelLocation(16, 16 + defaultControlSpacing.Height + 24 + 24);
    std::unique_ptr<wing::Label> warningsLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Disabled warnings: (semicolon-separated list of warning numbers)").
        Location(warningsLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(warningsLabelPtr.release());

    std::unique_ptr<wing::TextBox> disabledWarningsTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults().
        SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(60), wing::ScreenMetrics::Get().MMToHorizontalPixels(5)))));
    disabledWarningsTextBox = disabledWarningsTextBoxPtr.get();
    disabledWarningsTextBox->TextChanged().AddHandler(this, &BuildSettingsDialog::TextBoxTextChanged);
    std::unique_ptr<wing::Control> paddedTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(disabledWarningsTextBoxPtr.release()).
        SetSize(wing::PaddedSize(disabledWarningsTextBox->GetSize(), wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTextBox.release()).Location(
        wing::Point(16, 16 + 24 + 24 + 24)).
        SetSize(BorderedSize(wing::PaddedSize(disabledWarningsTextBox->GetSize(), wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(borderedTextBox.release());

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
    okButton->Disable();
    AddChild(okButtonPtr.release());
    AddChild(cancelButtonPtr.release());
    SetDefaultButton(okButton);
    SetCancelButton(cancelButton);
    singleThreadedCompileCheckBox->SetFocus();
    if (Valid())
    {
        okButton ->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

bool BuildSettingsDialog::Valid() const
{
    if (disabledWarningsTextBox->Text().empty()) return true;
    std::string warnings = disabledWarningsTextBox->Text();
    std::vector<std::string> warningTexts = util::Split(warnings, ';');
    for (const auto& warningText : warningTexts)
    {
        if (warningText.empty()) return false;
        for (const auto c : warningText)
        {
            if (c < '0' || c > '9') return false;
        }
        int warning = std::stoi(warningText);
        if (warning <= 0 || warning >= cmajor::symbols::maxWarningNumber)
        {
            return false;
        }
    }
    return true;
}

void BuildSettingsDialog::TextBoxTextChanged()
{
    if (Valid())
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

void BuildSettingsDialog::SetValuesFrom(const BuildSettings& buildSettings)
{
    singleThreadedCompileCheckBox->SetChecked(buildSettings.singleThreadedCompile);
    generateIntermediateCodeFilesCheckBox->SetChecked(buildSettings.generateIntermediateCodeFiles);
    disabledWarningsTextBox->SetText(buildSettings.disabledWarnings);
}

BuildSettings BuildSettingsDialog::GetValues() const
{
    BuildSettings buildSettings;
    buildSettings.singleThreadedCompile = singleThreadedCompileCheckBox->Checked();
    buildSettings.generateIntermediateCodeFiles = generateIntermediateCodeFilesCheckBox->Checked();
    buildSettings.disabledWarnings = disabledWarningsTextBox->Text();
    return buildSettings;
}

} // namespace cmcode
