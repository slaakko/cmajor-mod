// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

export module cmajor.build.parsing;

import std.core;
import cmajor.ast;
import soul.lexer;

export namespace cmajor::build {

enum class Flags
{
    none = 0, 
    verbose = 1 << 0,
    singleThreaded = 1 << 1,
    ast = 1 << 2,
    print = 1 << 3
};

constexpr Flags operator|(Flags left, Flags right)
{
    return Flags(int(left) | int(right));
}

constexpr Flags operator&(Flags left, Flags right)
{
    return Flags(int(left) & int(right));
}

constexpr Flags operator~(Flags flags)
{
    return Flags(~int(flags));
}

std::unique_ptr<cmajor::ast::CompileUnitNode> ParseSourceFile(Flags flags, int fileIndex, soul::lexer::FileMap& fileMap);
std::unique_ptr<cmajor::ast::Project> ParseProjectFile(const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend, const std::string& toolChain);
std::unique_ptr<cmajor::ast::Project> ParseProject(Flags flags, const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend,
    const std::string& toolChain, soul::lexer::FileMap& fileMap);
std::unique_ptr<cmajor::ast::Solution> ParseSolutionFile(const std::string& solutionFilePath);
std::unique_ptr<cmajor::ast::Solution> ParseSolution(Flags flags, const std::string& solutionFilePath, const std::string& config, cmajor::ast::BackEnd backend,
    const std::string& toolChain);

} // namespace cmajor::build
