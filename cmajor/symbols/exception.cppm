// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.symbols.exception;

import cmajor.info;
import std.core;
import soul.ast.span;
import soul.xml.dom;
import util;

export namespace cmajor::symbols {

class Module;

std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& fullSpan);
std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& primarySpan, const soul::ast::FullSpan& referenceSpan);
std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& primarySpan, const soul::ast::FullSpan& referenceSpan, const std::string& title);
std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& fullSpan, const std::vector<soul::ast::FullSpan>& references);
std::string Expand(const std::string& errorMessage, const soul::ast::FullSpan& fullSpan, const std::vector<soul::ast::FullSpan>& references, const std::string& title);

class Exception
{
public:
    Exception(const std::string& message_, const soul::ast::FullSpan& defined_);
    Exception(const std::string& message_, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_);
    Exception(const std::string& message_, const soul::ast::FullSpan& defined_, const std::vector<soul::ast::FullSpan>& references_);
    virtual ~Exception();
    const std::string& What() const { return what; }
    const std::string& Message() const { return message; }
    const soul::ast::FullSpan& Defined() const { return defined; }
    const std::vector<soul::ast::FullSpan>& References() const { return references; }
    void SetProject(const std::string& projectName_);
    const std::string& ProjectName() const { return projectName; }
    std::vector<cmajor::info::bs::CompileError> ToErrors() const;
private:
    std::string what;
    std::string message;
    std::string projectName;
    soul::ast::FullSpan defined;
    std::vector<soul::ast::FullSpan> references;
};

class ModuleImmutableException : public Exception
{
public:
    ModuleImmutableException(Module* module_, Module* immutableModule, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_);
};

class SymbolCheckException : public Exception
{
public:
    SymbolCheckException(const std::string& message_, const soul::ast::FullSpan& defined_);
};

class CastOverloadException : public Exception
{
public:
    CastOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_);
    CastOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_);
    CastOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, const std::vector<soul::ast::FullSpan>& references_);
};

class CannotBindConstToNonconstOverloadException : public Exception
{
public:
    CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_);
    CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_);
    CannotBindConstToNonconstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, const std::vector<soul::ast::FullSpan>& references_);
};

class CannotAssignToConstOverloadException : public Exception
{
public:
    CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_);
    CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_);
    CannotAssignToConstOverloadException(const std::string& message_, const soul::ast::FullSpan& defined_, const std::vector<soul::ast::FullSpan>& references_);
};

class NoViableFunctionException : public Exception
{
public:
    NoViableFunctionException(const std::string& message_, const soul::ast::FullSpan& defined_);
    NoViableFunctionException(const std::string& message_, const soul::ast::FullSpan& defined_, const soul::ast::FullSpan& referenced_);
    NoViableFunctionException(const std::string& message_, const soul::ast::FullSpan& defined_, const std::vector<soul::ast::FullSpan>& references_);
};

} // namespace cmajor::symbols
