// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#undef min
#undef max

module cmajor.view.call.stack.view;

namespace cmajor::view {

FrameSelectedEventArgs::FrameSelectedEventArgs(cmajor::info::db::Location* frame_) : frame(frame_)
{
}

CallStackView::CallStackView() :
    Control(wing::ControlCreateParams().WindowClassName("cmajor.view.CallStackView").WindowClassBackgroundColor(COLOR_WINDOW).
        BackgroundColor(wing::GetColor("call.stack.background")).SetDock(wing::Dock::fill)),
    container(this), treeView(nullptr), child(nullptr), depth(-1)
{
    std::unique_ptr<wing::TreeView> treeViewPtr(new wing::TreeView(wing::TreeViewCreateParams().BackgroundColor(wing::GetColor("call.stack.background")).
        TextColor(wing::GetColor("call.stack.text")).
        SelectedNodeColor(wing::GetColor("call.stack.node.selected")).StateIndicatorColor(wing::GetColor("call.stack.state.indicator"))));
    treeView = treeViewPtr.get();
    treeView->SetBackgroundItemName("call.stack.background");
    treeView->SetTextItemName("call.stack.text");
    treeView->SetStateIndicatorItemName("call.stack.state.indicator");
    treeView->SetNodeSelectedItemName("call.stack.node.selected");
    treeView->SetFlag(wing::ControlFlags::scrollSubject);
    treeView->SetDoubleBuffered();
    treeView->NodeDoubleClick().AddHandler(this, &CallStackView::TreeViewNodeDoubleClick);
    std::unique_ptr<Control> paddedTreeView(new wing::PaddedControl(wing::PaddedControlCreateParams(treeViewPtr.release()).Defaults()));
    std::unique_ptr<Control> scrollableTreeView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(paddedTreeView.release()).SetDock(wing::Dock::fill)));
    child = scrollableTreeView.get();
    container.AddChild(scrollableTreeView.release());
    Clear();
}

void CallStackView::Clear()
{
    treeView->SetRoot(new wing::TreeViewNode("Frames"));
    depth = -1;
    frames.clear();
    frameRange = std::make_pair(-1, -1);
    child->Invalidate();
}

bool CallStackView::IsEmpty() const
{
    return depth == -1 || frameRange.first == frameRange.second;
}

void CallStackView::SetDepth(int depth_)
{
    if (depth != depth_)
    {
        depth = depth_;
        PopulateTreeView();
    }
}

void CallStackView::PopulateTreeView()
{
    for (int i = 0; i < depth; ++i)
    {
        std::string nodeText = std::to_string(i);
        wing::TreeViewNode* node = new wing::TreeViewNode(nodeText);
        node->SetIndex(i);
        treeView->Root()->AddChild(node);
    }
    treeView->Root()->Expand();
    treeView->Invalidate();
}

const std::pair<int, int>& CallStackView::GetFrameRange()
{
    std::vector<wing::TreeViewNode*> visibleNodes = treeView->GetVisibleNodes(1);
    if (visibleNodes.empty())
    {
        frameRange = std::make_pair(-1, -1);
        return frameRange;
    }
    wing::TreeViewNode* first = visibleNodes.front();
    wing::TreeViewNode* last = visibleNodes.back();
    while (first != last && first->Data() != nullptr)
    {
        first = static_cast<wing::TreeViewNode*>(first->NextSibling());
    }
    while (first != last && last->Data() != nullptr)
    {
        last = static_cast<wing::TreeViewNode*>(last->PrevSibling());
    }
    int lowFrame = first->Index();
    int highFrame = last->Index();
    frameRange = std::make_pair(lowFrame, highFrame);
    return frameRange;
}

void CallStackView::SetFrameRange(const std::vector<cmajor::info::db::Location>& frames_)
{
    if (frameRange.first == frameRange.second) return;
    frames.resize(std::max(frameRange.second + 1, static_cast<int>(frameRange.first + frames_.size())));
    for (int i = 0; i < frames_.size(); ++i)
    {
        frames[frameRange.first + i] = std::unique_ptr<cmajor::info::db::Location>(new cmajor::info::db::Location(frames_[i]));
    }
    UpdateNodes();
    treeView->SetChanged();
    treeView->Invalidate();
}

void CallStackView::UpdateNodes()
{
    std::vector<wing::TreeViewNode*> visibleNodes = treeView->GetVisibleNodes(1);
    if (visibleNodes.empty()) return;
    wing::TreeViewNode* first = visibleNodes.front();
    wing::TreeViewNode* last = visibleNodes.back();
    int firstIndex = first->Index();
    int lastIndex = last->Index();
    wing::TreeViewNode* node = first;
    for (int i = firstIndex; i <= lastIndex; ++i)
    {
        if (node && i >= 0 && i < frames.size())
        {
            node->SetData(frames[i].get());
            UpdateNode(node);
            node = static_cast<wing::TreeViewNode*>(node->NextSibling());
        }
    }
}

void CallStackView::UpdateNode(wing::TreeViewNode* node)
{
    cmajor::info::db::Location* location = static_cast<cmajor::info::db::Location*>(node->Data());
    if (location)
    {
        std::string func = location->func;
        if (func.empty())
        {
            func = "<external>";
        }
        std::string nodeText = std::to_string(node->Index());
        nodeText.append(" - ").append(func);
        node->SetText(nodeText);
        node->RemoveChildren();
        if (!location->func.empty())
        {
            wing::TreeViewNode* fileNode = new wing::TreeViewNode(location->file);
            node->AddChild(fileNode);
            std::string lineText;
            lineText.append("Line ").append(std::to_string(location->line));
            wing::TreeViewNode* lineNode = new wing::TreeViewNode(lineText);
            node->AddChild(lineNode);
            if (!location->addr.empty())
            {
                std::string addrText;
                addrText.append("Address ").append(location->addr);
                wing::TreeViewNode* addrNode = new wing::TreeViewNode(addrText);
                node->AddChild(addrNode);
            }
        }
    }
}

void CallStackView::TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args)
{
    wing::TreeViewNode* node = args.node;
    if (node->Data())
    {
        cmajor::info::db::Location* frame = static_cast<cmajor::info::db::Location*>(node->Data());
        FrameSelectedEventArgs args(frame);
        OnFrameSelected(args);
    }
}

void CallStackView::OnFrameSelected(FrameSelectedEventArgs& args)
{
    frameSelected.Fire(args);
}

void CallStackView::OnLocationChanged()
{
    Control::OnLocationChanged();
    SetChildPos();
}

void CallStackView::OnSizeChanged()
{
    Control::OnSizeChanged();
    SetChildPos();
}

void CallStackView::SetChildPos()
{
    child->SetLocation(wing::Point());
    child->SetSize(GetSize());
}

void CallStackView::OnPaint(wing::PaintEventArgs& args)
{
    treeView->Invalidate();
    Control::OnPaint(args);
}

} // namespace cmajor::view
