// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.resources;

import cmajor.symbols;
import cmajor.ast;
import std.core;

export namespace cmajor::build {

std::recursive_mutex& GetResourceLock();

void AddResources(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, std::vector<std::string>& objectFilePaths);

void GenerateRuntimeResourceFile(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule, 
    const std::string& classIndexFilePath, const std::string& traceDataFilePath);

void CompileResourceScriptFiles(cmajor::ast::Project* project, cmajor::symbols::Module* rootModule);

} // namespace cmajor::build
