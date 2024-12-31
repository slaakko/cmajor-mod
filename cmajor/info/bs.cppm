// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.info.bs;

import std.core;

export namespace cmajor::info::bs {

struct BuildCommand
{
    BuildCommand();
    std::string backend;
    std::string config;
    std::string optimizationLevel;
    std::string disabledWarnings;
    std::string filePath;
    std::vector<std::string> defines;
    bool verbose;
    bool quiet;
    bool clean;
    bool rebuild;
    bool emitIR;
    bool disableModuleCache;
    bool singleThreadedCompile;
    bool buildAllDependencies;
};

struct CompileError
{
    CompileError();
    std::string message;
    std::string project;
    std::string file;
    int line;
    int scol;
    int ecol;
};

struct Warning
{
    Warning();
    int number;
    std::string message;
    std::string project;
    std::string file;
    int line;
    int scol;
    int ecol;
};

struct BuildResult
{
    BuildResult();
    bool success;
    std::vector<CompileError> errors;
    std::vector<Warning> warnings;
};

struct DefinitionSourceLocation
{
    DefinitionSourceLocation();
    std::string file;
    int line;
    int scol;
};

struct GetDefinitionRequest
{
    GetDefinitionRequest();
    std::string projectName;
    std::string projectFilePath;
    std::string backend;
    std::string config;
    std::string optimizationLevel;
    std::string identifier;
    DefinitionSourceLocation identifierLocation;
};

struct GetDefinitionReply
{
    GetDefinitionReply();
    DefinitionSourceLocation definitionLocation;
    bool succeeded;
    std::string error;
};

} // namespace cmajor::info::bs
