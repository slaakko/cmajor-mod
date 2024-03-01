// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module cmajor.view.error.view;

import util;

namespace cmajor::view {

std::vector<std::u32string> GetErrorLines(const std::u32string& errorLinesStr)
{
    std::vector<std::u32string> errorLines = wing::SplitTextIntoLines(errorLinesStr);
    return errorLines;
}

ErrorViewCreateParams::ErrorViewCreateParams() : controlCreateParams(), treeViewCreateParams()
{
    controlCreateParams.WindowClassName("cmajor.view.ErrorView");
    controlCreateParams.WindowClassBackgroundColor(COLOR_WINDOW);
    controlCreateParams.BackgroundColor(wing::GetColor("error.view.background"));
    controlCreateParams.SetDock(wing::Dock::fill);
    treeViewCreateParams.BackgroundColor(wing::GetColor("error.view.background")).TextColor(wing::GetColor("error.view.text")).
        SelectedNodeColor(wing::GetColor("error.view.node.selected")).StateIndicatorColor(wing::GetColor("error.view.state.indicator"));
    treeViewCreateParams.NormalNodeFontFamilyName("Consolas");
}

ErrorViewCreateParams& ErrorViewCreateParams::Defaults()
{
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

ErrorViewCreateParams& ErrorViewCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

ErrorView::ErrorView(ErrorViewCreateParams& createParams) : Control(createParams.controlCreateParams), container(this), treeView(nullptr), child(nullptr)
{
    std::unique_ptr<wing::TreeView> treeViewPtr(new wing::TreeView(createParams.treeViewCreateParams));
    treeView = treeViewPtr.get();
    treeView->SetBackgroundItemName("error.view.background");
    treeView->SetTextItemName("error.view.text");
    treeView->SetStateIndicatorItemName("error.view.state.indicator");
    treeView->SetNodeSelectedItemName("error.view.node.selected");
    treeView->SetFlag(wing::ControlFlags::scrollSubject);
    treeView->SetDoubleBuffered();
    treeView->NodeDoubleClick().AddHandler(this, &ErrorView::TreeViewNodeDoubleClick);
    std::unique_ptr<wing::Control> paddedTreeView(new wing::PaddedControl(wing::PaddedControlCreateParams(treeViewPtr.release()).Defaults()));
    std::unique_ptr<wing::Control> scollableTreeView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(paddedTreeView.release()).SetDock(wing::Dock::fill)));
    child = scollableTreeView.get();
    container.AddChild(scollableTreeView.release());
    Clear();
}

void ErrorView::Clear()
{
    treeView->SetRoot(new wing::TreeViewNode("Errors"));
    child->Invalidate();
}

void ErrorView::SetErrors(const std::vector<cmajor::info::bs::CompileError>&& errors_)
{
    errors = errors_;
    Clear();
    for (cmajor::info::bs::CompileError& error : errors)
    {
        std::vector<std::u32string> lines = GetErrorLines(util::ToUtf32(error.message));
        std::string firstLine;
        if (!lines.empty())
        {
            firstLine = util::ToUtf8(lines.front());
        }
        wing::TreeViewNode* node = new wing::TreeViewNode(firstLine);
        if (!error.project.empty())
        {
            wing::TreeViewNode* projectNode = new wing::TreeViewNode("Module: " + error.project);
            node->AddChild(projectNode);
        }
        if (!error.file.empty())
        {
            wing::TreeViewNode* file = new wing::TreeViewNode("File: " + error.file);
            node->AddChild(file);
            wing::TreeViewNode* line = new wing::TreeViewNode("Line: " + std::to_string(error.line));
            node->AddChild(line);
        }
        if (lines.size() > 1)
        {
            wing::TreeViewNode* code = new wing::TreeViewNode("Code:");
            for (int i = 1; i < lines.size(); ++i)
            {
                wing::TreeViewNode* codeLineNode = new wing::TreeViewNode(util::ToUtf8(lines[i]));
                code->AddChild(codeLineNode);
            }
            node->AddChild(code);
        }
        node->SetData(&error);
        treeView->Root()->AddChild(node);
    }
    Invalidate();
}

void ErrorView::TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args)
{
    cmajor::info::bs::CompileError* error = static_cast<cmajor::info::bs::CompileError*>(args.node->Data());
    OnViewError(error);
}

void ErrorView::OnViewError(cmajor::info::bs::CompileError* error)
{
    if (!error || error->file.empty()) return;
    ViewErrorArgs args(error);
    viewError.Fire(args);
}

void ErrorView::OnLocationChanged()
{
    wing::Control::OnLocationChanged();
    SetChildPos();
}

void ErrorView::OnSizeChanged()
{
    wing::Control::OnSizeChanged();
    SetChildPos();
}

void ErrorView::SetChildPos()
{
    wing::Point loc;
    wing::Size size = GetSize();
    child->SetLocation(loc);
    child->SetSize(size);
}

void ErrorView::OnPaint(wing::PaintEventArgs& args)
{
    treeView->Root()->ExpandAll();
    treeView->Invalidate();
    child->Invalidate();
}

} // namespace cmajor::view
