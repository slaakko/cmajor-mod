// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module cmajor.view.warnings.view;

import util;

namespace cmajor::view {

std::vector<std::u32string> GetWarningLines(const std::u32string& warningLinesStr)
{
    std::vector<std::u32string> warningLines = wing::SplitTextIntoLines(warningLinesStr);
    return warningLines;
}

WarningsViewCreateParams::WarningsViewCreateParams() : controlCreateParams(), treeViewCreateParams()
{
    controlCreateParams.WindowClassName("cmajor.view.WarningsView");
    controlCreateParams.WindowClassBackgroundColor(COLOR_WINDOW);
    controlCreateParams.BackgroundColor(wing::GetColor("warnings.view.background"));
    controlCreateParams.SetDock(wing::Dock::fill);
    treeViewCreateParams.BackgroundColor(wing::GetColor("warnings.view.background")).TextColor(wing::GetColor("warnings.view.text")).
        SelectedNodeColor(wing::GetColor("warnings.view.node.selected")).StateIndicatorColor(wing::GetColor("warnings.view.state.indicator"));
    treeViewCreateParams.NormalNodeFontFamilyName("Consolas");
}

WarningsViewCreateParams& WarningsViewCreateParams::Defaults()
{
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

WarningsViewCreateParams& WarningsViewCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

WarningsView::WarningsView(WarningsViewCreateParams& createParams) : Control(createParams.controlCreateParams), container(this), treeView(nullptr), child(nullptr)
{
    std::unique_ptr<wing::TreeView> treeViewPtr(new wing::TreeView(createParams.treeViewCreateParams));
    treeView = treeViewPtr.get();
    treeView->SetBackgroundItemName("warnings.view.background");
    treeView->SetTextItemName("warnings.view.text");
    treeView->SetStateIndicatorItemName("warnings.view.state.indicator");
    treeView->SetNodeSelectedItemName("warnings.view.node.selected");
    treeView->SetFlag(wing::ControlFlags::scrollSubject);
    treeView->SetDoubleBuffered();
    treeView->NodeDoubleClick().AddHandler(this, &WarningsView::TreeViewNodeDoubleClick);
    std::unique_ptr<wing::Control> paddedTreeView(new wing::PaddedControl(wing::PaddedControlCreateParams(treeViewPtr.release()).Defaults()));
    std::unique_ptr<wing::Control> scollableTreeView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(paddedTreeView.release()).SetDock(wing::Dock::fill)));
    child = scollableTreeView.get();
    container.AddChild(scollableTreeView.release());
    Clear();
}

void WarningsView::Clear()
{
    treeView->SetRoot(new wing::TreeViewNode("Warnings"));
    child->Invalidate();
}

void WarningsView::SetWarnings(const std::vector<cmajor::info::bs::Warning>&& warnings_)
{
    warnings = warnings_;
    Clear();
    for (cmajor::info::bs::Warning& warning : warnings)
    {
        std::vector<std::u32string> lines = GetWarningLines(util::ToUtf32(warning.message));
        std::string firstLine = "warning " + std::to_string(warning.number);
        if (!lines.empty())
        {
            firstLine.append(": ").append(util::ToUtf8(lines.front()));
        }
        wing::TreeViewNode* node = new wing::TreeViewNode(firstLine);
        if (!warning.project.empty())
        {
            wing::TreeViewNode* projectNode = new wing::TreeViewNode("Module: " + warning.project);
            node->AddChild(projectNode);
        }
        if (!warning.file.empty())
        {
            wing::TreeViewNode* file = new wing::TreeViewNode("File: " + warning.file);
            node->AddChild(file);
            wing::TreeViewNode* line = new wing::TreeViewNode("Line: " + std::to_string(warning.line));
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
        node->SetData(&warning);
        treeView->Root()->AddChild(node);
    }
    Invalidate();
}

void WarningsView::TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args)
{
    cmajor::info::bs::Warning* warning = static_cast<cmajor::info::bs::Warning*>(args.node->Data());
    OnViewWarning(warning);
}

void WarningsView::OnViewWarning(cmajor::info::bs::Warning* warning)
{
    if (!warning || warning->file.empty()) return;
    ViewWarningArgs args(warning);
    viewWarning.Fire(args);
}

void WarningsView::OnLocationChanged()
{
    wing::Control::OnLocationChanged();
    SetChildPos();
}

void WarningsView::OnSizeChanged()
{
    wing::Control::OnSizeChanged();
    SetChildPos();
}

void WarningsView::SetChildPos()
{
    wing::Point loc;
    wing::Size size = GetSize();
    child->SetLocation(loc);
    child->SetSize(size);
}

void WarningsView::OnPaint(wing::PaintEventArgs& args)
{
    treeView->Root()->ExpandAll();
    treeView->Invalidate();
    child->Invalidate();
}

} // namespace cmajor::view
