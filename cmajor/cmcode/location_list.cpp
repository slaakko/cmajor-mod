// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.location.list;

import cmcode.main.window;

namespace cmcode {

LocationList::LocationList(MainWindow* mainWindow_) : mainWindow(mainWindow_), currentIndex(0), prevToolButton(nullptr), nextToolButton(nullptr)
{
}

void LocationList::SetToolButtons(wing::ToolButton* prevToolButton_, wing::ToolButton* nextToolButton_)
{
    prevToolButton = prevToolButton_;
    nextToolButton = nextToolButton_;
    UpdateToolButtons();
}

void LocationList::UpdateToolButtons()
{
    if (CanGotoPrev())
    {
        prevToolButton->Enable();
    }
    else
    {
        prevToolButton->Disable();
    }
    if (CanGotoNext())
    {
        nextToolButton->Enable();
    }
    else
    {
        nextToolButton->Disable();
    }
}

void LocationList::AddLocation(const bs::DefinitionSourceLocation& location)
{
    while (locations.size() > maxLocations)
    {
        locations.erase(locations.begin());
    }
    locations.push_back(location);
    currentIndex = static_cast<int>(locations.size());
    UpdateToolButtons();
}

void LocationList::GotoPreviousLocation(const bs::DefinitionSourceLocation& currentLocation)
{
    while (CanGotoPrev())
    {
        --currentIndex;
        if (locations[currentIndex] != currentLocation)
        {
            mainWindow->GotoLocation(locations[currentIndex]);
            break;
        }
    }
    UpdateToolButtons();
}

void LocationList::GotoNextLocation(const bs::DefinitionSourceLocation& currentLocation)
{
    while (CanGotoNext())
    {
        if (locations[currentIndex] != currentLocation)
        {
            mainWindow->GotoLocation(locations[currentIndex]);
            ++currentIndex;
            break;
        }
        else
        {
            ++currentIndex;
        }
    }
    UpdateToolButtons();
}

} // namespace cmcode
