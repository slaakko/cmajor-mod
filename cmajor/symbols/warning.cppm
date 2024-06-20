// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.warning;

import soul.ast.span;
import std.core;
import util.uuid;

export namespace cmajor::symbols {

class Module;

const int nodiscardWarning = 1;
const int fileNotUpToDateWarning = 2;
const int jsonAttributeWarning = 3;
const int nothrowWarning = 4;
const int maxWarningNumber = nothrowWarning + 1;

class Warning
{
public:
    Warning(int number_, const std::u32string& project_, const std::string& message_);
    int Number() const { return number; }
    const std::u32string& Project() const { return project; }
    const std::string& Message() const { return message; }
    const soul::ast::FullSpan& Defined() const { return defined; }
    void SetDefined(const soul::ast::FullSpan& defined_) { defined = defined_; }
    const std::vector<soul::ast::FullSpan>& References() const { return references; }
    void SetReferences(const std::vector<soul::ast::FullSpan>& references_);
private:
    int number;
    std::u32string project;
    std::string message;
    soul::ast::FullSpan defined;
    std::vector<soul::ast::FullSpan> references;
};

class CompileWarningCollection
{
public:
    CompileWarningCollection();
    void AddWarning(const Warning& warning);
    const std::vector<Warning>& Warnings() const { return warnings; }
    void Clear();
private:
    std::vector<Warning> warnings;
};

bool IsWarningDisabled(int warningNumber);
void DisableWarning(int warningNumber);
void ClearDisabledWarnings();
void LogWarning(int logStreamId, const Warning& warning);
void ClearGlobalWarningCollection();
void SetUseGlobalWarningCollection(bool use);
CompileWarningCollection& GetGlobalWarningCollection();

} // namespace cmajor::symbols

