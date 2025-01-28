// =================================
// Copyright (c) 2025 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.parsing;

import std.core;
import cmajor.ast;
import soul.lexer;
import cmajor.build.flags;
import cmajor.symbols;

export namespace cmajor::build {

std::unique_ptr<cmajor::ast::CompileUnitNode> ParseSourceFile(int fileIndex, soul::lexer::FileMap& fileMap, Flags flags, cmajor::symbols::Module* module);

std::unique_ptr<cmajor::ast::Project> ParseProjectFile(const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend, int optLevel);

void ParseSourceFiles(cmajor::ast::Project* project, soul::lexer::FileMap& fileMap, Flags flags, cmajor::symbols::Module* module);

std::unique_ptr<cmajor::ast::Project> ParseProject(const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend, int optLevel,
    soul::lexer::FileMap& fileMap, Flags flags);

std::unique_ptr<cmajor::ast::Solution> ParseSolutionFile(const std::string& solutionFilePath);

std::unique_ptr<cmajor::ast::Solution> ParseSolution(const std::string& solutionFilePath, const std::string& config, cmajor::ast::BackEnd backend, int optLevel, Flags flags);

} // namespace cmajor::build
