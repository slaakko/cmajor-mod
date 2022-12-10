// =================================
// Copyright (c) 2022 Seppo Laakko
// Distributed under the MIT license
// =================================
module;
#include <boost/uuid/uuid.hpp>

export module cmajor.symbols.exception;

import std.core;
import soul.ast.source.pos;
import soul.xml.dom;
import util;
/*
#include <sngxml/dom/Element.hpp>
*/

export namespace cmajor::symbols {

class Module;

std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId);
std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& primarySourcePos, const boost::uuids::uuid& primaryModuleId, const soul::ast::SourcePos& referenceSourcePos, const boost::uuids::uuid& referenceModuleId);
std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& primarySourcePos, const boost::uuids::uuid& primaryModuleId, const soul::ast::SourcePos& referenceSourcePos, const boost::uuids::uuid& referenceModuleId,
    const std::string& title);
std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId, const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& references);
std::string Expand(const std::string& errorMessage, const soul::ast::SourcePos& sourcePos, const boost::uuids::uuid& moduleId, const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& references,
    const std::string& title);

std::unique_ptr<util::JsonObject> SourcePosToJson(Module* module, const soul::ast::SourcePos& sourcePos);
std::unique_ptr<soul::xml::Element> SourcePosToDomElement(Module* module, const soul::ast::SourcePos& sourcePos);

class Exception
{
public:
    Exception(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_);
    Exception(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_, const soul::ast::SourcePos& referenced_, const boost::uuids::uuid& referencedModuleId_);
    Exception(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_, const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& references_);
    virtual ~Exception();
    const std::string& What() const { return what; }
    const std::string& Message() const { return message; }
    const soul::ast::SourcePos& Defined() const { return defined; }
    const boost::uuids::uuid& DefinedModuleId() const { return definedModuleId; }
    const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& References() const { return references; }
    std::unique_ptr<util::JsonValue> ToJson() const;
    void AddToDiagnosticsElement(soul::xml::Element* diagnosticsElement) const;
private:
    std::string what;
    std::string message;
    soul::ast::SourcePos defined;
    boost::uuids::uuid definedModuleId;
    std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>> references;
};

class ModuleImmutableException : public Exception
{
public:
    ModuleImmutableException(Module* module_, Module* immutableModule, const soul::ast::SourcePos& defined_, const soul::ast::SourcePos& referenced_);
};

class SymbolCheckException : public Exception
{
public:
    SymbolCheckException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& moduleId_);
};

class CastOverloadException : public Exception
{
public:
    CastOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_);
    CastOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_, const soul::ast::SourcePos& referenced_, const boost::uuids::uuid& referencedModuleId_);
    CastOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_, const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& references_);
};

class CannotBindConstToNonconstOverloadException : public Exception
{
public:
    CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_);
    CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_,
        const soul::ast::SourcePos& referenced_, const boost::uuids::uuid& referencedModuleId_);
    CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_,
        const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& references_);
};

class CannotAssignToConstOverloadException : public Exception
{
public:
    CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_);
    CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_,
        const soul::ast::SourcePos& referenced_, const boost::uuids::uuid& referencedModuleId_);
    CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_,
        const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& references_);
};

class NoViableFunctionException : public Exception
{
public:
    NoViableFunctionException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_);
    NoViableFunctionException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_, const soul::ast::SourcePos& referenced_, const boost::uuids::uuid& referencedModuleId_);
    NoViableFunctionException(const std::string& message_, const soul::ast::SourcePos& defined_, const boost::uuids::uuid& definedModuleId_, const std::vector<std::pair<soul::ast::SourcePos, boost::uuids::uuid>>& references_);
};

} // namespace cmajor::symbols
