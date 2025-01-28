// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.debug.strip;

import cmajor.service;
import cmajor.debuggers;
import wing;
import std.core;

export namespace cmajor::view {

struct AddBreakpointEventArgs
{
    AddBreakpointEventArgs();
    cmajor::debugger::Breakpoint* breakpoint;
};

struct RemoveBreakpointEventArgs
{
    std::string breakpointId;
};

using ChangeBreakpointsEvent = wing::EventWithArgs<wing::CancelArgs>;
using AddBreakpointEvent = wing::EventWithArgs<AddBreakpointEventArgs>;
using RemoveBreakpointEvent = wing::EventWithArgs<RemoveBreakpointEventArgs>;

wing::Color DefaultDebugStripBackgroundColor();
wing::Color DefaultBreakpointColor();
wing::Color DefaultTransparentColor();
wing::Point DefaultArrowOffset();
wing::Point DefaultBreakpointOffset();
wing::Size DefaultBreakpointInflate();
int DefaultDebugStripWidth();
wing::Dock DefaultDebugStripDock();

struct DebugStripCreateParams
{
    DebugStripCreateParams(cmajor::debugger::BreakpointList* breakpointList_);
    DebugStripCreateParams& Defaults();
    DebugStripCreateParams& WindowClassName(const std::string& windowClassName_);
    DebugStripCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    DebugStripCreateParams& WindowStyle(int windowStyle_);
    DebugStripCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    DebugStripCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    DebugStripCreateParams& Text(const std::string& text_);
    DebugStripCreateParams& Location(wing::Point location_);
    DebugStripCreateParams& SetSize(wing::Size size_);
    DebugStripCreateParams& SetAnchors(wing::Anchors anchors_);
    DebugStripCreateParams& SetDock(wing::Dock dock_);
    DebugStripCreateParams& SetTextView(wing::TextView* textView_);
    DebugStripCreateParams& BreakpointColor(const wing::Color& color);
    DebugStripCreateParams& TransparentColor(const wing::Color& color);
    DebugStripCreateParams& ArrowOffset(const wing::Point& offset);
    DebugStripCreateParams& BreakpointOffset(const wing::Point& offset);
    DebugStripCreateParams& BreakpointInflate(const wing::Size& inflate);
    DebugStripCreateParams& Width(int width_);
    wing::ControlCreateParams controlCreateParams;
    wing::TextView* view;
    cmajor::debugger::BreakpointList* breakpointList;
    wing::Color breakpointColor;
    wing::Color transparentColor;
    wing::Point arrowOffset;
    wing::Point breakpointOffset;
    wing::Size breakpointInflate;
    int width;
};

class DebugStrip : public wing::Control
{
public:
    DebugStrip(DebugStripCreateParams& createParams);
    cmajor::debugger::Breakpoint* GetBreakpoint(int line) const;
    void AddBreakpoint(cmajor::debugger::Breakpoint* breakpoint);
    void RemoveBreakpoint(cmajor::debugger::Breakpoint* breakpoint);
    const wing::SourceSpan& DebugLocation() const { return debugLocation; }
    void SetDebugLocation(const wing::SourceSpan& debugLocation_);
    void ResetDebugLocation();
    void Update();
    ChangeBreakpointsEvent& ChangeBreakpoints() { return changeBreakpoints; }
    AddBreakpointEvent& BreakpointAdded() { return breakpointAdded; }
    RemoveBreakpointEvent& BreakpointRemoved() { return breakpointRemoved; }
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
    void OnMouseDown(wing::MouseEventArgs& args) override;
    virtual void OnChangeBreakpoints(wing::CancelArgs& args);
    virtual void OnBreakpointAdded(cmajor::debugger::Breakpoint* breakpoint);
    virtual void OnBreakpointRemoved(cmajor::debugger::Breakpoint* breakpoint);
private:
    void DrawArrow(wing::Graphics& graphics, const wing::PointF& location);
    void ViewContentLocationChanged();
    void ViewLineInserted(wing::LineEventArgs& args);
    void ViewLineDeleted(wing::LineEventArgs& args);
    wing::TextView* view;
    cmajor::debugger::BreakpointList* breakpointList;
    wing::SourceSpan debugLocation;
    int topLineNumber;
    wing::Point origin;
    wing::Color breakpointColor;
    wing::Color transparentColor;
    wing::Point arrowOffset;
    wing::Point breakpointOffset;
    wing::Size breakpointInflate;
    wing::Bitmap* arrowBitmap;
    wing::SolidBrush backgroundBrush;
    wing::SolidBrush breakpointBrush;
    wing::Pen breakpointPen;
    ChangeBreakpointsEvent changeBreakpoints;
    AddBreakpointEvent breakpointAdded;
    RemoveBreakpointEvent breakpointRemoved;
};

} // namespace cmajor::view
