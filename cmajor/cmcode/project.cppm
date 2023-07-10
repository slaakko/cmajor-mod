// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmcode.project;

import cmajor.service;
import cmajor.ast;
import std.core;

export namespace cmcode {

class ProjectData
{
public:
    ProjectData(cmajor::ast::Project* project_);
    void Load(const std::string& projectSettingsFilePath);
    void Save(const std::string& projectSettingsFilePath);
    bool Changed();
    cmajor::ast::Project* GetProject() const { return project; }
    cmajor::service::BreakpointCollection& GetBreakpointCollection() { return breakpointCollection; }
    const std::string& ProgramArguments() const { return programArguments; }
    void SetProgramArguments(const std::string& programArguments_);
private:
    bool changed;
    cmajor::ast::Project* project;
    cmajor::service::BreakpointCollection breakpointCollection;
    std::string programArguments;
};

} // namespace cmcode
