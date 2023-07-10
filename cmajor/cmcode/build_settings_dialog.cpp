// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.build.settings.dialog;

namespace cmcode {

BuildSettingsDialog::BuildSettingsDialog() : wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.BuildSettingsDialog").WindowStyle(wing::DialogWindowStyle()).
    Text("Build Settings").WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(100)))),
    okButton(nullptr), cancelButton(nullptr), singleThreadedCompileCheckBox(nullptr), generateIntermediateCodeFilesCheckBox(nullptr),
    generateOptimizedIntermediateCodeFilesCheckBox(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();
    wing::Size defaultCheckBoxSize = wing::ScreenMetrics::Get().DefaultCheckBoxSize();

/*
    wing::Point projectBuildThreadsTextBoxLocation(16, 16 + 24);
    std::unique_ptr<wing::TextBox> projectBuildThreadsTextBoxPtr(new wing::TextBox(wing:: TextBoxCreateParams().Text("0")));
    projectBuildThreadsTextBox = projectBuildThreadsTextBoxPtr.get();
    projectBuildThreadsTextBox->TextChanged().AddHandler(this, &BuildSettingsDialog::TextBoxTextChanged);
    std::unique_ptr<wing::Control> paddedTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(projectBuildThreadsTextBoxPtr.release()).
        SetSize(wing::PaddedSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(10), defaultTextBoxSize.Height + 4), DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTextBox.release()).Location(projectBuildThreadsTextBoxLocation).
        SetSize(wing::BorderedSize(wing::PaddedSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(10), defaultTextBoxSize.Height + 4), wing::DefaultPadding()), 
            wing::BorderStyle::single)).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(borderedTextBox.release());
*/

    wing::Point singleThreadedCheckBoxLocation(16, 16 + 24);
    std::unique_ptr<wing::CheckBox> singleThreadedCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().
        Text("Compile using only one thread").
        Location(singleThreadedCheckBoxLocation).SetSize(defaultCheckBoxSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    singleThreadedCompileCheckBox = singleThreadedCheckBoxPtr.get();
    AddChild(singleThreadedCheckBoxPtr.release());

    wing::Point generateIntermediateCodeFilesCheckBoxLocation(16, 16 + 24 + 24);
    std::unique_ptr<wing::CheckBox> generateIntermediateCodeCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Generate intermediate code files").
        Location(generateIntermediateCodeFilesCheckBoxLocation).SetSize(defaultCheckBoxSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    generateIntermediateCodeFilesCheckBox = generateIntermediateCodeCheckBoxPtr.get();
    AddChild(generateIntermediateCodeCheckBoxPtr.release());

    wing::Point generateOptimizedIntermediateCodeFilesCheckBoxLocation(16, 16 + 24 + 24 + 24);
    std::unique_ptr<wing::CheckBox> generateOptimizedIntermediateCodeCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Generate optimized intermediate code files").
        Location(generateOptimizedIntermediateCodeFilesCheckBoxLocation).SetSize(defaultCheckBoxSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    generateOptimizedIntermediateCodeFilesCheckBox = generateOptimizedIntermediateCodeCheckBoxPtr.get();
    AddChild(generateOptimizedIntermediateCodeCheckBoxPtr.release());

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
    singleThreadedCompileCheckBox->SetFocus();
}

void BuildSettingsDialog::SetValuesFrom(const BuildSettings& buildSettings)
{
    singleThreadedCompileCheckBox->SetChecked(buildSettings.singleThreadedCompile);
    generateIntermediateCodeFilesCheckBox->SetChecked(buildSettings.generateIntermediateCodeFiles);
    generateOptimizedIntermediateCodeFilesCheckBox->SetChecked(buildSettings.generateOptimizedIntermediateCodeFiles);
}

BuildSettings BuildSettingsDialog::GetValues() const
{
    BuildSettings buildSettings;
    buildSettings.singleThreadedCompile = singleThreadedCompileCheckBox->Checked();
    buildSettings.generateIntermediateCodeFiles = generateIntermediateCodeFilesCheckBox->Checked();
    buildSettings.generateOptimizedIntermediateCodeFiles = generateOptimizedIntermediateCodeFilesCheckBox->Checked();
    return buildSettings;
}

} // namespace cmcode
