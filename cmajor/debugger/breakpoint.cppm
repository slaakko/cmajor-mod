// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.debugger.breakpoint;

import cmajor.info;
import std.core;

export namespace cmajor::debugger {

class BreakpointList;

struct Breakpoint
{
    Breakpoint(int line_);
    Breakpoint(int line_, const std::string& condition_);
    Breakpoint(int line_, const std::string& condition_, bool disabled_);
    int LocationOrLine() const { return location.line != 0 ? location.line : line; }
    int line;
    std::string condition;
    bool disabled;
    cmajor::info::db::Location location;
    BreakpointList* list;
    std::vector<std::string> ids;
};

class BreakpointCollection;

class BreakpointList
{
public:
    BreakpointList();
    ~BreakpointList();
    void SetBreakpointCollection(BreakpointCollection* breakpointCollection_) { breakpointCollection = breakpointCollection_; }
    bool IsEmpty() const { return breakpoints.empty(); }
    const std::string& FilePath() const { return filePath; }
    void SetFilePath(const std::string& filePath_);
    const std::vector<Breakpoint*>& Breakpoints() const { return breakpoints; }
    std::vector<Breakpoint*>& Breakpoints() { return breakpoints; }
    void Clear();
    void RemoveBreakpoint(Breakpoint* breakpoint);
    Breakpoint* GetBreakpoint(int line) const;
    void AddBreakpoint(Breakpoint* breakpoint);
private:
    BreakpointCollection* breakpointCollection;
    std::string filePath;
    std::vector<Breakpoint*> breakpoints;
};

class BreakpointCollection
{
public:
    BreakpointCollection();
    std::map<std::string, BreakpointList>& BreakpointListMap() { return breakpointListMap; }
    BreakpointList& GetBreakpointList(const std::string& filePath);
    void Clear();
    bool Changed() const { return changed; }
    void SetChanged() { changed = true; }
    void ResetChanged() { changed = false; }
private:
    std::map<std::string, BreakpointList> breakpointListMap;
    bool changed;
};

} // namespace cmajor::debugger
