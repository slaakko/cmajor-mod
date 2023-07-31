// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module cmajor.view.debug.strip;

import cmajor.view.bitmaps;

namespace cmajor::view {

AddBreakpointEventArgs::AddBreakpointEventArgs() : breakpoint(nullptr)
{
}

wing::Color DefaultDebugStripBackgroundColor()
{
    return wing::Color(230, 231, 232);
}

wing::Color DefaultBreakpointColor()
{
    return wing::Color(228, 20, 0);
}

wing::Color DefaultTransparentColor()
{
    return wing::Color(0, 128, 128);
}

wing::Point DefaultArrowOffset()
{
    return wing::Point(1, 1);
}

wing::Point DefaultBreakpointOffset()
{
    return wing::Point(0, -2);
}

wing::Size DefaultBreakpointInflate()
{
    return wing::Size(-4, -4);
}

int DefaultDebugStripWidth()
{
    return 20;
}

wing::Dock DefaultDebugStripDock()
{
    return wing::Dock::left;
}

DebugStripCreateParams::DebugStripCreateParams(cmajor::debugger::BreakpointList* breakpointList_) : 
    controlCreateParams(), view(nullptr), breakpointList(breakpointList_)
{
    controlCreateParams.WindowClassName("cmajor.view.DebugStrip");
    controlCreateParams.WindowClassBackgroundColor(COLOR_WINDOW);
    controlCreateParams.BackgroundColor(DefaultDebugStripBackgroundColor());
    breakpointColor = DefaultBreakpointColor();
    transparentColor = DefaultTransparentColor();
    arrowOffset = DefaultArrowOffset();
    breakpointOffset = DefaultBreakpointOffset();
    breakpointInflate = DefaultBreakpointInflate();
    width = DefaultDebugStripWidth();
    controlCreateParams.SetSize(wing::Size(width, 0));
    controlCreateParams.SetDock(DefaultDebugStripDock());
}

DebugStripCreateParams& DebugStripCreateParams::Defaults()
{
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::SetTextView(wing::TextView* textView_)
{
    view = textView_;
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::BreakpointColor(const wing::Color& color)
{
    breakpointColor = color;
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::TransparentColor(const wing::Color& color)
{
    transparentColor = color;
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::ArrowOffset(const wing::Point& offset)
{
    arrowOffset = offset;
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::BreakpointOffset(const wing::Point& offset)
{
    breakpointOffset = offset;
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::BreakpointInflate(const wing::Size& inflate)
{
    breakpointInflate = inflate;
    return *this;
}

DebugStripCreateParams& DebugStripCreateParams::Width(int width_)
{
    width = width_;
    controlCreateParams.SetSize(wing::Size(width, 0));
    return *this;
}

DebugStrip::DebugStrip(DebugStripCreateParams& createParams) :
    Control(createParams.controlCreateParams),
    view(createParams.view),
    topLineNumber(1),
    origin(),
    breakpointList(createParams.breakpointList),
    breakpointColor(createParams.breakpointColor),
    transparentColor(createParams.transparentColor),
    arrowOffset(createParams.arrowOffset),
    breakpointOffset(createParams.breakpointOffset),
    breakpointInflate(createParams.breakpointInflate),
    arrowBitmap(GetBitmap("cm.views.arrow.bitmap")),
    backgroundBrush(BackgroundColor()),
    breakpointBrush(breakpointColor),
    breakpointPen(breakpointColor)
{
    view->ContentLocationChanged().AddHandler(this, &DebugStrip::ViewContentLocationChanged);
    view->LineInserted().AddHandler(this, &DebugStrip::ViewLineInserted);
    view->LineDeleted().AddHandler(this, &DebugStrip::ViewLineDeleted);
}

cmajor::debugger::Breakpoint* DebugStrip::GetBreakpoint(int line) const
{
    return breakpointList->GetBreakpoint(line);
}

void DebugStrip::AddBreakpoint(cmajor::debugger::Breakpoint* breakpoint)
{
    bool cancel = false;
    wing::CancelArgs cancelArgs(cancel);
    OnChangeBreakpoints(cancelArgs);
    if (cancel) return;
    breakpointList->AddBreakpoint(breakpoint);
    OnBreakpointAdded(breakpoint);
}

void DebugStrip::RemoveBreakpoint(cmajor::debugger::Breakpoint* breakpoint)
{
    bool cancel = false;
    wing::CancelArgs cancelArgs(cancel);
    OnChangeBreakpoints(cancelArgs);
    if (cancel) return;
    std::string breakpointId = breakpoint->id;
    breakpointList->RemoveBreakpoint(breakpoint);
    OnBreakpointRemoved(breakpointId);
}

void DebugStrip::SetDebugLocation(const wing::SourceSpan& debugLocation_)
{
    if (debugLocation != debugLocation_)
    {
        debugLocation = debugLocation_;
        if (view)
        {
            view->Invalidate();
        }
        Invalidate();
    }
}

void DebugStrip::ResetDebugLocation()
{
    SetDebugLocation(wing::SourceSpan());
}

void DebugStrip::Update()
{
    bool changed = false;
    for (cmajor::debugger::Breakpoint* breakpoint : breakpointList->Breakpoints())
    {
        bool hasId = !breakpoint->id.empty();
        if (breakpoint->disabled != !hasId)
        {
            changed = true;
            breakpoint->disabled = !hasId;
        }
        if (breakpoint->location.line != breakpoint->line)
        {
            changed = true;
        }
    }
    if (changed)
    {
        Invalidate();
    }
}

void DebugStrip::OnPaint(wing::PaintEventArgs& args)
{
    try
    {
        args.graphics.Clear(BackgroundColor());
        if (breakpointList->IsEmpty() && debugLocation == wing::SourceSpan()) return;
        if (view->CharHeight() == 0 || view->CharWidth() == 0) return;
        Gdiplus::SmoothingMode prevSmoothingMode = args.graphics.GetSmoothingMode();
        args.graphics.SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeHighQuality);
        wing::Point myOrigin = origin;
        view->TranslateContentLocationInternal(myOrigin);
        int visibleLineCount = view->GetVisibleLineCount();
        float topLineDiff = view->TopLineDiff();
        wing::PointF o(myOrigin.X, myOrigin.Y);
        float charHeight = view->CharHeight();
        wing::Size s = GetSize();
        wing::SizeF size(s.Width, s.Width);
        for (int line = topLineNumber; line < topLineNumber + visibleLineCount; ++line)
        {
            cmajor::debugger::Breakpoint* bp = GetBreakpoint(line);
            wing::RectF r(o, size);
            r.Inflate(breakpointInflate.Width, breakpointInflate.Height);
            r.Offset(breakpointOffset.X, breakpointOffset.Y);
            r.Offset(0.0f, topLineDiff);
            if (bp)
            {
                if (bp->disabled)
                {
                    wing::CheckGraphicsStatus(args.graphics.FillEllipse(&backgroundBrush, r));
                    wing::CheckGraphicsStatus(args.graphics.DrawEllipse(&breakpointPen, r));
                }
                else
                {
                    wing::CheckGraphicsStatus(args.graphics.FillEllipse(&breakpointBrush, r));
                }
            }
            if (debugLocation.line == line)
            {
                DrawArrow(args.graphics, wing::PointF(r.X, r.Y));
            }
            o.Y = o.Y + charHeight;
        }
        Control::OnPaint(args);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void DebugStrip::OnMouseDown(wing::MouseEventArgs& args)
{
    try
    {
        wing::Point location = args.location;
        view->TranslateMousePosInternal(location);
        int line = static_cast<int>(location.Y / view->CharHeight() + 1);
        if (args.buttons == wing::MouseButtons::lbutton && args.clicks == 1)
        {
            cmajor::debugger::Breakpoint* bp = GetBreakpoint(line);
            if (bp)
            {
                RemoveBreakpoint(bp);
                Invalidate();
            }
            else
            {
                cmajor::debugger::Breakpoint* bp = new cmajor::debugger::Breakpoint(line);
                AddBreakpoint(bp);
                Invalidate();
            }
        }
        else if (args.buttons == wing::MouseButtons::rbutton && args.clicks == 1)
        {
            // todo
        }
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void DebugStrip::OnChangeBreakpoints(wing::CancelArgs& args)
{
    changeBreakpoints.Fire(args);
}

void DebugStrip::OnBreakpointAdded(cmajor::debugger::Breakpoint* breakpoint)
{
    AddBreakpointEventArgs args;
    args.breakpoint = breakpoint;
    breakpointAdded.Fire(args);
    Invalidate();
}

void DebugStrip::OnBreakpointRemoved(const std::string& breakpointId)
{
    RemoveBreakpointEventArgs args;
    args.breakpointId = breakpointId;
    breakpointRemoved.Fire(args);
    Invalidate();
}

void DebugStrip::DrawArrow(wing::Graphics& graphics, const wing::PointF& location)
{
    wing::PointF bitmapLocation(location);
    bitmapLocation.X = bitmapLocation.X + arrowOffset.X;
    bitmapLocation.Y = bitmapLocation.Y + arrowOffset.Y;
    wing::RectF r(bitmapLocation, wing::SizeF(arrowBitmap->GetWidth(), arrowBitmap->GetHeight()));
    Gdiplus::ImageAttributes attributes;
    attributes.SetColorKey(transparentColor, transparentColor, Gdiplus::ColorAdjustType::ColorAdjustTypeDefault);
    wing::Rect s(wing::Point(), wing::Size(arrowBitmap->GetWidth(), arrowBitmap->GetHeight()));
    wing::CheckGraphicsStatus(graphics.DrawImage(arrowBitmap, r, s.X, s.Y, s.Width, s.Height, wing::Unit::UnitPixel, &attributes));
}

void DebugStrip::ViewContentLocationChanged()
{
    topLineNumber = view->TopLine();
    origin = view->ContentLocation();
    Invalidate();
}

void DebugStrip::ViewLineInserted(wing::LineEventArgs& args)
{
    bool changed = false;
    int line = args.lineIndex + 1 - 1;
    for (cmajor::debugger::Breakpoint* breakpoint : breakpointList->Breakpoints())
    {
        if (breakpoint->line >= line)
        {
            ++breakpoint->line;
            changed = true;
        }
    }
    if (changed)
    {
        Invalidate();
    }
}

void DebugStrip::ViewLineDeleted(wing::LineEventArgs& args)
{
    bool changed = false;
    std::vector<cmajor::debugger::Breakpoint*> breakpointsToDelete;
    int line = args.lineIndex + 1;
    for (cmajor::debugger::Breakpoint* breakpoint : breakpointList->Breakpoints())
    {
        if (breakpoint->line == line)
        {
            breakpointsToDelete.push_back(breakpoint);
        }
        else if (breakpoint->line > line)
        {
            --breakpoint->line;
            changed = true;
        }
    }
    for (cmajor::debugger::Breakpoint* breakpointToDelete : breakpointsToDelete)
    {
        breakpointList->RemoveBreakpoint(breakpointToDelete);
        changed = true;
    }
    if (changed)
    {
        Invalidate();
    }
}

} // namespace cmajor::view
