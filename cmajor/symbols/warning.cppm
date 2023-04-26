export module cmajor.symbols.warning;

// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

import soul.ast.source.pos;
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
    const soul::ast::SourcePos& Defined() const { return defined; }
    void SetDefined(const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_) { defined = defined_; definedModuleId = definedModuleId_; }
    const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& References() const { return references; }
    void SetReferences(const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_);
    //std::unique_ptr<JsonValue> ToJson() const;
private:
    std::u32string project;
    std::string message;
    soul::ast::SourcePos defined;
    util::uuid definedModuleId;
    std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
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

