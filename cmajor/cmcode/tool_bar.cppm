// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.tool.bar;

import wing;

export namespace cmcode {

wing::Color DefaultToolBarBorderColor();
wing::Color DefaultToolBarTextButtonColor();
wing::Color DefaultToolBarToolTipColor();
int DefaultToolBarToolButtonHeight();
wing::Padding DefaultToolBarPadding();
wing::Padding DefaultToolBarToolButtonPadding();

wing::ToolBar* MakeToolBar();

} // namespace cmcode
