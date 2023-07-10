// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>
#undef min
#undef max

module cmajor.view.locals.view;

namespace cmajor::view {

LocalsView::LocalsView() :
    wing::Control(wing::ControlCreateParams().WindowClassName("cmajor.view.LocalsView").WindowClassBackgroundColor(COLOR_WINDOW).BackgroundColor(wing::GetColor("locals.background")).
        SetDock(wing::Dock::fill)),
    localCount(-1), localCountRequested(false), childExtentRequested(false), fetchNode(nullptr), container(this), treeView(nullptr), child(nullptr)
{
    std::unique_ptr<wing::TreeView> treeViewPtr(new wing::TreeView(wing::TreeViewCreateParams().BackgroundColor(wing::GetColor("locals.background")).TextColor(wing::GetColor("locals.text")).
        SelectedNodeColor(wing::GetColor("locals.node.selected")).StateIndicatorColor(wing::GetColor("locals.state.indicator"))));
    treeView = treeViewPtr.get();
    treeView->SetBackgroundItemName("locals.background");
    treeView->SetTextItemName("locals.text");
    treeView->SetStateIndicatorItemName("locals.state.indicator");
    treeView->SetNodeSelectedItemName("locals.node.selected");
    treeView->SetFlag(wing::ControlFlags::scrollSubject);
    treeView->SetDoubleBuffered();
    treeView->NodeExpanded().AddHandler(this, &LocalsView::TreeViewNodeExpanded);
    treeView->NodeHovered().AddHandler(this, &LocalsView::TreeViewNodeHover);
    treeView->ContentLocationChanged().AddHandler(this, &LocalsView::TreeViewContentLocationChanged);
    std::unique_ptr<wing::Control> paddedTreeView(new wing::PaddedControl(wing::PaddedControlCreateParams(treeViewPtr.release()).Defaults()));
    std::unique_ptr<wing::Control> scrollableTreeView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(paddedTreeView.release()).SetDock(wing::Dock::fill)));
    child = scrollableTreeView.get();
    container.AddChild(scrollableTreeView.release());
    Clear();
}

void LocalsView::Clear()
{
    localCount = -1;
    localCountRequested = false;
    childExtentRequested = false;
    fetchNode = nullptr;
    treeView->SetRoot(new wing::TreeViewNode("Locals"));
    Update();
}

void LocalsView::SetLocalCount(int localCount_)
{
    wing::LogView* logView = wing::Application::GetLogView();
    if (localCount != localCount_)
    {
        localCount = localCount_;
        if (logView)
        {
            logView->WriteLine("setLocalCount=" + std::to_string(localCount));
        }
        childExtentRequested = false;
        fetchNode = nullptr;
        PopulateTreeViewNode(treeView->Root(), localCount);
    }
}

void LocalsView::SetFetchNode(wing::TreeViewNode* fetchNode_)
{
    wing::LogView* logView = wing::Application::GetLogView();
    childExtentRequested = false;
    fetchNode = fetchNode_;
    if (logView)
    {
        logView->WriteLine("setFetchNode=" + std::to_string(fetchNode->Index()));
    }
}

const ChildExtent& LocalsView::GetChildExtent()
{
    wing::LogView* logView = wing::Application::GetLogView();
    childExtentRequested = false;
    childExtent = ChildExtent();
    wing::TreeViewNode* parentNode = fetchNode;
    if (!parentNode)
    {
        parentNode = treeView->Root();
    }
    std::vector<wing::TreeViewNode*> visibleNodes;
    parentNode->GetVisibleNodes(visibleNodes, parentNode->Level() + 1);
    if (visibleNodes.empty()) return childExtent;
    wing::TreeViewNode* firstChild = static_cast<wing::TreeViewNode*>(parentNode->Children().FirstChild());
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
    int dataStart = first->Index();
    int start = first->Index() - firstChild->Index();
    int count = last->Index() - first->Index() + 1;
    if (count == 1 && first->Data() != nullptr)
    {
        return childExtent;
    }
    childExtent = ChildExtent(start, count, dataStart);
    if (logView)
    {
        logView->WriteLine("getChildExtent=" +
            std::to_string(parentNode->Index()) + ": [" + std::to_string(start) + ":" + std::to_string(first->Index()) + ", " + std::to_string(count) + ":" + std::to_string(last->Index()) + "]");
    }
    return childExtent;
}

std::string LocalsView::FetchExpression() const
{
    if (fetchNode)
    {
        db::ChildResult* childResult = static_cast<db::ChildResult*>(fetchNode->Data());
        if (childResult)
        {
            return childResult->expr;
        }
    }
    return "@locals";
}

void LocalsView::SetChildResults(const std::vector<db::ChildResult>& childResults)
{
    wing::LogView* logView = wing::Application::GetLogView();
    if (!childExtent.IsEmpty())
    {
        childResultVec.resize(childExtent.dataStart + std::max(childExtent.count, static_cast<int>(childResults.size())));
        for (int i = 0; i < childResults.size(); ++i)
        {
            childResultVec[childExtent.dataStart + i].reset(new db::ChildResult(childResults[i]));
        }
        int n = childResultVec.size();
        wing::TreeViewNode* parentNode = fetchNode;
        if (!parentNode)
        {
            parentNode = treeView->Root();
        }
        std::vector<wing::TreeViewNode*> visibleNodes;
        parentNode->GetVisibleNodes(visibleNodes, parentNode->Level() + 1);
        if (logView && !visibleNodes.empty())
        {
            logView->WriteLine("setChildResults=" + std::to_string(visibleNodes.front()->Index()) + "-" + std::to_string(visibleNodes.back()->Index()));
        }
        for (int i = 0; i < visibleNodes.size(); ++i)
        {
            wing::TreeViewNode* node = visibleNodes[i];
            int index = node->Index();
            if (index >= 0 && index < childResultVec.size())
            {
                db::ChildResult* childResult = childResultVec[index].get();
                node->SetData(childResult);
                std::string text = std::to_string(index);
                if (childResult)
                {
                    text = childResult->name;
                    if (!childResult->value.empty())
                    {
                        text.append(" = ").append(childResult->value);
                    }
                }
                node->SetText(text);
                PopulateTreeViewNode(node, n);
                if (childResult)
                {
                    n += childResult->count;
                }
            }
        }
    }
}

void LocalsView::UpdateFetchSet()
{
    std::vector<wing::TreeViewNode*> visibleNodes = treeView->GetVisibleNodes();
    for (wing::TreeViewNode* visibleNode : visibleNodes)
    {
        if (visibleNode != treeView->Root())
        {
            if (!visibleNode->Data())
            {
                fetchSet.insert(visibleNode->Parent());
            }
        }
    }
}

void LocalsView::Fetch()
{
    if (!fetchSet.empty())
    {
        wing::TreeViewNode* node = *fetchSet.begin();
        fetchSet.erase(node);
        SetFetchNode(node);
    }
}

void LocalsView::Update()
{
    treeView->SetChanged();
    treeView->Invalidate();
    child->Invalidate();
    treeView->Update();
}

void LocalsView::HideToolTipWindow()
{
    treeView->HideToolTipWindow();
}

void LocalsView::OnLocationChanged()
{
    wing::Control::OnLocationChanged();
    SetChildPos();
}

void LocalsView::OnSizeChanged()
{
    wing::Control::OnSizeChanged();
    SetChildPos();
}

void LocalsView::SetChildPos()
{
    child->SetLocation(wing::Point());
    child->SetSize(GetSize());
}

void LocalsView::PopulateTreeViewNode(wing::TreeViewNode* node, int n)
{
    if (!node->Children().IsEmpty()) return;
    if (node == treeView->Root())
    {
        for (int i = 0; i < n; ++i)
        {
            wing::TreeViewNode* node = new wing::TreeViewNode("local " + std::to_string(i) + "...");
            node->SetIndex(i);
            treeView->Root()->AddChild(node);
        }
        treeView->Root()->Expand();
    }
    else
    {
        db::ChildResult* result = static_cast<db::ChildResult*>(node->Data());
        if (result)
        {
            int count = result->count;
            for (int i = 0; i < count; ++i)
            {
                int index = n + i;
                wing::TreeViewNode* childNode = new wing::TreeViewNode("child node " + std::to_string(i) + "...");
                childNode->SetIndex(index);
                node->AddChild(childNode);
            }
        }
    }
}

void LocalsView::OnPaint(wing::PaintEventArgs& args)
{
    treeView->Invalidate();
    wing::Control::OnPaint(args);
}

void LocalsView::TreeViewNodeExpanded(wing::TreeViewNodeEventArgs& args)
{
    OnLocalNodeExpanded(args);
}

void LocalsView::TreeViewNodeHover(wing::TreeViewNodeEventArgs& args)
{
    OnLocalNodeHovered(args);
}

void LocalsView::TreeViewContentLocationChanged()
{
    OnUpdateNeeded();
}

void LocalsView::OnLocalNodeExpanded(wing::TreeViewNodeEventArgs& args)
{
    localNodeExpanded.Fire(args);
}

void LocalsView::OnLocalNodeHovered(wing::TreeViewNodeEventArgs& args)
{
    localNodeHovered.Fire(args);
}

void LocalsView::OnUpdateNeeded()
{
    updateNeeded.Fire();
}

} // namespace cmajor::view
