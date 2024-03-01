// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.add_new_source_file.dialog;

import util;

namespace cmcode {

bool IsValidSourceFileName(const std::string& name)
{
    if (name.empty()) return false;
    std::u32string s(util::ToUtf32(name));
    if (!util::IsIdStart(s[0]))
    {
        return false;
    }
    for (int i = 1; i < s.length(); ++i)
    {
        char32_t c = s[i];
        if (!util::IsIdCont(c) && c != '.')
        {
            return false;
        }
    }
    return true;
}

AddNewSourceFileDialog::AddNewSourceFileDialog() : wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.AddNewSourceFileDialog").WindowStyle(wing::DialogWindowStyle()).
    Text("Add New Source File").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(60)))),
    createButton(nullptr),
    cancelButton(nullptr),
    sourceFileNameTextBox(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();
    wing::Size defaultCheckBoxSize = wing::ScreenMetrics::Get().DefaultCheckBoxSize();

    wing::Point sourceFileNameLabelLocation(16, 16);
    std::unique_ptr<wing::Label> sourceFileNameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Source file name:").Location(sourceFileNameLabelLocation).
        SetSize(defaultLabelSize).SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(sourceFileNameLabelPtr.release());

    wing::Size sourceFileNameTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> sourceFileNameTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    sourceFileNameTextBox = sourceFileNameTextBoxPtr.get();
    sourceFileNameTextBox->TextChanged().AddHandler(this, &AddNewSourceFileDialog::SourceFileNameTextBoxTextChanged);
    wing::Point sourceFileNameTextBoxLocation(16, 16 + 24);
    std::unique_ptr<wing::Control> paddedSourceFileNameTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(sourceFileNameTextBoxPtr.release()).
        SetSize(wing::PaddedSize(sourceFileNameTextBoxSize, wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedSourceFileNameTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedSourceFileNameTextBox.release()).
        Location(sourceFileNameTextBoxLocation).SetSize(wing::BorderedSize(wing::PaddedSize(sourceFileNameTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(borderedSourceFileNameTextBox.release());

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
    sourceFileNameTextBox->SetFocus();
}

std::string AddNewSourceFileDialog::SourceFileName() const
{
    return sourceFileNameTextBox->Text();
}

void AddNewSourceFileDialog::SourceFileNameTextBoxTextChanged()
{
    if (IsValidSourceFileName(sourceFileNameTextBox->Text()))
    {
        createButton->Enable();
    }
    else
    {
        createButton->Disable();
    }
}

} // namespace cmcode
