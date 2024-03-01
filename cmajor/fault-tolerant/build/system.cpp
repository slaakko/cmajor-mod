// =================================
// Copyright (c) 2024 Seppo Laakko
// Distributed under the MIT license
// =================================

module cmajor.fault.tolerant.build.system;

import cmajor.fault.tolerant.build.compilation;
import cmajor.fault.tolerant.ast;
import cmajor.fault.tolerant.parser;

namespace cmajor::fault::tolerant::build {

std::string SystemSolutionPath()
{
    std::string systemSolutionPath = util::Path::Combine(util::Path::Combine(util::Path::Combine(util::Path::Combine(util::GetFullPath(util::CmajorRoot()), "system"), "platform"), 
        "windows"), "System.cms");
    return systemSolutionPath;
}

void BuildSystem()
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Solution> solution = cmajor::fault::tolerant::parser::ParseSolution(SystemSolutionPath());
    for (const auto& projectFilePath : solution->ProjectFilePaths())
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::Project> project = cmajor::fault::tolerant::parser::ParseProject(
            projectFilePath, 
            "debug", 
            cmajor::fault::tolerant::ast::Backend::llvm);
        solution->AddProject(project.release());
    }
    std::vector<cmajor::fault::tolerant::ast::Project*> buildOrder = solution->CreateBuildOrder();
    CompilationTable::Instance().Clear();
    std::vector<Compilation*> compilations;
    for (auto childProject : buildOrder)
    {
        int index = childProject->Index();
        std::unique_ptr<cmajor::fault::tolerant::ast::Project> project = solution->ReleaseProject(index);
        Compilation* compilation = new Compilation(project.release());
        CompilationTable::Instance().AddCompilation(compilation);
        compilations.push_back(compilation);
    }
    for (auto compilation : compilations)
    {
        compilation->CreateSymbolTable(true);
        compilation->ImportSymbolTables();
        compilation->BuildProject();
    }
}

void ReadSystem()
{
    std::unique_ptr<cmajor::fault::tolerant::ast::Solution> solution = cmajor::fault::tolerant::parser::ParseSolution(SystemSolutionPath());
    for (const auto& projectFilePath : solution->ProjectFilePaths())
    {
        std::unique_ptr<cmajor::fault::tolerant::ast::Project> project = cmajor::fault::tolerant::parser::ParseProject(
            projectFilePath,
            "debug",
            cmajor::fault::tolerant::ast::Backend::llvm);
        solution->AddProject(project.release());
    }
    std::vector<cmajor::fault::tolerant::ast::Project*> buildOrder = solution->CreateBuildOrder();
    CompilationTable::Instance().Clear();
    std::vector<Compilation*> compilations;
    for (auto childProject : buildOrder)
    {
        int index = childProject->Index();
        std::unique_ptr<cmajor::fault::tolerant::ast::Project> project = solution->ReleaseProject(index);
        Compilation* compilation = new Compilation(project.release());
        CompilationTable::Instance().AddCompilation(compilation);
        compilations.push_back(compilation);
    }
    for (auto compilation : compilations)
    {
        compilation->CreateSymbolTable(false);
        compilation->ImportSymbolTables();
        compilation->Read();
    }
}

} // namespace cmajor::fault::tolerant::build
