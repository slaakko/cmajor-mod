// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.masm.build.vs_project_file;

import cmajor.ast;
import cmajor.symbols;
import std.core;

export namespace cmajor::masm::build {

std::string MakeVSProjectFile(cmajor::ast::Project* project, cmajor::symbols::Module* module, const std::vector<std::string> asmFilePaths, 
    const std::vector<std::string> cppFilePaths, const std::vector<std::string>& resourceScriptFiles, 
    const std::string& classIndexFilePath, const std::string& traceDataFilePath, bool verbose);

} // namespace cmajor::masm::build
