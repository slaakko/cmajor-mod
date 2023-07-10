// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.view.search.results.view;

import wing;
import std.core;

export namespace cmajor::view {

struct SearchResult
{
    SearchResult(const std::string& fileName_, int lineNumber_, const std::string& line_);
    std::string fileName;
    int lineNumber;
    std::string line;
};

struct ViewSearchResultEventArgs
{
    ViewSearchResultEventArgs(SearchResult* searchResult_);
    SearchResult* searchResult;
};

using ViewSearchResultEvent = wing::EventWithArgs<ViewSearchResultEventArgs>;

class SearchResultsView : public wing::Control
{
public:
    SearchResultsView();
    ViewSearchResultEvent& ViewSearchResult() { return viewSearchResult; }
    void Clear();
    void Add(SearchResult* searchResult);
protected:
    void OnLocationChanged() override;
    void OnSizeChanged() override;
    void OnPaint(wing::PaintEventArgs& args) override;
    virtual void OnViewSearchResult(ViewSearchResultEventArgs& args);
private:
    void TreeViewNodeDoubleClick(wing::TreeViewNodeClickEventArgs& args);
    void SetChildPos();
    wing::Container container;
    wing::TreeView* treeView;
    wing::Control* child;
    std::vector<std::unique_ptr<SearchResult>> searchResults;
    ViewSearchResultEvent viewSearchResult;
};

} // namespace cmajor::view
