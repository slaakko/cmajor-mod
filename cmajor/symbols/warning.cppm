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

class Warning
{
public:
    Warning(const std::u32string& project_, const std::string& message_);
    const std::u32string& Project() const { return project; }
    const std::string& Message() const { return message; }
    const soul::ast::FullSpan& Defined() const { return defined; }
    void SetDefined(const soul::ast::FullSpan& defined_) { defined = defined_; }
    const std::vector<soul::ast::FullSpan>& References() const { return references; }
    void SetReferences(const std::vector<soul::ast::FullSpan>& references_);
private:
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
private:
    std::vector<Warning> warnings;
};

} // namespace cmajor::symbols

