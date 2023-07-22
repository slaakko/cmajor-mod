// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.command;

import std.core;

export namespace cmajor::command {

struct BuildCommand
{
    BuildCommand();
    std::string backend;
    std::string config;
    std::string optimizationLevel;
    std::string filePath;
    std::vector<std::string> defines;
    bool verbose;
    bool quiet;
    bool clean;
    bool rebuild;
    bool emitIR;
    bool linkWithDebugRuntime;
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

struct BuildResult
{
    BuildResult();
    bool success;
    std::vector<CompileError> errors;
};

} // cmajor::command
