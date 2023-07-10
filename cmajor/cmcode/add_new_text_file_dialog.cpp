// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.add_new_text_file.dialog;

import util;

namespace cmcode {

bool IsValidFileName(const std::string& name)
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

AddNewTextFileDialog::AddNewTextFileDialog() : wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.AddNewTextFileDialog").WindowStyle(wing::DialogWindowStyle()).
    Text("Add New Text File").WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(60)))),
    createButton(nullptr),
    cancelButton(nullptr),
    fileNameTextBox(nullptr)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();
    wing::Size defaultCheckBoxSize = wing::ScreenMetrics::Get().DefaultCheckBoxSize();

    wing::Point fileNameLabelLocation(16, 16);
    std::unique_ptr<wing::Label> fileNameLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Text file name:").Location(fileNameLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(fileNameLabelPtr.release());

    wing::Size fileNameTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::TextBox> fileNameTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    fileNameTextBox = fileNameTextBoxPtr.get();
    fileNameTextBox->TextChanged().AddHandler(this, &AddNewTextFileDialog::FileNameTextBoxTextChanged);
    wing::Point fileNameTextBoxLocation(16, 16 + 24);
    std::unique_ptr<wing::Control> paddedFileNameTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(fileNameTextBoxPtr.release()).
        SetSize(wing::PaddedSize(fileNameTextBoxSize, wing::DefaultPadding()))));
    std::unique_ptr<wing::Control> borderedFileNameTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedFileNameTextBox.release()).
        Location(fileNameTextBoxLocation).SetSize(wing::BorderedSize(wing::PaddedSize(fileNameTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    AddChild(borderedFileNameTextBox.release());

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
    fileNameTextBox->SetFocus();
}

std::string AddNewTextFileDialog::FileName() const
{
    return fileNameTextBox->Text();
}

void AddNewTextFileDialog::FileNameTextBoxTextChanged()
{
    if (IsValidFileName(fileNameTextBox->Text()))
    {
        createButton->Enable();
    }
    else
    {
        createButton->Disable();
    }
}

} // namespace cmcode
