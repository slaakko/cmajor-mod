// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.warnings.view;

import cmajor.info.bs;
import wing;
import std.core;

export namespace cmajor::view {

struct ViewWarningArgs
{
    ViewWarningArgs(cmajor::info::bs::Warning* warning_) : warning(warning_) {}
    cmajor::info::bs::Warning* warning;
};

using ViewWarningEvent = wing::EventWithArgs<ViewWarningArgs>;

struct WarningsViewCreateParams
{
    WarningsViewCreateParams();
    WarningsViewCreateParams& Defaults();
    WarningsViewCreateParams& WindowClassName(const std::string& windowClassName_);
    WarningsViewCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    WarningsViewCreateParams& WindowStyle(int windowStyle_);
    WarningsViewCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    WarningsViewCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    WarningsViewCreateParams& Text(const std::string& text_);
    WarningsViewCreateParams& Location(wing::Point location_);
    WarningsViewCreateParams& SetSize(wing::Size size_);
    WarningsViewCreateParams& SetAnchors(wing::Anchors anchors_);
    WarningsViewCreateParams& SetDock(wing::Dock dock_);
    wing::ControlCreateParams controlCreateParams;
    wing::TreeViewCreateParams treeViewCreateParams;
};

class WarningsView : public wing::Control
{
public:
    WarningsView(WarningsViewCreateParams& createParams);
    void Clear();
    void SetWarnings(const std::vector<cmajor::info::bs::Warning>&& warnigns_);
    ViewWarningEvent& ViewWarning() { return viewWarning; }
protected:
    virtual void OnViewWarning(cmajor::info::bs::Warning* warning);
    void OnLocationChanged() override;
    void OnSizeChanged() override;
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    void TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args);
    void SetChildPos();
    wing::Container container;
    wing::TreeView* treeView;
    wing::Control* child;
    std::vector<cmajor::info::bs::Warning> warnings;
    ViewWarningEvent viewWarning;
};

} // namespace cmajor::view
