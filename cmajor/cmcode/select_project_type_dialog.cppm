// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.select.project.type.dialog;

import wing;
import cmajor.ast;
import std.core;

export namespace cmcode {

wing::Color DefaultSelectProjectTypeIconListViewBorderColor();

class SelectProjectTypeDialog : public wing::Window
{
public:
    SelectProjectTypeDialog();
    cmajor::ast::Target GetTarget();
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    void IconListViewSelectedIndexChanged();
    void IconDoubleClicked();
    void KeyPreview(wing::Keys key, wing::KeyState keyState, bool& handled);
    wing::IconListView* iconListView;
    wing::Control* borderedIconListView;
    wing::Button* selectButton;
    wing::Button* cancelButton;
    cmajor::ast::Target target;
    std::map<int, cmajor::ast::Target> targetMap;
};

} // namespace cmcode
