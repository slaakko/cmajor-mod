// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.search.dialog;

namespace cmcode {

SearchDialog::SearchDialog(bool hasCurrentFile_) : wing::Window(wing::WindowCreateParams().WindowClassName("cmcode.SearchDialog").WindowStyle(wing::DialogWindowStyle()).Text("Search").
    WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
    Location(wing::DefaultLocation()).SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(160), wing::ScreenMetrics::Get().MMToVerticalPixels(90)))),
    searchButton(nullptr),
    cancelButton(nullptr),  
    searchTextBox(nullptr),
    wholeWordsCheckBox(nullptr),
    caseInsensitiveCheckBox(nullptr),
    regularExpressionCheckBox(nullptr),
    whereListBox(nullptr),
    hasCurrentFile(hasCurrentFile_)
{
    wing::Size s = GetSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultTextBoxSize = wing::ScreenMetrics::Get().DefaultTextBoxSize();
    wing::Size defaultLabelSize = wing::ScreenMetrics::Get().DefaultLabelSize();

    wing::Point searchLabelLocation(16, 16);
    std::unique_ptr<wing::Label> searchLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Search:").
        Location(searchLabelLocation).SetSize(defaultLabelSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(searchLabelPtr.release());

    wing::Point searchTextBoxLocation(16, 16 + 24);
    std::unique_ptr<wing::TextBox> searchTextBoxPtr(new wing::TextBox(wing::TextBoxCreateParams().Defaults()));
    searchTextBox = searchTextBoxPtr.get();
    searchTextBox->TextChanged().AddHandler(this, &SearchDialog::SearchTextBoxTextChanged);
    wing::Size searchTextBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(120), defaultTextBoxSize.Height + 4);
    std::unique_ptr<wing::Control> paddedSearchTextBox(new wing::PaddedControl(wing::PaddedControlCreateParams(searchTextBoxPtr.release()).
        SetSize(wing::PaddedSize(searchTextBoxSize, wing::DefaultPadding())).
        Location(searchTextBoxLocation)));
    std::unique_ptr<wing::Control> borderedSearchTextBox(new wing::BorderedControl(wing::BorderedControlCreateParams(paddedSearchTextBox.release()).Location(searchTextBoxLocation).
        SetSize(wing::BorderedSize(wing::PaddedSize(searchTextBoxSize, wing::DefaultPadding()), wing::BorderStyle::single)).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(borderedSearchTextBox.release());

    wing::Point wholeWordsCheckBoxLocation(16, 24 + 24 + 24);
    std::unique_ptr<wing::CheckBox> wholeWordsCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Whole words only").
        Location(wholeWordsCheckBoxLocation).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    wholeWordsCheckBox = wholeWordsCheckBoxPtr.get();
    AddChild(wholeWordsCheckBoxPtr.release());

    wing::Point caseInsensitiveCheckBoxLocation(16, 24 + 24 + 24 + 24);
    std::unique_ptr<wing::CheckBox> caseInsensitiveCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Case-insensitive").
        Location(caseInsensitiveCheckBoxLocation).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    caseInsensitiveCheckBox = caseInsensitiveCheckBoxPtr.get();
    AddChild(caseInsensitiveCheckBoxPtr.release());

    wing::Point regularExpressionCheckBoxLocation(16, 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::CheckBox> regularExpressionCheckBoxPtr(new wing::CheckBox(wing::CheckBoxCreateParams().Text("Regular expression").
        Location(regularExpressionCheckBoxLocation).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    regularExpressionCheckBox = regularExpressionCheckBoxPtr.get();
    AddChild(regularExpressionCheckBoxPtr.release());

    wing::Point whereLabelLocation(16, 24 + 24 + 24 + 24 + 24 + 24);
    std::unique_ptr<wing::Label> whereLabelPtr(new wing::Label(wing::LabelCreateParams().Text("Where:").Location(whereLabelLocation).SetSize(defaultLabelSize).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    AddChild(whereLabelPtr.release());

    wing::Point listBoxLocation(16, 24 + 24 + 24 + 24 + 24 + 24 + 24);
    wing::Size listBoxSize(wing::ScreenMetrics::Get().MMToHorizontalPixels(70), wing::ScreenMetrics::Get().MMToVerticalPixels(16));
    std::unique_ptr<wing::ListBox> listBoxPtr(new wing::ListBox(wing::ListBoxCreateParams().Location(listBoxLocation).SetSize(listBoxSize).
        SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    whereListBox = listBoxPtr.get();
    std::unique_ptr<wing::Control> borderedListBoxPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(listBoxPtr.release()).
        Location(listBoxLocation).SetSize(listBoxSize).SetAnchors(wing::Anchors::top | wing::Anchors::left)));
    wing::Control* borderedListBox = borderedListBoxPtr.get();
    AddChild(borderedListBoxPtr.release());
    borderedListBox->SetSize(BorderedSize(whereListBox->GetSize(), wing::BorderStyle::single));
    if (hasCurrentFile)
    {
        whereListBox->AddItem("Current file");
    }
    whereListBox->AddItem("Active project");
    whereListBox->AddItem("Entire solution");
    whereListBox->SetSelectedIndex(0);

    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;
    std::unique_ptr<wing::Button> cancelButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Cancel").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    cancelButton = cancelButtonPtr.get();
    cancelButton->SetDialogResult(wing::DialogResult::cancel);
    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;
    std::unique_ptr<wing::Button> searchButtonPtr(new wing::Button(wing::ControlCreateParams().Location(wing::Point(x, y)).SetSize(defaultButtonSize).Text("Search").
        SetAnchors(wing::Anchors::right | wing::Anchors::bottom)));
    searchButton = searchButtonPtr.get();
    searchButton->Disable();
    searchButton->SetDialogResult(wing::DialogResult::ok);
    searchButton->SetDefault();
    AddChild(searchButtonPtr.release());
    AddChild(cancelButtonPtr.release());
    SetDefaultButton(searchButton);
    SetCancelButton(cancelButton);
    searchTextBox->SetFocus();
}

std::string SearchDialog::SearchText() const
{
    return searchTextBox->Text();
}

bool SearchDialog::WholeWords() const
{
    return wholeWordsCheckBox->Checked();
}

bool SearchDialog::CaseInsensitive() const
{
    return caseInsensitiveCheckBox->Checked();
}

bool SearchDialog::RegularExpression() const
{
    return regularExpressionCheckBox->Checked();
}

SearchExtent SearchDialog::GetSearchExtent() const
{
    int selectedIndex = whereListBox->GetSelectedIndex();
    if (!hasCurrentFile)
    {
        return static_cast<SearchExtent>(selectedIndex + 1);
    }
    else
    {
        return static_cast<SearchExtent>(selectedIndex);
    }
}

void SearchDialog::SearchTextBoxTextChanged()
{
    if (searchTextBox->Text().empty())
    {
        searchButton->Disable();
    }
    else
    {
        searchButton->Enable();
    }
}

} // namespace cmcode
