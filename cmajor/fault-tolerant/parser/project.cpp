// =================================
// Copyright (c) 2023 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.parser.project;

import cmajor.fault.tolerant.parser.token.value;
import cmajor.lexers;
import util;

namespace cmajor::fault::tolerant::parser {

using namespace cmajor::container::file::token;

soul::parser::Match ParseQualifiedId(ContainerFileLexer& lexer)
{
    std::unique_ptr<std::string> name;
    name.reset(new std::string());
    if (*lexer == ID)
    {
        const auto& token = lexer.GetToken(lexer.GetPos());
        name->append(util::ToUtf8(token.ToString()));
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    while (true)
    {
        int64_t save = lexer.GetPos();
        if (*lexer == DOT)
        {
            name->append(1, '.');
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
        if (*lexer == ID)
        {
            const auto& token = lexer.GetToken(lexer.GetPos());
            name->append(util::ToUtf8(token.ToString()));
            ++lexer;
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    return soul::parser::Match(true, name.release());
}

soul::parser::Match ParseReferenceDeclaration(ContainerFileLexer& lexer)
{
    if (*lexer == REFERENCE)
    {
        ++lexer;
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ReferenceDeclaration> referenceDeclaration;
    if (*lexer == FILEPATH)
    {
        std::string filePath = ParseFilePath(lexer.FileName(), lexer.GetToken(lexer.GetPos()));
        referenceDeclaration.reset(new cmajor::fault::tolerant::ast::ReferenceDeclaration(filePath));
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
    return soul::parser::Match(true, referenceDeclaration.release());
}

soul::parser::Match ParseSourceFileDeclaration(ContainerFileLexer& lexer)
{
    if (*lexer == SOURCE)
    {
        ++lexer;
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::SourceFileDeclaration> sourceFileDeclaration;
    if (*lexer == FILEPATH)
    {
        std::string filePath = ParseFilePath(lexer.FileName(), lexer.GetToken(lexer.GetPos()));
        sourceFileDeclaration.reset(new cmajor::fault::tolerant::ast::SourceFileDeclaration(filePath));
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
    return soul::parser::Match(true, sourceFileDeclaration.release());
}

soul::parser::Match ParseResourceFileDeclaration(ContainerFileLexer& lexer)
{
    if (*lexer == RESOURCE)
    {
        ++lexer;
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ResourceFileDeclaration> resourceFileDeclaration;
    if (*lexer == FILEPATH)
    {
        std::string filePath = ParseFilePath(lexer.FileName(), lexer.GetToken(lexer.GetPos()));
        resourceFileDeclaration.reset(new cmajor::fault::tolerant::ast::ResourceFileDeclaration(filePath));
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
    return soul::parser::Match(true, resourceFileDeclaration.release());
}

soul::parser::Match ParseTextFileDeclaration(ContainerFileLexer& lexer)
{
    if (*lexer == TEXT)
    {
        ++lexer;
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::TextFileDeclaration> textFileDeclaration;
    if (*lexer == FILEPATH)
    {
        std::string filePath = ParseFilePath(lexer.FileName(), lexer.GetToken(lexer.GetPos()));
        textFileDeclaration.reset(new cmajor::fault::tolerant::ast::TextFileDeclaration(filePath));
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
    return soul::parser::Match(true, textFileDeclaration.release());
}

soul::parser::Match ParseActionFileDeclaration(ContainerFileLexer& lexer)
{
    if (*lexer == ACTION)
    {
        ++lexer;
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::ActionFileDeclaration> actionFileDeclaration;
    if (*lexer == FILEPATH)
    {
        std::string filePath = ParseFilePath(lexer.FileName(), lexer.GetToken(lexer.GetPos()));
        actionFileDeclaration.reset(new cmajor::fault::tolerant::ast::ActionFileDeclaration(filePath));
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
    return soul::parser::Match(true, actionFileDeclaration.release());
}

cmajor::fault::tolerant::ast::Target ParseTarget(ContainerFileLexer& lexer)
{
    switch (*lexer)
    {
        case PROGRAM:
        {
            ++lexer;
            return cmajor::fault::tolerant::ast::Target::program;
        }
        case WINGUIAPP:
        {
            ++lexer;
            return cmajor::fault::tolerant::ast::Target::winguiapp;
        }
        case WINAPP:
        {
            ++lexer;
            return cmajor::fault::tolerant::ast::Target::winapp;
        }
        case LIBRARY:
        {
            ++lexer;
            return cmajor::fault::tolerant::ast::Target::library;
        }
        case WINLIB:
        {
            ++lexer;
            return cmajor::fault::tolerant::ast::Target::winlib;
        }
        case UNITTEST:
        {
            ++lexer;
            return cmajor::fault::tolerant::ast::Target::unitTest;
        }
    }
    return cmajor::fault::tolerant::ast::Target::program;
}

soul::parser::Match ParseTargetDeclaration(ContainerFileLexer& lexer)
{
    if (*lexer == TARGET)
    {
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    if (*lexer == ASSIGN)
    {
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<cmajor::fault::tolerant::ast::TargetDeclaration> targetDeclaration;
    cmajor::fault::tolerant::ast::Target target = ParseTarget(lexer);
    targetDeclaration.reset(new cmajor::fault::tolerant::ast::TargetDeclaration(target));
    if (*lexer == SEMICOLON)
    {
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    return soul::parser::Match(true, targetDeclaration.release());
}

soul::parser::Match ParseProjectDeclaration(ContainerFileLexer& lexer)
{
    int64_t save = lexer.GetPos();
    soul::parser::Match referenceDeclarationMatch = ParseReferenceDeclaration(lexer);
    if (referenceDeclarationMatch.hit)
    {
        return referenceDeclarationMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match sourceFileDeclarationMatch = ParseSourceFileDeclaration(lexer);
    if (sourceFileDeclarationMatch.hit)
    {
        return sourceFileDeclarationMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match resourceFileDeclarationMatch = ParseResourceFileDeclaration(lexer);
    if (resourceFileDeclarationMatch.hit)
    {
        return resourceFileDeclarationMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match textFileDeclarationMatch = ParseTextFileDeclaration(lexer);
    if (textFileDeclarationMatch.hit)
    {
        return textFileDeclarationMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match actionFileDeclarationMatch = ParseActionFileDeclaration(lexer);
    if (actionFileDeclarationMatch.hit)
    {
        return actionFileDeclarationMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    soul::parser::Match targetDeclarationMatch = ParseTargetDeclaration(lexer);
    if (targetDeclarationMatch.hit)
    {
        return targetDeclarationMatch;
    }
    else
    {
        lexer.SetPos(save);
    }
    return soul::parser::Match(false);
}

soul::parser::Match ParseProject(ContainerFileLexer& lexer, const std::string& config, cmajor::fault::tolerant::ast::Backend backend)
{
    if (*lexer == PROJECT)
    {
        ++lexer;
    }
    else
    {
        return soul::parser::Match(false);
    }
    std::unique_ptr<std::string> name;
    soul::parser::Match qualifiedIdMatch = ParseQualifiedId(lexer);
    if (qualifiedIdMatch.hit)
    {
        name.reset(static_cast<std::string*>(qualifiedIdMatch.value));
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
    std::unique_ptr<cmajor::fault::tolerant::ast::Project> project;
    project.reset(new cmajor::fault::tolerant::ast::Project(*name, lexer.FileName(), config, backend));
    while (true)
    {
        int64_t save = lexer.GetPos();
        soul::parser::Match declarationMatch = ParseProjectDeclaration(lexer);
        if (declarationMatch.hit)
        {
            std::unique_ptr<cmajor::fault::tolerant::ast::ProjectDeclaration> declaration;
            declaration.reset(static_cast<cmajor::fault::tolerant::ast::ProjectDeclaration*>(declarationMatch.value));
            project->AddDeclaration(declaration.release());
        }
        else
        {
            lexer.SetPos(save);
            break;
        }
    }
    project->ResolveDeclarations();
    return soul::parser::Match(true, project.release());
}

std::unique_ptr<cmajor::fault::tolerant::ast::Project> ParseProjectAst(ContainerFileLexer& lexer, const std::string& config, cmajor::fault::tolerant::ast::Backend backend)
{
    ++lexer;
    std::unique_ptr<cmajor::fault::tolerant::ast::Project> project;
    soul::parser::Match projectMatch = ParseProject(lexer, config, backend);
    if (projectMatch.hit)
    {
        project.reset(static_cast<cmajor::fault::tolerant::ast::Project*>(projectMatch.value));
    }
    else
    {
        return project;
    }
    if (*lexer != soul::lexer::END_TOKEN)
    {
        project->SetValid(false);
    }
    return project;
}

std::unique_ptr<cmajor::fault::tolerant::ast::Project> ParseProject(const std::string& projectFilePath, const std::string& config, cmajor::fault::tolerant::ast::Backend backend)
{
    std::string projectFile = util::ReadFile(projectFilePath);
    std::u16string content = util::ToUtf16(projectFile);
    ContainerFileLexer lexer = cmajor::container::file::lexer::MakeLexer(content.c_str(), content.c_str() + content.length(), projectFilePath);
    std::unique_ptr<cmajor::fault::tolerant::ast::Project> project = ParseProjectAst(lexer, config, backend);
    return project;
}

} // namespace cmajor::fault::tolerant::parser
