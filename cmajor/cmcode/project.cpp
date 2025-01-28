// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmcode.project;

import cmcode.project.settings;
import soul.xml.dom;
import soul.xml.dom.parser;
import util;
import std.filesystem;

namespace cmcode {

ProjectData::ProjectData(cmajor::ast::Project* project_) : changed(false), project(project_), breakpointCollection(), programArguments()
{
}

void ProjectData::Load(const std::string& projectSettingsFilePath)
{
    if (std::filesystem::exists(projectSettingsFilePath))
    {
        std::unique_ptr<soul::xml::Document> projectSettingsDoc = soul::xml::ParseXmlFile(projectSettingsFilePath);
        ProjectSettings projectSettings;
        projectSettings.FromXml(projectSettingsDoc->DocumentElement());
        programArguments = projectSettings.programArguments;
        for (const ProjectBreakpoint& breakpoint : projectSettings.breakpoints)
        {
            cmajor::debugger::BreakpointList& breakpointList = breakpointCollection.GetBreakpointList(breakpoint.file);
            breakpointList.AddBreakpoint(new cmajor::debugger::Breakpoint(breakpoint.line, breakpoint.condition, breakpoint.disabled));
        }
        breakpointCollection.ResetChanged();
    }
    changed = false;
}

void ProjectData::Save(const std::string& projectSettingsFilePath)
{
    if (!Changed()) return;
    ProjectSettings projectSettings;
    projectSettings.programArguments = programArguments;
    for (auto& bm : breakpointCollection.BreakpointListMap())
    {
        cmajor::debugger::BreakpointList& breakpointList = bm.second;
        for (cmajor::debugger::Breakpoint* breakpoint : breakpointList.Breakpoints())
        {
            ProjectBreakpoint projectBreakpoint;
            projectBreakpoint.file = breakpointList.FilePath();
            projectBreakpoint.line = breakpoint->line;
            projectBreakpoint.condition = breakpoint->condition;
            projectBreakpoint.disabled = breakpoint->disabled;
            projectSettings.breakpoints.push_back(projectBreakpoint);
        }
    }
    soul::xml::Document projectSettingsDoc;
    std::unique_ptr<soul::xml::Element> projectSettingsElement(projectSettings.ToXml("projectSettings"));
    projectSettingsDoc.AppendChild(projectSettingsElement.release());
    std::ofstream projectSettingsFile(projectSettingsFilePath);
    util::CodeFormatter formatter(projectSettingsFile);
    projectSettingsDoc.Write(formatter);
    breakpointCollection.ResetChanged();
    changed = false;
}

bool ProjectData::Changed()
{
    if (breakpointCollection.Changed()) changed = true;
    return changed;
}

void ProjectData::SetProgramArguments(const std::string& programArguments_)
{
    if (programArguments != programArguments_)
    {
        programArguments = programArguments_;
        changed = true;
    }
}

} // namespace cmcode
