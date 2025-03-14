// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.combobox;

import wing.control;
import wing.listbox;
import wing.color;
import wing.graphics;
import std.core;

export namespace wing {

struct ComboBoxCreateParams
{
    ComboBoxCreateParams();
    ComboBoxCreateParams& Defaults();
    ComboBoxCreateParams& WindowClassName(const std::string& windowClassName_);
    ComboBoxCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    ComboBoxCreateParams& WindowStyle(int windowStyle_);
    ComboBoxCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    ComboBoxCreateParams& BackgroundColor(const Color& backgroundColor_);
    ComboBoxCreateParams& Text(const std::string& text_);
    ComboBoxCreateParams& Location(Point location_);
    ComboBoxCreateParams& SetSize(Size size_);
    ComboBoxCreateParams& SetAnchors(Anchors anchors_);
    ComboBoxCreateParams& SetDock(Dock dock_);
    ControlCreateParams controlCreateParams;
};

class ComboBox : public Control
{
public:
    ComboBox(ComboBoxCreateParams& createParams);
    bool IsComboBox() const override { return true; }
    void AddItem(const std::string& item);
    std::string GetItem(int index) const;
    void DeleteItem(int index);
    void InsertItem(int index, const std::string& item);
    void SetItem(int index, const std::string& item);
    int GetItemCount() const;
    void SetSelectedIndex(int selectedIndex);
    int GetSelectedIndex() const;
    void SelectedIndexChangedInternal();
    SelectedIndexChangedEvent& SelectedIndexChanged() { return selectedIndexChanged; }
protected:
    void OnCreated() override;
    virtual void OnSelectedIndexChanged();
private:
    SelectedIndexChangedEvent selectedIndexChanged;
};

} // wing
