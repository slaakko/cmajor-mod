// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

import util;
import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.parser;
import cmajor.fault.tolerant.symbols;
import soul.ast.span;
import std.core;

void ParseProject(const std::string& filePath, bool verbose)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Project> project = cmajor::fault::tolerant::parser::ParseProject(filePath, "debug", cmajor::fault::tolerant::ast::Backend::llvm);
    cmajor::fault::tolerant::symbols::SymbolTable symbolTable;
    symbolTable.SetName(project->Name());
    for (const auto& sourceFilePath : project->SourceFilePaths())
    {
        if (verbose)
        {
            std::cout << ">> " << sourceFilePath << "\n";
        }
        std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> compileUnit = cmajor::fault::tolerant::parser::ParseCmajorCompileUnit(sourceFilePath);
        cmajor::fault::tolerant::symbols::Source* source = new cmajor::fault::tolerant::symbols::Source(compileUnit->FilePath());
        symbolTable.AddSource(source);
        cmajor::fault::tolerant::symbols::SymbolCreatorVisitor visitor(&symbolTable, source);
        compileUnit->Accept(visitor);
        source->SetLineStarts(compileUnit->LineStarts());
        project->AddCompileUnit(compileUnit.release());
    }
    std::string symbolFilePath = project->SymbolFilePath();
    {
        cmajor::fault::tolerant::symbols::SymbolWriter writer(symbolFilePath);
        symbolTable.Write(writer);
    }
    {
        cmajor::fault::tolerant::symbols::SymbolTable symbolTableRead;
        cmajor::fault::tolerant::symbols::SymbolReader reader(symbolFilePath);
        symbolTableRead.Read(reader);
    }
    for (const auto& compileUnit : project->CompileUnits())
    {
        std::vector<cmajor::fault::tolerant::symbols::TokenInfo> tokenInfo = cmajor::fault::tolerant::symbols::GetTokenInfo(compileUnit.get(), &symbolTable);
    }
}

void ParseSolution(const std::string& filePath, bool verbose)
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Solution> solution = cmajor::fault::tolerant::parser::ParseSolution(filePath);
    for (const auto& projectFilePath : solution->ProjectFilePaths())
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::Project> project = cmajor::fault::tolerant::parser::ParseProject(projectFilePath, "debug", cmajor::fault::tolerant::ast::Backend::llvm);
        solution->AddProject(project.release());
    }
    std::vector<cmajor::fault::tolerant::ast::Project*> buildOrder = solution->CreateBuildOrder();
    for (auto project : buildOrder)
    {
        cmajor::fault::tolerant::symbols::SymbolTable symbolTable;
        symbolTable.SetName(project->Name());
        if (verbose)
        {
            std::cout << ">> " << project->FilePath() << "\n";
        }
        for (const auto& sourceFilePath : project->SourceFilePaths())
        {
            if (verbose)
            {
                std::cout << ">>> " << sourceFilePath << "\n";
            }
            std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> compileUnit = cmajor::fault::tolerant::parser::ParseCmajorCompileUnit(sourceFilePath);
            cmajor::fault::tolerant::symbols::Source* source = new cmajor::fault::tolerant::symbols::Source(compileUnit->FilePath());
            symbolTable.AddSource(source);
            cmajor::fault::tolerant::symbols::SymbolCreatorVisitor visitor(&symbolTable, source);
            compileUnit->Accept(visitor);
            source->SetLineStarts(compileUnit->LineStarts());
            project->AddCompileUnit(compileUnit.release());
        }
        for (const auto& compileUnit : project->CompileUnits())
        {
            std::vector<cmajor::fault::tolerant::symbols::TokenInfo> tokenInfo = cmajor::fault::tolerant::symbols::GetTokenInfo(compileUnit.get(), &symbolTable);
        }
    }
}

int main(int argc, const char** argv)
{
    try
    {
        util::Init();

        std::vector<std::string> files;
        bool verbose = false;
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.starts_with("--"))
            {
                if (arg == "--verbose")
                {
                    verbose = true;
                }
                else
                {
                    throw std::runtime_error("unknown option '" + arg + "'");
                }
            }
            else if (arg.starts_with("-"))
            {
                std::string options = arg.substr(1);
                for (char o : options)
                {
                    switch (o)
                    {
                        case 'v':
                        {
                            verbose = true;
                            break;
                        }
                        default:
                        {
                            throw std::runtime_error("unknown option '-" + std::string(1, o) + "'");
                        }
                    }
                }
            }
            else
            {
                files.push_back(util::GetFullPath(arg));
            }
        }
        for (const auto& filePath : files)
        {
            if (verbose)
            {
                std::cout << "> " << filePath << "\n";
            }
            if (filePath.ends_with(".cm"))
            {
                std::unique_ptr<cmajor::fault::tolerant::ast::CompileUnitNode> compileUnit = cmajor::fault::tolerant::parser::ParseCmajorCompileUnit(filePath);
            }
            else if (filePath.ends_with(".cmp"))
            {
                ParseProject(filePath, verbose);
            }
            else if (filePath.ends_with(".cms"))
            {
                ParseSolution(filePath, verbose);
            }
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    util::Done();
    return 0;
}