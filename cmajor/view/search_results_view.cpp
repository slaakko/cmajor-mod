// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module;
#include <Windows.h>

module cmajor.view.search.results.view;

namespace cmajor::view {

SearchResult::SearchResult(const std::string& fileName_, int lineNumber_, const std::string& line_) : fileName(fileName_), lineNumber(lineNumber_), line(line_)
{
}

ViewSearchResultEventArgs::ViewSearchResultEventArgs(SearchResult* searchResult_) : searchResult(searchResult_)
{
}

SearchResultsView::SearchResultsView() :
    wing::Control(wing::ControlCreateParams().WindowClassName("cmajor.view.SearchResultsView").WindowClassBackgroundColor(COLOR_WINDOW).
        BackgroundColor(wing::GetColor("search.results.background")).SetDock(wing::Dock::fill)),
    container(this), treeView(nullptr), child(nullptr)
{
    std::unique_ptr<wing::TreeView> treeViewPtr(new wing::TreeView(wing::TreeViewCreateParams().BackgroundColor(wing::GetColor("search.results.background")).
        TextColor(wing::GetColor("search.results.text")).
        SelectedNodeColor(wing::GetColor("search.results.node.selected")).StateIndicatorColor(wing::GetColor("search.results.state.indicator"))));
    treeView = treeViewPtr.get();
    treeView->SetBackgroundItemName("search.results.background");
    treeView->SetTextItemName("search.results.text");
    treeView->SetStateIndicatorItemName("search.results.state.indicator");
    treeView->SetNodeSelectedItemName("search.results.node.selected");
    treeView->SetFlag(wing::ControlFlags::scrollSubject);
    treeView->SetDoubleBuffered();
    treeView->NodeDoubleClick().AddHandler(this, &SearchResultsView::TreeViewNodeDoubleClick);
    std::unique_ptr<wing::Control> paddedTreeView(new wing::PaddedControl(wing::PaddedControlCreateParams(treeViewPtr.release()).Defaults()));
    std::unique_ptr<wing::Control> scrollableTreeView(new wing::ScrollableControl(wing::ScrollableControlCreateParams(paddedTreeView.release()).SetDock(wing::Dock::fill)));
    child = scrollableTreeView.get();
    container.AddChild(scrollableTreeView.release());
    Clear();
}

void SearchResultsView::Clear()
{
    treeView->SetRoot(new wing::TreeViewNode("Search Results"));
    child->Invalidate();
}

void SearchResultsView::Add(SearchResult* searchResult)
{
    searchResults.push_back(std::unique_ptr<SearchResult>(searchResult));
    std::string text = searchResult->fileName;
    text.append(1, ':').append(std::to_string(searchResult->lineNumber));
    wing::TreeViewNode* resultNode = new wing::TreeViewNode(text);
    resultNode->SetData(searchResult);
    resultNode->AddChild(new wing::TreeViewNode(searchResult->line));
    treeView->Root()->AddChild(resultNode);
}

void SearchResultsView::TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args)
{
    SearchResult* searchResult = static_cast<SearchResult*>(args.node->Data());
    ViewSearchResultEventArgs viewSearchResultArgs(searchResult);
    OnViewSearchResult(viewSearchResultArgs);
}

void SearchResultsView::SetChildPos()
{
    wing::Point loc;
    wing::Size size = GetSize();
    child->SetLocation(loc);
    child->SetSize(size);
}

void SearchResultsView::OnLocationChanged()
{
    wing::Control::OnLocationChanged();
    SetChildPos();
}

void SearchResultsView::OnSizeChanged()
{
    wing::Control::OnSizeChanged();
    SetChildPos();
}

void SearchResultsView::OnPaint(wing::PaintEventArgs& args)
{
    treeView->Root()->ExpandAll();
    treeView->Invalidate();
    child->Invalidate();
    wing::Control::OnPaint(args);
}

void SearchResultsView::OnViewSearchResult(ViewSearchResultEventArgs& args)
{
    viewSearchResult.Fire(args);
}

} // namespace cmajor::view
