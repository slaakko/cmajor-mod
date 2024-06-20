// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.symbols.warning;

import soul.ast.span;
import util;
import cmajor.symbols.modules;
import cmajor.symbols.module_cache;
import std.core;

namespace cmajor::symbols {

Warning::Warning(int number_, const std::u32string& project_, const std::string& message_) : number(number_), project(project_), message(message_)
{
}

void Warning::SetReferences(const std::vector<soul::ast::FullSpan>& references_)
{
    references = references_;
}

CompileWarningCollection::CompileWarningCollection()
{
}

void CompileWarningCollection::AddWarning(const Warning& warning)
{
    warnings.push_back(warning);
}

void CompileWarningCollection::Clear()
{
    warnings.clear();
}

std::set<int> disabledWarnings;

bool IsWarningDisabled(int warningNumber)
{
    if (disabledWarnings.find(warningNumber) != disabledWarnings.end()) return true;
    return false;
}

void DisableWarning(int warningNumber)
{
    disabledWarnings.insert(warningNumber);
}

void ClearDisabledWarnings()
{
    disabledWarnings.clear();
}

CompileWarningCollection globalWarningCollection;

bool useGlobalWarningCollection = false;

void LogWarning(int logStreamId, const Warning& warning)
{
    std::string warningText("warning " + std::to_string(warning.Number()) + ": " + warning.Message());
    soul::ast::FullSpan defined = warning.Defined();
    if (defined.IsValid())
    {
        Module* module = GetModuleById(defined.moduleId);
        if (module)
        {
            std::string fileName = module->GetFilePath(defined.fileIndex);
            if (!fileName.empty())
            {
                soul::ast::LineColLen lineColLen;
                std::string errorLines = module->GetErrorLines(defined.span, defined.fileIndex, lineColLen);
                warningText.append(" (file '" + fileName + "', line " + std::to_string(lineColLen.line) + ")");
                warningText.append(":\n").append(errorLines);
            }
        }
    }
    for (const soul::ast::FullSpan& referenceSpan : warning.References())
    {
        if (!referenceSpan.IsValid()) continue;
        if (referenceSpan == defined) continue;
        Module* module = GetModuleById(referenceSpan.moduleId);
        if (module)
        {
            std::string fileName = module->GetFilePath(referenceSpan.fileIndex);
            if (!fileName.empty())
            {
                soul::ast::LineColLen lineColLen;
                std::string errorLines = module->GetErrorLines(referenceSpan.span, referenceSpan.fileIndex, lineColLen);
                warningText.append("\nsee reference to file '" + fileName + "', line " + std::to_string(lineColLen.line));
                warningText.append(":\n").append(errorLines);
            }
        }
    }
    util::LogMessage(logStreamId, warningText);
    if (useGlobalWarningCollection)
    {
        globalWarningCollection.AddWarning(warning);
    }
}

void ClearGlobalWarningCollection()
{
    globalWarningCollection.Clear();
}

void SetUseGlobalWarningCollection(bool use)
{
    useGlobalWarningCollection = use;
}

CompileWarningCollection& GetGlobalWarningCollection()
{
    return globalWarningCollection;
}

} // namespace cmajor::symbols
