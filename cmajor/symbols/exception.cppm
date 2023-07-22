// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.exception;

import cmajor.command;
import std.core;
import soul.ast.source.pos;
import soul.xml.dom;
import util;

export namespace cmajor::symbols {

class Module;

std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId);
std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& primarySourcePos, const util::uuid& primaryModuleId, const soul::ast::SourcePos& referenceSourcePos, const util::uuid& referenceModuleId);
std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& primarySourcePos, const util::uuid& primaryModuleId, const soul::ast::SourcePos& referenceSourcePos, const util::uuid& referenceModuleId,
    const std::string& title);
std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references);
std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& sourcePos, const util::uuid& moduleId, const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references,
    const std::string& title);

std::unique_ptr<util::JsonObject> SourcePosToJson(Module* module, const soul::ast::SourcePos& sourcePos);
std::unique_ptr<soul::xml::Element> SourcePosToDomElement(Module* module, const soul::ast::SourcePos& sourcePos);

class Exception
{
public:
    Exception(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_);
    Exception(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_, const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_);
    Exception(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_, const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_);
    virtual ~Exception();
    const std::string& What() const { return what; }
    const std::string& Message() const { return message; }
    const soul::ast::SourcePos& Defined() const { return defined; }
    const util::uuid& DefinedModuleId() const { return definedModuleId; }
    const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& References() const { return references; }
    void SetProject(const std::string& projectName_);
    const std::string& ProjectName() const { return projectName; }
    std::vector<cmajor::command::CompileError> ToErrors() const;
private:
    std::string what;
    std::string message;
    std::string projectName;
    soul::ast::SourcePos defined;
    util::uuid definedModuleId;
    std::vector<std::pair<soul::ast::SourcePos, util::uuid>> references;
};

class ModuleImmutableException : public Exception
{
public:
    ModuleImmutableException(Module* module_, Module* immutableModule, const soul::ast::SourcePos& defined_, const soul::ast::SourcePos& referenced_);
};

class SymbolCheckException : public Exception
{
public:
    SymbolCheckException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& moduleId_);
};

class CastOverloadException : public Exception
{
public:
    CastOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_);
    CastOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_, const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_);
    CastOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_, const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_);
};

class CannotBindConstToNonconstOverloadException : public Exception
{
public:
    CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_);
    CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
        const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_);
    CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
        const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_);
};

class CannotAssignToConstOverloadException : public Exception
{
public:
    CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_);
    CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
        const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_);
    CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_,
        const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_);
};

class NoViableFunctionException : public Exception
{
public:
    NoViableFunctionException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_);
    NoViableFunctionException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_, const soul::ast::SourcePos& referenced_, const util::uuid& referencedModuleId_);
    NoViableFunctionException(const std::string& message_, const soul::ast::SourcePos& defined_, const util::uuid& definedModuleId_, const std::vector<std::pair<soul::ast::SourcePos, util::uuid>>& references_);
};

} // namespace cmajor::symbols
