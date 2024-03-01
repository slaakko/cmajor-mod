// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.add_new_text_file.dialog;

import wing;

export namespace cmcode {

class AddNewTextFileDialog : public wing::Window
{
public:
    AddNewTextFileDialog();
    std::string FileName() const;
private:
    void FileNameTextBoxTextChanged();
    wing::Button* createButton;
    wing::Button* cancelButton;
    wing::TextBox* fileNameTextBox;
};

} // namespace cmcode
