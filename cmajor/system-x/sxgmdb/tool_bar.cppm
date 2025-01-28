// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module sxgmdb.tool.bar;

import wing;

export namespace sxgmdb {

wing::Color DefaultToolBarBorderColor();
wing::Color DefaultToolBarTextButtonColor();
wing::Color DefaultToolBarToolTipColor();
int DefaultToolBarToolButtonHeight();
wing::Padding DefaultToolBarPadding();
wing::Padding DefaultToolBarToolButtonPadding();

wing::ToolBar* MakeToolBar();

} // namespace sxgmdb
