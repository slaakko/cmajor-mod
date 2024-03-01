// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module cmcode.solution.explorer;

import cmcode.solution;
import cmcode.main.window;
import wing;
import util;

namespace cmcode {

wing::Color DefaultSolutionExplorerBackgroundColor()
{
    return wing::Color::White;
}

wing::Padding SolutionExplorerNodeImagePadding()
{
    return wing::Padding(2, 2, 2, 2);
}

SolutionExplorerCreateParams::SolutionExplorerCreateParams() : controlCreateParams()
{
    controlCreateParams.WindowClassName("cmajor.cmcode.solution_explorer");
    controlCreateParams.WindowClassBackgroundColor(COLOR_WINDOW);
    controlCreateParams.BackgroundColor(DefaultSolutionExplorerBackgroundColor());
    controlCreateParams.WindowStyle(wing::DefaultChildWindowStyle());
    treeViewCreateParams = wing::TreeViewCreateParams().StateIndicatorColor(wing::GetColor("solution.explorer.state.indicator")).SelectedNodeColor(wing::GetColor("solution.explorer.node.selected")).
        BackgroundColor(wing::GetColor("solution.explorer.background")).TextColor(wing::GetColor("solution.explorer.text")).
        NodeIndentPercent(100).NodeImagePadding(SolutionExplorerNodeImagePadding());
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::Defaults()
{
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::WindowClassName(const std::string& windowClassName_)
{
    controlCreateParams.WindowClassName(windowClassName_);
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::WindowClassStyle(uint32_t windowClassStyle_)
{
    controlCreateParams.WindowClassStyle(windowClassStyle_);
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::WindowStyle(int windowStyle_)
{
    controlCreateParams.WindowStyle(windowStyle_);
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::WindowClassBackgroundColor(int windowClassBackgroundColor_)
{
    controlCreateParams.WindowClassBackgroundColor(windowClassBackgroundColor_);
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::BackgroundColor(const wing::Color& backgroundColor_)
{
    controlCreateParams.BackgroundColor(backgroundColor_);
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::Text(const std::string& text_)
{
    controlCreateParams.Text(text_);
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::Location(wing::Point location_)
{
    controlCreateParams.Location(location_);
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::SetSize(wing::Size size_)
{
    controlCreateParams.SetSize(size_);
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::SetAnchors(wing::Anchors anchors_)
{
    controlCreateParams.SetAnchors(anchors_);
    return *this;
}

SolutionExplorerCreateParams& SolutionExplorerCreateParams::SetDock(wing::Dock dock_)
{
    controlCreateParams.SetDock(dock_);
    return *this;
}

SolutionExplorer::SolutionExplorer(SolutionExplorerCreateParams& createParams, MainWindow* mainWindow_) :
    ContainerControl(createParams.controlCreateParams), mainWindow(mainWindow_), solutionTreeView(nullptr), child(nullptr), imageList(nullptr),
    treeViewCreateParams(createParams.treeViewCreateParams)
{
    MakeView();
}

void SolutionExplorer::SetRoot(wing::TreeViewNode* solutionNode)
{
    MakeView();
    solutionTreeView->SetImageList(imageList);
    solutionTreeView->SetRoot(solutionNode);
}

void SolutionExplorer::SetImageList(wing::ImageList* imageList_)
{
    imageList = imageList_;
}

void SolutionExplorer::OnPaint(wing::PaintEventArgs& args)
{
    try
    {
        if (solutionTreeView)
        {
            solutionTreeView->Invalidate();
        }
        if (child)
        {
            child->Invalidate();
        }
        wing::Control::OnPaint(args);
    }
    catch (const std::exception& ex)
    {
        wing::ShowErrorMessageBox(Handle(), ex.what());
    }
}

void SolutionExplorer::MakeView()
{
    if (child)
    {
        RemoveChild(child);
        child = nullptr;
        solutionTreeView = nullptr;
    }
    std::unique_ptr<wing::TreeView> solutionTreeViewPtr(new wing::TreeView(treeViewCreateParams));
    solutionTreeView = solutionTreeViewPtr.get();
    solutionTreeView->SetFlag(wing::ControlFlags::scrollSubject);
    solutionTreeView->NodeDoubleClick().AddHandler(mainWindow, &MainWindow::TreeViewNodeDoubleClick);
    solutionTreeView->NodeClick().AddHandler(mainWindow, &MainWindow::TreeViewNodeClick);
    solutionTreeView->NodeExpanded().AddHandler(mainWindow, &MainWindow::TreeViewNodeExpanded);
    solutionTreeView->NodeCollapsed().AddHandler(mainWindow, &MainWindow::TreeViewNodeCollapsed);
    solutionTreeView->SetDoubleBuffered();
    solutionTreeView->SetBackgroundItemName("solution.explorer.background");
    solutionTreeView->SetTextItemName("solution.explorer.text");
    solutionTreeView->SetStateIndicatorItemName("solution.explorer.state.indicator");
    solutionTreeView->SetNodeSelectedItemName("solution.explorer.node.selected");
    std::unique_ptr<wing::PaddedControl> paddedTreeViewPtr(new wing::PaddedControl(wing::PaddedControlCreateParams(solutionTreeViewPtr.release()).Defaults()));
    std::unique_ptr<wing::ScrollableControl> scrollableTreeViewPtr(new wing::ScrollableControl(wing::ScrollableControlCreateParams(paddedTreeViewPtr.release()).SetDock(wing::Dock::fill)));
    child = scrollableTreeViewPtr.get();
    AddChild(scrollableTreeViewPtr.release());
}

} // namespace cmcode
