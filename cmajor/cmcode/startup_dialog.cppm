// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.startup.dialog;

import cmcode.config;
import cmcode.configuration;
import wing;
import std.core;

export namespace cmcode {

class StartupDialog : public wing::Window
{
public:
    StartupDialog();
    const RecentSolution& GetSelectedSolution() const;
protected:
    void OnShown() override;
private:
    void SelectedIndexChanged();
    void RemoveClick();
    std::vector<RecentSolution> recentSolutions;
    wing::Button* startButton;
    wing::Button* removeButton;
    wing::Button* startWithoutSolutionButton;
    wing::ListBox* listBox;
    wing::Label* solutionFilePathLabel;
};

} // namespace cmcode
