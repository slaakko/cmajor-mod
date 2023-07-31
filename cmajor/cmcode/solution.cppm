// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.solution;

import cmcode.project;
import cmajor.ast;
import cmajor.debuggers;
import wing;
import std.core;

export namespace cmcode {

class SolutionExplorer;

std::unique_ptr<cmajor::ast::Project> ReadProject(const std::string& projectFilePath, std::vector<std::unique_ptr<std::u32string>>& contents);
std::unique_ptr<cmajor::ast::Solution> ReadSolution(const std::string& solutionFilePath, std::vector<std::unique_ptr<std::u32string>>& contents);

enum class SolutionTreeViewNodeDataKind : int
{
    solution, project, file
};

struct SolutionTreeViewNodeData
{
    SolutionTreeViewNodeData(SolutionTreeViewNodeDataKind kind_, cmajor::ast::Solution* solution_, cmajor::ast::Project* project_, 
        const std::string& filePath_, const std::string& fileName_);
    SolutionTreeViewNodeDataKind kind;
    cmajor::ast::Solution* solution;
    cmajor::ast::Project* project;
    std::string filePath;
    std::string fileName;
    std::string key;
};

class SolutionData
{
public:
    SolutionData(std::unique_ptr<cmajor::ast::Solution>&& solution_, SolutionExplorer* solutionExplorer_);
    bool Changed();
    void Load(const std::string& solutionSettingsFilePath);
    void Save(const std::string& solutionSettingsFilePath);
    cmajor::ast::Solution* GetSolution() const { return solution.get(); }
    SolutionExplorer* GetSolutionExplorer() const { return solutionExplorer; }
    wing::TreeViewNode* ActiveProjectNode() const { return activeProjectNode; }
    void SetActiveProjectNode(wing::TreeViewNode* activeProjectNode_) { activeProjectNode = activeProjectNode_; }
    SolutionTreeViewNodeData* GetSolutionTreeViewNodeDataByKey(const std::string& key) const;
    ProjectData* GetProjectDataByProject(cmajor::ast::Project* project) const;
    void SetCallStackOpen(bool callStackOpen_);
    bool CallStackOpen() const { return callStackOpen; }
    void SetLocalsViewOpen(bool localsViewOpen_);
    bool LocalsViewOpen() const { return localsViewOpen; }
    void AddOpenFile(const std::string& filePath);
    void RemoveOpenFile(const std::string& filePath);
    const std::set<std::string>& OpenFiles() const { return openFiles; }
    const std::string& CurrentOpenFile() const { return currentOpenFile; }
    void SetCurrentOpenFile(const std::string& openFile);
    int CurrentCursorLine() const { return currentCursorLine; }
    void SetCurrentCursorLine(int line);
    void AddExpandedProject(const std::string& expandedProject);
    void RemoveExpandedProject(const std::string& project);
    const std::set<std::string>& ExpandedProjects() const { return expandedProjects; }
    cmajor::debugger::BreakpointCollection& GetSolutionBreakpointCollection() { return solutionBreakpointCollection; }
    void AddTreeViewNodeData(SolutionTreeViewNodeData* data);
    const std::vector<std::unique_ptr<ProjectData>>& Projects() const { return projectDataVec; }
    std::vector<cmajor::debugger::Breakpoint*> GetBreakpoints();
    wing::TreeViewNode* GetProjectNodeByName(const std::string& projectName) const;
    void RemoveProjectNode(const std::string& projectName);
private:
    void SetActiveProject();
    SolutionExplorer* solutionExplorer;
    std::unique_ptr<cmajor::ast::Solution> solution;
    std::vector<std::unique_ptr<SolutionTreeViewNodeData>> treeViewData;
    std::map<std::string, SolutionTreeViewNodeData*> treeViewDataMap;
    wing::TreeViewNode* activeProjectNode;
    std::vector<std::unique_ptr<ProjectData>> projectDataVec;
    std::unordered_map<cmajor::ast::Project*, ProjectData*> projectDataMap;
    std::map<std::string, wing::TreeViewNode*> projectNodeMap;
    bool changed;
    cmajor::debugger::BreakpointCollection solutionBreakpointCollection;
    bool callStackOpen;
    bool localsViewOpen;
    std::set<std::string> openFiles;
    std::string currentOpenFile;
    int currentCursorLine;
    std::set<std::string> expandedProjects;
};

} // namespace cmcode
