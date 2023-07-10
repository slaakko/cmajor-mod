// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.search.dialog;

import wing;
import std.core;

export namespace cmcode {

enum class SearchExtent : int
{
    currentFile = 0, activeProject = 1, entireSolution = 2
};

class SearchDialog : public wing::Window
{
public:
    SearchDialog(bool hasCurrentFile_);
    std::string SearchText() const;
    bool WholeWords() const;
    bool CaseInsensitive() const;
    bool RegularExpression() const;
    SearchExtent GetSearchExtent() const;
private:
    void SearchTextBoxTextChanged();
    wing::Button* searchButton;
    wing::Button* cancelButton;
    wing::TextBox* searchTextBox;
    wing::CheckBox* wholeWordsCheckBox;
    wing::CheckBox* caseInsensitiveCheckBox;
    wing::CheckBox* regularExpressionCheckBox;
    wing::ListBox* whereListBox;
    bool hasCurrentFile;
};

} // namespace cmcode
