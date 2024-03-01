// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.solution;

import cmajor.fault.tolerant.parser.project;
import cmajor.fault.tolerant.parser.token.value;
import cmajor.lexers;
import util;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::container::file::token;

soul::parser::Match ParseSolutionProjectDeclaration(ContainerFileLexer& lexer)
{
    if (*lexer == PROJECT)
    {
        ++lexer;
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SolutionProjectDeclaration> solutionProjectDeclaration;
    if (*lexer == FILEPATH)
    {
        std::string filePath = ParseFilePath(lexer.FileName(), lexer.GetToken(lexer.GetPos()));
        solutionProjectDeclaration.reset(new cmajor::fault::tolerant::ast::SolutionProjectDeclaration(filePath));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == SEMICOLON)
    {
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, solutionProjectDeclaration.release());
}

soul::parser::Match ParseActiveProjectDeclaration(ContainerFileLexer& lexer)
{
    if (*lexer == ACTIVEPROJECT)
    {
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::string activeProjectName;
    soul::parser::Match qualifiedIdMatch = ParseQualifiedId(lexer);
    if (qualifiedIdMatch.hit)
    {
        activeProjectName = *static_cast<std::string*>(qualifiedIdMatch.value);
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == SEMICOLON)
    {
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ActiveProjectDeclaration> activeProjectDeclaration;
    activeProjectDeclaration.reset(new cmajor::fault::tolerant::ast::ActiveProjectDeclaration(activeProjectName));
    return soul::parser::Match(true, activeProjectDeclaration.release());
}

soul::parser::Match ParseSolutionDeclaration(ContainerFileLexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match solutionProjectDeclarationMatch = ParseSolutionProjectDeclaration(lexer);
    if (solutionProjectDeclarationMatch.hit)
    {
        return solutionProjectDeclarationMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match activeProjectDeclarationMatch = ParseActiveProjectDeclaration(lexer);
    if (activeProjectDeclarationMatch.hit)
    {
        return  activeProjectDeclarationMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseSolution(ContainerFileLexer& lexer)
{
    if (*lexer == SOLUTION)
    {
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::string solutionName;
    soul::parser::Match qualifiedIdMatch = ParseQualifiedId(lexer);
    if (qualifiedIdMatch.hit)
    {
        solutionName = *static_cast<std::string*>(qualifiedIdMatch.value);
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == SEMICOLON)
    {
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::Solution> solution;
    solution.reset(new cmajor::fault::tolerant::ast::Solution(solutionName, lexer.FileName()));
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match declarationMatch = ParseSolutionDeclaration(lexer);
        if (declarationMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::SolutionDeclaration> declaration;
            declaration.reset(static_cast<cmajor::fault::tolerant::ast::SolutionDeclaration*>(declarationMatch.value));
            solution->AddDeclaration(declaration.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    solution->ResolveDeclarations();
    return soul::parser::Match(true, solution.release());
}

std::unique_ptr<cmajor::fault::tolerant::ast::Solution> ParseSolutionAst(ContainerFileLexer& lexer)
{
    ++lexer;
    std::unique_ptr<cmajor::fault::tolerant::ast::Solution> solution;
    soul::parser::Match solutionMatch = ParseSolution(lexer);
    if (solutionMatch.hit)
    {
        solution.reset(static_cast<cmajor::fault::tolerant::ast::Solution*>(solutionMatch.value));
    }
    else
    {
        return solution;
    }
    if (*lexer != soul::lexer::END_TOKEN)
    {
        solution->SetValid(false);
    }
    return solution;
}

std::unique_ptr<cmajor::fault::tolerant::ast::Solution> ParseSolution(const std::string& solutionFilePath)
{
    std::string solutionFile = util::ReadFile(solutionFilePath);
    std::u16string content = util::ToUtf16(solutionFile);
    ContainerFileLexer lexer = cmajor::container::file::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), solutionFilePath);
    std::unique_ptr<cmajor::fault::tolerant::ast::Solution> solution = ParseSolutionAst(lexer);
    return solution;
}

} // namespace cmajor::fault::tolerant::parser
