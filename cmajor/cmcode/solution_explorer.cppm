// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.solution.explorer;

import cmajor.view;
import cmajor.ast;
import wing;
import std.core;

export namespace cmcode {

class MainWindow;

wing::Color DefaultSolutionExplorerBackgroundColor();

struct SolutionExplorerCreateParams
{
    SolutionExplorerCreateParams();
    SolutionExplorerCreateParams& Defaults();
    SolutionExplorerCreateParams& WindowClassName(const std::string& windowClassName_);
    SolutionExplorerCreateParams& WindowClassStyle(uint32_t windowClassStyle_);
    SolutionExplorerCreateParams& WindowStyle(int windowStyle_);
    SolutionExplorerCreateParams& WindowClassBackgroundColor(int windowClassBackgroundColor_);
    SolutionExplorerCreateParams& BackgroundColor(const wing::Color& backgroundColor_);
    SolutionExplorerCreateParams& Text(const std::string& text_);
    SolutionExplorerCreateParams& Location(wing::Point location_);
    SolutionExplorerCreateParams& SetSize(wing::Size size_);
    SolutionExplorerCreateParams& SetAnchors(wing::Anchors anchors_);
    SolutionExplorerCreateParams& SetDock(wing::Dock dock_);
    wing::ControlCreateParams controlCreateParams;
    wing::TreeViewCreateParams treeViewCreateParams;
};

class SolutionExplorer : public wing::ContainerControl
{
public:
    SolutionExplorer(SolutionExplorerCreateParams& createParams, MainWindow* mainWindow_);
    wing::TreeView* GetTreeView() const { return solutionTreeView; }
    void MakeView();
    void SetRoot(wing::TreeViewNode* solutionNode);
    void SetImageList(wing::ImageList* imageList_);
    wing::ImageList* GetImageList() const { return imageList; }
protected:
    void OnPaint(wing::PaintEventArgs& args) override;
private:
    wing::TreeViewCreateParams treeViewCreateParams;
    MainWindow* mainWindow;
    wing::TreeView* solutionTreeView;
    wing::Control* child;
    wing::ImageList* imageList;
};

} // namespace cmcode
