// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.select.project.type.dialog;

namespace cmcode {

wing::Color DefaultSelectProjectTypeIconListViewBorderColor()
{
    return wing::Color(204, 206, 219);
}

SelectProjectTypeDialog::SelectProjectTypeDialog() :
    wing::Window(wing::WindowCreateParams().WindowStyle(wing::DialogWindowStyle()).WindowClassName("cmcode.SelectProjectTypeDialog").Text("Select Project Type").
        WindowClassBackgroundColor(wing::DefaultControlWindowClassBackgroundColor()).BackgroundColor(wing::DefaultControlBackgroundColor()).
        SetSize(wing::Size(wing::ScreenMetrics::Get().MMToHorizontalPixels(180), wing::ScreenMetrics::Get().MMToVerticalPixels(100)))),
    iconListView(nullptr),
    borderedIconListView(nullptr),
    selectButton(nullptr),
    cancelButton(nullptr),
    target(cmajor::ast::Target::program)
{
    targetMap[0] = cmajor::ast::Target::program;
    targetMap[1] = cmajor::ast::Target::winguiapp;
    targetMap[2] = cmajor::ast::Target::library;
    targetMap[3] = cmajor::ast::Target::winlib;
    wing::Size s = GetSize();
    wing::Size defaultButtonSize = wing::ScreenMetrics::Get().DefaultButtonSize();
    wing::Size defaultControlSpacing = wing::ScreenMetrics::Get().DefaultControlSpacing();
    wing::Point iconListViewLocation(defaultControlSpacing.Width, defaultControlSpacing.Height);
    std::unique_ptr<wing::IconListView> iconListViewPtr(new wing::IconListView(wing::IconListViewCreateParams().SetSize(wing::Size(80, 80))));
    iconListView = iconListViewPtr.get();
    iconListView->AddItem(new wing::IconListViewItem("console.app.bitmap", "Console Application"));
    iconListView->AddItem(new wing::IconListViewItem("win.gui.app.bitmap", "Windows GUI Application"));
    iconListView->AddItem(new wing::IconListViewItem("lib.bitmap", "Library"));
    iconListView->AddItem(new wing::IconListViewItem("win.lib.bitmap", "Windows Library"));
    iconListView->SelectedIndexChanged().AddHandler(this, &SelectProjectTypeDialog::IconListViewSelectedIndexChanged);
    iconListView->ItemDoubleClicked().AddHandler(this, &SelectProjectTypeDialog::IconDoubleClicked);
    std::unique_ptr<wing::Control> borderedIconListViewPtr(new wing::BorderedControl(wing::BorderedControlCreateParams(iconListViewPtr.release()).
        NormalSingleBorderColor(DefaultSelectProjectTypeIconListViewBorderColor()).FocusedSingleBorderColor(wing::DefaultFocusedSingleBorderColor()).
        Location(iconListViewLocation).
        SetSize(wing::Size(80, 80)).
        SetAnchors(wing::Anchors::left | wing::Anchors::top)));
    borderedIconListView = borderedIconListViewPtr.get();
    AddChild(borderedIconListViewPtr.release());
    int x = s.Width - defaultButtonSize.Width - defaultControlSpacing.Width;
    int y = s.Height - defaultButtonSize.Height - defaultControlSpacing.Height;
    std::unique_ptr<wing::Button> cancelButtonPtr(new wing::Button(wing::ControlCreateParams().Text("Cancel").Location(wing::Point(x, y)).SetSize(defaultButtonSize).SetAnchors(wing::Anchors::bottom | wing::Anchors::right)));
    cancelButton = cancelButtonPtr.get();
    cancelButton->SetDialogResult(wing::DialogResult::cancel);
    x = x - defaultButtonSize.Width - defaultControlSpacing.Width;
    std::unique_ptr<wing::Button> selectButtonPtr(new wing::Button(wing::ControlCreateParams().Text("Select").Location(wing::Point(x, y)).SetSize(defaultButtonSize).SetAnchors(wing::Anchors::bottom | wing::Anchors::right)));
    selectButton = selectButtonPtr.get();
    selectButton->SetDefault();
    selectButton->SetDialogResult(wing::DialogResult::ok);
    selectButton->Disable();
    AddChild(selectButtonPtr.release());
    AddChild(cancelButtonPtr.release());
    SetDefaultButton(selectButton);
    SetCancelButton(cancelButton);
    iconListView->SetFocus();
}

cmajor::ast::Target SelectProjectTypeDialog::GetTarget()
{
    int selectedIndex = iconListView->SelectedIndex();
    if (selectedIndex != -1)
    {
        auto it = targetMap.find(selectedIndex);
        if (it != targetMap.cend())
        {
            target = it->second;
        }
    }
    return target;
}

void SelectProjectTypeDialog::OnPaint(wing::PaintEventArgs& args)
{
    wing::Window::OnPaint(args);
    wing::Size origSize = iconListView->GetSize();
    iconListView->Invalidate();
    iconListView->Update();
    wing::Size newSize = iconListView->GetSize();
    bool eq = newSize.Width == origSize.Width && newSize.Height == origSize.Height;
    if (!eq)
    {
        borderedIconListView->SetSize(wing::BorderedSize(newSize, wing::BorderStyle::single));
    }
}

void SelectProjectTypeDialog::IconListViewSelectedIndexChanged()
{
    int selectedIndex = iconListView->SelectedIndex();
    if (selectedIndex != -1)
    {
        selectButton->Enable();
    }
    else
    {
        selectButton->Disable();
    }
}

void SelectProjectTypeDialog::IconDoubleClicked()
{
    selectButton->DoClick();
}

} // namespace cmcode
