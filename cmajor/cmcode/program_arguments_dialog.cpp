// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.program.arguments.dialog;

namespace cmcode {

ProgramArgumentsDialog::ProgramArgumentsDialog() :
    wing::Window(wing::WindowCreateParams().Text("Program Arguments").WindowClassName("cmcode.ProgramArgumentsDialog").
        WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).
        BackgroundColor(wing::DefaultControlBackgroundColor()).
        SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(140), wing::ScreenMetrics::Get().MMToVerticalPixels(50)))),
    okButton(nullptr),
    cancelButton(nullptr),
    programArgumentsTextBox(nullptr)
{
    SetSmallIcon(wing::Application::GetResourceManager().GetIcon("cmcode.small.icon"));
    wing::Size s = GetSize();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();

    std::unique_ptr<wing::TextBox> programArgumentsTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().
        SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), wing::ScreenMetrics::Get().MMToHorizontalPixels(5)))));
    programArgumentsTextBox = programArgumentsTextBoxPtr.get();
    std::unique_ptr<wing::Control> paddedTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(programArgumentsTextBoxPtr.release()).
        SetSize(wing::PaddedSize(programArgumentsTextBox->GetSize(), wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedTextBox.release()).Location(wing::Point(16, 16)).
        SetSize(BorderedSize(wing::PaddedSize(programArgumentsTextBox->GetSize(), wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(borderedTextBox.release());

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;
    std::unique_ptr<wing::Button> cancelButtonPtr(new wing::Button(wing::ControlCreateParams().Text("Cancel").Location(wing::Point(x, y)).SetSize(defaultButtonSize).
        SetAnchors(wing::Anchors::bottom | wing::Anchors::right)));
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
    programArgumentsTextBox->SetFocus();
}

void ProgramArgumentsDialog::SetProgramArguments(const std::string& programArguments)
{
    programArgumentsTextBox->SetText(programArguments);
    programArgumentsTextBox->SelectAll();
}

std::string ProgramArgumentsDialog::ProgramArguments() const
{
    return programArgumentsTextBox->Text();
}

} // namespace cmcode
