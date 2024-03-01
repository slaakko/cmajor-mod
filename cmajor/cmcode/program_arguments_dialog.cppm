// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.program.arguments.dialog;

import wing;
import std.core;

export namespace cmcode {

class ProgramArgumentsDialog : public wing::Window
{
public:
    ProgramArgumentsDialog();
    void SetProgramArguments(const std::string& programArguments);
    std::string ProgramArguments() const;
private:
    wing::Button* okButton;
    wing::Button* cancelButton;
    wing::TextBox* programArgumentsTextBox;
};

} // namespace cmcode
