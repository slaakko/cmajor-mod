// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.build.parsing;

import cmajor.lexers;
import cmajor.parsers;
import util;

namespace cmajor::build {

std::unique_ptr<cmajor::ast::CompileUnitNode> ParseSourceFile(const std::string& sourceFilePath, soul::lexer::FileMap& fileMap) 
{
    std::string content = util::ReadFile(sourceFilePath);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = cmajor::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), sourceFilePath);
    int32_t file = fileMap.MapFile(sourceFilePath);
    lexer.SetFile(file);
    using LexerType = decltype(lexer);
    cmajor::parser::context::Context context;
    std::unique_ptr<cmajor::ast::CompileUnitNode> compileUnit = cmajor::compile::unit::parser::CompileUnitParser<LexerType>::Parse(lexer, &context);
    fileMap.AddFileContent(file, std::move(ucontent), lexer.GetLineStartIndeces());
    return compileUnit;
}

std::unique_ptr<cmajor::ast::Project> ParseProjectFile(const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend, const std::string& toolChain)
{
    std::string content = util::ReadFile(projectFilePath);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = cmajor::container::file::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), projectFilePath);
    using LexerType = decltype(lexer);
    std::unique_ptr<cmajor::ast::Project> project = cmajor::projects::parser::ProjectParser<LexerType>::Parse(lexer, config, backend, toolChain);
    project->ResolveDeclarations();
    return project;
}

std::unique_ptr<cmajor::ast::Solution> ParseSolutionFile(const std::string& solutionFilePath)
{
    std::string content = util::ReadFile(solutionFilePath);
    std::u32string ucontent = util::ToUtf32(content);
    auto lexer = cmajor::container::file::lexer::MakeLexer(ucontent.c_str(), ucontent.c_str() + ucontent.length(), solutionFilePath);
    using LexerType = decltype(lexer);
    std::unique_ptr<cmajor::ast::Solution> solution = cmajor::solutions::parser::SolutionParser<LexerType>::Parse(lexer);
    solution->ResolveDeclarations();
    return solution;
}

} // namespace cmajor::build

