// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module wing.scroll.bar;

import wing.control;
import wing.graphics;
import std.core;

export namespace wing {

enum class ScrollBarOrientation
{
    horizontal, vertical
};

struct ScrollBarCreateParams
{
    ScrollBarCreateParams(ScrollBarOrientation orientation);
    ScrollBarCreateParams& Defaults();
    ScrollBarCreateParams& WindowClassName(const std::string& windowClassName_);
    ScrollBarCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    ScrollBarCreateParams& WindowStyle(int windowStyle_);
    ScrollBarCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    ScrollBarCreateParams& BackgroundColor(const Color& backgroundColor_);
    ScrollBarCreateParams& Text(const std::string& text_);
    ScrollBarCreateParams& Location(Point location_);
    ScrollBarCreateParams& SetSize(Size size_);
    ScrollBarCreateParams& SetAnchors(Anchors anchors_);
    ScrollBarCreateParams& SetDock(Dock dock_);
    ControlCreateParams controlCreateParams;
};

class ScrollBar : public Control
{
public:
    ScrollBar(ScrollBarCreateParams& createParams);
};

} // wing
