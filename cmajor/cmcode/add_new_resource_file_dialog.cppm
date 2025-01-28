// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.add_new_resource_file.dialog;

import wing;

export namespace cmcode {

class AddNewResourceFileDialog : public wing::Window
{
public:
    AddNewResourceFileDialog();
    std::string ResourceFileName() const;
private:
    void ResourceFileNameTextBoxTextChanged();
    wing::Button* createButton;
    wing::Button* cancelButton;
    wing::TextBox* resourceFileNameTextBox;
};

} // namespace cmcode
