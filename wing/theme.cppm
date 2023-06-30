// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.theme;

import wing.graphics;
import std.core;

export namespace wing {

class Control;

class Theme
{
public:
    Theme(const std::string& name_, const std::string& filePath_);
    void Save();
    void Load();
    const std::string& Name() const { return name; }
    const std::string& FilePath() const { return filePath; }
    Color GetColor(const std::string& itemName) const;
    void SetColor(const std::string& itemName, const Color& color);
    std::string GetBitmapName(const std::string& itemName) const;
    void SetBitmapName(const std::string& itemName, const std::string& bitmapName);
    std::string GetCursorName(const std::string& itemName) const;
    void SetCursorName(const std::string& itemName, const std::string& cursorName);
private:
    std::string name;
    std::string filePath;
    std::map<std::string, Color> itemColorMap;
    std::map<std::string, std::string> itemBitmapNameMap;
    std::map<std::string, std::string> itemCursorNameMap;
};

std::string GetTheme();
void SetTheme(const std::string& themeName);
std::vector<std::string> GetThemes();
void AddTheme(Theme* theme);
bool ThemesFileExists();
void SaveThemes();
void LoadThemes();
void RegisterUpdateColorsListener(Control* control);
void UnregisterUpdateColorsListener(Control* control);
Color GetColor(const std::string& itemName);
std::string GetBitmapName(const std::string& itemName);
std::string GetCursorName(const std::string& itemName);
void ThemeInit();
void ThemeDone();

} // wing
