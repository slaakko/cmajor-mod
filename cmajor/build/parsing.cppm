// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.parsing;

import std.core;
import cmajor.ast;
import soul.lexer;

export namespace cmajor::build {

std::unique_ptr<cmajor::ast::CompileUnitNode> ParseSourceFile(const std::string& sourceFilePath, soul::lexer::FileMap& fileMap);
std::unique_ptr<cmajor::ast::Project> ParseProjectFile(const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend, const std::string& toolChain);
std::unique_ptr<cmajor::ast::Solution> ParseSolutionFile(const std::string& solutionFilePath);

} // namespace cmajor::build
