// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.add_new_source_file.dialog;

import wing;
import std.core;

export namespace cmcode {

class AddNewSourceFileDialog : public wing::Window
{
public:
    AddNewSourceFileDialog();
    std::string SourceFileName() const;
private:
    void SourceFileNameTextBoxTextChanged();
    wing::Button* createButton;
    wing::Button* cancelButton;
    wing::TextBox* sourceFileNameTextBox;
};

} // namespace cmcode
