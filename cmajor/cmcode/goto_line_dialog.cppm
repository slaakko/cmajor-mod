// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.go.to.line.dialog;

import wing;
import std.core;

export namespace cmcode {

class GotoLineDialog : public wing::Window
{
public:
    GotoLineDialog();
    int LineNumber() const;
private:
    void TextBoxTextChanged();
    wing::TextBox* textBox;
    wing::Button* okButton;
    wing::Button* cancelButton;
};

} // namespace cmcode
