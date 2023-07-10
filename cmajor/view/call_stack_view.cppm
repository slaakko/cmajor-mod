// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.call.stack.view;

import cmajor.debug.message;
import wing;
import std.core;

export namespace cmajor::view {

struct FrameSelectedEventArgs
{
    FrameSelectedEventArgs(db::Location* frame_);
    db::Location* frame;
};

using FrameSelectedEvent = wing::EventWithArgs<FrameSelectedEventArgs>;

class CallStackView : public wing::Control
{
public:
    CallStackView();
    void Clear();
    bool IsEmpty() const;
    int Depth() const { return depth; }
    void SetDepth(int depth_);
    const std::pair<int, int>& GetFrameRange();
    void SetFrameRange(const std::vector<db::Location>& frames_);
    FrameSelectedEvent& FrameSelected() { return frameSelected; }
protected:
    void OnLocationChanged() override;
    void OnSizeChanged() override;
    void OnPaint(wing::PaintEventArgs& args) override;
    virtual void OnFrameSelected(FrameSelectedEventArgs& args);
private:
    void SetChildPos();
    void PopulateTreeView();
    void UpdateNodes();
    void UpdateNode(wing::TreeViewNode* node);
    void TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args);
    wing::Container container;
    wing::TreeView* treeView;
    wing::Control* child;
    int depth;
    std::pair<int, int> frameRange;
    std::vector<std::unique_ptr<db::Location>> frames;
    FrameSelectedEvent frameSelected;
};

} // namespace cmajor::view
