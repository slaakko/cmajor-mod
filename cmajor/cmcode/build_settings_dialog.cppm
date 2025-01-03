// =================================
// Copyright (c) 20223Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.build.settings.dialog;

import cmcode.configuration;
import wing;
import std.core;

export namespace cmcode {

class BuildSettingsDialog : public wing::Window
{
public:
    BuildSettingsDialog();
    void SetValuesFrom(const BuildSettings& buildSettings);
    BuildSettings GetValues() const;
    bool Valid() const;
private:
    void TextBoxTextChanged();
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::CheckBox* singleThreadedCompileCheckBox;
    wing::CheckBox* generateIntermediateCodeFilesCheckBox;
    wing::TextBox* disabledWarningsTextBox;
};

} // namespace cmcode
