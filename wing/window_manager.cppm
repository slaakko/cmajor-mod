// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#include <gdiplus.h>

export module wing.window.manager;

import std.core;

export namespace wing {

using Color = Gdiplus::Color;

class Control;

class WindowManager
{
public:
    WindowManager();
    bool IsSystemClassName(const std::string& windowClassName) const;
    uint16_t Register(const std::string& windowClassName, uint32_t windowClassStyle, HBRUSH backgroundBrush);
    void AddWindow(Control* window);
    void RemoveWindow(Control* window);
    Control* GetWindow(HWND handle) const;
private:
    std::set<std::string> systemClassNames;
    std::map<std::string, uint16_t> registeredWindowClasses;
    std::unordered_map<HWND, Control*> windowMap;
};

} // wing
