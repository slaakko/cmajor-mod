// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.options.dialog;

import cmcode.configuration;
import wing;
import std.core;

export namespace cmcode {

class OptionsDialog : public wing::Window
{
public:
    OptionsDialog();
    void SetOptionsFrom(const Options& options);
    Options GetOptions() const;
protected:
    void OnShown() override;
private:
    void TextBoxTextChanged();
    void ParsingFrequencyTextBoxTextChanged();
    void CheckValid();
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::CheckBox* showStartupDialogCheckBox;
    wing::TextBox* numberOfRecentSolutionsTextBox;
    wing::CheckBox* codeCompletionCheckBox;
    wing::TextBox* parsingFrequencyTextBox;
    wing::ComboBox* themeComboBox;
    std::vector<std::string> themes;
    int selectedThemeIndex;
};

} // namespace cmcode
