// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.error.view;

import cmajor.info.bs;
import wing;

export namespace cmajor::view {

struct ViewErrorArgs
{
    ViewErrorArgs(cmajor::info::bs::CompileError* error_) : error(error_) {}
    cmajor::info::bs::CompileError* error;
};

using ViewErrorEvent = wing::EventWithArgs<ViewErrorArgs>;

struct ErrorViewCreateParams
{
    ErrorViewCreateParams();
    ErrorViewCreateParams& Defaults();
    ErrorViewCreateParams& WindowClassName(const std::string& windowClassName_);
    ErrorViewCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    ErrorViewCreateParams& WindowStyle(int windowStyle_);
    ErrorViewCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    ErrorViewCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    ErrorViewCreateParams& Text(const std::string& text_);
    ErrorViewCreateParams& Location(wing::Point location_);
    ErrorViewCreateParams& SetSize(wing::Size size_);
    ErrorViewCreateParams& SetAnchors(wing::Anchors anchors_);
    ErrorViewCreateParams& SetDock(wing::Dock dock_);
    wing::ControlCreateParams controlCreateParams;
    wing::TreeViewCreateParams treeViewCreateParams;
};

class ErrorView : public wing::Control
{
public:
    ErrorView(ErrorViewCreateParams& createParams);
    void Clear();
    void SetErrors(const std::vector<cmajor::info::bs::CompileError>&& errors_);
    ViewErrorEvent& ViewError() { return viewError; }
protected:
    virtual void OnViewError(cmajor::info::bs::CompileError* error);
    void OnLocationChanged() override;
    void OnSizeChanged() override;
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    void TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args);
    void SetChildPos();
    wing::Container container;
    wing::TreeView* treeView;
    wing::Control* child;
    std::vector<cmajor::info::bs::CompileError> errors;
    ViewErrorEvent viewError;
};

} // namespace cmajor::view
