// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.location.list;

import cmajor.info;
import wing;
import std.core;

export namespace cmcode {

const int maxLocations = 20;

class MainWindow;

inline bool operator==(const cmajor::info::bs::DefinitionSourceLocation& left, const cmajor::info::bs::DefinitionSourceLocation& right)
{
    return left.file == right.file && left.line == right.line;
}

class LocationList
{
public:
    LocationList(MainWindow* mainWindow_);
    void SetToolButtons(wing::ToolButton* prevToolButton_, wing::ToolButton* nextToolButton_);
    void UpdateToolButtons();
    bool CanGotoPrev() const { return currentIndex > 0; }
    bool CanGotoNext() const { return currentIndex < locations.size(); }
    void AddLocation(const cmajor::info::bs::DefinitionSourceLocation& location);
    void GotoPreviousLocation(const cmajor::info::bs::DefinitionSourceLocation& currentLocation);
    void GotoNextLocation(const cmajor::info::bs::DefinitionSourceLocation& currentLocation);
private:
    MainWindow* mainWindow;
    int currentIndex;
    wing::ToolButton* prevToolButton;
    wing::ToolButton* nextToolButton;
    std::vector<cmajor::info::bs::DefinitionSourceLocation> locations;
};

} // namespace cmcode
