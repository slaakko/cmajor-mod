module;
#include <boost/uuid/uuid.hpp>

export module cmajor.symbols.warning;

// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================

/*
#include <cmajor/symbols/SymbolsApi.hpp>
#include <soulng/util/Json.hpp>
#include <soulng/lexer/soul::ast::SourcePos.hpp>
#include <boost/uuid/uuid.hpp>
*/
import soul.ast.source.pos;
import std.core;

export namespace cmajor::symbols {

class Module;

class Warning
{
public:
    Warning(const std::u32string& project_, const std::string& message_);
    const std::u32string& Project() const { return project; }
    const std::string& Message() const { return message; }
    const soul::ast::SourcePos& Defined() const { return defined; }
    void SetDefined(const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_) { defined = defined_; definedModuleId = definedModuleId_; }
    const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& References() const { return references; }
    void SetReferences(const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& references_);
    //std::unique_ptr<JsonValue> ToJson() const;
private:
    std::u32string project;
    std::string message;
    soul::ast::SourcePos defined;
    boost::uuids::uuid definedModuleId;
    std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>> references;
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

