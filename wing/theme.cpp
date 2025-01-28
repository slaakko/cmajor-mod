// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module wing.theme;

import wing.control;
import wing.base;
import soul.xml.dom;
import soul.xml.dom.parser;
import soul.xml.xpath;
import util;

namespace wing {

bool IsHexNibble(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

bool IsHexByte(std::string& s)
{
    return s.length() == 2 && IsHexNibble(s[0]) && IsHexNibble(s[1]);
}

void ThrowInvalidColorString(const std::string& itemName)
{
    throw std::runtime_error("invalid item '" + itemName + "' color attribute: not in format '#RRGGBB' where RR, GG and BB are red, green and blue color values in hex");
}

std::string ToColorHexString(const Color& color)
{
    uint8_t r = color.GetRed();
    uint8_t g = color.GetGreen();
    uint8_t b = color.GetBlue();
    return "#" + util::ToHexString(r) + util::ToHexString(g) + util::ToHexString(b);
}

Color ParseColorHexString(const std::string& colorHexString, const std::string& itemName)
{
    std::string rr;
    std::string gg;
    std::string bb;
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    std::string chs;
    if (colorHexString[0] == '#')
    {
        chs = colorHexString.substr(1);
    }
    else
    {
        ThrowInvalidColorString(itemName);
    }
    if (chs.length() >= 6)
    {
        rr = chs.substr(0, 2);
        gg = chs.substr(2, 2);
        bb = chs.substr(4, 2);
        if (!IsHexByte(rr) || !IsHexByte(gg) || !IsHexByte(bb))
        {
            ThrowInvalidColorString(itemName);
        }
    }
    else
    {
        ThrowInvalidColorString(itemName);
    }
    r = util::ParseHexByte(rr);
    g = util::ParseHexByte(gg);
    b = util::ParseHexByte(bb);
    Color color(r, g, b);
    return color;
}

std::string ConfigDir()
{
    std::string cmajorRoot = util::GetFullPath(util::CmajorRoot());
    std::string configDir = util::Path::Combine(cmajorRoot, "config");
    return configDir;
}

std::string GetThemesFilePath()
{
    std::string themesFilePath = util::Path::Combine(ConfigDir(), "themes.xml");
    return themesFilePath;
}

Theme::Theme(const std::string& name_, const std::string& filePath_) : name(name_), filePath(filePath_)
{
}

void Theme::Save()
{
    soul::xml::Document themeDoc;
    soul::xml::Element* root = soul::xml::MakeElement("theme");
    root->SetAttribute("name", name);
    for (const auto& p : itemColorMap)
    {
        const std::string& name = p.first;
        const Color& color = p.second;
        soul::xml::Element* item = soul::xml::MakeElement("item");
        item->SetAttribute("name", name);
        item->SetAttribute("color", ToColorHexString(color));
        root->AppendChild(item);
    }
    for (const auto& p : itemBitmapNameMap)
    {
        const std::string& itemName = p.first;
        const std::string& bitmapName = p.second;
        soul::xml::Element* item = soul::xml::MakeElement("item");
        item->SetAttribute("name", itemName);
        item->SetAttribute("bitmap", bitmapName);
        root->AppendChild(item);
    }
    for (const auto& p : itemCursorNameMap)
    {
        const std::string& itemName = p.first;
        const std::string& cursorName = p.second;
        soul::xml::Element* item = soul::xml::MakeElement("item");
        item->SetAttribute("name", itemName);
        item->SetAttribute("cursor", cursorName);
        root->AppendChild(item);
    }
    themeDoc.AppendChild(root);
    std::ofstream file(util::GetFullPath(util::Path::Combine(util::GetFullPath(ConfigDir()), filePath)));
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    themeDoc.Write(formatter);
}

void Theme::Load()
{
    try
    {
        std::unique_ptr<soul::xml::Document> themeDoc = soul::xml::ParseXmlFile(util::GetFullPath(util::Path::Combine(util::GetFullPath(ConfigDir()), filePath)));
        std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/theme/item", themeDoc.get());
        int n = nodeSet->Count();
        for (int i = 0; i < n; ++i)
        {
            soul::xml::Node* node = nodeSet->GetNode(i);
            if (node->IsElementNode())
            {
                soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
                std::string itemNameAttr = element->GetAttribute("name");
                if (itemNameAttr.empty())
                {
                    throw std::runtime_error("item element " + std::to_string(i) + " has no 'name' attribute");
                }
                std::string itemName = itemNameAttr;
                std::string colorAttr = element->GetAttribute("color");
                std::string bitmapAttr = element->GetAttribute("bitmap");
                std::string cursorAttr = element->GetAttribute("cursor");
                if (colorAttr.empty() && bitmapAttr.empty() && cursorAttr.empty())
                {
                    throw std::runtime_error("item '" + itemName + "' element " + std::to_string(i) + " has no 'color', no 'bitmap' and no 'cursor' attribute");
                }
                if (!colorAttr.empty())
                {
                    std::string colorHexString = colorAttr;
                    SetColor(itemName, ParseColorHexString(colorHexString, itemName));
                }
                else if (!bitmapAttr.empty())
                {
                    std::string bitmapName = bitmapAttr;
                    SetBitmapName(itemName, bitmapName);
                }
                else if (!cursorAttr.empty())
                {
                    std::string cursorName = cursorAttr;
                    SetCursorName(itemName, cursorName);
                }
            }
        }
    }
    catch (const std::exception& ex)
    {
        ShowErrorMessageBox(nullptr, "Error loading theme '" + name + "' from  file " + filePath + ": " + ex.what());
    }
}

Color Theme::GetColor(const std::string& itemName) const
{
    auto it = itemColorMap.find(itemName);
    if (it != itemColorMap.cend())
    {
        return it->second;
    }
    else
    {
        return Color::Red;
    }
}

void Theme::SetColor(const std::string& itemName, const Color& color)
{
    itemColorMap[itemName] = color;
}

std::string Theme::GetBitmapName(const std::string& itemName) const
{
    auto it = itemBitmapNameMap.find(itemName);
    if (it != itemBitmapNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void Theme::SetBitmapName(const std::string& itemName, const std::string& bitmapName)
{
    itemBitmapNameMap[itemName] = bitmapName;
}

std::string Theme::GetCursorName(const std::string& itemName) const
{
    auto it = itemCursorNameMap.find(itemName);
    if (it != itemCursorNameMap.cend())
    {
        return it->second;
    }
    else
    {
        return std::string();
    }
}

void Theme::SetCursorName(const std::string& itemName, const std::string& cursorName)
{
    itemCursorNameMap[itemName] = cursorName;
}

class Themes
{
public:
    static void Init();
    static void Done();
    static Themes& Instance() { return *instance; }
    Theme* GetCurrentTheme() const { return currentTheme; }
    void SetCurrentTheme(const std::string& themeName);
    void AddTheme(Theme* theme);
    void LoadThemes();
    void SaveThemes();
    std::vector<std::string> GetThemeNames() const;
    void RegisterUpdateColorsListener(Control* control);
    void UnregisterUpdateColorsListener(Control* control);
private:
    static std::unique_ptr<Themes> instance;
    std::vector<std::unique_ptr<Theme>> themeVec;
    std::map<std::string, Theme*> themeMap;
    Theme* currentTheme;
    std::set<Control*> updateColorsListeners;
    Themes();
    void UpdateColors();
};

std::unique_ptr<Themes> Themes::instance;

void Themes::Init()
{
    instance.reset(new Themes());
}

void Themes::Done()
{
    instance.reset();
}

Themes::Themes() : currentTheme(nullptr)
{
}

void Themes::AddTheme(Theme* theme)
{
    themeVec.push_back(std::unique_ptr<Theme>(theme));
}

void Themes::UpdateColors()
{
    for (Control* control : updateColorsListeners)
    {
        control->UpdateColors();
    }
}

void Themes::RegisterUpdateColorsListener(Control* control)
{
    updateColorsListeners.insert(control);
}

void Themes::UnregisterUpdateColorsListener(Control* control)
{
    updateColorsListeners.erase(control);
}

std::vector<std::string> Themes::GetThemeNames() const
{
    std::vector<std::string> themeNames;
    for (const auto& theme : themeVec)
    {
        themeNames.push_back(theme->Name());
    }
    return themeNames;
}

void Themes::SaveThemes()
{
    soul::xml::Document themesDoc;
    soul::xml::Element* root = soul::xml::MakeElement("themes");
    for (const auto& theme : themeVec)
    {
        theme->Save();
        soul::xml::Element* themeElement = soul::xml::MakeElement("theme");
        themeElement->SetAttribute("name", theme->Name());
        themeElement->SetAttribute("filePath", theme->FilePath());
        root->AppendChild(themeElement);
    }
    themesDoc.AppendChild(root);
    std::string themesFilePath = GetThemesFilePath();
    std::ofstream file(themesFilePath);
    util::CodeFormatter formatter(file);
    formatter.SetIndentSize(1);
    themesDoc.Write(formatter);
}

void Themes::LoadThemes()
{
    themeVec.clear();
    std::string themesFilePath = GetThemesFilePath();
    std::unique_ptr<soul::xml::Document> themesDoc = soul::xml::ParseXmlFile(themesFilePath);
    std::unique_ptr<soul::xml::xpath::NodeSet> nodeSet = soul::xml::xpath::EvaluateToNodeSet("/themes/theme", themesDoc.get());
    int n = nodeSet->Count();
    for (int i = 0; i < n; ++i)
    {
        soul::xml::Node* node = nodeSet->GetNode(i);
        if (node->IsElementNode())
        {
            soul::xml::Element* element = static_cast<soul::xml::Element*>(node);
            std::string themeNameAttr = element->GetAttribute("name");
            if (themeNameAttr.empty())
            {
                throw std::runtime_error("theme number " + std::to_string(i) + " has no 'name' attribute");
            }
            std::string themeName = themeNameAttr;
            std::string themeFilePathAttr = element->GetAttribute("filePath");
            if (themeFilePathAttr.empty())
            {
                throw std::runtime_error("theme '" + themeName + " number " + std::to_string(i) + " has no 'filePath' attribute");
            }
            std::string themeFilePath = themeFilePathAttr;
            std::unique_ptr<Theme> themePtr(new Theme(themeName, themeFilePath));
            Theme* theme = themePtr.get();
            theme->Load();
            themeMap[theme->Name()] = theme;
            themeVec.push_back(std::move(themePtr));
        }
    }
}

void Themes::SetCurrentTheme(const std::string& themeName)
{
    if (currentTheme && currentTheme->Name() == themeName) return;
    auto it = themeMap.find(themeName);
    if (it != themeMap.cend())
    {
        currentTheme = it->second;
        UpdateColors();
    }
}

Color GetColor(const std::string& itemName)
{
    Theme* currentTheme = Themes::Instance().GetCurrentTheme();
    if (currentTheme)
    {
        return currentTheme->GetColor(itemName);
    }
    else
    {
        return Color::Red;
    }
}

std::string GetBitmapName(const std::string& itemName)
{
    Theme* currentTheme = Themes::Instance().GetCurrentTheme();
    if (currentTheme)
    {
        return currentTheme->GetBitmapName(itemName);
    }
    else
    {
        return std::string();
    }
}

std::string GetCursorName(const std::string& itemName)
{
    Theme* currentTheme = Themes::Instance().GetCurrentTheme();
    if (currentTheme)
    {
        return currentTheme->GetCursorName(itemName);
    }
    else
    {
        return std::string();
    }
}

std::string GetTheme()
{
    Theme* currentTheme = Themes::Instance().GetCurrentTheme();
    if (currentTheme)
    {
        return currentTheme->Name();
    }
    else
    {
        return std::string();
    }
}

void SetTheme(const std::string& themeName)
{
    Themes::Instance().SetCurrentTheme(themeName);
}

std::vector<std::string> GetThemes()
{
    return Themes::Instance().GetThemeNames();
}

void AddTheme(Theme* theme)
{
    Themes::Instance().AddTheme(theme);
}

bool ThemesFileExists()
{
    return std::filesystem::exists(GetThemesFilePath());
}

void SaveThemes()
{
    Themes::Instance().SaveThemes();
}

void LoadThemes()
{
    Themes::Instance().LoadThemes();
}

void RegisterUpdateColorsListener(Control* control)
{
    Themes::Instance().RegisterUpdateColorsListener(control);
}

void UnregisterUpdateColorsListener(Control* control)
{
    Themes::Instance().UnregisterUpdateColorsListener(control);
}

void ThemeInit()
{
    Themes::Init();
}

void ThemeDone()
{
    Themes::Done();
}

} // wing
