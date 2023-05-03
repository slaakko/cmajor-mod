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
    singleThreaded = 1 << 0
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

std::unique_ptr<cmajor::ast::CompileUnitNode> ParseSourceFile(int fileIndex, soul::lexer::FileMap& fileMap);
std::unique_ptr<cmajor::ast::Project> ParseProjectFile(const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend, const std::string& toolChain);
std::unique_ptr<cmajor::ast::Project> ParseProject(Flags flags, const std::string& projectFilePath, const std::string& config, cmajor::ast::BackEnd backend,
    const std::string& toolChain, soul::lexer::FileMap& fileMap);
std::unique_ptr<cmajor::ast::Solution> ParseSolutionFile(const std::string& solutionFilePath);

} // namespace cmajor::build
