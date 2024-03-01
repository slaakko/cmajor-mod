// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.locals.view;

import cmajor.info.db;
import wing;

export namespace cmajor::view {

using LocalNodeExpandedEvent = wing::EventWithArgs<wing::TreeViewNodeEventArgs>;
using LocalNodeHoverEvent = wing::EventWithArgs<wing::TreeViewNodeEventArgs>;
using LocalsViewUpdateNeededEvent = wing::Event;

struct ChildExtent
{
    ChildExtent() : start(-1), count(-1), dataStart(-1) {}
    ChildExtent(int start_, int count_, int dataStart_) : start(start_), count(count_), dataStart(dataStart_) {}
    bool IsEmpty() const { return start == -1 && count == -1 && dataStart == -1; }
    int start;
    int count;
    int dataStart;
};

class LocalsView : public wing::Control
{
public:
    LocalsView();
    void Clear();
    int LocalCount() const { return localCount; }
    bool LocalCountRequested() const { return localCountRequested; }
    void SetLocalCountRequested() { localCountRequested = true; }
    void SetLocalCount(int localCount_);
    wing::TreeViewNode* FetchNode() const { return fetchNode; }
    void SetFetchNode(wing::TreeViewNode* fetchNode_);
    const ChildExtent& GetChildExtent();
    void SetChildExtentRequested() { childExtentRequested = true; }
    void ResetChildExtentRequested() { childExtentRequested = false; }
    bool ChildExtentRequested() const { return childExtentRequested; }
    std::string FetchExpression() const;
    void SetChildResults(const std::vector<cmajor::info::db::ChildResult>& childResults);
    bool IsFetchSetEmpty() const { return fetchSet.empty(); }
    void UpdateFetchSet();
    void Fetch();
    void Update();
    void HideToolTipWindow();
    LocalNodeExpandedEvent& LocalNodeExpanded() { return localNodeExpanded; }
    LocalNodeHoverEvent& LocalNodeHovered() { return localNodeHovered; }
    LocalsViewUpdateNeededEvent& UpdateNeeded() { return updateNeeded; }
protected:
    void OnLocationChanged() override;
    void OnSizeChanged() override;
    void OnPaint(wing::PaintEventArgs& args) override;
    virtual void OnLocalNodeExpanded(wing::TreeViewNodeEventArgs& args);
    virtual void OnLocalNodeHovered(wing::TreeViewNodeEventArgs& args);
    virtual void OnUpdateNeeded();
private:
    void SetChildPos();
    void PopulateTreeViewNode(wing::TreeViewNode* node, int n);
    void TreeViewNodeExpanded(wing::TreeViewNodeEventArgs& args);
    void TreeViewNodeHover(wing::TreeViewNodeEventArgs& args);
    void TreeViewContentLocationChanged();
    int localCount;
    bool localCountRequested;
    ChildExtent childExtent;
    bool childExtentRequested;
    wing::TreeViewNode* fetchNode;
    std::set<wing::TreeViewNode*> fetchSet;
    std::vector<std::unique_ptr<cmajor::info::db::ChildResult>> childResultVec;
    wing::Container container;
    wing::TreeView* treeView;
    wing::Control* child;
    LocalNodeExpandedEvent localNodeExpanded;
    LocalNodeHoverEvent localNodeHovered;
    LocalsViewUpdateNeededEvent updateNeeded;
};

} // namespace cmajor::view
