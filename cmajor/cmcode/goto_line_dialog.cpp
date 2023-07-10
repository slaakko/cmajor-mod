// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.go.to.line.dialog;

namespace cmcode {

bool IsValidLineNumber(const std::string& text)
{
    if (text.empty()) return false;
    int lineNumber = -1;
    try
    {
        for (char c : text)
        {
            if (c < '0' || c > '9') return false;
        }
        lineNumber = std::stoi(text);
        if (lineNumber >= 1) return true;
    }
    catch (...)
    {
    }
    return false;
}

GotoLineDialog::GotoLineDialog() : wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.GotoLineDialog").WindowStyle(wing::DialogWindowStyle()).Text("Go To Line").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(80), wing::ScreenMetrics::Get().MMToVerticalPixels(60)))),
    textBox(nullptr),
    okButton(nullptr),
    cancelButton(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point labelLocation(16, 16);
    std::unique_ptr<wing::Label> label(new wing::Label(wing::LabelCreateParams().Text("Line number:").Location(labelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(label.release());

    wing::Point textBoxLocation(16, 20 + 16);
    std::unique_ptr<wing::TextBox> textBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    textBox = textBoxPtr.get();
    textBox->TextChanged().AddHandler(this, &GotoLineDialog::TextBoxTextChanged);
    wing::Size textBoxSize = wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(20), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::Control> paddedControl(new wing::PaddedControl(wing::PaddedControlCreateParams(textBoxPtr.release()).SetSize(wing::PaddedSize(textBoxSize, 
        wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedControl(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedControl.release()).
        SetSize(wing::BorderedSize(wing::PaddedSize(textBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).Location(textBoxLocation).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(borderedControl.release());

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
    okButton->SetDialogResult(wing::DialogResult::ok);
    okButton->SetDefault();
    okButton->Disable();
    AddChild(okButtonPtr.release());
    AddChild(cancelButtonPtr.release());
    SetDefaultButton(okButton);
    SetCancelButton(cancelButton);

    textBox->SetFocus();
}

void GotoLineDialog::TextBoxTextChanged()
{
    if (IsValidLineNumber(textBox->Text()))
    {
        okButton->Enable();
    }
    else
    {
        okButton->Disable();
    }
}

int GotoLineDialog::LineNumber() const
{
    return std::stoi(textBox->Text());
}

} // namespace cmcode
