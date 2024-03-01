// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.tool.bar;

import wing;

namespace cmcode {

wing::Color DefaultToolBarBorderColor()
{
    return wing::Color(204, 206, 219);
}

wing::Color DefaultToolBarTextButtonColor()
{
    return wing::Color(0, 78, 170);
}

wing::Color DefaultToolBarToolTipColor()
{
    return wing::Color(241, 242, 247);
}

int DefaultToolBarToolButtonHeight()
{
    return 30;
}

wing::Padding DefaultToolBarPadding()
{
    return wing::Padding(4, 4, 4, 4);
}

wing::Padding DefaultToolBarToolButtonPadding()
{
    return wing::Padding(2, 0, 2, 0);
}

wing::ToolBar* MakeToolBar()
{
    wing::ToolBar* toolBar = new wing::ToolBar(wing::ToolBarCreateParams().BackgroundColor(wing::GetColor("tool.bar.background")).
        DarkColor(wing::GetColor("tool.bar.tool.button.dark")).
        LightColor(wing::GetColor("tool.bar.tool.button.light")).
        MediumLightColor(wing::GetColor("tool.bar.tool.button.medium.light")).
        MediumDarkColor(wing::GetColor("tool.bar.tool.button.medium.dark")).
        TextColor(wing::GetColor("tool.bar.tool.button.text")).
        DisabledColor(wing::GetColor("tool.bar.tool.button.disabled")).
        SeparatorColor1(wing::GetColor("tool.bar.tool.button.separator1")).
        SeparatorColor2(wing::GetColor("tool.bar.tool.button.separator2")).
        ToolTipWindowColor(wing::GetColor("tool.bar.tool.tip")).
        SetDock(wing::Dock::none).ToolButtonHeight(DefaultToolBarToolButtonHeight()).
        ToolBarPadding(DefaultToolBarPadding()).ToolBarToolButtonPadding(DefaultToolBarToolButtonPadding()).ToolTipWindowColor(DefaultToolBarToolTipColor()));
    toolBar->SetBackgroundItemName("tool.bar.background");
    return toolBar;
}

} // namespace cmcode
